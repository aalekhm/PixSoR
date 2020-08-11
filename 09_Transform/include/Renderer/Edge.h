#pragma once
#include "Common/Defines.h"
#include "3D/Vector4f.h"
#include "3D/Vertex.h"

enum class EVaryingsInternal
{
	X_VARYING,
	Y_VARYING,
	MAX
};

class Edge
{
	public:
		friend class Graphics;

		void		set(VertexInternal& vStart, VertexInternal& vEnd);
		void		step();
	protected:
	private:
		float		m_fYStep;					// Value when we step for each 'y'
		float		m_fYLerpAmount;				// The amount by which the value steps.

		// EVaryingsInternal for internal calculations.
		float		m_pInternalVaryings[(int32_t)EVaryingsInternal::MAX];
		float		m_pInternalVaryingsStart[(int32_t)EVaryingsInternal::MAX];
		float		m_pInternalVaryingsEnd[(int32_t)EVaryingsInternal::MAX];

		// Each Vertex will have attribute that will vary(interpolate) from one Vertex to the other.
		float		m_pVertexVaryings[MAX_VERTEX_VARYINGS];
		float		m_pVertexVaryingsStart[MAX_VERTEX_VARYINGS];
		float		m_pVertexVaryingsEnd[MAX_VERTEX_VARYINGS];

		int32_t		m_pVertexVaryingsCount;
};