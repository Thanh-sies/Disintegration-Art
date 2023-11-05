#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

#include <boost/functional/hash.hpp>
#include <wx/rawbmp.h>
#include <wx/spinctrl.h>
#include <wx/window.h>
#include <wx/wx.h>

#include "ImageMetaInfo.hpp"
#include "constants.hpp"
#include "MainFrame.hpp"
#include "Selection.hpp"

std::string DISINTEGRATION_WINDOW = "Disintegration Animation";

void MainFrame::SetUpFrame()
{
  this->points_selection = SELECTED_POINTS;
  this->points_count = 0;
  this->sizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(sizer);
  this->image_meta = ImageMetaInfo();

  this->start_game = new wxPanel(this);
  wxStaticText *disintegration_points_text = new wxStaticText(this->start_game, wxID_ANY, "Disintegration Points - Range : [1,10]", wxPoint(50, 675));
  this->disintegration_points = new wxSpinCtrl(this->start_game, wxID_ANY, "", wxPoint(280, 675));
  this->disintegration_points->SetRange(1, 10);
  wxStaticText *threshold_text = new wxStaticText(this->start_game, wxID_ANY, "Threshold For Ignoring Intensity Values Under - Range : [0,255]", wxPoint(50, 625));
  this->threshold_intensity = new wxSpinCtrl(this->start_game, wxID_ANY, "", wxPoint(435, 625));
  this->threshold_intensity->SetRange(0, 255);
  
  sizer->Add(this->start_game, 1, wxGROW);

  this->in_game_view = new wxPanel(this);
  this->points_selected = new wxStaticText(this->in_game_view, wxID_ANY, this->points_selection, wxPoint(25, 675));
  wxButton *restart_btn = new wxButton(this->in_game_view, wxID_ANY, "Restart", wxPoint(1100, 675));
  restart_btn->Bind(wxEVT_BUTTON, &MainFrame::Restart, this);
  sizer->Add(this->in_game_view, 2, wxGROW);
  CreateStatusBar();
}

void MainFrame::ImportSelection()
{
  namespace fs = std::filesystem;
  std::string cwd = std::filesystem::current_path().string() + "/Photos";
  std::vector<std::pair<u_int, u_int>> pos = {{wx_WIDTH/4.2,wx_HEIGHT/14}, {wx_WIDTH/4.2,wx_HEIGHT/2}, {wx_WIDTH/(1.7),wx_HEIGHT/14}, {wx_WIDTH/(1.7), wx_HEIGHT/2}};
  u_int max_entry = pos.size();
  for (const auto & entry : fs::directory_iterator(cwd))
  {
    if (max_entry == 0) {return;}
    max_entry -= 1;
    Selection* new_entry = new Selection((*this->start_game), entry.path(), pos[max_entry].first, pos[max_entry].second);
    this->image_path_info[new_entry->getButtonId()] = new_entry->getImagePath();
    new_entry->getButton()->Bind(wxEVT_LEFT_DOWN, &MainFrame::SelectOnClick, this);
    this->images.push_back(new_entry);
  }
}

void MainFrame::SwitchPage(int panel_no)
{
  switch (panel_no) {
    case 1:
      this->in_game_view->Hide();
      this->start_game->Show();
      break;
    case 2:
      this->start_game->Hide();
      this->in_game_view->Show();
      break;
  }
  this->sizer->Layout();
}

void MainFrame::Restart(wxCommandEvent &evt)
{
  this->coordinates.clear();
  this->image_meta.reset();
  delete this->bmp_b;
  cv::destroyWindow(DISINTEGRATION_WINDOW);
  this->points_count = 0;
  this->points_selection = SELECTED_POINTS;
  this->points_selected->SetLabel(this->points_selection);
  this->points_selected->Refresh();
  this->disintegration_points->Refresh();
  this->SwitchPage(1);
}

