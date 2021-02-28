/* This program finds an object from vector of objects by attribute
Example: From a vector of objects want get the object which has an attribute set as "ABC"*/

// Author: Mufti Md
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class MyObject
{
public:
	void set(const std::string& myString)
	{
		m_MyPart = myString;
	}

	std::string get() const
	{
		return m_MyPart;
	}
private:
	std::string m_MyPart;
};

template<typename T>
class AttribFind
{
public:
	AttribFind(const std::string& s) : mYs(s){}

	bool operator()(const T& obj) const
	{
	   return obj.get() == mYs;
	}

private:
	const std::string& mYs;

};

int main()
{
    	MyObject a, b, c;
    	a.set("Hello");
    	b.set("Mufti");
	c.set("Hi");

	std::string myString = "Mufti";

	std::vector<MyObject> mObj;
	mObj.push_back(a);
	mObj.push_back(b);
	mObj.push_back(c);

	std::vector<MyObject>::iterator it = find_if(mObj.begin(), 
	    mObj.end(), AttribFind<MyObject>(myString));
	std::cout << it->get() << std::endl;
	
	return 0;
}