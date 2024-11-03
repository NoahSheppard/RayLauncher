#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/settings.h>
#include <wx/image.h> 

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	wxInitAllImageHandlers();
	wxImage::AddHandler(new wxJPEGHandler());
	MainFrame* mainFrame = new MainFrame("RayLauncher");
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}