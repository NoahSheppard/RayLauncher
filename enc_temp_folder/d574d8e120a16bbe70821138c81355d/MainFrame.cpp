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
#include "AccountsFrame.h"
#include <wx/file.h>
#include <wx/filename.h>
#include <vector>
#include <wx/clipbrd.h>
/*
libcurl.dll MSVCP140D.dll VCRUNTIME140D.dll VCRUNTIME140_1D.dll
// DLL's that dont work
*/

using json = nlohmann::json;

const wxColour darkPurple = wxColour(113, 96, 232, 100);
const wxColour lightPurple = wxColour(0xa679dc);
const wxColour lightColour = wxColour(0x202020);
const wxColour darkColour = wxColour(0x0a0a0a);
wxPanel* rightPanel;
wxBoxSizer* rightSizer;
wxWindow* WINDOW;
wxBoxSizer* oneScrollSizer;

bool MainFrame::accountsWindowOpen = false;
bool MainFrame::creditsWindowOpen = false;
bool MainFrame::settingsWindowOpen = false;
static bool FontCreated = false;

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
    "1_1", // 1_dropdown
    "1_2", // 1_search
    "1_3", // 1_lookup_button
	"1_4", // 1_scroll_window

    "2_1", // 2_dropdown
    "2_2", // 2_open_settings_button

    "3_1", // 3_dropdown
    "3_2", // 3_device_auth_button

    "5_1", // 5_dropdown
    "5_2", // 5_exchange_code_button

    "7_1", // 7_dropdown
    "7_2", // 7_launch_button

    "8_1", // 8_dropdown
    "8_2", // 8_research_button
};

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize,
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    WINDOW = this;

    // Top
    wxPanel* topPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTRANSPARENT_WINDOW);
    RoundedButton* topButton = new RoundedButton(topPanel, wxID_ANY, "Accounts", wxPoint(0, 0), wxSize(70, 30), lightColour);
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
        RoundedButton* _l_button = new RoundedButton(leftPanel, wxID_ANY, page, wxPoint(5, (5 + (30 * leftButtonIndex))), wxSize(230, 25), darkPurple);
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
    SetupFont();
    LoadPageContent("Home"); // go!!!
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCloseWindow, this);
    //std::ofstream outFile("accounts.json");
    //outFile << "{}" << std::endl; outFile.close();
    // TODO: CHECK IF ACCOUNTS FILE EXISTS FIRST, IF NOT, THEN CREATE IT
    // RETARD

}

// # ------------------------------------------------------------------------------------------ Events

void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    wxButton* button = dynamic_cast<wxButton*>(event.GetEventObject());
    if (button) {}                                                                                                 // no reg buttons used, this should never be called..
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
                    ShowCreditsDialog(WINDOW);
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
                wxLogStatus((wxString)std::to_string(button->GetId()));
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

    if (!m_image.IsOk()){
        wxLogMessage("No valid image to paint");
        return;
    }

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

    // Calculate how much of the image needs to be cropped
    int excessWidth = newWidth - panelWidth;

    // If image is wider than panel, crop equally from both sides
    int sourceX = 0;
    if (excessWidth > 0) {
        sourceX = excessWidth / 2;
    }

    // Get the portion of the image we want to display
    wxBitmap displayBitmap = scaledBitmap.GetSubBitmap(
        wxRect(sourceX, 0, std::min(newWidth, panelWidth), newHeight)
    );

    // Draw the main image
    dc.DrawBitmap(displayBitmap, 0, 0, false);

    // Create a blurred overlay
    wxImage blurOverlay = displayBitmap.ConvertToImage();

    // Apply Gaussian blur
    blurOverlay = blurOverlay.Blur(m_blurRadius);

    // Modify the alpha channel for the entire image
    if (!blurOverlay.HasAlpha()) {
        blurOverlay.InitAlpha();
    }

    unsigned char* alpha = blurOverlay.GetAlpha();
    for (int i = 0; i < panelWidth * panelHeight; i++) {
        alpha[i] = m_blurOpacity;
    }

    // Convert back to bitmap with alpha
    wxBitmap blurredBitmap(blurOverlay);

    // Draw the blurred overlay
    wxMemoryDC memDC;
    memDC.SelectObject(blurredBitmap);
    dc.Blit(0, 0, panelWidth, panelHeight, &memDC, 0, 0, wxCOPY, true);
}

void MainFrame::OnCloseWindow(wxCloseEvent& event) {
    //Destroy the AccountsWindow
    AccountsFrame::DestroyWindow();
    Destroy();
}

