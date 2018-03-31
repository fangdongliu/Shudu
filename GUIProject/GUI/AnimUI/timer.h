#pragma once
namespace AnimUI {
	class Timer
	{
	public:
		Timer();
		float TotalTime()const;
		float DeltaTime()const;
		void Tick();
		void Stop();
		void Start();
		void Reset();
	protected:
		double mSecondsPerCount;
		double mDeltaTime;
		bool mStopped;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mBaseTime;
		__int64 mPrevTime;
		__int64 mCurrTime;
	};

}