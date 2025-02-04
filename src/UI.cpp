#include "UI.h"
#include "Utilities.h"
using namespace std;

// Constants initialization
const unordered_map<int, vector<int>> UI::FilterConstants::FRAME_COLORS = {
        {1, {0,0,0}}, {2, {255,255,255}}, {3, {80,40,0}},
        {4, {255,0,0}}, {5, {0,255,0}}, {6, {0,0,255}}, {7, {255,255,0}}
};

const string UI::FilterConstants::FILTER_MENU =
        "\nAvailable Filters: \n"
        " 1)  Gray Scale \n 2)  Black-and-White \n 3)  Invert \n 4)  Merge \n"
        " 5)  Flip \n 6)  Rotate \n 7)  Lighten/Darken \n 8)  Crop \n"
        " 9)  Frame \n10) Detect Edges \n11) Resize \n12) Blur \n"
        "13) Sunlight \n14) Oil Painting \n15) Old TV \n16) Quit \n\n"
        "Choose a filter (1-16): ";


namespace {
    struct FilterParams {
        int rotation = 0;
        int lighten = 0;
        int cropX = 0, cropY = 0, cropW = 0, cropH = 0;
        int color = 0, direction = 0, resizeFlag = 0, fancy = 0;
        int newWidth = 0, newHeight = 0;
        int blurRadius = 0, callNum = 0;
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
        using namespace UI;

        switch(choice) {
            case FilterOption::GrayScale:
                Filters::grayScale(*input1, *output);
                break;
            case FilterOption::BlackWhite:
                Filters::Black_White(*input1, *output);
                break;
            case FilterOption::Invert:
                Filters::invert(*input1, *output);
                break;
            case FilterOption::Merge:
                Filters::merge(*input1, *input2, *output, params.resizeFlag);
                break;
            case FilterOption::Flip:
                Filters::flip(*input1, *output, params.direction);
                break;
            case FilterOption::Rotate:
                Filters::rotate(*input1, *output, params.rotation);
                break;
            case FilterOption::Exposure:
                Filters::exposure(*input1, *output, params.lighten);
                break;
            case FilterOption::Crop:
                Filters::crop(*input1, *output, params.cropX, params.cropY);
                break;
            case FilterOption::Frame:
                Filters::frame(*input1, *output, params.fancy, params.color, FilterConstants::FRAME_COLORS);
                break;
            case FilterOption::Edges:
                Filters::edges(*input1, *output);
                break;
            case FilterOption::Resize:
                Filters::resize(*input1, *output);
                break;
            case FilterOption::Blur:
                Filters::blur(*input1, *output, params.blurRadius, params.callNum);
                break;
            case FilterOption::Sunlight:
                Filters::sunlight(*input1, *output);
                break;
            case FilterOption::OilPainting:
                Filters::oilPainting(*input1, *output);
                break;
            case FilterOption::OldTV:
                Filters::oldTV(*input1, *output);
                break;
        }
    }
}

void UI::Run() {
    while(true) {
        int choice = Utilities::Validations::v_numericalInput(
                FilterConstants::FILTER_MENU, 1, 16
        );
        FilterOption filter = static_cast<FilterOption>(choice);
        if(filter == FilterOption::Quit) break;

        // Memory management with raw pointers
        Image* input1 = nullptr;
        Image* input2 = nullptr;
        Image* output = nullptr;

        try {
            input1 = getImageInput("Enter first image filename: ");

            if(filter == FilterOption::Merge) {
                input2 = getImageInput("Enter second image filename: ");
            }

            FilterParams params;
            // Parameter collection logic here...

            // Create output image
            output = new Image(input1->width, input1->height);

            applyFilter(filter, input1, input2, output, params);
            Utilities::exportIMG(*output);
        }
        catch(...) {
            // Cleanup on error
            delete input1;
            delete input2;
            delete output;
            throw;
        }

        // Manual cleanup
        delete input1;
        delete input2;
        delete output;
    }
}