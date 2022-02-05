#pragma once

#include <GL/glew.h>

#include "../common/container.h"
#include "../common/math.h"
#include "../common/string.h"
#include "../common/enums.h"


namespace el {
	inline sizet gLineIndices[2] = { 0, 1 };
	inline sizet gBox2dFillIndices[6] = { 0, 1, 3, 1, 3, 2 };
	inline sizet gBox2dLineIndices[8] = { 0, 1, 1, 2, 2, 3, 3, 0 };


	//=======================================================================================
	// Inherit to make a Vertex Data Type (include macro EL_DECLARE_VERTEX_DATA_H on top)
	//=======================================================================================
	struct IVertexData
	{
		IVertexData(sizet count) : mCount(count) {}

		virtual void bindAttribLocation(sizet id) = 0;
		virtual void vertexAttribPointer() = 0;
		void enableVertexAttribArray() {
			for (sizet i = 0; i < mCount; i++)
				glEnableVertexAttribArray(GLuint(i));
		}
		void disableVertexAttribArray() {
			for (sizet i = 0; i < mCount; i++)
				glDisableVertexAttribArray(GLuint(i));
		}

		virtual sizet index() = 0;
		virtual sizet getSize() = 0;
		protected:
			~IVertexData() {};
			sizet mCount;
	};

	//=======================================================================================
	// GlslProgram used by Painters
	//=======================================================================================
	struct GlslProgram
	{
		uint32 shader;
		IVertexData* vertdata;
		string key;

		GlslProgram() : shader(-1), vertdata(0) {};
		GlslProgram(uint32 shader_, IVertexData* vertdata_, const string& key_) : shader(shader_), vertdata(vertdata_), key(key_) {};
	};
}

#define EL_DECLARE_VERTEX_DATA_H \
static const sizet sVertexDataIndex = __COUNTER__; \
virtual sizet index() override { return sVertexDataIndex; } 


namespace el {
	//=======================================================================================
	// (void)
	//=======================================================================================
	#pragma warning ( disable : 4250 )
	struct VoidVertexData : IVertexData
	{
		EL_DECLARE_VERTEX_DATA_H

		VoidVertexData() : IVertexData(0) {};
		sizet getSize() { return 0; };

		void bindAttribLocation(sizet id) override {}
		void vertexAttribPointer() override {}
	};

	//=======================================================================================
	// (vec3 pos, vec2 uv)
	//=======================================================================================
	struct DefaultVertex
	{
		vec3 pos;
		vec2 uv;
	};
	//=======================================================================================
	// (vec3 pos, vec2 uv)
	//=======================================================================================
	struct DefaultVertexData : IVertexData
	{
		EL_DECLARE_VERTEX_DATA_H

		DefaultVertexData() : IVertexData(2) {};
		sizet getSize() override { return sizeof(DefaultVertex); }

		void bindAttribLocation(sizet id) override {
			glBindAttribLocation(GLuint(id), 0, "vPosition");
			glBindAttribLocation(GLuint(id), 1, "vUV");
		}
		void vertexAttribPointer() override {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DefaultVertex), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DefaultVertex), (void*)offsetof(DefaultVertex, uv));
		}
	};

	//=======================================================================================
	// (vec3 pos, color col)
	//=======================================================================================
	struct PrimitiveVertex 
	{
		vec3 pos;
		color col;
	};
	//=======================================================================================
	// (vec3 pos, color col)
	//=======================================================================================
	struct PrimitiveVertexData : IVertexData
	{
		EL_DECLARE_VERTEX_DATA_H

		PrimitiveVertexData() : IVertexData(2) {};
		sizet getSize() override { return sizeof(PrimitiveVertex); }

		void bindAttribLocation(sizet id) override {
			glBindAttribLocation(GLuint(id), 0, "vPosition");
			glBindAttribLocation(GLuint(id), 1, "vColor");
		}
		void vertexAttribPointer() override {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveVertex), 0);
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(PrimitiveVertex), (void*)offsetof(PrimitiveVertex, col));
		}
	};


	//=======================================================================================
	// (vec2 pos)
	//=======================================================================================
	struct Position2DVertex
	{
		vec2 pos;
	};
	//=======================================================================================
	// (vec2 pos)
	//=======================================================================================
	struct Position2DVertexData : IVertexData
	{
		EL_DECLARE_VERTEX_DATA_H

		Position2DVertexData() : IVertexData(1) {};
		sizet getSize() override { return sizeof(vec2); }

		void bindAttribLocation(sizet id) override {
			glBindAttribLocation(GLuint(id), 0, "vPosition2D");
		}
		void vertexAttribPointer() override {
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Position2DVertexData), 0);
		}
	};

	//=======================================================================================
	// (vec2 pos, color col)
	//=======================================================================================
	struct Primitive2DVertex 
	{
		vec2 pos;
		color col;
	};
	//=======================================================================================
	// (vec2 pos, color col)
	//=======================================================================================
	struct Primitive2DVertexData : IVertexData
	{
		EL_DECLARE_VERTEX_DATA_H

		Primitive2DVertexData() : IVertexData(2) {};
		sizet getSize() override { return sizeof(Primitive2DVertex); }

		void bindAttribLocation(sizet id) override {
			glBindAttribLocation(GLuint(id), 0, "vPosition2D");
			glBindAttribLocation(GLuint(id), 1, "vColor");
		}

		void vertexAttribPointer() override {
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Primitive2DVertex), 0);
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Primitive2DVertex), (void*)offsetof(Primitive2DVertex, col));
		}
	};


	//=======================================================================================
	// (vec2 pos, vec2 uv)
	//=======================================================================================
	struct SpriteVertex
	{
		vec2 pos;
		vec2 uv;

		SpriteVertex() {}
		SpriteVertex(const vec2& pos_, const vec2& uv_) : pos(pos_), uv(uv_) {}
	};
	//=======================================================================================
	// (vec2 pos, vec2 uv)
	//=======================================================================================
	struct SpriteVertexData : IVertexData
	{
		EL_DECLARE_VERTEX_DATA_H

		SpriteVertexData() : IVertexData(2) {};
		sizet getSize() override { return sizeof(SpriteVertex); }

		void bindAttribLocation(sizet id) override {
			glBindAttribLocation(GLuint(id), 0, "vPosition2D");
			glBindAttribLocation(GLuint(id), 1, "vUV");
		}

		void vertexAttribPointer() override {
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, uv));
		}
	};

	//=======================================================================================
	// (vec2 pos, vec2 uv, color col)
	//=======================================================================================
	struct ColoredSpriteVertex
	{
		vec2 pos;
		vec2 uv;
		color col;
	};
	//=======================================================================================
	// (vec2 pos, vec2 uv, color col)
	//=======================================================================================
	struct ColoredSpriteVertexData : IVertexData
	{
		EL_DECLARE_VERTEX_DATA_H

			ColoredSpriteVertexData() : IVertexData(3) {};
		sizet getSize() override { return sizeof(ColoredSpriteVertex); }

		void bindAttribLocation(sizet id) override {
			glBindAttribLocation(GLuint(id), 0, "vPosition2D");
			glBindAttribLocation(GLuint(id), 1, "vUV");
			glBindAttribLocation(GLuint(id), 2, "vColor");
		}

		void vertexAttribPointer() override {
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredSpriteVertex), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredSpriteVertex), (void*)offsetof(ColoredSpriteVertex, uv));
			glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ColoredSpriteVertex), (void*)offsetof(ColoredSpriteVertex, col));
		}
	};
}