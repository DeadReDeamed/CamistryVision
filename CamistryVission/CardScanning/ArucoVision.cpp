#include "ArucoVision.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include "MarkerData.h"
#include <opencv2/imgproc.hpp>

//#include <WinUser.h>
#include <Windows.h>

namespace Aruco {
   
    

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
        printf("Loading camera parameters");
        
        /*if (readCameraParameters(cameraParamPath, cameraMatrix, distCoeffs))
            printf("Loaded camera parameters succesfully");
        else printf("Failed loading camera parameters");*/
        dictionary = cv::aruco::getPredefinedDictionary(dictionaryType);
        //calibrateCamera();

        double dist_[] = { 0, 0, 0, 0, 0 };
        cv::Mat distCoeffs = cv::Mat(5, 1, CV_64F, dist_).clone();

        double baseCamera[3][3] =
        { { 675, 0, 320 },
        { 0, 675, 240 },
        { 0, 0, 1 } };
        cameraMatrix = cv::Mat(3, 3, CV_64F, baseCamera).clone();
    }

    
    void ArucoVision::calibrateCamera() {
        //std::vector<std::vector<cv::Point2f>> corners;
        //std::vector<int> ids;
        //std::vector<int> markerCount;
        //cv::Size size;
        ////cv::Ptr<cv::aruco::Board> board = cv::aruco::Board::create(corners, dictionary, ids);
        //
        ////cv::namedWindow("Camera Calibration", 400);
        //cv::VideoCapture camera;
        //camera.open(0);
        //cv::Mat camFrame;
        //cv::Mat calFrame(100,400, CV_8UC3, cv::Scalar(0,0,0));
        //
        //
        //cv::putText(calFrame, "Press space to make picture", cv::Point(50, 50),
        //    cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 255, 255));
        //
        //cv::imshow("Camera Calibration", calFrame);
        //for (int i = 0; i < 5; i++) {
        //    //check if space has been checked.
        //    cv::imshow("Camera Calibration", calFrame);
        //    int key = -1;
        //    while (key != ' ') {
        //        
        //        if (camera.grab()) {
        //            camera.retrieve(camFrame);
        //       
        //            cv::imshow("Camera Calibration live", camFrame);
        //            key = cv::waitKey(1) % 256;
        //        }
        //        //cv::waitKey(1);
        //        
        //    }
        //    cv::Mat arucoFrame;
        //    camFrame.copyTo(arucoFrame);
        //    cv::Mat grey;
        //    cv::cvtColor(arucoFrame, grey, cv::COLOR_BGR2GRAY);
        //    SimpleMarkerData simpleData = detectMarkers(grey);
        //    if (simpleData.ids.empty()) { 
        //        i--; 
        //        continue;
        //    }
        //    for (auto& c : simpleData.corners)
        //        corners.push_back(c);
        //    for(auto& id : simpleData.ids)
        //        ids.push_back(id);
        //   
        //}
        //std::vector<cv::Mat> rvecs, tvecs;
        //size = cv::Size(camFrame.rows * 2, camFrame.cols * 2);
        //int calibrationFlags = 0;
        //cv::calibrateCamera(corners, ids, size, cameraMatrix, distCoeffs, rvecs, tvecs);
        ////double repError = cv::aruco::calibrateCameraAruco(corners, ids, markerCount, NULL, size,
        //  //  cameraMatrix, distCoeffs, rvecs, tvecs, calibrationFlags);
        //cv::destroyWindow("Camera Calibration live");
        //cv::destroyWindow("Camera Calibration");
    }

    SimpleMarkerData ArucoVision::detectMarkers(cv::InputArray image) {
        Aruco::SimpleMarkerData markerdata;
        //cv::Mat undistorted;
        //image.copyTo(undistorted);
        //cv::undistort(image, undistorted, cameraMatrix, distCoeffs);
        
        cv::aruco::detectMarkers(image, dictionary, markerdata.corners, markerdata.ids);
        return markerdata;
    }
    Aruco::AdvancedMarkerData ArucoVision::estimateMarkerPosition(cv::InputArrayOfArrays corners) {
        Aruco::AdvancedMarkerData markerdata;
        cv::aruco::estimatePoseSingleMarkers(corners, 1.75, cameraMatrix, distCoeffs,
            markerdata.rvecs, markerdata.tvecs);
        return markerdata;
    }
    void ArucoVision::drawFrameAxes(cv::InputOutputArray image, int markerAmount, Aruco::AdvancedMarkerData markerdata) {
        //cv::aruco::drawAxis(image, cameraMatrix, distCoeffs, markerdata.rvecs, markerdata.tvecs, 0.1); 
    }
}
