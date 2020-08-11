#pragma once
#include "3D/Vector4f.h"
#include "Common/Pixel.h"
#include "3D/Matrix4f.h"

// Utility defines
#define ATTRIBUTE(__type__, __varName__)	__type__* __varName__;
#define VARYING(__type__, __varName__)		__type__* __varName__;
#define UNIFORM(__type__, __varName__)		__type__* __varName__;

#define GET(__variable__)	(*__variable__)
#define SET(__variable__)	(*__variable__)

#define ATTRIBUTE_LIST(__attributeList__)	__attributeList__
#define VARYINGS_LIST(__varyingsList__)		__varyingsList__

#define COMPILE(__attributes__, __varyings__) \
	virtual void compile() \
	{ \
		initVaryings __varyings__; \
	} \
	\
	virtual void setupPointers() \
	{ \
		assignAttributePointers __attributes__; \
		assignVaryingPointers __varyings__; \
	} \

class IShader
{
	public:
		virtual			~IShader() {}

		virtual void	preVertex(void* pBuffer, void* pVaryings)	// Called before the call to vertex()
		{
			m_pVertexBuffer = pBuffer;
			m_pVertexVaryings = (float*)pVaryings;

			setupPointers();
		}

		virtual void	preFragment(void* pVaryings)				// Called before the call to fragment()
		{
			m_pVertexVaryings = (float*)pVaryings;

			setupPointers();
		}

		virtual void	compile() = 0;

		virtual void	vertex() = 0;				// The main goal of the 'Vertex Shader' is to transform the coordinates of the vertices to screen coordinates.
													// Secondary goal is to prepare data for the fragment shader.
		virtual bool	fragment() = 0;				// The main goal of the 'Fragment Shader' is to determine the colour of the current pixel.
													// Secondary goal, discard the current pixel by returning false.

		virtual void	setupPointers() = 0;		// Called by the Base class to setup data pointers

		Vector4f		gl_Position;				// Set by the 'Vertex Shader'
		Pixel			gl_Colour;					// Set by the 'Fragment Shader'

		void*			m_pVertexBuffer = nullptr;	// Pointer that holds the vertex data.
		float*			m_pVertexVaryings = nullptr;// Pointer thats created depending on the number of varyings.
		int8_t			m_iVaryingsByteSize;		// Holds the number of varyings count.
	protected:
		// Utility functions...
		template<typename U>
		size_t			SIZEOF_VARIABLE(U** __lastVar__)
		{
			return sizeof(U);
		}

		template<typename U, typename... T>
		size_t			SIZEOF_VARIABLE(U** __firstVar__, T** ...__restVars__)
		{
			return sizeof(U) + SIZEOF_VARIABLE(__restVars__...);
		}

		template<typename U>
		void			ASSIGN_POINTERS(void* pBuffer, U** __lastVar__)
		{
			*__lastVar__ = (U*)pBuffer;
		}

		template<typename U, typename... T>
		void			ASSIGN_POINTERS(void* pBuffer, U** __firstVar__, T** ...__restVars__)
		{
			*__firstVar__ = (U*)pBuffer;

			int8_t* pTempBuff = ((int8_t*)pBuffer);
			pTempBuff += sizeof(U);

			ASSIGN_POINTERS(pTempBuff, __restVars__...);
		}

		template<typename... T>
		void			initVaryings(T* ...__VaryingsVars__)
		{
			m_iVaryingsByteSize = SIZEOF_VARIABLE(__VaryingsVars__...);
		}

		template<typename... T>
		void			assignAttributePointers(T* ...__AttributeVars__)
		{
			ASSIGN_POINTERS(m_pVertexBuffer, __AttributeVars__...);
		}

		template<typename... T>
		void			assignVaryingPointers(T* ...__VaryingsVars__)
		{
			ASSIGN_POINTERS(m_pVertexVaryings, __VaryingsVars__...);
		}
	private:
};

class BasicShader : public IShader
{
	public:
		COMPILE( ATTRIBUTE_LIST((&m_pAttribute_Position)), VARYINGS_LIST((&m_pVarying_Position)) )

		virtual void	vertex()
		{
			// POSITION
			{
				// Vertex Model Position
				Vector4f vPosition = m_pUniform_WVPMatrix->transform(GET(m_pAttribute_Position));	// Transform from Model -> World -> Screen 
				gl_Position = vPosition;															// Final Screen Position

				SET(m_pVarying_Position) = vPosition;
			}
		}

		virtual bool	fragment()
		{
			gl_Colour = Vector4f(255, 255, 0, 255);
			return true;
		}

		ATTRIBUTE(Vector4f, m_pAttribute_Position)

		UNIFORM(Matrix4f, m_pUniform_WVPMatrix)

		VARYING(Vector4f, m_pVarying_Position)
	protected:
	private:
};

class VertexColourShader : public IShader
{
	public:
		COMPILE( ATTRIBUTE_LIST((&m_pAttribute_Position, &m_pAttribute_Colour)), VARYINGS_LIST((&m_pVarying_Colour)) )

		virtual void		vertex()
		{
			// POSITION
			{
				// Vertex Model Position
				Vector4f vPosition = m_pUniform_WVPMatrix->transform(GET(m_pAttribute_Position));	// Transform from Model -> World -> Screen 
				gl_Position = vPosition;															// Final Screen Position
			}

			// COLOUR
			{
				Vector4f vColour = GET(m_pAttribute_Colour) * 255;
				vColour.w = 255;

				SET(m_pVarying_Colour) = vColour;
			}
		}

		virtual bool		fragment()
		{
			gl_Colour = GET(m_pVarying_Colour);
			return true;
		}

		ATTRIBUTE(Vector4f, m_pAttribute_Position)
		ATTRIBUTE(Vector4f, m_pAttribute_Colour)

		UNIFORM(Matrix4f, m_pUniform_WVPMatrix)

		VARYING(Vector4f, m_pVarying_Colour)
	protected:
	private:
};