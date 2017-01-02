///////////////////////////////////////////////////////////////////////
// Executive.cpp - Implement the File Cataloger program              //
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
*
*  This module will call the function in the FileCataloger package to 
*  implement the File Cataloger program.
*
* It defines following functions:
*   main - implement the File Cataloger program
*
* Required Files:
*  DataStore.cpp FileSystem.cpp FileMgr.cpp Display.cpp FileCataloger.cpp FileCataloger.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 8 Feb 2015
* - first release
*
*/
#include "../FileCataloger/FileCataloger.h"
#include <iostream>


//---- < test stub > ---------------------------------
#ifdef TEST_EXECUTIVE
void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}

int main(int argc, char *argv[])
{
	title("Welcome to use Ziyi Zhao's File Catalogue");
	std::cout << "\n" << std::endl;
	Cataloger myCataloger;
	myCataloger.CommandParser(argv);
	myCataloger.start();

	return 0;
}
#endif