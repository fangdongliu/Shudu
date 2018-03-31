#pragma once
#include "UIElement.h"
namespace AnimUI {
	enum TextAlign {
		TextAlignStart,
		TextAlignCenter,
		TextAlignEnd
	};
	class UIText :
		public UIElement
	{
	public:
		UIText();
		~UIText();
		void			OnPaint(UIWindow *destWindow)override;
		void			OnMeasure(UIWindow*destWindow)override;
		RuntimeId		GetRuntimeId()override {return RuntimeId::RuntimeIdUIText;}
		
		std::wstring	contentText;
		IDWriteTextLayout *textLayout;
		TextAlign		textAlignHorizontal,textAlignVertical;
	};
}
