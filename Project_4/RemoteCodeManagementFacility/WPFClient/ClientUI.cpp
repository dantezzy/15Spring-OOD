/////////////////////////////////////////////////////////////////////
// ClientUI.cpp   -  C++\CLI implementation of WPF Application       //
// ver 1.2                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////

#include "ClientUI.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo()
{
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;
	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	// set event handlers
	this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing += gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendUploadMessage);
	hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendDownloadMessage);
	hDownloadExploringButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendExploringMessage);
	hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
	//hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
	hDownloadClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearDownloadInfo);

	hSearchFileButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendSearchFileMessage);
	hSearchTextButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendSearchTextMessage);
	hSearchClearResultButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchClearResult);

	hSearchAddServerButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchAddServer);
	hSearchDeleteServerButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchDeleteServer);
	hSearchAddPatternrButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchAddPattern);
	hSearchDeletePatternButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchDeletePattern);
	// set Window properties
	this->Title = "Remote Code Management Facility-by Ziyi Zhao";
	this->Width = 800;
	this->Height = 600;
	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setUpSendMessageView();
	setDownloadFileView();
	setSearchFileTextView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}

//----< set up status bar >----------------------------------------
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//----< set up tab >----------------------------------------
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hUploadDownloadFileTab->Header = "Upload";
	hSearchFileTextTab->Header = "Search File/Text";
	hDownloadFileTab->Header = "Download";
	hTabControl->Items->Add(hUploadDownloadFileTab);
	hTabControl->Items->Add(hDownloadFileTab);
	hTabControl->Items->Add(hSearchFileTextTab);

}

//----< set up upload server information >----------------------------------------
void WPFCppCliDemo::setUploadDesTextBlockProperties()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(50);
	hSendMessageGrid->RowDefinitions->Add(hRowDef);
	Border^ hBorder = gcnew Border(); //des IP text block
	hBorder->Width = 150;hBorder->Height = 25;
	hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hTextBlockDesIP;
	hTextBlockDesIP->Text = "Server IP:";
	hTextBlockDesIP->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlockDesIP->FontWeight = FontWeights::Bold;
	hTextBlockDesIP->FontSize = 16;
	Border^ hBorder1 = gcnew Border(); //des port text block
	hBorder1->Width = 150;hBorder1->Height = 25;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = hTextBlockDesPort;
	hTextBlockDesPort->Text = "Server Port:";
	hTextBlockDesPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlockDesPort->FontWeight = FontWeights::Bold;
	hTextBlockDesPort->FontSize = 16;
	Border^ hBorder2 = gcnew Border(); // des IP text box
	hBorder2->Width = 150;hBorder2->Height = 25;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hTextBoxDesPort;
	hTextBoxDesPort->Text = "localhost";
	hTextBoxDesPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBoxDesPort->FontWeight = FontWeights::Bold;
	hTextBoxDesPort->FontSize = 14;
	Border^ hBorder3 = gcnew Border(); // des IP text box
	hBorder3->Width = 150;hBorder3->Height = 25;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hTextBoxDesIP;
	hTextBoxDesIP->Text = "8080";
	hTextBoxDesIP->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBoxDesIP->FontWeight = FontWeights::Bold;
	hTextBoxDesIP->FontSize = 14;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 100;
	hStackPanel0->Children->Add(hBorder);hStackPanel0->Children->Add(hBorder2);hStackPanel0->Children->Add(hSpacer);
	hStackPanel0->Children->Add(hBorder1);hStackPanel0->Children->Add(hBorder3);
	hStackPanel0->Orientation = Orientation::Horizontal;
	hStackPanel0->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSendMessageGrid->SetRow(hStackPanel0, 0);
	hSendMessageGrid->Children->Add(hStackPanel0);
}

