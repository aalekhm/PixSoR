#include <stdlib.h>
#include "Engine/GameEngine.h"
#include "Engine/InputManager.h"

class MyEngine : public GameEngine
{
	public:
		MyEngine()
		{
			createWindow(480, 320, "06_Triangle");
		}

		virtual void onCreate()
		{
		}

		virtual void onUpdate(uint32_t iDeltaTimeMs, uint64_t lElapsedTime)
		{
			InputManager::get()->getMousePos(m_fMouseX, m_fMouseY);
		}

		virtual void onPaint(Graphics* pGraphics)
		{
			pGraphics->clear();

			pGraphics->setColour(Pixel(0, 255, 255, 255));
			pGraphics->fillTriangle(	Vertex( Vector4f(m_iWidth >> 1, m_iHeight >> 2, 0) ),
										Vertex( Vector4f(m_iWidth >> 2, m_iHeight >> 1, 0) ),
										Vertex( Vector4f(m_fMouseX, m_fMouseY, 0) )
									);

			pGraphics->setColour(Pixel(255, 255, 255, 255));
			pGraphics->drawTriangle(	Vertex( Vector4f(m_iWidth >> 1, m_iHeight >> 2, 0) ),
										Vertex( Vector4f(m_iWidth >> 2, m_iHeight >> 1, 0) ),
										Vertex( Vector4f(m_fMouseX, m_fMouseY, 0) )
									);
		}

		virtual void onDestroy()
		{

		}
	protected:
	private:
		double	m_fMouseX;
		double	m_fMouseY;
};

int main(int argc, char** argv)
{
	MyEngine me;

	exit(EXIT_SUCCESS);
}