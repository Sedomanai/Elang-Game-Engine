/*****************************************************************//**
 * @file   enums.h
 * @brief  Contains very common enum structs (enum class). All enum structs are prefixed with e.
 *         Yes I know, hungarian notation, I got too used to it.
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
 /**
  * @brief Do not change the order of the states; more than one files depend on it
  * @brief Once refers to the first press; after that the state should become Hold
  * @brief Lift refers to the first release; after that the state should become None
  * @brief Default state is None
  * @brief Flap and Snap are for very slow framerates. They must be internally resolved to either of the four above
  *
  */
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
	NoChange,
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