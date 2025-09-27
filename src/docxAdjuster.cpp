#include "docxAdjuster.h"


docxAdjuster::docxAdjuster()
{
	filePath = "";
	outputPath = "";
}

docxAdjuster::~docxAdjuster()
{

}

bool docxAdjuster::verifyPath() {

	doc = duckx::Document(filePath);

	doc.open();

	return doc.is_open();
}

std::vector<int> docxAdjuster::findParagraphs() {
	std::vector <int> paragraphsFound;
	int linenum = 0;

	doc.open();
	for (auto p : doc.paragraphs()) {
		linenum++;
		std::string paragraphText = "";
		for (auto r : p.runs())
			if (r.get_text() == PARAGRAPH_CODE)
				paragraphsFound.push_back(linenum);
	}

	return paragraphsFound;
}

bool docxAdjuster::editDocument(companyData company) {
	if (filePath == "" || outputPath == "")
		return false;

	//duckx::Document copy = duckx::Document(outputPath);
	std::filesystem::path output = outputPath + "\\" + company.getName() + " CoverLetter.docx";
	std::filesystem::path templateLoc = filePath;

	std::filesystem::create_directories(output.parent_path());

	// copy original to destination (overwrite if exists)
	std::filesystem::copy_file(templateLoc, output, std::filesystem::copy_options::overwrite_existing);

	duckx::Document newdoc(output.string());
	newdoc.open();

	std::vector<int> foundparagraphs = findParagraphs();
	std::vector<int> selectedparagraphs = company.getParagraphs();
	std::vector<int> removeparagraphs;

	for (int i = 0; i < foundparagraphs.size(); i++) {
		if (foundparagraphs[i] != selectedparagraphs[0] &&
			foundparagraphs[i] != selectedparagraphs[1] &&
			foundparagraphs[i] != selectedparagraphs[2]) 
				removeparagraphs.push_back(foundparagraphs[i]);

	}
	

	foundparagraphs.erase(foundparagraphs.begin(), foundparagraphs.begin() + 3);
	for (auto p : newdoc.paragraphs()) {

	}



	for (auto p : newdoc.paragraphs()) {
		for (auto run = p.runs(); run.has_next(); run.next()) {
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
			replace(text, PARAGRAPH_CODE, " ");
			
			//std::cout << text << std::endl;
			run.set_text(text);
			
		}
	}

	newdoc.save();
	return 0;
}  

void docxAdjuster::replace(std::string& str, const std::string& from, const std::string& to) {
	size_t pos = 0;
	while ((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
}