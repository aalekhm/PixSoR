#pragma once
#include "Common/Defines.h"
#include <chrono>

class Timer
{
	public:
		void			start();
		void			stop();
		double			duration();
		uint64_t		duration(DURATION_TYPE eDURATION_TYPE);
	protected:
	private:
		std::chrono::high_resolution_clock::time_point		m_tpStart;
		std::chrono::duration<double>						m_Duration;
};