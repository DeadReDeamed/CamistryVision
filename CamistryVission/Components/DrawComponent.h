#ifndef DRAW_COMP_H
#define DRAW_COMP_H

#include "Component.h"

#include "../Data/Model/Model.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../lib/tigl/tigl.h"

namespace camvis 
{
	namespace component
	{
		class DrawComponent : public Component
		{
		private:
			camvis::data::Model* model;
			std::vector<tigl::VBO*> VBOPerGroup;
			
		public:
			DrawComponent(camvis::data::Model* model) : model(model) {
				for (int i = 0; i < model->groups.size(); i++) {
					camvis::data::Group* object = model->groups[i];
					std::vector<tigl::Vertex> verts;
					for (const camvis::data::Face& face : object->faces) {
						for (const camvis::data::Vertex& vertex : face.vertices) {
							tigl::Vertex vertexI;
							if (model->texCoords.size() <= 0) {
								vertexI = tigl::Vertex::PN(model->vertexPositions.at(vertex.positionIndex), model->normals.at(vertex.normalIndex));
							}
							else
								vertexI = tigl::Vertex::PTN(model->vertexPositions.at(vertex.positionIndex),
									model->texCoords.at(vertex.texCoordIndex),
									model->normals.at(vertex.normalIndex));

							verts.push_back(vertexI);
						}
					}
					tigl::VBO* vbo = tigl::createVbo(verts);
					VBOPerGroup.push_back(vbo);
				}
			};

			void update(float deltaTime);
			virtual void draw();
			virtual void draw(glm::vec4 color);
			~DrawComponent() {
				for (tigl::VBO* vbo : VBOPerGroup) {
					free(vbo);
				}
			}
		};
	}
}

#endif
