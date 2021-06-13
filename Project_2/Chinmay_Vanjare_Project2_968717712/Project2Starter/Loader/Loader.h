#pragma once
///////////////////////////////////////////////////////////////////////////
// Loader.h : defines facade/executive for OOD Pr1 S19                   //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Student Solution                              //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syarcuse University                  //
//				   cuvanjar@syr.edu                                      //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodePublisher which uses different components of
*  CodeConverter, Display and DirectoryN to put together the entire
*  functionality of the CodePublisher. The mentioned packages are indep-
*  ndent of each other and their only interaction is through this facade
*
*  Required Files:
* =======================
*  StringUtilities.h, CodeUtilities.h, DirExplorerN.h
*
*  Maintainence History:
* =======================
*  ver 1.0 - 8 Apr 2019
*  - added file parsing support to extract information
*  - added automated dependency analysis support
*/
#include "../StringUtilities/StringUtilities.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include <regex>

using namespace CodeUtilities;
using namespace FileSystem;

class Loader
{
private:
	std::vector<std::string> address;
	std::vector<std::string> fileMatch;
	std::vector<std::string> filenames;
public:
	Loader();
	~Loader();

	std::vector<std::string> filenameDisplay(int argc, char* argv[])
	{
		ProcessCmdLine pcl(argc, argv);
		DirExplorerN de(pcl.path());
		for (auto patt : pcl.patterns())
		{
			de.addPattern(patt);
		}
		de.recurse();
		de.search();
		address = de.filepathReturn();
		std::regex cmdRegex = std::regex(argv[5]);
		for (size_t i = 0; i < address.size(); i++)
		{
			if (std::regex_match(FileSystem::Path::getName(address[i]), cmdRegex))
			{
				fileMatch.push_back(address[i]);
			}
		}
		if (fileMatch.empty())
		{
			return fileMatch;
		}
		for (size_t j = 0; j < fileMatch.size(); j++)
		{
			std::cout << "\n" << FileSystem::Path::getName(fileMatch[j]) << "\n";
		}
		return fileMatch;
	}

	std::vector<std::string> onlyfileNamesreturn()
	{
		for (size_t i = 0; i < fileMatch.size(); i++)
		{
			filenames.push_back(FileSystem::Path::getName(fileMatch[i]));
		}
		return filenames;
	}
};

