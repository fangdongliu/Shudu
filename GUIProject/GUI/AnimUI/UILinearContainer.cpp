#include"LesStack.h"
#include "UILinearContainer.h"
#include"uiwindow.h"
#include<algorithm>
using namespace AnimUI;
using namespace D2D1;

UILinearContainer::UILinearContainer()
{
	linearFloat = LinearFloat::LinearFloatNone;
}

bool UILinearContainer::DispatchEventToChild(MouseEvent &mouseEvent) {

	if (childs.empty())return false;

	auto& point = mouseEvent.GetPoint();

	for (auto i = lastVisibleChild; i >= firstVisibleChild; i--) {

		auto child = childs[i];

		if (child->isHidden)
			continue;

		auto radius = child->currentStyle.borderRadius;

		if (radius != 0.0f) {

			if (child->rect.ContainWidthRadius(point, radius)) {
				
				child->OnMouse(mouseEvent);
			
				if (mouseEvent.GetEventHandledId())
					return true;
		
			}

		}
		else if (child->rect.Contains(point)) {
		
			child->OnMouse(mouseEvent);
			
			if (mouseEvent.GetEventHandledId())
				return true;
		
		}
	
	}

	return false;

}
void UILinearContainer::OnLayout() {

	if (childs.size() == 0)
		return;
	
	childcontentRect.Clear();
	lineHeights.clear();
	lineBreaks.clear();

//	if (id == L"3")
	//	printf("3\n");

	switch (linearFloat) {
		
		case LinearFloat::LinearFloatNone:
			LayoutFloatNone();
			break;

		case LinearFloat::LinearFloatLeft:
			LayoutFloatLeft();
			break;

		case LinearFloat::LinearFloatRight:
			LayoutFloatRight();
			break;
	}

	childcontentRect.Height = lineHeights.back();

	AnimUI::PointF paddingTransform(currentStyle.padding.Left, currentStyle.padding.Top);
	
	if (linearFloat == LinearFloat::LinearFloatRight&&currentStyle.isWidthOfChild) 
		paddingTransform.X -= childcontentRect.X;
	
	for (auto i : childs) 
		i->relativePos += paddingTransform;
	if(id==L"123"){
//		printf("%f\n", childcontentRect.Height);
	}
	if (id == L"123") {
	//	printf("%f\n", childcontentRect.Height);
	}
	
	childcontentRect.Height += currentStyle.padding.Bottom + currentStyle.padding.Top;
	if (id == L"123") {
		printf("%f\n", childcontentRect.Height);
	}
	childcontentRect.Width += currentStyle.padding.Right + currentStyle.padding.Left;

}
void UILinearContainer::PaintChilds(UIWindow*destWindow) {

	firstVisibleChild = 0;
	lastVisibleChild = childs.size() - 1;
	
	if (childs.size() == 0)
		return;
	
	int lowerY=0;
	
	if (childs.size() > 20) {

		if (scrollY != 0.0f) {
	
			lowerY = std::lower_bound(lineHeights.data(), lineHeights.data() + lineHeights.size(), - scrollY ) - lineHeights.data();
			
			if (lowerY == lineHeights.size()) 
				return;
			
		
			if (lowerY != 0) {
				lowerY--;
				firstVisibleChild = lineBreaks[lowerY];
			}

		}

	}

	auto validRect = destWindow->rect;
	float relativeBottom = -scrollY + rect.Height - currentStyle.padding.Top;
	int cur = firstVisibleChild;

	validRect.Intersect(rect);

	for (int line = lowerY; line != lineBreaks.size(); line++) {

		if (line>1&&lineHeights[line-2] >relativeBottom) {

			lastVisibleChild = cur-1;

			break;
		}

		for (; cur<= lineBreaks[line]; cur++) {
		
			auto child = childs[cur];
			
			if (child->isHidden)
				continue;

			if (child->needMeasure) {
				child->OnMeasure(destWindow);
			}
			
			child->rect.X = child->relativePos.X + scrollX + rect.X;
			child->rect.Y = child->relativePos.Y + scrollY + rect.Y;
			
			if (child->rect.IntersectsWith(validRect)) 
				child->OnPaint(destWindow);
			
		}

	}

}
void UILinearContainer::LayoutFloatNone() {
	
	lineHeights.resize(childs.size() + 1);
	lineHeights[0] = 0.0f;
	lineBreaks.resize(childs.size() + 1);

	for (int i = 0; i != childs.size(); i++) {

		auto child = childs[i];

		if (child->isHidden)
			continue;

		float heightNeed = child->currentStyle.margin.Top + child->currentStyle.margin.Bottom + child->rect.Height;
		float lineHeight = child->currentStyle.lineHeight;
		
		if (lineHeight > heightNeed) {
			lineHeights[i + 1] = lineHeights[i] + lineHeight;
			child->relativePos = PointF(child->currentStyle.margin.Left, lineHeights[i] + (lineHeight-heightNeed)/2.0f + currentStyle.margin.Top);
		}
		else {
			lineHeights[i + 1] = lineHeights[i] + heightNeed;
			child->relativePos= PointF(child->currentStyle.margin.Left, lineHeights[i] +  currentStyle.margin.Top);
		}
		
		lineBreaks[i] = i;
		childcontentRect.Union(RectF(child->relativePos, SizeF(child->rect.Width, child->rect.Height)));
	
	}

}

