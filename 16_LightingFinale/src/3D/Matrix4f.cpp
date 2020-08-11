#include "3D/Matrix4f.h"

Matrix4f::Matrix4f()
{
	initIdentity();
}

Matrix4f Matrix4f::initIdentity()
{
	m[0] = 1;	m[1] = 0;	m[2] = 0;	m[3] = 0;
	m[4] = 0;	m[5] = 1;	m[6] = 0;	m[7] = 0;
	m[8] = 0;	m[9] = 0;	m[10] = 1;	m[11] = 0;
	m[12] = 0;	m[13] = 0;	m[14] = 0;	m[15] = 1;

	return *this;
}

Matrix4f Matrix4f::initScreenSpaceTransform(float halfWidth, float halfHeight)
{
	m[0] = halfWidth;	m[1] = 0;			m[2] = 0;	m[3] = halfWidth;
	m[4] = 0;			m[5] = -halfHeight;	m[6] = 0;	m[7] = halfHeight;
	m[8] = 0;			m[9] = 0;			m[10] = 1;	m[11] = 0;
	m[12] = 0;			m[13] = 0;			m[14] = 0;	m[15] = 1;

	return *this;
}

Matrix4f Matrix4f::initTranslation(float x, float y, float z)
{
	m[0] = 1;	m[1] = 0;	m[2] = 0;	m[3] = x;
	m[4] = 0;	m[5] = 1;	m[6] = 0;	m[7] = y;
	m[8] = 0;	m[9] = 0;	m[10] = 1;	m[11] = z;
	m[12] = 0;	m[13] = 0;	m[14] = 0;	m[15] = 1;

	return *this;
}

Matrix4f Matrix4f::initRotation(float x, float y, float z, float angle)
{
	float _sin = (float)sin(angle);
	float _cos = (float)cos(angle);

	m[0] = _cos + x*x*(1 - _cos); 		m[1] = x*y*(1 - _cos) - z*_sin; 	m[2] = x*z*(1 - _cos) + y*_sin; 	m[3] = 0;
	m[4] = y*x*(1 - _cos) + z*_sin; 	m[5] = _cos + y*y*(1 - _cos);		m[6] = y*z*(1 - _cos) - x*_sin; 	m[7] = 0;
	m[8] = z*x*(1 - _cos) - y*_sin; 	m[9] = z*y*(1 - _cos) + x*_sin; 	m[10] = _cos + z*z*(1 - _cos); 		m[11] = 0;
	m[12] = 0;							m[13] = 0;							m[14] = 0;							m[15] = 1;

	return *this;
}

Matrix4f Matrix4f::initRotation(float x, float y, float z)
{
	Matrix4f rx;
	Matrix4f ry;
	Matrix4f rz;

	rz.m[0] = (float)cos(z);		rz.m[1] = -(float)sin(z);			rz.m[2] = 0;						rz.m[3] = 0;
	rz.m[4] = (float)sin(z);		rz.m[5] = (float)cos(z);			rz.m[6] = 0;						rz.m[7] = 0;
	rz.m[8] = 0;					rz.m[9] = 0;						rz.m[10] = 1;						rz.m[11] = 0;
	rz.m[12] = 0;					rz.m[13] = 0;						rz.m[14] = 0;						rz.m[15] = 1;

	rx.m[0] = 1;					rx.m[1] = 0;						rx.m[2] = 0;						rx.m[3] = 0;
	rx.m[4] = 0;					rx.m[5] = (float)cos(x);			rx.m[6] = -(float)sin(x);			rx.m[7] = 0;
	rx.m[8] = 0;					rx.m[9] = (float)sin(x);			rx.m[10] = (float)cos(x);			rx.m[11] = 0;
	rx.m[12] = 0;					rx.m[13] = 0;						rx.m[14] = 0;						rx.m[15] = 1;

	ry.m[0] = (float)cos(y);		ry.m[1] = 0;						ry.m[2] = -(float)sin(y);			ry.m[3] = 0;
	ry.m[4] = 0;					ry.m[5] = 1;						ry.m[6] = 0;						ry.m[7] = 0;
	ry.m[8] = (float)sin(y);		ry.m[9] = 0;						ry.m[10] = (float)cos(y);			ry.m[11] = 0;
	ry.m[12] = 0;					ry.m[13] = 0;						ry.m[14] = 0;						ry.m[15] = 1;

	memcpy(m, rz.mul(ry.mul(rx)).getM(), sizeof(float) * 16);

	return *this;
}

