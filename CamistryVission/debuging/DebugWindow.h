#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace camvis
{
	namespace debugging
	{

		class DebugWindow
		{
		public:
			
			static void init(GLFWwindow* window);

			static void startFrame();
			static void endFrame();

		
		};

	}
}