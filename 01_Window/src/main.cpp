#include <stdlib.h>
#include <GLFW/glfw3.h>

GLFWwindow* m_pWindow = nullptr;

void onKeyCallback(GLFWwindow* pWindow, int iKey, int iScanCode, int iAction, int iModifierKeys);
void onMouseButtonCallback(GLFWwindow* pWindow, int iButton, int iAction, int iModifierKeys);
void onMouseCursorCallback(GLFWwindow* pWindow, double xpos, double ypos);

void createWindow(uint32_t iWidth, uint32_t iHeight, const char* sWindowTitle);
void update();

int main(int argc, char** argv)
{
	createWindow(480, 320, "PixSoR Window!");

	update();

	exit(EXIT_SUCCESS);
}

void createWindow(uint32_t iWidth, uint32_t iHeight, const char* sWindowTitle)
{
	m_pWindow = nullptr;
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_pWindow = glfwCreateWindow(iWidth, iHeight, sWindowTitle, nullptr, nullptr);
	if (m_pWindow == nullptr)
	{
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(m_pWindow, onKeyCallback);
	glfwSetMouseButtonCallback(m_pWindow, onMouseButtonCallback);
	glfwSetCursorPosCallback(m_pWindow, onMouseCursorCallback);
}

void update()
{
	while (!glfwWindowShouldClose(m_pWindow))
	{
		// Update & Render Game here...

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}

	glfwDestroyWindow(m_pWindow);
}

void onKeyCallback(GLFWwindow* pWindow, int iKey, int iScanCode, int iAction, int iModifierKeys)
{
	if (iAction == GLFW_PRESS)
	{
		if (iKey == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
		}
	}
	else
	if (iAction == GLFW_RELEASE)
	{

	}
}

void onMouseButtonCallback(GLFWwindow* pWindow, int iButton, int iAction, int iModifierKeys)
{
	if (iAction == GLFW_PRESS)
	{
	}
	else
	if (iAction == GLFW_RELEASE)
	{

	}
}

void onMouseCursorCallback(GLFWwindow* pWindow, double xpos, double ypos)
{

}
