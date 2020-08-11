#pragma once
#include "Common/Singleton.h"
#include <bitset>

class InputManager : public Singleton<InputManager>
{
	public:
		friend class GameEngine;

		bool				isKeyPressed(int32_t iKey);

		bool				isMousePressed(int32_t iKey);
		void				getMousePos(double& xPos, double& yPos);
		double				getMouseX();
		double				getMouseY();
	protected:
	private:
		void				onKeyPressed(uint32_t iKey);
		void				onKeyReleased(uint32_t iKey);

		void				onMousePressed(uint32_t iKey);
		void				onMouseReleased(uint32_t iKey);

		void				onMouseMoved(double xPos, double yPos);

		std::bitset<512>	m_KeyboardBits;
		std::bitset<8>		m_MouseBits;

		double				m_dMouseX;
		double				m_dMouseY;
};