#include "MainFrame.h"
#include <wx/wx.h>
#include "RoundedButton.h"
#include <list>
#include <map>

const wxColour darkPurple = wxColour(113, 96, 232, 100);
const wxColour lightPurple = wxColour(0xa679dc);
const wxColour lightColour = wxColour(0x202020);
const wxColour darkColour = wxColour(0x0a0a0a);
wxPanel* rightPanel;
wxBoxSizer* rightSizer;
bool rightHidden = false;
int winid;

const std::list<std::string> pagesMap = {
	"Account ID Lookup",
    "Account Settings",
	"Device Auth",
    "Friends List",
    "Exchange Code",
    "Invite Players",
    "Launch Game",
    "Research Lab",
    "Strom Shields",
};

const std::list<std::string> buttonMap = {
	"1_1", // 0_dropdown
	"1_2", // 0_search
	"1_3", // 0_lookup_button
};

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    winid = (int) this;

    // Create the transparent panel at the top
    wxPanel* topPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW);
    RoundedButton* topButton = new RoundedButton(topPanel, wxID_ANY, "Accounts", wxPoint(0,0), wxSize(70, 30), lightColour);
    topButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    RoundedButton* topButton2 = new RoundedButton(topPanel, wxID_ANY, "Credits", wxPoint(70, 0), wxSize(70, 30), lightColour);
    topButton2->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    RoundedButton* topButton3 = new RoundedButton(topPanel, wxID_ANY, "Settings", wxPoint(140, 0), wxSize(70, 30), lightColour);
    topButton3->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);

    mainSizer->Add(topPanel, 0, wxEXPAND);
	mainSizer->SetItemMinSize(topPanel, -1, 30);

    SetBackgroundColour(lightColour);
    SetSizer(mainSizer);

    wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Left Panel
    wxPanel* leftPanel = new wxPanel(this, wxID_ANY);
    leftPanel->SetBackgroundColour(lightColour);  // Darker blue
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    int leftButtonIndex = 0;
    for (std::string page : pagesMap) {
        RoundedButton* _l_button = new RoundedButton(leftPanel, wxID_ANY, page, wxPoint(5, (5 + (30 * leftButtonIndex))), wxSize(230,25), darkPurple);
        _l_button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        leftButtonIndex++;
    }
    leftPanel->SetSizer(leftSizer);

    // Right Panel 
    rightPanel = new wxPanel(this, wxID_ANY);
    rightPanel->SetBackgroundColour(darkColour);
    rightSizer = new wxBoxSizer(wxVERTICAL);
    rightPanel->SetSizer(rightSizer);

    // Add panels to horizontal sizer with proportion
    horizontalSizer->Add(leftPanel, 30, wxEXPAND);
    horizontalSizer->Add(rightPanel, 70, wxEXPAND);
    mainSizer->Add(horizontalSizer, 1, wxEXPAND);
    SetSizer(mainSizer);
    CreateStatusBar();
}



//Events
void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    wxButton* button = dynamic_cast<wxButton*>(event.GetEventObject());
    if (button) { }
    else {
		RoundedButton* button = dynamic_cast<RoundedButton*>(event.GetEventObject());
        wxString buttonText = button->GetNewLabel();
        if (buttonText == "Accounts" || buttonText == "Credits" || buttonText == "Settings") {}
        else if (std::find(pagesMap.begin(), pagesMap.end(), buttonText.ToStdString()) != pagesMap.end()) {
            rightPanel->DestroyChildren();
            LoadPageContent(buttonText.ToStdString());
        }
        else {
			for (std::string button_id : buttonMap) {
				std::vector<std::string> button_id_split = split(button_id, "_");
                std::string __button_id = ((std::string)button_id_split[0] + (std::string)button_id_split[1]);
				if (button->GetId() == std::stoi(__button_id)) {
                    Logic(std::stoi(__button_id));
                    break;
				}
			}
        }
    }
}

/*void MainFrame::OnSliderChanged(wxCommandEvent& event) {
    wxSlider* slider = dynamic_cast<wxSlider*>(event.GetEventObject());
    if (slider) {
        wxString side = (slider == m_sliderLeft) ? "Left" : "Right";
        wxString str = wxString::Format("%s Slider Value: %d", side, event.GetInt());
        wxLogStatus(str);
    }
} */

void MainFrame::OnTextChanged(wxCommandEvent& event) {
    wxTextCtrl* text = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
    if (text) {
        wxString side = (text == m_textLeft) ? "Left" : "Right";
        wxString str = wxString::Format("%s Text: %s", side, event.GetString());
        wxLogStatus(str);
    }
}






//main code functions
void MainFrame::LoadPageContent(std::string page) { // change title code but I cba
    if (page == "Account ID Lookup") {
        wxArrayString choices;
		choices.Add("acc 1");
		choices.Add("acc 2");
		choices.Add("acc 3");

		wxChoice* accountsDropdown = new wxChoice(rightPanel, wxID_ANY, wxPoint(10, 5), wxSize(225, 25), choices);
       
        //RoundedButton* accountsDropdownPlaceholder = new RoundedButton(rightPanel, wxID_ANY, "Dropdown", wxPoint(10, 5), wxSize(225, 25), darkPurple);
        //accountsDropdownPlaceholder->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);

		wxTextCtrl* searchInput = new wxTextCtrl(rightPanel, wxID_ANY, "Search", wxPoint(10+225+10, 5), wxSize(305, 25), wxTE_PROCESS_ENTER);
		searchInput->Bind(wxEVT_TEXT, &MainFrame::OnTextChanged, this);

        //RoundedButton* searchPlaceholder = new RoundedButton(rightPanel, wxID_ANY, "Search", wxPoint((10+225+10), 5), wxSize(305, 25), darkPurple);
        //searchPlaceholder->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);

        RoundedButton* logicButton = new RoundedButton(rightPanel, 13, "Lookup", wxPoint((10 + 225 + 10) + (153 - 63), 5 + 35), wxSize(125, 25), darkPurple);
		logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    }
    else if (page == "Account Settings") {
        // List(Accounts)
        // Logic Button
    }
    else if (page == "Device Auth") {
        // List(Accounts)
        // Logic Button
    }
    else if (page == "Friends List") { // will need scrolling code
        // List(Accounts)
        // Load Friends List Button
        //\n
        // Display Name Dialogue
        // Send Request Button
        // Search Dialogue (will need event)
    }
    else if (page == "Exchange Code") {
        // List(Accounts)
        // Logic Button
    }
    else if (page == "Invite Players") { // will need scrolling code
        // List(Accounts)
        // Logic Button
        // Search
    }
    else if (page == "Launch Game") {
        // List(Accounts)
        // Logic Button
    }
    else if (page == "Research Lab") {
        // List(Accounts)
        // Logic Button
    }
    else if (page == "Strom Shields") {
        // List(Accounts)
        // Logic Button
    }
    else {
        // Uh oh
    }
}

void MainFrame::Logic(int id) {
    wxLogStatus("Logic: " + wxString::Format("%d", id));
}





//util
std::vector<std::string> MainFrame::split(const std::string& str, const std::string& delim) {
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do {
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}