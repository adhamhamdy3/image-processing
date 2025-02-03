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

namespace UI
{
    void Run();
}
#endif // UI_H