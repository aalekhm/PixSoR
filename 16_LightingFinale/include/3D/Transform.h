#pragma once
#include "3D/Vector4f.h"
#include "3D/Matrix4f.h"
#include "3D/Quaternion.h"

class Transform
{
	public:
					Transform();
					Transform(Vector4f pos);
					Transform(Vector4f pos, Quaternion rot, Vector4f scale);

		void		setPos(Vector4f pos);
		void		setScale(Vector4f scale);
		void		rotate(Quaternion rotation);

		Quaternion	getLookAtRotation(Vector4f point, Vector4f up);

		Matrix4f	getTransformation();
		Vector4f	getTransformedPos();
		Quaternion	getTransformedRot();

		Vector4f	getPos();
		Vector4f	getScale();
		Quaternion	getRot();

		void		setPosX(float fValue);
		void		setPosY(float fValue);
		void		setPosZ(float fValue);

		void		translateX(float fValue);
		void		translateY(float fValue);
		void		translateZ(float fValue);
	protected:
	private:
		Vector4f	m_pos;
		Vector4f	m_scale;
		Quaternion	m_rot;
};
