#include "UITable.h"
#include"uiwindow.h"
#include<algorithm>
using namespace AnimUI;

bool UITable::DispatchEventToChild(MouseEvent &mouseEvent) {
	if (childs.empty())return false;
	auto point = mouseEvent.GetPoint();
	int pos = 0;
	float relativeY = point.Y - currentStyle.padding.Top - scrollY-rect.Y;
	int lowerY = std::lower_bound(offsetHeights.data(), offsetHeights.data() + offsetHeights.size(), relativeY) - offsetHeights.data();
	
	if (lowerY == offsetHeights.size()) 
		return false;
	
	float relativeX = point.X - currentStyle.padding.Left - scrollX-rect.X;
	
	if (lowerY > 0)
		lowerY--;
	
	int lowerX = std::lower_bound(offsetWidths.data(), offsetWidths.data() + offsetWidths.size(), relativeX) - offsetWidths.data();

	if (lowerX == offsetWidths.size()) 
		return false;
	
	if (lowerX > 0)
		lowerX--;

	if (lowerY == 0) 
		if (lineStops[lowerY] <= lowerX) 
			return false;
		else 
			pos = lowerX;
	else if (lineStops[lowerY] - lineStops[lowerY - 1] <= lowerX) 
		return false;
	else 
		pos = lineStops[lowerY - 1] + lowerX;
	
	auto child = childs[pos];
	
	if (child->isHidden)
		return false;

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

	return false;
}

void UITable::PaintChilds(UIWindow*destWindow) {

	if (childs.empty())return;

	firstVisibleChild = 0;
	lastVisibleChild = childs.size() - 1;

	if (lineStops.size() > 15) {
		if (scrollY != 0.0f) {

			float relativeTop = -currentStyle.padding.Top - scrollY;
			int low = 0, high = lineStops.size()-2;

			while (low < high) {

				int mid = (low + high) >> 1;

				if (offsetHeights[mid] > relativeTop)
					high = mid - 1;
				else
					low = mid + 1;

			}

			if (low > 1) 
				firstVisibleChild = lineStops[low - 2];
			else if (low != 0) 
				firstVisibleChild = lineStops[low-1];
			
			
		}
	}

	auto validRect = destWindow->rect;

	validRect.Intersect(rect);

	auto wrapperWidth = rect.Width - currentStyle.margin.Left - currentStyle.margin.Right;
	auto wrapperHeight = rect.Height - currentStyle.margin.Bottom - currentStyle.margin.Top;
	auto offsetX = scrollX + rect.X + currentStyle.padding.Left;
	auto offsetY = scrollY + rect.Y + currentStyle.padding.Top;
	for (int i = firstVisibleChild; i != childs.size(); i++) {

		auto child = childs[i];

		if (child->isHidden)
			continue;

		if (child->needMeasure) {
			child->OnMeasure(destWindow);
		}

		child->rect.X = child->relativePos.X + offsetX;
		child->rect.Y = child->relativePos.Y + offsetY;

		if (child->rect.IntersectsWith(validRect)) 
			child->OnPaint(destWindow);
		else if (child->rect.Y > destWindow->rect.Height) {
			lastVisibleChild = i;
			break;
		}

	}

}
void UITable::OnLayout() {

	childcontentRect.Clear();

	if (childs.empty())return;

	float *rowCellHeights = new float[lineStops.size()];

	for (int i = 0; i != lineStops.size(); i++) 
		rowCellHeights[i] = 0.0f;
	
	std::vector<float>colCellWidth;
	int currentLine = 0;
	int currentCol = 0;
	
	for (int loc = 0; loc != childs.size(); loc++) {
	
		float cellHeight = childs[loc]->rect.Height + childs[loc]->currentStyle.margin.Top + childs[loc]->currentStyle.margin.Bottom;
		float cellWidth = childs[loc]->rect.Width + childs[loc]->currentStyle.margin.Left + childs[loc]->currentStyle.margin.Right;
		
		if (cellHeight < childs[loc]->currentStyle.lineHeight)
			cellHeight = childs[loc]->currentStyle.lineHeight;

		if (loc == lineStops[currentLine]) {
			++currentLine;
			currentCol = 0;
		}

		if (colCellWidth.size() <= currentCol) 
			colCellWidth.push_back(0.0f);
		

		if (!childs[loc]->isHidden) {
			if (colCellWidth[currentCol] < cellWidth)
				colCellWidth[currentCol] = cellWidth;

			if (rowCellHeights[currentLine] < cellHeight)
				rowCellHeights[currentLine] = cellHeight;
		}
		

		++currentCol;
	
	}
	
	offsetHeights.resize(lineStops.size()+1);
	offsetWidths.resize(colCellWidth.size()+1);
	offsetHeights[0] = 0.0f;
	offsetWidths[0] = 0.0f;
	
	for (int i = 1; i <= lineStops.size(); ++i) 
		offsetHeights[i] = offsetHeights[i - 1] + rowCellHeights[i-1];
	
	for (int i = 1; i <= colCellWidth.size(); ++i) 
		offsetWidths[i] = offsetWidths[i - 1] + colCellWidth[i - 1];
	
	currentCol = 0;
	currentLine = 0;

	for (int loc = 0; loc != childs.size(); ++loc) {
		if (loc == lineStops[currentLine]) {
			++currentLine;
			currentCol = 0;
		}
		childs[loc]->relativePos.X = offsetWidths[currentCol] + (colCellWidth[currentCol] - childs[loc]->rect.Width) / 2.0f;
		childs[loc]->relativePos.Y = offsetHeights[currentLine] + (rowCellHeights[currentLine] - childs[loc]->rect.Height) / 2.0f;
		++currentCol;
	}



	if (childs.size()>0) {
		childcontentRect.Width=offsetWidths[colCellWidth.size()-1]+ colCellWidth[colCellWidth.size() - 1];
		childcontentRect.Height= offsetHeights[lineStops.size() - 1] + rowCellHeights[lineStops.size() - 1];
	}

	delete rowCellHeights;

}

void UITable::ChildNeedUpdate() {
	if (!needMeasure) {
		needMeasure = true;

		if (parentElement)
			parentElement->ChildNeedUpdate();
	}

}