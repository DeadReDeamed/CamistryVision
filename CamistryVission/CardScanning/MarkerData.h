#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
class MarkerData {
public:
	int id;
	std::vector<cv::Point2f> corners;
	cv::Vec3d transform, rotation;
public:
	MarkerData(int id, std::vector<cv::Point2f> corners, cv::Vec3d transform, cv::Vec3d rotation) 
		: id(id), corners(corners), transform(transform), rotation(rotation) {};
};