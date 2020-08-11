#include "Engine/InputManager.h"

bool InputManager::isKeyPressed(int32_t iKey)
{
	return m_KeyboardBits.test(iKey);
}

bool InputManager::isMousePressed(int32_t iKey)
{
	return m_MouseBits.test(iKey);
}

void InputManager::getMousePos(double& xPos, double& yPos)
{
	xPos = m_dMouseX;
	yPos = m_dMouseY;
}

double InputManager::getMouseX()
{
	return m_dMouseX;
}

double InputManager::getMouseY()
{
	return m_dMouseY;
}

void InputManager::onKeyPressed(uint32_t iKey)
{
	m_KeyboardBits.set(iKey, 1);
}

void InputManager::onKeyReleased(uint32_t iKey)
{
	m_KeyboardBits.reset(iKey);
}
	 
void InputManager::onMousePressed(uint32_t iKey)
{
	m_MouseBits.set(iKey, 1);
}

void InputManager::onMouseReleased(uint32_t iKey)
{
	m_MouseBits.reset(iKey);
}
	 
void InputManager::onMouseMoved(double xPos, double yPos)
{
	m_dMouseX = xPos;
	m_dMouseY = yPos;
}
