#include "companyData.h"

companyData::companyData()
{
	name = "";
	address = "";
	city = "";
	province = "";
	postalCode = "";
	country = "";
	position = "";
	paragraphs.resize(3);
	for (auto& para : paragraphs)
		para = "";	
}

companyData::~companyData()
{
}