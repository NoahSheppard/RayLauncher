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

using json = nlohmann::json;

const wxColour darkPurple = wxColour(113, 96, 232, 100);
const wxColour lightPurple = wxColour(0xa679dc);
const wxColour lightColour = wxColour(0x202020);
const wxColour darkColour = wxColour(0x0a0a0a);
wxPanel* rightPanel;
wxBoxSizer* rightSizer;

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
            //tbd
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
		std::string webRequest = Web::webRequest("https://httpbin.org/ip", "", Web::RequestType::GET);
		nlohmann::json jsonObj = nlohmann::json::parse(webRequest);
        wxLogStatus("Test: " + (wxString)((std::string)jsonObj["origin"]) + " Should be your ip");
    }

    else if (page == "Friends List") { // will need scrolling code
        // List(Accounts)
        // Load Friends List Button
        //\n
        // Display Name Dialogue
        // Send Request Button
        // Search Dialogue (will need event)
        JSON::AddAccountToFile("AccountID1", "AccountUsername1", "DeviceID", "DeviceSecret", JSON::File::ACCOUNTS);
		nlohmann::json jsonObj = JSON::GetAccountInformation("AccountID", JSON::File::ACCOUNTS);
		wxLogStatus("Test: " + (wxString)((std::string)jsonObj["username"]) + " Should be AccountUsername");
    }

    else if (page == "Exchange Code") {
        // List(Accounts)
        // Logic Button
        //const std::string basicToken = "YWY0M2RjNzFkZDkxNDUyMzk2ZmNkZmZiZDdhOGU4YTk6NFlYdlNFQkxGUlBMaDFoekdaQWtmT2k1bXF1cEZvaFo=";
        // //
        //const std::string clientId = "af43dc71dd91452396fcdffbd7a8e8a9";
        //std::string jsonBody = "{ 'Authorization': 'basic ' " + basicToken + ", 'Content-Type': 'application/x-www-form-urlencoded' }";
        //std::string code = "43176d14ee4a4a4788da3cc7051efeee";
        //std::string firstRequest = Web::webRequest("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/token", code, Web::RequestType::POST);
        std::string firstRequest = Web::GetInitialInfo("17429e7677b746fd882fdc831dbdad1f");
        wxLogStatus("Request: " + (wxString)firstRequest);
        JSON::AddAccountToFile("Error", firstRequest, "", "", JSON::File::ACCOUNTS);
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
    std::string response = Web::webRequest("localhost:80/post", "", Web::RequestType::POST);
    nlohmann::json jsonObj = nlohmann::json::parse(response);
    //std::cout << (std::string)jsonObj["origin"] << std::endl;
    wxLogStatus("Logic: " + (wxString)((std::string)jsonObj["origin"])); // logic placeholder
}