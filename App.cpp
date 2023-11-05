#include <wx/image.h> 
#include <wx/wx.h>

#include "App.hpp"
#include "constants.hpp"
#include "MainFrame.hpp"


wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  wxInitAllImageHandlers();
  MainFrame *mainFrame = new MainFrame("Disintegrate");
  mainFrame->SetClientSize(wx_WIDTH, wx_HEIGHT);
  mainFrame->Center();
  mainFrame->Show(true);
  return true;
};
