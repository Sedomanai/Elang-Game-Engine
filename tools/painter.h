#pragma once

#include "../common/stream.h"
#include "../common/batch.h"
#include "../common/shape2d.h"
#include "material.h"
#include "camera.h"
#include "vertex.h"
#include "shader.h"

namespace el
{
	enum
	{
		E_DANGLING_VERTICES = 1,
		E_DANGLING_INDICES = 2,
	};
	
	//=======================================================================================
	// Graphic layer
	// A painter has its own GL Shader Pipeline, a VBO, and a VBO
	// Can only batch Visages with a matching Vertex Type
	//=======================================================================================
	struct ELANG_DLL Painter
	{
		enum
		{
			MULTI_MATERIAL	= 1,
			DEPTH_SORT		= 2,
			LOCKED			= 4,
			Z_READ_ONLY		= 8,
			Z_CLEAR			= 16,
		};

		Painter();
		Painter(strview vertexShader, strview fragmentShader, sizet maxVertexCount = 3000U, 
			asset<Camera> camera = NullEntity, Projection projection = Projection::eOrtho, sizet flags = 0);

		static void sCreateNullTexture();

		asset<Camera> camera;
		Projection projection;
		sizet flags, drawtype;
		vec4 color;

		template<typename T>
		void serialize(T& archive) {
			archive(mVertLabel, mFragLabel, mLocked, mTargetCount, camera, projection, flags, drawtype, color);
		}
		void init() {
			bindShaderBuffer();
			bindDataBuffer();
		}
		void destroy();
		void batchline(const line& line, const color8& c, float depth = 0.0f);
		void batchAABB(const aabb& aabb, const color8& c, float depth = 0.0f);
		void batchBox(const poly2d& box, const color8& c, float depth = 0.0f);
		void batchCircle(const circle& circ, const color8& c, float depth = 0.0f);
		void batchPoly(const poly2d& circ, const color8& c, float depth = 0.0f);

		template<typename T, typename ...Arg>
		void batch(Arg... args) {
			if (!mLocked && sizeof(T) == mVert->size()) {
				mBatches.emplace_back(args...);
				mBatchOrder.emplace_back(mBatchOrder.size());
			}
		}
		void paint();
		void lock() { flags |= LOCKED; }
		void unlock() { flags &= ~LOCKED; }
		void forceUnlock();

		string vertLabel() { return mVertLabel; }
		string fragLabel() { return mFragLabel; }
		sizet targetCount() { return mTargetCount; }
		sizet currentBatchCount() { return mBatches.size(); }
		bool empty() { return !(bool)mBumpers.size(); }

		void setUniformFloat(uint shader, float dat, const char* name);
		void setUniformVec2(uint shader, const vec2& dat, const char* name);
		void setUniformVec3(uint shader, const vec3& dat, const char* name);
		void setUniformVec4(uint shader, const vec4& dat, const char* name);
		void setUniformMatrix(uint shader, const matrix4x4& dat, const char* name);
	protected:
		virtual void setCamera();
		virtual void bindMaterial(uint32 material);
		void bindMaterialBody(Material& mat);


		void sort();
		void flush();
		void clear();
		void bindShaderBuffer();
		void bindDataBuffer();
		void circlePoints(vector<Primitive2DVertex>& buffer, const color8& c, float cx, float cy, float x, float y);

		string mVertLabel, mFragLabel;
		VertexShader* mVert;
		FragmentShader* mFrag;

		vector<Batch> mBatches;
		vector<sizet> mBatchOrder;
		vector<sizet> mBumpers;

		unsigned int mPipeline, mVao, mVbo, mIbo;
		sizet mTargetCount;
		sizet mIndexCount;
		
		bool mLocked;

		static uint32 sNullTextureID;
	};

	struct ShapeDebug
	{
		Painter point;
		Painter line;
		Painter fill;

		ShapeDebug() : mInit(false),
			point("debug2d", "debug", 100000, NullEntity, Projection::eOrtho, Painter::Z_CLEAR),
			line("debug2d", "debug", 100000, NullEntity, Projection::eOrtho, Painter::Z_CLEAR),
			fill("debug2d", "debug", 100000, NullEntity, Projection::eOrtho, Painter::Z_CLEAR)
		{};
		~ShapeDebug() {
			if (mInit) {
				point.destroy();
				line.destroy();
				fill.destroy();
			}
		}

		void init(asset<Camera> camera_) {
			if (!mInit) {
				mInit = true;
				fill.camera = point.camera = fill.camera = line.camera = point.camera = camera_;
				fill.color = vec4(1, 1, 1, 0.2f);
				point.drawtype = GL_POINTS;
				line.drawtype = GL_LINES;
				point.init();
				line.init();
				fill.init();
			}
		}

		void draw() {
			// order matters
			point.paint();
			line.paint();
			fill.paint();
		}

	private:
		bool mInit;
	};

	extern ELANG_DLL vector<Entity> gAtelier;
};