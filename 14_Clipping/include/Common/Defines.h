#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "3D/Vector4f.h"

#define DEFAULT_WIDTH	480
#define DEFAULT_HEIGHT	320

#define USE_BRESENHAM

enum DURATION_TYPE
{
	nanoseconds = 0,
	microseconds,
	milliseconds,
	seconds,
	minutes,
	hours
};

#define TIMER_START_NEW(__identifier__) \
	Timer __identifier__; \
	TIMER_START(__identifier__); \

#define TIMER_START(__identifier__) __identifier__.start();
#define TIMER_STOP(__identifier__) __identifier__.stop();

#define CLAMP(__min__, __value__, __max__) \
if(__value__ < __min__) __value__ = __min__; \
else if(__value__ > __max__) __value__ = __max__; \

#define LERP(__start__, __end__, __amount__) __start__ + (__end__ - __start__) * __amount__

#define MAX_VERTEX_VARYINGS		32
#define MAX_TEXTURES			4

#define DEG_2_RAD(__angleInDegrees__) (__angleInDegrees__ * (3.142f / 180.0f))

#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; }}
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] p; p = nullptr; }}

#define X_AXIS		Vector4f(1, 0, 0)
#define Y_AXIS		Vector4f(0, 1, 0)
#define Z_AXIS		Vector4f(0, 0, 1)

enum EAXIS_COMPONENT
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_W
};

enum ECLIP_STATUS
{
	DISCARD = 0,
	ONE_INSIDE = 1,
	TWO_INSIDE,
	THREE_INSIDE
};

// Vertex structure for internal purposes
struct VertexInternal
{
	Vector4f			m_vVertPosition;
	float				m_vVertVaryings[MAX_VERTEX_VARYINGS];
	int32_t				m_iVaryingsCount;

	VertexInternal		lerp(VertexInternal& vOther, float fLerpAmount)
	{
		VertexInternal vResult;

		vResult.m_vVertPosition = m_vVertPosition.lerp(vOther.m_vVertPosition, fLerpAmount);
		for (int32_t i = 0; i < m_iVaryingsCount; i++)
		{
			vResult.m_vVertVaryings[i] = LERP(m_vVertVaryings[i], vOther.m_vVertVaryings[i], fLerpAmount);
		}

		vResult.m_iVaryingsCount = m_iVaryingsCount;
		return vResult;
	}

	bool				isInsideViewFrustum()
	{
		return (	abs(m_vVertPosition.x) <= abs(m_vVertPosition.w)
					&&
					abs(m_vVertPosition.y) <= abs(m_vVertPosition.w)
					&&
					abs(m_vVertPosition.z) <= abs(m_vVertPosition.w)
			);
	}

	float				get(EAXIS_COMPONENT eIndex)
	{
		switch (eIndex)
		{
			case EAXIS_COMPONENT::AXIS_X:	return m_vVertPosition.x;
			case EAXIS_COMPONENT::AXIS_Y:	return m_vVertPosition.y;
			case EAXIS_COMPONENT::AXIS_Z:	return m_vVertPosition.z;
			case EAXIS_COMPONENT::AXIS_W:	return m_vVertPosition.w;
		}
	}
};

struct BufferDetails
{
	uint32_t		m_iSize;
	uint32_t		m_iStride;
	void*			m_pBufferPointer;
};