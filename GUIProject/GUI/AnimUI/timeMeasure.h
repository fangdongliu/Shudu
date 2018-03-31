#pragma once
#include<vector>
#include<windows.h>
class TimeMeasurer {
public:
	TimeMeasurer() {
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequence);
	}
	void Start() {
		QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
	}
	double End() {
		__int64 end;
		QueryPerformanceCounter((LARGE_INTEGER*)&end);
		return (double)(end - m_start) / m_frequence;
	}
private:
	__int64 m_frequence;
	__int64 m_start;
};