//----< set up download server information >----------------------------------------
void WPFCppCliDemo::setDownloadDesTextBlockProperties()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(50);
	hDownloadMessageGrid->RowDefinitions->Add(hRowDef);
	Border^ hBorder = gcnew Border(); //des IP text block
	hBorder->Width = 150; hBorder->Height = 25;
	hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hDownloadTextBlockDesIP;
	hDownloadTextBlockDesIP->Text = "Server IP:";
	hDownloadTextBlockDesIP->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockDesIP->FontWeight = FontWeights::Bold;
	hDownloadTextBlockDesIP->FontSize = 16;
	Border^ hBorder1 = gcnew Border(); //des port text block
	hBorder1->Width = 150; hBorder1->Height = 25;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = hDownloadTextBlockDesPort;
	hDownloadTextBlockDesPort->Text = "Server Port:";
	hDownloadTextBlockDesPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockDesPort->FontWeight = FontWeights::Bold;
	hDownloadTextBlockDesPort->FontSize = 16;
	Border^ hBorder2 = gcnew Border(); // des IP text box
	hBorder2->Width = 150; hBorder2->Height = 25;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDownloadTextBoxDesPort;
	hDownloadTextBoxDesPort->Text = "localhost";
	hDownloadTextBoxDesPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxDesPort->FontWeight = FontWeights::Bold;
	hDownloadTextBoxDesPort->FontSize = 14;
	Border^ hBorder3 = gcnew Border(); // des IP text box
	hBorder3->Width = 150; hBorder3->Height = 25;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadTextBoxDesIP;
	hDownloadTextBoxDesIP->Text = "8080";
	hDownloadTextBoxDesIP->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxDesIP->FontWeight = FontWeights::Bold;
	hDownloadTextBoxDesIP->FontSize = 14;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 100;
	hStackPanel8->Children->Add(hBorder); hStackPanel8->Children->Add(hBorder2); hStackPanel8->Children->Add(hSpacer);
	hStackPanel8->Children->Add(hBorder1); hStackPanel8->Children->Add(hBorder3);
	hStackPanel8->Orientation = Orientation::Horizontal;
	hStackPanel8->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDownloadMessageGrid->SetRow(hStackPanel8, 0);
	hDownloadMessageGrid->Children->Add(hStackPanel8);
}

//----< set up search server infomation >----------------------------------------
void  WPFCppCliDemo::setSearchDesTextBlockProperties()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(40);hSearchGrid->RowDefinitions->Add(hRowDef);
	Border^ hBorder = gcnew Border(); //des IP text block
	hBorder->Width = 90;hBorder->Height = 25;hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hSearchTextBlockDesIP;
	hSearchTextBlockDesIP->Text = "Server IP:";hSearchTextBlockDesIP->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockDesIP->FontWeight = FontWeights::Bold;hSearchTextBlockDesIP->FontSize = 16;
	Border^ hBorder1 = gcnew Border(); //des port text block
	hBorder1->Width = 100;hBorder1->Height = 25;hBorder1->BorderThickness = Thickness(0);
	hBorder1->Child = hSearchTextBlockDesPort;
	hSearchTextBlockDesPort->Text = "Server Port:";hSearchTextBlockDesPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockDesPort->FontWeight = FontWeights::Bold;hSearchTextBlockDesPort->FontSize = 16;
	Border^ hBorder2 = gcnew Border(); // des IP text box
	hBorder2->Width = 90;hBorder2->Height = 25;hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;hBorder2->Child = hSearchTextBoxDesIP;
	hSearchTextBoxDesIP->Text = "localhost";hSearchTextBoxDesIP->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBoxDesIP->FontWeight = FontWeights::Bold;hSearchTextBoxDesIP->FontSize = 14;
	Border^ hBorder3 = gcnew Border(); // des IP text box
	hBorder3->Width = 90;hBorder3->Height = 25;hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;hBorder3->Child = hSearchTextBoxDesPort;
	hSearchTextBoxDesPort->Text = "8080";hSearchTextBoxDesPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBoxDesPort->FontWeight = FontWeights::Bold;hSearchTextBoxDesPort->FontSize = 14;
	Border^ hBorder4 = gcnew Border(); // des IP text box
	hBorder4->Width = 100;hBorder4->Height = 25;hBorder4->BorderThickness = Thickness(0);
	hBorder4->BorderBrush = Brushes::Black;hBorder4->Child = hSearchTextBlockClientPort;
	hSearchTextBlockClientPort->Text = "Client Port:";hSearchTextBlockClientPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockClientPort->FontWeight = FontWeights::Bold;hSearchTextBlockClientPort->FontSize = 16;
	Border^ hBorder5 = gcnew Border(); // des IP text box
	hBorder5->Width = 90;hBorder5->Height = 25;hBorder5->BorderThickness = Thickness(1);
	hBorder5->BorderBrush = Brushes::Black;hBorder5->Child = hSearchTextBoxFileClientPort;
	hSearchTextBoxFileClientPort->Text = "5050";hSearchTextBoxFileClientPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBoxFileClientPort->FontWeight = FontWeights::Bold;hSearchTextBoxFileClientPort->FontSize = 14;
	Border^ hBorder6 = gcnew Border(); // des IP text box
	hBorder6->Height = 25;hBorder6->BorderThickness = Thickness(0);
	hBorder6->BorderBrush = Brushes::Black;hBorder6->Child = hXMLString;
	hXMLString->Content = "XML String";hXMLString->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hXMLString->FontWeight = FontWeights::Bold;hXMLString->FontSize = 14;
	TextBlock^ hSpacer = gcnew TextBlock();hSpacer->Width = 20;TextBlock^ hSpacer2 = gcnew TextBlock();hSpacer2->Width = 20;
	TextBlock^ hSpacer3 = gcnew TextBlock();hSpacer3->Width = 20;
	hStackPanel3->Children->Add(hBorder);hStackPanel3->Children->Add(hBorder2);hStackPanel3->Children->Add(hSpacer);
	hStackPanel3->Children->Add(hBorder1);hStackPanel3->Children->Add(hBorder3);hStackPanel3->Children->Add(hSpacer2);
	hStackPanel3->Children->Add(hBorder4);hStackPanel3->Children->Add(hBorder5);hStackPanel3->Children->Add(hSpacer3);
	hStackPanel3->Children->Add(hBorder6);
	hStackPanel3->Orientation = Orientation::Horizontal;
	hStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSearchGrid->SetRow(hStackPanel3, 0);hSearchGrid->Children->Add(hStackPanel3);
}

