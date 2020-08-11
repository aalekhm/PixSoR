#include "3D/Vector4f.h"

Vector4f::Vector4f()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vector4f::Vector4f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 1.0f;
}

Vector4f::Vector4f(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float Vector4f::length()
{
	return (float)sqrt(x * x + y * y + z * z + w * w);
}

float Vector4f::max_()
{
	return fmax( fmax(x, y), fmax(z, w));
}

float Vector4f::dot(Vector4f r)
{
	return x * r.getX() + y * r.getY() + z * r.getZ() + w * r.getW();
}

float Vector4f::dot(Vector4f* r)
{
	return x * r->getX() + y * r->getY() + z * r->getZ() + w * r->getW();
}

Vector4f Vector4f::cross(Vector4f r)
{
	float x_ = y * r.getZ() - z * r.getY();
	float y_ = z * r.getX() - x * r.getZ();
	float z_ = x * r.getY() - y * r.getX();

	return Vector4f(x_, y_, z_, 0);
}

Vector4f Vector4f::normalized()
{
	float fLength = length();

	return Vector4f(x / fLength, y / fLength, z / fLength, w / fLength);
}

Vector4f Vector4f::rotate(Vector4f axis, float angle)
{
	float sinAngle = (float)sin(-angle);
	float cosAngle = (float)cos(-angle);

	return cross(	axis.mul(sinAngle)).add(						// Rotation on local X
						(mul(cosAngle)).add(						// Rotation on local Z
						axis.mul(dot(axis.mul(1 - cosAngle)))));	// Rotation on local Y
}

Vector4f Vector4f::lerp(Vector4f dest, float lerpFactor)
{
	return dest.sub(*this).mul(lerpFactor).add(*this);
}

Vector4f Vector4f::operator+(Vector4f r)
{
	return Vector4f(x + r.getX(), y + r.getY(), z + r.getZ(), w + r.getW());
}

void Vector4f::operator+=(Vector4f r)
{
	x = x + r.getX();
	y = y + r.getY();
	z = z + r.getZ();
	w = w + r.getW();
}

Vector4f Vector4f::operator+(float r)
{
	return Vector4f(x + r, y + r, z + r, w + r);
}

void Vector4f::operator+=(float r)
{
	x = x + r;
	y = y + r;
	z = z + r;
	w = w + r;
}

Vector4f Vector4f::add(Vector4f r)
{
	return Vector4f(x + r.getX(), y + r.getY(), z + r.getZ(), w + r.getW());
}

Vector4f Vector4f::add(float r)
{
	return Vector4f(x + r, y + r, z + r, w + r);
}

Vector4f Vector4f::operator-(Vector4f r)
{
	return Vector4f(x - r.getX(), y - r.getY(), z - r.getZ(), w - r.getW());
}

Vector4f Vector4f::operator-(float r)
{
	return Vector4f(x - r, y - r, z - r, w - r);
}

void Vector4f::operator-=(Vector4f r)
{
	x = x - r.getX();
	y = y - r.getY();
	z = z - r.getZ();
	w = w - r.getW();
}

void Vector4f::operator-=(float r)
{
	x = x - r;
	y = y - r;
	z = z - r;
	w = w - r;
}

Vector4f Vector4f::sub(Vector4f r)
{
	return Vector4f(x - r.getX(), y - r.getY(), z - r.getZ(), w - r.getW());
}

Vector4f Vector4f::sub(float r)
{
	return Vector4f(x - r, y - r, z - r, w - r);
}

Vector4f Vector4f::operator*(Vector4f r)
{
	return Vector4f(x * r.getX(), y * r.getY(), z * r.getZ(), w * r.getW());
}

Vector4f Vector4f::operator*(float r)
{
	return Vector4f(x * r, y * r, z * r, w * r);
}

void Vector4f::operator*=(Vector4f r)
{
	x = x * r.getX();
	y = y * r.getY();
	z = z * r.getZ();
	w = w * r.getW();
}

void Vector4f::operator*=(float r)
{
	x = x * r;
	y = y * r;
	z = z * r;
	w = w * r;
}

Vector4f Vector4f::mul(Vector4f r)
{
	return Vector4f(x * r.getX(), y * r.getY(), z * r.getZ(), w * r.getW());
}

Vector4f Vector4f::mul(float r)
{
	return Vector4f(x * r, y * r, z * r, w * r);
}

Vector4f Vector4f::operator/(Vector4f r)
{
	return Vector4f(x / r.getX(), y / r.getY(), z / r.getZ(), w / r.getW());
}

Vector4f Vector4f::operator/(float r)
{
	return Vector4f(x / r, y / r, z / r, w / r);
}

void Vector4f::operator/=(Vector4f r)
{
	x = x / r.getX();
	y = y / r.getY();
	z = z / r.getZ();
	w = w / r.getW();
}

void Vector4f::operator/=(float r)
{
	x = x / r;
	y = y / r;
	z = z / r;
	w = w / r;
}

Vector4f Vector4f::div(Vector4f r)
{
	return Vector4f(x / r.getX(), y / r.getY(), z / r.getZ(), w / r.getW());
}

Vector4f Vector4f::div(float r)
{
	return Vector4f(x / r, y / r, z / r, w / r);
}

Vector4f Vector4f::abs_()
{
	return Vector4f(abs(x), abs(y), abs(z), abs(w));
}

std::string Vector4f::toString()
{
	char str[255];
	snprintf(str, 255, "( x = %3.2f, y = %3.2f, z = %3.2f, w = %3.2f", x, y, z, w);

	return std::string(str);
}

float Vector4f::getX()
{
	return x;
}

float Vector4f::getY()
{
	return y;
}

float Vector4f::getZ()
{
	return z;
}

float Vector4f::getW()
{
	return w;
}

bool Vector4f::equals(Vector4f r)
{
	return (x == r.getX() && y == r.getY() && z == r.getZ() && w == r.getW());
}