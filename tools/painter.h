/*****************************************************************//**
 * @file   painter.h
 * @brief  Graphic Painter
 *		   Painter can be likened to a graphic pipeline	stage. In Elang engines, this interface is bared open.
 *		   While materials hold uniform and texture data, they do not hold shaders in Elang - only painters do.
 *		   This includes vertex/fragment shader info, layer order, sorting, z-buffer options, static batches, and so on.
 * 
 *		   Painters only accept batches based on the data size of the vertex - 
 *		   It does not care about internal data or whether it matches with the structure of the vertex.
 *		   This makes them extremely flexible, allowing them to accept multiple materials
 *		   It does also holds most common uniforms like camera or color data
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "registry.h"
#include "tool_declarer.h"
#include "project.h"
#include "camera.h"
#include "shader.h"
#include "../common/batch.h"
#include "../common/vec4.h"

inline void test() {};

namespace el
{
	struct Camera;
	struct Material;

	struct line;
	struct circle;
	struct aabb;
	struct poly2d;
	struct color8;
	struct Primitive2DVertex;

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

	/**
	 * @brief Grphic layer. A painter has its own GL Shader Pipeline, a VBO, and a VBO.
	 * @brief Can only batch Visages (and their children) with a matching vertex size
	 */
	struct ELANG_DLL Painter
	{
		Painter();
		Painter(strview vertexShader, strview fragmentShader, sizet maxVertexCount = 3000U,
			asset<Camera> camera = NullEntity, Projection projection = Projection::eOrtho, sizet flags = 0);

		static void sCreateNullTexture();

		asset<Camera> camera;
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
		void setGlobalUniform();
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

	struct ELANG_DLL ShapeDebug2d
	{
		Painter point;
		Painter line;
		Painter fill;

		ShapeDebug2d();
		~ShapeDebug2d();

		void init(asset<Camera> camera_);
		void draw();
	private:
		bool mInit;
	};

	extern ELANG_DLL vector<Entity> gAtelier;
};