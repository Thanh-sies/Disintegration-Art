#include <wx/wx.h>

#include "constants.hpp"
#include "ImageMetaInfo.hpp"

ImageMetaInfo::ImageMetaInfo()
{
  this->pos_x = -1;
  this->pos_y = -1;
  this->width = -1;
  this->height = -1;
  this->image_path = "";
};

void ImageMetaInfo::reset()
{
  this->pos_x = -1;
  this->pos_y = -1;
  this->width = -1;
  this->height = -1;
  this->image_path = "";
}

int ImageMetaInfo::get_pos_x() {return this->pos_x;}
int ImageMetaInfo::get_pos_y() {return this->pos_y;}
int ImageMetaInfo::get_width() {return this->width;}
int ImageMetaInfo::get_height() {return this->height;}
std::string ImageMetaInfo::get_image_path() {return this->image_path;}

void ImageMetaInfo::set_pos_x_and_width(int set_w)
{
  this->width = set_w;
  this->pos_x = (wx_WIDTH - set_w)/2;
}
void ImageMetaInfo::set_pos_y_and_height(int set_h)
{
  this->height = set_h;
  this->pos_y = (wx_HEIGHT - set_h)/2;
}

void ImageMetaInfo::set_image_path(std::string image_path)
{
  this->image_path = image_path;
}