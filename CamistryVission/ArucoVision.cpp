#include "ArucoVision.h"

#include <opencv2/calib3d.hpp>
//class ArucoVisionImpl : public ArucoVision {
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

    ArucoVision::ArucoVision(std::string cameraParamPath,
        cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryType) {
        printf("Loading camera parameters");
        if (readCameraParameters(cameraParamPath, cameraMatrix, distCoeffs))
            printf("Loaded camera parameters succesfully");
        else printf("Failed loading camera parameters");
        dictionary = cv::aruco::getPredefinedDictionary(dictionaryType);
    }

    ArucoVision::SimpleMarkerData ArucoVision::detectMarkers(cv::InputArray image) {
        ArucoVision::SimpleMarkerData markerdata;
        cv::aruco::detectMarkers(image, dictionary, markerdata.corners, markerdata.ids);
        return markerdata;
    }
    ArucoVision::AdvancedMarkerData ArucoVision::estimateMarkerPosition(cv::InputArrayOfArrays corners) {
        ArucoVision::AdvancedMarkerData markerdata;
        cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs,
            markerdata.rvecs, markerdata.tvecs);
        return markerdata;
    }
    void ArucoVision::drawFrameAxes(cv::InputOutputArray image, int markerAmount, ArucoVision::AdvancedMarkerData markerdata) {
        for (int i = 0; i < markerAmount; i++) {
            cv::drawFrameAxes(image, cameraMatrix, distCoeffs, markerdata.rvecs, markerdata.tvecs, 0.1);
        }
    }
//}
