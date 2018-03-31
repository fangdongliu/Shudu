#include<iostream>
#include<algorithm>
#include<unordered_map>
#include"SD.h"
using namespace FD;
using namespace std;

enum ProgramInput {
	ProgramInputDefault,
	ProgramInputSolute,
	ProgramInputGenerate,
	ProgramInputHelp
};


void Generate(int n) {
	SD sd;
	sd.GenerateMap(n);
	FILE *file;
	fopen_s(&file,"sudoku.txt", "wb");
	if (file) {
		int cnt = 0;
		while (!sd.mapQueue.empty()) {
			if(cnt++!=0)fprintf(file, "\r\n");
			char numMap[20] = { '1','2','3','4','5','6','7','8','9' };
			auto sdMap = sd.mapQueue.front();
			sd.mapQueue.pop();
			swap(numMap[3], numMap[sdMap.map[0][0]]);
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					fprintf(file, "%c%s", numMap[sdMap.map[i][j]], j == 8 ? "\r\n" : " ");
				}
			}
		}
		fclose(file);
	}
}
void Solute(FILE *file) {
	SDMap sdMap;
	FILE *outFile;
	fopen_s(&outFile,"sudoku.txt", "wb");
	int lineCnt = 0,sdcnt=0;
	char data[30][30];
	char numMap[20] = { '1','2','3','4','5','6','7','8','9' };
	if (outFile) {
		while (fgets(data[lineCnt], 30, file)) {
			if (strlen(data[lineCnt]) < 10)continue;
			else lineCnt++;
			if (lineCnt == 9) {
				if (sdcnt > 0) {
					fprintf(outFile, "\r\n");
				}
				sdMap.Reset();
				lineCnt = 0;
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						if (data[i][j * 2] != '0') {
							sdMap.TrySetElement(i, j, data[i][j * 2] - '1');
							sdMap.map[i][j] = data[i][j * 2] - '1';
						}
					}
				}
				sdMap.FillEmpty(0);
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						fprintf(outFile, "%c%s", numMap[sdMap.map[i][j]], j == 8 ? "\r\n" : " ");
					}
				}
				sdcnt++;
			}

		}
		fclose(outFile);
	}
}
int main(int argc,char **argv) {
	if (argc >= 2) {
		ProgramInput input=ProgramInputDefault;
		if (argv[1][0]=='-'&&argv[1][1]=='s'){
			input = ProgramInputSolute;
		}
		else if (argv[1][0] == '-'&&argv[1][1] == 'c') {
			input = ProgramInputGenerate;
		}
		else if (argv[1][0] == '/'&&argv[1][1] == '?') {
			input = ProgramInputHelp;
		}
		switch (input) {
		case ProgramInput::ProgramInputGenerate:
			if (argc > 2) {
				int n;
				if (sscanf_s(argv[2], "%d", &n)) {
					Generate(n);
				}
				else {
					printf("error01:wrong input\n");
				}
			}
			break;
		case ProgramInput::ProgramInputHelp:
			printf("input:-s 'filename' -- to solute a problem\n");
			printf("input:-c n -- to create n problems\n");
			break;
		case ProgramInput::ProgramInputSolute:
			if (argc > 2) {
				FILE *file=nullptr;
				fopen_s(&file,argv[2], "rb");
				if (file) {
					Solute(file);
					fclose(file);
				}
				else {
					printf("error02:no such file--%s\n", argv[2]);
				}
				
			}
			break;
		default:
			printf("error01:wrong input\n");
			break;
		}
	}

	
}