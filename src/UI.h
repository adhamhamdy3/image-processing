#ifndef UI_H
#define UI_H

#include "Filters.h"
#include "Photo.h"
#include <unordered_map>

enum class FilterOption
{
    GrayScale = 1,
    BlackWhite,
    Invert,
    Merge,
    Flip,
    Rotate,
    Exposure,
    Crop,
    Frame,
    Edges,
    Resize,
    Blur,
    Sunlight,
    OilPainting,
    OldTV,
    Quit = 16
};

namespace FilterConstants
{
    extern const std::unordered_map<int, std::vector<int>> FRAME_COLORS;
    extern const std::string FILTER_MENU;
}

namespace PromptHandlers
{
    namespace ConstantPrompts
    {
        std::string img1_Prompt(U8 choice);
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

    struct FilterParams
    {
        int resize_or_not;
        int horizontal_or_vertical;
        int rotation_angle;
        int lighten;
        int vertex_row_no, vertex_col_no, crop_width, crop_height;
        int fancy;
        int resize_width, resize_height;
    };
}

class UI
{
public:
    Photo *inputPhoto1{nullptr};
    Photo *inputPhoto2{nullptr};
    Photo *outputPhoto{nullptr};
    std::string img1_fileName{};
    std::string img2_fileName{};

    void Run();
    void cleanUp();
    ~UI();
};

#endif