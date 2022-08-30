#pragma once

#include "../elang_library_builder.h"
#include "../tools/painter.h"
//
#include "../elements/basic.h"
#include "../elements/sprite.h"
#include "../elements/canvas.h"
//#include "../elements/textbox.h" 

namespace el
{
	struct GraphicUpdate2d {};

	extern ELANG_DLL void updateBasicGraphics2d();
	extern ELANG_DLL void paintStageOrder();
	extern ELANG_DLL void batchBasic2DComponents();
	extern ELANG_DLL void batchDebuggingComponents(ShapeDebug2d* debugger, const color8&);
};