// AutoCoverLetterFiller.cpp : Defines the entry point for the application.
//

#include "AutoCoverLetterFiller.h"

using namespace std;

int main() {
	CAutoCoverLetter app;
	return app.run();
}

CAutoCoverLetter::CAutoCoverLetter() {
    recruiterInfo = false;
	grabfiles = false;
    fileVerified = false;
	selectedParagraph1 = 0;
    selectedParagraph2 = 1;
	selectedParagraph3 = 2;
}

CAutoCoverLetter::~CAutoCoverLetter()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

int CAutoCoverLetter::run() {

    if (initialize() == ERROR_INTIALIZE)
        return ERROR_INTIALIZE;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        events();
        interface();
        render();
    }

    return 0;
}

int CAutoCoverLetter::initialize() {
    // Initialize GLFW
    if (!glfwInit())
        return ERROR_INTIALIZE;

    // Setup OpenGL version (3.0+ is common)
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
        return ERROR_INTIALIZE;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return 0;
}

void CAutoCoverLetter::events() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void CAutoCoverLetter::interface() {

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Primary", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    templateSelection();

    parameterAdjustment();

    documentCreation();

    ImGui::End();
}

void CAutoCoverLetter::templateSelection() {

    if (ImGui::CollapsingHeader("Template Selection", ImGuiTreeNodeFlags_DefaultOpen)) {
        
        char text[128];
        strncpy_s(text, docxAdj.getFilePath().c_str(), sizeof(text));
        text[sizeof(text) - 1] = '\0';

        if (ImGui::InputText("###fileselector", text, IM_ARRAYSIZE(text)))
            docxAdj.setFilePath(text);
        ImGui::SameLine();

        if (ImGui::Button("Open File Dialog")) {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog(Template_FileKey, "Choose File", Template_FileType, config);
        }
		docxAdj.setFilePath(fileSelectorInstance(Template_FileKey, Template_FileType));
        
        
        if (fileVerified) {

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));//green
            ImGui::Text("File Verified");
            ImGui::PopStyleColor();

            if (grabfiles == true) {
				grabfiles = false;
                std::vector<int> numParagraphs = docxAdj.findParagraphs();
                for (int i = 0; i < numParagraphs.size(); i++)
                    foundParagraphs.push_back("Template Paragraph " + std::to_string(i+1));
            }
			
        }

        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));//red
            ImGui::Text("File Not Verified");
            ImGui::PopStyleColor();
        }
    }
}

void CAutoCoverLetter::parameterAdjustment() {


    if (ImGui::CollapsingHeader("Parameter Adjustments", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Recruiter Info", &recruiterInfo);
        recruiterInformation();

        company.setName(inputText("Company Name"));
        company.setAddress(inputText("Company Address"));
        company.setCity(inputText("City"));
        company.setProvince(inputText("Province/State"));
        company.setPostalCode(inputText("Postal Code"));
        company.setCountry(inputText("Country"));
        company.setPosition(inputText("Position"));

        
		if (foundParagraphs.size() > 0)
            dropDownBox("Paragraph 1", selectedParagraph1);
        if (foundParagraphs.size() > 1)
		    dropDownBox("Paragraph 2", selectedParagraph2);
		if (foundParagraphs.size() > 2)
		    dropDownBox("Paragraph 3", selectedParagraph3);

        
    }
}

void CAutoCoverLetter::documentCreation() {

    if (ImGui::CollapsingHeader("Document Creation, ImGuiTreeNodeFlags_DefaultOpen")) {
        //fileSelector(".docx");
        ImGui::Button("Start");
    }
}

void CAutoCoverLetter::recruiterInformation() {
    float myValue;
    if (ImGui::CollapsingHeader("Recruiter Information", &recruiterInfo))
        ImGui::SliderFloat("Value", &myValue, 0.0f, 1.0f);
}

void CAutoCoverLetter::render() {
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

std::string CAutoCoverLetter::fileSelectorInstance(const char* dialog, const char* fileType0) {
	std::string filepath = "";
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            //docxAdj.setFilePath(ImGuiFileDialog::Instance()->GetFilePathName());
          
            //filepath = ImGuiFileDialog::Instance()->GetCurrentPath();
            filepath = ImGuiFileDialog::Instance()->GetFilePathName();
            //wasChanged = true;


        }

        ImGuiFileDialog::Instance()->Close();
    }
	return filepath;
}

//bool CAutoCoverLetter::fileSelector(const char* fileType, std::string& filepath) {
    //bool wasChanged = false;

    

    

    
    //std::cout << filepath;
//}

std::string CAutoCoverLetter::inputText(const char* label) {
    std::string returnVal = "";
    static char text[128];

    if (ImGui::InputText(label, text, IM_ARRAYSIZE(text)))
        returnVal = text;
    return returnVal;
}


std::string CAutoCoverLetter::dropDownBox(const char* label, int &selectedItem) {
    std::string returnVal = "";
    if (ImGui::BeginCombo(label, foundParagraphs[selectedItem].c_str())) {
        for (int n = 0; n < foundParagraphs.size(); n++) {
            bool is_selected = (selectedItem == n);
                
            if (ImGui::Selectable(foundParagraphs[n].c_str(), is_selected)) {
                selectedItem = n;
            }        
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        
        ImGui::EndCombo();
    }
    returnVal = foundParagraphs[selectedItem];
    return returnVal;
}