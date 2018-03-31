#pragma once
#include<windows.h>
#include"predef.h"
#include<unordered_map>
#include<d2d1.h>
#include<string>
#include<wincodec.h>
namespace AnimUI {
	class ImageLoaderInfo {
	public:
		ImageLoaderInfo() {
			converter = nullptr;
			imageName = nullptr;
		}
		ImageLoaderInfo(std::wstring& fileName ,HWND dest,int id) {
			imageName = fileName;
			desthWindow = dest;
			loadId = id;
			converter = nullptr;
		}
		std::wstring imageName;
		IWICFormatConverter*converter;
		HWND desthWindow;
		int loadId;
	};
	class AsyncImageLoader
	{
	public:
		static IWICImagingFactory *imageFactory;
		static void Start();
	//	static void RequestImage(wchar_t *filename, HWND hWndDest);
		static int LoadImageToCache(std::wstring&fileName, HWND hWndDest);
		static std::unordered_map<std::wstring, ImageLoaderInfo*>imageCache;
		static IWICFormatConverter* QueryImage(std::wstring&);
	};

}