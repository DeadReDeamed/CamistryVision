#pragma once
#include "ArucoVision.h"

#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core/opengl.hpp>

#include "MarkerData.h"
namespace Aruco {
	class ArucoHandler
	{
	private:
		Aruco::ArucoVision aruco;
		std::vector<MarkerData> DetectedMarkers;
		cv::Mat lastImage;
		std::thread arucothread;

		cv::VideoCapture camera;
		bool isRunning;
	private:
		void run();
	public:
		ArucoHandler() { isRunning = false; }

		/**
		* Used to start marker detection.
		*/
		void start();
		/**
		* Used to stop marker detection.
		*/
		void stop();
		/*
		* Used to get a list of markers that are currently detected.
		* @return a list of MarkerData.
		*/
		std::vector<MarkerData> getMarkers();
		/**
		* Used to get some data of the camera.
		* @return A 2d vector which contains the width and height of the camera view.
		*/
		cv::Vec2d getCameraData();
		/**
		* Used to get the last captured image.
		* @return A Mat.
		*/
		cv::Mat getLastImage();
	};

}