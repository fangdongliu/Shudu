#pragma once
#include"predef.h"
#include<mutex>
#include"UIElement.h"
#include"LayerStack.h"
#include"AnimationManager.h"
#include"timer.h"
#include"helper.h"
namespace AnimUI {
	enum WindowState {
		WindowStateHide,
		WindowStateMaxmized,
		WindowStateNormal,
	};
	enum WindowAction {
		WindowActionDestroy,
		WindowActionMinimize,
		WindowActionMaximize,
		WindowActionNormal,
	};

	class UIWindow {
	public:
		static UIWindow		* CreateMainWindow(int x, int y, int width, int height, bool IsTransparentWindow = false);
		static ID2D1Factory * d2dfactory;
		~UIWindow();

	public:
		AnimUI::RectF				rect;
		LayerStack				layerStack;
		
		ID2D1RenderTarget	 *	currentRenderTarget;
		ID2D1BitmapBrush	 *	bitmapBrush;
		ID2D1SolidColorBrush *	solidColorBrush;
		
	private:
		WindowState				windowState;
		std::mutex				mutex;
		bool					isTransparentWindow;
		bool					isMainWindow;
		bool					hasDefaultWrapper;

		AnimationManager		animationManager;
		HWND					ownhWnd;

		ID2D1HwndRenderTarget *	hwndRenderTarget;
		ID2D1DCRenderTarget	  *	dcRenderTarget;

		UIElement			  * focusElement;
		UIElement			  * currentPage;
		UIElement			  * hoverElement;
		UIElement			  * captureElement;
	
		std::unordered_map<std::wstring, FDImage*>bitmapCache;
	public:
		LRESULT				MessageProc(UINT message, WPARAM wParam, LPARAM lParam);
		UIWindow	*		CreateChildWindow(int x, int y, int width, int height, bool IsTransparentWindow = false);
		ID2D1Bitmap	*		RequestImage(std::wstring&imageName);
		
		inline Animation *RequestAnimation(UIElement*element, int* animId) {
			return animationManager.RequestAnimation(element, animId);
		}
		
		inline void PushLayer(D2D1_LAYER_PARAMETERS& parameters) { layerStack.PushLayer(currentRenderTarget,parameters); }
		inline void PopLayer() { layerStack.PopLayer(currentRenderTarget); }
		inline void DeleteAnimation(int animId) { animationManager.Delete(animId); }
		void BeginWait();
		void EndWait();

		void Center();
		void Show();
		void Hide();
		void DoAction(WindowAction action);

	private:
		UIWindow(int x,int y,int width,int height,HWND parentHWnd,bool IsTransparentWindow=false,bool IsMainWindow=false);
		void DiscardDeviceResource();


		HRESULT CreateDeviceResource();
		HRESULT CreateBrushResource();
		HRESULT Paint();
		void OnDestroy();
		void OnPaint();
		void OnResize(UINT width, UINT height);
	public:
		//Getters and Setters...
		void SetCurrentPage(UIElement*page, bool WithDefaultWrapper = true);

		inline WindowState GetWindowState() { return windowState; }

		inline HWND GetHWnd() { return ownhWnd; }
		
		inline UIElement* GetHoverElement() {
			return hoverElement;
		}
		inline UIElement* GetFocusElement() {
			return focusElement;
		}
		inline UIElement* GetCaptureElement() {
			return captureElement;
		}
	
		void SetCaptureElement(UIElement*element);
		void SetFocusElement(UIElement*element);
		void SetHoverElement(UIElement*element);
	};
	

}