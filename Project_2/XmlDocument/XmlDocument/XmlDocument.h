#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a tool to operate the node in the tree        //
// Ver 1.3                                                       //
// Application: XML Document Pr#2, Spring 2015                   //
// Platform:    Macbook Pro 13, Win8.1, SP1                      //
// Author:      Ziyi Zhao, zzhao37@syr.edu                       //
// Source:      Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to use the XmlElement package to build a tree.
* And finish some functions like add child, remove child, add attribute,
* remove attribute etc. And this package can also find element/elements and 
* find children and descendents;
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*     xmlElementParts.h, xmlElementParts.cpp
*     Tokenizer.h, Tokenizer.cpp
*
* Public Interface:
* =================
* XmlDocument& element(const std::string& tag);     // found_[0] contains first element (DFS order) with tag
* XmlDocument& elements(const std::string& tag);    // found_ contains all elements with tag
* XmlDocument& children(const std::string& tag = "");  // found_ contains sPtrs to children of prior found_[0]
* XmlDocument& descendents(const std::string& tag = ""); // found_ contains sPtrs to descendents of prior found_[0]
* XmlDocument& findattributeid(const std::string& name); //find any element based on a unique id attribute for all those elements that possess id attributes.
*
* bool addchildbyid(std::shared_ptr < AbstractXmlElement > pChild, const std::string& name); //search by id to add child
* bool removechildbyid(const std::string& parentname, const std::string& childtag); //search by id to remove child
* bool addchildbytag(std::shared_ptr < AbstractXmlElement > pChild, const std::string& tag); //search by tag to add child
* bool removechildbytag(const std::string& tag,const std::string& childtag); //search by tag to remove child
*
* bool addattributebyid(const std::string& name, const std::string& attrname, const std::string& value); //search by id to add attribute
* bool removeattributebyid(const std::string& name); //search by id to remove attribute
* bool addattributebytag(const std::string& tag, const std::string& attrname, const std::string& value); //search by tag to add attribute
* bool removeattributebytag(const std::string& tag, const std::string& name); // search by tag to remove attribute
*
* void addroot(); // add a root element to an empty document tree
*
* std::vector<std::shared_ptr < AbstractXmlElement >> select();  // return reference to found_.  Use std::move(found_) to clear found_
* void TreeWalk(std::shared_ptr < AbstractXmlElement > pNode); //traversal the whole tree
*
* void readfile(std::string file); //read from file
* void readstring(std::string xmlstring); //read from string
* std::shared_ptr < AbstractXmlElement > parsefile(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts); // parse the file to build the tree
*
* std::vector<std::pair<std::string, std::string>> returnattribute(); //return attribute pair
*
* void writetostring(std::shared_ptr < AbstractXmlElement > pNode); //write xml to the string
* void writetofile(std::string filename, std::shared_ptr < AbstractXmlElement > pNode); //write xml to the file
*
* std::shared_ptr < AbstractXmlElement > returnptr(){ return pDocElement_; } //return the pointer
* void helptestadd(XmlDocument& xd);
* void helptestremove(XmlDocument& xd);
* void helptestfind(XmlDocument& xd);
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.6 : 15 Mar 15
* - add return child function
* - add return attribute function
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15
* - first release
*/

#include <memory>
#include <string>
#include <stack>
#include "../XmlElement/XmlElement.h"
#include "../XmlElementParts/xmlElementParts.h"
#include "../XmlElementParts/Tokenizer.h"

namespace XmlProcessing
{
	class XmlDocument
	{
	public:

		enum sourceType { string, filename };

		XmlDocument(const std::string& src, sourceType srcType);

		XmlDocument(XmlDocument && xd) :pDocElement_(std::move(xd.pDocElement_)), found_(std::move(xd.found_)){}
		XmlDocument& operator=(XmlDocument&& xd);

		XmlDocument(const XmlDocument& doc) = delete;
		XmlDocument& operator=(const XmlDocument& doc) = delete;

		// queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();

		XmlDocument& element(const std::string& tag);     // found_[0] contains first element (DFS order) with tag
		XmlDocument& elements(const std::string& tag);    // found_ contains all elements with tag
		XmlDocument& children(const std::string& tag = "");  // found_ contains sPtrs to children of prior found_[0] 
		XmlDocument& descendents(const std::string& tag = ""); // found_ contains sPtrs to descendents of prior found_[0]
		XmlDocument& findattributeid(const std::string& name); //find any element based on a unique id attribute for all those elements that possess id attributes.
		
		bool addchildbyid(std::shared_ptr < AbstractXmlElement > pChild, const std::string& name); //search by id to add child
		bool removechildbyid(const std::string& parentname, const std::string& childtag); //search by id to remove child
		bool addchildbytag(std::shared_ptr < AbstractXmlElement > pChild, const std::string& tag); //search by tag to add child
		bool removechildbytag(const std::string& tag,const std::string& childtag); //search by tag to remove child

		bool addattributebyid(const std::string& name, const std::string& attrname, const std::string& value); //search by id to add attribute
		bool removeattributebyid(const std::string& name); //search by id to remove attribute
		bool addattributebytag(const std::string& tag, const std::string& attrname, const std::string& value); //search by tag to add attribute
		bool removeattributebytag(const std::string& tag, const std::string& name); // search by tag to remove attribute

		void addroot(); // add a root element to an empty document tree
		
		std::vector<std::shared_ptr < AbstractXmlElement >> select();  // return reference to found_.  Use std::move(found_) to clear found_
		void TreeWalk(std::shared_ptr < AbstractXmlElement > pNode); //traversal the whole tree

		void readfile(std::string file); //read from file
		void readstring(std::string xmlstring); //read from string
		std::shared_ptr < AbstractXmlElement > parsefile(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts); // parse the file to build the tree
		
		std::vector<std::pair<std::string, std::string>> returnattribute(); //return attribute pair

		void writetostring(std::shared_ptr < AbstractXmlElement > pNode); //write xml to the string
		void writetofile(std::string filename, std::shared_ptr < AbstractXmlElement > pNode); //write xml to the file

		std::shared_ptr < AbstractXmlElement > returnptr(){ return pDocElement_; } //return the pointer
		void helptestadd(XmlDocument& xd);
		void helptestremove(XmlDocument& xd);
		void helptestfind(XmlDocument& xd);

	private:
		std::shared_ptr < AbstractXmlElement > pDocElement_;         // AST that holds procInstr, comments, XML root, and more comments
		std::vector<std::shared_ptr < AbstractXmlElement >> found_;  // query results
		std::vector<std::shared_ptr < AbstractXmlElement >> element_;
		std::vector<std::shared_ptr < AbstractXmlElement >> elements_;
		std::vector<std::shared_ptr < AbstractXmlElement >> descendents_;
		std::vector<std::shared_ptr < AbstractXmlElement >> children_;
		std::vector<std::shared_ptr < AbstractXmlElement >> attribute_;
		std::vector<std::shared_ptr < AbstractXmlElement >> store_;
		std::vector<std::pair<std::string, std::string>> returnattribs_;
		void DelProProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts);
		void CommentProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts);
		void TagEleProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts);
		void TextProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts);
	};
}
#endif
