// AutoCoverLetterFiller.cpp : Defines the entry point for the application.
//

#include "AutoCoverLetterFiller.h"

using namespace std;

CAutoCoverLetter::CAutoCoverLetter() {
    recruiterInfo = false;
	grabfiles = false;
    fileVerified = false;
	doxcsCreated = 0;
	selectedParagraph1 = 0;
    selectedParagraph2 = 1;
	selectedParagraph3 = 2;

    red = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    blue = ImVec4(0.3f, 0.3f, 0.8f, 1.0f);
    green = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
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
        
        if (selectTemplate() && docxAdj.verifyPath()) {

            ImGui::PushStyleColor(ImGuiCol_Text, green);
            ImGui::Text("File Verified");
            ImGui::PopStyleColor();

            numParagraphs = docxAdj.findParagraphs();
            for (int i = 0; i < numParagraphs.size(); i++)
                foundParagraphs.push_back("Template Paragraph " + std::to_string(i + 1));

        }

        else if (docxAdj.verifyPath()) {
            ImGui::PushStyleColor(ImGuiCol_Text, green);
            ImGui::Text("File Verified");
            ImGui::PopStyleColor();
        }

        else {
            ImGui::PushStyleColor(ImGuiCol_Text, red);
            ImGui::Text("File Not Verified");
            ImGui::PopStyleColor();
        }
    }
}

void CAutoCoverLetter::parameterAdjustment() {
    if (ImGui::CollapsingHeader("Parameter Adjustments", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Recruiter Info", &recruiterInfo);
        recruiterInformation();

        company.setName(inputText("Company Name", company.getName()));
        company.setAddress(inputText("Company Address", company.getAddress()));
        company.setCity(inputText("City", company.getCity()));
        company.setProvince(inputText("Province/State", company.getProvince()));
        company.setPostalCode(inputText("Postal Code", company.getPostalCode()));
        company.setCountry(inputText("Country", company.getCountry()));
        company.setPosition(inputText("Position", company.getPosition()));
        
		if (foundParagraphs.size() > 0)
            dropDownBox("Paragraph 1", selectedParagraph1);
        if (foundParagraphs.size() > 1)
		    dropDownBox("Paragraph 2", selectedParagraph2);
		if (foundParagraphs.size() > 2)
		    dropDownBox("Paragraph 3", selectedParagraph3);
            
    }
}

void CAutoCoverLetter::documentCreation() {

    if (ImGui::CollapsingHeader("Document Creation", ImGuiTreeNodeFlags_DefaultOpen)) {
        selectDesination();

        int errorNum = errorCheck();
        if (errorNum == 0) {
            if (ImGui::Button("Start")) {
                company.setParagraph1(numParagraphs[selectedParagraph1]);
                company.setParagraph2(numParagraphs[selectedParagraph2]);
                company.setParagraph3(numParagraphs[selectedParagraph3]);


                if (foundParagraphs.size() > 1)
                    if (docxAdj.editDocument(company))
                        doxcsCreated++;

            }
            if (doxcsCreated > 0) {
                ImGui::PushStyleColor(ImGuiCol_Text, blue);
                ImGui::Text("Documents Created: %d", doxcsCreated);
                ImGui::PopStyleColor();
            }
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Text, red);
            ImGui::Text("Errors Detected: %d", errorNum);
            ImGui::PopStyleColor();
        }
    }
}

void CAutoCoverLetter::recruiterInformation() {
    float myValue;
    if (ImGui::CollapsingHeader("Recruiter Information", &recruiterInfo))
        //ImGui::SliderFloat("Value", &myValue, 0.0f, 1.0f);
		company.setRecruiter(inputText("Recruiter Name", company.getRecruiter()));
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

bool CAutoCoverLetter::selectDesination() {
    bool wasChanged = false;


    char boxtext[CharBufferSize];
    const char* tag = Output_Tag;
    const char* prompttext = Output_PromptText;
    const char* filetype = Output_FileType;
    strncpy_s(boxtext, docxAdj.getOutputPath().c_str(), sizeof(boxtext));

    boxtext[sizeof(boxtext) - 1] = '\0';// 1 to remove null from string end

    if (ImGui::InputText(tag, boxtext, IM_ARRAYSIZE(boxtext))) {
        docxAdj.setOutputPath(boxtext);
		wasChanged = true;
    }
    ImGui::SameLine();

    if (ImGui::Button(prompttext)) {
        IGFD::FileDialogConfig config;
        config.path = FileSearchStarter;
        ImGuiFileDialog::Instance()->OpenDialog(tag, prompttext, filetype, config);


    }
    if (ImGuiFileDialog::Instance()->Display(tag)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            docxAdj.setOutputPath(ImGuiFileDialog::Instance()->GetCurrentPath());
            wasChanged = true;
        }

        ImGuiFileDialog::Instance()->Close();
    }

    return wasChanged;
}

bool CAutoCoverLetter::selectTemplate() {
    bool wasChanged = false;

    char boxtext[CharBufferSize];
    const char* tag = Template_Tag;
    const char* prompttext = Template_PromptText;
    const char* filetype = Template_FileType;
    strncpy_s(boxtext, docxAdj.getFilePath().c_str(), sizeof(boxtext));

    boxtext[sizeof(boxtext) - 1] = '\0';// 1 to remove null from string end

    if (ImGui::InputText(tag, boxtext, IM_ARRAYSIZE(boxtext))) {
        wasChanged = true;
        docxAdj.setFilePath(boxtext);
    }
    ImGui::SameLine();

    if (ImGui::Button(prompttext)) {
        IGFD::FileDialogConfig config;
        config.path = FileSearchStarter;
        ImGuiFileDialog::Instance()->OpenDialog(tag, prompttext, filetype, config);
    }

    if (ImGuiFileDialog::Instance()->Display(tag)){
        if (ImGuiFileDialog::Instance()->IsOk()){
            docxAdj.setFilePath(ImGuiFileDialog::Instance()->GetFilePathName());
            wasChanged = true;
        }

        ImGuiFileDialog::Instance()->Close();
    }

    return wasChanged;
}

std::string CAutoCoverLetter::inputText(const char* label, std::string input) {
    char text[CharBufferSize];
    strncpy_s(text, input.c_str(), sizeof(text));
    text[sizeof(text) - 1] = '\0';

    if (ImGui::InputText(label, text, IM_ARRAYSIZE(text))) {
        std::string returnVal = text;
        return returnVal;
    }
    else
		return input;
}

void CAutoCoverLetter::dropDownBox(const char* label, int &selectedItem) {
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
}

int CAutoCoverLetter::errorCheck() {
    int counter = 0;
    if (company.getAddress() == "")
        counter++;
    if (company.getCity() == "")
        counter++;
    if (company.getCountry() == "")
        counter++;
    if (company.getName() == "")
        counter++;
    if (company.getPosition() == "")
        counter++;
    if (company.getPostalCode() == "")
        counter++;
    if (company.getProvince() == "")
        counter++;
    if (foundParagraphs.empty())
        counter++;

    return counter;
}