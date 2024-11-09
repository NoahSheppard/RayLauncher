#pragma once 
#include <vector>
#include <string>
#include <wx/wx.h>
#include <wx/settings.h>
#include <wx/frame.h>
#include <wx/hyperlink.h> 
class RayUtils
{
public:
	static enum Window 
	{ MAIN=560, ACCOUNTS=420, CREDITS, SETTINGS};
	static std::vector<std::string> split(const std::string& str, const std::string& delim);
	static wxStaticText* CreateCenteredText(wxPanel* panel, int id, std::string text, int y_height, int textSize, bool bold, Window window);
	static wxStaticText* CreateText(wxPanel* panel, int id, std::string text, wxPoint point, int textSize, bool bold);
	static wxSize GetTextSize(wxStaticText* textObj, int size, bool bold);
	static wxSize GetTextSize(wxHyperlinkCtrl* textObj, int size, bool bold);
	//static wxHyperlinkCtrl* CreateHyperlink(wxPanel* panel, int id, wxString text, wxString link, wxPoint point, int textSize);
	static wxHyperlinkCtrl* CreateHyperlink(wxPanel* panel, int id, wxString text, wxString link, wxPoint point, int textSize);
	static wxHyperlinkCtrl* CreateCenteredLink(wxPanel* panel, int id, wxString text, wxString link, int y_height, int textSize);

};

