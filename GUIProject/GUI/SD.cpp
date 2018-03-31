#include"SD.h"
#include<algorithm>
#include<stdlib.h>
using namespace std;
using namespace FD;

void SDMap::Reset() {
	memset(map, -1, sizeof(map));
	memset(validFlag, 0, sizeof(validFlag));
}

bool SDMap::TrySetElement(int x, int y, int c) {
	if (validFlag[0][y][c] == true ||
		validFlag[1][x][c] == true ||
		validFlag[2][(y / 3) * 3 + x / 3][c] == true)
		return false;
	validFlag[0][y][c] = true;
	validFlag[1][x][c] = true;
	validFlag[2][(y / 3) * 3 + x / 3][c] = true;
	return true;
}

void SDMap::UnSetElement(int x, int y) {
	validFlag[0][y][map[x][y]] = false;
	validFlag[1][x][map[x][y]] = false;
	validFlag[2][(y / 3) * 3 + x / 3][map[x][y]] = false;
	map[x][y] = -1;
}

void SDMap::InitBlock(int x, int y) {
	x *= 3;
	y *= 3;
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			map[i + x][j + y] = i * 3 + j;
		}
	}
	for (int i = 0; i<100; i++) {
		int a = rand() % 9;
		int b = rand() % 9;
		if (a != b) {
			swap(map[a / 3 + x][a % 3 + y], map[b / 3 + x][b % 3 + y]);
		}
	}
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			int line = i + x;
			int row = j + y;
			validFlag[0][row][map[line][row]] = true;
			validFlag[1][line][map[line][row]] = true;
			validFlag[2][(row / 3) * 3 + line / 3][map[line][row]] = true;
		}
	}
}

bool SDMap::FillEmpty(int n) {
	if (n == 81)
		return true;
	int x = n / 9, y = n % 9;
	if (map[x][y] != -1) {
		return FillEmpty(n + 1);
	}
	else {
		for (int i = 0; i<9; i++) {
			if (TrySetElement(x, y, i)) {
				map[x][y] = i;
				if (FillEmpty(n + 1))
					return true;
				UnSetElement(x, y);
			}
		}
	}
	return false;
}


void SD::GenerateMap(int cnt) {
	while (1) {
		SDMap map;
		map.InitBlock(0, 0);
		map.InitBlock(1, 2);
		map.InitBlock(2, 1);
		if (map.FillEmpty(3)) {
			mapQueue.push(map);
			if (--cnt <= 0)break;
		}
	}
}