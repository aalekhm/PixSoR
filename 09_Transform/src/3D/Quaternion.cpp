#include "3D/Quaternion.h"

Quaternion::Quaternion()
{
	m_x = 0;
	m_y = 0;
	m_z = 0;
	m_w = 1;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

Quaternion::Quaternion(Vector4f axis, float angle)
{
	float sinHalfAngle = (float)sin(angle / 2);
	float cosHalfAngle = (float)cos(angle / 2);

	m_x = axis.getX() * sinHalfAngle;
	m_y = axis.getY() * sinHalfAngle;
	m_z = axis.getZ() * sinHalfAngle;
	m_w = cosHalfAngle;
}

float Quaternion::length()
{
	return (float)sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

Quaternion Quaternion::normalized()
{
	float fLength = length();

	return Quaternion(m_x / fLength, m_y / fLength, m_z / fLength, m_w / fLength);
}

Quaternion Quaternion::conjugate()
{
	return Quaternion(-m_x, -m_y, -m_z, m_w);
}

Quaternion Quaternion::mul(float r)
{
	return Quaternion(m_x * r, m_y * r, m_z * r, m_w * r);
}

Quaternion Quaternion::mul(Quaternion r)
{
	float w_ = m_w * r.getW() - m_x * r.getX() - m_y * r.getY() - m_z * r.getZ();
	float x_ = m_x * r.getW() + m_w * r.getX() + m_y * r.getZ() - m_z * r.getY();
	float y_ = m_y * r.getW() + m_w * r.getY() + m_z * r.getX() - m_x * r.getZ();
	float z_ = m_z * r.getW() + m_w * r.getZ() + m_x * r.getY() - m_y * r.getX();

	return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::mul(Vector4f r)
{
	float w_ = -m_x * r.getX() - m_y * r.getY() - m_z * r.getZ();
	float x_ = m_w * r.getX() + m_y * r.getZ() - m_z * r.getY();
	float y_ = m_w * r.getY() + m_z * r.getX() - m_x * r.getZ();
	float z_ = m_w * r.getZ() + m_x * r.getY() - m_y * r.getX();

	return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::sub(Quaternion r)
{
	return Quaternion(m_x - r.getX(),
		m_y - r.getY(),
		m_z - r.getZ(),
		m_w - r.getW());
}

Quaternion Quaternion::add(Quaternion r)
{
	return Quaternion(m_x + r.getX(),
		m_y + r.getY(),
		m_z + r.getZ(),
		m_w + r.getW());
}

Matrix4f Quaternion::toRotationMatrix()
{
	Vector4f forward(2.0f * (m_x * m_z - m_w * m_y),
		2.0f * (m_y * m_z + m_w * m_x),
		1.0f - 2.0f * (m_x * m_x + m_y * m_y));

	Vector4f up(2.0f * (m_x * m_y + m_w * m_z),
		1.0f - 2.0f * (m_x * m_x + m_z * m_z),
		2.0f * (m_y * m_z - m_w * m_x));

	Vector4f right(1.0f - 2.0f * (m_y * m_y + m_z * m_z),
		2.0f * (m_x * m_y - m_w * m_z),
		2.0f * (m_x * m_z + m_w * m_y));

	return Matrix4f().initRotation(forward, up, right);
}

float Quaternion::dot(Quaternion r)
{
	return m_x * r.getX() + m_y * r.getY() + m_z * r.getZ() + m_w * r.getW();
}

Quaternion Quaternion::nLerp(Quaternion dest, float lerpFactor, bool shortest)
{
	Quaternion correctedDest = dest;

	if (shortest && dot(dest) < 0)
		correctedDest = Quaternion(-dest.getX(), -dest.getY(), -dest.getZ(), -dest.getW());

	return correctedDest.sub(*this).mul(lerpFactor).add(*this).normalized();
}

Quaternion Quaternion::sLerp(Quaternion dest, float lerpFactor, bool shortest)
{
	static float EPSILON = 1e3f;

	float cos = dot(dest);
	Quaternion correctedDest = dest;

	if (shortest && cos < 0)
	{
		cos = -cos;
		correctedDest = Quaternion(-dest.getX(), -dest.getY(), -dest.getZ(), -dest.getW());
	}

	if (abs(cos) >= 1 - EPSILON)
		return nLerp(correctedDest, lerpFactor, false);

	float fSin = (float)sqrt(1.0f - cos * cos);
	float fAngle = (float)atan2(fSin, cos);
	float fInvSin = 1.0f / fSin;

	float srcFactor = (float)sin((1.0f - lerpFactor) * fAngle) * fInvSin;
	float destFactor = (float)sin((lerpFactor)* fAngle) * fInvSin;

	return mul(srcFactor).add(correctedDest.mul(destFactor));
}

//From Ken Shoemake's "Quaternion Calculus and Fast Animation" article
Quaternion::Quaternion(Matrix4f rot)
{
	float trace = rot.get(0, 0) + rot.get(1, 1) + rot.get(2, 2);

	if (trace > 0)
	{
		float s = 0.5f / (float)sqrt(trace + 1.0f);
		m_w = 0.25f / s;
		m_x = (rot.get(1, 2) - rot.get(2, 1)) * s;
		m_y = (rot.get(2, 0) - rot.get(0, 2)) * s;
		m_z = (rot.get(0, 1) - rot.get(1, 0)) * s;
	}
	else
	{
		if (rot.get(0, 0) > rot.get(1, 1) && rot.get(0, 0) > rot.get(2, 2))
		{
			float s = 2.0f * (float)sqrt(1.0f + rot.get(0, 0) - rot.get(1, 1) - rot.get(2, 2));
			m_w = (rot.get(1, 2) - rot.get(2, 1)) / s;
			m_x = 0.25f * s;
			m_y = (rot.get(1, 0) + rot.get(0, 1)) / s;
			m_z = (rot.get(2, 0) + rot.get(0, 2)) / s;
		}
		else if (rot.get(1, 1) > rot.get(2, 2))
		{
			float s = 2.0f * (float)sqrt(1.0f + rot.get(1, 1) - rot.get(0, 0) - rot.get(2, 2));
			m_w = (rot.get(2, 0) - rot.get(0, 2)) / s;
			m_x = (rot.get(1, 0) + rot.get(0, 1)) / s;
			m_y = 0.25f * s;
			m_z = (rot.get(2, 1) + rot.get(1, 2)) / s;
		}
		else
		{
			float s = 2.0f * (float)sqrt(1.0f + rot.get(2, 2) - rot.get(0, 0) - rot.get(1, 1));
			m_w = (rot.get(0, 1) - rot.get(1, 0)) / s;
			m_x = (rot.get(2, 0) + rot.get(0, 2)) / s;
			m_y = (rot.get(1, 2) + rot.get(2, 1)) / s;
			m_z = 0.25f * s;
		}
	}

	float length = (float)sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
	m_x /= length;
	m_y /= length;
	m_z /= length;
	m_w /= length;
}

Vector4f Quaternion::getForward()
{
	return Vector4f(0, 0, 1, 1).rotate(this);
}

Vector4f Quaternion::getBack()
{
	return Vector4f(0, 0, -1, 1).rotate(this);
}

Vector4f Quaternion::getUp()
{
	return Vector4f(0, 1, 0, 1).rotate(this);
}

Vector4f Quaternion::getDown()
{
	return Vector4f(0, -1, 0, 1).rotate(this);
}

Vector4f Quaternion::getRight()
{
	return Vector4f(1, 0, 0, 1).rotate(this);
}

Vector4f Quaternion::getLeft()
{
	return Vector4f(-1, 0, 0, 1).rotate(this);
}

float Quaternion::getX()
{
	return m_x;
}

float Quaternion::getY()
{
	return m_y;
}

float Quaternion::getZ()
{
	return m_z;
}

float Quaternion::getW()
{
	return m_w;
}

bool Quaternion::equals(Quaternion r)
{
	return m_x == r.getX() && m_y == r.getY() && m_z == r.getZ() && m_w == r.getW();
}