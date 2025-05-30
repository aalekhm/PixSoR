#include <stdlib.h>
#include "Engine/GameEngine.h"
#include "Engine/InputManager.h"
#include "Renderer/OpenGL.h"
#include "Renderer/IShader.h"
#include "3D/Matrix4f.h"

VertexColourShader		gShader;
GLuint					VBO_0;

Matrix4f				gMatProjection;

Matrix4f				gMatTransformation;
float					gfRotCounter;
Matrix4f				matTranslation;
Matrix4f				matRotation;
Matrix4f				matWVP;

class MyEngine : public GameEngine
{
	public:
		MyEngine()
		{
			createWindow(480, 320, "09_Transform");
		}

		virtual void onCreate()
		{
			OpenGL::m_pGraphics = getGraphics();
			gMatProjection.initPerspective(	(float)DEG_2_RAD(70.0f),		// 'F'ield 'O'f 'V'iew
											(float)m_iWidth / m_iHeight,	// Aspect Ratio
											0.1f,							// Near Plane
											1000.0f);						// Far Plane

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
			gShader.m_pUniform_WVPMatrix = &gMatTransformation;
		}

		virtual void onUpdate(uint32_t iDeltaTimeMs, uint64_t lElapsedTime)
		{
			InputManager::get()->getMousePos(m_fMouseX, m_fMouseY);

			// Rotate Triangle
			{
				gfRotCounter += (iDeltaTimeMs / 1000.0f);

				matTranslation = matTranslation.initTranslation(0.0f, 0.0f, 2.0f);
				matRotation = matRotation.initRotation(0, gfRotCounter, 0);
				matWVP = gMatProjection.mul(matTranslation.mul(matRotation));

				gMatTransformation = matWVP;
			}
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