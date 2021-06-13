#pragma once
/////////////////////////////////////////////////////////////////////
// Executive.h - Entry Point of the Project.                       //
// ver 1.0                                                         //
// Chinmay Ujjwal Vanjare, CSE687 - Object Oriented Design, Spring 2019   //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * Nothing here for now
 * 
 * Maintenance History : 
 * ---------------------
 * ver 1.0: April 7,2019
 * - first release.
 *
 */
#include <vector>
#include <string>

namespace PublisherComm
{
	class CodePublisher
	{
	public:
		CodePublisher();
		std::vector<std::string> publish(int argc, char** argv);
		std::vector<std::string> getconfiles();
		~CodePublisher();


	private:
		std::vector<std::string> convertedfiles;
	};
}