// AutoCoverLetterFiller.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <iostream>
#include <string>	
#include <vector>

// TODO: Reference additional headers your program requires here.
#include <ImGuiFileDialog.h>
//https://github.com/aiekick/ImGuiFileDialog

//use for gui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
//https://github.com/ocornut/imgui

//test change

#include <docxAdjuster.h>
#include <gui.h>
#include <companyData.h>

//#define DEBUG_MODE_ON

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_NAME "Auto Cover Letter Filler v1.0"


#define ERROR_INTIALIZE 1

#define CharBufferSize 128

#define FileSearchStarter "."
#define Template_FileType ".docx"
#define Template_FileKey "templatefilekey"
#define Output_FileType nullptr
#define Output_FileKey "outputfilekey"

class CAutoCoverLetter
{
public:
	CAutoCoverLetter();
	~CAutoCoverLetter();

	int run();
	int initialize();//ran with run once

	void events();// for events
	void interface();// for computations
	void render();// for rendering

	void templateSelection();
	void parameterAdjustment();
	void recruiterInformation();
	void documentCreation();

	bool fileSelector(const char* key);

	//void stringTochar(char* text, std::string string);
	std::string inputText(const char* label, std::string input);
	void dropDownBox(const char* label, int& selectedItem);


private:
	bool recruiterInfo;

	companyData company;

	docxAdjuster docxAdj;

	bool fileVerified;
	bool grabfiles;

	std::vector<std::string> foundParagraphs;
	std::vector<int> numParagraphs;

	int selectedParagraph1;
	int selectedParagraph2;
	int selectedParagraph3;

	GLFWwindow* window;
};