// # ------------------------------------------------------------------------------------------ Logic

void MainFrame::LoadPageContent(std::string page) { // change title code but I cba
    if (page == "Home") { // DONE FOR NOW
        wxStaticText* titleText = RayUtils::CreateCenteredText(rightPanel, 01, "RayLauncher", 20, 30, true, RayUtils::Window::MAIN);
        titleText->SetBackgroundColour(wxColour(0x55395d));

        m_image.LoadFile(SetupResource("RayLauncher.jpg", 111));
        rightPanel->Bind(wxEVT_PAINT, &MainFrame::OnPanelPaint, this);
        rightPanel->Bind(wxEVT_SIZE, [this](wxSizeEvent& evt) {
            rightPanel->Refresh();
            evt.Skip();
            });
    }

    else if (page == "Account ID Lookup") {
        wxChoice* accountsDropdown = new wxChoice(rightPanel, 11, wxPoint(10, 5), wxSize(225, 25));
        MainFrame::FillDropdownWithAccounts(accountsDropdown);

        wxTextCtrl* searchInput = new wxTextCtrl(rightPanel, 12, "Search", wxPoint(10 + 225 + 10, 5), wxSize(305, 25), wxTE_PROCESS_ENTER);
        searchInput->Bind(wxEVT_TEXT, &MainFrame::OnTextChanged, this);

        RoundedButton* logicButton = new RoundedButton(rightPanel, 13, "Lookup", wxPoint((10 + 225 + 10) + (153 - 63), 5 + 35), wxSize(125, 25), darkPurple);
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);

        wxScrolledWindow* scrolledWindow = new wxScrolledWindow(rightPanel, 14, wxPoint(10, 100), wxSize(540, 380), wxBG_STYLE_TRANSPARENT);
        //scrolledWindow->SetBackgroundColour(wxTransparentColour);
        oneScrollSizer = new wxBoxSizer(wxVERTICAL);
        scrolledWindow->SetBackgroundColour(lightColour);
        scrolledWindow->Hide();
    }

    else if (page == "Account Settings") { // DONE
        wxChoice* accountsDropdown = new wxChoice(rightPanel, 21, wxPoint((560 / 2) - (225 / 2), 10), wxSize(225, 25));
        MainFrame::FillDropdownWithAccounts(accountsDropdown);
        RoundedButton* logicButton = new RoundedButton(rightPanel, 22, "Open Settings", wxPoint((560 / 2) - (125 / 2), 5 + 35), wxSize(125, 25), darkPurple); // (rightPanel / 2) - (buttonWidth / 2)
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    }

    else if (page == "Device Auth") { // BACK/SCROLL
        wxChoice* accountsDropdown = new wxChoice(rightPanel, 31, wxPoint((560 / 2) - (225 / 2), 10), wxSize(225, 25));
        MainFrame::FillDropdownWithAccounts(accountsDropdown);
        RoundedButton* logicButton = new RoundedButton(rightPanel, 32, "Device Auth", wxPoint((560 / 2) - (125 / 2), 5 + 35), wxSize(125, 25), darkPurple); // (rightPanel / 2) - (buttonWidth / 2)
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    }

    else if (page == "Friends List") { // FRONT/BACK/SCROLL
        // List(Accounts)
        // Load Friends List Button
        //\n
        // Display Name Dialogue
        // Send Request Button
        // Search Dialogue 
    }

    else if (page == "Exchange Code") { // FRONT/BACK
        wxChoice* accountsDropdown = new wxChoice(rightPanel, 51, wxPoint((560 / 2) - (225 / 2), 10), wxSize(225, 25));
        MainFrame::FillDropdownWithAccounts(accountsDropdown);
        RoundedButton* logicButton = new RoundedButton(rightPanel, 52, "Exchange Code", wxPoint((560 / 2) - (125 / 2), 5 + 35), wxSize(125, 25), darkPurple); // (rightPanel / 2) - (buttonWidth / 2)
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    }

    else if (page == "Invite Players") { // FRONT/BACK/SCROLL
        // List(Accounts)
        // Logic Button
        // Search
    }

    else if (page == "Launch Game") { // FRONT/BACK
        // List(Accounts)
        // Logic Button
        wxChoice* accountsDropdown = new wxChoice(rightPanel, 71, wxPoint((560 / 2) - (225 / 2), 10), wxSize(225, 25));
        MainFrame::FillDropdownWithAccounts(accountsDropdown);
        RoundedButton* logicButton = new RoundedButton(rightPanel, 72, "Launch", wxPoint((560 / 2) - (125 / 2), 5 + 35), wxSize(125, 25), darkPurple); // (rightPanel / 2) - (buttonWidth / 2)
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    }

    else if (page == "Research Lab") { // FRONT/BACK
        // List(Accounts)
        // Logic Button
        wxChoice* accountsDropdown = new wxChoice(rightPanel, 81, wxPoint((560 / 2) - (225 / 2), 10), wxSize(225, 25));
        MainFrame::FillDropdownWithAccounts(accountsDropdown);
        RoundedButton* logicButton = new RoundedButton(rightPanel, 82, "Load Research", wxPoint((560 / 2) - (125 / 2), 5 + 35), wxSize(125, 25), darkPurple); // (rightPanel / 2) - (buttonWidth / 2)
        logicButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    }

    else if (page == "Strom Shields") { // FRONT/BACK/SCROLL
        // List(Accounts)
        // Logic Button
    }

    else {
        // Uh oh
    }
}