//----< set up upload file information >----------------------------------------
void WPFCppCliDemo::setUploadFileNameTextBlockProperties()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(50);
	hSendMessageGrid->RowDefinitions->Add(hRowDef);
	Border^ hBorder = gcnew Border();
	hBorder->Width = 150;hBorder->Height = 25;
	hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hTextBlockFileName;
	hTextBlockFileName->Text = "File Name:";
	hTextBlockFileName->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlockFileName->FontWeight = FontWeights::Bold;
	hTextBlockFileName->FontSize = 16;
	Border^ hBorder1 = gcnew Border(); // des IP text box
	hBorder1->Width = 150;hBorder1->Height = 25;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBoxFileName;
	hTextBoxFileName->Text = "UnitTest.h";
	hTextBoxFileName->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBoxFileName->FontWeight = FontWeights::Bold;
	hTextBoxFileName->FontSize = 14;
	Border^ hBorder2 = gcnew Border(); // client port text block
	hBorder2->Width = 150;hBorder2->Height = 25;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = hTextBlockClientPort;
	hTextBlockClientPort->Text = "Client Port:";
	hTextBlockClientPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlockClientPort->FontWeight = FontWeights::Bold;
	hTextBlockClientPort->FontSize = 16;
	Border^ hBorder3 = gcnew Border(); // client port text box
	hBorder3->Width = 150;hBorder3->Height = 25;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hTextBoxFileClientPort;
	hTextBoxFileClientPort->Text = "5050";
	hTextBoxFileClientPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBoxFileClientPort->FontWeight = FontWeights::Bold;
	hTextBoxFileClientPort->FontSize = 14;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 100;
	hStackPanel2->Children->Add(hBorder);hStackPanel2->Children->Add(hBorder1);hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder2);hStackPanel2->Children->Add(hBorder3);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSendMessageGrid->SetRow(hStackPanel2, 1);
	hSendMessageGrid->Children->Add(hStackPanel2);
}

//----< set up download file information >----------------------------------------
void WPFCppCliDemo::setDownloadFileNameTextBlockProperties()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(50);
	hDownloadMessageGrid->RowDefinitions->Add(hRowDef);
	Border^ hBorder = gcnew Border();
	hBorder->Width = 150; hBorder->Height = 25;
	hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hDownloadTextBlockFileName;
	hDownloadTextBlockFileName->Text = "File Name:";
	hDownloadTextBlockFileName->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockFileName->FontWeight = FontWeights::Bold;
	hDownloadTextBlockFileName->FontSize = 16;
	Border^ hBorder1 = gcnew Border(); // des IP text box
	hBorder1->Width = 150; hBorder1->Height = 25;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownloadTextBoxFileName;
	hDownloadTextBoxFileName->Text = "UnitTest.h";
	hDownloadTextBoxFileName->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxFileName->FontWeight = FontWeights::Bold;
	hDownloadTextBoxFileName->FontSize = 14;
	Border^ hBorder2 = gcnew Border(); // client port text block
	hBorder2->Width = 150; hBorder2->Height = 25;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = hDownloadTextBlockClientPort;
	hDownloadTextBlockClientPort->Text = "Client Port:";
	hDownloadTextBlockClientPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockClientPort->FontWeight = FontWeights::Bold;
	hDownloadTextBlockClientPort->FontSize = 16;
	Border^ hBorder3 = gcnew Border(); // client port text box
	hBorder3->Width = 150; hBorder3->Height = 25;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadTextBoxFileClientPort;
	hDownloadTextBoxFileClientPort->Text = "5050";
	hDownloadTextBoxFileClientPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxFileClientPort->FontWeight = FontWeights::Bold;
	hDownloadTextBoxFileClientPort->FontSize = 14;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 100;
	hStackPanel9->Children->Add(hBorder); hStackPanel9->Children->Add(hBorder1); hStackPanel9->Children->Add(hSpacer);
	hStackPanel9->Children->Add(hBorder2); hStackPanel9->Children->Add(hBorder3);
	hStackPanel9->Orientation = Orientation::Horizontal;
	hStackPanel9->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDownloadMessageGrid->SetRow(hStackPanel9, 1);
	hDownloadMessageGrid->Children->Add(hStackPanel9);
}

