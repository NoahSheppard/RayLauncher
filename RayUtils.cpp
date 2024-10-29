#include "RayUtils.h"
#include <wx/hyperlink.h>

std::vector<std::string> RayUtils::split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

wxStaticText* RayUtils::CreateText(wxPanel* panel, int id, std::string text, wxPoint point, int textSize, bool bold) {
	wxStaticText* textObj = new wxStaticText(panel, id, text, point, wxDefaultSize);
	wxSize textObjWidth = GetTextSize(textObj, textSize, bold);
	textObj->SetPosition(point);
	return textObj;
}
wxStaticText* RayUtils::CreateCenteredText(wxPanel* panel, int id, std::string text, int y_height, int textSize, bool bold) {
	wxStaticText* textObj = new wxStaticText(panel, id, text, wxDefaultPosition, wxDefaultSize);
	wxSize textObjWidth = GetTextSize(textObj, textSize, bold);
	textObj->SetPosition(wxPoint((420 / 2) - ((textObjWidth.GetWidth()) / 2), y_height));
	return textObj;
}
wxSize RayUtils::GetTextSize(wxStaticText* textObj, int size, bool bold) {
	wxFont fontObj = wxFont(size, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, bold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_SEMIBOLD);
	textObj->SetForegroundColour(wxColour(0xffffff));
	textObj->SetFont(fontObj);
	wxClientDC dc(textObj);
	dc.SetFont(fontObj);
	wxSize textObjWidth = dc.GetTextExtent(textObj->GetLabelText());
	return textObjWidth;
}
wxSize RayUtils::GetTextSize(wxHyperlinkCtrl* textObj, int size, bool bold) {
	wxSize textObjWidth = GetTextSize((wxStaticText*)textObj, size, bold);
	return GetTextSize((wxStaticText*)textObj, size, bold);
}
wxHyperlinkCtrl* RayUtils::CreateHyperlink(wxPanel* panel, int id, wxString text, wxString link, wxPoint point, int textSize) {
	wxHyperlinkCtrl* linkObj = new wxHyperlinkCtrl(panel, id, text, link, point, wxSize(20, 20));
	linkObj->SetSize(GetTextSize(linkObj, textSize, false));
	return linkObj;
}
wxHyperlinkCtrl* RayUtils::CreateCenteredLink(wxPanel* panel, int id, wxString text, wxString link, int y_height, int textSize) {
	wxHyperlinkCtrl* linkObj = new wxHyperlinkCtrl(panel, id, text, link, wxPoint(0, y_height), wxSize(20, 20));
	wxSize textObjWidth = GetTextSize(linkObj, textSize, false);
	linkObj->SetPosition(wxPoint((420 / 2) - ((textObjWidth.GetWidth()) / 2), y_height));
	linkObj->SetSize(textObjWidth);
	return linkObj;
}