#ifndef CLIENT_H
#define CLIENT_H
/////////////////////////////////////////////////////////////////////
// ClientUI.h   -  C++\CLI implementation of WPF Application       //
// ver 1.2                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package is responsible for building a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*
Public Interface:
* ===============
* WPFCppCliDemo();
* ~WPFCppCliDemo();
* void setUpStatusBar();
* void setUpTabControl();
* void setUpSendMessageView();
* void setSearchFileTextView();
* void setDownloadFileView();
* void sendUploadMessage(Object^ obj, RoutedEventArgs^ args);
* void sendDownloadMessage(Object^ obj, RoutedEventArgs^ args);
* void sendExploringMessage(Object^ obj, RoutedEventArgs^ args);
* void sendSearchFileMessage(Object^ obj, RoutedEventArgs^ args);
* void sendSearchTextMessage(Object^ obj, RoutedEventArgs^ args);
* void searchClearResult(Object^ obj, RoutedEventArgs^ args);
* void searchAddServer(Object^ obj, RoutedEventArgs^ args);
* void searchDeleteServer(Object^ obj, RoutedEventArgs^ args);
* void searchAddPattern(Object^ obj, RoutedEventArgs^ args);
* void searchDeletePattern(Object^ obj, RoutedEventArgs^ args);
* void addText(String^ msg);
* void getMessage();
* void clear(Object^ sender, RoutedEventArgs^ args);
* void clearDownloadInfo(Object^ sender, RoutedEventArgs^ args);
* void OnLoaded(Object^ sender, RoutedEventArgs^ args);
* void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
*
*  Required Files:
*  ---------------
*  MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*
*  Maintenance History:
*  --------------------
*  ver 1.2 : 27 Apr 2015
*  - split the download GUI from the upload/download GUI
*  ver 1.1 : 25 Apr 2015
*  - add search file/text GUI
*  ver 1.0 : 24 Apr 2015
*  - first release
*  - add upload/download GUI
*/

using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;
using namespace System::Collections::Generic;

