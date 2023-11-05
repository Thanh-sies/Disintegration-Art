#include "Selection.hpp"
#include <wx/bmpbuttn.h>
#include <wx/frame.h>
#include <wx/gdicmn.h>
#include <wx/window.h>


Selection::Selection(wxWindow& window, const std::string &image_path, u_int x_pos, u_int y_pos)
{
  this->image_path = image_path;
  wxBitmap bmp;
  bmp.LoadFile(image_path, wxBITMAP_TYPE_ANY);
  if (!bmp.IsOk()) {return;}
  wxImage image = bmp.ConvertToImage();
  image.Rescale(IMAGE_LENGTH, IMAGE_HEIGHT);
  bmp = wxBitmap(image);
  this->button = new wxBitmapButton(&window, wxID_ANY, bmp, wxPoint(x_pos, y_pos), wxSize(IMAGE_LENGTH, IMAGE_HEIGHT),wxBORDER_NONE);
  this->button_id = this->button->GetId();
  this->button->Show();
}

std::string Selection::getImagePath()
{
  return this->image_path;
}

int Selection::getButtonId()
{
  return this->button_id;
}

wxBitmapButton* Selection::getButton()
{
  return this->button;
}