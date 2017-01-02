///////////////////////////////////////////////////////////////////
// Executive.cpp - test all function of the project              //
// Ver 1.2                                                       //
// Application: XML Document Pr#2, Spring 2015                   //
// Platform:    Macbook Pro 13, Win8.1, SP1                      //
// Author:      Ziyi Zhao, zzhao37@syr.edu                       //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is test case executive package, it can test the project fulfill the 
* requirement.
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*     xmlElementParts.h, xmlElementParts.cpp
*     Tokenizer.h, Tokenizer.cpp
*     
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 19 Mar 15
* - first released
*
*/


#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include "../XmlElementParts/Tokenizer.h"
#include "../XmlElementParts/xmlElementParts.h"



void title(const std::string& title, char underlineChar)
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, underlineChar);
}

//<--------------test find attribute function------------------>
void attributebyid(XmlProcessing::XmlDocument& xd)
{
	std::cout << "\n\n";
	title("Testing Requirement 5", '-');
	// test find attribute id function
	xd.TreeWalk(xd.returnptr());
	title("Testing find attribute by id", '-');
	xd.findattributeid("course");
	std::cout << " node contains the attribute id \"course\"";
	std::cout << std::endl;
	title("Testing find attribute by id£¨no node exist condition£©", '-');
	std::cout << "\n";
	xd.findattributeid("course123");
	std::cout << " node contains the attribute id \"course123\"";
	std::cout << std::endl;
}

//<--------------test find element function------------------>
void elementsbytag(XmlProcessing::XmlDocument& xd)
{
	std::cout << "\n\n";
	title("Testing Requirement 6", '-');
	xd.select().clear();
	xd.TreeWalk(xd.returnptr());
	title("Testing find attribute by specified tag", '-');
	xd.elements("author");
	std::cout << " node contains the specified tag \"author\"";
	std::cout << std::endl;
	title("Testing find attribute by specified tag£¨no node exist condition£©", '-');
	xd.elements("author123");
	std::cout << " node contains the specified tag \"author123\"";
	std::cout << std::endl;
}

//<--------------test add/remove child function------------------>
void addremovechild(XmlProcessing::XmlDocument& xd)
{
	std::cout << "\n\n";
	title("Testing Requirement 7", '-');
	//test add child to one element by searching tag
	xd.TreeWalk(xd.returnptr());
	title("Testing add child to one element by searching tag", '-');
	std::cout << std::endl;
	std::shared_ptr < XmlProcessing::AbstractXmlElement > child2 = XmlProcessing::makeTaggedElement("child2");
	child2->addChild(XmlProcessing::makeTextElement("this is second test"));
	child2->addAttrib("second", "test");
	child2->addAttrib("secondsecond", "test");
	if (xd.addchildbytag(child2, "reference"))
		std::cout << "\n\nAdd Child 2 Succeed!\n\n";
	else
		std::cout << "\n\nAdd Child 2 Failed!\n\n";
	//test add child to one element by searching id
	xd.TreeWalk(xd.returnptr());
	title("Testing add child to one element by searching id", '-');
	std::cout <<std::endl;
	std::shared_ptr < XmlProcessing::AbstractXmlElement > child3 = XmlProcessing::makeTaggedElement("child3");
	child3->addChild(XmlProcessing::makeTextElement("this is third test"));
	child3->addAttrib("third", "test");
	if (xd.addchildbyid(child3, "second"))
		std::cout << "\n\nAdd Child 3 Succeed!\n\n";
	else
		std::cout << "\n\nAdd Child 3 Failed!\n\n";
	xd.writetostring(xd.returnptr()); std::cout << std::endl << std::endl;
	//test remove child from one element by searching id
	xd.TreeWalk(xd.returnptr());
	title("Testing remove child from one element by searching id:", '-');
	std::cout << std::endl;
	if (xd.removechildbyid("second", "child3"))
		std::cout << "\n\nRemove Succeed!\n\n";
	else
		std::cout << "\n\nRemove Failed!\n\n";
	//test remove child from one element by searching tag
	xd.TreeWalk(xd.returnptr());
	title("Testing remove child from one element by searching tag:", '-');
	std::cout<< std::endl;
	if (xd.removechildbytag("reference", "child2"))
		std::cout << "\n\nRemove Succeed!\n\n";
	else
		std::cout << "\n\nRemove Failed!\n\n";

	xd.writetostring(xd.returnptr()); std::cout << std::endl << std::endl;
}

