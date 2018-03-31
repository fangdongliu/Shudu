#include<windows.h>
#include<timeMeasure.h>
#include"uiwindow.h"
#include"predef.h"
#include"AsyncImageLoader.h"
#include<stack>
using namespace std;
using namespace AnimUI;
ID2D1Factory* UIWindow::d2dfactory = nullptr;
UIWindow::UIWindow(int x, int y, int width, int height, HWND parentHWnd, bool IsTransparentWindow, bool IsMainWindow):rect(0,0,width,height) {
	
	currentPage			= Page::CreateDefaultPage();
	hoverElement		= nullptr;
	focusElement		= nullptr;
	captureElement		= nullptr;
	bitmapBrush			= nullptr;
	solidColorBrush		= nullptr;
	dcRenderTarget		= nullptr;
	hwndRenderTarget	= nullptr;

	isTransparentWindow = IsTransparentWindow;
	isMainWindow		= IsMainWindow;
	
	ownhWnd = CreateWindowExW(0, L"fdwindow", 0, WS_POPUP,
		x, y, width, height, parentHWnd, 0, HINST_THISCOMPONET, this);


}
UIWindow::~UIWindow() {
	
	DiscardDeviceResource();
	
	for (auto i : bitmapCache) 
		delete i.second;
	
	SafeDelete(currentPage);

}

UIWindow* UIWindow::CreateMainWindow(int x, int y, int width, int height, bool IsTransparentWindow) {
	return new UIWindow(x, y, width, height, 0, IsTransparentWindow, true);
}
UIWindow* UIWindow::CreateChildWindow(int x, int y, int width, int height, bool IsTransparentWindow) {
	return new UIWindow(x, y, width, height, ownhWnd, IsTransparentWindow, false);
}

void UIWindow::DoAction(WindowAction action) {
	PostMessageW(ownhWnd, WM_DOACTION, action, 0);
}
void UIWindow::Hide() {
	::ShowWindow(ownhWnd, SW_HIDE);
	windowState = WindowStateHide;
}
void UIWindow::OnDestroy() {

	if (isMainWindow) 
		PostQuitMessage(0);
	
	delete this;
}
void UIWindow::Show() {
	ShowWindow(ownhWnd, SW_SHOW);
	UpdateWindow(ownhWnd);
	windowState = WindowStateNormal;
}
void UIWindow::Center() {
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int screenWidth=GetSystemMetrics(SM_CXSCREEN);
	MoveWindow(ownhWnd, (screenWidth - rect.Width) / 2, (screenHeight - rect.Height) / 2, rect.Width, rect.Height, 0);
}

