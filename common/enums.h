/*****************************************************************//**
 * @file   enums.h
 * @brief  Contains very common enum structs (enum class). All enum structs are previxed with e. 
 *         Yes I know, hungarian notation, I got too used to it.
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include "define.h"

enum struct eSwitch
{
	On = 0,
	Off
};

enum struct eInput
{
	None = 0,
	Lift,
	Snap,
	Once,
	Hold,
	Flap
};

enum struct eSelect
{
	No_change,
	Selected,
	Deselected
};


enum struct eAlignment
{
	Left = -1,
	Mid,
	Right
};

enum struct eDataType
{
	Void_type = 0,

	Int,
	Short,
	Long,
	Longlong,
	Int8,
	Int16,
	Int32,
	Int64,

	Uint,
	Ushort,
	Ulong,
	Ulonglong,
	Uint8,
	Uint16,
	Uint32,
	Uint64,

	Float,
	Vec2,
	Vec3,
	Vec4,
	Color8,

	Matrix3,
	Matrix4
};