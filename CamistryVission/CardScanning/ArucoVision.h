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

	public:
		ArucoVision();
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
	};
}

