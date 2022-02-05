#pragma once

#include "define.h"

enum struct eSwitch
{
	ON = 0,
	OFF
};

enum struct eInput
{
	NONE = 0,
	LIFT,
	SNAP,
	ONCE,
	HOLD,
	FLAP
};

enum struct eSelect
{
	NO_CHANGE,
	SELECTED,
	DESELECTED
};


enum struct eAlignment
{
	MID,
	LEFT,
	RIGHT
};