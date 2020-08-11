#pragma once
#include "3D/Vector4f.h"

class Vertex
{
	public:
		Vector4f		m_vPosition;
		Vector4f		m_vColour;

		Vertex() {}

		Vertex(Vector4f vPosition)
		{
			m_vPosition = vPosition;
		}

		Vertex(Vector4f vPosition, Vector4f vColour)
		{
			m_vPosition = vPosition;
			m_vColour = vColour;
		}
};