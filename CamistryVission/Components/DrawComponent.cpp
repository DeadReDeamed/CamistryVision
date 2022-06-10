#include "DrawComponent.h"
#include "../GameObject.h"
#include "../lib/tigl/tigl.h"

namespace camvis {
	namespace component
	{
		void DrawComponent::update(float deltaTime)
		{
			
		}

		void DrawComponent::draw()
		{
			if (model == nullptr) return;
			tigl::shader->setModelMatrix(gameObject->transform);

			for (int i = 0; i < model->groups.size(); i++) {
				camvis::data::Group* object = model->groups[i];
				if (object->texture != NULL) {
					object->texture->bind();
				}

				tigl::drawVertices(GL_TRIANGLES, VBOPerGroup[i]);
			}
		}

		void DrawComponent::draw(glm::vec4 color) {
			tigl::shader->enableColorMult(true);
			tigl::shader->setColorMult(color);
			DrawComponent::draw();
			tigl::shader->enableColorMult(false);

			
		}
		
	}
}