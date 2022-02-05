#pragma once

#include "../elang_library_builder.h"
#include "../tools/painter.h"

namespace el
{
	extern ELANG_DLL void updateBasicSprites(Observer&);
	extern ELANG_DLL void paintStageOrder();
	extern ELANG_DLL void batchBasic2DComponents();
	extern ELANG_DLL void batchDebuggingComponents(ShapeDebug* debugger, const color&);
};