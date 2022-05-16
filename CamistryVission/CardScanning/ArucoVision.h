#pragma once
#include <opencv2/aruco.hpp>
#include <string>
#include <vector>
namespace Aruco {
	struct SimpleMarkerData {
		std::vector<int> ids;
		std::vector<std::vector<cv::Point2f>> corners;
	};

	struct AdvancedMarkerData {
		std::vector<cv::Vec3d> rvecs, tvecs;
	};

	class ArucoVision
	{
	public:

	private:

	public:
		ArucoVision();
		ArucoVision(std::string cameraParamPath,
			cv::aruco::PREDEFINED_DICTIONARY_NAME dictionaryType);

		SimpleMarkerData detectMarkers(cv::InputArray image);
		AdvancedMarkerData estimateMarkerPosition(cv::InputArrayOfArrays corners);
		void drawFrameAxes(cv::InputOutputArray image, int markerAmount, AdvancedMarkerData markerdata);
	};
}
