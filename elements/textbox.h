#pragma once

#include "../tools/bmpfont.h"
#include "../tools/painter.h"
#include "../common/enums.h"

namespace el
{
	//TODO: creator destructors to free memory
	struct ELANG_DLL Text
	{
		Text() : mDepth(0), mCursor(0, 0),
			mVertices(0), mIndices(0), mVCount(0), mICount(0), mVMax(0), mIMax(0) {};
		Text(asset<FontFace> face, asset<Painter> painter, const string& text);
		
		void init();
		
		template<typename T>
		void save(T& archive) {
			archive(mDepth, mFace, mPainter, mText);
		}
		template<typename T>
		void load(T& archive) {
			archive(mDepth, mFace, mPainter, mText);
			init();
		}

	protected:
		asset<FontGlyph> getGlyph(uint32 charid);
		void addGlyph(FontGlyph& glyph);
		void fixGlyph(FontGlyph& glyph, sizet id);

		asset<FontFace> mFace;
		asset<Painter> mPainter;
		vec2 mCursor;
		string mText;

		SpriteVertex* mVertices;
		sizet* mIndices;
		sizet mVCount, mICount;
		sizet mVMax, mIMax;
		float mDepth;
	};

	struct ELANG_DLL Textfield : Text
	{
		Textfield() : Text() {};
		Textfield(asset<FontFace> face, asset<Painter> painter, const string& text);
		void batch();
		void update(Entity e);
		void addChar(char ch);
		void align(eAlignment alignment) { mAlign = alignment; }

		template<typename T>
		void save(T& archive) const {
			archive(mDepth, mFace, mPainter, mText, mAlign);
		}
		template<typename T>
		void load(T& archive) {
			archive(mDepth, mFace, mPainter, mText, mAlign);
			init();
		}

	private:
		float lineWidth();

		eAlignment mAlign;
	};

	struct ELANG_DLL Textbox : Text
	{
		struct Line {
			uint32 tbegin, tend;
			uint32 gbegin, gend;
			float adv;

			Line() : tbegin(0), tend(0), gbegin(0), gend(0), adv(0.0f) {}
		};

		Textbox() : Text() {};
		Textbox(asset<FontFace> face, asset<Painter> painter, const string& text);
		void batch(Entity e);
		void update(Entity e);
		void addChar(obj<aabb> e, char ch);

		template<typename T>
		void save(T& archive) const {
			archive(mDepth, mFace, mPainter, mText, spacing);
		}
		template<typename T>
		void load(T& archive) {
			archive(mDepth, mFace, mPainter, mText, spacing);
			init();
		}

		float spacing, scroll;
	private:
		void reinterpretLine();
		void splitAllLines(obj<aabb> rect);
		void wordwrap(obj<aabb> rect);

		vector<Line> mLines;
	};
};