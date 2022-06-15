#include "ElectronComponent.h"

namespace camvis {
	namespace component
	{
	
		ElectronComponent::ElectronComponent(std::vector<Shell*> shells) : shells(shells), DrawComponent(data::BallModelSingleton::getInstance().ballModel) {
		}

		void ElectronComponent::update(float deltaTime)
		{
			throw "Function not implemented!";
		}

		void ElectronComponent::draw()
		{
			throw "Function not implemented!";
		}
	}
}