#include "MainFrame.h"
#include <wx/wx.h>
#include "RoundedButton.h"
#include <list>
#include <map>
#include <fstream>
#include "json.hpp"
#include "RayUtils.h"
#include "JSON.h"
#include "Web.h"
#include <sstream>
#include "AccountsWindow.h"
#include <wx/icon.h>
#include <windows.h>

using json = nlohmann::json;

const wxColour darkPurple = wxColour(113, 96, 232, 100);
const wxColour lightPurple = wxColour(0xa679dc);
const wxColour lightColour = wxColour(0x202020);
const wxColour darkColour = wxColour(0x0a0a0a);
wxPanel* rightPanel;
wxBoxSizer* rightSizer;

bool MainFrame::accountsWindowOpen = false;
bool MainFrame::creditsWindowOpen = false;
bool MainFrame::settingsWindowOpen = false;

const std::list<std::string> pagesMap = {
    "Home", // 99
	"Account ID Lookup", // 0
    "Account Settings", // 1
	"Device Auth", // 2
    "Friends List", // 3
    "Exchange Code", // 4
    "Invite Players", // 5
    "Launch Game", // 6
    "Research Lab", // 7
    "Strom Shields", // 8
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
    
    // Top
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

    // Left
    wxPanel* leftPanel = new wxPanel(this, wxID_ANY); // creates the panel
    leftPanel->SetBackgroundColour(lightColour); // sets colour
	wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL); // creates sizer
    int leftButtonIndex = 0;
    for (std::string page : pagesMap) {
        RoundedButton* _l_button = new RoundedButton(leftPanel, wxID_ANY, page, wxPoint(5, (5 + (30 * leftButtonIndex))), wxSize(230,25), darkPurple);
        _l_button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        leftButtonIndex++;
    } // Left panel buttons
	leftPanel->SetSizer(leftSizer); // sets sizer

    // Right Panel 
    rightPanel = new wxPanel(this, wxID_ANY); // makes right panel
    rightPanel->SetBackgroundColour(darkColour); // sets colour
	rightSizer = new wxBoxSizer(wxVERTICAL); // makes sizer
	rightPanel->SetSizer(rightSizer); // sets sizer

    // Add panels to horizontal sizer with proportion
    horizontalSizer->Add(leftPanel, 30, wxEXPAND); // L
    horizontalSizer->Add(rightPanel, 70, wxEXPAND); // R
    mainSizer->Add(horizontalSizer, 1, wxEXPAND); // T
	SetSizer(mainSizer); // sets main sizer
    CreateStatusBar(); // debug bottom bar
    SetupIcon();
	LoadPageContent("Home"); // go!!!

}

// # ------------------------------------------------------------------------------------------ Events

