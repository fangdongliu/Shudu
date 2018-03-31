#include"helper.h"
#include"uiwindow.h"
#include<stdio.h>
using namespace AnimUI;
using namespace D2D1;
void AnimUI::WriteMessageToLogFile(const char* message) {
	FILE *logFile;
	fopen_s(&logFile, "fduilogFile.txt", "a+b");
	SYSTEMTIME time;
	GetLocalTime(&time);
	fprintf(logFile, "%d/%d/%d %d:%d ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
	fprintf(logFile, message);
	fclose(logFile);
}

MouseEventListener* AnimUI::PosMeasureListener() {
	return new NoParamMouseListener([](MouseEvent&e)->void {
		if (e.GetEventType() == MouseEventType::MouseEventLButtonDown) {
			auto rect = e.GetDestElement()->rect;
			printf("%f %f %f %f", rect.X, rect.Y, rect.Width, rect.Height);
		}
	});
}
void LinesPainter::Paint(UIElement*element, UIWindow*destWindow) {
	auto rect = element->rect;
	auto brush = destWindow->solidColorBrush;
	brush->SetColor(color);
	for (auto i : lineList) {
		auto point1 = D2D1::Point2F(rect.X + rect.Width*i.first.x, rect.Y + rect.Height*i.first.y);
		auto point2 = D2D1::Point2F(rect.X + rect.Width*i.second.x, rect.Y + rect.Height*i.second.y);
		destWindow->currentRenderTarget->DrawLine(point1, point2, brush, lineWidth);
	}
	//printf("%f %f %f %f\n", rect.X, rect.Y, rect.Width, rect.Height);
}

void LinesPainter::SetLines(int count, ...) {
	D2D1_POINT_2F *start = (D2D1_POINT_2F*)((char*)&count + 4);
	for (int i = 0; i < count; i++) {
		lineList.push_back({ *(start + i * 2),*(start + i * 2 + 1) });
	}
}

UIElement* Page::CreateCloseBtn() {
	UIElement *element = new UIElement;
	auto painter = new LinesPainter();
	auto white = D2D1::ColorF(ColorF::White);
	auto hoverStyle = new UIStyle;
	auto normalStyle = new UIStyle;
	auto stateStyle = new UIStateStyle;
	SizeCaculator size(30, 30, 0, 0, 0);
	
	painter->SetColor(white);
	painter->SetWidth(5);
	painter->SetLines(2, D2D1::Point2F(0.2, 0.2), D2D1::Point2F(0.8, 0.8), D2D1::Point2F(0.8, 0.2), D2D1::Point2F(0.2, 0.8));
	element->painter = painter;
	normalStyle->SetRight(10);
	normalStyle->SetTop(10);
	normalStyle->SetSizeCaculator(size);
	normalStyle->SetDuration(1.0f);
	hoverStyle->SetRight(10);
	hoverStyle->SetTop(10);
	hoverStyle->SetSizeCaculator(size);
	hoverStyle->SetBackgroundColor(ColorF(0x888888,0.7));
	stateStyle->SetState(UIStateNormal, normalStyle);
	stateStyle->SetState(UIStateHover, hoverStyle);
	element->SetStateStyle(stateStyle);
	element->mouseEventListener = new NoParamMouseListener(
		[](MouseEvent&e)->void {
		if (e.GetEventType() == MouseEventType::MouseEventLButtonUp) {
			if (e.GetDestElement() == e.GetDestWindow()->GetCaptureElement()) {
				e.GetDestWindow()->DoAction(WindowActionDestroy);
			}
		}
	});
	return element;
}

UIElement* Page::CreateMinimizeBtn() {
	auto element = new UIElement;
	auto painter = new LinesPainter();
	auto white = D2D1::ColorF(ColorF::White);
	auto hoverStyle = new UIStyle;
	auto normalStyle = new UIStyle;
	auto stateStyle = new UIStateStyle;
	SizeCaculator size(30, 30, 0, 0, 0);
	painter->SetColor(white);
	painter->SetLines(1, D2D1::Point2F(0.2, 0.6), D2D1::Point2F(0.8, 0.6));
	painter->SetWidth(5);
	element->painter = painter;
	normalStyle->SetDuration(1.0f);
	normalStyle->SetRight(40);
	normalStyle->SetTop(10);
	normalStyle->SetSizeCaculator(size);
	hoverStyle->SetRight(40);
	hoverStyle->SetTop(10);
	hoverStyle->SetSizeCaculator(size);
	hoverStyle->SetBackgroundColor(ColorF(0x888888, 0.7));
	stateStyle->SetState(UIStateNormal, normalStyle);
	stateStyle->SetState(UIStateHover, hoverStyle);
	element->SetStateStyle(stateStyle);

	element->mouseEventListener = new NoParamMouseListener(
		[](MouseEvent&e)->void {
		if (e.GetEventType() == MouseEventType::MouseEventLButtonUp) {
			if (e.GetDestElement() == e.GetDestWindow()->GetCaptureElement()) {
				e.GetDestWindow()->DoAction(WindowActionMinimize);
			}
		}
	});
	return element;
}
UIRelativeContainer* Page::CreateDraggableBackground() {
	UIRelativeContainer *relative = new UIRelativeContainer;
	relative->mouseEventListener = new NoParamMouseListener{
		[](MouseEvent&e)->void {
		if (e.GetEventType() == MouseEventType::MouseEventLButtonDown && !e.GetEventHandledId()) {
			ReleaseCapture();
			e.EventHandled();
			e.GetDestWindow()->SetCaptureElement(0);
			SendMessage(e.GetDestWindow()->GetHWnd(),WM_NCLBUTTONDOWN, HTCAPTION,0);
		}
	} };
	auto& style = relative->currentStyle;
	SizeCaculator size(0, 0, 1.f, 1.f, 0);
	style.SetSizeCaculator(size);
	style.SetBorderRadius(10.0f);
	return relative;
}
UIElement* Page::CreateDefaultPage() {
	UIRelativeContainer* relative = CreateDraggableBackground();
	
	relative->AddChild(CreateCloseBtn());
	relative->AddChild(CreateMinimizeBtn());
	return relative;
}

UIElement* Page::CreateWaitPage() {
	UIElement *element = new UIElement;
	auto& currentStyle = element->currentStyle;
	element->id = L"waitElement";
	auto owhite = ColorF(D2D1::ColorF::White, 0.8);
	currentStyle.SetBackgroundColor(owhite);
	SizeCaculator size(0, 0, 1, 1, 0);
	currentStyle.SetSizeCaculator(size);
	element->isHidden = true;
	element->mouseEventListener = new NoParamMouseListener([](MouseEvent&e)->void{
		if (e.GetEventType() == MouseEventType::MouseEventLButtonDown) {
			e.GetDestWindow()->EndWait();
		}
	});

	return element;
}