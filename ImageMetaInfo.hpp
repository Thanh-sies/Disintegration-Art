#pragma once

#include <wx/wx.h>


class ImageMetaInfo
{
public:
  ImageMetaInfo();
  void reset();
  int get_pos_x();
  int get_pos_y();
  int get_width();
  int get_height();
  std::string get_image_path();
  void set_pos_x_and_width(int set_w);
  void set_pos_y_and_height(int set_h);
  void set_image_path(std::string image_path);
private:
  int pos_x;
  int pos_y;
  int width;
  int height;
  std::string image_path;
};