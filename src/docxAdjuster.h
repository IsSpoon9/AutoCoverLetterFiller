// docxAdjuster.h
// Edits and Reads Documents
// By: Ethan Siemens
// Date: 2025-10-28

#pragma once
// Standard Includes
#include <cstdlib> //convert docx to pdf
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

// For Editing DOCX Files
// Not mine, see below for link
#include <duckx.hpp>
//https://github.com/amiremohamadi/DuckX

// Custom Includes
#include <companyData.h>

// Template Codes
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
	// Constructor and Destructor
	docxAdjuster();
	~docxAdjuster();

	// Methods
	bool verifyPath();
	std::vector<int> findParagraphs();
	bool editDocument(companyData company);
	void replace(std::string& str, const std::string& from, const std::string& to);

	// Setters
	void setFilePath(const std::string& path) { filePath = path; }
	void setFileName(const std::string& name) { fileName = name; }
	void setOutputPath(const std::string& path) { outputPath = path; }

	// Getters
	std::string getFilePath() const { return filePath; }
	std::string getFileName() const { return fileName; }
	std::string getOutputPath() const { return outputPath; }

private:
	// Variables
	duckx::Document doc;
	std::string filePath;
	std::string fileName; //Name of the file, not path.
	std::string outputPath;
};
