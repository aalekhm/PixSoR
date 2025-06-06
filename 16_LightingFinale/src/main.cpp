#include <stdlib.h>
#include "Engine/GameEngine.h"
#include "Engine/InputManager.h"
#include "Renderer/OpenGL.h"
#include "Renderer/IShader.h"
#include "3D/Matrix4f.h"
#include "3D/Mesh.h"
#include "3D/Camera.h"

#define THE_2_TRIANGLES		0

Matrix4f				gMatProjection;
float					gfRotCounter;

#if THE_2_TRIANGLES
VertexTextureShader		gShader;
GLuint					VBO_0;
GLuint					TEX_BUFFER[2];

Matrix4f				gMatTransformation;
Matrix4f				matTranslation;
Matrix4f				matRotation;
Matrix4f				matWVP;
#else
std::unique_ptr<Mesh>	gMesh;
std::unique_ptr<Mesh>	gTerrain;
#endif

class MyEngine : public GameEngine
{
	public:
		MyEngine()
		{
			createWindow(480, 320, "16_LightingFinale");
		}

		virtual void onCreate()
		{
			setClearColourAndDepth(Pixel(0, 0, 0, 0xFF), 1.0f);

			gMatProjection.initPerspective(	(float)DEG_2_RAD(70.0f),		// 'F'ield 'O'f 'V'iew
											(float)m_iWidth / m_iHeight,	// Aspect Ratio
											0.1f,							// Near Plane
											1000.0f);						// Far Plane

			OpenGL::m_pGraphics = getGraphics();
			OpenGL::m_pCamera = Camera::create(gMatProjection);
			OpenGL::m_pLightSource = std::make_unique<Light>();
			{
				OpenGL::m_pLightSource->m_vPosition = Vector4f();
				OpenGL::m_pLightSource->m_vDirection = Vector4f(0, 0, 1, 0);
				OpenGL::m_pLightSource->m_fDirectionIntensity = 0.9f;
				OpenGL::m_pLightSource->m_fAmbientIntensity = 0.2f;
			}
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
			gMesh = Mesh::create("../Content/smoothMonkey2.obj", "../Content/bricks2.tga");
			gMesh->setProjection(gMatProjection);
			gMesh->setPosition(Vector4f(0.0f, 0.0f, 5.0f, 0.0f));
			gMesh->setScale(Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
			gMesh->setShader( std::make_unique<VertexLightShader>() );
			{
				IShader* pShader = gMesh->getShader();
				((VertexLightShader*)pShader)->m_pUniform_WVPMatrix = &gMesh->getTransformation();
				((VertexLightShader*)pShader)->m_pUniform_Light = OpenGL::m_pLightSource.get();
			}

			gTerrain = Mesh::create("../Content/terrain2.obj", "../Content/bricks.tga");
			gTerrain->setProjection(gMatProjection);
			gTerrain->setPosition(Vector4f(0.0f, -1.0f, 0.0f, 0.0f));
			gTerrain->setScale(Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
			gTerrain->setShader( std::make_unique<VertexLightShader>() );
			{
				IShader* pShader = gTerrain->getShader();
				((VertexLightShader*)pShader)->m_pUniform_WVPMatrix = &gTerrain->getTransformation();
				((VertexLightShader*)pShader)->m_pUniform_Light = OpenGL::m_pLightSource.get();
			}
#endif
		}

		virtual void onUpdate(uint32_t iDeltaTimeMs, uint64_t lElapsedTime)
		{
			OpenGL::m_pCamera->update(iDeltaTimeMs);
			InputManager::get()->getMousePos(m_fMouseX, m_fMouseY);

			gfRotCounter += (iDeltaTimeMs / 1000.0f);
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
			gMesh->update(iDeltaTimeMs);
			gTerrain->update(iDeltaTimeMs);
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
			gTerrain->render(pGraphics);
#endif
		}

		virtual void onDestroy()
		{
#if THE_2_TRIANGLES
			OpenGL::glDeleteBuffers(1, &VBO_0);
			OpenGL::glDeleteTextures(2, TEX_BUFFER);
#else
			gMesh = nullptr;
			gTerrain = nullptr;
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