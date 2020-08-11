#pragma once
#include "3D/Matrix4f.h"
#include "3D/Transform.h"
#include "3D/Vector4f.h"
#include <memory>

class Camera
{
	private:
		struct _cookie {};
	public:
										Camera(_cookie);
										Camera(_cookie, Matrix4f matProjection);

		static std::unique_ptr<Camera>	create(Matrix4f matProjection);

		Matrix4f						getViewProjection();
		Matrix4f						getView();

		void							update(float fDeltaTimeMs);
		void							move(Vector4f vDir, float fAmount);
		void							rotate(Vector4f vAxis, float fAngle);

		Vector4f						getForward();
		Vector4f						getRight();
		Vector4f						getUp();

		void							setPosition(Vector4f vPosition);;
	protected:
	private:
		Transform						m_Transform;
		Matrix4f						m_ProjectionMatrix;

		Transform						getTransform();
};