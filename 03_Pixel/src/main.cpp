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
			setClearColour(Pixel(255, 255, 0, 255));
		}

		virtual void onUpdate(uint32_t iDeltaTimeMs, uint64_t lElapsedTime)
		{
		}

		virtual void onPaint(Graphics* pGraphics)
		{
			pGraphics->clear();

			for (int y = 0; y < m_iHeight; y++)
			{
				for (int x = 0; x < m_iWidth; x++)
				{
					pGraphics->setPixel(x, y, Pixel(rand() % 255, rand() % 255, rand() % 255));
				}
			}
		}

		virtual void onDestroy()
		{

		}
	protected:
	private:
};

int main(int argc, char** argv)
{
	MyEngine me;

	exit(EXIT_SUCCESS);
}