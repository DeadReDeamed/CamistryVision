#ifndef SCENE_HANDLER_H
#define SCENE_HANDLER_H

namespace camvis { namespace handlers {

		class SceneHandler
		{
		private:
			//Scene activeScene;


		public:
			void update(float deltaTime);
			void draw();

			void changeScene(int index);

		private:
			void handleArucoUpdate();
			void parseScene(int index);

		};
} }

#endif