void MainFrame::Logic(int id) {
    if (id == 13) {
		oneScrollSizer->Clear(true);
        //see if the user wants their own account searched, or someone elses
        wxChoice* dropdown = (wxChoice*)wxChoice::FindWindowById(11); 
        bool getUsernameFromDropdown;
		dropdown->GetSelection() == -1 ? getUsernameFromDropdown = false : getUsernameFromDropdown = true;

        std::string account_id; std::string loggedInAccountId; // account to check, account used to check (for api, must be auth'd)

		// Gets id, if dropdown is selected, get id from username, else get id from textctrl
        if (getUsernameFromDropdown) { 
            account_id = JSON::GetIdFromUsername((std::string)dropdown->GetStringSelection(), JSON::File::ACCOUNTS); 
            loggedInAccountId = account_id;
        }
        else { 
			account_id = ((wxTextCtrl*)wxTextCtrl::FindWindowById(12))->GetValue();
        }

        // gets all accounts, and sets to first account if so - if not, error/return;
        std::vector<std::string> accounts = JSON::GetAllUsers();
        if (accounts.size() == 0) {
			wxMessageBox(
                "No accounts found!", 
                "RayLauncher - Account ID Lookup", 
                wxICON_WARNING
            ); 
            return;
		}
        else {
			loggedInAccountId = accounts[0]; 
        }

        // Account ID is invalid, 98% user error, 1% account id in accounts.json is wrong, 1% i fucked up somewhere
        if (account_id.length() != 32) {
			wxMessageBox(
                "Invalid account ID!\nIf you selected an account, re-login. \nIf you inputted a string, make sure it is the right length", 
                "RayLauncher - Account ID Lookup", 
                wxICON_WARNING
            );
			return;
        }

        // Web request to get bearer token
        std::string bearer_token = Web::GetToken(loggedInAccountId);
        if (bearer_token.find("Error") != std::string::npos) { 
            wxMessageBox(
                "Error - Please select a different account, \nor re-login with the selected account", 
                "RayLauncher - Account Settings", 
                wxICON_ERROR
            );
            return; 
        } 
        
        std::list<std::string> headers = {
            "Authorization: bearer " + bearer_token,
            "Content-Type: application/json"
        };

        // Get account information
        std::string informationRawData = Web::GETRequest("https://account-public-service-prod.ol.epicgames.com/account/api/public/account?accountId=" + account_id, headers);
        // Parse, otherwise error. 
        nlohmann::json accountInformation;
        try {
            accountInformation = nlohmann::json::parse(informationRawData);
        }
        catch (const nlohmann::json::parse_error& e) {
            wxMessageBox("JSON Error: \n" + std::string(e.what()), "RayLauncher - Account Settings", wxICON_ERROR);
            return;
        }

		// Format data into variable for readable access - try/catch for if the data is not there
        std::string displayName;
        std::string psnName;
        std::string xblName;
        std::string switchName;
        try {
            displayName = accountInformation[0]["displayName"];
        }
        catch (std::exception e) {
            displayName = "None??";
        }

        try {
            psnName = accountInformation[0]["externalAuths"]["psn"]["externalDisplayName"];
        }
        catch (std::exception e) { 
			psnName = "None linked.";
        }

        try {
			xblName = accountInformation[0]["externalAuths"]["xbl"]["externalDisplayName"];
		}
		catch (std::exception e) {
			xblName = "None linked.";
        }

        try {
			switchName = accountInformation[0]["externalAuths"]["nintendo"]["externalDisplayName"];
        }
        catch (std::exception e) {
			switchName = "None linked.";
        }
       

        // Parse data aand add it to window
		wxScrolledWindow* scrolledWindow = (wxScrolledWindow*)wxScrolledWindow::FindWindowById(14);
        wxStaticText* displayNameText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Display Name: " + displayName, 0, 14, false, RayUtils::Window::MAIN);
		wxStaticText* psnNameText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "PSN Name: " + psnName, 0, 14, false, RayUtils::Window::MAIN);
		wxStaticText* xblNameText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "XBL Name: " + xblName, 0, 14, false, RayUtils::Window::MAIN);
		wxStaticText* switchText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Switch Name: " + switchName, 0, 14, false, RayUtils::Window::MAIN);

		MainFrame::AddItemToScroller(scrolledWindow, displayNameText, oneScrollSizer);
		MainFrame::AddItemToScroller(scrolledWindow, psnNameText, oneScrollSizer);
		MainFrame::AddItemToScroller(scrolledWindow, xblNameText, oneScrollSizer);
		MainFrame::AddItemToScroller(scrolledWindow, switchText, oneScrollSizer);
		scrolledWindow->Show();

        if (getUsernameFromDropdown) {
            headers = {
            "Authorization: bearer " + bearer_token,
            "Content-Type: application/json"
            };

            std::string advancedRawInformation = Web::GETRequest("https://account-public-service-prod.ol.epicgames.com/account/api/public/account/displayName/" + displayName, headers);
            nlohmann::json advancedInformation;
            try {
                advancedInformation = nlohmann::json::parse(advancedRawInformation);
            }
            catch (const nlohmann::json::parse_error& e) {
                wxMessageBox(
                    "JSON Error: \n" + std::string(e.what()),
                    "RayLauncher - Account Settings",
                    wxICON_ERROR
                );
                return;
            }

            JSON::WriteDebugToFile("C:\\Users\\Noah\\Desktop\\debug_3.json", advancedInformation);

            std::string country;
            std::string lastDisplayNameChange;
            std::string numberOfDisplayNameChanges;
            std::string email;
            std::string preferredLanguage;
            std::string lastLogin;
            std::string firstName;
            std::string lastName;
            std::string phoneNumber;

			try {
				country = advancedInformation["country"];
			}
            catch (std::exception e) {
                country = "None detected.";
            }

            try {
				lastDisplayNameChange = advancedInformation["lastDisplayNameChange"];
			}
			catch (std::exception e) {
				lastDisplayNameChange = "None detected.";
			}

            try {
				numberOfDisplayNameChanges = advancedInformation["numberOfDisplayNameChanges"];
            }
            catch (std::exception e){
				numberOfDisplayNameChanges = "None detected.";
            }

            try {
				email = advancedInformation["email"];
			}
			catch (std::exception e) {
				email = "None detected.";
			}

            try {
				preferredLanguage = advancedInformation["preferredLanguage"];
			}
			catch (std::exception e) {
				preferredLanguage = "None detected.";
			}

            try {
				lastLogin = advancedInformation["lastLogin"];
            }
			catch (std::exception e) {
				lastLogin = "None detected.";
			}

            try {
				firstName = advancedInformation["firstName"];
			}
			catch (std::exception e) {
				firstName = "None detected.";
			}

            try {
                lastName = advancedInformation["lastName"];
			}
			catch (std::exception e) {
				lastName = "None detected.";
            }

            try {
				phoneNumber = advancedInformation["phoneNumber"];
			}
			catch (std::exception e) {
				phoneNumber = "None linked.";
			}

			wxStaticText* countryText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Country: " + country, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* lastDisplayNameChangeText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Last Display Name Change: " + lastDisplayNameChange, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* numberOfDisplayNameChangesText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Number of Display Name Changes: " + numberOfDisplayNameChanges, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* emailText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Email: " + email, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* preferredLanguageText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Preferred Language: " + preferredLanguage, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* lastLoginText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Last Login: " + lastLogin, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* firstNameText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "First Name: " + firstName, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* lastNameText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Last Name: " + lastName, 0, 14, false, RayUtils::Window::MAIN);
			wxStaticText* phoneNumberText = RayUtils::CreateCenteredText(scrolledWindow, wxID_ANY, "Phone Number: " + phoneNumber, 0, 14, false, RayUtils::Window::MAIN);

			MainFrame::AddItemToScroller(scrolledWindow, countryText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, lastDisplayNameChangeText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, numberOfDisplayNameChangesText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, emailText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, preferredLanguageText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, lastLoginText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, firstNameText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, lastNameText, oneScrollSizer);
			MainFrame::AddItemToScroller(scrolledWindow, phoneNumberText, oneScrollSizer);
        }
        return;
	}
	else if (id == 22) {
        wxChoice* dropdown = (wxChoice*)wxChoice::FindWindowById(21);
        if (dropdown->GetSelection() == -1) {
            wxMessageBox("Please select an account!", "RayLauncher - Account Settings", wxICON_WARNING);
            return;
        }
        else {
            std::string account_id = JSON::GetIdFromUsername((std::string)dropdown->GetStringSelection(), JSON::File::ACCOUNTS);
            std::string bearer_token = Web::GetToken(account_id);
            if (bearer_token.find("Error") != std::string::npos) { wxMessageBox("Error - Please select a different account, \nor re-login with the selected account", "RayLauncher - Account Settings", wxICON_ERROR);}
            std::string exchange_code = Web::GetExchangeCodeWithBearer(bearer_token);
            if (exchange_code.find("Error") != std::string::npos) { wxMessageBox("Error - this shouldn't have happened! \nPlease open an issue at \nhttps://github.com/NoahSheppard/RayLauncher\nAnd say: \"MainFrame.cpp, switch 22, second if\"", "RayLauncher - Account Settings", wxICON_ERROR); }
            wxLaunchDefaultBrowser("https://www.epicgames.com/id/exchange?exchangeCode=" + exchange_code);
        }
        return;
	}

	else if (id == 32) {

	}

	else if (id == 52) {

	}

	else if (id == 72) {

	}

	else if (id == 82) {

	}

	else {
		wxLogStatus((wxString)"Button ID: " + std::to_string(id) + ". This should not have happened! Open an issue at https://github.com/NoahSheppard/RayLauncher with the button id!");
	}
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

