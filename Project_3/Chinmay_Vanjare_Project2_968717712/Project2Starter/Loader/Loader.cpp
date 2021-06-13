///////////////////////////////////////////////////////////////////////////
// Loader.h : defines facade/executive for OOD Pr1 S19                   //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Student Solution                              //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syarcuse University                  //
//				   cuvanjar@syr.edu                                      //
///////////////////////////////////////////////////////////////////////////

#include "Loader.h"
#include<conio.h>



Loader::Loader()
{
}


Loader::~Loader()
{
}

#ifdef TEST_LOADER



int main(int argc, char* argv[])
{
	std::vector<std::string> filePaths;
	std::vector<std::string> fileNames;
	std::vector<std::string> processedFiles;
	Loader ld;
	filePaths = ld.filenameDisplay(argc, argv);
	if (filePaths.empty())
	{
		std::cout << "\nNo files matched the given Regex!\n";
		_getch();
		return 0;
	}
	fileNames = ld.onlyfileNamesreturn();
}

#endif 