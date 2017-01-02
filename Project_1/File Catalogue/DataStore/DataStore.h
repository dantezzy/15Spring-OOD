#ifndef DATASTORE_H
#define DATASTORE_H
///////////////////////////////////////////////////////////////////////
// DataStore.h - Define the public function interface and            //
//               the private memeber of the DataStore.cpp            //
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
*   save - save the file info to the store according to the file spec or the name and path.
*   getstoresize - return the number of the files in the store.
*   getpathsize - return the number of the paths in the path set.
*   begin - return the first iterator of the store
*   end - return the end iterator of the store
*
* Required Files:
*
*
* Maintenance History:
* --------------------
* ver 1.0 : 30 JAN 2015
* - first release
*
*/

#include <vector>
#include <set>
#include <list>
#include <map>
#include <string>

class DataStore
{
public:
	using Path = std::string;
	using Paths = std::set < Path > ;
	using PathIter = Paths::iterator;
	using ListOfIters = std::list < PathIter > ;
	using File = std::string;
	using Store = std::map < File, ListOfIters > ;
	using iterator = Store::iterator;
	using const_iterator = Store::const_iterator;

	void save(const std::string& filespec);
	void save(const std::string& filename, const std::string& path);
	int getstoresize() const;
	int getpathsize() const;
	iterator begin() { return store.begin(); }
    iterator end() { return store.end(); }
	const_iterator begin() const { return store.begin(); }
	const_iterator end() const { return store.end(); }
private:
	Store store;
	Paths paths;
};
#endif
