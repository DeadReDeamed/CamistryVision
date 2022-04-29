#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <string>

void generateImages();

inline static bool readCameraParameters(std::string filename, cv::Mat& camMatrix, cv::Mat& distCoeffs) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        std::cout << "Could not open file!" << std::endl;
        return false;
    }
    std::cout << "Succesfully opened file!" << std::endl;
    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}


int main()
{
    /* generateImages();
     std::string path = "markers.PNG";
     cv::Mat image, imageCopy;
     image = cv::imread(path);
     cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

     inputVideo.retrieve(image);
     image.copyTo(imageCopy);

     std::vector<int> ids;
     std::vector<std::vector<cv::Point2f> > corners;
     cv::aruco::detectMarkers(image, dictionary, corners, ids);
      if at least one marker detected
     if (ids.size() > 0)
         cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
     cv::imshow("out", imageCopy);

     char key = (char)cv::waitKey(waitTime);
     cv::waitKey(0);*/
    int waitTime = 1;
    cv::VideoCapture inputVideo;
    inputVideo.open(0);

    cv::Mat cameraMatrix, distCoeffs;

    readCameraParameters("tutorial_camera_params.yml",
        cameraMatrix, distCoeffs);


    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    while (inputVideo.grab()) {
        cv::Mat image, imageCopy;
        inputVideo.retrieve(image);
        image.copyTo(imageCopy);
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f> > corners;
        cv::aruco::detectMarkers(image, dictionary, corners, ids);
        // if at least one marker detected
        if (ids.size() > 0) {
            //  cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);

            std::vector<cv::Vec3d> rvecs, tvecs;
            cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix,
                distCoeffs, rvecs, tvecs);

            for (int i = 0; i < ids.size(); i++) {
                cv::drawFrameAxes(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
            }


        }


        cv::imshow("out", imageCopy);

        char key = (char)cv::waitKey(waitTime);
        if (key == 27)
            break;
    }
}