void MainFrame::Disintegrator()
{
  cv::Mat mat = cv::imread(this->image_meta.get_image_path());
  if (mat.empty())
  {
    std::cout << "Failed to load image in disntegrator phase." << std::endl;
    return;
  }
  // Create a window for display.
  cv::namedWindow(DISINTEGRATION_WINDOW, cv::WINDOW_AUTOSIZE);
  cv::imshow(DISINTEGRATION_WINDOW, mat); 
  cv::waitKey(1);
  int original_width = mat.cols;
  int original_height = mat.rows;
  std::unordered_set<std::pair<int, int>, 
        boost::hash< std::pair<int, int> > 
    > seen;
  std::unordered_set<std::pair<int, int>, 
        boost::hash< std::pair<int, int> > 
    > queue;
  std::unordered_set<std::pair<int, int>, 
        boost::hash< std::pair<int, int> > 
    > next_queue;
  float scale_x = (float)original_width/this->image_meta.get_width();
  float scale_y = (float)original_height/this->image_meta.get_height();
  for(auto &pair : this->coordinates)
  {
    queue.insert(std::pair<int,int>{pair.first*scale_x, pair.second*scale_y});
  }
  int iterations = 0;
  int threshold  = this->threshold_intensity->GetValue();
  while (!queue.empty())
  {
    iterations += 1;
    for(auto &pair : queue)
    {
      bool is_in = seen.find(pair) != seen.end();
      if (is_in) {continue;}
      // if pixel is dark enough, we ignore it
      seen.insert(pair);
      int a = mat.at<cv::Vec3b>(cv::Point(pair.first, pair.second))[0];
      int b = mat.at<cv::Vec3b>(cv::Point(pair.first, pair.second))[1];
      int c = mat.at<cv::Vec3b>(cv::Point(pair.first, pair.second))[2];
      // You can adjust the 3 parameters for the threshold for capturing the pixel
      if (a < threshold && b < threshold && c < threshold) {continue;}
      mat.at<cv::Vec3b>(cv::Point(pair.first, pair.second))[0] = 0;
      mat.at<cv::Vec3b>(cv::Point(pair.first, pair.second))[1] = 0;
      mat.at<cv::Vec3b>(cv::Point(pair.first, pair.second))[2] = 0;
      if (pair.first -1 >= 0)
      {
        auto left = std::pair<int,int>{pair.first-1, pair.second};
        bool is_in = seen.find(left) != seen.end();
        if (!is_in) {next_queue.insert(left);}        
      }
      if (pair.first + 1 < original_width)
      {
        auto right = std::pair<int,int>{pair.first+1, pair.second};
        bool is_in = seen.find(right) != seen.end();
        if (!is_in) {next_queue.insert(right);}
      }
      if (pair.second + 1 < original_height)
      {
        auto bot = std::pair<int,int>{pair.first, pair.second+1};
        bool is_in = seen.find(bot) != seen.end();
        if (!is_in) {next_queue.insert(bot);}
      }
      if (pair.second - 1 >= 0)
      {
        auto top = std::pair<int,int>{pair.first, pair.second-1};
        bool is_in = seen.find(top) != seen.end();
        if (!is_in) {next_queue.insert(top);}
      }
  }
  cv::imshow(DISINTEGRATION_WINDOW, mat); 
  cv::waitKey(1);
  queue = next_queue;
  next_queue.clear();
  }
  this->points_selection += " Iterations - " + std::to_string(iterations);
  this->points_selected->SetLabel(this->points_selection);
  this->sizer->Layout();
}

void MainFrame::SetPoint(wxMouseEvent &evt)
{
  if (this->points_count == 0)
  {
    // Placed the count here due to wxSpinCtrl refreshing after it is out of focus.
    this->points_selection += std::to_string(this->disintegration_points->GetValue());
    this->points_selection += " ,Threshold " + std::to_string(this->threshold_intensity->GetValue()) + " - ";
  }
  if (this->disintegration_points->GetValue() == this->points_count) {return;}
  this->points_count += 1;
  wxPoint point = evt.GetPosition();
  this->coordinates.insert(std::pair(point.x, point.y));
  this->points_selection += " [" + std::to_string(point.x) + "," + std::to_string(point.y) + "]";
  this->points_selected->SetLabel(this->points_selection);
  this->points_selected->Refresh();
  this->sizer->Layout();
  if (this->disintegration_points->GetValue() == this->points_count)
  {
    this->Disintegrator();
  }
}

void MainFrame::SelectOnClick(wxMouseEvent &evt)
{
  std::string curr_image_path = this->image_path_info[evt.GetId()];
  wxBitmap bmp;
  bmp.LoadFile(curr_image_path, wxBITMAP_TYPE_ANY);
  if (!bmp.IsOk()) {return;}
  float h = bmp.GetHeight();
  float w = bmp.GetWidth();
  if (h == 0.0 || w == 0.0) {return;}
  float BORDER_FILL = 100;
  
  if (h > wx_HEIGHT - BORDER_FILL)
  {
    float reduction_factor = (wx_HEIGHT - BORDER_FILL)/h;
    h = h*reduction_factor;
    w = w*reduction_factor;
  }
  if (w > wx_WIDTH - BORDER_FILL)
  {
    float reduction_factor = (wx_WIDTH - BORDER_FILL)/w;
    h = h*reduction_factor;
    w = w*reduction_factor;
  }
  int set_w = (int)w;
  int set_h = (int)h;
  int pos_x = (wx_WIDTH - set_w)/2;
  int pos_y = (wx_HEIGHT - set_h)/2;
  wxImage image = bmp.ConvertToImage();
  this->image_meta.set_pos_x_and_width(set_w);
  this->image_meta.set_pos_y_and_height(set_h);
  this->image_meta.set_image_path(curr_image_path);
  image.Rescale(set_w, set_h);
  bmp = wxBitmap(image);

  this->bmp_b = new wxBitmapButton((this->in_game_view), wxID_ANY, bmp, wxPoint(pos_x, pos_y), wxSize(set_w, set_h),wxBORDER_NONE);
  this->bmp_b->Bind(wxEVT_LEFT_DOWN, &MainFrame::SetPoint, this);
  this->bmp_b->Show();
  this->points_selected->SetLabel(this->points_selection);
  this->points_selected->Refresh();
  this->SwitchPage(2);
}

MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title)
{ 
  this->SetUpFrame();
  this->ImportSelection();
  this->sizer->Layout();
}