void MainFrame::FillDropdownWithAccounts(wxChoice* dropdown) {
    std::vector<std::string> data = JSON::GetAllUsers();
	if (data.size() == 0) { return; } // no accounts
    for (std::string account_id : data) {
        dropdown->Append(JSON::GetAccountUsername(account_id, JSON::File::ACCOUNTS));
    }
}

void MainFrame::ShowCreditsDialog(wxWindow* parent) {
    wxMessageDialog* dialog = new wxMessageDialog(
        parent,
        "Developers: Noah Sheppard (NOAHBENJ)\nGitHub: https://github.com/NoahSheppard/RayLauncher/",
        "RayLauncher - Credits",
        wxOK | wxCENTRE | wxICON_INFORMATION
    );

    // Disable the alert sound
    dialog->SetYesNoLabels("OK", "");

    dialog->ShowModal();
    dialog->Destroy();
}

void MainFrame::AddItemToScroller(wxScrolledWindow* scrolledWindow, RoundedButton* button, wxBoxSizer* scrollSizer) {
    scrollSizer->Add(button, 0, wxALL | wxEXPAND, 5);
	scrolledWindow->SetSizer(scrollSizer);
	scrolledWindow->FitInside();
	scrolledWindow->SetScrollRate(0, 20);
	//scrolledWindow->SetBackgroundColour(lightColour);
}

