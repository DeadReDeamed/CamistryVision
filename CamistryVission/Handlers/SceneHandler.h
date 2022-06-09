#ifndef SCENE_HANDLER_H
#define SCENE_HANDLER_H

#include "../Data/Scene.h"
#include "../CardScanning/ArucoHandler.h"
#include "../Handlers/DataHandler.h"

namespace camvis { namespace handlers {

		class SceneHandler
		{
		private:
			data::Scene* activeScene = nullptr;
			std::vector<data::Atom> existingAtoms = handlers::DataHandler::getInstance()->atoms;
			std::vector<data::Molecule> existingMolecules = handlers::DataHandler::getInstance()->molecules;

			Aruco::ArucoHandler* cardHandler;

		public:
			SceneHandler(Aruco::ArucoHandler* cardHandler) : cardHandler(cardHandler), activeScene(nullptr) {};

			void update(float deltaTime);
			void draw();

			void changeScene(int index);

		private:
			void updateAruco();
			void checkCollision();
			void parseScene(int index);


		};
} }

#endif
