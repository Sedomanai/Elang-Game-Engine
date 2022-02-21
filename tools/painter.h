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


	enum ePainterFlags
	{
		MULTI_MATERIAL = 1,
		DEPTH_SORT = 2,
		LOCKED = 4,
		Z_READ_ONLY = 8,
		Z_CLEAR = 16,
	};

	template<int M, int T, int C>
	struct ELANG_DLL PainterImpl
	{
		PainterImpl();
		PainterImpl(strview vertexShader, strview fragmentShader, sizet maxVertexCount = 3000U,
			asset<CameraImpl<C>> camera = NullEntity, Projection projection = Projection::eOrtho, sizet flags = 0);

		// 귀찮은데 언젠가 고쳐야 된다...
		static void sCreateNullTexture();

		asset<CameraImpl<C>> camera;
		Projection projection;
		sizet flags, drawtype;
		vec4 color;

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(mVertLabel, mFragLabel, mLocked, mTargetCount, camera, projection, flags, drawtype, color);
		}
		void init();
		void destroy();
		void batchline(const line& line, const color8& c, float depth = 0.0f);
		void batchAABB(const aabb& aabb, const color8& c, float depth = 0.0f);
		void batchBox(const poly2d& box, const color8& c, float depth = 0.0f);
		void batchCircle(const circle& circ, const color8& c, float depth = 0.0f);
		void batchPoly(const poly2d& circ, const color8& c, float depth = 0.0f);

		template<typename A, typename ...Arg>
		void batch(Arg... args) {
			if (!mLocked && sizeof(A) == mVert->size()) {
				mBatches.emplace_back(args...);
				mBatchOrder.emplace_back(mBatchOrder.size());
			}
		}
		void paint();
		void lock() { flags |= ePainterFlags::LOCKED; }
		void unlock() { flags &= ~ePainterFlags::LOCKED; }
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
	private:

		void sort();
		void flush();
		void clear();
		void setCamera();
		void bindMaterial(uint32 material);
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

	using Painter = PainterImpl<0, 0, 0>;
	using EditorPainter = PainterImpl<1, 1, 1>;
	using EditorProjectPainter = PainterImpl<1, 0, 1>;
	using EditorWorldPainter = PainterImpl<1, 1, 0>;
	using EditorWorldProjectPainter = PainterImpl<1, 0, 0>; // probably won't be used
	using EditorShapeDebugPainter = PainterImpl<0, 0, 1>;

	template<int C>
	struct ShapeDebugImpl
	{
		PainterImpl<0, 0, C> point;
		PainterImpl<0, 0, C> line;
		PainterImpl<0, 0, C> fill;

		ShapeDebugImpl() : mInit(false),
			point("debug2d", "debug", 100000, NullEntity, Projection::eOrtho, ePainterFlags::Z_CLEAR),
			line("debug2d", "debug", 100000, NullEntity, Projection::eOrtho, ePainterFlags::Z_CLEAR),
			fill("debug2d", "debug", 100000, NullEntity, Projection::eOrtho, ePainterFlags::Z_CLEAR)
		{};
		~ShapeDebugImpl() {
			if (mInit) {
				point.destroy();
				line.destroy();
				fill.destroy();
			}
		}

		void init(asset<CameraImpl<C>> camera_) {
			if (!mInit && camera_) {
				mInit = true;
				fill.camera = point.camera = fill.camera = line.camera = point.camera = camera_;
				//fill.color = vec4(1, 1, 1, 0.2f);
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


	using ShapeDebug = ShapeDebugImpl<0>;
	using EditorShapeDebug = ShapeDebugImpl<1>;

	extern ELANG_DLL vector<Entity> gAtelier;
};