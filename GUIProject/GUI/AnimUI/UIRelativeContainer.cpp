#include "UIRelativeContainer.h"
#include"uiwindow.h"
using namespace AnimUI;


UIRelativeContainer::UIRelativeContainer()
{
}


UIRelativeContainer::~UIRelativeContainer()
{
}
void UIRelativeContainer::PaintChilds(UIWindow*destWindow) {
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
			auto &style = i->currentStyle;
			if (style.IsAlignToLeft()) {
				i->relativePos.X = style.GetLeftOrRight();
			}
			else {
				i->relativePos.X = wrapperWidth - i->rect.Width - style.GetLeftOrRight();
			}
			if (style.IsAlignToTop()) {
				i->relativePos.Y = style.GetTopOrBottom();
			}
			else {
				i->relativePos.Y = wrapperHeight - i->rect.Height - style.GetTopOrBottom();
			}
		}

		i->rect.X = i->relativePos.X + scrollX + rect.X;
		i->rect.Y = i->relativePos.Y + scrollY + rect.Y;

		if (i->rect.IntersectsWith(validRect))
			i->OnPaint(destWindow);

	}
}

void UIRelativeContainer::OnLayout() {
	float contentMaxWidth = rect.Width - currentStyle.padding.Left - currentStyle.padding.Right;
	float contentMaxHeight = rect.Height - currentStyle.padding.Top - currentStyle.padding.Bottom;
	for (auto i : childs) {

		if (i->isHidden)
			continue;

		auto &style = i->currentStyle;
		if (style.IsAlignToLeft()) {
			i->relativePos.X = style.GetLeftOrRight();
		}
		else {
			i->relativePos.X = contentMaxWidth - i->rect.Width - style.GetLeftOrRight();
		}
		if (style.IsAlignToTop()) {
			i->relativePos.Y = style.GetTopOrBottom();
		}
		else {
			i->relativePos.Y = contentMaxHeight - i->rect.Height - style.GetTopOrBottom();
		}
	}
	AnimUI::PointF paddingTransform(currentStyle.padding.Left, currentStyle.padding.Top);
	for (auto i : childs) {
		i->relativePos += paddingTransform;
	}
	childcontentRect.Height += currentStyle.padding.Bottom + currentStyle.padding.Top;
	childcontentRect.Width += currentStyle.padding.Right + currentStyle.padding.Left;
}