Matrix4f Matrix4f::initScale(float x, float y, float z)
{
	m[0] = x;	m[1] = 0;	m[2] = 0;	m[3] = 0;
	m[4] = 0;	m[5] = y;	m[6] = 0;	m[7] = 0;
	m[8] = 0;	m[9] = 0;	m[10] = z;	m[11] = 0;
	m[12] = 0;	m[13] = 0;	m[14] = 0;	m[15] = 1;

	return *this;
}

Matrix4f Matrix4f::initPerspective(float fov, float aspectRatio, float zNear, float zFar)
{
	// 0.
	{
		float tanHalfFOV = (float)tan(fov / 2);
		float zRange = zNear - zFar;

		m[0] = 1.0f / (tanHalfFOV * aspectRatio);	m[1] = 0;					m[2] = 0;							m[3] = 0;
		m[4] = 0;									m[5] = 1.0f / tanHalfFOV;	m[6] = 0;							m[7] = 0;
		m[8] = 0;									m[9] = 0;					m[10] = (-zNear - zFar) / zRange;	m[11] = 2 * zFar * zNear / zRange;
		m[12] = 0;									m[13] = 0;					m[14] = 1;							m[15] = 0;
	}

	// OR

	// 1.
	//{
	//	float tangent	= tanf(DEG2RAD(fov/2));		// tangent of half fovY
	//	float height	= zNear * tangent;          // half height of near plane
	//	float width		= height * aspectRatio;     // half width of near plane
	//
	//	// params: left, right, bottom, top, near, far
	//	setPerspectiveFrustum(-width, width, -height, height, zNear, zFar);
	//}

	return *this;
}

void Matrix4f::setPerspectiveFrustum(float l, float r, float b, float t, float n, float f)
{
	initIdentity();
	m[0] = 2 * n / (r - l);
	m[2] = (r + l) / (r - l);
	m[5] = 2 * n / (t - b);
	m[6] = (t + b) / (t - b);
	m[10] = -(f + n) / (f - n);
	m[11] = -(2 * f * n) / (f - n);
	m[14] = -1;
	m[15] = 0;
}

Matrix4f Matrix4f::initOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	float width = right - left;
	float height = top - bottom;
	float depth = far - near;

	m[0] = 2 / width;	m[1] = 0;			m[2] = 0;				m[3] = -(right + left) / width;
	m[4] = 0;			m[5] = 2 / height;	m[6] = 0;				m[7] = -(top + bottom) / height;
	m[8] = 0;			m[9] = 0;			m[10] = -2 / depth;		m[11] = -(far + near) / depth;
	m[12] = 0;			m[13] = 0;			m[14] = 0;				m[15] = 1;

	return *this;
}

Matrix4f Matrix4f::initRotation(Vector4f forward, Vector4f up)
{
	Vector4f f = forward.normalized();

	Vector4f r = up.normalized();
	r = r.cross(f);

	Vector4f u = f.cross(r);

	initRotation(f, u, r);

	return *this;
}

Matrix4f Matrix4f::initRotation(Vector4f forward, Vector4f up, Vector4f right)
{
	Vector4f f = forward;
	Vector4f r = right;
	Vector4f u = up;

	m[0] = r.getX();	m[1] = r.getY();	m[2] = r.getZ();	m[3] = 0;
	m[4] = u.getX();	m[5] = u.getY();	m[6] = u.getZ();	m[7] = 0;
	m[8] = f.getX();	m[9] = f.getY();	m[10] = f.getZ();	m[11] = 0;
	m[12] = 0;			m[13] = 0;			m[14] = 0;			m[15] = 1;

	return *this;
}

Vector4f Matrix4f::transform(Vector4f r)
{
	return Vector4f(m[0] * r.getX() + m[1] * r.getY() + m[2] * r.getZ() + m[3] * r.getW(),
		m[4] * r.getX() + m[5] * r.getY() + m[6] * r.getZ() + m[7] * r.getW(),
		m[8] * r.getX() + m[9] * r.getY() + m[10] * r.getZ() + m[11] * r.getW(),
		m[12] * r.getX() + m[13] * r.getY() + m[14] * r.getZ() + m[15] * r.getW());
}

Matrix4f Matrix4f::mul(Matrix4f m2)
{
	Matrix4f res;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res.set(i, j, m2.get(i, 0) * get(0, j) +
				m2.get(i, 1) * get(1, j) +
				m2.get(i, 2) * get(2, j) +
				m2.get(i, 3) * get(3, j));
		}
	}

	return res;
}

float* Matrix4f::getM()
{
	return m;
}

float Matrix4f::get(int x, int y)
{
	return m[y * 4 + x];
}

void Matrix4f::setM(float* m)
{
	memcpy(this->m, m, sizeof(float) * 16);
}

void Matrix4f::set(int x, int y, float value)
{
	m[y * 4 + x] = value;
}