//----< set up search file information >----------------------------------------
void WPFCppCliDemo::setSearchFileNameTextBlockProperties()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(40);
	hSearchGrid->RowDefinitions->Add(hRowDef);
	Border^ hBorder2 = gcnew Border(); // client port text block
	hBorder2->Width = 90;hBorder2->Height = 25;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = hSearchTextBlockString;
	hSearchTextBlockString->Text = "Text:";
	hSearchTextBlockString->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockString->FontWeight = FontWeights::Bold;
	hSearchTextBlockString->FontSize = 16;
	Border^ hBorder3 = gcnew Border(); // client port text box
	hBorder3->Width = 90;hBorder3->Height = 25;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hSearchTextBoxFileString;
	hSearchTextBoxFileString->Text = "ZzY";
	hSearchTextBoxFileString->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBoxFileString->FontWeight = FontWeights::Bold;
	hSearchTextBoxFileString->FontSize = 14;
	hSearchAddServerButton->Content = "Add Server";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 90;hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hSearchAddServerButton;
	hSearchDeleteServerButton->Content = "Clear Server";
	Border^ hBorder5 = gcnew Border();
	hBorder5->Width = 90;hBorder5->Height = 30;
	hBorder5->BorderThickness = Thickness(1);
	hBorder5->BorderBrush = Brushes::Black;
	hBorder5->Child = hSearchDeleteServerButton;
	TextBlock^ hSpacer2 = gcnew TextBlock();hSpacer2->Width = 250;
	TextBlock^ hSpacer3 = gcnew TextBlock();hSpacer3->Width = 50;
	hStackPanel4->Children->Add(hBorder2);
	hStackPanel4->Children->Add(hBorder3);
	hStackPanel4->Children->Add(hSpacer2);
	hStackPanel4->Children->Add(hBorder4);
	hStackPanel4->Children->Add(hSpacer3);
	hStackPanel4->Children->Add(hBorder5);
	hStackPanel4->Orientation = Orientation::Horizontal;
	hStackPanel4->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSearchGrid->SetRow(hStackPanel4, 1);
	hSearchGrid->Children->Add(hStackPanel4);
}

//----< set up search path and pattern information >---------------------------------
void WPFCppCliDemo::setSearchPathPatternTextBlockProperties()
{
	RowDefinition^ hRowDef = gcnew RowDefinition();
	hRowDef->Height = GridLength(40);
	hSearchGrid->RowDefinitions->Add(hRowDef);
	Border^ hBorder = gcnew Border();
	hBorder->Width = 90;hBorder->Height = 25;hBorder->BorderThickness = Thickness(0);
	hBorder->Child = hSearchTextBlockPath;
	hSearchTextBlockPath->Text = "Path:";
	hSearchTextBlockPath->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockPath->FontWeight = FontWeights::Bold;hSearchTextBlockPath->FontSize = 16;
	Border^ hBorder1 = gcnew Border(); // des IP text box
	hBorder1->Width = 90;hBorder1->Height = 25;hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;hBorder1->Child = hSearchTextBoxPath;
	hSearchTextBoxPath->Text = "ServerRepository/";
	hSearchTextBoxPath->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBoxPath->FontWeight = FontWeights::Bold;hSearchTextBoxPath->FontSize = 14;
	Border^ hBorder2 = gcnew Border(); // client port text block
	hBorder2->Width = 90;hBorder2->Height = 25;hBorder2->BorderThickness = Thickness(0);
	hBorder2->Child = hSearchTextBlockPattern;
	hSearchTextBlockPattern->Text = "Pattern:";
	hSearchTextBlockPattern->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockPattern->FontWeight = FontWeights::Bold;hSearchTextBlockPattern->FontSize = 16;
	Border^ hBorder3 = gcnew Border(); // client port text box
	hBorder3->Width = 90;hBorder3->Height = 25;hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;hBorder3->Child = hSearchTextBoxDesPattern;
	hSearchTextBoxDesPattern->Text = "*.*";
	hSearchTextBoxDesPattern->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBoxDesPattern->FontWeight = FontWeights::Bold;hSearchTextBoxDesPattern->FontSize = 14;
	hSearchAddPatternrButton->Content = "Add Pattern";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 90;hBorder4->Height = 30;hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;hBorder4->Child = hSearchAddPatternrButton;
	hSearchDeletePatternButton->Content = "Clear Pattern";
	Border^ hBorder5 = gcnew Border();
	hBorder5->Width = 90;hBorder5->Height = 30;hBorder5->BorderThickness = Thickness(1);
	hBorder5->BorderBrush = Brushes::Black;hBorder5->Child = hSearchDeletePatternButton;
	TextBlock^ hSpacer = gcnew TextBlock(); hSpacer->Width = 20;
	TextBlock^ hSpacer2 = gcnew TextBlock();hSpacer2->Width = 50;
	TextBlock^ hSpacer3 = gcnew TextBlock();hSpacer3->Width = 50;
	hStackPanel7->Children->Add(hBorder);hStackPanel7->Children->Add(hBorder1);hStackPanel7->Children->Add(hSpacer);
	hStackPanel7->Children->Add(hBorder2);hStackPanel7->Children->Add(hBorder3);hStackPanel7->Children->Add(hSpacer2);
	hStackPanel7->Children->Add(hBorder4);hStackPanel7->Children->Add(hSpacer3);hStackPanel7->Children->Add(hBorder5);
	hStackPanel7->Orientation = Orientation::Horizontal;
	hStackPanel7->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSearchGrid->SetRow(hStackPanel7, 2);
	hSearchGrid->Children->Add(hStackPanel7);
}

