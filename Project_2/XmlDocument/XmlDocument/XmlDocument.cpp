///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a tool to operate the node in the tree      //
//                                                               //
// Application: XML Document Pr#2, Spring 2015                   //
// Platform:    Macbook Pro 13, Win8.1, SP1                      //
// Author:      Ziyi Zhao, zzhao37@syr.edu                       //
// Source:      Jim Fawcett, CST 4-187, 443-3948                 //
//              jfawcett@twcny.rr.com                            //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include "XmlDocument.h"

using namespace XmlProcessing;

XmlProcessing::XmlDocument::XmlDocument(const std::string& src, sourceType srcType)
{
	pDocElement_ = makeDocElement();
	if (srcType == sourceType::filename)
		readfile(src);
	else
		readstring(src);		
}

//<--------------move assignment------------------>
XmlDocument& XmlDocument::operator=(XmlDocument&& xd)
{
	pDocElement_ = std::move(xd.pDocElement_);
	found_ = std::move(xd.found_);
	/*if (this == &xd) return *this;
	pDocElement_ = xd.pDocElement_;
	xd.pDocElement_ = nullptr;*/
	return *this;
}

//<--------------find element function------------------>
XmlDocument& XmlDocument::element(const std::string& tag)
{
	try
	{
		for (auto iter = begin(found_); iter != end(found_); ++iter)
		{
			if ((*iter)->value() == tag)
			{
				std::cout << "\n" << (*iter)->value()<<" node was founded." << "\n";
				element_.push_back(*iter);
				break;
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	if (element_.size() != 0)
	{
		found_.clear();
		found_ = element_;
		element_.clear();
	}
	else
	{
		found_.clear();
	}
	return *this;
}

//<--------------find elements function------------------>
XmlDocument& XmlDocument::elements(const std::string& tag)
{
	try
	{
		for (auto iter = begin(found_); iter != end(found_); ++iter)
		{
			if ((*iter)->value() == tag)
			{
				std::cout << "\n" << (*iter)->value();
				elements_.push_back(*iter);
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	if (elements_.size() != 0)
	{
		found_.clear();
		found_ = elements_;
		elements_.clear();
	}
	else
	{
		found_.clear();
		std::cout << "\n" << "No";
	}
	return *this;
}

//<--------------find childern function------------------>
XmlDocument& XmlDocument::children(const std::string& tag)
{
	if (found_.size()!=0&&found_[0]->children().size() != 0)
	{
		try
		{
			std::stack<std::shared_ptr < AbstractXmlElement >> nodeStack;
			nodeStack.push(found_[0]);
			while (nodeStack.size() > 0)
			{
				std::shared_ptr < AbstractXmlElement > pCurrentNode = nodeStack.top();
				nodeStack.pop();
				children_.push_back(pCurrentNode);
				if (pCurrentNode->children().size())
				{
					for (auto iter = begin(pCurrentNode->children()); iter != end(pCurrentNode->children()); ++iter)
					{ 
						if (tag.size() != NULL&&(*iter)->value() == tag)
							children_.push_back(*iter);
					   if (tag.size() == NULL)
						children_.push_back(*iter);
					}
				}
			}
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		found_.clear();
		children_.erase(children_.begin() + 0);
		found_ = children_;
		children_.clear();
		if (found_.size() != 0)
		{
			for (size_t i = 0; i < found_.size(); i++)
				std::cout << "\n" << "\"" << found_[0]->value() << "\"";
		}
		
	}
	else
	{
		found_.clear();
		std::cout << "\n" << found_.size() << "\n";
	}
	return *this;
}

//<--------------find descendents function------------------>
XmlDocument& XmlDocument::descendents(const std::string& tag)
{
	if (found_.size()!=0&&found_[0]->children().size() != 0)
	{
		try
		{
			std::stack<std::shared_ptr < AbstractXmlElement >> nodeStack;
			nodeStack.push(found_[0]);
			while (nodeStack.size() > 0)
			{
				std::shared_ptr < AbstractXmlElement > pCurrentNode = nodeStack.top();
				nodeStack.pop();
				if (tag.size()!=NULL&&pCurrentNode->value() == tag)
					descendents_.push_back(pCurrentNode);
				if (tag.size()==NULL)
					descendents_.push_back(pCurrentNode);
				if (pCurrentNode->children().size())
				{
					for (auto pChild : pCurrentNode->children())
						nodeStack.push(pChild);
				}
			}
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}

		if (found_[0] == descendents_[0])
			descendents_.erase(descendents_.begin() + 0);
		found_.clear();
		found_ = descendents_;
		descendents_.clear();
		//std::cout << "\n" << found_.size() << "\n";
		if (found_.size() != 0)
		{
			for (size_t i = 0; i < found_.size(); i++)
				std::cout << "\n" << "\"" << found_[i]->value() << "\"";
		}
	}
	else
	{
		found_.clear();
		std::cout << "\n" << found_.size() << "\n";
	}
	return *this;
}

//<--------------find attribute by id function------------------>
XmlDocument& XmlDocument::findattributeid(const std::string& name)
{
	try
	{
		for (auto iter = begin(found_); iter != end(found_); ++iter)
		{
			if (attribute_.size() == 1)
				break;
				for (auto itertwo = begin((*iter)->attribte()); itertwo != end((*iter)->attribte()); ++itertwo)
				{
					if (itertwo->first == name)
					{
						attribute_.push_back(*iter);
						break;
					}
				}
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	if (attribute_.size() != 0)
	{
		found_.clear();
		found_ = attribute_;
		attribute_.clear();
		//std::cout << "\n" << found_.size() << "\n";
	}
	else
	{
		found_.clear();
		//std::cout << "\n" << found_.size() << "\n";
	}
	if (found_.size()!=0)
	{ 
	for (size_t i = 0; i < found_.size(); i++)
	    std::cout << "\n" <<"\""<< found_[0]->value()<<"\"";
	}
	else
		std::cout << "No";
	return *this;
}

//<--------------find attribute function------------------>
std::vector<std::pair<std::string, std::string>> XmlDocument::returnattribute()
{

	if (found_.size()!=0&&found_[0]->attribte().size() != 0)
	{
		for (auto attr : found_[0]->attribte())
		{
			std::pair<std::string, std::string> atPair(attr.first, attr.second);
			returnattribs_.push_back(atPair);
		}
		for (auto attr : returnattribs_)
			std::cout << "\n" << attr.first << "-" << attr.second;
		std::cout << " is a attribute pair.\n";
	}
	else
	{
		returnattribs_.clear();
		std::cout << "\nThere is no attribute pair in this node.\n";
	}

	return returnattribs_;
}

//<--------------find attribute pair function------------------>
void XmlDocument::TreeWalk(std::shared_ptr < AbstractXmlElement > pNode)
{
	std::stack<std::shared_ptr < AbstractXmlElement >> nodeStack;
	nodeStack.push(pNode);
	while (nodeStack.size() > 0)
	{
		std::shared_ptr < AbstractXmlElement > pCurrentNode = nodeStack.top();
		nodeStack.pop();
		found_.push_back(pCurrentNode);
		if (pCurrentNode->children().size())
		{
			for (auto pChild : pCurrentNode->children())
				nodeStack.push(pChild);
		}
	}
}

//<--------------add root to an empty tree------------------>
void XmlDocument::addroot()
{
	std::shared_ptr < AbstractXmlElement > root = makeTaggedElement("root");

	pDocElement_ = makeDocElement(root);
	std::cout << "  " << pDocElement_->toString();
}

//<--------------return found_------------------>
std::vector<std::shared_ptr < AbstractXmlElement >> XmlDocument::select()
{
	return std::move(found_);
}

//<--------------read from file------------------>
void XmlDocument::readfile(std::string file)
{
	Toker toker(file);
	toker.setMode(Toker::xml);
	XmlParts parts(&toker);
	pDocElement_->children().push_back(parsefile(store_, parts));
	//std::cout << "  " << pDocElement_->toString();
}

//<--------------read from string------------------>
void XmlDocument::readstring(std::string xmlstring)
{
	Toker toker(xmlstring,false);
	toker.setMode(Toker::xml);
	XmlParts parts(&toker);
	pDocElement_->children().push_back(parsefile(store_, parts));
	//std::cout << "  " << pDocElement_->toString();
}

//<--------------parse the file to xml element------------------>
std::shared_ptr < AbstractXmlElement > XmlDocument::parsefile(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts)
{
	std::shared_ptr < AbstractXmlElement > root;
	while (parts.get())
	{
		if (parts[0] == "<"&&parts[1] == "?")
		{
			DelProProcess(store, parts);
			continue;
		}
		if (parts[0]=="<"&&parts[1]=="!")
		{
			CommentProcess(store, parts);
			continue;
		}
		if (parts[0]=="<"&&parts[1]=="/")
		{
			root = store.front();
			if (store.size())
			store.pop_back();
			continue;
		}
		if (parts[0]=="<")
		{
			TagEleProcess(store, parts);
		}
		else
		{
			TextProcess(store, parts);
		}
	}
	return root;
}

//<--------------process the declaration and the instruction------------------>
void XmlDocument::DelProProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts)
{
	std::shared_ptr < AbstractXmlElement > docEl;
	std::vector<int> position;
	if (parts[2]=="xml")
	{
		docEl = makeXmlDeclarElement();
		for (int i = 0; i < parts.length() - 1; i++)
			if (parts[i] == "=")
				position.push_back(i);

		for (auto pos_ : position)
			docEl->addAttrib(parts[pos_ - 1], parts[pos_ + 1]);
	}
	else
	{
		docEl = makeProcInstrElement(parts[3]);
		for (int i = 0; i < parts.length() - 1; i++)
			if (parts[i] == "=")
				position.push_back(i);

		for (auto pos_ : position)
			docEl->addAttrib(parts[pos_ - 1], parts[pos_ + 1]);
	}	
		pDocElement_->children().push_back(docEl);
		
}

//<--------------process the comment------------------>
void XmlDocument::CommentProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts)
{
	std::string xmlcomment;
	for (int i = 3; i < parts.length() - 2; i++)
		xmlcomment.append(" ").append(parts[i]);
	if (store.size() != 0)
		store.back()->addChild(makeCommentElement(xmlcomment));
	else
		pDocElement_->addChild(makeCommentElement(xmlcomment));
}

//<--------------process tag element------------------>
void XmlDocument::TagEleProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts)
{
	std::shared_ptr < AbstractXmlElement > TagPtr = makeTaggedElement(parts[1]);
	std::vector<int> position;
	
	for (int i = 0; i < parts.length() - 1; i++)
		if (parts[i] == "=")
			position.push_back(i);
	
	for (auto pos_ : position)
		TagPtr->addAttrib(parts[pos_ -1], parts[pos_ + 1]);

	if (store.size())
		store.back()->addChild(TagPtr);

	store.push_back(TagPtr);
}

//<--------------process text element------------------>
void XmlDocument::TextProcess(std::vector<std::shared_ptr < AbstractXmlElement >>& store, XmlParts& parts)
{
	std::string xmltext;
	for (int i = 0; i < parts.length(); i++)
		xmltext.append(" ").append(parts[i]);
	store.back()->addChild(makeTextElement(xmltext));
}

//<--------------add child by id------------------>
bool XmlDocument::addchildbyid(std::shared_ptr < AbstractXmlElement > pChild, const std::string& name)
{
	findattributeid(name);
	if (found_.size() != 0)
	{
		if (found_[0]->addChild(pChild))
			return true;	
	}
	else
		std::cout << "\nNo element is searched by id." << std::endl;
	return false;
}

//<--------------remove child by id------------------>
bool XmlDocument::removechildbyid(const std::string& parentname, const std::string& childtag)
{
	std::shared_ptr < AbstractXmlElement > temp;
	findattributeid(parentname);
	if (found_.size() != 0)
	temp = found_[0];
	findattributeid(parentname).children(childtag);
	if (found_.size() != 0)
	{
		if (temp->removeChild(found_[0]))
			return true;
	}
	else
		std::cout << "\nNo element is searched by id." << std::endl;
	return false;
}

//<--------------add child by tag------------------>
bool XmlDocument::addchildbytag(std::shared_ptr < AbstractXmlElement > pChild, const std::string& tag)
{
	element(tag);
	if (found_.size() != 0)
	{
		if (found_[0]->addChild(pChild))
			return true;
	}
	else
		std::cout << "\nNo element is searched by tag." << std::endl;
	return false;
}

//<--------------remove child by tag------------------>
bool XmlDocument::removechildbytag(const std::string& tag, const std::string& childtag)
{
	std::shared_ptr < AbstractXmlElement > temp;
	element(tag);
	if (found_.size() != 0)
	temp = found_[0];
	element(tag).children(childtag);
	if (found_.size() != 0)
	{
		if (temp->removeChild(found_[0]))
			return true;
	}
	else
		std::cout << "\nNo element is searched by id." << std::endl;
	return false;
}

//<--------------add attribute by id------------------>
bool XmlDocument::addattributebyid(const std::string& name, const std::string& attrname, const std::string& value)
{
	findattributeid(name);
	if (found_.size() != 0)
	{
		if (found_[0]->addAttrib(attrname,value))
			return true;
	}
	else
		std::cout << "\nNo element is searched by id." << std::endl;
	return false;
}

//<--------------remove attribute by id------------------>
bool XmlDocument::removeattributebyid(const std::string& name)
{
	findattributeid(name);
	if (found_.size() != 0)
	{
		//std::cout << found_[0]->value();
		if (found_[0]->removeAttrib(name))
			return true;
	}
	else
		std::cout << "\nNo element is searched by tag." << std::endl;
	return false;
}

//<--------------add attribute by tag------------------>
bool XmlDocument::addattributebytag(const std::string& tag, const std::string& attrname, const std::string& value)
{
	element(tag);
	if (found_.size() != 0)
	{
		//std::cout << found_[0]->value();
		if (found_[0]->addAttrib(attrname, value))
			return true;
	}
	else
		std::cout << "\nNo element is searched by tag." << std::endl;
	return false;
}

//<--------------remove attribute by tag------------------>
bool XmlDocument::removeattributebytag(const std::string& tag,const std::string& name)
{
	element(tag);
	if (found_.size() != 0)
	{
		//std::cout << found_[0]->value();
		if (found_[0]->removeAttrib(name))
			return true;
	}
	else
		std::cout << "\nNo element is searched by tag." << std::endl;
	return false;
}

//<--------------write to string------------------>
void XmlDocument::writetostring(std::shared_ptr < AbstractXmlElement > pNode)
{
	std::cout << pNode->toString();
}

//<--------------write to file------------------>
void XmlDocument::writetofile(std::string filename, std::shared_ptr < AbstractXmlElement > pNode)
{
	std::ofstream outtwo(filename);
	if (outtwo.good())
	{
		outtwo << pNode->toString();
		outtwo.close();
	}
	std::cout<<"\n" << "Writing XML file to" << "\"" << filename << "\"." << std::endl;
}

void XmlDocument::helptestadd(XmlDocument& xd)
{
	// test add attribute to one element by searching tag
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest add attribute to one element by searching tag:" << std::endl;
	if (xd.addattributebytag("note", "zzy", "123"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";

	// test add attribute to one element by searching id
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest add attribute to one element by searching tag:" << std::endl;
	if (xd.addattributebyid("zzy", "zzy", "456"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";

	//test add child to one element by searching tag
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest add child to one element by searching tag:" << std::endl;
	std::shared_ptr < AbstractXmlElement > child2 = makeTaggedElement("child2");
	child2->addChild(makeTextElement("this is second test"));
	child2->addAttrib("second", "test");
	child2->addAttrib("secondsecond", "test");
	if (xd.addchildbytag(child2, "reference"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";

	//test add child to one element by searching id
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest add child to one element by searching tag:" << std::endl;
	std::shared_ptr < AbstractXmlElement > child3 = makeTaggedElement("child3");
	child3->addChild(makeTextElement("this is third test"));
	child3->addAttrib("third", "test");
	if (xd.addchildbyid(child3, "second"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";

}

void XmlDocument::helptestremove(XmlDocument& xd)
{
	//test remove child from one element by searching id
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest remove child from one element by searching id:" << std::endl;
	if (xd.removechildbyid("second", "child3"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";


	//test remove child from one element by searching tag
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest remove child from one element by searching tag:" << std::endl;
	if (xd.removechildbytag("reference", "child2"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";

	//test remove attribute by searching id
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest remove attribute by searching id:" << std::endl;
	if (xd.removeattributebyid("zzy"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";

	//test remove attribute by searching id
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest remove attribute by searching id:" << std::endl;
	if (xd.removeattributebytag("note", "zzy"))
		std::cout << "\n\nSucceed!\n\n";
	else
		std::cout << "\n\nFailed!\n\n";
}

void XmlDocument::helptestfind(XmlDocument& xd)
{
	// test element function
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest find element:" << std::endl;
	xd.element("author");
	std::cout << std::endl;
	// test elements function
	xd.select().clear();
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest find elements:" << std::endl;
	xd.elements("author");
	std::cout << std::endl;
	// test children function with argument
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest find element,then find tag child:" << std::endl;
	xd.element("author").children("note");
	std::cout << std::endl;
	// test children function without argument
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest find element,then find withouttag child:" << std::endl;
	xd.element("author").children();
	std::cout << std::endl;
	// test descendents function with argument
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest find element,then find tag descendents:" << std::endl;
	xd.element("author").descendents("note");
	std::cout << std::endl;
	// test descendents function without argument
	std::cout << "\ntest find element,then find withouttag descendents:" << std::endl;
	xd.TreeWalk(xd.returnptr());
	xd.element("author").descendents();
	std::cout << std::endl;
	// test find attribute id function
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest find attribute by id:" << std::endl;
	xd.findattributeid("course");
	std::cout << std::endl;
	// test find attribute of one element
	xd.TreeWalk(xd.returnptr());
	std::cout << "\ntest find attribute in one node:" << std::endl;
	xd.element("note").returnattribute();
	std::cout << std::endl;
}

#ifdef TEST_XMLDOCUMENT
int main(int argc, char* argv[])
{
	using sPtr = std::shared_ptr < AbstractXmlElement >;

	title("Testing XmlDocument class read from file");
	std::cout << "\n";
	XmlDocument doc(argv[1], XmlProcessing::XmlDocument::filename);

	doc.helptestfind(doc);
	doc.helptestadd(doc);
	doc.writetostring(doc.returnptr());
	doc.helptestremove(doc);
	doc.writetostring(doc.returnptr());

	doc.writetofile("xml.xml", doc.returnptr());
	std::cout << "\n\n";

	title("Testing XmlDocument class read from string");
	std::string xmlstring = "<?xml version=\"1.0\"?> <!-- top level comment --> <parent att1='val1' att2='val2'> <child2> child2 body </child2> </parent>";

	XmlDocument doctwo(xmlstring, XmlProcessing::XmlDocument::string);
	std::cout << std::endl << std::endl;
	doctwo.writetostring(doctwo.returnptr());
	doctwo.writetofile("xml2.xml", doctwo.returnptr());	
	std::cout << "\n\n";

	std::cout << "\n\nTest move constructor\n\n";
	XmlProcessing::XmlDocument docmove(std::move(doctwo));
	docmove.writetostring(docmove.returnptr());

	std::cout << "\n\nTest move assignment operator\n\n";
	XmlProcessing::XmlDocument docmovetwo = std::move(doctwo);
	docmovetwo.writetostring(docmove.returnptr());

	// test add root
	doc.returnptr() == nullptr;
	std::cout << "\ntest add root:" << std::endl;
	doc.addroot();
	std::cout << std::endl<<std::endl;
	return 0;
}
#endif