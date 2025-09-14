#include "docxAdjuster.h"


doxcAdjuster::doxcAdjuster()
{
	filePath = "";
	outputPath = "";
}

doxcAdjuster::~doxcAdjuster()
{

}

bool doxcAdjuster::verifyPath() {

	doc = duckx::Document(filePath);

	doc.open();

	for (auto p : doc.paragraphs())
		for (auto r : p.runs())
			if(r.get_text() != "")
				return true;
	return false;
}

std::vector<int> doxcAdjuster::findParagraphs() {
	std::vector <std::string> paragraphsFound;
	doc.open();
	for (auto p : doc.paragraphs()) {
		std::string paragraphText = "";
		for (auto r : p.runs())
			if (r.get_text() != "")
				paragraphText += (r.get_text());
		paragraphsFound.push_back(paragraphText);
	}

	return paragraphsFound;
}