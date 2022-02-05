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


enum struct eExtension
{
	MID,
	LEFT,
	RIGHT
};


enum struct eDataType
{
	VOID = 0,

	INT,
	SHORT,
	LONG,
	LONGLONG,
	INT8,
	INT16,
	INT32,
	INT64,

	UINT,
	USHORT,
	ULONG,
	ULONGLONG,
	UINT8,
	UINT16,
	UINT32,
	UINT64,

	FLOAT,
	VEC2,
	VEC3,
	VEC4,

	MATRIX3,
	MATRIX4
};