#pragma once
/////////////////////////////////////////////////////////////////////
// IComm.h - interface for message-passing communication facility  //
// ver 1.0                                                         //
// Chinmay Ujjwal Vanjare, CSE687-OnLine Object Oriented Design, Fall 2017    //
/////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>


namespace PublisherComm
{
	class IPublisherComm
	{
	public:
		static IPublisherComm* createPublisher();
		virtual void publish(int argc, char** argv) = 0;
		virtual std::vector<std::string> getconfiles() = 0;
		
	};
}
