#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <algorithm>
#include <regex>
#include <random>
#include "Image_Class.h"

using namespace std;

class Utilities{
public:
    template <typename T>
    static constexpr const T& clamp(const T& value, const T& lower, const T& upper) {
        return (value < lower) ? lower : (value > upper) ? upper : value;
    }
    static void display_progress_bar(int current_index, int max_index, string optional_title = "");
    static int get_int(const string& prompt, int l = INT_MIN, int r = INT_MAX);
    static int get_int(const string& prompt, vector<int> value_list);
    static string get_valid_image_filename(const string& prompt, bool existing = false);
    static bool in_rectangular_frame (int i, int j, int center_row, int center_column, int half_of_outer_height,
                                      int half_of_outer_width, int thickness, int outer_margin_with_overlap,
                                      int image_height, int image_width);

    static int radial_distance(int i_1, int j_1, int i_2, int j_2, int height, int width);
};


#endif //_UTILITIES_H
