/*****************************************************************//**
 * @file   vector.h
 * @brief  Vector, using std::vector
 *		   I separated the interface because there's a chance I may use another vector than std
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <vector>

namespace el
{
	template <typename T>
	using vector = std::vector<T>;
}