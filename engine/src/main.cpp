#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Clock.h"
#include"Level.h"

#include<iostream>


int main()
{
	ios::sync_with_stdio(0), cout.tie(0);
	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//GLFWwindow* window = glfwCreateWindow(1920, 1080, "AutomatonChess", glfwGetPrimaryMonitor(), nullptr);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "AutomatonChess", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "GLFW:窗口创建失败" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD:函数加载失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	Clock ck;
	ck.setSpeed(1.0f);

	GlobalRenderManger.init();
	while (!glfwWindowShouldClose(window))
	{
		float delta = ck.getDeltaSeconds();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (!swapLevel())
			currentLevel->tick(delta);

		GlobalRenderManger.Render();
		
		glfwSwapBuffers(window);

		glfwPollEvents();

		std::cout << "FPS:" << 1.0f / delta << std::endl;
	}
	
	glfwTerminate();
	return (0 - 0);
}