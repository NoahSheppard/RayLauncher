#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
	static bool accountsWindowOpen;
	static bool creditsWindowOpen;
	static bool settingsWindowOpen;
    enum WINDOWS
    { ACCOUNTS, CREDITS, SETTINGS };
    static void WindowClosed(MainFrame::WINDOWS window);
    
private:
    void OnButtonClicked(wxCommandEvent& event);
    void OnSliderChanged(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
	void LoadPageContent(std::string page);
    //std::vector<std::string> split(const std::string& str, const std::string& delim);
    void Logic(int id);
    void SetupIcon();
    void OnPanelPaint(wxPaintEvent& evt);

    wxTextCtrl* m_textLeft;
    wxTextCtrl* m_textRight;
    wxSlider* m_sliderLeft;
    wxSlider* m_sliderRight;
    wxImage m_image;
};