void UILinearContainer::AlignToCenter(float maxHeight, int start,int count) {
	
	for (int i = start; i >start - count; i--) {
		
		auto child = childs[i];

		if (child->isHidden)
			continue;

		auto& style = child->currentStyle;
		float heightNeed = child->rect.Height + style.margin.Top + style.margin.Bottom;
		
		child->relativePos.Y += (maxHeight - heightNeed) / 2.0f+style.margin.Top;
	}

}
void UILinearContainer::LayoutFloatLeft() {
	
	float contentMaxWidth = rect.Width - currentStyle.padding.Left - currentStyle.padding.Right;
	float currentInlineWidth = 0.0f;
	float maxHeightInline=0.0f;
	int objCount = 0;

	lineHeights.push_back(0.0f);
	lineBreaks.push_back(0);
	
	for (int i = 0; i != childs.size();++i) {
	
		auto child = childs[i];

		if (child->isHidden)
			continue;

		auto& style = child->currentStyle;
		float lineHeight = style.lineHeight;
		float widthNeed = child->rect.Width + style.margin.Left + style.margin.Right;
		float heightNeed = child->rect.Height + style.margin.Top + style.margin.Bottom;
		
		if (widthNeed < 0.0f)
			widthNeed = 0.0f;
		
		if (heightNeed < 0.0f)
			heightNeed = 0.0f;
		
		switch (style.displayMode) {
			
			case UIDisplayModeBlock:
				if (objCount != 0) {
					childcontentRect.Width = max(childcontentRect.Width, currentInlineWidth);
					AlignToCenter(maxHeightInline, i-1, objCount);
					lineHeights.push_back(lineHeights.back() + maxHeightInline);
					objCount = 0;
					lineBreaks.push_back(i-1);
				}

				currentInlineWidth = 0.0f;
				maxHeightInline = 0.0f;
			
				if (lineHeight > heightNeed) {
					child->relativePos = PointF(style.margin.Left, lineHeights.back() + (lineHeight - heightNeed ) / 2.0f + style.margin.Top);
					lineHeights.push_back(lineHeights.back() + lineHeight);
				}
				else {
					child->relativePos = PointF(style.margin.Left, lineHeights.back() + style.margin.Top);
					lineHeights.push_back(lineHeights.back() +heightNeed);
				}
				childcontentRect.HorizontalUnion(child->relativePos.X - style.margin.Left, widthNeed);
				lineBreaks.push_back(i);
				break;
		
			case UIDisplayModeInline:
				if (currentInlineWidth + widthNeed > contentMaxWidth) {
					childcontentRect.Width = max(childcontentRect.Width, currentInlineWidth);

					if (objCount != 0) {
						AlignToCenter(maxHeightInline, i-1, objCount);
						lineHeights.push_back(lineHeights.back() + maxHeightInline);
						objCount = 0;
						lineBreaks.push_back(i-1);
					}
					currentInlineWidth = 0.0f;
					maxHeightInline = 0.0f;
					
				}
				child->relativePos = PointF(currentInlineWidth + style.margin.Left, lineHeights.back());
				currentInlineWidth += widthNeed;
				objCount++;
				if (maxHeightInline < heightNeed) 
					maxHeightInline = heightNeed;
	
				if (maxHeightInline < lineHeight) 
					maxHeightInline = lineHeight;
				
				break;
		}

	}

	AlignToCenter(maxHeightInline, childs.size()-1, objCount);
	lineBreaks.push_back(childs.size() - 1);
	lineHeights.push_back(lineHeights.back() + maxHeightInline);
	childcontentRect.Width = max(childcontentRect.Width, currentInlineWidth);
}

