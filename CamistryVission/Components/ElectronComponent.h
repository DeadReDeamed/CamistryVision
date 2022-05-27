#ifndef ELECTRON_COMP_H
#define ELECTRON_COMP_H

#include "DrawComponent.h"
#include "../GameObject.h"
#include "../lib/tigl/tigl.h"

namespace camvis {
	namespace component
	{
		struct Shell {
			float distance;
			int amount;
			glm::vec3 rotation;
			glm::vec3 speed;
		};

		class ElectronComponent : public DrawComponent
		{
		public:
			ElectronComponent(std::vector<Shell*> shells);
			void update(float deltaTime);
			void draw();

		private:
			std::vector<Shell*> shells;


			void DrawCircle(float cx, float cy, float r, int num_segments) {
				tigl::shader->setModelMatrix(gameObject->transform);
				tigl::shader->enableColorMult(true);
				tigl::shader->setColorMult(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				tigl::begin(GL_LINE_LOOP);
				for (int ii = 0; ii < num_segments; ii++) {
					float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
					float x = r * cosf(theta);//calculate the x component 
					float y = r * sinf(theta);//calculate the y component 
					tigl::addVertex(tigl::Vertex::P(glm::vec3(x + cx, y + cy, 0)));//output vertex 
				}
				tigl::end();
				tigl::shader->enableColorMult(false);

			}

		};
	}
}

#endif