void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    wxButton* button = dynamic_cast<wxButton*>(event.GetEventObject());                                             
    if (button) { }                                                                                                 // no reg buttons used, this should never be called
	else {                                                                                                          // the only 2 things that call MainFrame::OnButtonClicked is wxButton and RoundedButton
		RoundedButton* button = dynamic_cast<RoundedButton*>(event.GetEventObject());
        wxString buttonText = button->GetNewLabel();
        if (buttonText == "Accounts" || buttonText == "Credits" || buttonText == "Settings") {
            if (buttonText == "Accounts") {
                if (!accountsWindowOpen) {
                    AccountsWindow::OnInit();
					accountsWindowOpen = true;
                }
            }
			else if (buttonText == "Credits") {
				if (!creditsWindowOpen) {
                    //CreditsWindow::OnInit();
					creditsWindowOpen = true;
				}
			}
			else if (buttonText == "Settings") {
				if (!settingsWindowOpen) {
					//SettingsWindow::OnInit();
					settingsWindowOpen = true;
				}
			}
        }
        else if (std::find(pagesMap.begin(), pagesMap.end(), buttonText.ToStdString()) != pagesMap.end()) {         // if in pagesMap list
            rightPanel->DestroyChildren(); 
            LoadPageContent(buttonText.ToStdString()); 
        }
        else {                                                                                                      // only possible to be called if: is inside of rightpanel/is logic, or I fucked up somewhere
			for (std::string button_id : buttonMap) {
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

void MainFrame::OnTextChanged(wxCommandEvent& event) {
    wxTextCtrl* text = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
    if (text) {
    }
}

void MainFrame::OnPanelPaint(wxPaintEvent& evt)
{
    wxPaintDC dc(rightPanel);

    if (!m_image.IsOk()) return;

    // Get panel dimensions
    int panelWidth = rightPanel->GetSize().GetWidth();
    int panelHeight = rightPanel->GetSize().GetHeight();

    // Calculate scaling factor based on height
    double scale = (double)panelHeight / m_image.GetHeight();
    int newWidth = m_image.GetWidth() * scale;
    int newHeight = panelHeight;

    // Scale the image
    wxImage scaledImage = m_image.Scale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);
    wxBitmap scaledBitmap(scaledImage);

    // Center horizontally
    int x = (panelWidth - newWidth) / 2;
    if (x < 0) x = 0;  // If image is wider than panel, start from left edge

    // Draw the bitmap
    dc.DrawBitmap(scaledBitmap, x, 0, false);
}

// # ------------------------------------------------------------------------------------------ Logic

void MainFrame::LoadPageContent(std::string page) { // change title code but I cba
    if (page == "Home") {
        wxStaticText* accountName = new wxStaticText(rightPanel, wxID_ANY, "RayLauncher", wxPoint(0, 5 + 35 + 35), wxDefaultSize);
		wxFont font = wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        accountName->SetForegroundColour(wxColour(0xffffff));
		accountName->SetFont(font);

        wxClientDC dc(accountName);
        dc.SetFont(font);
        wxSize accountName_width = dc.GetTextExtent(accountName->GetLabelText());
        
        accountName->SetPosition(wxPoint((560/2)-((accountName_width.GetWidth())/2), 20));

        //Image
        /*if (!m_image.LoadFile("RayLauncher.jpg"))
        {
            wxMessageBox("Failed to load image", "Error");
        }

        // Bind events
        rightPanel->Bind(wxEVT_PAINT, &MainFrame::OnPanelPaint, this);
        rightPanel->Bind(wxEVT_SIZE, [this](wxSizeEvent& evt) {
            rightPanel->Refresh();
            evt.Skip();
        });*/
    }

    else if (page == "Account ID Lookup") {
        wxArrayString choices; choices.Add("acc 1"); choices.Add("acc 2"); choices.Add("acc 3");

		wxChoice* accountsDropdown = new wxChoice(rightPanel, wxID_ANY, wxPoint(10, 5), wxSize(225, 25), choices);
		//auto currentChoice = accountsDropdown->GetSelection(); // dont know type yet and I cba to figure it out

		wxTextCtrl* searchInput = new wxTextCtrl(rightPanel, wxID_ANY, "Search", wxPoint(10+225+10, 5), wxSize(305, 25), wxTE_PROCESS_ENTER);
		searchInput->Bind(wxEVT_TEXT, &MainFrame::OnTextChanged, this);

        RoundedButton* logicButton = new RoundedButton(rightPanel, 13, "Lookup", wxPoint((10 + 225 + 10) + (153 - 63), 5 + 35), wxSize(125, 25), darkPurple);
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    }

    else if (page == "Account Settings") {
        wxTextCtrl* searchInput = new wxTextCtrl(rightPanel, wxID_ANY, "Search", wxPoint(10 + 225 + 10, 5), wxSize(305, 25), wxTE_PROCESS_ENTER);
        searchInput->Bind(wxEVT_TEXT, &MainFrame::OnTextChanged, this);
        nlohmann::json data = nlohmann::json::parse(R"({"pi": 3.141,"happy": true})");
        std::string piValue = std::to_string(data["pi"].get<double>());
        std::string happyValue = data["happy"].get<bool>() ? "true" : "false";
        RoundedButton* logicButton = new RoundedButton(rightPanel, 13, happyValue,
        wxPoint((10 + 225 + 10) + (153 - 63), 5 + 35), wxSize(125, 25), darkPurple);
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);

        
    }

    else if (page == "Device Auth") {
    //https://httpbin.org/ip 
		//std::string webRequest = Web::webRequest("https://httpbin.org/ip", "", Web::RequestType::GET);
		//nlohmann::json jsonObj = nlohmann::json::parse(webRequest);
        //wxLogStatus("Test: " + (wxString)((std::string)jsonObj["origin"]) + " Should be your ip");
    }

    else if (page == "Friends List") { // will need scrolling code
        // List(Accounts)
        // Load Friends List Button
        //\n
        // Display Name Dialogue
        // Send Request Button
        // Search Dialogue (will need event)
		wxLogStatus("Works so far!");
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
    //std::string response = Web::webRequest("localhost:80/post", "", Web::RequestType::POST);
    //nlohmann::json jsonObj = nlohmann::json::parse(response);
    //std::cout << (std::string)jsonObj["origin"] << std::endl;
    //wxLogStatus("Logic: " + (wxString)((std::string)jsonObj["origin"])); // logic placeholder
}

void MainFrame::WindowClosed(MainFrame::WINDOWS window) {
	switch (window) {
        case MainFrame::WINDOWS::ACCOUNTS:
		    accountsWindowOpen = false;
		    break;
	    case MainFrame::WINDOWS::CREDITS:
		    creditsWindowOpen = false;
		    break;
	    case MainFrame::WINDOWS::SETTINGS:
		    settingsWindowOpen = false;
		    break;
	    default:
		    break;
	}
}

void MainFrame::SetupIcon()
{
    // Load icon from embedded resources using Windows API
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(107));
    if (hIcon)
    {
        wxIcon icon;
        icon.CreateFromHICON(hIcon);
        if (icon.IsOk())
        {
            SetIcon(icon);
        }
        DestroyIcon(hIcon);
    }
}