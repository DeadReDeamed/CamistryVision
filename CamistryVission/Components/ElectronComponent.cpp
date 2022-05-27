#include "ElectronComponent.h"
#include "AtomComponent.h"


namespace camvis {
	namespace component
	{
	
		ElectronComponent::ElectronComponent(std::vector<Shell*> shells) : shells(shells), DrawComponent(new camvis::data::Model("Resources\\models\\ball.obj")) {
		}

		void ElectronComponent::update(float deltaTime)
		{
			for (const auto& shell : shells)
			{
				shell->rotation.x += shell->speed.x * deltaTime;
				shell->rotation.y += shell->speed.y * deltaTime;
				shell->rotation.z += shell->speed.z * deltaTime;
			}
		}


		void ElectronComponent::draw()
		{
			

			for (const auto& shell : shells)
			{
				if (shell->amount == 0) continue;

				float distanceBetweenElectrons = 360.0f / shell->amount;

				gameObject->rotate(shell->rotation, false);
				//gameObject->translate(glm::vec3(-shell->distance, 0.0f, 0.0f));
				DrawCircle(0,0, shell->distance, 200);
				//gameObject->translate(glm::vec3(shell->distance, 0.0f, 0.0f));
				gameObject->rotate(-shell->rotation, true);

				for (size_t i = 0; i < shell->amount; i++)
				{
					glm::vec3 rot = shell->rotation;
					rot.z += i * distanceBetweenElectrons;

					gameObject->rotate(rot, false);
					gameObject->translate(glm::vec3(shell->distance, 0.0f, 0.0f));
					DrawComponent::draw();
					gameObject->translate(glm::vec3(-shell->distance, 0.0f, 0.0f));
					gameObject->rotate(-rot, true);
				}
			}
		}
	}
}