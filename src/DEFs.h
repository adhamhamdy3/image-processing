#include "UI.h"
#include <vector>
#include <unordered_map>

// Constants initialization
const std::unordered_map<int, std::vector<int>> FilterConstants::FRAME_COLORS =
        {
                {1, {0, 0, 0}}, {2, {255, 255, 255}},
                {3, {80, 40, 0}}, {4, {255, 0, 0}},
                {5, {0, 255, 0}}, {6, {0, 0, 255}},
                {7, {255, 255, 0}}
        };

const std::string FilterConstants::FILTER_MENU =
        "\nAvailable Filters: \n"
        " 1)  Gray Scale \n 2)  Black-and-White \n 3)  Invert \n 4)  Merge \n 5)  Flip \n 6)  Rotate \n"
        " 7)  Lighten/Darken \n 8)  Crop \n 9)  Frame \n 10) Detect Edges \n 11) Resize \n 12) Blur \n"
        " 13) Sunlight \n 14) Oil Painting \n 15) Old TV \n 16) Quit \n\n"
        "Choose a filter, or quit (1-16): ";

std::string PromptHandlers::ConstantPrompts::img1_Prompt(U8 choice)
{
    return {"Please enter the filename (NOT FILEPATH, including extension) of the " + std::string(choice == 4 ? "first " :"")
    + "image (must exist in this directory) " + "\n" + "(Available formats: .jpg, .png, .bmp, .tga): "
    };
}

const std::string PromptHandlers::ConstantPrompts::img2_Prompt = "Please enter the filename (NOT FILEPATH, "
                                                            "including extension) of the second image "
                                                            "(must exist in this directory) \n(Available formats: "
                                                            ".jpg, .png, .bmp, .tga): ";

const std::string PromptHandlers::ConstantPrompts::mergeFilterArg = "Choose whether to crop and merge the common part of"
                                                               " the images" + std::string("\n") + "or resize them to"
                                                                                              " the same size (0: Crop, 1: Resize): ";

const std::string PromptHandlers::ConstantPrompts::flipFilterArg = "Choose whether to flip the image (1: Horizontally, "
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

const std::string PromptHandlers::ConstantPrompts::frameFilterArg = "Choose whether to apply a simple or fancy frame "
                                                               "(0 for simple, 1 for fancy): ";

const std::string PromptHandlers::ConstantPrompts::frameFilterColorArg = "Available Colors: \n"
                                                                    " 1) Black \n 2) White \n 3) Brown \n 4) Red \n 5) Green \n 6) Blue \n"
                                                                    " 7) Yellow \n\nChoose a color (1-7): ";

const std::string PromptHandlers::ConstantPrompts::resizeFilter_wArg = "Choose the width of the rescaled image: ";

const std::string PromptHandlers::ConstantPrompts::resizeFilter_hArg = "Choose the height of the rescaled image: ";


namespace {
    struct FilterParams {
        int resize_or_not;
        int horizontal_or_vertical;
        int rotation_angle;
        int lighten;
        int vertex_row_no, vertex_col_no, crop_width, crop_height;
        int fancy;
        int resize_width, resize_height;
    };

    void applyFilter(UI::FilterOption choice, Photo* input1, Photo* input2,
                     Photo* output, const FilterParams& params, int color)
    {

        switch(choice) {
            case UI::FilterOption::GrayScale:
                Filters::grayScale(*input1->currentImage, *output->currentImage);
                // input1->pushChanges();
                break;
            case UI::FilterOption::BlackWhite:
                Filters::Black_White(*input1->currentImage, *output->currentImage);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Invert:
                Filters::invert(*input1->currentImage, *output->currentImage);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Merge:
                Filters::merge(*input1->currentImage, *input2->currentImage, *output->currentImage, params.resize_or_not);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Flip:
                Filters::flip(*input1->currentImage, *output->currentImage, params.horizontal_or_vertical);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Rotate:
                Filters::rotate(*input1->currentImage, *output->currentImage, params.rotation_angle);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Exposure:
                Filters::exposure(*input1->currentImage, *output->currentImage, params.lighten);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Crop:
                Filters::crop(*input1->currentImage, *output->currentImage, params.vertex_row_no, params.vertex_col_no);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Frame:
                Filters::frame(*input1->currentImage, *output->currentImage, params.fancy, color, FilterConstants::FRAME_COLORS);
                // input1->pushChanges();
                break;
            case UI::FilterOption::Edges:
                Filters::edges(*input1->currentImage, *output->currentImage);
                break;
            case UI::FilterOption::Resize:
                Filters::resize(*input1->currentImage, *output->currentImage);
                break;
            case UI::FilterOption::Blur:
                Filters::blur(*input1->currentImage, *output->currentImage,
                              sqrt((input1->currentImage->height * input1->currentImage->width))/160, 1);

                Filters::blur(*output->currentImage, *output->currentImage,
                              sqrt((input1->currentImage->height * input1->currentImage->width))/80, 2);
                break;
            case UI::FilterOption::Sunlight:
                Filters::sunlight(*input1->currentImage, *output->currentImage);
                break;
            case UI::FilterOption::OilPainting:
                Filters::oilPainting(*input1->currentImage, *output->currentImage);
                break;
            case UI::FilterOption::OldTV:
                Filters::oldTV(*input1->currentImage, *output->currentImage);
                break;
        }
    }
}