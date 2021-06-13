///////////////////////////////////////////////////////////////////////////
// Display.cpp   : defines webpage display using browser functions       //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Chinmay Vanjare, Syarcuse University                  //
//				   cuvanjar@syr.edu                                      //
// Source        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include "../Process/Process/Process.h"


Display::Display()
{
}


Display::~Display()
{
}

void Display:: popupBrowser(std::vector<std::string> filePaths, std::vector<std::string> fileNames, std::string dirPath)
{
	Process pro;
	pro.title("test application");
	std::string appPath = "C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe";  
	pro.application(appPath);

	for (size_t i = 0; i < filePaths.size(); i++)
	{
		if (i > 3) 
		{
			std::cout << "\nExiting after showing "<<i<<" files";
			break;
		}
		cmdLine = "/c \"" + dirPath + "/"+fileNames[i] + ".html\"";  
		pro.commandLine(cmdLine);
		std::cout << "\n  starting process: \"" << appPath << "\"";
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
		pro.create();
		static CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
		pro.setCallBackProcessing(callback);

		pro.registerCallback();

		WaitForSingleObject(pro.getProcessHandle(), INFINITE);  
	}
	char ch;
	std::cin.read(&ch, 1);
}

#ifdef TEST_DISPLAY

int main()
{
	std::vector<std::string> testVectorpath;
	testVectorpath.push_back("./test.cpp");
	std::vector<std::string> testVectorname;
	testVectorname.push_back("test.cpp");
	std::string testDirpath = "./";
	Display disp;
	disp.popupBrowser(testVectorpath, testVectorname, testDirpath);
	return 0;
}
#endif 