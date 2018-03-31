#include "UIContainer.h"
#include"uiwindow.h"
#include"AnimationObject.h"
#include<d2d1effects.h>
#include"UIText.h"
#include<queue>
using namespace AnimUI;
using namespace D2D1;
using namespace std;

D2D1_COLOR_F UIContainer::scrollColor = D2D1::ColorF(0x888888);
float UIContainer::scrollWidth = 10;
float UIContainer::scrollBorderRadius = 2;
UIContainer::UIContainer()
{
	scrollX = 0.0f;
	scrollY = 0.0f;
}


UIContainer::~UIContainer()
{

	for (auto i : childs) 
		SafeDelete(i);

}

void UIContainer::ChildNeedUpdate() {
	
	if (!needMeasure) {
		if (currentStyle.isWidthOfChild || currentStyle.isHeightOfChild) {
			needMeasure = true;

			if (parentElement) 
				parentElement->ChildNeedUpdate();

		}
	}

}

UIElement*	UIContainer::GetElementById(wstring& id) {

	return idMap[id];
	
}

shared_ptr<vector<UIElement*>>
UIContainer::GetElementsByType(RuntimeId runtimeId) {
	shared_ptr<vector<UIElement*>>elements(new vector<UIElement*>);
	queue<UIElement*>q;
	q.push(this);
	if (RuntimeId() == runtimeId) {
		elements->push_back(this);
	}
	while (!q.empty()) {
		auto e = q.front();
		q.pop();
		if (e->GetRuntimeId() <= RuntimeId::RuntimeIdUIContainer) {
			for (auto i : ((UIContainer*)e)->childs) {
				if (i->GetRuntimeId() == runtimeId) {
					elements->push_back(i);
				}
				if (i->GetRuntimeId() <= RuntimeId::RuntimeIdUIContainer) {
					q.push(i);
				}
			}
		}
	}
	return elements;
}

bool UIContainer::DispatchEventToChild(MouseEvent &mouseEvent) {

	auto point = mouseEvent.GetPoint();

	if (mouseEvent.GetEventType() == MouseEventType::MouseEventLButtonDown) {
		int a=0;
		a++;
	}

	for (auto i = childs.rbegin(); i != childs.rend(); i++) {

		if ((*i)->isHidden)
			continue;

		if ((*i)->currentStyle.IsFlagValid(AttributionFlag_borderRadius)) {

			if ((*i)->rect.ContainWidthRadius(point, (*i)->currentStyle.borderRadius)) {

				(*i)->OnMouse(mouseEvent);

				if (mouseEvent.GetEventHandledId())
					return true;
			
			}

		}
		else if ((*i)->rect.Contains(point)) {

			(*i)->OnMouse(mouseEvent);

			if (mouseEvent.GetEventHandledId())
				return true;
	
		}

	}

	return false;
}
void UIContainer::OnMouse(MouseEvent& mouseEvent) {

	if (DispatchEventToChild(mouseEvent)) 
		if (mouseEvent.GetEventType() == MouseEventType::MouseEventScroll)return;

	UIElement::OnMouse(mouseEvent);

}
void UIContainer::OnScroll(MouseEvent &mouseEvent) {

	if (childcontentRect.Height <= rect.Height)
		return;

	float curr = scrollY + (short)HIWORD(mouseEvent.GetWParam());
	
	if (curr > 0.0f)
		curr = 0.0f;
	
	if (curr < rect.Height - childcontentRect.Height)
		curr = rect.Height - childcontentRect.Height;
	
	if (curr != scrollY) {
		scrollY=curr;
		mouseEvent.EventHandled();
	}

}
void UIContainer::AddChild(UIElement*child) {

	childs.push_back(child);
	if (child->id.length() > 0) {
		idMap[child->id] = child;
	}

	child->SetParent(this);

}
void UIContainer::OnMeasure(UIWindow*destWindow) {

	needMeasure = false;

	float Width = rect.Width - currentStyle.padding.Left - currentStyle.padding.Right;
	float Height = rect.Height - currentStyle.padding.Top - currentStyle.padding.Bottom;

	for (auto i : childs) {

		if (i->isHidden)
			continue;

		float lastWidth = i->rect.Width;
		float lastHeight = i->rect.Height;

		i->CaculateSize(Width,Height);

		if (i->currentStyle.isHeightOfChild || i->currentStyle.isWidthOfChild)
			i->OnMeasure(destWindow);
		else if (i->rect.Width == lastWidth&&i->rect.Height == lastHeight)
			continue;
		else if (i->GetRuntimeId() <= RuntimeId::RuntimeIdUIContainer)
			i->needMeasure = true;

	}

	OnLayout();

	if (currentStyle.isWidthOfChild) 
		rect.Width = childcontentRect.Width;
	
	if (currentStyle.isHeightOfChild) 
		rect.Height = childcontentRect.Height;

}
void UIContainer::PaintChilds(UIWindow*destWindow) {

	auto validRect = destWindow->rect;

	validRect.Intersect(rect);
	auto wrapperWidth = rect.Width - currentStyle.margin.Left - currentStyle.margin.Right;
	auto wrapperHeight = rect.Height - currentStyle.margin.Bottom - currentStyle.margin.Top;
	for (auto i : childs) {

		if (i->isHidden)
			continue;

		if (i->needMeasure) {
			i->CaculateSize(wrapperWidth, wrapperHeight);
			i->OnMeasure(destWindow);
		}

		i->rect.X = i->relativePos.X + scrollX + rect.X;
		i->rect.Y = i->relativePos.Y + scrollY + rect.Y;

		if (i->rect.IntersectsWith(validRect)) 
			i->OnPaint(destWindow);
	
	}

}