void UILinearContainer::LayoutFloatRight() {

	float contentMaxWidth = rect.Width - currentStyle.padding.Left - currentStyle.padding.Right;
	float currentInlineWidth = 0.0f;
	float maxHeightInline = 0.0f;
	int objCount = 0;

	lineHeights.push_back(0.0f);
	lineBreaks.push_back(0);
	childcontentRect.Clear();
	childcontentRect.X = contentMaxWidth;

	for (int i = 0; i != childs.size(); ++i) {

		auto child = childs[i];

		if (child->isHidden)
			continue;

		auto& style = child->currentStyle;
		float lineHeight = style.lineHeight;
		float widthNeed = child->rect.Width + style.margin.Left + style.margin.Right;
		float heightNeed = child->rect.Height + style.margin.Top + style.margin.Bottom;

		if (widthNeed < 0.0f)
			widthNeed = 0.0f;

		if (heightNeed < 0.0f)
			heightNeed = 0.0f;

		switch (style.displayMode) {
	
			case UIDisplayModeBlock:
				if (objCount != 0) {
					AlignToCenter(maxHeightInline, i-1, objCount);
					lineHeights.push_back(lineHeights.back() + maxHeightInline);
					objCount = 0;
					lineBreaks.push_back(i - 1);
				}
			
				currentInlineWidth = 0.0f;
				maxHeightInline = 0.0f;
				
				if (lineHeight > heightNeed) {
					child->relativePos = PointF(contentMaxWidth-widthNeed+style.margin.Left, lineHeights.back() + (lineHeight - heightNeed) / 2.0f + style.margin.Top);
					lineHeights.push_back(lineHeights.back() + lineHeight);
				}
				else {
					child->relativePos = PointF(contentMaxWidth - widthNeed+style.margin.Left, lineHeights.back() + style.margin.Top);
					lineHeights.push_back(lineHeights.back() + heightNeed);
				}
		
				childcontentRect.Union(RectF(child->relativePos + PointF(-style.margin.Left, -style.margin.Top), SizeF(widthNeed, heightNeed)));
				lineBreaks.push_back(i);
				break;
			
			case UIDisplayModeInline:
				if (currentInlineWidth + widthNeed > contentMaxWidth) {
					if (objCount != 0) {
						AlignToCenter(maxHeightInline, i-1, objCount);
						lineHeights.push_back(lineHeights.back() + maxHeightInline);
						objCount = 0;
						lineBreaks.push_back(i - 1);
					}
					currentInlineWidth = 0.0f;
					maxHeightInline = 0.0f;
				}
			
				currentInlineWidth += widthNeed;
				child->relativePos = PointF(contentMaxWidth - currentInlineWidth + style.margin.Left, lineHeights.back());
				
				objCount++;

				if (maxHeightInline < heightNeed)
					maxHeightInline = heightNeed;

				if (maxHeightInline < lineHeight)
					maxHeightInline = lineHeight;
			
				break;
		}

	}

	AlignToCenter(maxHeightInline, childs.size() - 1, objCount);
	lineBreaks.push_back(childs.size() - 1);
	lineHeights.push_back(lineHeights.back() + maxHeightInline);

}

void UILinearContainer::ChildNeedUpdate() {

	if (!needMeasure) {
		needMeasure = true;
//		printf("need measure\n");
		if (parentElement)
			parentElement->ChildNeedUpdate();
	}

}