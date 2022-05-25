#include "ArucoVision.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include "MarkerData.h"
#include <opencv2/imgproc.hpp>

//#include <WinUser.h>
#include <Windows.h>

namespace Aruco {
   
    
    const float calibrationSquareDimension = 0.01905f;
    const float arucoSquareDimension = 0.1016f;
    const cv::Size chessboardDimension = cv::Size(7,7);
    inline static bool readCameraParameters(std::string filename, cv::Mat& camMatrix, cv::Mat& distCoeffs) {
        cv::FileStorage fs(filename, cv::FileStorage::READ);
        if (!fs.isOpened())
            return false;
        fs["camera_matrix"] >> camMatrix;
        fs["distortion_coefficients"] >> distCoeffs;
        fs.release();
        return true;
    }
    ArucoVision::ArucoVision() {}
    
    ArucoVision::ArucoVision(cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryType) {
        dictionary = cv::aruco::getPredefinedDictionary(dictionaryType);
        calibrateCamera();
    }

    ArucoVision::ArucoVision(std::string cameraParamPath,
        cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryType) {
        dictionary = cv::aruco::getPredefinedDictionary(dictionaryType);
        std::cout << "Checking if camera parameters exist" << std::endl;
        if (readCameraParameters(cameraParamPath, cameraMatrix, distCoeffs)) {
            std::cout << "Loaded camera parameters succesfully" << std::endl;
            //ask if user still wants to calibrate camera.
            cv::Mat calFrame(100, 400, CV_8UC3, cv::Scalar(0, 0, 0));


            cv::putText(calFrame, "Press y to recalibrate\n press n to not.", cv::Point(50, 50),
                cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 255, 255));

            cv::imshow("Camera Calibration", calFrame);
            bool shouldCalibrate = false;
            while (true) {
                cv::imshow("Camera Calibration", calFrame);
                int key = cv::waitKey(1) % 256;
                if (key == 'y') {
                    //calibrate
                    shouldCalibrate = true;
                    break;
                }   
                else if (key == 'n') {
                    //dont calibrate
                    break;
                }
                
            }
            cv::destroyWindow("Camera Calibration");
            if (shouldCalibrate) { 
                std::cout << "Starting calibration!" << std::endl;
                calibrateCamera(); 
            }

        }
        else {
            std::cout << "Failed loading camera parameters, starting calibration!" << std::endl;
            calibrateCamera();
        }
    }

    void getChessboardCorners(std::vector<cv::Mat> images, std::vector<std::vector<cv::Point2f>>& allFoundCorners, bool showResults = false) {
        printf("Checking %d images", images.size());
        int i = 0;
        for (std::vector<cv::Mat>::iterator it = images.begin(); it != images.end(); it++) {
            std::vector<cv::Point2f> pointBuf;
            printf("Looking in image: %d", i);
            bool found = cv::findChessboardCorners(*it, chessboardDimension, pointBuf, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

            if (found) {
                allFoundCorners.push_back(pointBuf);
            }

            if (showResults) {
                cv::drawChessboardCorners(*it, chessboardDimension, pointBuf, found);
                cv::imshow("Looking for corners", *it);
                cv::waitKey(1);
            }
            i++;
        }
    }

    void createKnownBoardPosition(cv::Size boardSize, float squareEdgeLength, std::vector<cv::Point3f>& corners) {
        for (int i = 0; i < boardSize.height; i++) {
            for (int j = 0; j < boardSize.width; j++) {
                corners.push_back(cv::Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
            }
        }
    }

    void cameraCalibration(std::vector<cv::Mat> calibrationImages, cv::Size boardSize, float squareEdgeLength, cv::Mat& _cameraMatrix, cv::Mat& distanceCoefficients) {
        std::vector<std::vector<cv::Point2f>> checkerboardImageSpacePoints;
        getChessboardCorners(calibrationImages, checkerboardImageSpacePoints, false);

        std::vector<std::vector<cv::Point3f>> worldSpaceCornerPoints(1);
        createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
        worldSpaceCornerPoints.resize(checkerboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

        std::vector<cv::Mat> rVectors, tVectors;
        distanceCoefficients = cv::Mat::zeros(8, 1, CV_64F);
        cv::calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoints, boardSize, _cameraMatrix, distanceCoefficients, rVectors, tVectors);
        std::cout << "done calibrating" << std::endl;
    }

    void ArucoVision::calibrateCamera() {
       
        std::vector<int> ids;
        std::vector<int> markerCount;
        cv::Size size;
        //cv::Ptr<cv::aruco::Board> board = cv::aruco::Board::create(corners, dictionary, ids);
        cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        
        std::vector<cv::Mat> savedImages;
        //cv::namedWindow("Camera Calibration", 400);
        
        cv::VideoCapture camera;
        camera.open(0);
        cv::Mat camFrame;
        cv::Mat calFrame(100,400, CV_8UC3, cv::Scalar(0,0,0));
        
        
        cv::putText(calFrame, "Press space to make picture", cv::Point(50, 50),
            cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 255, 255));
        
        cv::imshow("Camera Calibration", calFrame);
        bool calibrated = false;
        while (!calibrated) {
            //check if space has been checked.
            cv::imshow("Camera Calibration", calFrame);

            if (camera.grab()) {
                camera.retrieve(camFrame);
                cv::imshow("Camera", camFrame);
                
                int key = cv::waitKey(1) % 256;
                
                switch (key) {
                case ' ':
                    cameraCalibration(savedImages, chessboardDimension, calibrationSquareDimension, cameraMatrix, distCoeffs);
                    calibrated = true;
                    break;
                case 'p':
                    savedImages.push_back(camFrame);
                    std::cout << std::endl << "Captured frame" << std::endl;
                    break;
                }

            }

            cv::waitKey(1);
        }
        
        std::vector<cv::Mat> rvecs, tvecs;
        
        cv::FileStorage fs("Resources/cam_params.yml", cv::FileStorage::WRITE);
        if (fs.isOpened()) {
            fs << "camera_matrix" << cameraMatrix;
            fs << "distortion_coefficients" << distCoeffs;
            fs.release();
        }

//        cv::destroyWindow("Camera Calibration live");
        cv::destroyWindow("Camera Calibration");
        cv::destroyWindow("Camera");
    }

    SimpleMarkerData ArucoVision::detectMarkers(cv::InputArray image) {
        Aruco::SimpleMarkerData markerdata;
        cv::Mat undistorted;
        image.copyTo(undistorted);
        cv::undistort(image, undistorted, cameraMatrix, distCoeffs);
        cv::aruco::detectMarkers(undistorted, dictionary, markerdata.corners, markerdata.ids);
        return markerdata;
    }
    Aruco::AdvancedMarkerData ArucoVision::estimateMarkerPosition(cv::InputArrayOfArrays corners) {
        Aruco::AdvancedMarkerData markerdata;
        cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs,
            markerdata.rvecs, markerdata.tvecs);
        return markerdata;
    }
    void ArucoVision::drawFrameAxes(cv::InputOutputArray image, int markerAmount, Aruco::AdvancedMarkerData markerdata) {
       
        cv::drawFrameAxes(image, cameraMatrix, distCoeffs, markerdata.rvecs, markerdata.tvecs, 0.1);
        
        
    }
}
