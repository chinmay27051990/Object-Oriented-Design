///////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h : performs dependency analysis using #include    //
// ver 1.0                                                               //                                                               //
//                                                                       // 
// Application   : OOD-S19 Student Solution                              //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syarcuse University                  //
//				   cuvanjar@syr.edu                                      //
///////////////////////////////////////////////////////////////////////////

#include "Dependencies.h"



Dependencies::Dependencies()
{
}


Dependencies::~Dependencies()
{
}

#ifdef TEST_DEPENDENCIES
#include<fstream>
#include<vector>
#include<string>
#include<streambuf>
#include<iostream>
#include<sstream>

int main()
{
	std::vector<std::string> testDependency, fileInlines;
	std::vector<std::string> files;
	files.push_back("C:\\Users\\Pranav\\source\\repos\\Project2Starter\\test\\display.h");
	std::string line;
	for (size_t i = 0; i < files.size(); i++)
	{
		fileInlines.clear();
		std::ifstream myFile(files[i]);
		while (std::getline(myFile, line))
		{
			std::istringstream inputStream(line);
			fileInlines.push_back(line);
		}
	}
	Dependencies dependencies;
	testDependency= dependencies.includehref(files,fileInlines);
	for (size_t i = 0; i < testDependency.size(); i++) {
		std::cout << testDependency[i];
	}
}
#endif 
