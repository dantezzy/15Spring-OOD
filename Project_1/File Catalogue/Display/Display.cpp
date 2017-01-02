///////////////////////////////////////////////////////////////////////
// Display.cpp - Implement the function in the Display.h file        //
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
* This module contains the following functions:

*  This module will output different results to user. It can show the summary
*  duplicate files, directories the file which contains some specific texts.
*
* It defines following functions:
*   ShowSummery - show the summary of how many files in how many directories.
*   ShowDuplicate - show a list of duplicate file names along with their paths.
*   ShwoDirectory - show the entire directiores.
*   ShowMatchFile - show all the files stored in the catalog that contain
*                   the search text.
*
* Required Files:
*  DataStore.h FileSystem.h FileMgr.h Display.h 
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Feb 2015
* - first release
*
*/
#include "Display.h"
#include "../DataStore/DataStore.h"
#include "../FileMgr/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include <iostream>

//----< show the summary of the file catalogue >------------------------------
void Display::ShowSummary(const DataStore& ds)
{
	std::cout << "************************************************" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "**                File Summary                **" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << std::endl;
	
	int store = ds.getstoresize(); //get the size of the filename
	int path = ds.getpathsize(); //get the size of the paths

	std::cout << store << " Files foud in " << path << " directories.\n" << std::endl;
}

//----< show the duplicate file >----------------------------------------
void Display::ShowDuplicate(const DataStore& ds)
{
	int flag = 0;
	std::cout << "************************************************" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "**         Duplicate File Demonstrate         **" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << std::endl;
	for (auto& fs : ds)
	{
		if (fs.second.size()>1) //find the filename which has multiple paths
		{
			std::cout << "The file name is:" << fs.first << std::endl;
			flag++;
			for (auto& y : fs.second) //list all paths
			{
				std::cout << "The file path is:" << *y << std::endl;
				
			}
			std::cout << std::endl;
		}
	}
	if (flag == 0) //if there is no duplicate file
	{
		std::cout << "There is no duplicate file." << std::endl<<std::endl;
	}
	//std::cout << "There are " << counta << " files in " << countb << " directories." << std::endl << std::endl;
}

//----< show the whole directory of the file catalogue >----------------------------
void Display::ShowDirectory(const DataStore& ds)
{
	std::cout << "************************************************" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "**  Search Entire Directory Tree Demonstrate  **" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << std::endl;
	for (auto& fs : ds) //show all items in the file catalogue
	{ 
		std::cout << "The file name is:" << fs.first << std::endl;
		for (auto& y : fs.second)
		{
			std::cout << "The file path is:" << *y << std::endl;
		}
		std::cout << std::endl;
	}
}

//----< show the file which contains the searching text >-----------------------
void Display::ShowMatchFile(const DataStore& ds, Text text_)
{
	int flag = 0;
	std::cout << "************************************************" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "**                 Match File                 **" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "************************************************" << std::endl;
	std::cout << std::endl;
	if (text_.size() != 0)
	{
		for (auto& fs : ds)
		{
			for (auto& y : fs.second)
			{
				std::string FileSpec = *y + fs.first; //get the filespec to do the search
				FileSystem::File findall(FileSpec); //call the findall funciton
				findall.open(FileSystem::File::in); //open the file
				if (findall.isGood())
				{ //ignore the case
					std::string all = FileSystem::Path::toLower(findall.readAll()); //change all file content to lower case
					std::string textcopy = FileSystem::Path::toLower(text_); //change all searching content to lower case
					int optfind = all.find(textcopy); //find the matching text
					if (optfind != -1) //output the matching file
					{
						std::cout << fs.first << " file contains the key text " << text_;
						std::cout << " in path: " << *y << std::endl;
						std::cout << std::endl;
						flag++;
					}
				}
				findall.close();
			}
		}
		if (flag == 0) //if there is no matching file
		{
			std::cout << "There is no file contains the searching text." << std::endl;
		}
	}
	else
	{
		std::cout << "There is no searching text." << std::endl<<std::endl;
	}
}


//---- < test stub > ---------------------------------
#ifdef TEST_DISPLAY

void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}

int main(int argc, char *argv[])
{
	title("Demonstrate Display");
	std::cout << "\n" << std::endl;

	Display myDisplay;
	DataStore myDatastore;
	FileMgr myFilemgr("../",myDatastore);
	myFilemgr.addPattern("*.h");
	myFilemgr.addPattern("*.*");
	myFilemgr.setRecursive();
	myFilemgr.search();
	myDisplay.ShowSummary(myDatastore);
	myDisplay.ShowDuplicate(myDatastore);
	//myDisplay.ShowDirectory(myDatastore);
	myDisplay.ShowMatchFile(myDatastore,"zzy");
	return 0;
}
#endif