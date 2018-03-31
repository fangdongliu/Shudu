#include "AppHelper.h"
#include"UIText.h"
using namespace AnimUI;
using namespace D2D1;

Helper::Helper()
{
	editableTextStyleOdd.needAutoDelete = false;
	notEditableTextStyleOdd.needAutoDelete = false;
	editableTextStyleEven.needAutoDelete = false;
	notEditableTextStyleEven.needAutoDelete = false;
	auto normalStyle = new UIStyle;
	auto focusStyle = new UIStyle;
	auto redColor = ColorF(ColorF::Red);
	normalStyle->SetFontFamily(L"gabriola");
	auto textColor = ColorF(0);
	normalStyle->SetTextColor(textColor);
	normalStyle->SetFontSize(30);
	SizeCaculator size(0, 0, 1.f / 9.f, 0, 1.0f);
	normalStyle->SetSizeCaculator(size);
	auto lightBlue = ColorF(ColorF::LightBlue);
	auto white = ColorF(ColorF::White);
	normalStyle->SetBackgroundColor(lightBlue);
	normalStyle->SetDuration(1.0f);

	auto hoverStyle = new UIStyle;
	hoverStyle->SetFontFamily(L"gabriola");
	hoverStyle->SetTextColor(ColorF(0xaaaaaa));
	hoverStyle->SetFontSize(30);
	hoverStyle->SetSizeCaculator(size);
	auto cyan = ColorF(ColorF::LightCyan);
	hoverStyle->SetBackgroundColor(cyan);
	hoverStyle->SetDuration(0.0f);

	focusStyle->SetFontFamily(L"gabriola");
	focusStyle->SetTextColor(ColorF(0xffffff));
	focusStyle->SetFontSize(30);
	focusStyle->SetSizeCaculator(size);
	focusStyle->SetBackgroundColor(ColorF(0x000000));
	focusStyle->SetDuration(0.0f);

	notEditableTextStyleOdd.SetState(UIStateHover, hoverStyle);
	notEditableTextStyleOdd.SetState(UIStateNormal, normalStyle);
	
	normalStyle = new UIStyle;

	normalStyle->SetFontFamily(L"gabriola");
	normalStyle->SetTextColor(textColor);
	normalStyle->SetFontSize(30);
	normalStyle->SetSizeCaculator(size);
	normalStyle->SetBackgroundColor(white);
	normalStyle->SetDuration(1.0f);

	hoverStyle = new UIStyle;
	hoverStyle->SetFontFamily(L"gabriola");
	hoverStyle->SetTextColor(textColor);
	hoverStyle->SetFontSize(30);
	hoverStyle->SetSizeCaculator(size);
	hoverStyle->SetBackgroundColor(cyan);
	hoverStyle->SetDuration(0.0f);

	notEditableTextStyleEven.SetState(UIStateHover, hoverStyle);
	notEditableTextStyleEven.SetState(UIStateNormal, normalStyle);

	normalStyle = new UIStyle;

	normalStyle->SetFontFamily(L"gabriola");
	normalStyle->SetTextColor(redColor);
	normalStyle->SetFontSize(30);
	normalStyle->SetSizeCaculator(size);
	normalStyle->SetBackgroundColor(lightBlue);
	normalStyle->SetDuration(1.0f);

	hoverStyle = new UIStyle;
	hoverStyle->SetFontFamily(L"gabriola");
	hoverStyle->SetTextColor(ColorF(ColorF::Yellow));
	hoverStyle->SetFontSize(30);
	hoverStyle->SetSizeCaculator(size);
	hoverStyle->SetBackgroundColor(cyan);
	hoverStyle->SetDuration(0.0f);

	editableTextStyleOdd.SetState(UIStateHover, hoverStyle);
	editableTextStyleOdd.SetState(UIStateNormal, normalStyle);
	editableTextStyleOdd.SetState(UIStateFocus, focusStyle);
	normalStyle = new UIStyle;

	normalStyle->SetFontFamily(L"gabriola");
	normalStyle->SetTextColor(redColor);
	normalStyle->SetFontSize(30);
	normalStyle->SetSizeCaculator(size);
	normalStyle->SetBackgroundColor(white);
	normalStyle->SetDuration(1.0f);

	hoverStyle = new UIStyle;
	hoverStyle->SetFontFamily(L"gabriola");
	hoverStyle->SetTextColor(ColorF(ColorF::Yellow));
	hoverStyle->SetFontSize(30);
	hoverStyle->SetSizeCaculator(size);
	hoverStyle->SetBackgroundColor(cyan);
	hoverStyle->SetDuration(0.0f);

	editableTextStyleEven.SetState(UIStateHover, hoverStyle);
	editableTextStyleEven.SetState(UIStateNormal, normalStyle);
	editableTextStyleEven.SetState(UIStateFocus, new UIStyle(*focusStyle));

}


Helper::~Helper()
{
}
UIElement* Helper::CreateJudgeBtn() {
	auto btn = new UIText;
	DECLARE_STYLE;
	btn->contentText = L"ÑéÖ¤";
	btn->textAlignVertical = TextAlignCenter;
	btn->textAlignHorizontal = TextAlignCenter;
	ColorF white = ColorF(ColorF::White);
	ColorF grey = ColorF(0xaaaaaa);
	SizeCaculator size(60.0f, 60.0f, 0, 0, 0);
	float borderRadius = 30.0f;
	normalStyle->SetSizeCaculator(size);
	hoverStyle->SetSizeCaculator(size);
	normalStyle->SetBackgroundColor(white);
	hoverStyle->SetBackgroundColor(grey);
	normalStyle->SetBorderRadius(borderRadius);
	hoverStyle->SetBorderRadius(borderRadius);
	normalStyle->SetDuration(1.0f);
	normalStyle->SetRight(10);
	normalStyle->SetTop(270);
	hoverStyle->SetRight(10);
	hoverStyle->SetTop(270);

	stateStyle->SetState(UIStateNormal, normalStyle);
	stateStyle->SetState(UIStateHover, hoverStyle);
	btn->SetStateStyle(stateStyle);
	return btn;
}