//----< set Upload TextBlock Properties >----------------------------------------
void WPFCppCliDemo::setUploadTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);

	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;

	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 16;

	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;

	hSendMessageGrid->SetRow(hScrollViewer1, 2);
	hSendMessageGrid->Children->Add(hScrollViewer1);
}

//----< set Download TextBlock Properties >----------------------------------------
void WPFCppCliDemo::setDownloadTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hDownloadMessageGrid->RowDefinitions->Add(hRow1Def);

	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownloadResultBlock;

	hDownloadResultBlock->Padding = Thickness(15);
	hDownloadResultBlock->Text = "";
	hDownloadResultBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadResultBlock->FontWeight = FontWeights::Bold;
	hDownloadResultBlock->FontSize = 16;

	hScrollViewer4->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer4->Content = hBorder1;

	hDownloadMessageGrid->SetRow(hScrollViewer4, 4);
	hDownloadMessageGrid->Children->Add(hScrollViewer4);
}

//----< set Search Server Patt Info TextBlock Properties >------------------------------
void WPFCppCliDemo::setSearchServerPattInfoTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(140);
	hSearchGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 240;hBorder1->Height = 120;
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hServerInfoBlock;
	hServerInfoBlock->Padding = Thickness(15);
	hServerInfoBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hServerInfoBlock->FontWeight = FontWeights::Bold;
	hServerInfoBlock->FontSize = 16;
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 240;hBorder2->Height = 120;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hPatternInfoBlock;
	hPatternInfoBlock->Padding = Thickness(15);
	hPatternInfoBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hPatternInfoBlock->FontWeight = FontWeights::Bold;
	hPatternInfoBlock->FontSize = 16;
	Border^ hBorder3 = gcnew Border();
	hBorder3->BorderThickness = Thickness(0);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hSearchTextBlockServerInfo;
	hSearchTextBlockServerInfo->Padding = Thickness(15);
	hSearchTextBlockServerInfo->Text = "<Server Info>";
	hSearchTextBlockServerInfo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockServerInfo->FontWeight = FontWeights::Bold;
	hSearchTextBlockServerInfo->FontSize = 10;
	Border^ hBorder4 = gcnew Border();
	hBorder4->BorderThickness = Thickness(0);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hSearchTextBlockPatternInfo;
	hSearchTextBlockPatternInfo->Padding = Thickness(15);
	hSearchTextBlockPatternInfo->Text = "<Pattern Info>";
	hSearchTextBlockPatternInfo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchTextBlockPatternInfo->FontWeight = FontWeights::Bold;
	hSearchTextBlockPatternInfo->FontSize = 10;
	TextBlock^ hSpacer = gcnew TextBlock();hSpacer->Width = 10;
	hStackPanel6->Children->Add(hBorder3);hStackPanel6->Children->Add(hBorder1);hStackPanel6->Children->Add(hSpacer);
	hStackPanel6->Children->Add(hBorder4);hStackPanel6->Children->Add(hBorder2);
	hStackPanel6->Orientation = Orientation::Horizontal;
	hStackPanel6->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hSearchGrid->SetRow(hStackPanel6, 3);
	hSearchGrid->Children->Add(hStackPanel6);
}

//----< set Download Server File Info ListBox Properties >---------------------------------
void WPFCppCliDemo::setDownloadServerFileInfoListBoxProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(120);
	hDownloadMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hServerFileListBlock;
	hServerFileListBlock->Padding = Thickness(15);
	hServerFileListBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hServerFileListBlock->FontWeight = FontWeights::Bold;
	hServerFileListBlock->FontSize = 16;
	
	TextBlock^ hSpacer = gcnew TextBlock(); hSpacer->Width = 10;

	hScrollViewer5->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer5->Content = hBorder1;

	hDownloadMessageGrid->SetRow(hScrollViewer5, 2);
	hDownloadMessageGrid->Children->Add(hScrollViewer5);

}

