#include "AtomComponent.h"
#include "../tigl.h"

#include <tuple>
#include <queue>

#include "../GameObject.h"

using tigl::Vertex;

#define DISTANCE_SIZE 1.0f

namespace camvis {
	namespace component
	{
        std::vector<glm::vec4> colors = {
             glm::vec4(1, 0, 0, 0),
             glm::vec4(1, 1, 0, 0),
        };


		void AtomComponent::update(float deltaTime)
		{
			// No update needed
		}

		void AtomComponent::draw()
		{
            objectMatrix = glm::mat4(1.0f);

            objectMatrix = glm::scale(objectMatrix, gameObject->scale);
            objectMatrix = glm::translate(objectMatrix, gameObject->position);
            objectMatrix = glm::rotate(objectMatrix, glm::radians(gameObject->rotation.x), glm::vec3(1, 0, 0));
            objectMatrix = glm::rotate(objectMatrix, glm::radians(gameObject->rotation.y), glm::vec3(0, 1, 0));
            objectMatrix = glm::rotate(objectMatrix, glm::radians(gameObject->rotation.z), glm::vec3(0, 0, 1));
            
            if (core.empty()) { generateCore(bolAmount, glm::mat4(1.0f)); }

            for (auto& model : core)
            {
                AtomComponent::drawCube(std::get<0>(model), std::get<1>(model));
            }
		}

        glm::vec4 AtomComponent::selectCoreColor()
        {
            int randomValue = rand() % colors.size();
            return colors[randomValue];
        }

        void AtomComponent::drawCube(glm::mat4 model, glm::vec4 color)
        {
            // TODO: Cube model is temporary, should use this.model
            std::vector<Vertex> cube{

                // Front face   
                Vertex::PC(glm::vec3(0, 0, 0), color),
                Vertex::PC(glm::vec3(1, 0, 0), color),
                Vertex::PC(glm::vec3(1, 1, 0), color),
                Vertex::PC(glm::vec3(0, 1, 0), color),

                // Back face
                Vertex::PC(glm::vec3(0, 0, 1), color),
                Vertex::PC(glm::vec3(1, 0, 1), color),
                Vertex::PC(glm::vec3(1, 1, 1), color),
                Vertex::PC(glm::vec3(0, 1, 1), color),

                // Right face
                Vertex::PC(glm::vec3(1, 0, 0), color),
                Vertex::PC(glm::vec3(1, 0, 1), color),
                Vertex::PC(glm::vec3(1, 1, 1), color),
                Vertex::PC(glm::vec3(1, 1, 0), color),

                // Left Face
                Vertex::PC(glm::vec3(0, 0, 0), color),
                Vertex::PC(glm::vec3(0, 0, 1), color),
                Vertex::PC(glm::vec3(0, 1, 1), color),
                Vertex::PC(glm::vec3(0, 1, 0), color),

                // Top Face
                Vertex::PC(glm::vec3(0, 1, 0), color),
                Vertex::PC(glm::vec3(1, 1, 0), color),
                Vertex::PC(glm::vec3(1, 1, 1), color),
                Vertex::PC(glm::vec3(0, 1, 1), color),

                // Down Face
                Vertex::PC(glm::vec3(0, 1, 0), color),
                Vertex::PC(glm::vec3(1, 0, 0), color),
                Vertex::PC(glm::vec3(1, 0, 1), color),
                Vertex::PC(glm::vec3(0, 0, 1), color),
            };

            // Move the cube based on gameObject posistion
            //model *= objectMatrix;
            model = objectMatrix * model;

            tigl::shader->setModelMatrix(model);
            tigl::drawVertices(GL_QUADS, cube);

        }

        void AtomComponent::generateCore(int size, glm::mat4 startPoint)
        {
            // Create the core list
            core = std::vector<std::tuple<glm::mat4, glm::vec4>>();
            core.reserve(size);

            // Create the backstack for the build argorithm
            std::queue<glm::vec3> backQueue = std::queue<glm::vec3>();

            // Create the open list to compare to
            std::vector<glm::vec3> openList = std::vector<glm::vec3>();

            // Add the starting point to the queue
            backQueue.push(glm::vec3(0, 0, 0));

            int step = 0;
            while (step < size && !backQueue.empty())
            {
                glm::vec3 currentModel = backQueue.front();

                // Clear the item from the stack
                backQueue.pop();

                // Check if the cube already exits
                if (std::find(openList.begin(), openList.end(), currentModel) != openList.end()) continue;

                core.push_back(std::make_tuple(glm::translate(startPoint, currentModel), selectCoreColor()));

                openList.push_back(currentModel);

                for (int i = 0; i < 6; i++)
                {
                    glm::vec3 offsets[6] = {
                        glm::vec3(DISTANCE_SIZE, 0.0f, 0.0f),
                        glm::vec3(0.0f, DISTANCE_SIZE, 0.0f),
                        glm::vec3(-DISTANCE_SIZE, 0.0f, 0.0f),
                        glm::vec3(0.0f,-DISTANCE_SIZE, 0.0f),
                        glm::vec3(0.0f, 0.0f, DISTANCE_SIZE),
                        glm::vec3(0.0f, 0.0f, -DISTANCE_SIZE),
                    };

                    glm::vec3 nextModel = glm::vec3(currentModel);
                    nextModel += offsets[i];

                    backQueue.push(nextModel);
                }

                step++;
            }

        }
	}
}