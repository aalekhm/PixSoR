#pragma once
#include <string>

class Vector4f
{
	public:
						Vector4f();
						Vector4f(float x, float y, float z);
						Vector4f(float x, float y, float z, float w);
		float			length();
		float			max_();
		float			dot(Vector4f r);
		float			dot(Vector4f* r);
		Vector4f		cross(Vector4f r);
		Vector4f		normalized();
		Vector4f		rotate(Vector4f axis, float angle);
		Vector4f		lerp(Vector4f dest, float lerpFactor);

		Vector4f		operator+(Vector4f r);
		void			operator+=(Vector4f r);
		Vector4f		operator+(float r);
		void			operator+=(float r);
		Vector4f		add(Vector4f r);
		Vector4f		add(float r);

		Vector4f		operator-(Vector4f r);
		void			operator-=(Vector4f r);
		Vector4f		operator-(float r);
		void			operator-=(float r);
		Vector4f		sub(Vector4f r);
		Vector4f		sub(float r);

		Vector4f		operator*(Vector4f r);
		Vector4f		operator*(float r);
		void			operator*=(Vector4f r);
		void			operator*=(float r);
		Vector4f		mul(Vector4f r);
		Vector4f		mul(float r);
		
		Vector4f		operator/(Vector4f r);
		Vector4f		operator/(float r);
		void			operator/=(Vector4f r);
		void			operator/=(float r);
		Vector4f		div(Vector4f r);
		Vector4f		div(float r);

		Vector4f		abs_();
		std::string		toString();
		float			getX();
		float			getY();
		float			getZ();
		float			getW();
		bool			equals(Vector4f r);

		float			x;
		float			y;
		float			z;
		float			w;
	protected:
	private:
};