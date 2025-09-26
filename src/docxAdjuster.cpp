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