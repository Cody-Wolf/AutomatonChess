#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Clock.h"

#include<iostream>


int main()
{
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
	
	cng::Clock ck;
	ck.setSpeed(5.0f);
	float r = 0, g = 0, b = 0;
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			ck.setPause(ck.isPause() ^ 1);
		}
		float delta = ck.getCountSeconds();
		r = cos(delta*0.1);
		g = cos(delta*0.2);
		b = cos(delta*0.3);
		std::cout << "r:" << r << " g:" << g << " b:" << b << std::endl;
		glClearColor(r , g , b , 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return (0 - 0);
}