#include "Renderer/Edge.h"

void Edge::set(Vector4f& vStart, Vector4f& vEnd)
{
	m_pVertexPositionStart = vStart;
	m_pVertexPositionStart.x = (int32_t)(m_pVertexPositionStart.x + 0.5f); // Rounding off to the next integer value

	m_pVertexPositionEnd = vEnd;
	m_pVertexPositionEnd.x = (int32_t)(m_pVertexPositionEnd.x + 0.5f);		// Rounding off to the next integer value

	m_fYStep = 1.0f / (float)(m_pVertexPositionEnd.y - m_pVertexPositionStart.y);
	m_fYLerpAmount = 0.0f;

	// Linearly Interpolate the Varying from the 'Start' to 'End' by the 'Lerp Amount'
	m_pVertexPositionVarying = LERP(m_pVertexPositionStart, m_pVertexPositionEnd, m_fYLerpAmount);
	m_pVertexPositionVarying.x = (int32_t)(m_pVertexPositionVarying.x + 0.5f);	// Rounding off to the next integer value
}

void Edge::step()
{
	m_fYLerpAmount += m_fYStep;

	// CLAMP the variable within 'Min' & 'Max' limits.
	CLAMP(0.0f, m_fYLerpAmount, 1.0f);

	// Linearly Interpolate the Varying from the 'Start' to 'End' by the 'Lerp Amount'
	m_pVertexPositionVarying = LERP(m_pVertexPositionStart, m_pVertexPositionEnd, m_fYLerpAmount);
	m_pVertexPositionVarying.x = (int32_t)(m_pVertexPositionVarying.x + 0.5f);	// Rounding off to the next integer value
}
