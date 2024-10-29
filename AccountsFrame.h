#include <wx/wx.h>
#include <wx/settings.h>
#include <wx/frame.h>
#pragma once
class AccountsFrame : public wxFrame
{
public: 
	AccountsFrame(const wxString& title);
private:
    wxPanel* rightPanel;
    void OnButtonClicked(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    void LoadPageContent(std::string page);
    void OnCloseWindow(wxCloseEvent& event);
    //std::vector<std::string> split(const std::string& str, const std::string& delim);
    
    void Logic(int id);

};

