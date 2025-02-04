#ifndef UI_H
#define UI_H

#include "Filters.h"
#include <unordered_map>

namespace UI {
    enum class FilterOption {
        GrayScale = 1, BlackWhite, Invert, Merge, Flip, Rotate,
        Exposure, Crop, Frame, Edges, Resize, Blur, Sunlight,
        OilPainting, OldTV, Quit = 16
    };

    namespace FilterConstants {
        const std::unordered_map<int, std::vector<int>> FRAME_COLORS;
        const std::vector<std::pair<int, int>> ROTATION_ANGLES;
        const std::string FILTER_MENU;
    };

    void Run();
}
#endif