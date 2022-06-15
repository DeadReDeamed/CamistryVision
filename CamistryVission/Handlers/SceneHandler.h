#ifndef SCENE_HANDLER_H
#define SCENE_HANDLER_H

namespace camvis { namespace handlers {

		class SceneHandler
		{
		private:
			data::Scene* activeScene = nullptr;
			std::unordered_map<int, GameObject*> emptyGameObjects;
			std::vector<data::Atom> existingAtoms = handlers::DataHandler::getInstance()->atoms;
			std::vector<data::Molecule> existingMolecules = handlers::DataHandler::getInstance()->molecules;


		public:
			void update(float deltaTime);
			void draw();

			void changeScene(int index);

		private:
			void updateAruco(float deltaTime);
			void checkCollision();
			void parseScene(int index);
			void handleEmptyCard(Aruco::MarkerData detectedMarker, bool empty);

		};
} }

#endif
