#pragma once
#include<vector>
#include"AnimationObject.h"
#include<d2d1.h>
#include"AnimUITypes.h"

namespace AnimUI {
	class AnimationManager;
	class UIElement;
	inline float linearPolater(float v) {
		return v;
	}
	class Animation{
		friend AnimationManager;
	private:
		UIElement                     * dest;
		std::vector<AnimationObject*>	animList;
		float							startTime;
		float							duration;
		
		float							(*interpolater)(float);
		bool							isRequireMeasure;
		Animation(float(*interpolater)(float) = linearPolater);
		
	public:
		~Animation();
		void(*animationEndCallBack)(Animation*);
		void Release();

		bool Update(float delta);

		inline UIElement* GetDest() {
			return dest;
		}

		inline void RequireMeasure() { if (!isRequireMeasure)isRequireMeasure = true; }

		template<typename T>
		inline void Add(T& dest, T start, T end) {
			
			if(start!=end)
				animList.push_back(new AnimationTemplate<T>(dest, start, end));

		}
		template<>
		inline void Add(D2D1_COLOR_F& dest, D2D1_COLOR_F start, D2D1_COLOR_F end) {

			if (start.a == end.a&&start.b == end.b&&start.g == end.g&&start.r == end.r)
				return;
			animList.push_back(new AnimationTemplate<D2D1_COLOR_F>(dest, start, end));

		}

		template<>
		inline void Add(AnimUI::RECTF& dest, AnimUI::RECTF start, AnimUI::RECTF end) {
			Add(dest.Left, start.Left, end.Left);
			Add(dest.Right, start.Right, end.Right);
			Add(dest.Top, start.Top, end.Top);
			Add(dest.Bottom, start.Bottom, end.Bottom);
		}

		template<>
		inline void Add(D2D1_SIZE_F& dest, D2D1_SIZE_F start, D2D1_SIZE_F end) {
			Add(dest.width, start.width, end.width);
			Add(dest.height, start.height, end.height);
		}

		template<>
		inline void Add(D2D1_POINT_2F& dest, D2D1_POINT_2F start, D2D1_POINT_2F end) {
			Add(dest.x, start.x, end.x);
			Add(dest.y, start.y, end.y);
		}

		inline void Restart() {
			startTime = 0.0f;
		}

		inline void SetDuration(float duration_) {
			duration = duration_;
		}
		
		
	};

}