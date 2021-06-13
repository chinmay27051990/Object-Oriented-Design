#pragma once

///////////////////////////////////////////////////////////////////////////
// DependencyAnalysis.h : performs dependency analysis using #include    //
// ver 1.0                                                               //                                                               //
//                                                                       // 
// Application   : OOD-S19 Student Solution                              //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syarcuse University                  //
//				   cuvanjar@syr.edu                                      //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines DependencyAnalysis functionality which builds up
*  DependencyTable instance (already in use by CodeConverter) to hold the
*  extracted relationships. It accepts a list of files, a map holding a
*  vector which contains the #include statements for the corresponding
*  File. Then it performs the analysis and produces the final set of files.
*
*  Note: if a file is in the #include vector but not in the file list, it
*  will NOT be added as a relationship.
*
*  Required Files:
* =======================
*  Dependency.h Dependency.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 8 Apr 2019
*  - first release
*/

#include<vector>

using namespace std;

class Dependencies
{
private:

public:
	Dependencies();
	~Dependencies();

	inline static std::vector<std::string> includehref(std::vector<std::string> files, std::vector<std::string> data)
	{
		std::vector<std::string> dependency;
		std::string headerFile;
		for (size_t i = 0; i < data.size(); i++)
		{
			int firstQuote = data[i].find("#include \"");
			int lastQuote = data[i].find_last_of("\"");
			int lastSlash = data[i].find_last_of("/");
			if (firstQuote == 0)
			{
				headerFile = data[i].substr(lastSlash + 1);
				headerFile = headerFile.substr(0, headerFile.size() - 1);
				for (size_t j = 0; j < files.size(); j++)
				{
					if (headerFile == files[j])
					{
						dependency.push_back(headerFile);
					}
				}
			}
		}
		return dependency;
	}
};