//<--------------test return attribute pair function------------------>
void returnpair(XmlProcessing::XmlDocument& xd)
{
	std::cout << "\n\n";
	title("Testing Requirement 8", '-');
	// test find attribute of one element
	xd.TreeWalk(xd.returnptr());
	title("Testing return attribute pair in one node:", '-');
	std::cout << "\n";
	xd.element("note").returnattribute();
	std::cout << std::endl;
	xd.TreeWalk(xd.returnptr());
	title("Testing return attribute pair in one node(do not consist attribute):", '-');
	std::cout << "\n";
	xd.element("publisher").returnattribute();
	std::cout << std::endl;
	xd.TreeWalk(xd.returnptr());
	title("Testing return all childs of one node:", '-');
	// test descendents function with argument
	xd.TreeWalk(xd.returnptr());
	title("Testing return all childs of one node,with tag(note) search child:", '-');
	std::cout << std::endl;
	xd.element("author").descendents("note");
	std::cout << std::endl;
	// test descendents function without argument
	title("Testing return all childs of one node,without tag search child:", '-');
	xd.TreeWalk(xd.returnptr());
	xd.element("author").descendents();
	std::cout << std::endl;
}

//<--------------test add/remove attribute function------------------>
void addremoveattribute(XmlProcessing::XmlDocument& xd)
{
	std::cout << "\n\n";
	title("Testing Requirement 9", '-');
	// test add attribute to one element by searching tag
	xd.TreeWalk(xd.returnptr());
	title("Testing add attribute to one element by searching tag:", '-');
	std::cout << std::endl;
	if (xd.addattributebytag("note", "zzy", "123"))
		std::cout << "\n\nAdd \"zzy\", \"123\" attribute Succeed!\n\n";
	else
		std::cout << "\n\nAdd \"zzy\", \"123\" attribute Failed!\n\n";

	// test add attribute to one element by searching id
	xd.TreeWalk(xd.returnptr());
	title("Testing add attribute to one element by searching id:", '-');
	if (xd.addattributebyid("zzy", "zzy", "456"))
		std::cout << "\n\nAdd \"zzy\", \"456\" attribute pair Succeed!\n\n";
	else
		std::cout << "\n\nAdd \"zzy\", \"456\" attribute pair Failed!\n\n";

	xd.writetostring(xd.returnptr()); std::cout << std::endl << std::endl;

	//test remove attribute by searching id
	xd.TreeWalk(xd.returnptr());
	title("Testing remove attribute by searching id:", '-');
    std::cout<< std::endl;
	if (xd.removeattributebyid("zzy"))
		std::cout << "\n\nRemove zzy attribute Succeed!\n\n";
	else
		std::cout << "\n\nRemove zzy attribute Failed!\n\n";

	//test remove attribute by searching id
	xd.TreeWalk(xd.returnptr());
	title("Testing remove attribute by searching tag:", '-');
	std::cout << std::endl;
	if (xd.removeattributebytag("note", "zzy"))
		std::cout << "\n\nRemove note node attribute Succeed!\n\n";
	else
		std::cout << "\n\nRemove note node attribute Failed!\n\n";

	xd.writetostring(xd.returnptr()); std::cout << std::endl << std::endl;

}

int main(int argc, char* argv[])
{
	title("Testing Requirement 3", '-');
	title("Testing read from file",'-');std::cout << "\n";
	XmlProcessing::XmlDocument doc(argv[1], XmlProcessing::XmlDocument::filename);
	doc.writetostring(doc.returnptr());std::cout << std::endl << std::endl;
	title("Testing read from string", '-');
	std::string xmlstring = "<?xml version=\"1.0\"?> <!-- top level comment --> <parent att1='val1' att2='val2'> <child2> child2 body </child2> </parent>";
	XmlProcessing::XmlDocument doctwo(xmlstring, XmlProcessing::XmlDocument::string);std::cout << std::endl;
	doctwo.writetostring(doctwo.returnptr());std::cout << std::endl;

	title("Testing Requirement 4", '-');
	title("Testing move constructor", '-');
	XmlProcessing::XmlDocument docmove(std::move(doc));std::cout << std::endl;
	docmove.writetostring(docmove.returnptr());std::cout << std::endl << std::endl;
	title("Testing move assignment operator", '-');
	XmlProcessing::XmlDocument docmovetwo = std::move(doctwo);std::cout << std::endl;
	docmovetwo.writetostring(docmovetwo.returnptr());

	attributebyid(docmove);
	elementsbytag(docmove);
	addremovechild(docmove);
	doc.returnptr() == nullptr;
	title("Testing remove child from one element by searching tag:", '-');
	std::cout << std::endl;
	doc.addroot();
	returnpair(docmove);
	addremoveattribute(docmove);

	title("Testing Requirement 10", '-');
	std::cout << "\nAll demonstrate print in the command line use the XML write to string method.\n";
	docmove.writetofile("xml.xml", docmove.returnptr());
	docmovetwo.writetofile("xml2.xml", docmovetwo.returnptr());

	return 0;
}