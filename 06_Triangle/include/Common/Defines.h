#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH	480
#define DEFAULT_HEIGHT	320

#define USE_BRESENHAM

enum DURATION_TYPE
{
	nanoseconds = 0,
	microseconds,
	milliseconds,
	seconds,
	minutes,
	hours
};

#define TIMER_START_NEW(__identifier__) \
	Timer __identifier__; \
	TIMER_START(__identifier__); \

#define TIMER_START(__identifier__) __identifier__.start();
#define TIMER_STOP(__identifier__) __identifier__.stop();

#define CLAMP(__min__, __value__, __max__) \
if(__value__ < __min__) __value__ = __min__; \
else if(__value__ > __max__) __value__ = __max__; \

#define LERP(__start__, __end__, __amount__) __start__ + (__end__ - __start__) * __amount__
