#pragma once

///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syarcuse University                  //
//				   cuvanjar@syr.edu                                      //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages.
*
*  Required Files:
* =======================
*  Converter.h Converter.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 11 Feb 2019
*  - first release
*  - support for handling single line comments
*  - support for handling multi- line comments
*  - support for handling scopes (function and class begin and end)
*  - uses information extracted from FileProcessor and Dependency
*/

#include<fstream>
#include<vector>
#include<string>
#include<streambuf>
#include<iostream>
#include "../FileSystem/FileSystem.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Dependencies/Dependencies.h"

using namespace FileSystem;
using namespace Lexer;
using namespace Utilities;
using namespace CodeAnalysis;
using namespace std;

// ----------------< Class Converter >--------------------------
class Conversion
{
private:
	std::vector <std::string> stringData;
	std::vector <std::string> files;
	std::vector<std::string> stringReplace;
	std::string htmlStart = "<html> <head> <script> " + hideFunctionsJS + hideClassesJS + hideCommentsJS + " </script> </head> <body> " + buttonsBody + " <pre>";
	std::string hideFunctionsJS = "function hideFunctions() { var button = document.getElementById(\"hideFunction\"); var x = document.getElementsByClassName(\"function\"); for (var i=0;i<x.length;i++) { if (x[i].style.display === \"none\") { button.textContent = \"Hide Functions\";x[i].style.display = \"block\"; } else { button.textContent = \"Show Functions\" ;x[i].style.display = \"none\"; } } }";
	std::string hideClassesJS = "function hideClasses() { var button = document.getElementById(\"hideClass\"); var x = document.getElementsByClassName(\"class\"); for (var i=0;i<x.length;i++) { if (x[i].style.display === \"none\") { button.textContent = \"Hide Classes\"; x[i].style.display = \"block\"; } else { button.textContent = \"Show Classes\"; x[i].style.display = \"none\"; } } }";
	std::string hideCommentsJS = "function hideComments() { var button = document.getElementById(\"hideComments\"); var x = document.getElementsByClassName(\"comment\"); for (var i=0;i<x.length;i++) { if (x[i].style.display === \"none\") { button.textContent = \"Hide Comments\"; x[i].style.display = \"block\"; } else { button.textContent = \"Show Comments\" ;x[i].style.display = \"none\"; } } }";
	std::string buttonsBody = "<button id=\"hideFunction\" onclick=\"hideFunctions()\">Hide Functions</button> <button id=\"hideClass\" onclick=\"hideClasses()\">Hide Classes</button> <button id=\"hideComments\" onclick=\"hideComments()\">Hide Comments</button> ";
	std::string htmlEnd = "</pre> </body> </html>";
	std::string pathdir;
	std::string filespecs;
	std::vector<scopenode> nodedata;
	std::vector<std::vector<scopenode>> vectornode;
	std::string line;
	std::string alltheLines;
	std::vector<std::string> data;
	std::vector<std::vector<std::string>> filesData;
	std::vector<std::string> dependency;
	std::string dependencystr;
public:
	Conversion();
	~Conversion();

// ----------------< Parser in Conversion >--------------------------
	inline std::vector<scopenode> parserinConversion(std::string filePaths)
	{
		filespecs = FileSystem::Path::getFullFileSpec(filePaths);
		std::string msg = "Processing file" + filespecs;
		//Util::title(msg);
		ConfigParseForCodeAnal configure;
		Parser* pParser = configure.Build();
		std::string name;
		try
		{
			if (pParser)
			{
				name = FileSystem::Path::getName(filePaths);
				if (!configure.Attach(filespecs))
				{
					std::cout << "\n  could not open file " << name << std::endl;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
			}
			Repository* pRepo = Repository::getInstance();
			pRepo->package() = name;
			while (pParser->next())
			{
				pParser->parse();
			}
			std::cout << "\n";
			ASTNode* pGlobalScope = pRepo->getGlobalScope();
			complexityEval(pGlobalScope);
			std::vector<scopenode> nodedata;
			nodedata.clear();
			nodedata = TreeWalk(pGlobalScope,false,true);
			return nodedata;
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
		std::cout << "\n";
	}

	inline std::vector<std::string> filetoString(std::vector<std::string> filePaths, std::vector<std::string> fileNames)
	{
		files = fileNames;
		for (size_t i = 0; i < filePaths.size(); i++)
		{
				data.clear();
				std::ifstream myFile(filePaths[i]);
				nodedata = parserinConversion(filePaths[i]);
				vectornode.push_back(nodedata);
				while (std::getline(myFile, line))
				{
					std::istringstream inputStream(line);
					data.push_back(line);
				}
				filesData.push_back(data);

				std::string fileContent((std::istreambuf_iterator<char>(myFile)), std::istreambuf_iterator<char>());
				stringData.push_back(fileContent);
		}
		markupConverter();
		addDivTags();
		appendHTMLext(fileNames);
		return stringReplace;
	}

	inline void addDivTags()
	{
		for (size_t j = 0; j < vectornode.size(); j++)
		{
			nodedata = vectornode[j];
			for (size_t i = 0; i < nodedata.size(); i++)
			{
				if (nodedata[i].type == "function" || nodedata[i].type == "class")
				{					
						data = filesData[j];
						data[nodedata[i].startlineCount - 1] = "<div class = \"" + nodedata[i].type + "\"" + "style = \"display : inline-block; \">" + data[nodedata[i].startlineCount - 1];
						if (nodedata[i].type == "class")
							data[nodedata[i].endlineCount - 2] = data[nodedata[i].endlineCount - 2] + "</div>";
						else
							data[nodedata[i].endlineCount - 1] = data[nodedata[i].endlineCount - 1] + "</div>";
						filesData[j] = data;					
				}
			}
			vectornode[j] = nodedata;
			for (auto const& s : data)
			{
				alltheLines += s + "\n";
			}
		}
		for (auto const& s : data)
		{
			alltheLines += s + "\n";
		}
		commentdiv();
		dependencyhref();
	}
	
// ----------------< Div for Comments >--------------------------
	inline void commentdiv()
	{
		for (size_t i = 0; i < filesData.size(); i++) 
		{
			data = filesData[i];
			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i].find("//") == 0)
				{
					data[i] = "<div class = \"comment\" style = \"display : block; \">" + data[i] + "</div>";
				}
				else if (data[i].find("//") > 0)
				{
					int position = data[i].find("//");
					if (position > 0)
					{
						data[i].insert(position, "<div class = \"comment\" style = \"display : block; \">");
						data[i] = data[i] + "</div>";
					}
				}
			}
			filesData[i] = data;
		}

	}

