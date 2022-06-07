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

			Aruco::ArucoHandler* cardHandler;


		public:
			SceneHandler(Aruco::ArucoHandler* cardHandler) : cardHandler(cardHandler), activeScene(nullptr) {};

			void update(float deltaTime);
			void draw();

			void changeScene(int index);

		private:
			void updateAruco();
			void parseScene(int index);

		};
} }

#endif
