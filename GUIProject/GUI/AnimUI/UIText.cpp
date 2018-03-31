#include "UIText.h"
#include"UIWindow.h"
using namespace AnimUI;


UIText::UIText()
{
	textAlignVertical = TextAlignStart;
	textAlignHorizontal = TextAlignStart;
	textLayout = nullptr;
}


UIText::~UIText()
{
	SafeRelease<IDWriteTextLayout>(&textLayout);
}

void UIText::OnMeasure(UIWindow*destWindow) {

	
	float paddingX = currentStyle.padding.Left + currentStyle.padding.Right;
	float paddingY = currentStyle.padding.Top + currentStyle.padding.Bottom;
	IDWriteTextFormat*textFormat = nullptr;

	UIElement::dwriteFactory->CreateTextFormat(currentStyle.fontFamily.c_str(), 0,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, currentStyle.fontSize, L"", &textFormat);
	
	SafeRelease<IDWriteTextLayout>(&textLayout);
	
	UIElement::dwriteFactory->CreateTextLayout(
		contentText.c_str(),
		static_cast<UINT32>(contentText.length()),
		textFormat,
		rect.Width-paddingX,rect.Height-paddingY,
		&textLayout
	);

	DWRITE_TEXT_METRICS metrics;

	if (textLayout) {
		needMeasure = false;

		if (textAlignHorizontal == TextAlignCenter) 
			textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		else if (textAlignHorizontal == TextAlignStart)
			textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		else {
			textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		}

		if (textAlignVertical == TextAlignCenter) 
			textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		else if (textAlignVertical == TextAlignStart)
			textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		else {
			textLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		}
		DWRITE_TEXT_RANGE range = { 0,-1 };
		textLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD,range);

		textLayout->GetMetrics(&metrics);

		if (currentStyle.isWidthOfChild) {
			rect.Width = metrics.width + paddingX;
		}
		if (currentStyle.isHeightOfChild) {
			rect.Height = metrics.height + paddingY;
		}
	}
	SafeRelease<IDWriteTextFormat>(&textFormat);
}
void UIText::OnPaint(UIWindow *destWindow) {

	PaintBackground(destWindow);

	auto brush = destWindow->solidColorBrush;
	brush->SetColor(currentStyle.textColor);

	if(textLayout)
		destWindow->currentRenderTarget->DrawTextLayout(D2D1::Point2F(rect.X + currentStyle.margin.Left, rect.Y + currentStyle.margin.Top),
			textLayout, brush);

	PaintBorder(destWindow);

}