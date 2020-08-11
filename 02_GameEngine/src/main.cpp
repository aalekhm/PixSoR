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