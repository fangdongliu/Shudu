#pragma once
#include<string>
#include"predef.h"
#include"AnimUITypes.h"
#include"Animation.h"
namespace AnimUI {
	enum UIDisplayMode {
		UIDisplayModeBlock,
		UIDisplayModeInline,
	};
	enum AttributionFlag {
		AttributionFlag_isWidthOfChild,
		AttributionFlag_isHeightOfChild,
		AttributionFlag_align,
		AttributionFlag_border,
		AttributionFlag_background,
		AttributionFlag_margin,
		AttributionFlag_padding,
		AttributionFlag_sizeCaculator,
		AttributionFlag_transform,
		AttributionFlag_displayMode,
		AttributionFlag_textColor,
		AttributionFlag_fontSize,
		AttributionFlag_fontFamily,
		AttributionFlag_borderRadius,
		AttributionFlag_lineHeight,
		AttributionFlag_opacity,
		AttributionFlag_duration,
		AttributionFlag_cursor
	};
	enum UIState {
		UIStateNormal,
		UIStateHover,
		UIStateCapture,
		UIStateFocus
	};

	struct UITransform {
		float rotateAngle;
		D2D1_POINT_2F rotateCenter;
		D2D1_SIZE_F transform;
		D2D1_SIZE_F scale;
	};
	struct UIBackground {
		D2D1_COLOR_F backgroundColor;
		std::wstring backgroundImage;
	};
	struct UIBorder {
		D2D1_COLOR_F borderColor;
		float borderSize;
		bool hasLeft, hasTop, hasRight, hasBottom;
	};
	
	
	class UIStyle:public AutoDelete
	{
	public:
		UIStyle() {
			FillDefault();
		}
	public:	
		bool				isWidthOfChild;
		bool				isHeightOfChild;
		char				alignMode;
		SizeF				align;
		UIBorder			border;
		UIBackground		background;
		RECTF				margin;
		RECTF				padding;
		SizeCaculator		sizeCaculator;
		UITransform			transform;
		UIDisplayMode		displayMode;
		D2D1_COLOR_F		textColor;
		float				fontSize;	
		std::wstring		fontFamily;
		float				borderRadius;
		float				lineHeight;
		float				opacity;
		float				duration;
		unsigned int		attributionFlags;
		HCURSOR				cursor;
		void FillDefault() {
			isWidthOfChild = false;
			isHeightOfChild = false;
			attributionFlags = 0;
			duration = 0.0f;
			align = { 0,0 };
			margin = { 0,0,0,0 };
			padding = { 0,0,0,0 };
			sizeCaculator = { 0,0,0,0,0 };
			fontSize = 16.0f;
			textColor = D2D1::ColorF(D2D1::ColorF::Black);
			ZeroMemory(&border, sizeof(border));
			displayMode = UIDisplayMode::UIDisplayModeInline;
			alignMode = 3;
			borderRadius = 0.0f;
			lineHeight = 0.0f;
			background.backgroundColor.a = 0;
			opacity = 1.0f;
			transform = { .0f,0.5f,0.5f,1.f,1.f,0.f,0.f };
			cursor = LoadCursor(0, IDC_ARROW);
		}
		void SafeCopy(UIStyle*style) {

			if (!style)return;

			attributionFlags = style->attributionFlags;

			if (IsFlagValid(AttributionFlag_cursor)) {
				cursor = style->cursor;
			}
			if (IsFlagValid(AttributionFlag_isWidthOfChild)) {
				isWidthOfChild = style->isWidthOfChild;
			}
			if (IsFlagValid(AttributionFlag_isHeightOfChild)) {
				isHeightOfChild = style->isHeightOfChild;
			}
			if (IsFlagValid(AttributionFlag_align)) {
				alignMode = style->alignMode;
				align = style->align;
			}
			if (IsFlagValid(AttributionFlag_border)) {
				border = style->border;
			}
			if (IsFlagValid(AttributionFlag_background)) {
				background = style->background;
			}
			if (IsFlagValid(AttributionFlag_margin)) {
				margin = style->margin;
			}
			if (IsFlagValid(AttributionFlag_padding)) {
				padding = style->padding;
			}
			if (IsFlagValid(AttributionFlag_sizeCaculator)) {
				sizeCaculator = style->sizeCaculator;
			}
			if (IsFlagValid(AttributionFlag_transform)) {
				transform = style->transform;
			}
			if (IsFlagValid(AttributionFlag_displayMode)) {
				displayMode = style->displayMode;
			}
			if (IsFlagValid(AttributionFlag_textColor)) {
				textColor = style->textColor;
			}
			if (IsFlagValid(AttributionFlag_fontSize)) {
				fontSize = style->fontSize;
			}
			if (IsFlagValid(AttributionFlag_fontFamily)) {
				fontFamily = style->fontFamily;
			}
			if (IsFlagValid(AttributionFlag_borderRadius)) {
				borderRadius = style->borderRadius;
			}
			if (IsFlagValid(AttributionFlag_lineHeight)) {
				lineHeight = style->lineHeight;
			}
			if (IsFlagValid(AttributionFlag_opacity)) {
				opacity = style->opacity;
			}
			if (IsFlagValid(AttributionFlag_duration)) {
				duration = style->duration;
			}
		}