//----< set Blank Line Properties >----------------------------------------
void WPFCppCliDemo::setBlankLineProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(10);
	hDownloadMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hBlankLine;
	hBlankLine->Padding = Thickness(15);
	hBlankLine->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hBlankLine->FontWeight = FontWeights::Bold;
	hBlankLine->FontSize = 16;

	
	hStackPanel10->Children->Add(hBorder1);

	hDownloadMessageGrid->SetRow(hStackPanel10, 3);
	hDownloadMessageGrid->Children->Add(hStackPanel10);
}

//----< set Search Return Result TextBlock Properties >-----------------------------
void WPFCppCliDemo::setSearchReturnResultTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSearchGrid->RowDefinitions->Add(hRow1Def);

	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hSearchBlock;

	hSearchBlock->Padding = Thickness(15);
	hSearchBlock->Text = "";
	hSearchBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hSearchBlock->FontWeight = FontWeights::Bold;
	hSearchBlock->FontSize = 16;

	hScrollViewer3->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer3->Content = hBorder1;

	hSearchGrid->SetRow(hScrollViewer3, 4);
	hSearchGrid->Children->Add(hScrollViewer3);
}

//----< set Upload Buttons Properties >----------------------------------------
void WPFCppCliDemo::setUploadDownloadButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);

	hUploadButton->Content = "Upload File";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hUploadButton;

	hClearButton->Content = "Clear";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearButton;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 100;

	hStackPanel1->Children->Add(hBorder2);
	hStackPanel1->Children->Add(hSpacer);

	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSendMessageGrid->SetRow(hStackPanel1, 3);
	hSendMessageGrid->Children->Add(hStackPanel1);
}

//----< set Download Buttons Properties >----------------------------------------
void WPFCppCliDemo::setDownloadButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hDownloadMessageGrid->RowDefinitions->Add(hRow2Def);

	hDownloadClearButton->Content = "Clear";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadClearButton;

	hDownloadButton->Content = "Download File";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hDownloadButton;

	hDownloadExploringButton->Content = "Exploring File";
	Border^ hBorder5 = gcnew Border();
	hBorder5->Width = 90; hBorder5->Height = 30; hBorder5->BorderThickness = Thickness(1);
	hBorder5->BorderBrush = Brushes::Black; hBorder5->Child = hDownloadExploringButton;
	TextBlock^ hSpacer3 = gcnew TextBlock(); hSpacer3->Width = 100;

	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 100;
	hStackPanel11->Children->Add(hBorder5);
	hStackPanel11->Children->Add(hSpacer3);
	hStackPanel11->Children->Add(hBorder4);
	hStackPanel11->Children->Add(hSpacer2);
	hStackPanel11->Children->Add(hBorder3);

	hStackPanel11->Orientation = Orientation::Horizontal;
	hStackPanel11->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadMessageGrid->SetRow(hStackPanel11, 5);
	hDownloadMessageGrid->Children->Add(hStackPanel11);
}

//----< set Search File/Text Buttons Properties >----------------------------------------
void WPFCppCliDemo::setSearchFileTextButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(50);
	hSearchGrid->RowDefinitions->Add(hRow2Def);

	hSearchFileButton->Content = "Search File";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hSearchFileButton;

	hSearchClearResultButton->Content = "Clear Result";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hSearchClearResultButton;

	hSearchTextButton->Content = "Search Text";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hSearchTextButton;

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 100;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 100;

	hStackPanel5->Children->Add(hBorder2);
	hStackPanel5->Children->Add(hSpacer);
	hStackPanel5->Children->Add(hBorder4);
	hStackPanel5->Children->Add(hSpacer2);
	hStackPanel5->Children->Add(hBorder3);
	hStackPanel5->Orientation = Orientation::Horizontal;
	hStackPanel5->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSearchGrid->SetRow(hStackPanel5, 5);
	hSearchGrid->Children->Add(hStackPanel5);
}

//----< set Up Send Message View >----------------------------------------
void WPFCppCliDemo::setUpSendMessageView()
{
	Console::Write("\n  setting up sendMessage view");
	hSendMessageGrid->Margin = Thickness(20);
	hUploadDownloadFileTab->Content = hSendMessageGrid;

	setUploadDesTextBlockProperties();
	setUploadFileNameTextBlockProperties();
	setUploadTextBlockProperties();
	setUploadDownloadButtonsProperties();
}

