#pragma once
#include <wx/wx.h>

class RoundedButton;

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
	static bool accountsWindowOpen;
	static bool creditsWindowOpen;
	static bool settingsWindowOpen;
    enum WINDOWS
    { ACCOUNTS, CREDITS, SETTINGS };
    static void WindowClosed(MainFrame::WINDOWS window);
    std::string SetupResource(std::string fileName, int id);
    static void FillDropdownWithAccounts(wxChoice* dropdown);
    
private:
    void OnButtonClicked(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    void OnPanelPaint(wxPaintEvent& evt);
    void OnCloseWindow(wxCloseEvent& event);

    void ShowCreditsDialog(wxWindow* parent);
    void AddItemToScroller(wxScrolledWindow* scrolledWindow, RoundedButton* button, wxBoxSizer* scrollSizer);

    void AddItemToScroller(wxScrolledWindow* scrolledWindow, wxStaticText* text, wxBoxSizer* scrollSizer);

	void LoadPageContent(std::string page);
    void Logic(int id);
    void SetupIcon();
    void SetupFont();

    wxTextCtrl* m_textLeft;
    wxTextCtrl* m_textRight;
    wxSlider* m_sliderLeft;
    wxSlider* m_sliderRight;
    wxImage m_image;
    wxWindow* WINDOW;

    int m_blurRadius = 5;
    int m_blurOpacity = 255;  // 0-255

};