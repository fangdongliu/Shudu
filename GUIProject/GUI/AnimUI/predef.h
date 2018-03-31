#pragma once
#ifdef _WINDOWS_
#ifndef HINST_THISCOMPONENT 
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONET ((HINSTANCE)&__ImageBase) 
#endif
#define WM_LOADIMAGE WM_APP+1
#define WM_IMAGELOADED WM_APP+2
#define WM_DOACTION WM_APP+3
#endif
class ID2D1Bitmap;
#define SafeDelete(currentObject) \
		if (currentObject != nullptr&&currentObject->needAutoDelete){\
			delete currentObject;\
			currentObject = nullptr;\
		}


namespace AnimUI {
	class UIWindow;
	class UIElement;
	enum class ImageState {
		ImageStateUnload,
		ImageStateLoading,
		ImageStateLoaded
	};
	class FDImage {
	public:
		FDImage() {
			state = ImageState::ImageStateUnload;
			bitmap = nullptr;
		}
		ImageState state;
		ID2D1Bitmap *bitmap;
	};
	template <typename InterfaceType>
	inline void SafeRelease(InterfaceType** currentObject)
	{
		if (*currentObject != nullptr)
		{
			(*currentObject)->Release();
			*currentObject = nullptr;
		}
	}
	
	

	
	class SizeCaculator {
	public:
		SizeCaculator() {

		}
		SizeCaculator(float solidW,float solidH, float percentPW,float percentPH,float percentW) {
			m_solidH = solidH;
			m_solidW = solidW;
			m_percentPH = percentPH;
			m_percentPW = percentPW;
			m_percentW = percentW;
		}
		inline float CalcWidth(float PW) {
			return m_solidW + PW*m_percentPW;
		}
		inline float CalcHeight(float PH, float W) {
			return m_solidH + PH*m_percentPH + W*m_percentW;
		}
	public:
		float m_solidW;
		float m_solidH;
		float m_percentPW;
		float m_percentPH;
		float m_percentW;
	};
	class AutoDelete
	{
	public:
		AutoDelete() {
			needAutoDelete = true;
		}
		bool needAutoDelete;
	};
	class CanvasPainter:public AutoDelete {
	public:
		virtual void Paint(UIElement*element,UIWindow*dest) {};
	};
}