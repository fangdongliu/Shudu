#include<d2d1.h>
#include "UIElement.h"
#include"uiwindow.h"
#include"AnimationObject.h"
using namespace D2D1;
using namespace AnimUI;
using namespace std;
IDWriteFactory* UIElement::dwriteFactory = nullptr;

UIElement::UIElement()
{
	needMeasure			= true;
	isFocusable			= false;
	isHidden			= false;
	animationId			= 0;
	stateStyle			= nullptr;
	parentElement		= nullptr;
	keyEventListener	= nullptr;
	charEventListener	= nullptr;
	mouseEventListener	= nullptr;
	painter				= nullptr;
	state				= UIState::UIStateNormal;
}


UIElement::~UIElement()
{
	SafeDelete(painter);
	SafeDelete(stateStyle);
	SafeDelete(keyEventListener);
	SafeDelete(charEventListener);
	SafeDelete(mouseEventListener);
}

void UIElement::ChangeCurrentStyle(UIWindow*destWindow, UIState state_,float duration_) {
	
	if (state_ == state)
		return;
	else if (state_ == UIStateNormal&&destWindow->GetFocusElement() == this)
		return;

	if (stateStyle) {

		auto style = stateStyle->styles[state_];

		if (style) {

			state = state_;

			float duration=0.0f;

			if (duration_ == -1.0f) {
				if (style->IsFlagValid(AttributionFlag_duration))
					duration = style->duration;
			}
			else 
				duration = duration_;

			if (duration>0.0f) {

				auto i = destWindow->RequestAnimation(this, &animationId);

				i->SetDuration(duration);

				if (style->IsFlagValid(AttributionFlag_cursor)) {
					currentStyle.SetFlag(AttributionFlag_cursor);
					currentStyle.cursor = style->cursor;
				}

				if (style->IsFlagValid(AttributionFlag_isWidthOfChild)) {
					currentStyle.SetFlag(AttributionFlag_isWidthOfChild);

					if (currentStyle.isWidthOfChild != style->isWidthOfChild) {
						currentStyle.isWidthOfChild = style->isWidthOfChild;
						NeedUpdate();
					}
				
				}
				if (style->IsFlagValid(AttributionFlag_isHeightOfChild)) {
					currentStyle.SetFlag(AttributionFlag_isHeightOfChild);

					if (currentStyle.isHeightOfChild != style->isHeightOfChild) {
						currentStyle.isHeightOfChild = style->isHeightOfChild;
						NeedUpdate();
					}
			
				}
				if (style->IsFlagValid(AttributionFlag_align)) {
					currentStyle.alignMode = style->alignMode;
					if (currentStyle.IsFlagValid(AttributionFlag_align)) {
						i->Add<float>(currentStyle.align.Width, currentStyle.align.Width, style->align.Width);
						i->Add<float>(currentStyle.align.Height, currentStyle.align.Height, style->align.Height);
						i->RequireMeasure();
					}
					else {
						currentStyle.SetFlag(AttributionFlag_align);
						currentStyle.align = style->align;
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_border)) {
					if (currentStyle.IsFlagValid(AttributionFlag_border)) {
						currentStyle.border.hasBottom = style->border.hasBottom;
						currentStyle.border.hasLeft = style->border.hasLeft;
						currentStyle.border.hasRight = style->border.hasRight;
						currentStyle.border.hasTop = style->border.hasTop;
						i->Add<D2D1_COLOR_F>(currentStyle.border.borderColor, currentStyle.border.borderColor, style->border.borderColor);
						i->Add<float>(currentStyle.border.borderSize, currentStyle.border.borderSize, style->border.borderSize);
						i->RequireMeasure();
					}
					else {
						currentStyle.SetFlag(AttributionFlag_border);
						currentStyle.border = style->border;
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_background)) {
					if (currentStyle.IsFlagValid(AttributionFlag_background)) {
						currentStyle.background.backgroundImage = style->background.backgroundImage;
						i->Add<D2D1_COLOR_F>(currentStyle.background.backgroundColor, currentStyle.background.backgroundColor, style->background.backgroundColor);
					}
					else {
						currentStyle.SetFlag(AttributionFlag_background);
						currentStyle.background = style->background;
					}
				}
				if (style->IsFlagValid(AttributionFlag_margin)) {
					if (currentStyle.IsFlagValid(AttributionFlag_margin)) {
						i->Add<AnimUI::RECTF>(currentStyle.margin, currentStyle.margin, style->margin);
						i->RequireMeasure();
					}
					else {
						currentStyle.SetFlag(AttributionFlag_margin);
						currentStyle.margin = style->margin;
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_padding)) {
					if (currentStyle.IsFlagValid(AttributionFlag_padding)) {
						i->Add<AnimUI::RECTF>(currentStyle.padding, currentStyle.padding, style->padding);
						i->RequireMeasure();
					}
					else {
						currentStyle.SetFlag(AttributionFlag_padding);
						currentStyle.padding = style->padding;
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_sizeCaculator)) {
					if (currentStyle.IsFlagValid(AttributionFlag_sizeCaculator)) {
						i->Add<float>(currentStyle.sizeCaculator.m_percentPH, currentStyle.sizeCaculator.m_percentPH, style->sizeCaculator.m_percentPH);
						i->Add<float>(currentStyle.sizeCaculator.m_percentPW, currentStyle.sizeCaculator.m_percentPW, style->sizeCaculator.m_percentPW);
						i->Add<float>(currentStyle.sizeCaculator.m_solidH, currentStyle.sizeCaculator.m_solidH, style->sizeCaculator.m_solidH);
						i->Add<float>(currentStyle.sizeCaculator.m_solidW, currentStyle.sizeCaculator.m_solidW, style->sizeCaculator.m_solidW);
						i->Add<float>(currentStyle.sizeCaculator.m_percentW, currentStyle.sizeCaculator.m_percentW, style->sizeCaculator.m_percentW);
						i->RequireMeasure();
					}
					else {
						currentStyle.SetFlag(AttributionFlag_sizeCaculator);
						currentStyle.sizeCaculator = style->sizeCaculator; 
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_transform)) {
					if (currentStyle.IsFlagValid(AttributionFlag_transform)) {
						i->Add<D2D1_SIZE_F>(currentStyle.transform.scale, currentStyle.transform.scale, style->transform.scale);
						i->Add<D2D1_SIZE_F>(currentStyle.transform.transform, currentStyle.transform.transform, style->transform.transform);
						i->Add<D2D1_POINT_2F>(currentStyle.transform.rotateCenter, currentStyle.transform.rotateCenter, style->transform.rotateCenter);
						i->Add<float>(currentStyle.transform.rotateAngle, currentStyle.transform.rotateAngle, style->transform.rotateAngle);
					}
					else {
						currentStyle.SetFlag(AttributionFlag_transform);
						currentStyle.transform = style->transform; 
					}
				}
				if (style->IsFlagValid(AttributionFlag_displayMode)) {
					currentStyle.SetFlag(AttributionFlag_displayMode);
					currentStyle.displayMode = style->displayMode; 
					NeedUpdate();
				}
				if (style->IsFlagValid(AttributionFlag_textColor)) {
					if (currentStyle.IsFlagValid(AttributionFlag_textColor)) {
						i->Add<D2D1_COLOR_F>(currentStyle.textColor, currentStyle.textColor, style->textColor);
					}
					else { 
						currentStyle.SetFlag(AttributionFlag_textColor);
						currentStyle.textColor = style->textColor;
					}
				}
				if (style->IsFlagValid(AttributionFlag_fontSize)) {
					if (currentStyle.IsFlagValid(AttributionFlag_fontSize)) {
						i->Add<float>(currentStyle.fontSize, currentStyle.fontSize, style->fontSize);
						i->RequireMeasure();
					}
					else {
						currentStyle.SetFlag(AttributionFlag_fontSize);
						currentStyle.fontSize = style->fontSize;
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_fontFamily)) {
					currentStyle.SetFlag(AttributionFlag_fontFamily);
					currentStyle.fontFamily = style->fontFamily;
				}
				if (style->IsFlagValid(AttributionFlag_borderRadius)) {
					if (currentStyle.IsFlagValid(AttributionFlag_borderRadius)) {
						i->Add<float>(currentStyle.borderRadius, currentStyle.borderRadius, style->borderRadius);
						i->RequireMeasure();
					}
					else { 
						currentStyle.SetFlag(AttributionFlag_borderRadius);
						currentStyle.borderRadius = style->borderRadius; 
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_lineHeight)) {
					if (currentStyle.IsFlagValid(AttributionFlag_lineHeight)) {
						i->Add<float>(currentStyle.lineHeight, currentStyle.lineHeight, style->lineHeight);
						i->RequireMeasure();
					}
					else { 
						currentStyle.SetFlag(AttributionFlag_lineHeight);
						currentStyle.lineHeight = style->lineHeight;
						NeedUpdate();
					}
				}
				if (style->IsFlagValid(AttributionFlag_opacity)) {
					if (currentStyle.IsFlagValid(AttributionFlag_opacity)) {
						i->Add<float>(currentStyle.opacity, currentStyle.opacity, style->opacity);
					}
					else {
						currentStyle.SetFlag(AttributionFlag_opacity);
						currentStyle.opacity = style->opacity; 
					}
				}

				currentStyle.attributionFlags = style->attributionFlags;

			}
			else {
				if ((style->IsFlagValid(AttributionFlag_isWidthOfChild) && style->isWidthOfChild != currentStyle.isWidthOfChild) ||
					(style->IsFlagValid(AttributionFlag_isHeightOfChild) && style->isHeightOfChild != currentStyle.isHeightOfChild)||
					style->IsFlagValid(AttributionFlag_lineHeight)||
					style->IsFlagValid(AttributionFlag_borderRadius)||
					style->IsFlagValid(AttributionFlag_fontSize)||
					style->IsFlagValid(AttributionFlag_sizeCaculator)||
					style->IsFlagValid(AttributionFlag_padding)||
					style->IsFlagValid(AttributionFlag_margin)||
					style->IsFlagValid(AttributionFlag_border)||
					style->IsFlagValid(AttributionFlag_align)||
					style->IsFlagValid(AttributionFlag_displayMode)
					)
					NeedUpdate();
				
				destWindow->DeleteAnimation(animationId);
				currentStyle.SafeCopy(style);
				
			}
		}
	}

}
void UIElement::NeedUpdate() {

	if(!needMeasure)
		needMeasure = true; 

	if (parentElement)
		parentElement->ChildNeedUpdate();

}

