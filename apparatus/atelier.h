/*****************************************************************//**
 * @file   atelier.h
 * @brief  Painter order related
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "../tools/vertex.h"

namespace el
{
	struct GraphicUpdate2d {};
	struct ShapeDebug2d;

	extern ELANG_DLL void updateBasicGraphics2d();
	extern ELANG_DLL void paintStageOrder();
	extern ELANG_DLL void batchBasic2DComponents();
	extern ELANG_DLL void batchDebuggingComponents(ShapeDebug2d* debugger, const color8&);
};