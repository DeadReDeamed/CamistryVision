#pragma once
#include <opencv2/aruco.hpp>
#include <string>
#include <vector>
namespace Aruco {
	
	/**
	* A struct that contains some simple aruco data.
	*/
	struct SimpleMarkerData {
		std::vector<int> ids; // A list of marker ids.
		std::vector<std::vector<cv::Point2f>> corners; /// A list of a list of corners of a marker.
	};

	/**
	* A struct that contains some extra aruco data.
	*/
	struct AdvancedMarkerData {
		std::vector<cv::Vec3d> rvecs; // A list of 3d vectors that represent a rotation.
		std::vector<cv::Vec3d> tvecs; // A list of 3d vectors that represent a transform
	};

	class ArucoVision
	{
	public:

	private:
		cv::Mat cameraMatrix, distCoeffs;
		cv::Ptr<cv::aruco::Dictionary> dictionary;
	public:
		ArucoVision();
		ArucoVision(cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryType);
		ArucoVision(std::string cameraParamPath,
			cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryType);

		/*
		* Used to detect marker in an image. 
		* @param[out] image is the input image.
		* @return a struct of SimpleMarkerData.
		*/
		SimpleMarkerData detectMarkers(cv::InputArray image);
		/*
		* Used to estimate the 3d position of markers.
		* @param[in] corners is an InputArrayOfArrays.
		* @return a struct of AdvancedMarkerData.
		*/
		AdvancedMarkerData estimateMarkerPosition(cv::InputArrayOfArrays corners);
		/**
		* Used to draw 3d axes on an image with Advanced
		* @param[out] image is the image where the axes are drawn on.
		* @param[in] markerAmount is the amount of markers to draw.
		* @param[in] markerdata is a struct of AdvancedMarkerData.
		*/
		void drawFrameAxes(cv::InputOutputArray image, int markerAmount, AdvancedMarkerData markerdata);

	private:
		/**
		* Used to calibrate the camera used for detecting markers.
		* While calibrating it is wise to take atleast 20 pictures at different angles.
		* The calibration board exists of a 5x5 chess board. Most preferably black and white.
		* 
		* @note calibration might take a while. For 20 images it will take around 1-2 minutes to calibrate in debug mode.
		*/
		void calibrateCamera();
	};
}

