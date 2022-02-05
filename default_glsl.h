
#include "common/string.h"

namespace el {

////////// VERTEX SHADER //////////////

inline string gGlslVertexPosition2d = "#version 430\n\
\n\
in vec2 vPosition2D;\n\
out vec4 fColor;\n\
\n\
uniform vec4 uColor = vec4(1, 1, 1, 1);\n\
uniform mat4 uView;\n\
\n\
void main() {\n\
	gl_Position = uView * vec4(vPosition2D, 0, 1);\n\
	fColor = uColor;\n\
}";

inline string gGlslVertexDebug2d = "#version 430 \n\
\n\
in vec2 vPosition2D;\n\
in vec4 v8Color;\n\
\n\
out vec4 fColor;\n\
\n\
uniform vec4 uColor = vec4(1, 1, 1, 1);\n\
uniform mat4 uView;\n\
\n\
void main() {\n\
	gl_Position = uView * vec4(vPosition2D, 0, 1);\n\
	fColor = uColor * v8Color;\n\
}";

inline string gGlslVertexDebug3d = "#version 430 \n\
\n\
in vec3 vPosition;\n\
in vec4 v8Color;\n\
out vec4 fColor;\n\
\n\
uniform vec4 uColor = vec4(1, 1, 1, 1);\n\
uniform mat4 uView;\n\
\n\
void main() {\n\
	gl_Position = uView * vec4(vPosition, 1);\n\
	fColor = uColor * v8Color;\n\
}";

inline string gGlslVertexBasicSprite = "#version 430 \n\
\n\
in vec2 vPosition2D;\n\
in vec2 vUV;\n\
\n\
out vec2 fUV;\n\
out vec4 fColor;\n\
\n\
uniform vec4 uColor = vec4(1, 1, 1, 1);\n\
uniform mat4 uView;\n\
\n\
void main() {\n\
	gl_Position = uView * vec4(vPosition2D, 0, 1);\n\
\n\
	fUV = vUV;\n\
	fColor = uColor;\n\
}";

inline string gGlslVertexColorSprite = "#version 430 \n\
\n\
in vec2 vPosition2D;\n\
in vec2 vUV;\n\
in vec4 v8Color;\n\
\n\
out vec2 fUV;\n\
out vec4 fColor;\n\
\n\
uniform vec4 uShade = vec4(1, 1, 1, 1);\n\
uniform vec4 uTint = vec4(0, 0, 0, 0);\n\
uniform mat4 uView;\n\
\n\
void main() {\n\
	gl_Position = uView * vec4(vPosition2D, 0, 1);\n\
\n\
	fUV = vUV;\n\
	fColor = v8Color * uShade + uTint;\n\
}";



////////// FRAGMENT SHADER //////////////

inline string gGlslFragmentDebug = "#version 430 \n\
\n\
in vec4 fColor;\n\
out vec4 oColor;\n\
\n\
void main() {\n\
	oColor = fColor;\n\
}";

inline string gGlslFragmentTextureUV = "#version 430 \n\
\n\
in vec2 fUV;\n\
in vec4 fColor;\n\
\n\
out vec4 oColor;\n\
\n\
uniform sampler2D uDiffuse; \n\
\n\
void main() {\n\
	oColor = texture(uDiffuse, fUV) * fColor;\n\
}";

inline string gGlslFragmentGrid = "#version 430 \n\
\n\
in vec4 fColor;\n\
out vec4 oColor;\n\
\n\
uniform float uScreenWidth = 0;\n\
uniform float uScreenHeight = 0;\n\
\n\
\n\
uniform vec2 uResolution = vec2(800, 600);\n\
uniform vec2 uOffset = vec2(0, 0);\n\
uniform float uSize = 50;\n\
\n\
float alpha = 0.5;\n\
\n\
//void mixColor(float posv) {\n\
	//step(fract(posv / uSize * 2), 0) * alpha + step(fract(posv / uSize), 0) * 0.2)\n\
	//oColor.a = mix(oColor.a, step(fract(posv / uSize * 2), 0), step(oColor.a, 0)) * alpha;\n\
//}\n\
\n\
void main() {\n\
\n\
	vec2 pos = vec2(gl_FragCoord.x, gl_FragCoord.y) - uOffset - uResolution / 2 + vec2(0.5, 0.5);\n\
\n\
	alpha = step(pos.x, 0) + step(0, pos.x) + step(pos.y, 0) + step(0, pos.y) - 2;\n\
\n\
\n\
\n\
	float halfx = step(fract(pos.x / uSize * 2), 0);\n\
	float wholex = step(fract(pos.x / uSize), 0);\n\
	float halfy = step(fract(pos.y / uSize * 2), 0);\n\
	float wholey = step(fract(pos.y / uSize), 0);\n\
\n\
	alpha += halfx * 0.3f + wholex * 0.4f;\n\
	alpha += halfy * 0.3f + wholey * 0.4f;\n\
\n\
	oColor = vec4(1, 0, 0, alpha);\n\
	//mixColor(pos.x);\n\
	//mixColor(pos.y);\n\
	//mixColor(pos.x + 1);\n\
	//mixColor(pos.y + 1);\n\
}";



}