#include <elpch.h>
#include "painter.h"
#include "vertex.h"
#include "material.h"
#include "texture.h"
#include "asset_database.h"
#include "../common/line.h"
#include "../common/aabb.h"
#include "../common/poly2d.h"
#include "../common/circle.h"

namespace el
{
	uint32 Painter::sNullTextureID = -1;
	vector<Entity> gAtelier;

	Painter::Painter() : drawtype(GL_TRIANGLES) {}


	Painter::Painter(strview vertexShader, strview fragmentShader, sizet maxVertexCount,
		asset<Camera> camera_, Projection projection_, sizet flags_)
		: camera(camera_), projection(projection_), flags(flags_), drawtype(GL_TRIANGLES),
		color(vec4(1.0f, 1.0f, 1.0f, 1.0f)), mTargetCount(maxVertexCount), mLocked(false) {
		mVertLabel = vertexShader;
		mFragLabel = fragmentShader;
#ifdef QOPENGLVERSIONFUNCTIONS_4_5_CORE_H
		initializeOpenGLFunctions();
#endif
	}


	void Painter::sCreateNullTexture() {
		glGenTextures(1, &sNullTextureID);
		glBindTexture(GL_TEXTURE_2D, sNullTextureID);

		color8 c = color8(255, 0, 255, 255);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &c);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}


	void Painter::init() {
		bindShaderBuffer();
		bindDataBuffer();
	}


	void Painter::destroy() {
		glDeleteProgramPipelines(1, &mPipeline);
		glDeleteVertexArrays(1, &mVao);
		glDeleteBuffers(1, &mVbo);
		glDeleteBuffers(1, &mIbo);
	}


	void Painter::batchline(const line& line, const color8& c, float depth) {
		if (!mLocked && mVert->size() == sizeof(Primitive2DVertex) &&
			drawtype == GL_LINES) {
			auto& batch = mBatches.emplace_back(
				(Primitive2DVertex*)malloc(sizeof(Primitive2DVertex) * 2),
				&gLineIndices[0],
				2, 2, entt::id_type(NullEntity), E_DANGLING_VERTICES, depth
			);
			mBatchOrder.emplace_back(mBatchOrder.size());

			Primitive2DVertex* data = (Primitive2DVertex*)batch.vertices;
			data[0].pos = line.p1();
			data[0].col = c;
			data[1].pos = line.p2();
			data[1].col = c;
		}
	}


	void Painter::batchAABB(const aabb& aabb, const color8& c, float depth) {
		if (!mLocked && mVert->size() == sizeof(Primitive2DVertex)) {
			switch (drawtype) {
				case GL_LINES:
					mBatches.emplace_back(
						malloc(sizeof(Primitive2DVertex) * 4), &gBox2dLineIndices[0],
						4, 8, entt::id_type(NullEntity), E_DANGLING_VERTICES, depth
					);
					break;
				case GL_TRIANGLES:
					mBatches.emplace_back(
						malloc(sizeof(Primitive2DVertex) * 4), &gBox2dFillIndices[0],
						4, 6, entt::id_type(NullEntity), E_DANGLING_VERTICES, depth
					);
					break;
				default:
					return;
			}
			mBatchOrder.emplace_back(mBatchOrder.size());

			Primitive2DVertex* data = (Primitive2DVertex*)mBatches.back().vertices;
			data[0].pos = { aabb.l, aabb.t };
			data[0].col = c;
			data[1].pos = { aabb.r, aabb.t };
			data[1].col = c;
			data[2].pos = { aabb.r, aabb.b };
			data[2].col = c;
			data[3].pos = { aabb.l, aabb.b };
			data[3].col = c;
		}
	}


	void Painter::batchBox(const poly2d& box, const color8& c, float depth) {
		if (!mLocked && mVert->size() == sizeof(Primitive2DVertex) && box.count == 4) {
			switch (drawtype) {
				case GL_LINES:
					mBatches.emplace_back(
						malloc(sizeof(Primitive2DVertex) * 4), &gBox2dLineIndices[0],
						4, 8, entt::id_type(NullEntity), E_DANGLING_VERTICES, depth
					);
					break;
				case GL_TRIANGLES:
					mBatches.emplace_back(
						malloc(sizeof(Primitive2DVertex) * 4), &gBox2dFillIndices[0],
						4, 6, entt::id_type(NullEntity), E_DANGLING_VERTICES, depth
					);
					break;
				default:
					return;
			}
			mBatchOrder.emplace_back(mBatchOrder.size());

			Primitive2DVertex* data = (Primitive2DVertex*)mBatches.back().vertices;
			for (sizet i = 0; i < 4; i++) {
				data[i].pos = box.verts[i];
				data[i].col = c;
			}
		}
	}


	void Painter::batchPoly(const poly2d& poly, const color8& c, float depth) {
		if (!mLocked && mVert->size() == sizeof(Primitive2DVertex) &&
			drawtype == GL_LINES) {
			auto& batch = mBatches.emplace_back(
				malloc(sizeof(Primitive2DVertex) * poly.count),
				(sizet*)malloc(poly.count * 2 * sizeof(sizet)),
				(uint32)poly.count, (uint32)poly.count * 2,
				entt::id_type(NullEntity), E_DANGLING_INDICES | E_DANGLING_VERTICES, depth
			);
			mBatchOrder.emplace_back(mBatchOrder.size());

			Primitive2DVertex* data = (Primitive2DVertex*)batch.vertices;
			for (sizet i = 0; i < batch.vertex_count; i++) {
				data[i].pos = poly.verts[i];
				data[i].col = c;
			}
			sizet reader = 0;
			for (sizet i = 0; i < batch.vertex_count - 1; i++) {
				batch.indices[reader++] = i;
				batch.indices[reader++] = (i + 1);
			}
		}
	}


	void Painter::batchCircle(const circle& circ, const color8& c, float depth) {
		if (!mLocked && mVert->size() == sizeof(Primitive2DVertex)) {
			auto& batch = mBatches.emplace_back();
			mBatchOrder.emplace_back(mBatchOrder.size());

			vector<Primitive2DVertex> buffer;
			sizet reader = 0;

			switch (drawtype) {
				case GL_POINTS:
					float x = 0;
					float y = circ.r;
					float p = (5 - circ.r * 4) / 4;

					circlePoints(buffer, c, circ.x, circ.y, x, y);
					while (x < y) {
						x++;
						if (p < 0) {
							p += 2 * x + 1;
						} else {
							y--;
							p += 2 * (x - y) + 1;
						}
						circlePoints(buffer, c, circ.x, circ.y, x, y);
					}

					batch.vertex_count = (uint32)buffer.size();
					batch.vertices = malloc(batch.vertex_count * sizeof(Primitive2DVertex));
					memcpy(batch.vertices, buffer.data(), batch.vertex_count * sizeof(Primitive2DVertex));

					batch.index_count = ((uint32)buffer.size() - 1) * 2;
					batch.indices = (sizet*)malloc(batch.index_count * sizeof(sizet));

					for (sizet i = 0; i < batch.vertex_count - 2; i++) {
						batch.indices[reader++] = i;
						batch.indices[reader++] = i + 1;
					}
					batch.indices[reader++] = (batch.vertex_count - 1);
					batch.indices[reader++] = 0;
					assert(reader == batch.index_count);

					batch.material = entt::id_type(NullEntity);
					batch.depth = -1.0f;
					batch.flag = E_DANGLING_INDICES | E_DANGLING_VERTICES;
					break;
			}
		}
	}


	void Painter::circlePoints(vector<Primitive2DVertex>& buffer, const color8& c, float cx, float cy, float x, float y) {
		if (x == 0) {
			buffer.emplace_back(Primitive2DVertex{ vec2(cx, cy + y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx, cy - y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx + y, cy), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx - y, cy), c });
		} else if (x == y) {
			buffer.emplace_back(Primitive2DVertex{ vec2(cx + x, cy + y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx - x, cy + y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx + x, cy - y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx - x, cy - y), c });
		} else if (x < y) {
			buffer.emplace_back(Primitive2DVertex{ vec2(cx + x, cy + y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx - x, cy + y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx + x, cy - y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx - x, cy - y), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx + y, cy + x), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx - y, cy + x), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx + y, cy - x), c });
			buffer.emplace_back(Primitive2DVertex{ vec2(cx - y, cy - x), c });
		}
	}


	void Painter::sort() {
		if (!mBatches.empty()) {
			if (flags & ePainterFlags::MULTI_MATERIAL) {
				if (flags & ePainterFlags::DEPTH_SORT) {
					std::sort(mBatchOrder.begin(), mBatchOrder.end(), [&](sizet a, sizet b) -> bool {
						return (mBatches[a].depth > mBatches[b].depth)
							|| ((mBatches[a].depth == mBatches[b].depth) && mBatches[a].material < mBatches[b].material);
						});
				} else {
					std::sort(mBatchOrder.begin(), mBatchOrder.end(), [&](sizet a, sizet b) -> bool {
						return mBatches[a].material < mBatches[b].material;
						});
				}
			} else if (flags & ePainterFlags::DEPTH_SORT) {
				std::sort(mBatchOrder.begin(), mBatchOrder.end(), [&](sizet a, sizet b) -> bool {
					return  mBatches[a].depth > mBatches[b].depth;
					});
			}
		}
	}


	void Painter::flush() {
		sizet iCount = 0;
		if (!mBatches.empty()) {
			sizet vCount = 0;
			unsigned int* iBuffer = (unsigned int*)glMapNamedBuffer(mIbo, GL_WRITE_ONLY);
			auto size = mBatchOrder.size();
			for (sizet i = 0; i < size; i++) {
				auto& current = mBatches[mBatchOrder[i]];
				if (i > 0) {
					auto& previous = mBatches[mBatchOrder[i - 1]];
					if (previous.material != current.material) {
						mBumpers.push_back(iCount);
						mBumpers.push_back(current.material);
					}
				} else {
					mBumpers.push_back(0);
					mBumpers.push_back(current.material);
				}

				for (sizet i = 0; i < current.index_count; i++) {
					iBuffer[iCount + i] = (unsigned int)(current.indices[i] + vCount);
				}
				iCount += current.index_count;
				vCount += current.vertex_count;
			} glUnmapNamedBuffer(mIbo);

			vCount = 0;

			for (sizet i = 0; i < size; i++) {
				auto& current = mBatches[mBatchOrder[i]];
				glBindBuffer(GL_ARRAY_BUFFER, mVbo);
				glBufferSubData(GL_ARRAY_BUFFER, vCount * mVert->size(), current.vertex_count * mVert->size(), current.vertices);
				vCount += current.vertex_count;
			}
		} mBumpers.push_back(iCount);
	}


	// change name to setGlobalUniform

	void Painter::setGlobalUniform() {
		setUniformVec4(mVert->shader(), color, "uColor");
		if (camera) {
			setUniformMatrix(mVert->shader(), ((projection == Projection::ePerspective) ? gPerspective : gOrtho) * camera->inversed().matrix(), "uView");
		} else {
			setUniformMatrix(mVert->shader(), ((projection == Projection::ePerspective) ? gPerspective : gOrtho), "uView");
		}
	}



	void Painter::clear() {
		for (sizet i = 0; i < mBatchOrder.size(); i++) {
			auto& batch = mBatches[mBatchOrder[i]];
			if ((batch.flag & E_DANGLING_VERTICES) == E_DANGLING_VERTICES) {
				free(batch.vertices);
			}
			if ((batch.flag & E_DANGLING_INDICES) == E_DANGLING_INDICES) {
				free(batch.indices);
			}
		}

		mBatches.clear();
		mBatchOrder.clear();

		mLocked = (flags & ePainterFlags::LOCKED) ? true : false;
		if (!mLocked) {
			mBumpers.clear();
		}
	}


	void Painter::paint() {
		bool vaobind = false;
		if (!mLocked) {
			glBindVertexArray(mVao);
			sort();
			flush();
			vaobind = true;
		}

		if (mBumpers.size() > 1) {
			//bindAll
			glBindProgramPipeline(mPipeline);
			if (!vaobind) glBindVertexArray(mVao);
			glBindBuffer(GL_ARRAY_BUFFER, mVbo);

			if (flags & ePainterFlags::Z_CLEAR) {
				glClearDepth(1.0f);
				glClear(GL_DEPTH_BUFFER_BIT);
			}

			if (flags & ePainterFlags::Z_READ_ONLY) {
				glDepthMask(GL_FALSE);
			} else glDepthMask(GL_TRUE);

			setGlobalUniform();

			for (sizet i = 0; i < mBumpers.size() - 2; i += 2) {
				auto count = mBumpers[i + 2] - mBumpers[i];
				bindMaterial((uint32)mBumpers[i + 1]);
				glDrawElements((GLenum)drawtype, (GLsizei)count, GL_UNSIGNED_INT, (void*)((sizet)mBumpers[i] * sizeof(unsigned int)));
			}
		}

		clear();
	}


	void Painter::forceUnlock() {
		mLocked = false;
		flags &= ~ePainterFlags::LOCKED;
		mBatches.clear();
		mBatchOrder.clear();
		mBumpers.clear();
	}


	void Painter::bindShaderBuffer() {
		if (gShaders.names.contains(mVertLabel)) {
			mVert = &*(asset<VertexShader>(gShaders.names[mVertLabel]));
		} else {
			cout << "Vertex Shader " << mVertLabel << " does not exist." << endl;
			return;
		}
		if (gShaders.names.contains(mFragLabel)) {
			mFrag = &*(asset<FragmentShader>(gShaders.names[mFragLabel]));
		} else {
			cout << "Fragment Shader " << mFragLabel << " does not exist." << endl;
			return;
		}

		glGenProgramPipelines(1, &mPipeline);
		glBindProgramPipeline(mPipeline);
		glUseProgramStages(mPipeline, GL_VERTEX_SHADER_BIT, mVert->shader());
		glUseProgramStages(mPipeline, GL_FRAGMENT_SHADER_BIT, mFrag->shader());
	}

	void Painter::bindDataBuffer() {
		glGenVertexArrays(1, &mVao);
		glBindVertexArray(mVao);
		glGenBuffers(1, &mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		mVert->vertexAttribPointer();
		mVert->enableVertexAttribArray();
		glBufferData(GL_ARRAY_BUFFER, mTargetCount * mVert->size(), 0, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mIbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mTargetCount * 8 * sizeof(uint), 0, GL_DYNAMIC_DRAW);
	}

	void Painter::setUniformFloat(uint shader, float dat, const char* location) {
		glProgramUniform1f(shader, glGetUniformLocation(shader, location), dat);
	}

	void Painter::setUniformVec2(uint shader, const vec2& dat, const char* location) {
		glProgramUniform2f(shader, glGetUniformLocation(shader, location), dat.x, dat.y);
	}

	void Painter::setUniformVec3(uint shader, const vec3& dat, const char* location) {
		glProgramUniform3f(shader, glGetUniformLocation(shader, location), dat.x, dat.y, dat.z);
	}

	void Painter::setUniformVec4(uint shader, const vec4& dat, const char* location) {
		glProgramUniform4f(shader, glGetUniformLocation(shader, location), dat.x, dat.y, dat.z, dat.w);
	}

	void Painter::setUniformMatrix(uint shader, const matrix4x4& dat, const char* location) {
		glProgramUniformMatrix4fv(shader, glGetUniformLocation(shader, location), 1, GL_FALSE, &dat.data[0]);
	}

	void Painter::bindMaterial(uint32 material) {
		asset<Material> mate(material);
		if (mate) {
			auto& mat = *mate;
			for (sizet i = 0; i < mat.uniforms.size(); i++) {
				auto& uni = mat.uniforms[i];
				switch (uni.type) {
					case eDataType::Float:
						setUniformFloat(uni.vertex ? mVert->shader() : mFrag->shader(), *reinterpret_cast<float*>(uni.data), uni.name.c_str());
						break;
					case eDataType::Vec2:
						setUniformVec2(uni.vertex ? mVert->shader() : mFrag->shader(), *reinterpret_cast<vec2*>(uni.data), uni.name.c_str());
						break;
					case eDataType::Vec3:
						setUniformVec3(uni.vertex ? mVert->shader() : mFrag->shader(), *reinterpret_cast<vec3*>(uni.data), uni.name.c_str());
						break;
					case eDataType::Vec4:
						setUniformVec4(uni.vertex ? mVert->shader() : mFrag->shader(), *reinterpret_cast<vec4*>(uni.data), uni.name.c_str());
						break;
					case eDataType::Matrix4:
						setUniformMatrix(uni.vertex ? mVert->shader() : mFrag->shader(), *reinterpret_cast<matrix4x4*>(uni.data), uni.name.c_str());
						break;
				}
			}

			for (sizet i = 0; i < mat.textures.size(); ++i) {
				glActiveTexture(GL_TEXTURE0 + (GLenum)i);
				if (auto tex = mat.textures[i]) {
					auto id = (tex->id() == -1) ? sNullTextureID : tex->id();
					glBindTexture(GL_TEXTURE_2D, id);
				}
			}
		}
	}

	/////

	ShapeDebug2d::ShapeDebug2d() : mInit(false),
		point("__el_editor_/shader/debug2d.vert", "__el_editor_/shader/debug2d.frag", 100000, NullEntity, Projection::eOrtho, ePainterFlags::Z_CLEAR),
		line("__el_editor_/shader/debug2d.vert", "__el_editor_/shader/debug2d.frag", 100000, NullEntity, Projection::eOrtho, ePainterFlags::Z_CLEAR),
		fill("__el_editor_/shader/debug2d.vert", "__el_editor_/shader/debug2d.frag", 100000, NullEntity, Projection::eOrtho, ePainterFlags::Z_CLEAR) {
	};

	ShapeDebug2d::~ShapeDebug2d() {
		if (mInit) {
			point.destroy();
			line.destroy();
			fill.destroy();
		}
	}

	void ShapeDebug2d::init(asset<Camera> camera_) {
		if (!mInit && camera_) {
			mInit = true;
			fill.camera = point.camera = fill.camera = line.camera = point.camera = camera_;
			//fill.color = vec4(1, 1, 1, 0.2f);
			point.drawtype = 0x0000; // GL_POINTS
			line.drawtype = GL_LINES;
			point.init();
			line.init();
			fill.init();
		}
	}

	void ShapeDebug2d::draw() {
		// order matters
		point.paint();
		line.paint();
		fill.paint();
	}
}

