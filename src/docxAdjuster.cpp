#include "docxAdjuster.h"

// Constructor and Destructor
docxAdjuster::docxAdjuster()
{
	// Variables Initialization
	filePath = "";
	outputPath = "";
}
docxAdjuster::~docxAdjuster()
{

}

// Methods
bool docxAdjuster::verifyPath() {
	//Verify that the file path is valid
	doc = duckx::Document(filePath);
	doc.open();
	return doc.is_open();
}
std::vector<int> docxAdjuster::findParagraphs() {
	std::vector <int> paragraphsFound;
	int paranum = 0;

	doc.open();
	for (auto p : doc.paragraphs()) {
		paranum++;//Paragraph number counter
		std::string paragraphText = "";
		for (auto r : p.runs())
			if (r.get_text() == PARAGRAPH_CODE)
				paragraphsFound.push_back(paranum+1);
		//Plus one because we want the next paragraph, not the one with the code
	}

	return paragraphsFound;
}
bool docxAdjuster::editDocument(companyData company) {
	if (filePath == "" || outputPath == "")
		return false; // Invalid paths, Dont Run.
	
	// Create output directory
	std::filesystem::path output = outputPath + "\\" + company.getName() + " CoverLetter.docx";
	std::filesystem::create_directories(output.parent_path());

	// Copy original to destination
	std::filesystem::path templateLoc = filePath; // Template location
	std::filesystem::copy_file(templateLoc, output, std::filesystem::copy_options::overwrite_existing);

	// Open new document
	duckx::Document newdoc(output.string());
	newdoc.open();

	// Find paragraphs to keep and remove
	std::vector<int> foundparagraphs = findParagraphs(); // All paragraphs
	std::vector<int> selectedparagraphs = company.getParagraphs(); // Wanted paragraphs
	std::vector<int> removeparagraphs; // Paragraphs to remove
	std::vector<std::string> paragraphsKeepTexts; // Texts of wanted paragraphs

	// Collect texts of wanted paragraphs
	int paranum;
	for (int i = 0; i < selectedparagraphs.size(); i++) {
		paranum = 1; // Reset paragraph number counter for each selected paragraph
		for (auto p : newdoc.paragraphs()) {
			paranum++;
			if (paranum - 1 == selectedparagraphs[i]) {
				std::string paraText = "";
				for (auto run = p.runs(); run.has_next(); run.next())
					paraText += run.get_text();
				paragraphsKeepTexts.push_back(paraText);
			}
		}
	}

	// Find indexes of paragraphs to remove
	for (int i = 0; i < foundparagraphs.size(); i++) {
		if (foundparagraphs[i] != selectedparagraphs[0] &&
			foundparagraphs[i] != selectedparagraphs[1] &&
			foundparagraphs[i] != selectedparagraphs[2])
			removeparagraphs.push_back(foundparagraphs[i] + 1);
		//Have to plus one again because we are removing the next paragraph

	}

	// Change all elements to desired
	paranum = 1;
	int paraPlaced = 0;
	for (auto p : newdoc.paragraphs()) {
		paranum++;
		for (auto run : p.runs()) {
			std::string text = run.get_text();

			// Skip empty runs
			if(text == "")
				continue;

			// Simple replace: find "OLD" and replace with "NEW"
			replace(text, DATE_CODE, __DATE__);
			replace(text, COMPANY_CODE, company.getName());
			replace(text, ADDRESS_CODE, company.getAddress());
			replace(text, CITY_CODE, company.getCity());
			replace(text, PROVINCE_CODE, company.getProvince());
			replace(text, POSTAL_CODE, company.getPostalCode());
			replace(text, RECRUITER_CODE, company.getRecruiter());
			replace(text, JOB_CODE, company.getPosition());
			replace(text, PARAGRAPH_CODE, "");
			
			//Change to wanted paragraphs
			if (paraPlaced < paragraphsKeepTexts.size())
				for (int i = paraPlaced; i < foundparagraphs.size(); i++) {
					if ((paranum - 1 == foundparagraphs[i])) {
						text = paragraphsKeepTexts[i];
						paraPlaced++;
					}
				}

			//Remove unwanted paragraphs
			for (int i = 0; i < removeparagraphs.size(); i++)
				if (paranum == removeparagraphs[i])
					text = "";	

			
			//std::cout << text << std::endl;
			run.set_text(text);
			
		}
	}

	newdoc.save();

	#ifdef DEBUG // DEBUG THINGS
		for (int i = 0; i < paragraphsKeepTexts.size(); i++)
			std::cout << "Keeping paragraph: " << paragraphsKeepTexts[i] << std::endl;
		for (int i = 0; i < removeparagraphs.size(); i++)
			std::cout << "Removing paragraph number: " << removeparagraphs[i] << std::endl;
	#endif 

	return true;
}  
void docxAdjuster::replace(std::string& str, const std::string& from, const std::string& to) {
	// Replace all instances of "from" with "to" in str.
	size_t pos = 0;
	while ((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
}