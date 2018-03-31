#pragma once
#include"LesStack.h"
class ID2D1Layer;
class ID2D1RenderTarget;
struct D2D1_LAYER_PARAMETERS;
namespace AnimUI {
	
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		void Release();
		void PushLayer(ID2D1RenderTarget*renderTarget,D2D1_LAYER_PARAMETERS& parameters);
		void PopLayer(ID2D1RenderTarget*renderTarget);
	private:
		LesStack<ID2D1Layer*>layerStack;
		int layerCount;
	};
}
