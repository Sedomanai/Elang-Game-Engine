/*****************************************************************//**
 * @file   textbox.h
 * @brief  Textbox (and Textfields). Currently not used
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once
#include "../tools/all.h"

namespace el
{
	//TODO: creator destructors to free memory
	template<int M, int T, int C>
	struct ELANG_DLL TextImpl
	{
		TextImpl() : mDepth(0), mCursor(0, 0),
			mVertices(0), mIndices(0), mVCount(0), mICount(0), mVMax(0), mIMax(0) {};
		TextImpl(asset<FontFaceImpl<M, T>> face, asset<PainterImpl<M, T, C>> painter, const string& text);
		
		void init();
		
		template<typename Arc>
		void save(Arc& archive) {
			archive(mDepth, mFace, mPainter, mText);
		}
		template<typename Arc>
		void load(Arc& archive) {
			archive(mDepth, mFace, mPainter, mText);
			init();
		}

	protected:
		asset<FontGlyphImpl<T>> getGlyph(uint32 charid);
		void addGlyph(FontGlyphImpl<T>& glyph);
		void fixGlyph(FontGlyphImpl<T>& glyph, sizet id);

		asset<FontFaceImpl<M, T>> mFace;
		asset<PainterImpl<M, T, C>> mPainter;
		vec2 mCursor;
		string mText;

		SpriteVertex* mVertices;
		sizet* mIndices;
		sizet mVCount, mICount;
		sizet mVMax, mIMax;
		float mDepth;
	};

	template<int M, int T, int C>
	struct ELANG_DLL TextfieldImpl : TextImpl<M, T, C>
	{
		TextfieldImpl() : TextImpl<M, T, C>() {};
		TextfieldImpl(asset<FontFaceImpl<M, T>> face, asset<PainterImpl<M, T, C>> painter, const string& text);
		void batch();
		void update(Entity e);
		void addChar(char ch);
		void align(eAlignment alignment) { mAlign = alignment; }

		template<typename Arc>
		void save(Arc& archive) const {
			archive(TextImpl<M, T, C>::mDepth, TextImpl<M, T, C>::mFace, TextImpl<M, T, C>::mPainter, 
				TextImpl<M, T, C>::mText, mAlign);
		}
		template<typename Arc>
		void load(Arc& archive) {
			archive(TextImpl<M, T, C>::mDepth, TextImpl<M, T, C>::mFace, TextImpl<M, T, C>::mPainter, 
				TextImpl<M, T, C>::mText, mAlign);
			TextImpl<M, T, C>::init();
		}

	private:
		float lineWidth();

		eAlignment mAlign;
	};

	template<int M, int T, int C>
	struct ELANG_DLL TextboxImpl : TextImpl<M, T, C>
	{
		struct Line {
			uint32 tbegin, tend;
			uint32 gbegin, gend;
			float adv;

			Line() : tbegin(0), tend(0), gbegin(0), gend(0), adv(0.0f) {}
		};

		TextboxImpl() : TextImpl<M, T, C>() {};
		TextboxImpl(asset<FontFaceImpl<M, T>> face, asset<PainterImpl<M, T, C>> painter, const string& text);
		void batch(Entity e);
		void update(Entity e);
		void addChar(obj<aabb> e, char ch);

		template<typename Arc>
		void save(Arc& archive) const {
			archive(TextImpl<M, T, C>::mDepth, TextImpl<M, T, C>::mFace, TextImpl<M, T, C>::mPainter, 
				TextImpl<M, T, C>::mText, spacing);
		}
		template<typename Arc>
		void load(Arc& archive) {
			archive(TextImpl<M, T, C>::mDepth, TextImpl<M, T, C>::mFace, TextImpl<M, T, C>::mPainter, 
				TextImpl<M, T, C>::mText, spacing);
			TextImpl<M, T, C>::init();
		}

		float spacing, scroll;
	private:
		void reinterpretLine();
		void splitAllLines(obj<aabb> rect);
		void wordwrap(obj<aabb> rect);

		vector<Line> mLines;
	};

	using Text = TextImpl<0, 0, 0>;
	using Textfield = TextfieldImpl<0, 0, 0>;
	using Textbox = TextboxImpl<0, 0, 0>;

	using EditorText = TextImpl<1, 1, 1>;
	using EditorTextfield = TextfieldImpl<1, 1, 1>;
	using EditorTextbox = TextboxImpl<1, 1, 1>;
};