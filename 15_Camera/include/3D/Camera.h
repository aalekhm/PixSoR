#pragma once
#include "3D/Matrix4f.h"
#include "3D/Transform.h"
#include "3D/Vector4f.h"

class Camera
{
	public:
							Camera(Matrix4f matProjection);

		Matrix4f			getViewProjection();
		Matrix4f			getView();

		void				update(float fDeltaTimeMs);
		void				move(Vector4f vDir, float fAmount);
		void				rotate(Vector4f vAxis, float fAngle);

		Vector4f			getForward();
		Vector4f			getRight();
		Vector4f			getUp();
	protected:
	private:
		Transform			m_Transform;
		Matrix4f			m_ProjectionMatrix;

		Transform			getTransform();
};