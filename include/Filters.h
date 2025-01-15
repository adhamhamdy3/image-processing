#ifndef _FILTERS_H
#define _FILTERS_H

#include "Utilities.h"

class Filters {
public:
    static void grayscale(Image& input_image, Image& output_image);
    static void black_and_white(Image& input_image, Image& output_image);
    static void invert(Image& input_image, Image& output_image);
    static void merge(Image& input_image_1, Image& input_image_2, Image& output_image, const int& resize_or_not);
    static void flip(Image& input_image, Image& output_image, const int& horizontal_or_vertical);
    static void rotate(Image& input_image, Image& output_image, const int& rotation_angle);
    static void lighten_or_darken(Image& input_image, Image& output_image, const int& lighten);
    static void crop(Image& input_image, Image& output_image, const int& vertex_row_no, const int& vertex_col_no);
    static void frame(Image& input_image, Image& output_image, const int& fancy, const int& color, const map <int, vector <int>>& color_to_rgb);
    static void edges(Image& input_image, Image& output_image);
    static Image& resize(Image& input_image, Image& output_image);
    static void blur(Image& input_image, Image& output_image, const int& blur_radius, const int& call_no);
    static void sunlight(Image& input_image, Image& output_image);
    static void oil_painting(Image& input_image, Image& output_image);
    static void old_tv(Image& input_image, Image& output_image);
};


#endif //_FILTERS_H
