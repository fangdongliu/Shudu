#pragma once
#include<queue>

namespace FD {
	class SDMap {
	public:
		SDMap() {
			Reset();
		}
		void Reset();
		char map[10][10];
		bool validFlag[3][9][9];
		void InitBlock(int x, int y);
		bool FillEmpty(int n);
		bool TrySetElement(int x, int y, int c);
		void UnSetElement(int x, int y);
	};

	struct SD {
		void GenerateMap(int cnt);
		std::queue<SDMap>mapQueue;
	};
}