#include <wx/wx.h>
#include <wx/settings.h>
#include <wx/frame.h>
#pragma once
class AccountsFrame : public wxFrame
{
public: 
	AccountsFrame(const wxString& title);

    // Static method to get instance
    static AccountsFrame* GetInstance() { return instance; }

    // Static method to destroy the window
    static void DestroyWindow()
    {
        if (instance)
        {
            instance->Destroy();  // Call the wxWindow's Destroy method
            instance = nullptr;   // Reset the static pointer
        }
    }

    // Override window destruction to handle the static pointer
    virtual bool Destroy() override
    {
        instance = nullptr;
        return wxFrame::Destroy();
    }
private:
    wxPanel* rightPanel;
    void OnButtonClicked(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    void LoadPageContent(std::string page);
    void OnCloseWindow(wxCloseEvent& event);
    //std::vector<std::string> split(const std::string& str, const std::string& delim);
    
    void Logic(int id);
    static AccountsFrame* instance;

};

