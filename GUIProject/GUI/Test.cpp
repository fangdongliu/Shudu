#include"App.h"
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	App app;
	if (app.Init()) {
		app.Run();
	}
	return 0;
}