		void AddToAnimation(Animation* animation, UIStyle *style) {
			if (style->IsFlagValid(AttributionFlag_cursor)) {
				SetFlag(AttributionFlag_cursor);
				cursor = style->cursor;
			}

			if (style->IsFlagValid(AttributionFlag_isWidthOfChild)) {
				SetFlag(AttributionFlag_isWidthOfChild);

				if (isWidthOfChild != style->isWidthOfChild) {
					isWidthOfChild = style->isWidthOfChild;
					
				}

			}
			if (style->IsFlagValid(AttributionFlag_isHeightOfChild)) {
				SetFlag(AttributionFlag_isHeightOfChild);

				if (isHeightOfChild != style->isHeightOfChild) {
					isHeightOfChild = style->isHeightOfChild;
					
				}

			}
			if (style->IsFlagValid(AttributionFlag_align)) {
				alignMode = style->alignMode;
				if (IsFlagValid(AttributionFlag_align)) {
					animation->Add<float>(align.Width, align.Width, style->align.Width);
					animation->Add<float>(align.Height, align.Height, style->align.Height);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_align);
					align = style->align;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_border)) {
				if (IsFlagValid(AttributionFlag_border)) {
					border.hasBottom = style->border.hasBottom;
					border.hasLeft = style->border.hasLeft;
					border.hasRight = style->border.hasRight;
					border.hasTop = style->border.hasTop;
					animation->Add<D2D1_COLOR_F>(border.borderColor, border.borderColor, style->border.borderColor);
					animation->Add<float>(border.borderSize, border.borderSize, style->border.borderSize);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_border);
					border = style->border;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_background)) {
				if (IsFlagValid(AttributionFlag_background)) {
					background.backgroundImage = style->background.backgroundImage;
					animation->Add<D2D1_COLOR_F>(background.backgroundColor, background.backgroundColor, style->background.backgroundColor);
				}
				else {
					SetFlag(AttributionFlag_background);
					background = style->background;
				}
			}
			if (style->IsFlagValid(AttributionFlag_margin)) {
				if (IsFlagValid(AttributionFlag_margin)) {
					animation->Add<AnimUI::RECTF>(margin, margin, style->margin);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_margin);
					margin = style->margin;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_padding)) {
				if (IsFlagValid(AttributionFlag_padding)) {
					animation->Add<AnimUI::RECTF>(padding, padding, style->padding);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_padding);
					padding = style->padding;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_sizeCaculator)) {
				if (IsFlagValid(AttributionFlag_sizeCaculator)) {
					animation->Add<float>(sizeCaculator.m_percentPH, sizeCaculator.m_percentPH, style->sizeCaculator.m_percentPH);
					animation->Add<float>(sizeCaculator.m_percentPW, sizeCaculator.m_percentPW, style->sizeCaculator.m_percentPW);
					animation->Add<float>(sizeCaculator.m_solidH, sizeCaculator.m_solidH, style->sizeCaculator.m_solidH);
					animation->Add<float>(sizeCaculator.m_solidW, sizeCaculator.m_solidW, style->sizeCaculator.m_solidW);
					animation->Add<float>(sizeCaculator.m_percentW, sizeCaculator.m_percentW, style->sizeCaculator.m_percentW);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_sizeCaculator);
					sizeCaculator = style->sizeCaculator;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_transform)) {
				if (IsFlagValid(AttributionFlag_transform)) {
					animation->Add<D2D1_SIZE_F>(transform.scale, transform.scale, style->transform.scale);
					animation->Add<D2D1_SIZE_F>(transform.transform, transform.transform, style->transform.transform);
					animation->Add<D2D1_POINT_2F>(transform.rotateCenter, transform.rotateCenter, style->transform.rotateCenter);
					animation->Add<float>(transform.rotateAngle, transform.rotateAngle, style->transform.rotateAngle);
				}
				else {
					SetFlag(AttributionFlag_transform);
					transform = style->transform;
				}
			}
			if (style->IsFlagValid(AttributionFlag_displayMode)) {
				SetFlag(AttributionFlag_displayMode);
				displayMode = style->displayMode;
				
			}
			if (style->IsFlagValid(AttributionFlag_textColor)) {
				if (IsFlagValid(AttributionFlag_textColor)) {
					animation->Add<D2D1_COLOR_F>(textColor, textColor, style->textColor);
				}
				else {
					SetFlag(AttributionFlag_textColor);
					textColor = style->textColor;
				}
			}
			if (style->IsFlagValid(AttributionFlag_fontSize)) {
				if (IsFlagValid(AttributionFlag_fontSize)) {
					animation->Add<float>(fontSize, fontSize, style->fontSize);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_fontSize);
					fontSize = style->fontSize;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_fontFamily)) {
				SetFlag(AttributionFlag_fontFamily);
				fontFamily = style->fontFamily;
			}
			if (style->IsFlagValid(AttributionFlag_borderRadius)) {
				if (IsFlagValid(AttributionFlag_borderRadius)) {
					animation->Add<float>(borderRadius, borderRadius, style->borderRadius);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_borderRadius);
					borderRadius = style->borderRadius;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_lineHeight)) {
				if (IsFlagValid(AttributionFlag_lineHeight)) {
					animation->Add<float>(lineHeight, lineHeight, style->lineHeight);
					animation->RequireMeasure();
				}
				else {
					SetFlag(AttributionFlag_lineHeight);
					lineHeight = style->lineHeight;
					
				}
			}
			if (style->IsFlagValid(AttributionFlag_opacity)) {
				if (IsFlagValid(AttributionFlag_opacity)) {
					animation->Add<float>(opacity, opacity, style->opacity);
				}
				else {
					SetFlag(AttributionFlag_opacity);
					opacity = style->opacity;
				}
			}

			attributionFlags = style->attributionFlags;



		}

	public:
		inline void SetTransform(UITransform&transform_) {
			SetFlag(AttributionFlag_transform);
			transform = transform_;
		}
		inline void SetLineHeight(float lineHeight_) {
			SetFlag(AttributionFlag_lineHeight);
			lineHeight = lineHeight_;
		}
		inline void SetFontSize(float fontSize_) {
			SetFlag(AttributionFlag_fontSize);
			fontSize = fontSize_;
		}
		inline void SetTextColor(D2D1_COLOR_F&textColor_) {
			SetFlag(AttributionFlag_textColor);
			textColor = textColor_;
		}
		inline void SetFontFamily(std::wstring fontFamily_) {
			SetFlag(AttributionFlag_fontFamily);
			fontFamily = fontFamily_;
		}
		inline void SetBorderRadius(float borderRadius_) {
			SetFlag(AttributionFlag_borderRadius);
			borderRadius = borderRadius_;
		}

		inline void SetWidthOfChild(bool is) {
			SetFlag(AttributionFlag_isWidthOfChild);
			isWidthOfChild = is;
		}
		inline void SetHeightOfChild(bool is) {
			SetFlag(AttributionFlag_isHeightOfChild);
			isHeightOfChild = is;
		}
		inline void SetOpacity(float opacity_) {
			SetFlag(AttributionFlag_opacity);
			opacity = opacity_;
		}
		inline void SetBorder(UIBorder&bd) {
			SetFlag(AttributionFlag_border);
			border = bd;
		}
		inline void SetSizeCaculator(SizeCaculator&caculator) {
			SetFlag(AttributionFlag_sizeCaculator);
			sizeCaculator = caculator;
		}
		inline void SetMargin(RECTF&rect) {
			margin = rect;
			SetFlag(AttributionFlag_margin);
		}
		inline void SetPadding(RECTF&rect) {
			padding = rect;
			SetFlag(AttributionFlag_padding);
		}
		inline void SetBackgroundColor(D2D1_COLOR_F& color) {
			background.backgroundColor = color;
			SetFlag(AttributionFlag_background);
		}
		inline void SetDuration(float duration_) {
			duration = duration_;
			SetFlag(AttributionFlag_duration);
		}
		inline void SetBackgroundImage(std::wstring& imageName) {
			background.backgroundImage = imageName;
			SetFlag(AttributionFlag_background);
		}
		inline void SetCursor(HCURSOR cursor_) {
			cursor = cursor_;
			SetFlag(AttributionFlag_cursor);
		}
		//used for relativeLayout
		inline bool IsAlignToLeft() {
			return (bool)(alignMode&2);
		}
		inline bool IsAlignToTop() {
			return alignMode & 1;
		}
		inline float GetLeftOrRight() {
			return align.Width;
		}
		inline float GetTopOrBottom() {
			return align.Height;
		}
		inline void SetLeft(float left) {
			SetFlag(AttributionFlag_align);
			if (!(alignMode & 2)) {
				alignMode ^= 2;
			}
			align.Width = left;
		}
		inline void SetRight(float right) {
			SetFlag(AttributionFlag_align);
			if (alignMode & 2) {
				alignMode ^= 2;
			}
			align.Width = right;
		}
		inline void SetTop(float top) {
			SetFlag(AttributionFlag_align);
			if (!(alignMode & 1)) {
				alignMode ^= 1;
			}
			align.Height= top;
		}
		inline void SetBottom(float bottom) {
			SetFlag(AttributionFlag_align);
			if (alignMode & 1) {
				alignMode ^= 1;
			}
			align.Height = bottom;
		}

		inline void SetFlag(int i) {
			int pos = 1 << i;
			if (!(attributionFlags&pos)) {
				attributionFlags ^= pos;
			}
		}
		inline bool IsFlagValid(int i) {
			int pos = 1 << i;
			return (bool)(attributionFlags&pos);
		}
	};

	class UIStateStyle :public AutoDelete {
	public:
		UIStateStyle() {
			ZeroMemory(styles, sizeof(UIStyle*) * 4);
		}
		~UIStateStyle() {
			for (int i = 0; i != 4; ++i) {
				SafeDelete(styles[i]);
			}
		}
		void SetState(UIState state, UIStyle*style) {
			styles[state] = style;
		}
		UIStyle* styles[4];
	};
}