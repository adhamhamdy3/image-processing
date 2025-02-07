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

const string PromptHandlers::ConstantPrompts::frameFilterArg = "Choose whether to apply a simple or fancy frame "
                                                               "(0 for simple, 1 for fancy): ";

const string PromptHandlers::ConstantPrompts::frameFilterColorArg = "Available Colors: \n"
                                                                    " 1) Black \n 2) White \n 3) Brown \n 4) Red \n 5) Green \n 6) Blue \n"
                                                                    " 7) Yellow \n\nChoose a color (1-7): ";

const string PromptHandlers::ConstantPrompts::resizeFilter_wArg = "Choose the width of the rescaled image: ";

const string PromptHandlers::ConstantPrompts::resizeFilter_hArg = "Choose the height of the rescaled image: ";


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

//    Image* getImageInput(const string& prompt) {
//        string filename = Utilities::Validations::v_ImgName(prompt, true);
//        Image* img = new Image();
//        *img = Utilities::importIMG(filename);
//        return img;
//    }

    void applyFilter(UI::FilterOption choice, Photo* input1, Photo* input2,
                     Photo* output, const FilterParams& params, int color)
                     {

        switch(choice) {
            case UI::FilterOption::GrayScale:
                Filters::grayScale(*input1->currentImage, *output->currentImage);
                input1->pushChanges();
                break;
            case UI::FilterOption::BlackWhite:
                Filters::Black_White(*input1->currentImage, *output->currentImage);
                input1->pushChanges();
                break;
            case UI::FilterOption::Invert:
                Filters::invert(*input1->currentImage, *output->currentImage);
                input1->pushChanges();
                break;
            case UI::FilterOption::Merge:
                Filters::merge(*input1->currentImage, *input2->currentImage, *output->currentImage, params.resize_or_not);
                input1->pushChanges();
                break;
            case UI::FilterOption::Flip:
                Filters::flip(*input1->currentImage, *output->currentImage, params.horizontal_or_vertical);
                input1->pushChanges();
                break;
            case UI::FilterOption::Rotate:
                Filters::rotate(*input1->currentImage, *output->currentImage, params.rotation_angle);
                input1->pushChanges();
                break;
            case UI::FilterOption::Exposure:
                Filters::exposure(*input1->currentImage, *output->currentImage, params.lighten);
                input1->pushChanges();
                break;
            case UI::FilterOption::Crop:
                Filters::crop(*input1->currentImage, *output->currentImage, params.vertex_row_no, params.vertex_col_no);
                input1->pushChanges();
                break;
            case UI::FilterOption::Frame:
                Filters::frame(*input1->currentImage, *output->currentImage, params.fancy, color, FilterConstants::FRAME_COLORS);
                input1->pushChanges();
                break;
            case UI::FilterOption::Edges:
                Filters::edges(*input1->currentImage, *output->currentImage);
                break;
            case UI::FilterOption::Resize:
                Filters::resize(*input1->currentImage, *output->currentImage);
                break;
            case UI::FilterOption::Blur:
                Filters::blur(*input1->currentImage, *output->currentImage, sqrt((input1->currentImage->height * input1->currentImage->width))/160, 1);
                Filters::blur(*output->currentImage, *output->currentImage, sqrt((input1->currentImage->height * input1->currentImage->width))/80, 2);
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

using namespace PromptHandlers;
using namespace ConstantPrompts;

void UI::Run()
{
    while(true)
    {
        U8 choice = Utilities::Validations::v_numericalInput(
                FilterConstants::FILTER_MENU, 1, 16
        );
        FilterOption filter = static_cast<FilterOption>(choice);
        if(filter == FilterOption::Quit) break;

        image_1_filename = Utilities::Validations::v_ImgName(image_1_prompt(choice), true);
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
                        Utilities::Validations::v_numericalInput(mergeFilterArg, 0, 1);
                break;

            case FilterOption::Flip:
                params.horizontal_or_vertical = Utilities::Validations::v_numericalInput(flipFilterArg, 1, 2);
                break;
            case FilterOption::Rotate:
                params.rotation_angle = Utilities::Validations::v_numericalInput(rotateFilterArg,{90, 180, 270});
                break;
            case FilterOption::Exposure:
                params.lighten = Utilities::Validations::v_numericalInput(exposureFilterArg, 0, 1);
                break;

            case FilterOption::Crop:
            {
                string cropRow_P = cropFilterRow_Arg(inputPhoto1->currentImage->height);
                params.vertex_row_no = Utilities::Validations::v_numericalInput(cropRow_P, 1, inputPhoto1->currentImage->height) - 1;

                string cropCol_P = cropFilterCol_Arg(inputPhoto1->currentImage->width);
                params.vertex_col_no = Utilities::Validations::v_numericalInput(cropCol_P, 1, inputPhoto1->currentImage->width) - 1;

                string cropH_P = cropFilterHeight_Arg(inputPhoto1->currentImage->height, params.vertex_row_no);
                params.crop_height = Utilities::Validations::v_numericalInput(cropH_P, 1, inputPhoto1->currentImage->height - (params.vertex_row_no));

                string cropW_P = cropFilterWidth_Arg(inputPhoto1->currentImage->width, params.vertex_col_no);
                params.crop_width = Utilities::Validations::v_numericalInput(cropW_P, 1, inputPhoto1->currentImage->width - (params.vertex_col_no));
                break;
            }

            case FilterOption::Frame:
                params.fancy = Utilities::Validations::v_numericalInput(frameFilterArg, 0, 1);
                color = Utilities::Validations::v_numericalInput(frameFilterColorArg, 1, 7);
                break;

            case FilterOption::Resize:
                params.resize_width = Utilities::Validations::v_numericalInput(resizeFilter_wArg, 1);
                params.resize_height = Utilities::Validations::v_numericalInput(resizeFilter_hArg, 1);
                break;
        }
        // Determine output dimensions based on filter choice
        size_t outputWidth, outputHeight;
        switch ((FilterOption) choice)
        {
            case FilterOption::Merge:
            {
                if (params.resize_or_not == 1)
                {
                    outputHeight = max(inputPhoto1->currentImage->height, inputPhoto2->currentImage->height);
                    outputWidth = max(inputPhoto1->currentImage->width, inputPhoto2->currentImage->width);
                }
                else
                {
                    outputHeight = min(inputPhoto1->currentImage->height, inputPhoto2->currentImage->height);
                    outputWidth = min(inputPhoto1->currentImage->height, inputPhoto2->currentImage->height);
                }
                break;
            }
            case FilterOption::Rotate:
            {
                // Calculate output dimensions for rotation filter
                if (params.rotation_angle == 90 || params.rotation_angle == 270)
                {
                    outputHeight = inputPhoto1->currentImage->width;
                    outputWidth = inputPhoto1->currentImage->height;
                }
                else
                {
                    outputHeight = inputPhoto1->currentImage->height;
                    outputWidth = inputPhoto1->currentImage->width;
                }
                break;
            }
            case FilterOption::Crop:
                // Set output dimensions for crop filter
                outputHeight = params.crop_height;
                outputWidth = params.crop_width;
                break;
            case FilterOption::Resize:
                // Set output dimensions for resize filter
                outputHeight = params.resize_height;
                outputWidth = params.resize_width;
                break;
            default:
                // Default output dimensions
                outputHeight = inputPhoto1->currentImage->height;
                outputWidth = inputPhoto1->currentImage->width;
                break;
        }

        // Create output image object
        outputPhoto = new Photo(new Image(outputWidth, outputHeight));
        // Apply selected filter
        cout << endl << "Applying the filter.. " << endl;

        applyFilter((FilterOption) choice, inputPhoto1, inputPhoto2, outputPhoto, params, color);

        cout << endl;
        // Save the output image
        Utilities::exportIMG(*outputPhoto->currentImage);
        cout << endl << "---------------------------------------------------------------------------------------------------------"
             << endl << "---------------------------------------------------------------------------------------------------------";

    }
}

UI::~UI()
{
    if (inputPhoto1) delete inputPhoto1;
    if (inputPhoto2) delete inputPhoto2;
    if (outputPhoto) delete outputPhoto;
}