void UIContainer::PaintScroller(UIWindow*destWindow) {

	auto brush = destWindow->solidColorBrush;

	brush->SetColor(scrollColor);

	if (childcontentRect.Height > rect.Height) {
		if (scrollY + childcontentRect.Height - rect.Height < 0)
			scrollY = rect.Height- childcontentRect.Height ;

		scrollYRect.Height= rect.Height*rect.Height / childcontentRect.Height;
		scrollYRect.X = rect.X + rect.Width - scrollWidth;
		scrollYRect.Y = scrollYRect.Height*(-scrollY) / rect.Height + rect.Y;
		scrollYRect.Width = scrollWidth;
		destWindow->currentRenderTarget->FillRoundedRectangle(D2D1::RoundedRect(
			D2D1::RectF(scrollYRect.X, scrollYRect.Y, scrollYRect.GetRight(), scrollYRect.GetBottom())
			,scrollBorderRadius,scrollBorderRadius),brush);
	
	}

	if (childcontentRect.Width > rect.Width) {
		if (scrollX + childcontentRect.Width - rect.Width < 0)
			scrollX =  rect.Width- childcontentRect.Width;

		scrollXRect.Width = rect.Width*rect.Width / childcontentRect.Width;
		scrollXRect.Height = scrollWidth;
		scrollXRect.X = scrollXRect.Width*(-scrollX) / rect.Width;
		scrollXRect.Y = rect.Y + rect.Height - scrollWidth;
		destWindow->currentRenderTarget->FillRoundedRectangle(D2D1::RoundedRect(
			D2D1::RectF(scrollXRect.X, scrollXRect.Y, scrollXRect.GetRight(), scrollXRect.GetBottom())
			, scrollBorderRadius, scrollBorderRadius), brush);
	}

}
void UIContainer::DetachChild(UIElement*child) {
	for (int i = childs.size() - 1; i != -1; --i) {
		if (childs[i] == child) {
			childs.erase(childs.begin() + i);
			if (child->id.length() != 0) {
				idMap.erase(child->id);
			}
			break;
			NeedUpdate();
		}
	}
	
}

void UIContainer::DeleteChild(UIElement*child) {
	DetachChild(child);
	delete child;
}