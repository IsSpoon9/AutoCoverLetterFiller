#pragma once

#include "duckx.hpp"
//https://github.com/amiremohamadi/DuckX

#include <companyData.h>

#include <cstdlib> //convert docx to pdf
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#define DATE_CODE "[Date]"
#define COMPANY_CODE "[Company Name]" 
#define ADDRESS_CODE "[Company Address]"
#define CITY_CODE "[Company City]"
#define PROVINCE_CODE "[Company Province/State]"
#define POSTAL_CODE "[Company Postal Address]"
#define RECRUITER_CODE "[Recruitment Officer]"
#define JOB_CODE "[Job Position]"
#define PARAGRAPH_CODE "[Template Paragraph Begin]"


class docxAdjuster
{	
public:
	docxAdjuster();
	~docxAdjuster();

	bool verifyPath();
	std::vector<int> findParagraphs();
	bool editDocument(companyData company);
	void replace(std::string& str, const std::string& from, const std::string& to);

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
