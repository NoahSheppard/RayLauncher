#include "AccountsFrame.h"
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/event.h> 
#include <wx/settings.h>
#include <wx/hyperlink.h>
#include "RoundedButton.h"
#include <list>
#include "RayUtils.h"
#include "Web.h"
#include "MainFrame.h"
#include <string>

const std::list<std::string> pagesMap = {
    "Home", // 99
    "Exchange Code", // 0
};
const std::list<std::string> buttonMap = {
    "1_1",
    "1_2",
    "1_3",
    "1_4",
    "1_5",
};

const wxColour darkPurple = wxColour(113, 96, 232, 100);
const wxColour lightPurple = wxColour(0xa679dc);
const wxColour lightColour = wxColour(0x202020);
const wxColour darkColour = wxColour(0x0a0a0a);
wxTextCtrl* exchangeCodeInput;
//wxPanel* rightPanel;
//wxBoxSizer* rightSizer;



AccountsFrame::AccountsFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Top
    SetBackgroundColour(lightColour);
    SetSizer(mainSizer);

    wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Left
    wxPanel* leftPanel = new wxPanel(this, wxID_ANY); // creates the panel
    leftPanel->SetBackgroundColour(lightColour); // sets colour
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL); // creates sizer
    int leftButtonIndex = 0;
    for (std::string page : pagesMap) {
        RoundedButton* _l_button = new RoundedButton(leftPanel, wxID_ANY, page, wxPoint(5, (5 + (30 * leftButtonIndex))), wxSize(170, 25), darkPurple);
        _l_button->Bind(wxEVT_BUTTON, &AccountsFrame::OnButtonClicked, this);
        leftButtonIndex++;
    } // Left panel buttons
    leftPanel->SetSizer(leftSizer); // sets sizer

    // Right Panel 
    rightPanel = new wxPanel(this, wxID_ANY); // makes right panel
    rightPanel->SetBackgroundColour(darkColour); // sets colour
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL); // makes sizer
    rightPanel->SetSizer(rightSizer); // sets sizer

    // Add panels to horizontal sizer with proportion
    horizontalSizer->Add(leftPanel, 30, wxEXPAND); // L
    horizontalSizer->Add(rightPanel, 70, wxEXPAND); // R
    mainSizer->Add(horizontalSizer, 1, wxEXPAND); // T
    SetSizer(mainSizer); // sets main sizer
    CreateStatusBar(); // debug bottom bar

    Bind(wxEVT_CLOSE_WINDOW, &AccountsFrame::OnCloseWindow, this);
}

// # ------------------------------------------------------------------------------------------ Events

void AccountsFrame::OnButtonClicked(wxCommandEvent& event) {
    wxButton* button = dynamic_cast<wxButton*>(event.GetEventObject());
    if (button) {}                                                                                                  // no reg buttons used, this should never be called
    else {                                                                                                          // the only 2 things that call MainFrame::OnButtonClicked is wxButton and RoundedButton
        RoundedButton* button = dynamic_cast<RoundedButton*>(event.GetEventObject());
        wxString buttonText = button->GetNewLabel();
        if (std::find(pagesMap.begin(), pagesMap.end(), buttonText.ToStdString()) != pagesMap.end()) {              // if in pagesMap list
            rightPanel->DestroyChildren();
            LoadPageContent(buttonText.ToStdString());
        }
        else {                                                                                                      // only possible to be called if: is inside of rightpanel/is logic, or I fucked up somewhere
            for (std::string button_id : buttonMap) {
                wxLogStatus((wxString) button_id);
                std::vector<std::string> button_id_split = RayUtils::split(button_id, "_");
                std::string __button_id = ((std::string)button_id_split[0] + (std::string)button_id_split[1]);      // turn button id from x_y to xy (i <3 c++)
                if (button->GetId() == std::stoi(__button_id)) {
                    Logic(std::stoi(__button_id));
                    break;
                }
            }
        }
    }
}

void AccountsFrame::OnTextChanged(wxCommandEvent& event) {
    wxTextCtrl* text = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
    if (text) {}
}

void AccountsFrame::OnCloseWindow(wxCloseEvent& event) {
    MainFrame::WindowClosed(MainFrame::WINDOWS::ACCOUNTS);
    Destroy();
}

// # ------------------------------------------------------------------------------------------ Logic

void AccountsFrame::LoadPageContent(std::string page) {
    if (page == "Home") {
        wxStaticText* accountName = new wxStaticText(rightPanel, wxID_ANY, "RayLauncher - Accounts", wxPoint(0, 5 + 35 + 35), wxDefaultSize);
        wxFont font = wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        accountName->SetForegroundColour(wxColour(0xffffff));
        accountName->SetFont(font);

        wxClientDC dc(accountName);
        dc.SetFont(font);
        wxSize accountName_width = dc.GetTextExtent(accountName->GetLabelText());

        accountName->SetPosition(wxPoint((420 / 2) - ((accountName_width.GetWidth()) / 2), 20));
    }
    else if (page == "Exchange Code") {
		wxStaticText* exchangeCode = RayUtils::CreateCenteredText(rightPanel, 11, "Exchange Code", 20, 20, true);
		wxStaticText* exchangeCodeDesc = RayUtils::CreateCenteredText(rightPanel, 12, "Enter the code you received from the link", 60, 12, false);
		exchangeCodeInput = new wxTextCtrl(rightPanel, 13, "", wxPoint(120, 103), wxSize(290, 20));
        wxHyperlinkCtrl* exchangeCodeLink = RayUtils::CreateHyperlink(rightPanel, 14, "Get Code", "https://www.epicgames.com/id/api/redirect?clientId=af43dc71dd91452396fcdffbd7a8e8a9&responseType=code", wxPoint(10, 100), 16);
        Bind(wxEVT_TEXT, &AccountsFrame::OnTextChanged, this);
        RoundedButton* exchangeCodeButton = new RoundedButton(rightPanel, 15, "Login", wxPoint(120, 130), wxSize(290, 25), darkPurple);
        exchangeCodeButton->Bind(wxEVT_BUTTON, &AccountsFrame::OnButtonClicked, this);
    }
    else {}
}

void AccountsFrame::Logic(int id) {
    if (id == 15) {
        Web::Login((std::string)exchangeCodeInput->GetValue());
    }
}