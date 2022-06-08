#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Clock.h"
#include"Level.h"

#include<iostream>


int main()
{
	return 0;
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
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			if (typeid(*currentLevel) == typeid(DefaultLevel))
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			else
				setNextLevel<DefaultLevel>();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			setNextLevel<aLevel>();
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			setNextLevel<bLevel>();
		}
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		{
			setNextLevel<cLevel>();
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			ck.setPause(ck.isPause() ^ 1);
		}

		if (!swapLevel())
			currentLevel->tick(ck.getDeltaSeconds());
		
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return (0 - 0);
}