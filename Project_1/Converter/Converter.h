#pragma once
///////////////////////////////////////////////////////////////////////////////////
// converter.h - Change <> characters, Insert into html file                //
// ver 1.0																		 //
// Author: Chinmay Ujjwal Vanjare
//CSE 687-Object Oriented Design-Spring 2019
///////////////////////////////////////////////////////////////////////////////////
/*
*  Package Responsibilities:
* ---------------------------
*for each matching file, convert the file text, as described in the purpose section, to a valid html file, and save to a folder in your project tree, named "convertedPages". *
*
*  Required Files:
* -----------------
*
*  FileSystem.h
*
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 17 Feb 2019
*  - first release
*
*/
#include <iostream>
#include "../FileSystem/FileSystem.h"

class Convert
{
public:

	void ConvertHtml(std::string source,std::string destination);
	void ConvertFiles(const std::vector<std::string>& files);
	std::vector<std::string> convertedPages();
	std::vector<std::string> newContend(std::string source, std::vector<std::string> newcontent);
	void destination(const std::string& dest );


private:
	std::vector<std::string> convertedPages_;
	std::string destination_;
};


inline void Convert::ConvertHtml(std::string source, std::string destination = "../convertedPages")
{
	std::string fileToconvert = FileSystem::Path::getName(source);
	//std::cout << "\n\n" << FileSystem::Path::getFullFileSpec(source);
	std::cout << fileToconvert << std::endl;
	std::vector<std::string> newcontent;
	std::ofstream WebPage(destination + "/" + fileToconvert + ".html");

		WebPage << "<!Doctype html>";
		WebPage << "<html>";
		WebPage << "<head>";
		WebPage << "</head>";
		WebPage << "<body>";
		WebPage << "<pre>";

		std::vector<std::string> htmlcontent = newContend(source, newcontent);
		for (auto content : htmlcontent) {
			WebPage << content << "\n";
		}

		WebPage << "</pre>";
		WebPage << "</body>";
		WebPage << "</html>";
		WebPage.close();
		convertedPages_.push_back(destination + "/" + fileToconvert + ".html");
		newcontent.clear();
}

//--convert all files in specified folder--
inline void Convert::ConvertFiles(const std::vector<std::string>& files) {
	for (auto filepath : files) {
		ConvertHtml(FileSystem::Path::getFullFileSpec(filepath));
	}
}
//-- find files in specified destination--
inline void Convert::destination(const std::string& dest) {
	destination_ = dest;
	if (!FileSystem::Directory::exists(destination_))
		FileSystem::Directory::create(destination_);
}
//--all html markup characters are replaced with their exscape sequence equivalents, e.g., < is replaced by &lt, etc. 
inline std::vector<std::string> Convert::convertedPages() {
	return convertedPages_;
}

inline std::vector<std::string> Convert::newContend(std::string source, std::vector<std::string> newcontent) {
	std::ifstream changeContent;
	changeContent.open(source);
	if (changeContent.fail()) {
		std::cout<<"\n\n File failed!!!!";
		std::cout << "\n " << source;
		getchar();
	}
	for (std::string lines; std::getline(changeContent, lines);) {
		for (int i = 0; i < lines.length(); i++) {
			if (lines[i] == '<') {
				lines.replace(lines.begin() + i, lines.begin() + i + 1, "&lt");
			}
			if (lines[i] == '>') {
				lines.replace(lines.begin() + i, lines.begin() + i + 1, "&gt");
			}
		}
		for (int i = 0; i < lines.length(); i++)
		{
			if (lines[i] == '/')
			{
				if (lines[i + 1] == '*')
				{
					lines.insert(i, "<div class = \"comments\" style = \"display: inline-block\"");
				}
				else if (lines[i + 1] == '/')
				{
					lines.insert(i, "<div class = \"comments\" style = \"display: inline-block\"");
					lines = lines + "</div>";
				}
			}
			else if (lines[i] == '*')
			{
				if (lines[i+1] == '/')
				{
					lines.insert(i + 1, "</div>");
				}
			}
		}

		newcontent.push_back(lines);
	}
	changeContent.close();
	return newcontent;

}




 
