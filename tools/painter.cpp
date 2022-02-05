#include "painter.h"

namespace el
{
	uint32 Painter::sNullTextureID = -1;
	vector<Entity> gAtelier;

	Painter::Painter() : mVCount(0), mICount(0), drawtype(GL_TRIANGLES) {}
	Painter::Painter(strview vertexShader, strview fragmentShader, sizet maxVertexCount,
		asset<Camera> camera_, Projection projection_, sizet flags_) 
		: camera(camera_), projection(projection_), flags(flags_), drawtype(GL_TRIANGLES),
		color(vec4(1.0f, 1.0f, 1.0f, 1.0f)), mVCount(0), mICount(0), mTargetCount(maxVertexCount), mLocked(false)
	{
		mVert.key = vertexShader;
		mFrag.key = fragmentShader;
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

	void Painter::destroy() {
		glDeleteProgramPipelines(1, &mPipeline);
		glDeleteVertexArrays(1, &mVao);
		glDeleteBuffers(1, &mVbo);
		glDeleteBuffers(1, &mIbo);
	}

	void Painter::batchline(const line& line, const color8& c, float depth) {
		if (!mLocked && mVert.vertdata->index() == Primitive2DVertexData::sVertexDataIndex && 
			drawtype == GL_LINES) 
		{
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
		if (!mLocked && mVert.vertdata->index() == Primitive2DVertexData::sVertexDataIndex) {
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
		if (!mLocked && mVert.vertdata->index() == Primitive2DVertexData::sVertexDataIndex && box.count == 4) {
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
		if (!mLocked && mVert.vertdata->index() == Primitive2DVertexData::sVertexDataIndex &&
			drawtype == GL_LINES) 
		{
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
		if (!mLocked && mVert.vertdata->index() == Primitive2DVertexData::sVertexDataIndex) {
			auto& batch = mBatches.emplace_back();
			mBatchOrder.emplace_back(mBatchOrder.size());

			vector<Primitive2DVertex> buffer;
			sizet reader = 0;

			switch(drawtype) {
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
			if (flags & MULTI_MATERIAL) {
				if (flags & DEPTH_SORT) {
					std::sort(mBatchOrder.begin(), mBatchOrder.end(), [&](sizet a, sizet b) -> bool {
						return (mBatches[a].depth > mBatches[b].depth) 
							|| ((mBatches[a].depth == mBatches[b].depth) && mBatches[a].material < mBatches[b].material);
						});
				} else {
					std::sort(mBatchOrder.begin(), mBatchOrder.end(), [&](sizet a, sizet b) -> bool {
						return mBatches[a].material < mBatches[b].material;
						});
				}
			} else if (flags & DEPTH_SORT) {
				std::sort(mBatchOrder.begin(), mBatchOrder.end(), [&](sizet a, sizet b) -> bool {
					return  mBatches[a].depth > mBatches[b].depth;
					});
			}

			unsigned int* iBuffer = (unsigned int*)glMapNamedBuffer(mIbo, GL_WRITE_ONLY);
			auto size = mBatchOrder.size();
			for (sizet i = 0; i < size; i++) {
				auto& current = mBatches[mBatchOrder[i]];
				if (i > 0) {
					auto& previous = mBatches[mBatchOrder[i - 1]];
					if (previous.material != current.material) {
						mBumpers.push_back(mICount);
						mBumpers.push_back(current.material);
					}
				} else {
					mBumpers.push_back(0);
					mBumpers.push_back(current.material);
				}

				for (sizet i = 0; i < current.index_count; i++) {
					iBuffer[mICount + i] = (unsigned int)(current.indices[i] + mVCount);
				}
				mICount += current.index_count;
				mVCount += current.vertex_count;
			} glUnmapNamedBuffer(mIbo);

			mVCount = 0;

			for (sizet i = 0; i < size; i++) {
				auto& current = mBatches[mBatchOrder[i]];
				glBindBuffer(GL_ARRAY_BUFFER, mVbo);
				glBufferSubData(GL_ARRAY_BUFFER, mVCount * mVert.vertdata->getSize(), current.vertex_count * mVert.vertdata->getSize(), current.vertices);
				mVCount += current.vertex_count;
			}
		} mBumpers.push_back(mICount);
	}

	void Painter::paint() {
		if (!mLocked)
			sort();
		
		if (mICount > 0) {
			//bind
			glBindProgramPipeline(mPipeline);
			glBindVertexArray(mVao);
			glBindBuffer(GL_ARRAY_BUFFER, mVbo);

			if (flags & Z_CLEAR) {
				glClearDepth(1.0f);
				glClear(GL_DEPTH_BUFFER_BIT);
			}

			if (flags & Z_READ_ONLY) {
				glDepthMask(GL_FALSE);
			} else glDepthMask(GL_TRUE);


			if (camera) {
				setUniformMatrix(mVert.shader, ((projection == Projection::ePerspective) ? gPerspective : gOrtho) * camera->inversed().matrix(), "uView");
			} else {
				setUniformMatrix(mVert.shader, ((projection == Projection::ePerspective) ? gPerspective : gOrtho), "uView");
			}

			for (sizet i = 0; i < mBumpers.size() - 2; i += 2) {
				auto count = mBumpers[i + 2] - mBumpers[i];
				bindMaterial((uint32)mBumpers[i + 1]);
				glDrawElements((uint32)drawtype, (uint32)count, GL_UNSIGNED_INT, (void*)((uint32)mBumpers[i] * sizeof(unsigned int)));
			} 
		}

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

		mLocked = (flags & LOCKED) ? true : false;
		if (!mLocked) {
			mICount = mVCount = 0;
			mBumpers.clear();
		}
	}

	void Painter::bindShaderBuffer() {
		mVert = gGlsl.find(mVert.key);
		mFrag = gGlsl.find(mFrag.key);
		glGenProgramPipelines(1, &mPipeline);
		glBindProgramPipeline(mPipeline);
		glUseProgramStages(mPipeline, GL_VERTEX_SHADER_BIT, mVert.shader);
		glUseProgramStages(mPipeline, GL_FRAGMENT_SHADER_BIT, mFrag.shader);
	}
	void Painter::bindDataBuffer() {
		glGenVertexArrays(1, &mVao);
		glBindVertexArray(mVao);
		glGenBuffers(1, &mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		mVert.vertdata->vertexAttribPointer();
		mVert.vertdata->enableVertexAttribArray();
		glBufferData(GL_ARRAY_BUFFER, mTargetCount * mVert.vertdata->getSize(), 0, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mIbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mTargetCount * 8 * sizeof(unsigned int), 0, GL_DYNAMIC_DRAW);
	}
	void Painter::setUniformFloat(uint32 shader, float dat, const char* location) {
		glProgramUniform1f(shader, glGetUniformLocation(shader, location), dat);
	}
	void Painter::setUniformVec2(uint32 shader, const vec2& dat, const char* location) {
		glProgramUniform2f(shader, glGetUniformLocation(shader, location), dat.x, dat.y);
	}
	void Painter::setUniformVec3(uint32 shader, const vec3& dat, const char* location) {
		glProgramUniform3f(shader, glGetUniformLocation(shader, location), dat.x, dat.y, dat.z);
	}
	void Painter::setUniformVec4(uint32 shader, const vec4& dat, const char* location) {
		glProgramUniform4f(shader, glGetUniformLocation(shader, location), dat.x, dat.y, dat.z, dat.w);
	}
	void Painter::setUniformMatrix(uint32 shader, const matrix4x4& dat, const char* location) {
		glProgramUniformMatrix4fv(shader, glGetUniformLocation(shader, location), 1, GL_FALSE, &dat.data[0]);
	}
	void Painter::bindMaterial(uint32 material) {
		asset<Material> mate(material);

		if (mate) {
			auto& mat = *mate;

			for (sizet i = 0; i < mat.uniforms.size(); i++) {
				auto& uni = mat.uniforms[i];
				switch (uni.type) {
				case eDataType::FLOAT:
					setUniformFloat(uni.vertex ? mVert.shader : mFrag.shader, *reinterpret_cast<float*>(uni.data), uni.name.c_str());
					break;
				case eDataType::VEC2:
					setUniformVec2(uni.vertex ? mVert.shader : mFrag.shader, *reinterpret_cast<vec2*>(uni.data), uni.name.c_str());
					break;
				case eDataType::VEC3:
					setUniformVec3(uni.vertex ? mVert.shader : mFrag.shader, *reinterpret_cast<vec3*>(uni.data), uni.name.c_str());
					break;
				case eDataType::VEC4:
					setUniformVec4(uni.vertex ? mVert.shader : mFrag.shader, *reinterpret_cast<vec4*>(uni.data), uni.name.c_str());
					break;
				case eDataType::MATRIX4:
					setUniformMatrix(uni.vertex ? mVert.shader : mFrag.shader, *reinterpret_cast<matrix4x4*>(uni.data), uni.name.c_str());
					break;
				}
			}

			for (sizet i = 0; i < mat.textures.size(); ++i) {
				glActiveTexture(GL_TEXTURE0 + (uint32)i);
				if (auto tex = mat.textures[i])
					glBindTexture(GL_TEXTURE_2D, (tex->id() == -1) ? sNullTextureID : tex->id());
			}
		}
	}
}

