#include "DEFs.h"
#include "Utilities.h"
#include "UI.h"


using namespace std;
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

        img1_fileName = Utilities::Validations::v_ImgName(img1_Prompt(choice), true);
        inputPhoto1 = new Photo(new Image(Utilities::importIMG(img1_fileName)));

        if (choice == (int) FilterOption::Merge)
        {
            img2_fileName = Utilities::Validations::v_ImgName(img2_Prompt, true);
            inputPhoto2 = new Photo(new Image(Utilities::importIMG(img2_fileName)));
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

       this->cleanUp();
    }
}

UI::~UI()
{
    this->cleanUp();
}

void UI::cleanUp() {
    if (inputPhoto1) {
        delete inputPhoto1;
        inputPhoto1 = nullptr;
    }
    if (inputPhoto2) {
        delete inputPhoto2;
        inputPhoto2 = nullptr;
    }
    if (outputPhoto)
    {
        delete outputPhoto;
        outputPhoto = nullptr;
    }
}