void UIElement::OnPaint(UIWindow *destWindow) {

	float opacity = currentStyle.opacity;
	bool transformValid = currentStyle.IsFlagValid(AttributionFlag_transform);

	D2D1_MATRIX_3X2_F lastTransform;

	if (transformValid)
		destWindow->currentRenderTarget->GetTransform(&lastTransform);

	bool useLayer = opacity != 1.0f || transformValid;

	if (useLayer) {

		if (opacity == 0.0f)
			return;

		destWindow->currentRenderTarget->SetTransform(lastTransform*
			Matrix3x2F::Scale(currentStyle.transform.scale, Point2F(rect.X + rect.Width / 2.0f, rect.Y + rect.Height / 2.0f))*
			Matrix3x2F::Rotation(currentStyle.transform.rotateAngle, Point2F(rect.X + rect.Width*currentStyle.transform.rotateCenter.x, rect.Y + rect.Height*currentStyle.transform.rotateCenter.y))*
			Matrix3x2F::Translation(currentStyle.transform.transform)
		);
		auto param = D2D1::LayerParameters(D2D1::RectF(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height),0, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, Matrix3x2F::Identity(), opacity);
		destWindow->PushLayer(param);

	}
	PaintBackground(destWindow);

	if (painter)
		painter->Paint(this,destWindow);

	PaintChilds(destWindow);

	PaintScroller(destWindow);

	PaintBorder(destWindow);

	if (useLayer)
		destWindow->PopLayer();

	if (transformValid)
		destWindow->currentRenderTarget->SetTransform(lastTransform);
}

