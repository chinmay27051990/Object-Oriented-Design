///////////////////////////////////////////////////////////////////////////////////
// Display.cpp-Display Html files in default browser             //
// ver 1.0																		 //
// Author: Chinmay Ujjwal Vanjare
//CSE 687-Object Oriented Design-Spring 2019
///////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "Display.h"
#ifdef TEST_DISPLAY
int main()
{
	Display d;

	std::vector<std::string> files;
	files.push_back("../convertedPages/FileSystem.cpp.html");

	//d.files(files);
	d.display(files);
}
#endif