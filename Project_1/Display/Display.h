#pragma once
///////////////////////////////////////////////////////////////////////////////////
// Display.h-Display Html files in default browser              //
// ver 1.0																		 //
// Author: Chinmay Ujjwal Vanjare
//CSE 687-Object Oriented Design-Spring 2019
///////////////////////////////////////////////////////////////////////////////////

/*
*  Package Responsibilities:
* ---------------------------
*  display, in a child process using the default browser, each converted file, waiting for the child to exit before displaying the next file
*
*
*  Required Files:
* -----------------
*  
*  Process.h
*
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 17 Feb 2019
*  - first release
* 
*/


#include <iostream>
#include <vector>
#include "../Process/Process.h"
#include "../FileSystem/FileSystem.h"

class Display {
public:

	std::vector<std::string> files();
	void files(std::vector<std::string>);
	void display(std::vector<std::string> disp);
private:
	std::vector<std::string> displayfiles_;

};
//--reach private variable--
inline std::vector<std::string> Display::files() {
	return displayfiles_;
}
//--fill the files in folder--
inline 	void Display::files(std::vector<std::string> files) {
	displayfiles_ = files;
}
//--use default browser to show source code--
inline void Display::display(std::vector<std::string> disp) {
	
	
	Process p;
	//p.title("test application");
	std::string appPath = "C:\\Windows\\System32\\cmd.exe";  // path to application to start
	p.application(appPath);
	for (auto file : disp){
	std::string cmdLine = "/C " + FileSystem::Path::getFullFileSpec(file); 
	
	p.commandLine(cmdLine);
		std::cout << "\n  starting process: \"" << appPath << "\"";
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
		p.create();
		CBP callback = []() { };
		p.setCallBackProcessing(callback);
		p.registerCallback();

		WaitForSingleObject(p.getProcessHandle(), INFINITE);  // wait for created process to terminate
	}

	std::cout << "\n  after OnExit";
	std::cout.flush();
	char ch;
	std::cin.read(&ch, 1);
	
	
}