void UIElement::Detach() {
	if (parentElement) {
		((UIContainer*)parentElement)->DetachChild(this);
		parentElement = nullptr;
	}
}

void UIElement::CaculateSize(float parentWidth,float parentHeight){

	parentWidth		-= currentStyle.margin.Left + currentStyle.margin.Right;
	parentHeight	-= currentStyle.margin.Top + currentStyle.margin.Bottom;
	rect.Width		= currentStyle.sizeCaculator.CalcWidth(parentWidth);
	rect.Height		= currentStyle.sizeCaculator.CalcHeight(parentHeight, rect.Width);

}
void UIElement::OnMouse(MouseEvent &mouseEvent) {

	if (mouseEventListener) {
		mouseEvent.SetDestElement(this);
		mouseEventListener->OnMouse(mouseEvent);
	}

	switch (mouseEvent.GetEventType()) {
		case MouseEventRButtonDown:
		case MouseEventLButtonDown:
		case MouseEventMButtonDown:
			if (!mouseEvent.GetEventHandledId()) {
				mouseEvent.EventHandled();
				
				if(isFocusable)
					mouseEvent.GetDestWindow()->SetFocusElement(this);
				
				mouseEvent.GetDestWindow()->SetCaptureElement(this);
			}
			break;

		case MouseEventRButtonUp:
		case MouseEventMButtonUp:
		case MouseEventLButtonUp:
			if (!mouseEvent.GetEventHandledId()) {
				mouseEvent.EventHandled();
				mouseEvent.GetDestWindow()->SetCaptureElement(nullptr);
			}
			break;

		case MouseEventScroll:
			OnScroll(mouseEvent);
			break;

		case MouseEventMouseMove:
			if (!mouseEvent.GetEventHandledId()) {
				
				SetCursor(currentStyle.cursor);

				mouseEvent.GetDestWindow()->SetHoverElement(this);
				
				mouseEvent.EventHandled();
			}
			else 
				ChangeCurrentStyle(mouseEvent.GetDestWindow(), UIState::UIStateHover);
	
			break;
	}

	

}
void UIElement::OnMouseOut(UIWindow* destWindow) {
	
	ChangeCurrentStyle(destWindow, UIState::UIStateNormal);

	if (parentElement)
		parentElement->OnMouseOut(destWindow);

	if (mouseEventListener)
		mouseEventListener->OnMouseOut(this);

}
void UIElement::OnKey(KeyEvent &keyEvent) {

	if (keyEventListener) 
		keyEventListener->OnKey(keyEvent);

}
void UIElement::OnChar(CharEvent &charEvent) {

	if (charEventListener)
		charEventListener->OnChar(charEvent);

}
void UIElement::PaintBackground(UIWindow*window) {

	if (currentStyle.IsFlagValid(AttributionFlag_background)) {
		
		ID2D1Bitmap* image = nullptr;
		ID2D1Brush* currentBrush = window->solidColorBrush;

		if (currentStyle.background.backgroundImage.length() > 0)
			image = window->RequestImage(currentStyle.background.backgroundImage);

		auto rectangle = D2D1::RectF(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
		
		if (image) {
			window->bitmapBrush->SetBitmap(image);
			auto size = image->GetSize();
			size.height = rect.Height / size.height;
			size.width = rect.Width / size.width;
			window->bitmapBrush->SetTransform(D2D1::Matrix3x2F::Scale(size)*D2D1::Matrix3x2F::Translation(D2D1::SizeF(rect.X, rect.Y)));
			currentBrush = window->bitmapBrush;
		}
		else 
			window->solidColorBrush->SetColor(currentStyle.background.backgroundColor);

		if (currentStyle.IsFlagValid(AttributionFlag_borderRadius))
			window->currentRenderTarget->FillRoundedRectangle(
				D2D1::RoundedRect(rectangle, currentStyle.borderRadius, currentStyle.borderRadius), currentBrush);
		else
			window->currentRenderTarget->FillRectangle(rectangle, currentBrush);

	}

}
void UIElement::PaintBorder(UIWindow*window) {

	if (currentStyle.IsFlagValid(AttributionFlag_border)) {
		
		auto rectangle = D2D1::RectF(rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
		auto renderTarget = window->currentRenderTarget;
		auto brush = window->solidColorBrush;
		brush->SetColor(currentStyle.border.borderColor);

		if (currentStyle.IsFlagValid(AttributionFlag_borderRadius)) {
			renderTarget->DrawRoundedRectangle(D2D1::RoundedRect(rectangle,
				currentStyle.borderRadius, currentStyle.borderRadius), brush, currentStyle.border.borderSize);
		}
		else {
			if (currentStyle.border.hasLeft) {
				renderTarget->DrawLine(D2D1::Point2F(rectangle.left, rectangle.top), D2D1::Point2F(rectangle.left, rectangle.bottom), brush, currentStyle.border.borderSize);
			}
			if (currentStyle.border.hasRight) {
				renderTarget->DrawLine(D2D1::Point2F(rectangle.right, rectangle.top), D2D1::Point2F(rectangle.right, rectangle.bottom), brush, currentStyle.border.borderSize);
			}
			if (currentStyle.border.hasBottom) {
				renderTarget->DrawLine(D2D1::Point2F(rectangle.left, rectangle.bottom), D2D1::Point2F(rectangle.right, rectangle.bottom), brush, currentStyle.border.borderSize);
			}
			if (currentStyle.border.hasTop) {
				renderTarget->DrawLine(D2D1::Point2F(rectangle.left, rectangle.top), D2D1::Point2F(rectangle.right, rectangle.top), brush, currentStyle.border.borderSize);
			}
		}

	}

}

UIElement*	UIElement::GetElementById(wstring& id_) {
	if(id==id_)
	return this;
}

shared_ptr<vector<UIElement*>>
UIElement::GetElementsByType(RuntimeId runtimeId) {
	shared_ptr<vector<UIElement*>>elements(new vector<UIElement*>);

	if (RuntimeId() == runtimeId) 
		elements->push_back(this);

	return elements;
}