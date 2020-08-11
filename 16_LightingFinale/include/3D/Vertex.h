#pragma once
#include "3D/Vector4f.h"

class Vertex
{
	public:
		Vector4f		m_vPosition;
		Vector4f		m_vTexCoord;
		Vector4f		m_vNormal;

		Vertex() {}

		Vertex(Vector4f vPosition)
		{
			m_vPosition = vPosition;
		}

		Vertex(Vector4f vPosition, Vector4f vTexCoord)
		{
			m_vPosition = vPosition;
			m_vTexCoord = vTexCoord;
		}

		Vertex(Vector4f vPosition, Vector4f vTexCoord, Vector4f vNormal)
		{
			m_vPosition = vPosition;
			m_vTexCoord = vTexCoord;
			m_vNormal = vNormal;
		}
};