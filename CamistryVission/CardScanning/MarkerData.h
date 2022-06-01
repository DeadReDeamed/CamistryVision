#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
namespace Aruco {
	struct MarkerData {
	public:
		int id; // Is the id of a marker.
		std::vector<cv::Point2f> corners; // Is a list of points that represent the corners of a marker.
		cv::Vec3d transform; // Is a 3d vector that represents a transform.
		cv::Vec3d rotation; // Is a 3d vector that represents a rotation.

		AdvancedMarkerData advancedData;

	public:
		MarkerData(int id, std::vector<cv::Point2f> corners, cv::Vec3d transform, cv::Vec3d rotation, AdvancedMarkerData advancedData)
			: id(id), corners(corners), transform(transform), rotation(rotation), advancedData(advancedData) {};
	};
}