// ----------------< Dependency Reference >--------------------------
	inline void dependencyhref()
	{
		for (size_t j = 0; j < filesData.size(); j++)
		{
			data = filesData[j];
			dependency = Dependencies::includehref(files, filesData[j]);
			for (size_t i = 0; i < dependency.size(); i++)
			{
				dependency[i] = "<a href = \"" + FileSystem::Path::getFullFileSpec(dependency[i]) + ".html\">" + dependency[i] + "</a>";
			}
			dependencystr = "";
			for (auto const& content : dependency) {
				dependencystr += content + "\n";
			}
			filesData[j] = data;
		}
	}

// ----------------< Adding Div Tags >--------------------------
	inline std::string divtagsformultilineComments(std::string stringData)
	{
		alltheLines = stringData;
		for (int i = 0; i < alltheLines.length(); i++)
		{
			if (alltheLines[i] == '/')
			{
				if (alltheLines[i + 1] == '*')
				{
					std::string openingDiv = "<div class = \"comment\" style = \"display : block; \">";
					alltheLines.insert(i, openingDiv);
					i += openingDiv.length() + 1;
				}
			}
			else if (alltheLines[i] == '*')
			{
				if (alltheLines[i + 1] == '/')
				{
					alltheLines.insert(i + 2, "</div>");
					i += 7;
				}
			}
		}
		return alltheLines;
	}

// ----------------< Markup Character Conversion >--------------------------
	inline void markupConverter()
	{
		for (size_t i = 0; i < filesData.size(); i++)
		{
			data = filesData[i];
			const std::string replaceLT = "&lt";
			const std::string replaceGT = "&gt";
			for (size_t j = 0; j < data.size(); j++)
			{
				int position;
				position = data[j].find('<');
				if (position >= 0)
				{
					data[j].replace(position, 1, replaceLT);
				}
				position = data[j].find('>');
				if (position >= 0)
				{
					data[j].replace(position, 1, replaceGT);
				}
			}
			filesData[i] = data;
		}
	}

// ----------------< HTML Conversion >--------------------------
	inline void filetoHTML()
	{
		std::string htmlStart1 = "<html> <head> <script> " + hideFunctionsJS + hideClassesJS + hideCommentsJS + " </script> </head> <body> " + buttonsBody + " <pre>";
		std::string stringData;
		for (size_t i = 0; i < filesData.size(); i++)
		{
			data = filesData[i];
			for (auto const& content : data)
			{
				stringData += content + "\n";
			}
			stringData = "";
			filesData[i] = data;
		}
		alltheLines = divtagsformultilineComments(stringData);
		stringData = htmlStart1 + alltheLines + htmlEnd;
	}

	inline void appendHTMLext(std::vector<std::string> fileNames)
	{
		FileSystem::Directory::create("../../../convertedPages");
		FileSystem::Directory::setCurrentDirectory("../../../convertedPages");
		pathdir = FileSystem::Directory::getCurrentDirectory();
		FileSystem::Directory::setCurrentDirectory("../../../");
		for (size_t i = 0; i < fileNames.size(); i++)
		{
			std::ofstream myFile(pathdir + "/"+ fileNames[i] + ".html");
			std::string htmlStart1 = "<html> <head> <script> " + hideFunctionsJS + hideClassesJS + hideCommentsJS + " </script> </head> <body> " + buttonsBody + dependencystr + " <pre>";
			std::string stringData;
			std::string fileDataContent1;
			fileDataContent1 = "";
			for (auto const& content : filesData[i])
			{
					fileDataContent1 += content + "\n";
			}
			alltheLines = divtagsformultilineComments(fileDataContent1);
			stringData = htmlStart1 + alltheLines + htmlEnd;
			myFile << stringData;
			stringReplace.push_back(stringData);
		}
	}

	inline std::string returnDirectoryPath() 
	{
		return pathdir;
	}
};