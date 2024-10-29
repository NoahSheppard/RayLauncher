#include "AccountsWindow.h"
#include "AccountsFrame.h"
#include <wx/wx.h>
#include <wx/settings.h>

bool AccountsWindow::OnInit() {
	AccountsFrame* mainFrame = new AccountsFrame("RayLauncher - Accounts");
	mainFrame->SetClientSize(600, 300);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}