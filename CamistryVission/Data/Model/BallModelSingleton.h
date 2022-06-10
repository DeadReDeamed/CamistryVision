#pragma once
#include "Model.h"
namespace camvis {

	namespace data {
		class BallModelSingleton {
		public:
			Model* ballModel;
		private:
			BallModelSingleton() {
				ballModel = new Model("Resources\\models\\ball.obj");
			}
		public:
			static BallModelSingleton& getInstance() {
				static BallModelSingleton instance = BallModelSingleton();
				return instance;
			}

		public:
	
			void operator=(BallModelSingleton const&) = delete;
		};
	}
}