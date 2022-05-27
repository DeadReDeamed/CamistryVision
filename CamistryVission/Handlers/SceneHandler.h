#ifndef SCENE_HANDLER_H
#define SCENE_HANDLER_H

#include "../Data/Scene.h"
#include "../CardScanning/ArucoHandler.h"

namespace camvis { namespace handlers {

		class SceneHandler
		{
		

		private:
			data::Scene activeScene;
			Aruco::ArucoHandler* arucoHandler;


		public:
			void update(float deltaTime);
			void draw();

			void changeScene(int index);

		private:
			void handleArucoUpdate();
			void parseScene(int index);

		public:
			inline SceneHandler(data::Scene scene, Aruco::ArucoHandler* handler)
				: activeScene(scene), arucoHandler(handler) {};
		};
} }

#endif
