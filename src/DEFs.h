#include "UI.h"
#include <vector>
#include <unordered_map>

// Constants initialization
const std::unordered_map<U8, std::vector<U16>> FilterConstants::FRAME_COLORS =
    {
        {1, {0, 0, 0}},
        {2, {255, 255, 255}},
        {3, {80, 40, 0}},
        {4, {255, 0, 0}},
        {5, {0, 255, 0}},
        {6, {0, 0, 255}},
        {7, {255, 255, 0}}
    };

const std::string FilterConstants::FILTER_MENU =
    "\nAvailable Filters: \n"
    " 1)  Gray Scale \n 2)  Black-and-White \n 3)  Invert \n 4)  Merge \n 5)  Flip \n 6)  Rotate \n"
    " 7)  Lighten/Darken \n 8)  Crop \n 9)  Frame \n 10) Detect Edges \n 11) Resize \n 12) Blur \n"
    " 13) Sunlight \n 14) Oil Painting \n 15) Old TV \n 16) Back to Main Menu \n\n"
    "Choose a filter (1-16): ";

const std::string FilterConstants::MAIN_MENU =
    "\n==========================================\n"
    "         Image Editor v1.0\n"
    "==========================================\n"
    " 1) Apply Filter\n"
    " 2) About\n"
    " 3) Quit\n"
    "==========================================\n"
    "Your choice (1-3): ";

std::string PromptHandlers::ConstantPrompts::img1_Prompt(U8 choice)
{
    return {"Please enter the file path (absolute or relative) of the " + std::string(choice == 4 ? "first " : "") + "image " + "\n" + "(Available formats: .jpg, .jpeg, .png, .bmp, .tga): "};
}

const std::string PromptHandlers::ConstantPrompts::img2_Prompt = "Please enter the file path (absolute or relative) "
                                                                 "of the second image \n(Available formats: "
                                                                 ".jpg, .jpeg, .png, .bmp, .tga): ";

// Filters Prompts
const std::string PromptHandlers::ConstantPrompts::mergeFilterArg = "Choose whether to Crop and Merge the common part of"
                                                                    " the images" +
                                                                    std::string("\n") + "or Resize them to"
                                                                                        " the same size (0: Crop, 1: Resize): ";

const std::string PromptHandlers::ConstantPrompts::flipFilterArg = "Choose whether to Flip the image (1: Horizontally, "
                                                                   "2: Vertically): ";

const std::string PromptHandlers::ConstantPrompts::rotateFilterArg = "Choose a CLOCKWISE rotation angle (90/180/270): ";

const std::string PromptHandlers::ConstantPrompts::exposureFilterArg = "Choose whether to lighten or darken the image"
                                                                       " (1 to lighten, 0 to darken): ";

std::string PromptHandlers::ConstantPrompts::cropFilterRow_Arg(size_t imgH)
{
    return {"Choose the row number (top-to-bottom) to start cropping at (1-" + std::to_string(imgH) + ") : "};
}

std::string PromptHandlers::ConstantPrompts::cropFilterCol_Arg(size_t imgW)
{
    return {"Choose the column number (left-to-right) to start cropping at (1-" + std::to_string(imgW) + ") : "};
}

std::string PromptHandlers::ConstantPrompts::cropFilterHeight_Arg(size_t imgH, size_t vertexRN)
{
    return {"Choose the height of the cropped image (1-" + std::to_string(imgH - vertexRN) + ") : "};
}

std::string PromptHandlers::ConstantPrompts::cropFilterWidth_Arg(size_t imgW, size_t vertexCN)
{
    return {"Choose the width of the cropped image (1-" + std::to_string(imgW - (vertexCN)) + ") : "};
}

const std::string PromptHandlers::ConstantPrompts::frameFilterArg = "Choose whether to apply a simple or fancy Frame "
                                                                    "(0 for simple, 1 for fancy): ";

const std::string PromptHandlers::ConstantPrompts::frameFilterColorArg = "Available Colors: \n"
                                                                         " 1) Black \n 2) White \n 3) Brown \n 4) Red \n 5) Green \n 6) Blue \n"
                                                                         " 7) Yellow \n\nChoose a color (1-7): ";

const std::string PromptHandlers::ConstantPrompts::resizeFilter_wArg = "Choose the width of the rescaled image: ";

const std::string PromptHandlers::ConstantPrompts::resizeFilter_hArg = "Choose the height of the rescaled image: ";




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
//    case FilterOption::Merge:
//        Filters::Merge(*input1->currentImage, *input2->currentImage, *output->currentImage, params.resize_or_not);
//        break;
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
        break; // Merge is commented-out; Quit is handled by the caller
    }
}