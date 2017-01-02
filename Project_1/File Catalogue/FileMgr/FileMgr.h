#ifndef FILEMGR_H
#define FILEMGR_H
///////////////////////////////////////////////////////////////////////
// FileMgr.h - Define the public function interface and              //
//             the private memeber of the FileMgr.cpp                //
// ver 1.0                                                           //
// Language:    C++                                                  //
// Platform:    Macbook Pro 13, Win8.1, SP1                          //
// Application: File Catalogue, Project #1, Spring 2015              //
// Author:      Ziyi Zhao, Computer Engineering, Syracuse University //
//              (315) 708-5889, zzhao37@syr.edu                      //
// Source:      Jim Fawcett, CST 2-187, Syracuse University          //
//              (315) 443-3948, jfawcett@twcny.rr.com                //
///////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* --------------------------------------------------------------------*
*
* It defines following functions:
*   addPattern - according to the user input to add the patterns into the patterns vector
*   search - call the find function to do the specific finding.
*   find - according to the path and the patterns to search and save the result to the map and list.
*   setRecursive - if call the this function, the enire directory tree will be searched
*
* Required Files:
*  DataStore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 31 JAN 2015
* - first release
*
*/

#include "FileSystem.h"
#include "../DataStore/DataStore.h"

class FileMgr
{
public:
  using iterator = DataStore::iterator;
  using patterns = std::vector<std::string>;

  FileMgr(const std::string& path, DataStore& ds) : path_(path), store_(ds), recursive(false)
  {
    patterns_.push_back("*.*");
  }
  void addPattern(const std::string& patt);
  void search();
  void find(const std::string& path);
  void setRecursive(); //function to do the recursive search
private:
  std::string path_;
  DataStore& store_;
  patterns patterns_;
  bool recursive;
};

#endif
