#ifndef _FILTERS_H
#define _FILTERS_H

#include "Utilities.h"
#include "Image.h"

namespace Filters {
    void grayscale(Image& input_image, Image& output_image);
    void black_and_white(Image& input_image, Image& output_image);
    void invert(Image& input_image, Image& output_image);
    void merge(Image& input_image_1, Image& input_image_2, Image& output_image, const int& resize_or_not);
    void flip(Image& input_image, Image& output_image, const int& horizontal_or_vertical);
    void rotate(Image& input_image, Image& output_image, const int& rotation_angle);
    void lighten_or_darken(Image& input_image, Image& output_image, const int& lighten);
    void crop(Image& input_image, Image& output_image, const int& vertex_row_no, const int& vertex_col_no);
    void frame(Image& input_image, Image& output_image, const int& fancy, const int& color, const std::map <int,
               std::vector <int>>& color_to_rgb);

    void edges(Image& input_image, Image& output_image);
    Image& resize(Image& input_image, Image& output_image);
    void blur(Image& input_image, Image& output_image, const int& blur_radius, const int& call_no);
    void sunlight(Image& input_image, Image& output_image);
    void oil_painting(Image& input_image, Image& output_image);
    void old_tv(Image& input_image, Image& output_image);
};


#endif //_FILTERS_H
