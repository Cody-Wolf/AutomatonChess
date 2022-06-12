#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Clock.h"
#include"Level.h"

#include<iostream>
#include<Windows.h>
#include<Winbase.h>


int main()
{
	ios::sync_with_stdio(0), cout.tie(0);
	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef _DEBUG
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "AutomatonChess", nullptr, nullptr);
#else
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "AutomatonChess", glfwGetPrimaryMonitor(), nullptr);
#endif // _DEBUG

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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	Clock ck;
	ck.setSpeed(1.0f);
	GlobalRenderManger.init();
	nextLevel = new StartLevel;
	while (!glfwWindowShouldClose(window))
	{
		float delta = ck.getDeltaSeconds();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (!swapLevel())
			currentLevel->tick(window, delta);

		GlobalRenderManger.Render();
		
		glfwSwapBuffers(window);

		glfwPollEvents();
		std::cout << "FPS:" << 1.0f / delta << std::endl;
	}
	
	glfwTerminate();
	return (0 - 0);
}