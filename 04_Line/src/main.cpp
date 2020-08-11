#include <stdlib.h>
#include "Engine/GameEngine.h"
#include "Engine/InputManager.h"

class MyEngine : public GameEngine
{
	public:
		MyEngine()
		{
			createWindow(480, 320, "PixSoR Window!");
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
			
			pGraphics->setColour(Pixel(255, 255, 0, 255));
			pGraphics->drawLine(m_iWidth >> 1, m_iHeight >> 1, m_fMouseX, m_fMouseY);
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