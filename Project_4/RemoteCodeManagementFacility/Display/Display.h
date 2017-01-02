#ifndef DISPLAY_H
#define DISPLAY_H
///////////////////////////////////////////////////////////////////////
// Display.h - Define the public function interface and              //
//             the private memeber of the Display.cpp                //
// ver 1.0                                                           //
// Language:    C++                                                  //
// Platform:    Macbook Pro 13, Win8.1, SP1                          //
// Application: File Catalogue, Project #1, Spring 2015              //
// Author:      Ziyi Zhao, Computer Engineering, Syracuse University //
//              (315) 708-5889, zzhao37@syr.edu                      //
///////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* --------------------------------------------------------------------*
*
* It defines following functions:
*   ShowSummery - show the summary of how many files in how many directories.
*   ShowDuplicate - show a list of duplicate file names along with their paths.
*   ShwoDirectory - show the entire directiores.
*   ShowMatchFile - show all the files stored in the catalog that contain 
*                   the search text.
* Public Interface:
* ===============
* using Text = std::string;
* using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
* void ShowSummary(const DataStore& ds);
* void ShowDuplicate(const DataStore& ds);
* void ShowDirectory(const DataStore& ds);
* void ShowMatchFile(const DataStore& ds, Text text_);
* Text GetFileString();
*
* Required Files:
*  DataStore.h FileSystem.h FileMgr.h XmlElement.h
*  DataStore.cpp FileSystem.cpp FileMgr.cpp XmlElement.cpp

* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2015
* - first release
*
*/
#include "../DataStore/DataStore.h"
#include "../FileMgr/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../XmlElement/XmlElement.h"

class Display
{
public:
	using Text = std::string;
	using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
	void ShowSummary(const DataStore& ds);
	void ShowDuplicate(const DataStore& ds);
	void ShowDirectory(const DataStore& ds);
	void ShowMatchFile(const DataStore& ds, Text text_);
	Text GetFileString();
private:
	Text text_;
	Text filestring_;
};

#endif