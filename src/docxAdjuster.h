#pragma once

#include <duckx.hpp>
//https://github.com/amiremohamadi/DuckX


#include <cstdlib> //convert docx to pdf
#include <vector>
#include <string>
#include <iostream>

class doxcAdjuster
{
public:
	doxcAdjuster();
	~doxcAdjuster();

	bool verifyPath();
	std::vector<std::string> findParagraphs();
	//bool editDocument();

	std::string getFilePath() const { return filePath; }
	void setFilePath(const std::string& path) { filePath = path; }

	std::string getFileName() const { return fileName; }
	void setFileName(const std::string& name) { fileName = name; }

	std::string getOutputPath() const { return outputPath; }
	void setOutputPath(const std::string& path) { outputPath = path; }

private:
	duckx::Document doc;

	std::string filePath;
	std::string fileName;
	std::string outputPath;
};
