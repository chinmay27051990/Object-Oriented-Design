#pragma once
/////////////////////////////////////////////////////////////////////
// PulisherWrapper.h -  object factory                             //
// ver 1.0                                                         //
// Chinmay Vanjare, CSE687: Object Oriented Design, Spring 2019    //
/////////////////////////////////////////////////////////////////////
/*
*  Provides export and import declarations for Comm
*
*  Required Files:
* -----------------
*  ipublisher.h
*
*  Maintenance History:
* ----------------------
*  ver 1.0 : 17 Apr 2019
*  - first release
*/
#include <string>
#include "../Project2Starter/ipublisher.h"
using namespace PublisherComm;
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

namespace PublisherComm
{
  extern "C" {
    struct PublisherCommFactory {
      static DLL_DECL IPublisherComm* createPublisher();
    };
  }
}