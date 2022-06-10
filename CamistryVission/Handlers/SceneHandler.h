#ifndef SCENE_HANDLER_H
#define SCENE_HANDLER_H

#include "../Data/Scene.h"
#include "../CardScanning/ArucoHandler.h"

using namespace camvis;

namespace camvis { namespace handlers {

		class SceneHandler
		{
		private:
			data::Scene* activeScene = nullptr;
			GameObject* emptyGameObject;

			Aruco::ArucoHandler* cardHandler;


		public:
			SceneHandler(Aruco::ArucoHandler* cardHandler);
			~SceneHandler();

			void update(float deltaTime);
			void draw();

			void changeScene(int index);

		private:
			void updateAruco(float deltaTime);
			void parseScene(int index);
			void handleEmptyCard(Aruco::MarkerData detectedMarker);

		};
} }

#endif
