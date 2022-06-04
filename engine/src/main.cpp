#include<glad/glad.h>
#include<GLFW/glfw3.h>

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
	float r = 0, g = 0, b = 0;
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		r = cos(glfwGetTime()*0.1);
		g = cos(glfwGetTime()*0.2);
		b = cos(glfwGetTime()*0.3);
		glClearColor(r , g , b , 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return (0 - 0);
}