#include "3D/Transform.h"

Transform::Transform()
{
	m_pos = Vector4f(0, 0, 0, 0);
	m_scale = Vector4f(1, 1, 1, 1);
	m_rot = Quaternion(0, 0, 0, 1);
}

Transform::Transform(Vector4f pos)
{
	m_pos = pos;
	m_scale = Vector4f(1, 1, 1, 1);
	m_rot = Quaternion(0, 0, 0, 1);
}

Transform::Transform(Vector4f pos, Quaternion rot, Vector4f scale)
{
	m_pos = pos;
	m_scale = scale;
	m_rot = rot;
}

void Transform::setPos(Vector4f pos)
{
	m_pos = pos;
}

void Transform::setScale(Vector4f scale)
{
	m_scale = scale;
}

void Transform::rotate(Quaternion rotation)
{
	m_rot = rotation.mul(m_rot).normalized();
}

Quaternion Transform::getLookAtRotation(Vector4f point, Vector4f up)
{
	return Quaternion(Matrix4f().initRotation(point.sub(m_pos).normalized(), up));
}

Matrix4f Transform::getTransformation()
{
	Matrix4f translationMatrix;
	translationMatrix.initTranslation(	m_pos.getX(),
										m_pos.getY(),
										m_pos.getZ());

	Matrix4f rotationMatrix = m_rot.toRotationMatrix();

	Matrix4f scaleMatrix;
	scaleMatrix.initScale(	m_scale.getX(),
							m_scale.getY(),
							m_scale.getZ());

	return translationMatrix.mul( rotationMatrix.mul(scaleMatrix) );
}

Vector4f Transform::getTransformedPos()
{
	return m_pos;
}

Quaternion Transform::getTransformedRot()
{
	return m_rot;
}

Vector4f Transform::getPos()
{
	return m_pos;
}

Vector4f Transform::getScale()
{
	return m_scale;
}

Quaternion Transform::getRot()
{
	return m_rot;
}

void Transform::setPosX(float fValue)
{
	m_pos.x = fValue;
}

void Transform::setPosY(float fValue)
{
	m_pos.y = fValue;
}

void Transform::setPosZ(float fValue)
{
	m_pos.z = fValue;
}

void Transform::translateX(float fValue)
{
	m_pos.x += fValue;
}

void Transform::translateY(float fValue)
{
	m_pos.y += fValue;
}

void Transform::translateZ(float fValue)
{
	m_pos.z += fValue;
}