void UIWindow::OnPaint() {
	static TimeMeasurer m;
	static int count = 0;
	HRESULT hr = S_OK;
	hr=CreateDeviceResource();
	if (SUCCEEDED(hr)) {
		if (count == 0) {
			m.Start();
		}
		count++;
		if (count == 60) {
			printf("frame %lf\n", m.End());
			count = 0;
		}
		if (isTransparentWindow) {
			BLENDFUNCTION _Blend = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
			PAINTSTRUCT ps;
			BeginPaint(ownhWnd, &ps);
			HDC mdc = CreateCompatibleDC(ps.hdc);
			HBITMAP hbitmap = CreateCompatibleBitmap(ps.hdc, rect.Width, rect.Height);
			SelectObject(mdc, hbitmap);
			RECT rc = { 0,0,rect.Width,rect.Height };
			dcRenderTarget->BindDC(mdc, &rc);
			Paint();
			DWORD dwExStyle = GetWindowLong(ownhWnd, GWL_EXSTYLE);
			if (!(dwExStyle & 0x80000))
				SetWindowLong(ownhWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);
			POINT p = { 0,0 };
			SIZE size = { rect.Width,rect.Height };
			UpdateLayeredWindow(ownhWnd, ps.hdc, nullptr, &size, mdc, &p, RGB(0, 0, 0), &_Blend, ULW_ALPHA);
			DeleteDC(mdc);
			DeleteObject(hbitmap);
			EndPaint(ownhWnd, &ps);
		}
		else {
			Paint();
			ValidateRect(ownhWnd, 0);
		}
	}
	if (hr == D2DERR_RECREATE_TARGET) {
		DiscardDeviceResource();
	}
}
HRESULT UIWindow::Paint() {
	HRESULT hr = S_OK;
	currentRenderTarget->BeginDraw();
	currentRenderTarget->Clear(D2D1::ColorF(0xffffff,0.0f));
	currentRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	if (currentPage) {
		if (currentPage->needMeasure) {
			currentPage->CaculateSize(rect.Width, rect.Height);
			currentPage->OnMeasure(this);
		}
		currentPage->OnPaint(this);
	}
	currentRenderTarget->EndDraw();
	return hr;
}
ID2D1Bitmap* UIWindow::RequestImage(std::wstring&imageName){

	auto image = bitmapCache[imageName];

	if (image) {

		if (image->state == ImageState::ImageStateLoaded) 
			return image->bitmap;
		else if(image->state==ImageState::ImageStateLoading){
			
			auto i = AsyncImageLoader::QueryImage(imageName);
			
			if (i) {
				image->state = ImageState::ImageStateLoaded;
				currentRenderTarget->CreateBitmapFromWicBitmap(i, &image->bitmap);
			}
		}
		else if(image->state==ImageState::ImageStateUnload){

			auto i = AsyncImageLoader::QueryImage(imageName);

			if (i) {
				image->state = ImageState::ImageStateLoaded;
				currentRenderTarget->CreateBitmapFromWicBitmap(i, &image->bitmap);
			}
			else {
				image->state = ImageState::ImageStateLoading;
				AsyncImageLoader::LoadImageToCache(imageName, ownhWnd);
			}
		}

	}
	else {
		auto i = AsyncImageLoader::QueryImage(imageName);

		if (i) {
			image = new FDImage();
			image->state = ImageState::ImageStateLoaded;
			currentRenderTarget->CreateBitmapFromWicBitmap(i, &image->bitmap);
			bitmapCache[imageName] = image;
		}
		else {
			image = new FDImage();
			image->state = ImageState::ImageStateLoading;
			bitmapCache[imageName] = image;
			AsyncImageLoader::LoadImageToCache(imageName, ownhWnd);
		}

	}

	return image->bitmap;
}

void UIWindow::OnResize(UINT width, UINT height) {

	if (hwndRenderTarget) 
		hwndRenderTarget->Resize(D2D1::SizeU(width, height));
	
	rect.Width = width;
	rect.Height = height;

	if (currentPage) {
		currentPage->NeedUpdate();
	}
	InvalidateRect(ownhWnd, 0, 0);
}

void UIWindow::SetCaptureElement(UIElement*element) {

	if (captureElement != element) {
		if (captureElement) 
			captureElement->ChangeCurrentStyle(this, UIStateNormal);
		
		captureElement = element;

		if (element) {

			element->ChangeCurrentStyle(this, UIStateCapture);

			if (element->isFocusable) {
				if (focusElement) {
					focusElement->ChangeCurrentStyle(this, UIStateNormal);
					focusElement = element;
				}
			}
		}
		else {	
			if (focusElement)
				focusElement->ChangeCurrentStyle(this, UIStateFocus);
			if(hoverElement&&hoverElement!=focusElement)
				hoverElement->ChangeCurrentStyle(this, UIStateHover);
		}
	}
}
void UIWindow::SetFocusElement(UIElement*element) {

	if (focusElement != element) {

		auto temp = focusElement;
		focusElement = element;

		if (temp)
			temp->ChangeCurrentStyle(this, UIStateNormal);
	
		if (focusElement)
			focusElement->ChangeCurrentStyle(this, UIStateFocus);
	}

}
void UIWindow::SetHoverElement(UIElement*element) {

	if (hoverElement != element) {
		if (GetCaptureElement() != nullptr) {
		
			if (hoverElement == GetCaptureElement()) 
				captureElement->ChangeCurrentStyle(this, UIStateHover);
			else if (element == GetCaptureElement()) 
				element->ChangeCurrentStyle(this, UIStateCapture);
		
			hoverElement = element;
		}
		else if (GetFocusElement() != element) {
			if (hoverElement) 
				hoverElement->OnMouseOut(this);
			
			hoverElement = element;

			if (hoverElement)
				hoverElement->ChangeCurrentStyle(this, UIStateHover);

			if(focusElement)
			GetFocusElement()->ChangeCurrentStyle(this, UIStateFocus);
		}
		else {
			if (hoverElement)
				hoverElement->OnMouseOut(this);

			hoverElement = element;

			if (hoverElement)
				hoverElement->ChangeCurrentStyle(this, UIStateHover);

		}
	}

}

