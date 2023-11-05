#pragma once
#include <wx/bmpbuttn.h>


const u_int IMAGE_LENGTH = 250;
const u_int IMAGE_HEIGHT = 250;

class Selection
{
public:
  Selection(wxWindow& window, const std::string &image_path, u_int x_pos, u_int y_pos);
  std::string getImagePath();
  int getButtonId();
  wxBitmapButton* getButton();
private:
  std::string image_path;
  wxBitmapButton* button;
  int button_id;
};