#include "../MockChannel/MockChannel.h"
#include <iostream>
#include <vector>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references

		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;

		// Controls for Window

		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();

		TabItem^ hUploadDownloadFileTab = gcnew TabItem();
		TabItem^ hSearchFileTextTab = gcnew TabItem();
		TabItem^ hDownloadFileTab = gcnew TabItem();

		TabItem^ hFileListTab = gcnew TabItem();
		TabItem^ hConnectTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		// Controls for Upload View

		Grid^ hSendMessageGrid = gcnew Grid();
		Button^ hUploadButton = gcnew Button();
		Button^ hClearButton = gcnew Button();
		//Button^ hDownloadButton = gcnew Button();

		TextBlock^ hTextBlock1 = gcnew TextBlock(); //text block to show the reply information

		TextBlock^ hTextBlockDesIP = gcnew TextBlock();
		TextBox^ hTextBoxDesIP = gcnew TextBox();
		TextBlock^ hTextBlockDesPort = gcnew TextBlock();
		TextBox^ hTextBoxDesPort = gcnew TextBox();
		TextBlock^ hTextBlockFileName = gcnew TextBlock();
		TextBox^ hTextBoxFileName = gcnew TextBox();
		TextBlock^ hTextBlockClientPort = gcnew TextBlock();
		TextBox^ hTextBoxFileClientPort = gcnew TextBox();

		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ hStackPanel0 = gcnew StackPanel();//control the upload destination information
		StackPanel^ hStackPanel2 = gcnew StackPanel();//control the upload filename
		StackPanel^ hStackPanel1 = gcnew StackPanel();//control the upload button

		// Control for Download View
		Grid^ hDownloadMessageGrid = gcnew Grid();
		Button^ hDownloadExploringButton = gcnew Button();
		Button^ hDownloadClearButton = gcnew Button();
		Button^ hDownloadButton = gcnew Button();

		TextBlock^ hServerFileInfoTextBlock = gcnew TextBlock();
		TextBlock^ hServerFileListBlock = gcnew TextBlock();
		TextBlock^ hBlankLine = gcnew TextBlock();
		TextBlock^ hDownloadResultBlock = gcnew TextBlock(); //text block to show the reply information

		TextBlock^ hDownloadTextBlockDesIP = gcnew TextBlock();
		TextBox^ hDownloadTextBoxDesIP = gcnew TextBox();
		TextBlock^ hDownloadTextBlockDesPort = gcnew TextBlock();
		TextBox^ hDownloadTextBoxDesPort = gcnew TextBox();
		TextBlock^ hDownloadTextBlockFileName = gcnew TextBlock();
		TextBox^ hDownloadTextBoxFileName = gcnew TextBox();
		TextBlock^ hDownloadTextBlockClientPort = gcnew TextBlock();
		TextBox^ hDownloadTextBoxFileClientPort = gcnew TextBox();

		ScrollViewer^ hScrollViewer4 = gcnew ScrollViewer(); // for the download reply info text block
		ScrollViewer^ hScrollViewer5 = gcnew ScrollViewer(); //control the file list

		StackPanel^ hStackPanel8 = gcnew StackPanel();//control the download destination information
		StackPanel^ hStackPanel9 = gcnew StackPanel();//control the download filename
		StackPanel^ hStackPanel10 = gcnew StackPanel();//control the blank pannel
		StackPanel^ hStackPanel11 = gcnew StackPanel();//control the download button

		// receive thread

		Thread^ recvThread;

		// Controls for SearchView View
		Grid^ hSearchGrid = gcnew Grid();

		Button^ hSearchFileButton = gcnew Button();
		Button^ hSearchTextButton = gcnew Button();
		Button^ hSearchClearResultButton = gcnew Button();
		Button^ hSearchAddServerButton = gcnew Button();
		Button^ hSearchDeleteServerButton = gcnew Button();
		Button^ hSearchAddPatternrButton = gcnew Button();
		Button^ hSearchDeletePatternButton = gcnew Button();

		CheckBox^ hXMLString = gcnew CheckBox();

		TextBlock^ hSearchBlock = gcnew TextBlock(); //text block to show the reply information
		ListBox^ hServerInfoBlock = gcnew ListBox();
		ListBox^ hPatternInfoBlock = gcnew ListBox();

		TextBlock^ hSearchTextBlockDesIP = gcnew TextBlock();
		TextBox^ hSearchTextBoxDesIP = gcnew TextBox();
		TextBlock^ hSearchTextBlockDesPort = gcnew TextBlock();
		TextBox^ hSearchTextBoxDesPort = gcnew TextBox();
		TextBlock^ hSearchTextBlockFileName = gcnew TextBlock();
		TextBox^ hSearchTextBoxFileName = gcnew TextBox();
		TextBlock^ hSearchTextBlockString = gcnew TextBlock();
		TextBox^ hSearchTextBoxFileString = gcnew TextBox();
		TextBlock^ hSearchTextBlockClientPort = gcnew TextBlock();
		TextBox^ hSearchTextBoxFileClientPort = gcnew TextBox();
		TextBlock^ hSearchTextBlockPath = gcnew TextBlock();
		TextBox^ hSearchTextBoxPath = gcnew TextBox();
		TextBlock^ hSearchTextBlockPattern = gcnew TextBlock();
		TextBox^ hSearchTextBoxDesPattern = gcnew TextBox();

		TextBlock^ hSearchTextBlockServerInfo = gcnew TextBlock();
		TextBlock^ hSearchTextBlockPatternInfo = gcnew TextBlock();

		ScrollViewer^ hScrollViewer3 = gcnew ScrollViewer(); // for the reply info text block

		StackPanel^ hStackPanel3 = gcnew StackPanel();//control the search destination information
		StackPanel^ hStackPanel4 = gcnew StackPanel();//control the search filename
		StackPanel^ hStackPanel5 = gcnew StackPanel();//control the search button
		StackPanel^ hStackPanel6 = gcnew StackPanel();//control the server info text block and pattern info text block 
		StackPanel^ hStackPanel7 = gcnew StackPanel();//control the path and pattern

	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();

		void setUpStatusBar();
		void setUpTabControl();

		void setUpSendMessageView();
		void setSearchFileTextView();
		void setDownloadFileView();

		void sendUploadMessage(Object^ obj, RoutedEventArgs^ args);
		void sendDownloadMessage(Object^ obj, RoutedEventArgs^ args);
		void sendExploringMessage(Object^ obj, RoutedEventArgs^ args);
		void sendSearchFileMessage(Object^ obj, RoutedEventArgs^ args);
		void sendSearchTextMessage(Object^ obj, RoutedEventArgs^ args);
		void searchClearResult(Object^ obj, RoutedEventArgs^ args);

		void searchAddServer(Object^ obj, RoutedEventArgs^ args);
		void searchDeleteServer(Object^ obj, RoutedEventArgs^ args);
		void searchAddPattern(Object^ obj, RoutedEventArgs^ args);
		void searchDeletePattern(Object^ obj, RoutedEventArgs^ args);

		void addText(String^ msg);
		void getMessage();
		void clear(Object^ sender, RoutedEventArgs^ args);
		void clearDownloadInfo(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		List<String^>^ vServerInfo = gcnew List < String^ > ;
		List<String^>^ vPatternInfo = gcnew List < String^ >;

		void setUploadDesTextBlockProperties();
		void setUploadFileNameTextBlockProperties();

		void setDownloadDesTextBlockProperties();
		void setDownloadFileNameTextBlockProperties();
		void setDownloadServerFileInfoListBoxProperties();
		void setDownloadTextBlockProperties();
		void setDownloadButtonsProperties();
		void setBlankLineProperties();

		void setSearchDesTextBlockProperties();
		void setSearchFileNameTextBlockProperties();
		void setSearchPathPatternTextBlockProperties();

		void setUploadTextBlockProperties();
		void setSearchServerPattInfoTextBlockProperties();
		void setSearchReturnResultTextBlockProperties();

		void setUploadDownloadButtonsProperties();
		void setSearchFileTextButtonsProperties();

	};
}

#endif
