#include "textbox.h"

namespace el
{
	Text::Text(asset<FontFace> face, asset<Painter> painter, const string& text)
		: mFace(face), mPainter(painter), mText(text), mDepth(0), mCursor(0, 0),
		mVertices(0), mIndices(0), mVCount(0), mICount(0), mVMax(mText.size() * 4), mIMax(mText.size() * 6)
	{
		init();
	}

	void Text::init() {
		mVMax = (mVMax < 4) ? 4 * 128 : mVMax;
		mIMax = (mIMax < 4) ? 6 * 128 : mIMax;
		mVertices = (SpriteVertex*)malloc(mVMax * sizeof(SpriteVertex));
		mIndices = (sizet*)malloc(mIMax * sizeof(sizet));
	}

	asset<FontGlyph> Text::getGlyph(uint32 charid) {
		return mFace->glyphs[((mFace->glyphs.contains(charid)) ? charid : 32)];
	}

	void Text::addGlyph(FontGlyph& glyph) {
		if (mVCount + 4 > mVMax) {
			mVMax *= 2;
			mVertices = (SpriteVertex*)realloc(mVertices, mVMax * sizeof(SpriteVertex));
			mIMax *= 2;
			mIndices = (sizet*)realloc(mIndices, mIMax * sizeof(sizet));
		}

		for (sizet j = 0; j < 6; j++) {
			mIndices[mICount++] = mVCount + gBox2dFillIndices[j];
		}
		mVertices[mVCount  ].pos = vec2(glyph.left, glyph.up) + mCursor;
		mVertices[mVCount++].uv = vec2(glyph.uvLeft, glyph.uvUp);
		mVertices[mVCount  ].pos = vec2(glyph.right, glyph.up) + mCursor;
		mVertices[mVCount++].uv = vec2(glyph.uvRight, glyph.uvUp);
		mVertices[mVCount  ].pos = vec2(glyph.right, glyph.down) + mCursor;
		mVertices[mVCount++].uv = vec2(glyph.uvRight, glyph.uvDown);
		mVertices[mVCount  ].pos = vec2(glyph.left, glyph.down) + mCursor;
		mVertices[mVCount++].uv = vec2(glyph.uvLeft, glyph.uvDown);
	}

	void Text::fixGlyph(FontGlyph& glyph, sizet index) {
		auto vertex = index * 4;
		if (vertex < mVCount) {
			mVertices[vertex    ].pos = vec2(glyph.left, glyph.up) + mCursor;
			mVertices[vertex    ].uv = vec2(glyph.uvLeft, glyph.uvUp);
			mVertices[vertex + 1].pos = vec2(glyph.right, glyph.up) + mCursor;
			mVertices[vertex + 1].uv = vec2(glyph.uvRight, glyph.uvUp);
			mVertices[vertex + 2].pos = vec2(glyph.right, glyph.down) + mCursor;
			mVertices[vertex + 2].uv = vec2(glyph.uvRight, glyph.uvDown);
			mVertices[vertex + 3].pos = vec2(glyph.left, glyph.down) + mCursor;
			mVertices[vertex + 3].uv = vec2(glyph.uvLeft, glyph.uvDown);
		}
	}

	/// Textfield
	Textfield::Textfield(asset<FontFace> field, asset<Painter> painter, const string& text)
		: Text(field, painter, text), mAlign(eAlignment::LEFT) {}

	void Textfield::batch() {
		if (mPainter && mFace && mVCount > 0) {
			mPainter->batch<SpriteVertex>(
				&mVertices[0], &mIndices[0], 
				(uint32)mVCount, (uint32)mICount, (uint32)mFace->material.index(), 0, mDepth
			);
		}
	}

	void Textfield::update(Entity e)
	{
		if (mFace) {
			mVCount = 0;
			mICount = 0;

			asset<aabb> rect = e;
			mCursor = (rect) ? vec2(rect->l, rect->t) : vec2(0, 0);

			switch (mAlign) {
			case eAlignment::MID:
				mCursor.x += ((rect->width() - lineWidth()) / 2.0f);
				break;
			case eAlignment::RIGHT:
				mCursor.x += ((rect->width() - lineWidth()));
				break;
			}
			

			auto space = getGlyph(10);
			for (sizet i = 0; i < mText.size(); i++) {
				auto gl = getGlyph(mText[i]);
				if (gl != space) {
					addGlyph(*gl);
				}
					
				mCursor.x += gl->adv;
			}
		}
	}

	void Textfield::addChar(char ch) {
		mText.push_back(ch);
	}

	float Textfield::lineWidth() {
		float ladv = 0;
		for (sizet i = 0; i < mText.size(); i++) {
			ladv += getGlyph(mText[i])->adv;
		} return ladv;
	}



	/// Textbox
	Textbox::Textbox(asset<FontFace> face, asset<Painter> painter, const string& text)
		: Text(face, painter, text), spacing(0.0f), scroll(0.0f)
	{
	}

