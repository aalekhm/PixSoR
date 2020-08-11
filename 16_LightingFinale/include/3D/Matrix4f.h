#pragma once
#include "Vector4f.h"

class Matrix4f
{
	public:
						Matrix4f();
		Matrix4f		initIdentity();
		Matrix4f		initScreenSpaceTransform(float halfWidth, float halfHeight);
		Matrix4f		initTranslation(float x, float y, float z);
		Matrix4f		initRotation(float x, float y, float z, float angle);
		Matrix4f		initRotation(float x, float y, float z);
		Matrix4f		initScale(float x, float y, float z);
		Matrix4f		initPerspective(float fov, float aspectRatio, float zNear, float zFar);
		void			setPerspectiveFrustum(float l, float r, float b, float t, float n, float f);
		Matrix4f		initOrthographic(float left, float right, float bottom, float top, float near, float far);
		Matrix4f		initRotation(Vector4f forward, Vector4f up);
		Matrix4f		initRotation(Vector4f forward, Vector4f up, Vector4f right);
		Vector4f		transform(Vector4f r);
		Matrix4f		mul(Matrix4f r);
		float*			getM();
		float			get(int x, int y);
		void			setM(float* m);
		void			set(int x, int y, float value);
	protected:
	private:
		float			m[16];
};