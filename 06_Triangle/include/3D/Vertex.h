#pragma once
#include "3D/Vector4f.h"

class Vertex
{
	public:
		Vector4f		m_vPosition;

		Vertex() {}
		Vertex(Vector4f vPosition)
		{
			m_vPosition = vPosition;
		}
};