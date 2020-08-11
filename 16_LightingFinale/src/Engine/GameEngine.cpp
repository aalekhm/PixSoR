#include "Engine/GameEngine.h"
#include "Engine/InputManager.h"
#include <memory>

#define FRAME_START \
{ \
	TIMER_STOP(m_UpdateTimer); \
	TIMER_START(m_UpdateTimer); \
} \

#define UPDATE_PROLOGUE \
{ \
	FRAME_START \
} \

#define UPDATE_EPILOGUE \
{ \
	if(InputManager::get()->isKeyPressed(GLFW_KEY_ESCAPE)) \
	{ \
		glfwSetWindowShouldClose(m_pGLFWWindow, GLFW_TRUE); \
	} \
\
	glfwSwapBuffers(m_pGLFWWindow); \
	glfwPollEvents(); \
} \


void GameEngine::createWindow(uint32_t iWidth, uint32_t iHeight, const char* sWindowTitle)
{
	init(iWidth, iHeight, sWindowTitle);
	onCreate();
	update();
}

void GameEngine::init(uint32_t iWidth, uint32_t iHeight, const char* sWindowTitle)
{
	TIMER_START_NEW(t)
	{
		m_iWidth = iWidth;
		m_iHeight = iHeight;

		initGLFW(sWindowTitle);

		m_pGraphics = Graphics::create(m_iWidth, m_iHeight);
		m_fRenderCallback = std::bind(&GameEngine::onPaint, this, std::placeholders::_1);

		TIMER_START(m_EngineElapsedTimer)
	}
	TIMER_STOP(t)

	std::cout << "Time Taken by init() : " << t.duration(DURATION_TYPE::milliseconds) << "\n";
}

void GameEngine::initGLFW(const char* sWindowTitle)
{
	m_pGLFWWindow = nullptr;
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	m_pGLFWWindow = glfwCreateWindow(m_iWidth, m_iHeight, sWindowTitle, nullptr, nullptr);
	if (m_pGLFWWindow == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(m_pGLFWWindow,			onKeyCallback);
	glfwSetMouseButtonCallback(m_pGLFWWindow,	onMouseButtonCallback);
	glfwSetCursorPosCallback(m_pGLFWWindow,		onMouseCursorCallback);

	// This function makes the OpenGL or OpenGL ES context of the specified window current on the calling thread.
	glfwMakeContextCurrent(m_pGLFWWindow);
	glfwSwapInterval(1);
}

void GameEngine::update()
{
	FRAME_START
	while (!glfwWindowShouldClose(m_pGLFWWindow))
	{
		UPDATE_PROLOGUE
		{
			// Update & Render Game here...
			onUpdate(m_UpdateTimer.duration(DURATION_TYPE::milliseconds), getElapsedTime(DURATION_TYPE::milliseconds));
			m_pGraphics->paint( m_fRenderCallback );
		}
		UPDATE_EPILOGUE
	}

	onDestroy();
	glfwDestroyWindow(m_pGLFWWindow);
}

void GameEngine::setClearColourAndDepth(Pixel pPixel, float fClearDepth)
{
	m_pGraphics->setClearColourAndDepth(pPixel, fClearDepth);
}

uint64_t GameEngine::getElapsedTime(DURATION_TYPE eDURATION_TYPE)
{
	TIMER_STOP(m_EngineElapsedTimer)
	return m_EngineElapsedTimer.duration(eDURATION_TYPE);
}

Graphics* GameEngine::getGraphics()
{
	return m_pGraphics.get();
}

void GameEngine::onKeyCallback(GLFWwindow* pWindow, int iKey, int iScanCode, int iAction, int iModifierKeys)
{
	if (iAction == GLFW_PRESS)
	{
		InputManager::get()->onKeyPressed(iKey);
	}
	else
	if (iAction == GLFW_RELEASE)
	{
		InputManager::get()->onKeyReleased(iKey);
	}
}

void GameEngine::onMouseButtonCallback(GLFWwindow* pWindow, int iButton, int iAction, int iModifierKeys)
{
	if (iAction == GLFW_PRESS)
	{
		InputManager::get()->onMousePressed(iButton);
	}
	else
	if (iAction == GLFW_RELEASE)
	{
		InputManager::get()->onMouseReleased(iButton);
	}
}

void GameEngine::onMouseCursorCallback(GLFWwindow* pWindow, double xpos, double ypos)
{
	InputManager::get()->onMouseMoved(xpos, ypos);
}
