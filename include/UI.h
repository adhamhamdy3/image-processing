#ifndef UI_H
#define UI_H

#include "Filters.h"
#include <unordered_map>

using namespace std;

enum FilterChoice {
    GRAYSCALE = 1, BW, INVERT, MERGE, FLIP, ROTATE, LIGHTEN,
    CROP, FRAME, EDGES, RESIZE, BLUR, SUNLIGHT, OIL, TV, QUIT = 16
};

struct FilterParams {
    int resize_or_not = 0;
    int direction = 0;
    int angle = 0;
    int lighten = 0;
    int vertex_row = 0;
    int vertex_col = 0;
    int crop_h = 0;
    int crop_w = 0;
    int fancy = 0;
    int color = 0;
    int new_w = 0;
    int new_h = 0;
    int blur_radius = 1;
    int sun_intensity = 50;
    int brush_size = 5;
    int noise_intensity = 5;
};

class UI {
public:
    void MainMenu();

private:
    // Image operations
    Image load_image(const string& filename);
    void save_image(Image& image);

    // Color mapping (static as it's read-only)
    static const std::map<int, std::vector<int>> COLOR_MAP;

    // Filter processing
    //void process_filter(FilterChoice choice, const Image& input1, const Image& input2, const FilterParams& params);
};

#endif // UI_H