	void Textbox::batch(Entity e) {
		asset<aabb> rect = e;

		if (mPainter && mFace && mVCount > 0) {
			if (rect) {
				auto count = (sizet)ceil(rect->height() / (float)mFace->lineHeight);
				int32 fit = (int32)(mLines.size() - count);

				if (fit <= 0)
					goto default_paint;

				float scroll_limit = (float)mFace->lineHeight * mLines.size() - rect->height() - 0.01f;
				scroll = clamp(scroll, 0.0f, scroll_limit);

				auto first = (uint32)round(floor(scroll / (float)mFace->lineHeight));
				uint32 gbegin = mLines[first].gbegin;
				uint32 gend = mLines[first + count - 1].gend;
				uint32 gwidth = gend - gbegin;

				mPainter->batch<SpriteVertex>(
					&mVertices[gbegin * 4], &mIndices[0], 
					gwidth * 4, gwidth * 6, (uint32)mFace->material.index(), 0, mDepth
				);

			}
			else {
			default_paint:
				mPainter->batch<SpriteVertex>(
					&mVertices[0], &mIndices[0],
					(uint32)mVCount, (uint32)mICount, (uint32)mFace->material.index(), 0, mDepth
				);
			}
		}
	}

	void Textbox::update(Entity e) {
		if (mFace) {
			splitAllLines(e);
			wordwrap(e);
		}
	}

	void Textbox::addChar(obj<aabb> rect, char ch) {
		if (mLines.empty()) {
			mLines.emplace_back();
		}

		bool isChar = ((ch != ' ') && (ch != '\n'));
		auto& line = mLines.back();
		auto gl = getGlyph((uint32)ch);
		auto width = (line.adv + gl->adv) + spacing;
		mText.push_back(ch);

		bool wordBreak = false;

		if (width > rect->width()) {
			sizet cnt = 0;
			float ladv = 0.0f;

			if (isChar) {
				bool reachedEndOfWord = false;
				for (auto i = line.tend - 1; i >= line.tbegin; i--) {
					if (mText[i] != ' ' && mText[i] != '\n') {
						if (reachedEndOfWord) {
							wordBreak = true;
							break;
						}

						cnt++;
						ladv += getGlyph(mText[i])->adv + spacing;
					}
					else
						reachedEndOfWord = true;
				}
			}

			mLines.emplace_back();
			auto& next = mLines.back();
			auto& prev = mLines[mLines.size() - 2];

			if (wordBreak) { // always char (word break)
				prev.tend -= (uint32)cnt;
				prev.gend -= (uint32)cnt;
				prev.adv -= ladv;

				next.tbegin = prev.tend;
				next.gbegin = prev.gend;
				next.tend = next.tbegin + (uint32)cnt;
				next.gend = next.gbegin + (uint32)cnt;
				next.adv += ladv;
			} else { 
				if (!isChar)
					prev.tend++;
				next.tend = next.tbegin = prev.tend;
				next.gend = next.gbegin = prev.gend;
			}

			if (isChar) {
				mCursor = vec2(rect->l, rect->t - mFace->lineHeight * (mLines.size() - 1));
				for (sizet i = next.tbegin; i < next.tend; i++) {
					auto glyph = getGlyph(mText[i]);
					fixGlyph(*glyph, next.gbegin + (i - next.tbegin));
					mCursor.x += glyph->adv + spacing;
				}

				addGlyph(*gl);
				next.tend++;
				next.gend++;
				next.adv += gl->adv + spacing;
			}
		} else {
			if (ch == '\n') {
				mLines.emplace_back();
				auto& next = mLines.back();
				auto& prev = mLines[mLines.size() - 2];
				prev.tend++;
				next.tbegin = prev.tend;
				next.gbegin = prev.gend;
				next.tend = next.tbegin;
				next.gend = next.gbegin;
			} else {
				if (ch != ' ') {
					mCursor = vec2(rect->l + line.adv, rect->t - mFace->lineHeight * (mLines.size() - 1));
					addGlyph(*gl);
					line.gend++;
				}
				line.tend++;
				line.adv = width;
			}
		}
	}

	void Textbox::wordwrap(obj<aabb> rect) {
		mVCount = 0;
		mICount = 0;
		mCursor.y = rect->t;
		
		sizet glyphCount = 0;

		auto space = getGlyph(' ');
		for (sizet i = 0; i < mLines.size(); i++) {
			mCursor.x = rect->l;
			auto& line = mLines[i];
			line.gbegin = (uint32)glyphCount;

			for (sizet j = line.tbegin; j < line.tend; j++) {
				auto glyph = getGlyph(mText[j]);
				if (glyph != space) {
					addGlyph(*glyph);
					glyphCount++;
				}
				line.gend = (uint32)glyphCount;
				line.adv += glyph->adv + spacing;
				mCursor.x += glyph->adv + spacing;
			} 

			mCursor.y -= mFace->lineHeight;
		}
	}

	void Textbox::reinterpretLine() {
		auto& line = mLines.back();
	}

	void Textbox::splitAllLines(obj<aabb> rect) {
		mLines.clear();
		mLines.emplace_back();
		auto& line = mLines.back();

		// wbe: word beginning, lbe: line beginning, ladv: line advancement
		float ladv = 0.0f;
		sizet wbe = 0;

		for (sizet i = 0; i < mText.size(); i++) {
			auto ch = mText[i];
			bool isChar = (ch != '\n') && (ch != ' ');

			ladv += getGlyph(ch)->adv + spacing;

			if (!isChar)
				wbe = i + 1;

			// if '\n' or crossed rect width then make new line
			if ((ch == '\n') || ladv > (rect->width())) {
				ladv = 0.0f;
				// if the word starts from the beginning of line and is not ' ' or '\n'
				// make current letter the start of the new line
				auto& prev = mLines.back();
				if (prev.tbegin == wbe && isChar)
					wbe = i;
				prev.tend = (uint32)wbe;

				mLines.emplace_back();
				auto& next = mLines.back();
				next.tbegin = (uint32)wbe;
				i = wbe - 1;
			}
		}

		mLines.back().tend = (uint32)mText.size();
	}
}

