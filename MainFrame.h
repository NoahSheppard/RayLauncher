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
    void OnTextChanged(wxCommandEvent& event);
    void OnPanelPaint(wxPaintEvent& evt);
    void OnCloseWindow(wxCloseEvent& event);

	void LoadPageContent(std::string page);
    void Logic(int id);
    void SetupIcon();

    wxTextCtrl* m_textLeft;
    wxTextCtrl* m_textRight;
    wxSlider* m_sliderLeft;
    wxSlider* m_sliderRight;
    wxImage m_image;

    int m_blurRadius = 5;
    int m_blurOpacity = 255;  // 0-255

};