#pragma once

#include "3D/Vertex.h"
#include <vector>
#include "3D/MeshObjLoader.h"
#include "Renderer/IShader.h"
#include "3D/Transform.h"
#include <memory>

class Mesh
{
	private:
		struct _cookie {};
	public:
										Mesh(_cookie);
		static std::unique_ptr<Mesh>	create(const char* sFileName, const char* sTextureFileName);
		virtual							~Mesh();

		void							setProjection(Matrix4f& pMatProjection);
		void							setPosition(Vector4f vPos);
		void							setScale(Vector4f vScale);

		void							rotateX(float fAngleInDegrees);
		void							rotateY(float fAngleInDegrees);
		void							rotateZ(float fAngleInDegrees);

		void							setPositionX(float fValue);
		void							setPositionY(float fValue);
		void							setPositionZ(float fValue);

		void							translateX(float fValue);
		void							translateY(float fValue);
		void							translateZ(float fValue);

		void							update(uint32_t iDeltaTimeMs);
		void							render(Graphics* pGraphics);

		void							setShader(std::unique_ptr<IShader> pShaderProgram);
		IShader*						getShader();

		Matrix4f&						getTransformation();
	protected:
	private:
		void							loadMesh();
		void							loadTexture(const char* sTextureFileName);

		std::vector<Vector4f>			m_Vertices;
		uint32_t						m_iNumVertices;

		GLuint							m_iVBO;
		GLuint							m_iTexBuf;

		Matrix4f						m_MatTransformation;
		Matrix4f*						m_MatProjection;

		std::unique_ptr<MeshObjLoader>	m_pMeshObjLoader;
		std::unique_ptr<IShader>		m_pShader;

		Vector4f						m_vPosition;
		Vector4f						m_vScale;

		Transform						m_Transform;
};