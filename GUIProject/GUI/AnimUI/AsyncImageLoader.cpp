#include "AsyncImageLoader.h"
#include<mutex>
using namespace AnimUI;
IWICImagingFactory *AsyncImageLoader::imageFactory = nullptr;
std::unordered_map<std::wstring, ImageLoaderInfo*>AsyncImageLoader::imageCache;
namespace {
	DWORD threadId;
	int loadId;
	std::mutex mutex;
	enum LoadImageFlag {
		LoadImageFlagToCache,
		LoadImageFlagToBitmap
	};
	void LoadThreadImage(ImageLoaderInfo*info){
		HRESULT hr = S_OK;
		IWICBitmapDecoder *decoder = nullptr;
		IWICBitmapFrameDecode *pFrame = nullptr;
		hr = AsyncImageLoader::imageFactory->CreateDecoderFromFilename(info->imageName.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
		if (!SUCCEEDED(hr)) {
			goto end;
		}
		hr = decoder->GetFrame(0, &pFrame);
		if (!SUCCEEDED(hr)) {
			goto end;
		}
		hr = AsyncImageLoader::imageFactory->CreateFormatConverter(&info->converter);
		if (!SUCCEEDED(hr)) {
			goto end;
		}
		hr = info->converter->Initialize(
			pFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			0,
			0.f,
			WICBitmapPaletteTypeCustom
		);
		if (!SUCCEEDED(hr)) {
			SafeRelease<IWICFormatConverter>(&info->converter);
			goto end;
		}
		else {
			mutex.lock();
			AsyncImageLoader::imageCache[info->imageName] = info;
			mutex.unlock();
		}
		PostMessageW(info->desthWindow, WM_IMAGELOADED, 0, 0);
	end:
		SafeRelease<IWICBitmapDecoder>(&decoder);
		SafeRelease<IWICBitmapFrameDecode>(&pFrame);
	}
}
DWORD WINAPI LoadThread(LPVOID) {
	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0)) {
		switch (msg.message) {
		case WM_LOADIMAGE:
			LoadThreadImage((ImageLoaderInfo*)msg.wParam);
		
			break;
		}
	}

	return 0;
}

void AsyncImageLoader::Start() {
	
	CreateThread(0, 0, LoadThread, 0, 0, &threadId);
}

int AsyncImageLoader::LoadImageToCache(std::wstring& filename,HWND hWndDest) {

	PostThreadMessage(threadId, WM_LOADIMAGE, (WPARAM)(new ImageLoaderInfo(filename,hWndDest,++loadId)), 0);
	return loadId;

}
IWICFormatConverter* AsyncImageLoader::QueryImage(std::wstring&imageName) {
	mutex.lock();
	auto i = imageCache[imageName];
	mutex.unlock();
	if (i) {
		return i->converter;
	}
	return nullptr;
}