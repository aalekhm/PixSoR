#include "3D/Mesh.h"
#include "Renderer/OpenGL.h"
#include "Common/Sprite.h"

#define MAX_ATTRIBUTE_COUNT 3

Mesh::Mesh(_cookie)
	: m_MatProjection(nullptr)
	, m_pShader(nullptr)
	, m_pMeshObjLoader(nullptr)
	, m_iNumVertices(0)
	, m_iVBO(-1)
	, m_iTexBuf(-1)
{
}

std::unique_ptr<Mesh> Mesh::create(const char* sFileName, const char* sTextureFileName)
{
	std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>(_cookie{});
	if (pMesh != nullptr)
	{
		pMesh->m_pMeshObjLoader = std::make_unique<MeshObjLoader>();
		if (pMesh->m_pMeshObjLoader != nullptr)
		{
			pMesh->m_pMeshObjLoader->loadObject(sFileName);
			pMesh->loadMesh();
			pMesh->loadTexture(sTextureFileName);
		}
	}

	return pMesh;
}

void Mesh::loadMesh()
{
	for (int i = 0; i < m_pMeshObjLoader->getNumFaces(); i++)
	{
		SFace* pFace = m_pMeshObjLoader->getFace(i);
		{
			SVector3* sV1Pos		= m_pMeshObjLoader->getVertex(pFace->vertexIndex[0] - 1);
			SVector2* sV1TexCoord	= m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[0] - 1);
			SVector3* sV1Normal		= m_pMeshObjLoader->getNormal(pFace->vertexNormalIndex[0] - 1);
			{
				m_Vertices.push_back(Vector4f(sV1Pos->x,		sV1Pos->y,		sV1Pos->z,		1.0f));
				m_Vertices.push_back(Vector4f(sV1TexCoord->x,	sV1TexCoord->y, 0.0f,			0.0f));
				m_Vertices.push_back(Vector4f(sV1Normal->x,		sV1Normal->y,	sV1Normal->z,	0.0f));
			}

			SVector3* sV2Pos		= m_pMeshObjLoader->getVertex(pFace->vertexIndex[1] - 1);
			SVector2* sV2TexCoord	= m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[1] - 1);
			SVector3* sV2Normal		= m_pMeshObjLoader->getNormal(pFace->vertexNormalIndex[1] - 1);
			{
				m_Vertices.push_back(Vector4f(sV2Pos->x,		sV2Pos->y,		sV2Pos->z,		1.0f));
				m_Vertices.push_back(Vector4f(sV2TexCoord->x,	sV2TexCoord->y, 0.0f,			0.0f));
				m_Vertices.push_back(Vector4f(sV2Normal->x,		sV2Normal->y,	sV2Normal->z,	0.0f));
			}

			SVector3* sV3Pos		= m_pMeshObjLoader->getVertex(pFace->vertexIndex[2] - 1);
			SVector2* sV3TexCoord	= m_pMeshObjLoader->getTexCoord(pFace->texCoordIndex[2] - 1);
			SVector3* sV3Normal		= m_pMeshObjLoader->getNormal(pFace->vertexNormalIndex[2] - 1);
			{
				m_Vertices.push_back(Vector4f(sV3Pos->x,		sV3Pos->y,		sV3Pos->z,		1.0f));
				m_Vertices.push_back(Vector4f(sV3TexCoord->x,	sV3TexCoord->y, 0.0f,			0.0f));
				m_Vertices.push_back(Vector4f(sV3Normal->x,		sV3Normal->y,	sV3Normal->z,	0.0f));
			}
		}
	}

	m_iNumVertices = m_Vertices.size() / MAX_ATTRIBUTE_COUNT;

	OpenGL::glGenBuffers(1, &m_iVBO);
	OpenGL::glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	OpenGL::glBufferData(	m_Vertices.data(),						// Pointer to the data.
							m_Vertices.size() * sizeof(Vector4f),	// Maximum number of bytes.
							MAX_ATTRIBUTE_COUNT * sizeof(float));	// Stride to the next Vertice(remember its a float*)
}

void Mesh::loadTexture(const char* sTextureFileName)
{
	std::unique_ptr<Sprite> pTexture = Sprite::create(sTextureFileName);
	if (pTexture != nullptr)
	{
		OpenGL::glGenTextures(1, &m_iTexBuf);
		OpenGL::glBindTexture(GL_TEXTURE_2D, m_iTexBuf);
		OpenGL::glTexImage2D(GL_TEXTURE_2D, pTexture);
	}
}

void Mesh::setProjection(Matrix4f& pMatProjection)
{
	m_MatProjection = &pMatProjection;
}

void Mesh::setPosition(Vector4f vPos)
{
	m_Transform.setPos(vPos);
}

void Mesh::setScale(Vector4f vScale)
{
	m_Transform.setScale(vScale);
}

void Mesh::rotateX(float fAngleInDegrees)
{
	m_Transform.rotate(Quaternion(X_AXIS, DEG_2_RAD(fAngleInDegrees)));
}

void Mesh::rotateY(float fAngleInDegrees)
{
	m_Transform.rotate(Quaternion(Y_AXIS, DEG_2_RAD(fAngleInDegrees)));
}

void Mesh::rotateZ(float fAngleInDegrees)
{
	m_Transform.rotate(Quaternion(Z_AXIS, DEG_2_RAD(fAngleInDegrees)));
}

void Mesh::setPositionX(float fValue)
{
	m_Transform.setPosX(fValue);
}

void Mesh::setPositionY(float fValue)
{
	m_Transform.setPosY(fValue);
}

void Mesh::setPositionZ(float fValue)
{
	m_Transform.setPosZ(fValue);
}

void Mesh::translateX(float fValue)
{
	m_Transform.translateX(fValue);
}

void Mesh::translateY(float fValue)
{
	m_Transform.translateY(fValue);
}

void Mesh::translateZ(float fValue)
{
	m_Transform.translateZ(fValue);
}

void Mesh::update(uint32_t iDeltaTimeMs)
{
	Matrix4f viewProjectionMatrix = OpenGL::m_pCamera->getViewProjection();

	m_MatTransformation = viewProjectionMatrix.mul(m_Transform.getTransformation());
}

void Mesh::render(Graphics* pGraphics)
{
	OpenGL::glBindTexture(GL_TEXTURE_2D, m_iTexBuf);
	OpenGL::glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	OpenGL::useProgram(m_pShader.get());

	OpenGL::glDrawArrays(GL_TRIANGLES, 0, m_iNumVertices);
}

void Mesh::setShader(std::unique_ptr<IShader> pShaderProgram)
{
	if (pShaderProgram != nullptr)
	{
		m_pShader.release();

		m_pShader = std::move(pShaderProgram);
		m_pShader->compile();
	}
}

IShader* Mesh::getShader()
{
	return m_pShader.get();
}

Matrix4f& Mesh::getTransformation()
{
	return m_MatTransformation;
}

Mesh::~Mesh()
{
	OpenGL::glDeleteBuffers(1, &m_iVBO);
	OpenGL::glDeleteTextures(1, &m_iTexBuf);

	if (m_pMeshObjLoader != nullptr)
	{
		m_pMeshObjLoader.release();
	}

	if (m_pShader!= nullptr)
	{
		m_pShader.release();
	}
}
