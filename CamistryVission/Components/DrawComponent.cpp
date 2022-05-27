#include "DrawComponent.h"
#include "../lib/tigl/tigl.h"

namespace camvis {
	namespace component
	{
		void DrawComponent::update(float deltaTime)
		{
			throw "Function not implemented!";
		}

		void DrawComponent::draw()
		{
			if (model == nullptr) return;

			for (const auto& object : model->groups) {

				if (object->texture != NULL) {
					object->texture->bind();
				}
				else {
					glBindTexture(GL_TEXTURE_2D, 0);
				}

				//TODO: set material color
				tigl::begin(GL_TRIANGLES);
				int scale = 2.0;
				for (const camvis::data::Face& face : object->faces) {
					for (const camvis::data::Vertex& vertex : face.vertices) {
						/*tigl::Vertex vertexI = tigl::Vertex::PTN(model->vertexPositions.at(vertex.positionIndex) * glm::vec3(scale, scale, scale),
							model->texCoords.at(vertex.texCoordIndex),
							model->normals.at(vertex.normalIndex))*/
							tigl::Vertex vertexI = tigl::Vertex::PT(model->vertexPositions.at(vertex.positionIndex) * glm::vec3(scale, scale, scale),
								model->texCoords.at(vertex.texCoordIndex));
						tigl::addVertex(vertexI);
					}
				}
				tigl::end();
			}
		}
	}
}