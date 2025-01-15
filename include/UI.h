#ifndef _UI_H
#define _UI_H

#include "Filters.h"

class UI {
private:
    int choice;
    string filter_prompt;
    string image_1_prompt;
    string image_1_filename;
    Image input_image_1;
    Image empty_image_object;
    Image input_image_2;
    int output_width, output_height;
    //Image output_image;

private:
    int resize_or_not;
    int horizontal_or_vertical;
    int rotation_angle;
    int lighten;
    int vertex_row_no, vertex_col_no, crop_width, crop_height;
    int fancy;
    int resize_width, resize_height;
    map < int, vector <int> > color_to_rgb = { {1, {0, 0, 0}},
                                               {2, {255, 255, 255}},
                                               {3, {80, 40, 0}},
                                               {4, {255, 0, 0}},
                                               {5, {0, 255, 0}},
                                               {6, {0, 0, 255}},
                                               {7, {255, 255, 0}} };
    int color;

public:
    UI();
    void MainMenu();
    Image load_image(const string& input_filename);
    void save_image(Image& output_image);
};


#endif //_UI_H
