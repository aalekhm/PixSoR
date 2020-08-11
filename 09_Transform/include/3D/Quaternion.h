#pragma once
#include "Vector4f.h"
#include "Matrix4f.h"

class Quaternion
{
	public:
							Quaternion();
							Quaternion(float x, float y, float z, float w);
							Quaternion(Vector4f axis, float angle);
							//From Ken Shoemake's "Quaternion Calculus and Fast Animation" article
							Quaternion(Matrix4f rot);

		float				length();
		Quaternion			normalized();
		Quaternion			conjugate();
		Quaternion			mul(float r);
		Quaternion			mul(Quaternion r);
		Quaternion			mul(Vector4f r);
		Quaternion			sub(Quaternion r);
		Quaternion			add(Quaternion r);
		Matrix4f			toRotationMatrix();
		float				dot(Quaternion r);
		Quaternion			nLerp(Quaternion dest, float lerpFactor, bool shortest);
		Quaternion			sLerp(Quaternion dest, float lerpFactor, bool shortest);

		Vector4f			getForward();
		Vector4f			getBack();
		Vector4f			getUp();
		Vector4f			getDown();
		Vector4f			getRight();
		Vector4f			getLeft();
		float				getX();
		float				getY();
		float				getZ();
		float				getW();
		bool				equals(Quaternion r);
	protected:
	private:
		float				m_x;
		float				m_y;
		float				m_z;
		float				m_w;
};
