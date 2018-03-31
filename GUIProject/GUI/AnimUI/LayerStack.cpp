#include "LayerStack.h"
#include"uiwindow.h"
#include<stdlib.h>
using namespace AnimUI;


LayerStack::LayerStack()
{
	layerCount = 0;
}


LayerStack::~LayerStack()
{
	Release();
}

void LayerStack::PushLayer(ID2D1RenderTarget*renderTarget,D2D1_LAYER_PARAMETERS& parameters) {
	if (layerCount <= layerStack.m_cur) {
		layerStack += nullptr;
		layerCount++;
		renderTarget->CreateLayer(&layerStack.m_src[layerStack.m_cur-1]);
	}
	else {
		layerStack.m_cur++;
	}
	renderTarget->PushLayer(parameters, layerStack.m_src[layerStack.m_cur-1]);
}
void LayerStack::PopLayer(ID2D1RenderTarget*renderTarget) {
	layerStack.Pop();
	
	renderTarget->PopLayer();
}
void LayerStack::Release() {
	for (int i=0; i != layerCount;i++) {
		SafeRelease<ID2D1Layer>(&layerStack.m_src[i]);
	}
	layerStack.Clear();
	layerCount = 0;
}