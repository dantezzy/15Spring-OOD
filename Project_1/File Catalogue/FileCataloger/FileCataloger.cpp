///////////////////////////////////////////////////////////////////////
// FileCataloger.cpp - Implement the function in the                 //
//                     FileCataloger.h file                          //
// ver 1.2                                                           //
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

*  This module will parse the user input first and then add the path and
*  all patterns. Then it will call the start function to do the next step
*  according to the different options. /s option will search the entire
*  dirtectories. /d option will show all duplicate files. And the /f option
*  will search the spcific text.
*
* It defines following functions:
*   CommandParser - parse the user input into the path patterns and options
*   start - according to the path, different patterns and different options
*           to do different operations.
*   ContinueSearch - let the user can do the continue search to find the file
*                    which contains the specific text
*   InputText - let the user input the searching text
*   InputPattern - let the user input hte searchin patterns
*
* Required Files:
*  DataStore.h FileSystem.h FileMgr.h Display.h Executive.h
*
* Maintenance History:
* --------------------
* ver 1.2 : 8 Feb 2015
* - complete the function of continuesearch()
* - divide continuesearch() into small parts
* ver 1.1 : 3 Feb 2015
* - add continuesearch()
* ver 1.0 : 1 Feb 2015
* - first release
*
*/
#include "FileCataloger.h"
#include "../FileMgr/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../DataStore/DataStore.h"
#include "../Display/Display.h"
#include <iostream>

//----< parser user input >----------------------------------------
void Cataloger::CommandParser(char *argv[])
{
	path_ = argv[1];
	for (int i = 2; argv[i] != NULL; i++)
	{
		std::string temp = argv[i];
		int pospatt = temp.find(".");
		int posopt = temp.find("/");
		int optf = temp.find("/f");
		if (pospatt != -1)
		{
			patterns_.push_back(temp); //push the pattern into patterns
		}
		if (posopt != -1)
		{
			options_.push_back(temp);
			if (optf != -1)
			{
				text_ = temp.substr(optf + 2, temp.length()); //get the searching text
			}
		}
	}
}

//----< input continue searching text >------------------------------
void Cataloger::InputText()
{
	char str[3000];
	std::cout << "Please enter the text need to be searched(press ENTER to exist):";
	std::cin.getline(str, 3000); //get the user input into one string
	text_ = str;
	if (text_.size () == 0)
		exit(0);
	std::cout << std::endl;
}

//----< input continue searching patterns >----------------------------
void Cataloger::InputPattern()
{
	char str[3000];
	patterns_.clear(); //clear the exist patterns
	std::string a;
	std::string b;
	std::cout << "Please enter the file patterns need to be searched:";
	std::cin.getline(str, 3000);
	a = str;
	int posspace = a.find_last_of(" ");
	if (posspace!=a.length()-1)
	{
		a.append(" ");
	}
	while (a.length()>0)
	{
		int posb = a.find(" ");
		if (posb!=-1)
		{
			b = a.substr(0, posb);
			a = a.substr(posb + 1, a.length());
			int posspacetwo = b.find(" ");
			if (posspacetwo == 0)
				b = b.substr(1, b.length());
			if (posspacetwo != b.length())
				b = b.substr(0, b.length() - 1);
				int posstar = b.find("*");
				if (posstar != -1)
					b = b.substr(posstar + 1, b.length());
				patterns_.push_back(b);
		}
		else
		{
			int posspacetwo = b.find(" ");
			if (posspacetwo==0)
				b = b.substr(1, b.length());
			if (posspacetwo != b.length())
				b = b.substr(0, b.length()-1);
			patterns_.push_back(b);
		}
	}
	std::cout << std::endl;
}

//----< do the continue search >----------------------------------------
void Cataloger::ContinueSearch(const DataStore& ds)
{
	int count = 0;
	std::cout << "************************************************" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "**              Continue Search               **" << std::endl;
	std::cout << "**                                            **" << std::endl;
	std::cout << "************************************************" << std::endl << std::endl;
	InputText();
	InputPattern();
	if (text_.size() != 0)
	{
	for (auto& fs : ds)
	{
		for (auto& y : fs.second)
		{
			std::string FileSpec = *y + fs.first;
			FileSystem::File findall(FileSpec);
			findall.open(FileSystem::File::in);
			if (findall.isGood())
			{
				std::string all = FileSystem::Path::toLower(findall.readAll());
				std::string textcopy = FileSystem::Path::toLower(text_);
				int optfind = all.find(textcopy);
				if (optfind != -1)
				{
					for (int i = 0; i != patterns_.size(); i++)
					{
						if (patterns_[i] == ".*") //if user select all patterns
						{
							std::cout << fs.first << " file contains the key text " << text_;
							std::cout << " in path: " << *y << std::endl << std::endl;
							count++;
						}
						int pos = fs.first.find(patterns_[i]);
						if (pos != -1) //compare the result with the user selected pattern
						{
							std::cout << fs.first << " file contains the key text " << text_<<" in path: " << *y << std::endl << std::endl;
							count++;
						}}}}findall.close();}}
	if (count == 0)
		std::cout << "There is no file contains the searching text in the file catalogue." << std::endl << std::endl;
	}
	else
	{
		std::cout << "There is no searching text." << std::endl << std::endl;
	}
}

//----< initialize the file catalogue >----------------------------------------
void Cataloger::start()
{
	DataStore ds;
	Display myDisplay;
	FileMgr fm(path_, ds);
	if (patterns_.size() != 0)
	{
		for (auto& elem : patterns_)
		{
			fm.addPattern(elem);
		}
	}
	if (options_.size() != 0)
	{
		for (auto& iter = options_.begin(); iter != options_.end(); iter++)
		{
			int opts = iter->find("/s"); int optS = iter->find("/S");
			if (optS != -1 || opts != -1)
			{
				fm.setRecursive();
				fm.search();
				myDisplay.ShowSummary(ds);
			}
		}
		for (auto& item : options_)
		{
			int optd = item.find("/d"); int optD = item.find("/D");
			int optf = item.find("/f"); int optF = item.find("/F");
			if (optD != -1 || optd != -1)
			{
				fm.search();
				myDisplay.ShowDuplicate(ds);
			}
			if (optF != -1 || optf != -1)
			{
				myDisplay.ShowMatchFile(ds, text_);
			}
		}
	}
	else
	{
		fm.search();
		myDisplay.ShowSummary(ds);
	}
	while (1) // do the continue search
	{ContinueSearch(ds);}
}

//---- < test stub > ---------------------------------
#ifdef TEST_CATALOGER
void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}

int main(int argc, char *argv[])
{
	title("Demonstrate File Cataloger");
	std::cout << "\n" << std::endl;
	Cataloger myCataloger;
	myCataloger.CommandParser(argv);
	myCataloger.start();

	return 0;
}
#endif