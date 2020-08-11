#pragma once
#include "Common/Defines.h"
#include "3D/Vector4f.h"
#include "3D/Vertex.h"

class Edge
{
	public:
		void		set(Vector4f& vStart, Vector4f& vEnd);
		void		step();

		Vector4f	m_pVertexPositionStart;		// Start position of the Edge
		Vector4f	m_pVertexPositionEnd;		// End position of the Edge

		Vector4f	m_pVertexPositionVarying;	// Varying position across the Edge when we step each 'y'
	protected:
	private:
		float		m_fYStep;					// Value when we step for each 'y'
		float		m_fYLerpAmount;				// The amount by which the value steps.
};