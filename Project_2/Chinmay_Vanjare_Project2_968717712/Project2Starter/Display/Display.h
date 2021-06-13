#pragma once
///////////////////////////////////////////////////////////////////////////
// Display.h     : defines webpage display using browser functions       //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syracuse University                  // 
//                 cuvanjar@syr.edu                                      //
// Source        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Display class which accepts a list of files as a
*  vector<string> and uses the default internet browser to display them
*  one by one. Please note that the functionality has limiations:
*   1) Opera/MS Edge: will pop-up all tabs instantly.
*   2) Chrome/Firefox: will pop-up windows separately only if no
*      already existing Chrome/Firefox window is opened (all must be
*      closed before running this).
*
*  Required Files:
* =======================
*  Display.h Display.cpp Process.h Process.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 8 Apr 2019
*  - first release
*/

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>

class Display
{
private:
	std::string cmdLine;
public:
	Display();
	~Display();

	void popupBrowser(std::vector<std::string> filePaths, std::vector<std::string> fileNames, std::string dirPath);
};