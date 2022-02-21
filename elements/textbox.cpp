#include "textbox.h"

namespace el
{

	template struct TextImpl<0, 0, 0>;
	template struct TextfieldImpl<0, 0, 0>;
	template struct TextboxImpl<0, 0, 0>;

	template struct TextImpl<1, 1, 1>;
	template struct TextfieldImpl<1, 1, 1>;
	template struct TextboxImpl<1, 1, 1>;

	template<int M, int T, int C>
	TextImpl<M, T, C>::TextImpl(asset<FontFaceImpl<M, T>> face, asset<PainterImpl<M, T, C>> painter, const string& text)
		: mFace(face), mPainter(painter), mText(text), mDepth(0), mCursor(0, 0),
		mVertices(0), mIndices(0), mVCount(0), mICount(0), mVMax(mText.size() * 4), mIMax(mText.size() * 6)
	{
		init();
	}

	template<int M, int T, int C>
	void TextImpl<M, T, C>::init() {
		mVMax = (mVMax < 4) ? 4 * 128 : mVMax;
		mIMax = (mIMax < 4) ? 6 * 128 : mIMax;
		mVertices = (SpriteVertex*)malloc(mVMax * sizeof(SpriteVertex));
		mIndices = (sizet*)malloc(mIMax * sizeof(sizet));
	}

	template<int M, int T, int C>
	asset<FontGlyphImpl<T>> TextImpl<M, T, C>::getGlyph(uint32 charid) {
		return mFace->glyphs[((mFace->glyphs.contains(charid)) ? charid : 32)];
	}

