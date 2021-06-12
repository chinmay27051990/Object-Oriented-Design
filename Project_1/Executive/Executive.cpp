///////////////////////////////////////////////////////////////////////////////////
//Executive.cpp-Make together all files called class's objects and functions     //
// ver 1.0																		 //
// Author: Chinmay Ujjwal Vanjare												//
//CSE 687-Object Oriented Design-Spring 2019									//
///////////////////////////////////////////////////////////////////////////////////

#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Converter/Converter.h"
#include "../Process/Process.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../Display/Display.h"
#include <regex>

using namespace CodeUtilities;
using namespace FileSystem;

ProcessCmdLine::Usage customUsage()
{
	std::string usage;
	usage += "\n  Command Line: path [/option]* [/pattern]*";
	usage += "\n    path is relative or absolute path where processing begins";
	usage += "\n    [/option]* are one or more options of the form:";
	usage += "\n      /s - walk directory recursively";
	usage += "\n    [pattern]* are one or more pattern strings of the form:";
	usage += "\n      *.h *.cpp *.cs *.txt or *.*";
	usage += "\n";
	return usage;
}

int main(int argc, char ** argv) {
	ProcessCmdLine pcl(argc, argv);
	pcl.usage(customUsage());

	preface("Command Line: ");
	pcl.showCmdLine();
	if (pcl.parseError())
	{
		pcl.usage();
		std::cout << "\n\n";
		return 1;
	}

	DirExplorerN de(pcl.path());
	for (auto patt : pcl.patterns())
	{
		de.addPattern(patt);
	}

	//std::regex regg(pcl.patterns()[pcl.patterns().size() - 1]);

	if (pcl.hasOption('s'))
	{
		de.recurse();
	}

	de.search();
	de.showStats();
	
	//de.RegexMatch(regg,de.StorageFiles());
	Convert con;
	for (auto qq : de.StorageFilesFiltered()) {
		std::cout <<"\n" << qq;
	}
	con.ConvertFiles(de.StorageFiles());
	
	Display disp;
	disp.display(con.convertedPages());
	std::cout << "\n\n";
	getchar();
	return 0;
}