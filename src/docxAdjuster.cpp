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

	for (auto p : doc.paragraphs())
		for (auto r : p.runs())
			if(r.get_text() != "")
				return true;
	return false;
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
	std::cout << output << std::endl;
	std::filesystem::path templateLoc = filePath;

	std::filesystem::create_directories(output.parent_path());

	// copy original to destination (overwrite if exists)
	std::filesystem::copy_file(templateLoc, output, std::filesystem::copy_options::overwrite_existing);

	duckx::Document newdoc(output.string());
	newdoc.open();
	for (auto p : newdoc.paragraphs()) {
		std::string paragraphText = "";
		for (auto r : p.runs()) {
			paragraphText += r.get_text();

			if (paragraphText.find(DATE_CODE) != std::string::npos)
				replace(paragraphText, DATE_CODE, __DATE__);
			if (paragraphText.find(COMPANY_CODE) != std::string::npos)
				replace(paragraphText, COMPANY_CODE, company.getName());
			p.runs().set_text(paragraphText);

		}
	}

	newdoc.save();
	return 0;
}  

void docxAdjuster::replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return;
	str.replace(start_pos, from.length(), to);
}