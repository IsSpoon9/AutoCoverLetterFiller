#include "gui.h"

#ifdef _DEBUG
int main() {
	GuiApp app;
	int appResult = app.run();
	return appResult;
}
#else
#include "Windows.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
	GuiApp app;
	int appResult = app.run();
	return appResult;
}
#endif