#pragma once

#include <string>
#include <vector>

class companyData
{
public:
	companyData();
	~companyData();

	void setName(const std::string& n) { name = n; }
	void setAddress(const std::string& addr) { address = addr; }
	void setCity(const std::string& c) { city = c; }
	void setProvince(const std::string& prov) { province = prov; }
	void setPostalCode(const std::string& pc) { postalCode = pc; }
	void setCountry(const std::string& ctry) { country = ctry; }
	void setPosition(const std::string& pos) { position = pos; }
	void setRecruiter(const std::string& rec) { recruiter = rec; }

	void setParagraph1(int para) { paragraphs[0] = para; }
	void setParagraph2(int para) { paragraphs[1] = para; }
	void setParagraph3(int para) { paragraphs[2] = para; }

	std::string getName() const { return name; }
	std::string getAddress() const { return address; }
	std::string getCity() const { return city; }
	std::string getProvince() const { return province; }
	std::string getPostalCode() const { return postalCode; }
	std::string getCountry() const { return country; }
	std::string getPosition() const { return position; }
	std::string getRecruiter() const { return recruiter; }

	int Paragraph1() const { return paragraphs[0]; }
	int Paragraph2() const { return paragraphs[1]; }
	int Paragraph3() const { return paragraphs[2]; }
	std::vector<std::string> getParagraphs() const { return paragraphs; }

private:
	std::string name;
	std::string address;
	std::string city;
	std::string province;
	std::string postalCode;
	std::string country;
	std::string position;
	std::string recruiter;

	std::vector<int> paragraphs;

};
