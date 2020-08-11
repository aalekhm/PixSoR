#pragma once
#include "Common/Defines.h"
#include "Engine/Timer.h"
#include <functional>
#include "Renderer/Graphics.h"

class GameEngine
{
	public:
	protected:
		void				createWindow(uint32_t iWidth, uint32_t iHeight, const char* sWindowTitle);

		virtual void		onCreate() = 0;
		virtual void		onUpdate(uint32_t iDeltaTimeMs, uint64_t lElapsedTime) = 0;
		virtual void		onPaint(Graphics* pGraphics) = 0;
		virtual void		onDestroy() = 0;

		uint64_t			getElapsedTime(DURATION_TYPE eDURATION_TYPE);
		void				setClearColourAndDepth(Pixel pPixel, float fClearDepth);

		Graphics*			getGraphics();

		uint32_t			m_iWidth;
		uint32_t			m_iHeight;
	private:
		static void			onKeyCallback(GLFWwindow* pWindow, int iKey, int iScanCode, int iAction, int iModifierKeys);
		static void			onMouseButtonCallback(GLFWwindow* pWindow, int iButton, int iAction, int iModifierKeys);
		static void			onMouseCursorCallback(GLFWwindow* pWindow, double xpos, double ypos);

		void				init(uint32_t iWidth, uint32_t iHeight, const char* sWindowTitle);
		void				initGLFW(const char* sWindowTitle);

		void				update();

		GLFWwindow*			m_pGLFWWindow;
		Timer				m_EngineElapsedTimer;
		Timer				m_UpdateTimer;

		std::unique_ptr<Graphics>		m_pGraphics = nullptr;
		std::function<void(Graphics*)>	m_fRenderCallback = nullptr;
};