#include "UI.h"
#include "Utilities.h"
using namespace std;

// Constants initialization
const unordered_map<int, vector<int>> FilterConstants::FRAME_COLORS =
{
        {1, {0, 0, 0}}, {2, {255, 255, 255}},
        {3, {80, 40, 0}}, {4, {255, 0, 0}},
        {5, {0, 255, 0}}, {6, {0, 0, 255}},
        {7, {255, 255, 0}}
};

const string FilterConstants::FILTER_MENU =
        "\nAvailable Filters: \n"
        " 1)  Gray Scale \n 2)  Black-and-White \n 3)  Invert \n 4)  Merge \n 5)  Flip \n 6)  Rotate \n"
        " 7)  Lighten/Darken \n 8)  Crop \n 9)  Frame \n 10) Detect Edges \n 11) Resize \n 12) Blur \n"
        " 13) Sunlight \n 14) Oil Painting \n 15) Old TV \n 16) Quit \n\n"
        "Choose a filter, or quit (1-16): ";

string PromptHandlers::ConstantPrompts::image_1_prompt(U8 choice)
{
    return {"Please enter the filename (NOT FILEPATH, including extension) of the " + string(choice == 4 ? "first " :
    "") + "image (must exist in this directory) " + "\n" + "(Available formats: .jpg, .png, .bmp, .tga): "
    };
}

const string PromptHandlers::ConstantPrompts::img2_Prompt = "Please enter the filename (NOT FILEPATH, "
                                                               "including extension) of the second image "
                                                               "(must exist in this directory) \n(Available formats: "
                                                               ".jpg, .png, .bmp, .tga): ";

const string PromptHandlers::ConstantPrompts::mergeFilterArg = "Choose whether to crop and merge the common part of"
                                                                    " the images" + string("\n") + "or resize them to"
                                                                    " the same size (0: Crop, 1: Resize): ";

const string PromptHandlers::ConstantPrompts::flipFilterArg = "Choose whether to flip the image (1: Horizontally, "
                                                                    "2: Vertically): ";

const string PromptHandlers::ConstantPrompts::rotateFilterArg = "Choose a CLOCKWISE rotation angle (90/180/270): ";

const string PromptHandlers::ConstantPrompts::exposureFilterArg = "Choose whether to lighten or darken the image"
                                                                  " (1 to lighten, 0 to darken): ";

string PromptHandlers::ConstantPrompts::cropFilterRow_Arg(size_t imgH)
{
    return {"Choose the row number (top-to-bottom) to start cropping at (1-" + to_string(imgH) + ") : "};
}

string PromptHandlers::ConstantPrompts::cropFilterCol_Arg(size_t imgW)
{
    return {"Choose the column number (left-to-right) to start cropping at (1-" + to_string(imgW) + ") : "};
}
        
string PromptHandlers::ConstantPrompts::cropFilterHeight_Arg(size_t imgH, size_t vertexRN)
{
    return {"Choose the height of the cropped image (1-" + to_string(imgH - vertexRN) + ") : "};
}

string PromptHandlers::ConstantPrompts::cropFilterWidth_Arg(size_t imgW, size_t vertexCN)
{
    return {"Choose the width of the cropped image (1-" + to_string(imgW - (vertexCN)) + ") : "};
}


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

    Image* getImageInput(const string& prompt) {
        string filename = Utilities::Validations::v_ImgName(prompt, true);
        Image* img = new Image();
        *img = Utilities::importIMG(filename);
        return img;
    }

    void applyFilter(UI::FilterOption choice, Image* input1, Image* input2,
                     Image* output, const FilterParams& params)
                     {

        switch(choice) {
            case UI::FilterOption::GrayScale:
                Filters::grayScale(*input1, *output);
                break;
            case UI::FilterOption::BlackWhite:
                Filters::Black_White(*input1, *output);
                break;
            case UI::FilterOption::Invert:
                Filters::invert(*input1, *output);
                break;
            case UI::FilterOption::Merge:
                Filters::merge(*input1, *input2, *output, params.resizeFlag);
                break;
            case UI::FilterOption::Flip:
                Filters::flip(*input1, *output, params.direction);
                break;
            case UI::FilterOption::Rotate:
                Filters::rotate(*input1, *output, params.rotation);
                break;
            case UI::FilterOption::Exposure:
                Filters::exposure(*input1, *output, params.lighten);
                break;
            case UI::FilterOption::Crop:
                Filters::crop(*input1, *output, params.cropX, params.cropY);
                break;
            case UI::FilterOption::Frame:
                Filters::frame(*input1, *output, params.fancy, params.color, FilterConstants::FRAME_COLORS);
                break;
            case UI::FilterOption::Edges:
                Filters::edges(*input1, *output);
                break;
            case UI::FilterOption::Resize:
                Filters::resize(*input1, *output);
                break;
            case UI::FilterOption::Blur:
                Filters::blur(*input1, *output, params.blurRadius, params.callNum);
                break;
            case UI::FilterOption::Sunlight:
                Filters::sunlight(*input1, *output);
                break;
            case UI::FilterOption::OilPainting:
                Filters::oilPainting(*input1, *output);
                break;
            case UI::FilterOption::OldTV:
                Filters::oldTV(*input1, *output);
                break;
        }
    }
}

using namespace PromptHandlers;

void UI::Run() {
    while(true) {
        U8 choice = Utilities::Validations::v_numericalInput(
                FilterConstants::FILTER_MENU, 1, 16
        );
        FilterOption filter = static_cast<FilterOption>(choice);
        if(filter == FilterOption::Quit) break;

        image_1_filename = Utilities::Validations::v_ImgName(ConstantPrompts::image_1_prompt(choice), true);
        inputPhoto1 = new Photo(new Image(Utilities::importIMG(image_1_filename)));

        if (choice == (int) FilterOption::Merge)
        {
            image_2_filename = Utilities::Validations::v_ImgName(image_2_filename, true);
            inputPhoto2 = new Photo(new Image(Utilities::importIMG(image_2_filename)));
        }

        FilterParams params;
        int color;

        switch ((FilterOption) choice)
        {
            case FilterOption::Merge:
                params.resize_or_not =
                        Utilities::Validations::v_numericalInput(ConstantPrompts::mergeFilterArg, 0, 1);
                break;

            case FilterOption::Flip:
                params.horizontal_or_vertical = Utilities::Validations::v_numericalInput(ConstantPrompts::flipFilterArg, 1, 2);
                break;
            case FilterOption::Rotate:
                params.rotation_angle = Utilities::Validations::v_numericalInput(ConstantPrompts::rotateFilterArg,{90, 180, 270});
                break;
            case FilterOption::Exposure:
                params.lighten = Utilities::Validations::v_numericalInput(ConstantPrompts::exposureFilterArg, 0, 1);
                break;

            case FilterOption::Crop:
                // Get user choice for crop filter
                params.vertex_row_no = Utilities::Validations::v_numericalInput(filter_8_row_prompt, 1, input_image_1.height) - 1;
                params.vertex_col_no = Utilities::Validations::v_numericalInput(filter_8_column_prompt, 1, input_image_1.width) - 1;
                params.crop_height = Utilities::Validations::v_numericalInput(filter_8_height_prompt, 1, input_image_1.height - (vertex_row_no));
                params.crop_width = Utilities::Validations::v_numericalInput(filter_8_width_prompt, 1, input_image_1.width - (vertex_col_no));
                break;
        }

    }
}

UI::~UI()
{
    if (inputPhoto1) delete inputPhoto1;
    if (inputPhoto2) delete inputPhoto2;
    if (outputPhoto) delete outputPhoto;
}