//----< set Search File Text View >----------------------------------------
void WPFCppCliDemo::setSearchFileTextView()
{
	Console::Write("\n  setting up SearchMessage view");
	hSearchGrid->Margin = Thickness(20);
	hSearchFileTextTab->Content = hSearchGrid;

	setSearchDesTextBlockProperties();
	setSearchFileNameTextBlockProperties();
	setSearchPathPatternTextBlockProperties();
	setSearchServerPattInfoTextBlockProperties();
	setSearchReturnResultTextBlockProperties();
	setSearchFileTextButtonsProperties();
}

//----< set Download File View >----------------------------------------
void WPFCppCliDemo::setDownloadFileView()
{
	Console::Write("\n  setting up SearchMessage view");
	hDownloadMessageGrid->Margin = Thickness(20);
	hDownloadFileTab->Content = hDownloadMessageGrid;

	setDownloadDesTextBlockProperties();
	setDownloadFileNameTextBlockProperties();
	setDownloadServerFileInfoListBoxProperties();
	setBlankLineProperties();
	setDownloadTextBlockProperties();
	setDownloadButtonsProperties();
}

//----< convert to Std String >----------------------------------------
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

//----< send Upload Message >----------------------------------------
void WPFCppCliDemo::sendUploadMessage(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  Upload File");
	hStatus->Text = "Upload File";

	std::string DesIp = toStdString(hTextBoxDesPort->Text);
	std::string DesPort = toStdString(hTextBoxDesIP->Text);
	std::string FileName = toStdString(hTextBoxFileName->Text);
	std::string ClientPort = toStdString(hTextBoxFileClientPort->Text);
	std::string Command = "UPLOAD";
	pSendr_->postMessage(DesIp, DesPort, ClientPort, FileName,Command);
	hTextBlock1->Text += toSystemString(pSendr_->getfilesearchresult());

}

//----< send Download Message >----------------------------------------
void WPFCppCliDemo::sendDownloadMessage(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  Download File");
	hStatus->Text = "Download File";
	std::string DesIp = toStdString(hDownloadTextBoxDesPort->Text);
	std::string DesPort = toStdString(hDownloadTextBoxDesIP->Text);
	std::string FileName = toStdString(hDownloadTextBoxFileName->Text);
	std::string ClientPort = toStdString(hDownloadTextBoxFileClientPort->Text);
	std::string Command = "DOWNLOAD";
	pSendr_->postMessage(DesIp, DesPort, ClientPort, FileName,Command);
	hDownloadResultBlock->Text += toSystemString(pSendr_->getfilesearchresult());
}

//----< send Exploring Message >----------------------------------------
void WPFCppCliDemo::sendExploringMessage(Object^ obj, RoutedEventArgs^ args)
{
	hServerFileListBlock->Text = "";
	Console::Write("\n  Exploring File");
	hStatus->Text = "Exploring File";
	std::string DesIp = toStdString(hDownloadTextBoxDesPort->Text);
	std::string DesPort = toStdString(hDownloadTextBoxDesIP->Text);
	std::string ClientPort = toStdString(hDownloadTextBlockClientPort->Text);
	std::string Command = "SEARCHFILE";
	std::string Path = "ServerRepository/";
	std::string XML = "NOXML";
	std::string serverinfo = DesIp + ":" + DesPort;

	std::vector<std::string> servervec;
	servervec.push_back(serverinfo);

	std::vector<std::string> patternvec;
	patternvec.push_back("*.*");

	pSendr_->postSearchFileMessage(ClientPort, Path, patternvec, servervec, Command, XML);
	hServerFileListBlock->Text = toSystemString(pSendr_->getfilesearchresult());

}

//----< search Add Server >----------------------------------------
void WPFCppCliDemo::searchAddServer(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Add Server";
	//std::string serverinfo = toStdString(hSearchTextBoxDesIP->Text) + ":" + toStdString(hSearchTextBoxDesPort->Text);
	hServerInfoBlock->Items->Insert(0, hSearchTextBoxDesIP->Text +":"+ hSearchTextBoxDesPort->Text);
	vServerInfo->Add(hSearchTextBoxDesIP->Text + ":" + hSearchTextBoxDesPort->Text);
}

//----< search Delete Server >----------------------------------------
void WPFCppCliDemo::searchDeleteServer(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared Server");
	hStatus->Text = "Cleared Server";
	vServerInfo->Clear();
	hServerInfoBlock->Items->Clear();
}

//----< search Add Pattern >----------------------------------------
void WPFCppCliDemo::searchAddPattern(Object^ obj, RoutedEventArgs^ args)
{
	hStatus->Text = "Add Pattern";
	hPatternInfoBlock->Items->Insert(0, hSearchTextBoxDesPattern->Text);
	vPatternInfo->Add(hSearchTextBoxDesPattern->Text);

}

