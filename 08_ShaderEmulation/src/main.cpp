#include <stdlib.h>
#include "Engine/GameEngine.h"
#include "Engine/InputManager.h"
#include "Renderer/OpenGL.h"
#include "Renderer/IShader.h"

VertexColourShader		gShader;
GLuint					VBO_0;

class MyEngine : public GameEngine
{
	public:
		MyEngine()
		{
			createWindow(480, 320, "08_ShaderEmulation");
		}

		virtual void onCreate()
		{
			OpenGL::m_pGraphics = getGraphics();

			Vertex pVertices[3];
			{
								// Position									// Colour
				pVertices[0]	= Vertex( Vector4f(0,		0.5,	0, 1),	Vector4f(1.0f, 0.0f, 0.0f, 0.0f) );
				pVertices[1]	= Vertex( Vector4f(-0.5,	-0.5,	0, 1),	Vector4f(0.0f, 1.0f, 0.0f, 0.0f) );
				pVertices[2]	= Vertex( Vector4f(0.5,	-0.5,	0, 1),		Vector4f(0.0f, 0.0f, 1.0f, 0.0f) );

				OpenGL::glGenBuffers(1, &VBO_0);
				OpenGL::glBindBuffer(GL_ARRAY_BUFFER, VBO_0);
				OpenGL::glBufferData(&pVertices, sizeof(pVertices), sizeof(Vertex) / sizeof(float));
			}
			gShader.compile();
		}

		virtual void onUpdate(uint32_t iDeltaTimeMs, uint64_t lElapsedTime)
		{
			InputManager::get()->getMousePos(m_fMouseX, m_fMouseY);
		}

		virtual void onPaint(Graphics* pGraphics)
		{
			pGraphics->clear();

			OpenGL::useProgram(&gShader);
			OpenGL::glBindBuffer(GL_ARRAY_BUFFER, VBO_0);
			OpenGL::glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		virtual void onDestroy()
		{
			OpenGL::glDeleteBuffers(1, &VBO_0);
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