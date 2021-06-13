/////////////////////////////////////////////////////////////////////
//  Executive.cpp - Starter code for Project #2                    //
//  ver 1.0                                                        //
//  Language:      Visual C++, Visual Studio 2015                  //
//  Platform:      Dell XPS 8920, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects             //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "Executive.h"
#include "../Loader/Loader.h"
#include <conio.h> 
#include "../Converter/Converter.h"
#include "../Display/Display.h"

using namespace Lexer;
using namespace Utilities;
using Demo = Logging::StaticLogger<1>;

#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper

using namespace CodeAnalysis;

int main(int argc, char* argv[])
{
	try 
	{
		std::vector<std::string> filePaths;
		std::vector<std::string> fileNames;
		std::vector<std::string> processedFiles;
		Loader ld;
		Conversion conv;
		filePaths = ld.filenameDisplay(argc, argv);
		if (filePaths.empty())
		{
			std::cout << "\nNo files matched the given Regex!\n";
			_getch();
			return 0;
		}
		fileNames = ld.onlyfileNamesreturn();
		processedFiles = conv.filetoString(filePaths, fileNames);
		Display disp;
		std::string dirPath = conv.returnDirectoryPath();
		disp.popupBrowser(filePaths, fileNames, dirPath);
		_getch();
	}
	catch (...) 
	{
		std::cout << "Error";
	}
}

