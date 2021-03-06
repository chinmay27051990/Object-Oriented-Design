////////////////////////////////////////////////////////////////////
// PulisherWrapper.cpp -  object factory                          //
// ver 1.0                                                        //
// Chinmay Vanjare, CSE687: Object Oriented Design, Spring 2019   //
///////////////////////////////////////////////////////////////////

#define IN_DLL
#include <iostream>
#include "PublisherWrapper.h"
#include "../Project2Starter/Executive.h"

using namespace PublisherComm;

DLL_DECL IPublisherComm* PublisherCommFactory::createPublisher()
{
  std::cout << "\n  using CommFactory to invoke IComm::create\n\n";
  return IPublisherComm::createPublisher();
}