	template<int M, int T, int C>
	void TextImpl<M, T, C>::addGlyph(FontGlyphImpl<T>& glyph) {
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

	template<int M, int T, int C>
	void TextImpl<M, T, C>::fixGlyph(FontGlyphImpl<T> & glyph, sizet index) {
		auto vertex = index * 4;
		if (vertex < TextImpl<M, T, C>::mVCount) {
			mVertices[vertex    ].pos = vec2(glyph.left, glyph.up) + TextImpl<M, T, C>::mCursor;
			mVertices[vertex    ].uv = vec2(glyph.uvLeft, glyph.uvUp);
			mVertices[vertex + 1].pos = vec2(glyph.right, glyph.up) + TextImpl<M, T, C>::mCursor;
			mVertices[vertex + 1].uv = vec2(glyph.uvRight, glyph.uvUp);
			mVertices[vertex + 2].pos = vec2(glyph.right, glyph.down) + TextImpl<M, T, C>::mCursor;
			mVertices[vertex + 2].uv = vec2(glyph.uvRight, glyph.uvDown);
			mVertices[vertex + 3].pos = vec2(glyph.left, glyph.down) + TextImpl<M, T, C>::mCursor;
			mVertices[vertex + 3].uv = vec2(glyph.uvLeft, glyph.uvDown);
		}
	}

	/// TextfieldImpl<M, T, C>
	template<int M, int T, int C>
	TextfieldImpl<M, T, C>::TextfieldImpl(asset<FontFaceImpl<M, T>> field, asset<PainterImpl<M, T, C>> painter, const string& text)
		: TextImpl<M, T, C>(field, painter, text), mAlign(eAlignment::LEFT) {}

	template<int M, int T, int C>
	void TextfieldImpl<M, T, C>::batch() {
		if (TextImpl<M, T, C>::mPainter && TextImpl<M, T, C>::mFace && TextImpl<M, T, C>::mVCount > 0) {
			TextImpl<M, T, C>::mPainter->batch<SpriteVertex>(
				&TextImpl<M, T, C>::mVertices[0], &TextImpl<M, T, C>::mIndices[0], 
				(uint32)TextImpl<M, T, C>::mVCount, (uint32)TextImpl<M, T, C>::mICount, (uint32)TextImpl<M, T, C>::mFace->material.index(), 0, TextImpl<M, T, C>::mDepth
			);
		}
	}

	template<int M, int T, int C>
	void TextfieldImpl<M, T, C>::update(Entity e)
	{
		if (TextImpl<M, T, C>::mFace) {
			TextImpl<M, T, C>::mVCount = 0;
			TextImpl<M, T, C>::mICount = 0;

			asset<aabb> rect = e;
			TextImpl<M, T, C>::mCursor = (rect) ? vec2(rect->l, rect->t) : vec2(0, 0);

			switch (mAlign) {
			case eAlignment::MID:
				TextImpl<M, T, C>::mCursor.x += ((rect->width() - lineWidth()) / 2.0f);
				break;
			case eAlignment::RIGHT:
				TextImpl<M, T, C>::mCursor.x += ((rect->width() - lineWidth()));
				break;
			}
			

			auto space = TextImpl<M, T, C>::getGlyph(10);
			for (sizet i = 0; i < TextImpl<M, T, C>::TextImpl<M, T, C>::mText.size(); i++) {
				auto gl = TextImpl<M, T, C>::getGlyph(TextImpl<M, T, C>::mText[i]);
				if (gl != space) {
					TextImpl<M, T, C>::addGlyph(*gl);
				}
					
				TextImpl<M, T, C>::mCursor.x += gl->adv;
			}
		}
	}

	template<int M, int T, int C>
	void TextfieldImpl<M, T, C>::addChar(char ch) {
		TextImpl<M, T, C>::mText.push_back(ch);
	}

	template<int M, int T, int C>
	float TextfieldImpl<M, T, C>::lineWidth() {
		float ladv = 0;
		for (sizet i = 0; i < TextImpl<M, T, C>::mText.size(); i++) {
			ladv += TextImpl<M, T, C>::getGlyph(TextImpl<M, T, C>::mText[i])->adv;
		} return ladv;
	}



	/// Textbox
	template<int M, int T, int C>
	TextboxImpl<M, T, C>::TextboxImpl(asset<FontFaceImpl<M, T>> face, asset<PainterImpl<M, T, C>> painter, const string& text)
		: TextImpl<M, T, C>(face, painter, text), spacing(0.0f), scroll(0.0f)
	{
	}

	template<int M, int T, int C>
	void TextboxImpl<M, T, C>::batch(Entity e) {
		asset<aabb> rect = e;

		if (TextImpl<M, T, C>::mPainter && TextImpl<M, T, C>::mFace && TextImpl<M, T, C>::mVCount > 0) {
			if (rect) {
				auto count = (sizet)ceil(rect->height() / (float)TextImpl<M, T, C>::mFace->lineHeight);
				int32 fit = (int32)(mLines.size() - count);

				if (fit <= 0)
					goto default_paint;

				float scroll_limit = (float)TextImpl<M, T, C>::mFace->lineHeight * mLines.size() - rect->height() - 0.01f;
				scroll = clamp(scroll, 0.0f, scroll_limit);

				auto first = (uint32)round(floor(scroll / (float)TextImpl<M, T, C>::mFace->lineHeight));
				uint32 gbegin = mLines[first].gbegin;
				uint32 gend = mLines[first + count - 1].gend;
				uint32 gwidth = gend - gbegin;

				TextImpl<M, T, C>::mPainter->batch<SpriteVertex>(
					&TextImpl<M, T, C>::mVertices[gbegin * 4], &TextImpl<M, T, C>::mIndices[0], 
					gwidth * 4, gwidth * 6, (uint32)TextImpl<M, T, C>::mFace->material.index(), 0, TextImpl<M, T, C>::mDepth
				);

			}
			else {
			default_paint:
				TextImpl<M, T, C>::mPainter->batch<SpriteVertex>(
					&TextImpl<M, T, C>::mVertices[0], &TextImpl<M, T, C>::mIndices[0],
					(uint32)TextImpl<M, T, C>::mVCount, (uint32)TextImpl<M, T, C>::mICount, (uint32)TextImpl<M, T, C>::mFace->material.index(), 0, TextImpl<M, T, C>::mDepth
				);
			}
		}
	}

	template<int M, int T, int C>
	void TextboxImpl<M, T, C>::update(Entity e) {
		if (TextImpl<M, T, C>::mFace) {
			splitAllLines(e);
			wordwrap(e);
		}
	}

	template<int M, int T, int C>
	void TextboxImpl<M, T, C>::addChar(obj<aabb> rect, char ch) {
		if (mLines.empty()) {
			mLines.emplace_back();
		}

		bool isChar = ((ch != ' ') && (ch != '\n'));
		auto& line = mLines.back();
		auto gl = TextImpl<M, T, C>::getGlyph((uint32)ch);
		auto width = (line.adv + gl->adv) + spacing;
		TextImpl<M, T, C>::mText.push_back(ch);

		bool wordBreak = false;

		if (width > rect->width()) {
			sizet cnt = 0;
			float ladv = 0.0f;

			if (isChar) {
				bool reachedEndOfWord = false;
				for (auto i = line.tend - 1; i >= line.tbegin; i--) {
					if (TextImpl<M, T, C>::mText[i] != ' ' && TextImpl<M, T, C>::mText[i] != '\n') {
						if (reachedEndOfWord) {
							wordBreak = true;
							break;
						}

						cnt++;
						ladv += TextImpl<M, T, C>::getGlyph(TextImpl<M, T, C>::mText[i])->adv + spacing;
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
				TextImpl<M, T, C>::mCursor = vec2(rect->l, rect->t - TextImpl<M, T, C>::mFace->lineHeight * (mLines.size() - 1));
				for (sizet i = next.tbegin; i < next.tend; i++) {
					auto glyph = TextImpl<M, T, C>::getGlyph(TextImpl<M, T, C>::TextImpl<M, T, C>::mText[i]);
					TextImpl<M, T, C>::fixGlyph(*glyph, next.gbegin + (i - next.tbegin));
					TextImpl<M, T, C>::mCursor.x += glyph->adv + spacing;
				}

				TextImpl<M, T, C>::addGlyph(*gl);
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
					TextImpl<M, T, C>::mCursor = vec2(rect->l + line.adv, rect->t - TextImpl<M, T, C>::mFace->lineHeight * (mLines.size() - 1));
					TextImpl<M, T, C>::addGlyph(*gl);
					line.gend++;
				}
				line.tend++;
				line.adv = width;
			}
		}
	}

	template<int M, int T, int C>
	void TextboxImpl<M, T, C>::wordwrap(obj<aabb> rect) {
		TextImpl<M, T, C>::mVCount = 0;
		TextImpl<M, T, C>::mICount = 0;
		TextImpl<M, T, C>::mCursor.y = rect->t;
		
		sizet glyphCount = 0;

		auto space = TextImpl<M, T, C>::getGlyph(' ');
		for (sizet i = 0; i < mLines.size(); i++) {
			TextImpl<M, T, C>::mCursor.x = rect->l;
			auto& line = mLines[i];
			line.gbegin = (uint32)glyphCount;

			for (sizet j = line.tbegin; j < line.tend; j++) {
				auto glyph = TextImpl<M, T, C>::getGlyph(TextImpl<M, T, C>::mText[j]);
				if (glyph != space) {
					TextImpl<M, T, C>::addGlyph(*glyph);
					glyphCount++;
				}
				line.gend = (uint32)glyphCount;
				line.adv += glyph->adv + spacing;
				TextImpl<M, T, C>::mCursor.x += glyph->adv + spacing;
			} 

			TextImpl<M, T, C>::mCursor.y -= TextImpl<M, T, C>::mFace->lineHeight;
		}
	}

	template<int M, int T, int C>
	void TextboxImpl<M, T, C>::reinterpretLine() {
		auto& line = mLines.back();
	}

	template<int M, int T, int C>
	void TextboxImpl<M, T, C>::splitAllLines(obj<aabb> rect) {
		mLines.clear();
		mLines.emplace_back();
		auto& line = mLines.back();

		// wbe: word beginning, lbe: line beginning, ladv: line advancement
		float ladv = 0.0f;
		sizet wbe = 0;

		for (sizet i = 0; i < TextImpl<M, T, C>::mText.size(); i++) {
			auto ch = TextImpl<M, T, C>::mText[i];
			bool isChar = (ch != '\n') && (ch != ' ');

			ladv += TextImpl<M, T, C>::getGlyph(ch)->adv + spacing;

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

		mLines.back().tend = (uint32)TextImpl<M, T, C>::mText.size();
	}
}

