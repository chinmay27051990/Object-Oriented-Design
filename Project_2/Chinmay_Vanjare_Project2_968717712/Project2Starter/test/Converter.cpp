///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 2.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syarcuse University                  //
//				   cuvanjar@syr.edu                                      //
// Source        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////

#include "Converter.h"

Conversion::Conversion()
{
}

Conversion::~Conversion()
{
}

#ifdef TEST_CONVERTER

int main()
{
	std::vector<std::string> testVectorpath;
	testVectorpath.push_back("./test.cpp");
	std::vector<std::string> testVectorname;
	testVectorname.push_back("test.cpp");
	Conversion conv;
	std::vector<std::string> testVector = conv.filetoString(testVectorpath, testVectorname);
	for (size_t i = 0; i < testVector.size(); i++)
	{
		std::cout << testVector[i];
	}
}
#endif