#include "ArucoVision.h"

#include <opencv2/calib3d.hpp>

namespace Aruco {
   
    cv::Mat cameraMatrix, distCoeffs;
    cv::Ptr<cv::aruco::Dictionary> dictionary;

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
    ArucoVision::ArucoVision(std::string cameraParamPath,
        cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryType) {
        printf("Loading camera parameters");
        if (readCameraParameters(cameraParamPath, cameraMatrix, distCoeffs))
            printf("Loaded camera parameters succesfully");
        else printf("Failed loading camera parameters");
        dictionary = cv::aruco::getPredefinedDictionary(dictionaryType);
    }

    SimpleMarkerData ArucoVision::detectMarkers(cv::InputArray image) {
        Aruco::SimpleMarkerData markerdata;
        cv::aruco::detectMarkers(image, dictionary, markerdata.corners, markerdata.ids);
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