void MainFrame::AddItemToScroller(wxScrolledWindow* scrolledWindow, wxStaticText* text, wxBoxSizer* scrollSizer) {
    scrollSizer->Add(text, 0, wxALL | wxEXPAND, 5);
	scrolledWindow->SetSizer(scrollSizer);
	scrolledWindow->FitInside();
	scrolledWindow->SetScrollRate(0, 20);
	//scrolledWindow->SetBackgroundColour(lightColour);
}


// # ------------------------------------------------------------------------------------------ Setup

void MainFrame::SetupIcon() {
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(107));
    if (hIcon) {
        wxIcon icon;
        icon.CreateFromHICON(hIcon);
        if (icon.IsOk()) {
            SetIcon(icon);
        }
        DestroyIcon(hIcon);
    }
}

void MainFrame::SetupFont() {
    wxFont::AddPrivateFont(SetupResource("Burbank.ttf", 110));
}

std::string MainFrame::SetupResource(std::string fileName, int id) {
    HMODULE hModule = GetModuleHandle(NULL);
    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(id), RT_RCDATA);
    HGLOBAL hMemory = LoadResource(hModule, hResource);
    void* fontData = LockResource(hMemory);
    DWORD fontDataSize = SizeofResource(hModule, hResource);
    wxString tempPath = wxFileName::GetTempDir() + wxFILE_SEP_PATH + fileName;
    wxFile file;
    if (file.Create(tempPath, true)) {
        if (file.Write(fontData, fontDataSize) == fontDataSize) {
            file.Close();
            return (std::string)tempPath;
        }
    }
    return nullptr;
}

