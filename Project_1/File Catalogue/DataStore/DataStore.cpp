///////////////////////////////////////////////////////////////////////
// DataStore.cpp - Implement the function in the DataStore.h file    //
// ver 1.0                                                           //
// Language:    C++                                                  //
// Platform:    Macbook Pro 13, Win8.1, SP1                          //
// Application: File Catalogue, Project #1, Spring 2015              //
// Author:      Ziyi Zhao, Computer Engineering, Syracuse University //
//              (315) 708-5889, zzhao37@syr.edu                      //
///////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* --------------------------------------------------------------------
* This module contains the following functions:

*  According to the different input (filespce or filename & path) to store
*  it into the store and the paths. And getsize function can return the size 
*  of the filename and the paths.
*
* It defines following functions:
*  save - save the file info to the store according to the file spec or the name and path.
*  getstoresize - return the number of the files in the store.
*  getpathsize - return the number of the paths in the path set.
*  begin - return the first iterator of the store
*  end - return the end iterator of the store
*
* Required Files:
*  DataStore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 30 JAN 2015
* - first release
* - write the function to store the file name and path
*
*/
#include "DataStore.h"
#include <iostream>
#include <string>

//----< parser filespec and save >------------------------------------
void DataStore::save(const std::string& filespec)
{
	Path FilePath;
	File FileName;
	int pos = filespec.find_last_of("/");
	if (pos == -1)
	{
		pos = filespec.find_last_of("\\");
	}
	FilePath = filespec.substr(0, pos+1);
	FileName = filespec.substr(pos + 1, filespec.length());
	save(FileName, FilePath);
}

//----< save filename and path >----------------------------------------
void DataStore::save(const std::string& filename, const std::string& path)
{
	ListOfIters List_Iters;
	PathIter Path_Iter = paths.find(path);
	iterator Map_Iter = store.find(filename);
	Path FilePath;
	//both filename and path do not exist
	if (Map_Iter == store.end() && Path_Iter == paths.end())
	{
		paths.insert(path);
		Path_Iter = paths.find(path);
		List_Iters.push_back(Path_Iter);
		store.insert({ filename, List_Iters });
	}
	//filename does not exist but path exist
	if (Map_Iter == store.end() && Path_Iter != paths.end())
	{
		List_Iters.push_back(Path_Iter);
		store.insert({ filename, List_Iters });
	}
	//filename exist but path does not exist
	if (Map_Iter != store.end() && Path_Iter == paths.end())
	{
		paths.insert(path);
		Path_Iter = paths.find(path);
		iterator Map_Iter = store.find(filename);
		Map_Iter->second.push_back(Path_Iter);
		store.insert({ filename, Map_Iter->second });
	}
	//both filename and path exist
	if (Map_Iter != store.end() && Path_Iter != paths.end())
	{
		int flag = 0;
		for (auto& y : Map_Iter->second )
		{
			if (path.compare(*y)==0)
				flag++;
		}
		if (flag==0)
		{
			Path_Iter = paths.find(path);
			iterator Map_Iter = store.find(filename);
			Map_Iter->second.push_back(Path_Iter);
			store.insert({ filename, Map_Iter->second });
		}
	}
}

//----< get the size of the filename >----------------------------
int DataStore::getstoresize() const
{
	return store.size();
}

//----< get the size of the path >--------------------------------
int DataStore::getpathsize() const
{
	return paths.size();
}

//---- < test stub > ---------------------------------
#ifdef TEST_DATASTORE
void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}
int main()
{
	title("Demonstrate Data Store");
	std::cout <<"\n"<<std::endl;

	std::string Path1("C:/Users/Mark Zhao/Desktop");
	std::string Path2("C:/Users/Mark Zhao/Desktop/ZZY");
	std::string Path3("C:/ZZY/SJK/Lhj");
	std::string Path4("D:/ZZY/SJK/Lhj");

	std::string FileName1("ZZY.c");
	std::string FileName2("ZZY2.c");
	std::string FileName3("KB.c");

	std::string FileSpec("C:/ZZY/SJK/Lhj.c");
	std::string FileSpec2("C:\\ZZY\\SJK\\SJK.c");

	DataStore myDataStore;
	myDataStore.save(FileName1, Path1);
	myDataStore.save(FileName2, Path2);
	myDataStore.save(FileName3, Path2); //test existing path
	myDataStore.save(FileName2, Path3); //test existing filename
	myDataStore.save(FileName2, Path1); //test existing filename has multiple paths
	myDataStore.save(FileName2, Path2); //test add same filename and same path
	myDataStore.save(FileName1, Path4); //test \ and / symbol
	myDataStore.save(FileSpec); //test save the filespce
	myDataStore.save(FileSpec2);
	
	for (auto& x : myDataStore)
	{
		std::cout << "The file name is:"<<x.first<<std::endl;
		for (auto& y: x.second )
		{
				std::cout <<"The file path is:"<< *y << std::endl;		
		}
		std::cout << std::endl;
	}
}
#endif
