/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem/FileSystem.h"
#include "../Project2Starter/Executive.h"
#include <chrono>
#include <algorithm>
#include <string>
#include <vector>

namespace MsgPassComm = MsgPassingCommunication;
using namespace PublisherComm;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

Files Server::getFiles(const Repository::SearchPath& path)
{
	return Directory::getFiles(path);
}

Dirs Server::getDirs(const Repository::SearchPath& path)
{
	return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
	std::cout << "\n  " << msg.c_str();
	for (auto item : t)
	{
		std::cout << "\n    " << item.c_str();
	}
}

std::function<Msg(Msg)> echo = [](Msg msg) {
	Msg reply = msg;
	reply.to(msg.from());
	reply.from(msg.to());
	return reply;
};

std::function<Msg(Msg)> getFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getFiles");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files files = Server::getFiles(searchPath);
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;
};

std::function<Msg(Msg)> getDirs = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getDirs");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files dirs = Server::getDirs(searchPath);
		size_t count = 0;
		for (auto item : dirs)
		{
			if (item != ".." && item != ".")
			{
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				reply.attribute("dir" + countStr, item);
			}
		}
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};

std::function<Msg(Msg)> getFile = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("file");
	reply.file(msg.attributes()["name"]);
	return reply;
};

char** command_line_arg(Msg msg) {
	std::string no = msg.attributes()["tot_cmd"];
	int number;
	std::istringstream iss(no);
	iss >> number;
	char** argv = new char*[number];

	std::string s;
	for (int i = 0; i <= number; i++) {
		std::string m = "cmdline" + std::to_string(i);
		s = msg.attributes()[m];
		char *name = new char[s.size() + 1];
		s.copy(name, s.size() + 1);
		name[s.size()] = '\0';
		argv[i] = name;
	}

	return argv;
}

std::function<Msg(Msg)> convert = [](Msg msg) {

	std::string no = msg.attributes()["tot_cmd"];
	int number;
	std::istringstream iss(no);
	iss >> number;
	CodePublisher pub;
	std::vector<std::string> files_list = pub.publish(number, command_line_arg(msg));

	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("files_list");
	size_t count = 0;
	for (auto item : files_list)
	{
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("convfile" + countStr, item);
	}
	return reply;
};

int main()
{
	std::cout << "\n  Testing Server Prototype";
	std::cout << "\n ==========================";
	std::cout << "\n";

	//StaticLogger<1>::attach(&std::cout);
	//StaticLogger<1>::start();

	Server server(serverEndPoint, "ServerPrototype");
	server.start();

	std::cout << "\n ";
	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("getFile", getFile);
	server.addMsgProc("convert", convert);
	server.addMsgProc("serverQuit", echo);
	server.processMessages();


	server.stop();
	return 0;
}

