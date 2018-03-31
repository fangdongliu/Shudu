#include<Windows.h>

#include"AnimUI.h"
#include"predef.h"
#include<dwrite.h>
#include"uiwindow.h"
#include"helper.h"
#include<wincodec.h>

#include"AsyncImageLoader.h"
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
using namespace AnimUI;

bool InitFDUI(HICON icon) {
	CoInitialize(0);
	WNDCLASSW wc = { 0 };
	wc.hInstance = HINST_THISCOMPONET;
	wc.lpszClassName = L"fdwindow";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = icon;
	wc.cbWndExtra = sizeof(void*);
	wc.lpfnWndProc = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT {
		UIWindow *window = static_cast<UIWindow*>(reinterpret_cast<void*>(GetWindowLongPtr(hWnd, 0)));
		if (window) {
			return window->MessageProc(message, wParam, lParam);
		}
		else {
			if (message == WM_CREATE) {
				LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				window = static_cast<UIWindow*>(cs->lpCreateParams);
				SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(window));
				return window->MessageProc(message, wParam, lParam);
			}
			else return DefWindowProc(hWnd, message, wParam, lParam);
		}
	};
	if (!RegisterClassW(&wc)) {
		WriteMessageToLogFile("register class - failed\r\n");
		return false;
	}
	
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &UIWindow::d2dfactory);
	if (FAILED(hr)) {
		WriteMessageToLogFile("create d2dfactory - failed\r\n");
		return false;
	}
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&UIElement::dwriteFactory);
	if (FAILED(hr)) {
		WriteMessageToLogFile("create dwritefactory - failed\r\n");
		return false;
	}
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&AsyncImageLoader::imageFactory)
	);
	AsyncImageLoader::Start();
	return true;
}
void ReleaseFDUI() {
	
	SafeRelease<IDWriteFactory>(&UIElement::dwriteFactory);
	SafeRelease<ID2D1Factory>(&UIWindow::d2dfactory);
	SafeRelease<IWICImagingFactory>(&AsyncImageLoader::imageFactory);
	UnregisterClassW(L"fdshadow", HINST_THISCOMPONET);
	UnregisterClassW(L"fdwindow", HINST_THISCOMPONET);
	CoUninitialize();
}
int MessageLoop() {
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}