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

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "AutomatonChess", glfwGetPrimaryMonitor(), nullptr);
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
	ck.setSpeed(5.0f);
	float r = 0, g = 0, b = 0;
	Clock sck;
	while (!glfwWindowShouldClose(window))
	{
		std::cout << "Clost Cost:" << sck.getDeltaSeconds() << std::endl;
		float delta = ck.getDeltaSeconds();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		std::cout << "ESC cost:" << sck.getDeltaSeconds() << std::endl;

		if (!swapLevel())
			currentLevel->tick(delta);
		std::cout << "Game Cost:" << sck.getDeltaSeconds() << std::endl;

		GlobalRenderManger.Render();
		std::cout << "Render Cost:" << sck.getDeltaSeconds() << std::endl;
		
		glfwSwapBuffers(window);
		std::cout << "Swap Cost" << sck.getDeltaSeconds() << std::endl;

		glfwPollEvents();
		std::cout << "Poll Cost" << sck.getDeltaSeconds() << std::endl;

		std::cout << "Frame Cost:" <<  delta << std::endl;
	}
	
	glfwTerminate();
	return (0 - 0);
}