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
	std::filesystem::path output = outputPath + "\\" + company.getName() + "CoverLetter.docx";
	std::cout << output << std::endl;
	std::filesystem::path templateLoc = filePath;

	std::filesystem::create_directories(output.parent_path());

	// copy original to destination (overwrite if exists)
	std::filesystem::copy_file(templateLoc, output, std::filesystem::copy_options::overwrite_existing);

	duckx::Document newdoc(output.string());
	newdoc.open();
	for (auto p : newdoc.paragraphs()) {
		std::string paragraphText = "";
		for (auto r : p.runs())
			paragraphText += r.get_text();
		if (paragraphText != "") {
			//std::cout << paragraphText << std::endl;
			if (paragraphText.find(DATE_CODE) != std::string::npos) {
				time_t now = time(0);
				tm* ltm = localtime(&now);
				std::string date = std::to_string(ltm->tm_mday) + "/" + std::to_string(1 + ltm->tm_mon) + "/" + std::to_string(1900 + ltm->tm_year);
				std::string remove = DATE_CODE;
				paragraphText.replace(0, remove.length(), date);
				//p.runs().set_text(date);
			}
			if (paragraphText.find(COMPANY_CODE) != std::string::npos) {
				std::string remove = COMPANY_CODE;
				paragraphText.replace(0, remove.length(), company.getName());
				//p.runs().set_text(company.getName());
			}
			if (paragraphText.find(ADDRESS_CODE) != std::string::npos) {
				std::string remove = ADDRESS_CODE;
				paragraphText.replace(0, remove.length(), company.getAddress());
				//p.runs().set_text(company.getAddress());
			}
			if (paragraphText.find(CITY_CODE) != std::string::npos) {
				std::string remove = CITY_CODE;
				paragraphText.replace(0, remove.length(), company.getCity());
				//p.runs().set_text(company.getCity());
			}
			if (paragraphText.find(PROVINCE_CODE) != std::string::npos) {
				std::string remove = PROVINCE_CODE;
				paragraphText.replace(0, remove.length(), company.getProvince());
				//p.runs().set_text(company.getProvince());
			}
			if (paragraphText.find(POSTAL_CODE) != std::string::npos) {
				std::string remove = POSTAL_CODE;
				paragraphText.replace(0, remove.length(), company.getPostalCode());
				//p.runs().set_text(company.getPostalCode());
			}
			if (paragraphText.find(RECRUITER_CODE) != std::string::npos) {
				std::string remove = RECRUITER_CODE;
				paragraphText.replace(0, remove.length(), company.getRecruiter());
				//p.runs().set_text(company.getName());
			}
			if (paragraphText.find(JOB_CODE) != std::string::npos) {
				std::string remove = JOB_CODE;
				paragraphText.replace(0, remove.length(), company.getPosition());
				//p.runs().set_text(company.getPosition());
			}
			if (paragraphText.find(PARAGRAPH_CODE)) {
				std::string remove = PARAGRAPH_CODE;
				paragraphText.replace(0, remove.length(), "");
				//p.runs().set_text("");
			}
			std::cout <<"End:" << paragraphText << std::endl;
			
		}
		p.runs().set_text(paragraphText);
	}

	newdoc.save();
	return 0;
}  