//----< search Delete Pattern >----------------------------------------
void WPFCppCliDemo::searchDeletePattern(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared Pattern");
	hStatus->Text = "Cleared Pattern";
	vPatternInfo->Clear();
	hPatternInfoBlock->Items->Clear();
}

//----< send Search File Message >----------------------------------------
void  WPFCppCliDemo::sendSearchFileMessage(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  Search File");
	hStatus->Text = "Search File";
	std::string DesIp = toStdString(hSearchTextBoxDesIP->Text);
	std::string DesPort = toStdString(hSearchTextBoxDesPort->Text);
	//std::string FileName = toStdString(hTextBoxFileName->Text);
	std::string ClientPort = toStdString(hSearchTextBoxFileClientPort->Text);
	std::string Command = "SEARCHFILE";
	std::string Path = toStdString(hSearchTextBoxPath->Text);
	std::string XML = "NOXML";
	//hXMLString
	if (hXMLString->IsChecked)
	{
		XML = "RETURNXML";
	}
//////////////////////////////////////////////////
	std::vector<std::string> servervec;
	int size2 = vServerInfo->Count;
	for (int count2 = 0; count2 < size2; count2++)
	{
		servervec.push_back(toStdString(vServerInfo[count2]));
	}
//////////////////////////////////////////////////
	std::vector<std::string> patternvec;
	int size=vPatternInfo->Count;
	int flag = 0;
	for (int count = 0; count < size; count++)
	{
		patternvec.push_back(toStdString(vPatternInfo[count]));
		if (patternvec[count]=="*.*")
			flag = 1;
	}
	if (flag==1)
	{
		patternvec.clear();
		patternvec.push_back("*.*");
	}
///////////////////////////////////////////////////

	pSendr_->postSearchFileMessage(ClientPort, Path, patternvec, servervec, Command , XML);
	hSearchBlock->Text += toSystemString(pSendr_->getfilesearchresult());
}

//----< send Search Text Message >----------------------------------------
void  WPFCppCliDemo::sendSearchTextMessage(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  Search Text");
	hStatus->Text = "Search Text";
	std::string DesIp = toStdString(hSearchTextBoxDesIP->Text);
	std::string DesPort = toStdString(hSearchTextBoxDesPort->Text);
	//std::string FileName = toStdString(hTextBoxFileName->Text);
	std::string ClientPort = toStdString(hSearchTextBoxFileClientPort->Text);
	std::string Command = "SEARCHTEXT";
	std::string Path = toStdString(hSearchTextBoxPath->Text);
	std::string Text = toStdString(hSearchTextBoxFileString->Text);
	std::string XML = "NOXML";
	//hXMLString
	if (hXMLString->IsChecked)
	{
		XML = "RETURNXML";
	}
	//////////////////////////////////////////////////
	std::vector<std::string> servervec;
	int size2 = vServerInfo->Count;
	for (int count2 = 0; count2 < size2; count2++)
	{
		servervec.push_back(toStdString(vServerInfo[count2]));
	}
	//////////////////////////////////////////////////
	std::vector<std::string> patternvec;
	int size = vPatternInfo->Count;
	int flag = 0;
	for (int count = 0; count < size; count++)
	{
		patternvec.push_back(toStdString(vPatternInfo[count]));
		if (patternvec[count] == "*.*")
			flag = 1;
	}
	if (flag == 1)
	{
		patternvec.clear();
		patternvec.push_back("*.*");
	}
	///////////////////////////////////////////////////

	pSendr_->postSearchTextMessage(ClientPort, Path, patternvec, Text, servervec, Command, XML);
	hSearchBlock->Text += toSystemString(pSendr_->getfilesearchresult());

}

//----< convert to System String >----------------------------------------
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

//----< add Text >----------------------------------------
void WPFCppCliDemo::addText(String^ msg)
{
	hTextBlock1->Text += msg + "\n\n";
}

//----< get Message >----------------------------------------
void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function

	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		std::string msg = pRecvr_->getMessage();
		String^ sMsg = toSystemString(msg);
		
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;
		
		Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
		Dispatcher->Invoke(act, args);  // must call addText on main UI thread
	}
}

//----< clear >----------------------------------------
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleare Result");
	hStatus->Text = "Clear Result";
	hTextBlock1->Text = "";
}

//----< clear Download Info >----------------------------------------
void WPFCppCliDemo::clearDownloadInfo(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleare Result");
	hStatus->Text = "Clear Result";
	hDownloadResultBlock->Text = "";
}

//----< search Clear Result >----------------------------------------
void WPFCppCliDemo::searchClearResult(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  cleare Result");
	hStatus->Text = "Clear Result";
	hSearchBlock->Text = "";
}

//----< OnLoaded >----------------------------------------
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}

//----< Unloading >----------------------------------------
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppClient");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}