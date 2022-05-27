#include "AtomComponent.h"
#include "../tigl/tigl.h"

#include <tuple>
#include <queue>

#include "../GameObject.h"

using tigl::Vertex;

#define COLOR_RED glm::vec4(1, 0, 0, 0)
#define COLOR_YELLOW glm::vec4(1, 1, 0, 0)

#define DISTANCE_SIZE 1.0f

namespace camvis {
	namespace component
	{
        std::vector<glm::vec4> colors = {
             COLOR_RED,
             COLOR_YELLOW,
        };


		void AtomComponent::update(float deltaTime)
		{
			// No update needed
		}

		void AtomComponent::draw()
		{

            if (core.empty()) { generateCore(bolAmount, glm::mat4(1.0f)); }

            for (auto& coreAtom : core)
            {
                gameObject->transform = glm::translate(gameObject->transform, coreAtom.first);
                DrawComponent::draw(coreAtom.second);
                gameObject->transform = glm::translate(gameObject->transform, -coreAtom.first);
            }
		}

        glm::vec4 AtomComponent::selectCoreColor()
        {
            int randomValue = rand() % colors.size();
            return colors[randomValue];
        }

        void AtomComponent::generateCore(int size, glm::mat4 startPoint)
        {
            // Create the core list
            core = std::vector<std::pair<glm::vec3, glm::vec4>>();
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
                glm::vec3 currentLocation = backQueue.front();

                // Clear the item from the stack
                backQueue.pop();

                // Check if the cube already exits
                if (std::find(openList.begin(), openList.end(), currentLocation) != openList.end()) continue;

                core.push_back(std::make_pair(currentLocation, AtomComponent::selectCoreColor()));

                openList.push_back(currentLocation);

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

                    glm::vec3 nextModel = glm::vec3(currentLocation);
                    nextModel += offsets[i];

                    backQueue.push(nextModel);
                }

                step++;
            }

        }
	}
}