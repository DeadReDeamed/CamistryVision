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

			for (const auto& object : model->groups) {

				if (object->texture != NULL) {
					object->texture->bind();
				}

				tigl::begin(GL_TRIANGLES);
				for (const camvis::data::Face& face : object->faces) {
					for (const camvis::data::Vertex& vertex : face.vertices) {
						tigl::Vertex vertexI;
						if (model->texCoords.size() <= 0) {
							vertexI = tigl::Vertex::P(model->vertexPositions.at(vertex.positionIndex));
						}
						else
							vertexI = tigl::Vertex::PT(model->vertexPositions.at(vertex.positionIndex),
								model->texCoords.at(vertex.texCoordIndex));
						
						tigl::addVertex(vertexI);
					}
				}
				tigl::end();
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