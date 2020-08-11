#include "Renderer/Edge.h"

#define GET_VARYING_START(__varying__)	m_pVertexVaryingsStart[__varying__]
#define GET_VARYING_END(__varying__)	m_pVertexVaryingsEnd[__varying__]
#define GET_VARYING(__varying__)		m_pVertexVaryings[__varying__]

#define SET_VARYING_START(__varying__, __value__)	m_pVertexVaryingsStart[__varying__] = __value__;
#define SET_VARYING_END(__varying__, __value__)		m_pVertexVaryingsEnd[__varying__] = __value__;
#define SET_VARYING(__varying__, __value__)			m_pVertexVaryings[__varying__] = __value__;

#define GET_INTERNAL_VARYING_START(__internalVarying__)		m_pInternalVaryingsStart[(int32_t)__internalVarying__]
#define GET_INTERNAL_VARYING_END(__internalVarying__)		m_pInternalVaryingsEnd[(int32_t)__internalVarying__]

#define SET_INTERNAL_VARYING_START(__internalVarying__, __value__)	m_pInternalVaryingsStart[(int32_t)__internalVarying__] = __value__;
#define SET_INTERNAL_VARYING_END(__internalVarying__, __value__)	m_pInternalVaryingsEnd[(int32_t)__internalVarying__] = __value__;
#define SET_INTERNAL_VARYING(__internalVarying__, __value__)		m_pInternalVaryings[(int32_t)__internalVarying__] = __value__;

void Edge::set(VertexInternal& vVertexStart, VertexInternal& vVertexEnd)
{
	SET_INTERNAL_VARYING_START(EVaryingsInternal::X_VARYING, ((int32_t)(vVertexStart.m_vVertPosition.x + 0.5f)))
	SET_INTERNAL_VARYING_START(EVaryingsInternal::Y_VARYING, ((int32_t)(vVertexStart.m_vVertPosition.y)))

	SET_INTERNAL_VARYING_END(EVaryingsInternal::X_VARYING, ((int32_t)(vVertexEnd.m_vVertPosition.x + 0.5f)))
	SET_INTERNAL_VARYING_END(EVaryingsInternal::Y_VARYING, ((int32_t)(vVertexEnd.m_vVertPosition.y)))

	m_fYStep = 1.0f / (float)(GET_INTERNAL_VARYING_END(EVaryingsInternal::Y_VARYING) - GET_INTERNAL_VARYING_START(EVaryingsInternal::Y_VARYING));
	m_fYLerpAmount = 0.0f;

	// Init Varyings
	{
		// Init Position Varying
		SET_INTERNAL_VARYING(EVaryingsInternal::X_VARYING, (int32_t)(LERP(GET_INTERNAL_VARYING_START(EVaryingsInternal::X_VARYING), GET_INTERNAL_VARYING_END(EVaryingsInternal::X_VARYING), m_fYLerpAmount) + 0.5f))
		SET_INTERNAL_VARYING(EVaryingsInternal::Y_VARYING, (LERP(GET_INTERNAL_VARYING_START(EVaryingsInternal::Y_VARYING), GET_INTERNAL_VARYING_END(EVaryingsInternal::Y_VARYING), m_fYLerpAmount)))

		// Init Varyings - Linearly Interpolate the Varying from the 'Start' to 'End' by the 'Lerp Amount'
		m_pVertexVaryingsCount = vVertexStart.m_iVaryingsCount;
		for (int32_t i = 0; i < m_pVertexVaryingsCount; i++)
		{
			SET_VARYING_START(i, (*(vVertexStart.m_vVertVaryings + i)))
			SET_VARYING_END(i, (*(vVertexEnd.m_vVertVaryings + i)))

			SET_VARYING(i, LERP( GET_VARYING_START(i), GET_VARYING_END(i), m_fYLerpAmount) )
		}
	}
}

void Edge::step()
{
	m_fYLerpAmount += m_fYStep;

	// CLAMP the variable within 'Min' & 'Max' limits.
	CLAMP(0.0f, m_fYLerpAmount, 1.0f);

	// Step Varyings
	{
		// Update Position Varyings
		SET_INTERNAL_VARYING(EVaryingsInternal::X_VARYING, (int32_t)( LERP(GET_INTERNAL_VARYING_START(EVaryingsInternal::X_VARYING), GET_INTERNAL_VARYING_END(EVaryingsInternal::X_VARYING), m_fYLerpAmount) + 0.5f ))
		SET_INTERNAL_VARYING(EVaryingsInternal::Y_VARYING, ( LERP(GET_INTERNAL_VARYING_START(EVaryingsInternal::Y_VARYING), GET_INTERNAL_VARYING_END(EVaryingsInternal::Y_VARYING), m_fYLerpAmount) ))

		// Update Varyings
		for (int32_t i = 0; i < m_pVertexVaryingsCount; i++)
		{
			SET_VARYING(i, LERP( GET_VARYING_START(i), GET_VARYING_END(i), m_fYLerpAmount) )
		}
	}
}
