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
        
        
        
        if (fileSelector(Template_FileKey))
			if (docxAdj.verifyPath()) // THIS CANNOT BE SIMPLIFIED INTO AN AND STATEMENT
                fileVerified = true;

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
        fileSelector(Output_FileKey);
        if(ImGui::Button("Start")) {
			docxAdj.editDocument(company);
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

bool CAutoCoverLetter::fileSelector(const char* key) {
	bool wasChanged = false;

    const char* label;
	const char* buttontext;
	const char* filetype;

    char text[CharBufferSize];

    if (key == Template_FileKey) {
        label = "###templateselector";
		buttontext = "Choose File";
		filetype = Template_FileType;
        strncpy_s(text, docxAdj.getFilePath().c_str(), sizeof(text));
    }
    else if (key == Output_FileKey) {
        label = "###outputselector";
		buttontext = "Choose Destination";
		filetype = Output_FileType;
        strncpy_s(text, docxAdj.getOutputPath().c_str(), sizeof(text));
    }
    else
		return wasChanged;

    text[sizeof(text) - 1] = '\0';

    if (ImGui::InputText(label, text, IM_ARRAYSIZE(text))) {
        if (key == Template_FileKey)
            docxAdj.setFilePath(text);
        else if (key == Output_FileKey)
            docxAdj.setOutputPath(text);
        //docxAdj.setFilePath(text);
    }
    ImGui::SameLine();

    if (ImGui::Button(buttontext)) {
        IGFD::FileDialogConfig config;
        config.path = FileSearchStarter;
        ImGuiFileDialog::Instance()->OpenDialog(key, buttontext, filetype, config);

        
    }
    if (ImGuiFileDialog::Instance()->Display(key)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            if(key == Template_FileKey)
				docxAdj.setFilePath(ImGuiFileDialog::Instance()->GetFilePathName());
			else if (key == Output_FileKey)
				docxAdj.setOutputPath(ImGuiFileDialog::Instance()->GetCurrentPath());

            //filepath = ImGuiFileDialog::Instance()->GetCurrentPath();
            //filepath = ImGuiFileDialog::Instance()->GetFilePathName();
            wasChanged = true;
			std::cout << "Selected file: " << ImGuiFileDialog::Instance()->GetCurrentPath() << std::endl;
			std::cout << docxAdj.getFilePath() << std::endl;
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