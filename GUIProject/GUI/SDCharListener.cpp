#include "SDCharListener.h"
#include"UIText.h"
#include"UIWindow.h"
using namespace AnimUI;
void SDCharListener::OnChar(AnimUI::CharEvent&e){
	
	if (e.GetCharCode() >= '1'&&e.GetCharCode() <= '9'&&e.GetDestElement()->isFocusable) {
		wchar_t str[10] = { 0 };
		str[0] = e.GetCharCode();
		UIText* uitext = (UIText*)e.GetDestElement();
		uitext->contentText = str;
		e.GetDestElement()->NeedUpdate();
		InvalidateRect(e.GetDestWindow()->GetHWnd(), 0, 0);
	}
}