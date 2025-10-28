#include "companyData.h"

companyData::companyData()
{
	// Variable Initialization
	name = DEFAULT_COMPANY_NAME;
	address = DEFAULT_COMPANY_ADDRESS;
	city = DEFAULT_COMPANY_CITY;
	province = DEFAULT_COMPANY_PROVINCE;
	postalCode = DEFAULT_COMPANY_POSTALCODE;
	country = DEFAULT_COMPANY_COUNTRY;
	position = DEFAULT_COMPANY_POSITION;
	recruiter = DEFAULT_COMPANY_RECRUITER;
	paragraphs.resize(3);
	for (auto& para : paragraphs)
		para = 0;	
}

companyData::~companyData()
{
}