#include "UI.h"

void applyFilter(FilterOption choice, Photo *input1, [[maybe_unused]] Photo *input2,
                 [[maybe_unused]] Photo *output, const PromptHandlers::FilterParams &params, int color)
{
    switch (choice)
    {
    case FilterOption::GrayScale:
        Filters::Grayscale(input1);
        break;
    case FilterOption::BlackWhite:
        Filters::Black_White(input1);
        break;
    case FilterOption::Invert:
        Filters::Invert(input1);
        break;
    case FilterOption::Merge:
        Filters::Merge(input1, input2, output, params.resize_or_not);
        break;
    case FilterOption::Flip:
        Filters::Flip(input1, params.horizontal_or_vertical);
        break;
    case FilterOption::Rotate:
        Filters::Rotate(input1, params.rotation_angle);
        break;
    case FilterOption::Exposure:
        Filters::Exposure(input1, params.lighten);
        break;
    case FilterOption::Crop:
        Filters::Crop(input1, params.vertex_row_no, params.vertex_col_no, params.crop_width, params.crop_height);
        break;
    case FilterOption::Frame:
        Filters::Frame(input1, params.fancy, color, FilterConstants::FRAME_COLORS);
        break;
    case FilterOption::Edges:
        Filters::DetectEdges(input1);
        break;
    case FilterOption::Resize:
        Filters::Resize(input1, params.resize_width, params.resize_height);
        break;
    case FilterOption::Blur:
    {
        float bR = Utilities::Validations::getBlurLevel();
        Filters::Blur(input1, bR);
        break;
    }
    case FilterOption::Sunlight:
        Filters::Sunlight(input1);
        break;
    case FilterOption::OilPainting:
        Filters::OilPainting(input1);
        break;
    case FilterOption::OldTV:
        Filters::OldTV(input1);
        break;
    default:
        break;
    }
}