void UIWindow::SetCurrentPage(UIElement* element, bool WithDefaultWrapper) {
	SafeDelete(currentPage);
	hasDefaultWrapper = WithDefaultWrapper;
	if (WithDefaultWrapper) {
		currentPage = Page::CreateDefaultPage();
		auto* childs = &((UIContainer*)currentPage)->childs;
		childs->insert(childs->begin(),element );
		element->SetParent(currentPage);
	}
	else 
	currentPage = element;
}

LRESULT UIWindow::MessageProc(UINT message, WPARAM wParam, LPARAM lParam) {
	if (currentPage == nullptr) {
		if (message == WM_DESTROY) {
			OnDestroy();
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(ownhWnd, message, wParam, lParam);
	}

	//mouse event
	{
		MouseEvent e(this);
		RECT windowRect;
		switch (message) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			SetFocus(ownhWnd);
			SetCapture(ownhWnd);
			e = MouseEvent(this, (MouseEventType)(message - 0x201), (float)LOWORD(lParam), (float)HIWORD(lParam));
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			ReleaseCapture();
			e = MouseEvent(this, (MouseEventType)(message - 0x201), (float)LOWORD(lParam), (float)HIWORD(lParam));
			break;
		case WM_MOUSEMOVE:
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = ownhWnd;
			TrackMouseEvent(&tme);
			e = MouseEvent(this, MouseEventType::MouseEventMouseMove, (float)LOWORD(lParam), (float)HIWORD(lParam));
			break;
		case WM_MOUSEWHEEL: 
			GetWindowRect(ownhWnd, &windowRect);
			e = MouseEvent(this, MouseEventType::MouseEventScroll,
				(float)LOWORD(lParam) - windowRect.left, (float)HIWORD(lParam) - windowRect.top, wParam);
			break;
		default:
			goto nomouseevent;
		}
		currentPage->OnMouse(e);
		if (e.GetEventHandledId()==1) {
			InvalidateRect(ownhWnd, 0, 0);
		}
		return 0;
	}

nomouseevent:
	switch (message) {
	case WM_DOACTION:

		switch (wParam) {
		case WindowActionDestroy:
			DestroyWindow(ownhWnd);
			break;
		case WindowActionMinimize:
			ShowWindow(ownhWnd, SW_MINIMIZE);
			break;
		case WindowActionMaximize:
			ShowWindow(ownhWnd, SW_MAXIMIZE);
			break;
		case WindowActionNormal:
			ShowWindow(ownhWnd, SW_NORMAL);
			break;
		}
		return 0;
	case WM_MOUSELEAVE:
		if (hoverElement) {
			hoverElement->OnMouseOut(this);
			InvalidateRect(ownhWnd, 0, 0);
		}
		SetHoverElement(nullptr);
		return 0;
	case WM_KEYDOWN:
		if (focusElement) {
			bool heldShift = (GetKeyState(VK_SHIFT) & 0x80) != 0;
			bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;
			KeyEvent e(this, wParam, heldShift, heldControl);
			e.SetDestElement(GetFocusElement());
			focusElement->OnKey(e);
		}
		return 0;
	case WM_IMAGELOADED:
		InvalidateRect(ownhWnd, 0, 0);
		break;
	case WM_CHAR:
		if (focusElement) {
			CharEvent e(this, wParam);
			e.SetDestElement(GetFocusElement());
			focusElement->OnChar(e);
		}
		return 0;
	
	case WM_SIZE:
		switch (wParam) {
		case SIZE_MINIMIZED:
			windowState = WindowStateHide;
			printf("minimize\n");
			break;
		case SIZE_MAXIMIZED:
			windowState = WindowStateMaxmized;
			printf("maximize\n");
			break;
		case SIZE_RESTORED:
			printf("normal\n");
			windowState = WindowStateNormal;
			break;
		}
		OnResize((short)LOWORD(lParam),(short)HIWORD(lParam));
		
		
	case WM_PAINT:
	{
		bool needUpdate = animationManager.Update();
		OnPaint();
		if(needUpdate)
		InvalidateRect(ownhWnd, 0, 0);
		
	}
		
		if (currentPage) {
			//currentPage->needMeasure = true;
		}
		

		return 0;
	case WM_DISPLAYCHANGE:
		InvalidateRect(ownhWnd, 0, 0);
		return 0;
	case WM_DESTROY:
		OnDestroy();
	case WM_ERASEBKGND:
		return 0;

	case WM_SHOWWINDOW:
		if (!wParam) {
			windowState = WindowStateHide;
		}
		else {
			windowState = WindowStateNormal;
		}
		return 0;
		
	}


	return DefWindowProc(ownhWnd, message, wParam, lParam);

}
HRESULT UIWindow::CreateBrushResource() {
	
	HRESULT hr=S_OK;
	
	if (!bitmapBrush) 
		hr = currentRenderTarget->CreateBitmapBrush(0, &bitmapBrush);
	
	if (SUCCEEDED(hr) && !solidColorBrush) 
		hr = currentRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0xffffff, 1.0f), &solidColorBrush);
	
	return hr;
}
HRESULT UIWindow::CreateDeviceResource() {
	
	HRESULT hr = S_OK;
	
	if (isTransparentWindow) {
		if (!dcRenderTarget) {

			D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
			
			hr = d2dfactory->CreateDCRenderTarget(&properties, &dcRenderTarget);
			currentRenderTarget = dcRenderTarget;
			currentRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
			if (SUCCEEDED(hr)) 
				hr = CreateBrushResource();
	
		}
	}
	else if (!hwndRenderTarget) {
			D2D1_SIZE_U size = D2D1::SizeU(rect.Width, rect.Height);
			
			hr = d2dfactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(ownhWnd, size), &hwndRenderTarget);
			currentRenderTarget = hwndRenderTarget;
			currentRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
			if (SUCCEEDED(hr)) 
				hr = CreateBrushResource();
	}
	
	return hr;
}
void UIWindow::DiscardDeviceResource() {

	for (auto i : bitmapCache) {
		SafeRelease<ID2D1Bitmap>(&i.second->bitmap);
		i.second->state = ImageState::ImageStateUnload;
	}

	currentRenderTarget = nullptr;
	SafeRelease<ID2D1BitmapBrush>(&bitmapBrush);
	SafeRelease<ID2D1SolidColorBrush>(&solidColorBrush);
	SafeRelease<ID2D1HwndRenderTarget>(&hwndRenderTarget);
	SafeRelease<ID2D1DCRenderTarget>(&dcRenderTarget);
	layerStack.Release();
}

void UIWindow::BeginWait() {
	if (hasDefaultWrapper) {
		wstring str(L"waitElement");
		auto e = currentPage->GetElementById(str);
		if (e) {
			e->Show();
		}
	}
}

void UIWindow::EndWait() {
	if (hasDefaultWrapper) {
		wstring str(L"waitElement");
		auto e = currentPage->GetElementById(str);
		if (e) {
			e->Hidden();
		}

	}

}