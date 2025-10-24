#include "AutoCoverLetterFiller.h"

#ifdef _DEBUG
int main() {
	CAutoCoverLetter app;
	int appResult = app.run();
	return appResult;
}
#else
#include "Windows.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
	CAutoCoverLetter app;
	int appResult = app.run();
	return appResult;
}
#endif