#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>
#include <opencv2/core/mat.hpp>
#include <wx/spinctrl.h>
#include <wx/wx.h>

#include "ImageMetaInfo.hpp"
#include "Selection.hpp"

const std::string SELECTED_POINTS = "Selected Points: ";

class MainFrame : public wxFrame
{
public:
  MainFrame(const wxString &title);

private:
  std::string points_selection;
  int points_count;
  cv::Mat mat;
  ImageMetaInfo image_meta;
  wxPanel* start_game;
  wxWindow* in_game_view;
  wxBoxSizer *sizer;
  wxBitmapButton* bmp_b;
  wxStaticText* points_selected;
  wxSpinCtrl* disintegration_points;
  wxSpinCtrl* threshold_intensity;
  std::unordered_set<std::pair<int, int>, 
        boost::hash< std::pair<int, int> > 
    > coordinates;
  std::vector<Selection*> images;
  std::unordered_map<int, std::string> image_path_info;
  void Restart(wxCommandEvent &evt);
  void SetPoint(wxMouseEvent &evt);
  void SelectOnClick(wxMouseEvent &evt);
  void SetUpFrame();
  void ImportSelection();
  void SwitchPage(int panel_no);
  void Disintegrator();
};