#ifndef UI_H
#define UI_H

#include "Filters.h"
#include "Photo.h"
#include <unordered_map>

namespace FilterConstants
{
    extern const std::unordered_map<int, std::vector<int>> FRAME_COLORS;
    extern const std::vector<std::pair<int, int>> ROTATION_ANGLES;
    extern const std::string FILTER_MENU;
}

namespace PromptHandlers
{
    namespace ConstantPrompts
    {
        std::string image_1_prompt(U8 choice);
        extern const std::string img2_Prompt;
        extern const std::string mergeFilterArg;
        extern const std::string flipFilterArg;
        extern const std::string rotateFilterArg;
        extern const std::string exposureFilterArg;
        extern const std::string frameFilterArg;
        extern const std::string frameFilterColorArg;
        extern const std::string resizeFilter_wArg;
        extern const std::string resizeFilter_hArg;

        // Crop
        std::string cropFilterRow_Arg(size_t);
        std::string cropFilterCol_Arg(size_t);
        std::string cropFilterHeight_Arg(size_t, size_t);
        std::string cropFilterWidth_Arg(size_t, size_t);
    }
}

class UI
{
public:
    enum class FilterOption
    {
        GrayScale = 1, BlackWhite, Invert, Merge, Flip, Rotate,
        Exposure, Crop, Frame, Edges, Resize, Blur, Sunlight,
        OilPainting, OldTV, Quit = 16
    };

    void Run();
    Photo *inputPhoto1{nullptr};
    Photo *inputPhoto2{nullptr};
    Photo *outputPhoto{nullptr};
    std::string image_1_filename{};
    std::string image_2_filename{};
    ~UI();
};
#endif