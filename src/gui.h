// Gui.h
// Main GUI Application Class
// By: Ethan Siemens.
// Date: 2025-10-28

#pragma once
// Standard Includes
#include <iostream>
#include <string>	
#include <vector>
#include <random>

// Used for file dialog
// Not mine, see below for link
#include <ImGuiFileDialog.h>
//https://github.com/aiekick/ImGuiFileDialog

// Used for Gui
// Not mine, see below for link
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
//https://github.com/ocornut/imgui


// Custom Includes
#include <docxAdjuster.h>
#include <companyData.h>

// Gui Constants
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_NAME "Auto Cover Letter Filler v1.0"

#define ERROR_INTIALIZE 1

#define CharBufferSize 128

#define FileSearchStarter ""
#define Template_FileType ".docx"
#define Template_PromptText "Choose Template Docx"
#define Template_Tag "###templateselector"

#define Output_FileType nullptr
#define Output_PromptText "Choose Destination Folder"
#define Output_Tag "###outputselector"


class GuiApp
{
public:
	// Constructor and Destructor
	GuiApp();
	~GuiApp();

	// Main application loop
	int run();
	void events();// for events
	void interface();// for computations
	void render();// for rendering

	// Application Sections
	void templateSelection();
	void parameterAdjustment();
	void documentCreation();

	// File Selection
	bool selectDesination();
	bool selectTemplate();

	// UI Elements
	std::string inputText(const char* label, std::string input);
	void paraDropDown(const char* label, int& selectedItem);

	// System Functions
	int errorCheck();

private:
	// System Variables
	bool initializeError;
	ImVec4 red;
	ImVec4 blue;
	ImVec4 green;
	int doxcsCreated;
	GLFWwindow* window;

	// App Variables
	companyData company;
	docxAdjuster docxAdj;
	
	// Paragraph Variables
	std::vector<std::string> foundParagraphs;
	std::vector<int> numParagraphs;
	int selectedParagraph1;
	int selectedParagraph2;
	int selectedParagraph3;


};