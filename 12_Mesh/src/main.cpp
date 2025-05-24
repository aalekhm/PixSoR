#include <stdlib.h>
#include "Engine/GameEngine.h"
#include "Engine/InputManager.h"
#include "Renderer/OpenGL.h"
#include "Renderer/IShader.h"
#include "3D/Matrix4f.h"
#include "3D/Mesh.h"

#define THE_2_TRIANGLES		0

Matrix4f				gMatProjection;

#if THE_2_TRIANGLES
VertexTextureShader		gShader;
GLuint					VBO_0;
GLuint					TEX_BUFFER[2];

Matrix4f				gMatTransformation;
float					gfRotCounter;
Matrix4f				matTranslation;
Matrix4f				matRotation;
Matrix4f				matWVP;
#else
std::unique_ptr<Mesh>	gMesh;
#endif

class MyEngine : public GameEngine
{
	public:
		MyEngine()
		{
			createWindow(480, 320, "12_Mesh");
		}

		virtual void onCreate()
		{
			OpenGL::m_pGraphics = getGraphics();
			gMatProjection.initPerspective(	(float)DEG_2_RAD(70.0f),		// 'F'ield 'O'f 'V'iew
											(float)m_iWidth / m_iHeight,	// Aspect Ratio
											0.1f,							// Near Plane
											1000.0f);						// Far Plane

#if THE_2_TRIANGLES
			Vertex pVertices[6];
			{
								// Position									// Texture Coordinates
				pVertices[0]	= Vertex( Vector4f(-1,	1,	0, 1),	Vector4f(0.0f, 0.0f, 0.0f, 0.0f) );
				pVertices[1]	= Vertex( Vector4f(-1,	-1,	0, 1),	Vector4f(0.0f, 1.0f, 0.0f, 0.0f) );
				pVertices[2]	= Vertex( Vector4f(1,	-1,	0, 1),	Vector4f(1.0f, 1.0f, 0.0f, 0.0f) );

				pVertices[3]	= Vertex( Vector4f(-1,	1,	0, 1),	Vector4f(0.0f, 0.0f, 0.0f, 0.0f) );
				pVertices[4]	= Vertex( Vector4f(1,	1,	0, 1),	Vector4f(1.0f, 0.0f, 0.0f, 0.0f) );
				pVertices[5]	= Vertex( Vector4f(1,	-1,	0, 1),	Vector4f(1.0f, 1.0f, 0.0f, 0.0f) );

				OpenGL::glGenBuffers(1, &VBO_0);
				OpenGL::glBindBuffer(GL_ARRAY_BUFFER, VBO_0);
				OpenGL::glBufferData(&pVertices, sizeof(pVertices), sizeof(Vertex) / sizeof(float));

				std::unique_ptr<Sprite> pTexture;
				{
					OpenGL::glGenTextures(2, TEX_BUFFER);

					pTexture = Sprite::create("../Content/Cartoon.tga");
					OpenGL::glBindTexture(GL_TEXTURE_2D, TEX_BUFFER[0]);
					OpenGL::glTexImage2D(GL_TEXTURE_2D, pTexture);

					pTexture = Sprite::create("../Content/X-Men.tga");
					OpenGL::glBindTexture(GL_TEXTURE_2D, TEX_BUFFER[1]);
					OpenGL::glTexImage2D(GL_TEXTURE_2D, pTexture);
				}
			}
			gShader.compile();
			gShader.m_pUniform_WVPMatrix = &gMatTransformation;
#else
			gMesh = Mesh::create("../Content/smoothMonkey0.obj", "../Content/bricks2.tga");
			gMesh->setProjection(gMatProjection);
			gMesh->setPosition(Vector4f(0.0f, 0.0f, 5.0f, 0.0f));
			gMesh->setScale(Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
			gMesh->setShader( std::make_unique<VertexTextureShader>() );
			{
				IShader* pShader = gMesh->getShader();
				((VertexTextureShader*)pShader)->m_pUniform_WVPMatrix = &gMesh->getTransformation();
			}
#endif
		}

		virtual void onUpdate(uint32_t iDeltaTimeMs, uint64_t lElapsedTime)
		{
			InputManager::get()->getMousePos(m_fMouseX, m_fMouseY);

#if THE_2_TRIANGLES
			// Rotate Triangle
			{
				gfRotCounter += (iDeltaTimeMs / 1000.0f);

				matTranslation = matTranslation.initTranslation(0.0f, 0.0f, 3.0f);
				matRotation = matRotation.initRotation(0, gfRotCounter, 0);
				matWVP = gMatProjection.mul(matTranslation.mul(matRotation));

				gMatTransformation = matWVP;
			}
#else
			gMesh->rotateY(iDeltaTimeMs * 0.1f);
			gMesh->update(iDeltaTimeMs);
#endif
		}

		virtual void onPaint(Graphics* pGraphics)
		{
			pGraphics->clear();

#if THE_2_TRIANGLES
			OpenGL::useProgram(&gShader);
			OpenGL::glBindTexture(GL_TEXTURE_2D, TEX_BUFFER[0]);
			OpenGL::glBindBuffer(GL_ARRAY_BUFFER, VBO_0);
			OpenGL::glDrawArrays(GL_TRIANGLES, 0, 3);

			OpenGL::glBindTexture(GL_TEXTURE_2D, TEX_BUFFER[1]);
			OpenGL::glBindBuffer(GL_ARRAY_BUFFER, VBO_0);
			OpenGL::glDrawArrays(GL_TRIANGLES, 3, 3);
#else
			gMesh->render(pGraphics);
#endif
		}

		virtual void onDestroy()
		{
#if THE_2_TRIANGLES
			OpenGL::glDeleteBuffers(1, &VBO_0);
			OpenGL::glDeleteTextures(2, TEX_BUFFER);
#else
			gMesh = nullptr;
#endif
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