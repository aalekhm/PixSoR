#include "3D/Camera.h"
#include "3D/Quaternion.h"
#include "Common/Defines.h"
#include "Engine/InputManager.h"

Camera::Camera(_cookie)
{

}

Camera::Camera(_cookie, Matrix4f matProjection)
{
	m_ProjectionMatrix = matProjection;
}

std::unique_ptr<Camera> Camera::create(Matrix4f matProjection)
{
	return std::make_unique<Camera>(_cookie{}, matProjection);
}

Matrix4f Camera::getViewProjection()
{
	Matrix4f cameraRotation = m_Transform.getTransformedRot().conjugate().toRotationMatrix();
	Vector4f cameraPos = m_Transform.getTransformedPos().mul(-1);

	Matrix4f cameraTranslation;
	cameraTranslation.initTranslation(	cameraPos.getX(),
										cameraPos.getY(),
										cameraPos.getZ());

	return m_ProjectionMatrix.mul(cameraRotation.mul(cameraTranslation));
}

Matrix4f Camera::getView()
{
	Matrix4f cameraRotation = m_Transform.getTransformedRot().conjugate().toRotationMatrix();
	Vector4f cameraPos = m_Transform.getTransformedPos().mul(-1);

	Matrix4f cameraTranslation;
	cameraTranslation.initTranslation(	cameraPos.getX(),
										cameraPos.getY(),
										cameraPos.getZ());

	return cameraRotation.mul(cameraTranslation);
}

void Camera::update(float fDeltaTimeMs)
{
	float fMoveAmount	= 0.005f * fDeltaTimeMs;
	float fSensitivityX = 0.001f * fDeltaTimeMs;
	float fSensitivityY = 0.001f * fDeltaTimeMs;

	InputManager* pInputManager = InputManager::get();

	if (pInputManager->isKeyPressed('W') || pInputManager->isKeyPressed('w'))
		move(m_Transform.getRot().getForward(), fMoveAmount);

	if (pInputManager->isKeyPressed('S') || pInputManager->isKeyPressed('s'))
		move(m_Transform.getRot().getForward(), -fMoveAmount);

	if (pInputManager->isKeyPressed('A') || pInputManager->isKeyPressed('a'))
		move(m_Transform.getRot().getLeft(), fMoveAmount);

	if (pInputManager->isKeyPressed('D') || pInputManager->isKeyPressed('d'))
		move(m_Transform.getRot().getRight(), fMoveAmount);

	if (pInputManager->isKeyPressed('E') || pInputManager->isKeyPressed('e'))
		rotate(Y_AXIS, fSensitivityX);

	if (pInputManager->isKeyPressed('Q') || pInputManager->isKeyPressed('q'))
		rotate(Y_AXIS, -fSensitivityX);

	if (pInputManager->isKeyPressed('R') || pInputManager->isKeyPressed('r'))
		rotate(m_Transform.getRot().getRight(), -fSensitivityY);

	if (pInputManager->isKeyPressed('F') || pInputManager->isKeyPressed('f'))
		rotate(m_Transform.getRot().getRight(), fSensitivityY);
}

void Camera::move(Vector4f vDir, float fAmount)
{
	m_Transform.setPos(m_Transform.getPos().add(vDir.mul(fAmount)));
}

void Camera::rotate(Vector4f vAxis, float fAngle)
{
	m_Transform.rotate(Quaternion(vAxis, fAngle));
}

Vector4f Camera::getForward()
{
	return m_Transform.getRot().getForward();
}

Vector4f Camera::getRight()
{
	return m_Transform.getRot().getRight();
}

Vector4f Camera::getUp()
{
	return m_Transform.getRot().getUp();
}

void Camera::setPosition(Vector4f vPosition)
{
	m_Transform.setPos(vPosition);
}

Transform Camera::getTransform()
{
	return m_Transform;
}
