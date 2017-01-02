///////////////////////////////////////////////////////////////////////
// FileMgr.cpp - Implement the function in the FileMgr.h file        //
// ver 1.1                                                           //
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
* This module contains the following functions:

*  This module can add the user input patterns into the vector.
*  Then the file catalogue will depend on different patterns to 
*  search the single path or the entire directories. Finally, the
*  result will be saved into the DataStore.
*
* It defines following functions:
*   addPattern - according to the user input to add the patterns into the patterns vector
*   search - call the find function to do the specific finding.
*   find - according to the path and the patterns to search and save the result to the map and list.
*   setRecursive - if call the this function, the enire directory tree will be searched
*
* Required Files:
*  DataStore.h FileMgr.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 31 JAN 2015
* - first release
*
*/

#include "FileMgr.h"
#include "../DataStore/DataStore.h"
#include <iostream>

//----< add patterns to the vector >---------------------------
void FileMgr::addPattern(const std::string& patt)
{
		if (patterns_.size() == 1 && patterns_[0] == "*.*")
		patterns_.pop_back();
		patterns_.push_back(patt); //add patterns into the vector
}

//----< call the find function >---------------------------------
void FileMgr::search(sPtr& root_,int a,std::string text_)
{
	if (a == 1)
		find(path_, root_);
	if (a == 2)
		find2XML(path_, root_,text_);
}

//----< find and call save function >----------------------------------------
void FileMgr::find(const std::string& path,sPtr& root)
{
	for (auto patt : patterns_)
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);

		for (auto f : files)
		{
			store_.save(f, path); //call the save function to save the filename and path
			root->addChild(XmlProcessing::makeTextElement(f));
		}
		if (recursive) //if recursive flag is true then conduct the recursive search
		{
			std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);

			// remove "." and ".." from dirs

			auto iter = std::find(std::begin(dirs), std::end(dirs), ".");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}
			iter = std::find(std::begin(dirs), std::end(dirs), "..");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}
			for (auto d : dirs)
			{
				sPtr child = XmlProcessing::makeTaggedElement(d);
				root->addChild(child);
				std::string RecusivePath = path + d + "/";
				find(RecusivePath,child);
			}
		}
	}
}


void FileMgr::find2XML(const std::string& path, sPtr& root, Text text_)
{
	for (auto patt : patterns_)
	{
		std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);

		for (auto f : files)
		{
			std::string FileSpec = path + f;
			FileSystem::File findall(FileSpec); //call the findall funciton
			findall.open(FileSystem::File::in); //open the file
			if (findall.isGood())
			{ //ignore the case
				std::string all = FileSystem::Path::toLower(findall.readAll()); //change all file content to lower case
				std::string textcopy = FileSystem::Path::toLower(text_); //change all searching content to lower case
				int optfind = all.find(textcopy); //find the matching text
				if (optfind != -1) //output the matching file
				{
					//filestring_ += fs.first + " file contains the key text in path: " + *y + "\n";
					//std::cout << fs.first << " file contains the key text " << text_;
					//std::cout << " in path: " << *y << std::endl;
					//std::cout << std::endl;
					//flag++;
					store_.save(f, path); //call the save function to save the filename and path
					root->addChild(XmlProcessing::makeTextElement(f));
				}
			}
			findall.close();
			//store_.save(f, path); //call the save function to save the filename and path
			//root->addChild(XmlProcessing::makeTextElement(f));
		}
		if (recursive) //if recursive flag is true then conduct the recursive search
		{
			std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);

			// remove "." and ".." from dirs

			auto iter = std::find(std::begin(dirs), std::end(dirs), ".");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}
			iter = std::find(std::begin(dirs), std::end(dirs), "..");
			if (iter != std::end(dirs))
			{
				dirs.erase(iter);
			}
			for (auto d : dirs)
			{
				sPtr child = XmlProcessing::makeTaggedElement(d);
				root->addChild(child);
				std::string RecusivePath = path + d + "/";
				find2XML(RecusivePath, child,text_);
			}
		}
	}
}
//----< change recursive flag >----------------------------------------
void FileMgr::setRecursive()
{
	recursive = true;
}

//---- < test stub > ---------------------------------
#ifdef TEST_FILEMGR
void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}
int main()
{
  title("Demonstrate File Manager");
  std::cout << "\n" << std::endl;

  DataStore ds;
  FileMgr fm("../", ds);
  fm.addPattern("*.h");
  fm.addPattern("*.cpp");
  //fm.addPattern("*.*");
  fm.setRecursive();
  fm.root_ = XmlProcessing::makeTaggedElement("ServerRepository/");
  fm.search(fm.root_,1,"");

  for (auto fs : ds)
  {
	  std::cout << "The file name is:" << fs.first << std::endl;
	  for (auto& y : fs.second)
	  {
		  std::cout << "The file path is:" << *y << std::endl;
	  }
	  std::cout << std::endl;
  }
  return 0;
}
#endif