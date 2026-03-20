#include "Utilities.h"
#include "UI.h"
#include <iostream>

using namespace std;
using namespace PromptHandlers;
using namespace ConstantPrompts;

void UI::runFilterLoop()
{
    while (true)
    {
        int choice = Utilities::Validations::v_numericalInput(
            FilterConstants::FILTER_MENU, 1, 16);
        FilterOption filter = static_cast<FilterOption>(choice);

        if (filter == FilterOption::Quit)
        {
            cout << "\nReturning to main menu...\n";
            break;
        }

        img1_fileName = Utilities::Validations::v_ImgName(img1_Prompt((U8)choice), true);
        inputPhoto1 = new Photo(new Image(Utilities::importIMG(img1_fileName)));

        if (choice == (int)FilterOption::Merge)
        {
            img2_fileName = Utilities::Validations::v_ImgName(img2_Prompt, true);
            inputPhoto2 = new Photo(new Image(Utilities::importIMG(img2_fileName)));
        }

        FilterParams params;
        int color = 1;

        switch (filter)
        {
        case FilterOption::Merge:
            params.resize_or_not =
                Utilities::Validations::v_numericalInput(mergeFilterArg, 0, 1);
            break;

        case FilterOption::Flip:
            params.horizontal_or_vertical =
                Utilities::Validations::v_numericalInput(flipFilterArg, 1, 2);
            break;

        case FilterOption::Rotate:
            params.rotation_angle =
                Utilities::Validations::v_numericalInput(rotateFilterArg, {90, 180, 270});
            break;

        case FilterOption::Exposure:
            params.lighten =
                Utilities::Validations::v_numericalInput(exposureFilterArg, 0, 1);
            break;

        case FilterOption::Crop:
        {
            string cropRow_P = cropFilterRow_Arg(inputPhoto1->currentImage->height);
            params.vertex_row_no =
                Utilities::Validations::v_numericalInput(cropRow_P, 1, inputPhoto1->currentImage->height) - 1;

            string cropCol_P = cropFilterCol_Arg(inputPhoto1->currentImage->width);
            params.vertex_col_no =
                Utilities::Validations::v_numericalInput(cropCol_P, 1, inputPhoto1->currentImage->width) - 1;

            string cropH_P = cropFilterHeight_Arg(inputPhoto1->currentImage->height, params.vertex_row_no);
            params.crop_height =
                Utilities::Validations::v_numericalInput(cropH_P, 1,
                    inputPhoto1->currentImage->height - params.vertex_row_no);

            string cropW_P = cropFilterWidth_Arg(inputPhoto1->currentImage->width, params.vertex_col_no);
            params.crop_width =
                Utilities::Validations::v_numericalInput(cropW_P, 1,
                    inputPhoto1->currentImage->width - params.vertex_col_no);
            break;
        }

        case FilterOption::Frame:
            params.fancy = Utilities::Validations::v_numericalInput(frameFilterArg, 0, 1);
            color        = Utilities::Validations::v_numericalInput(frameFilterColorArg, 1, 7);
            break;

        case FilterOption::Resize:
            params.resize_width  = Utilities::Validations::v_numericalInput(resizeFilter_wArg, 1);
            params.resize_height = Utilities::Validations::v_numericalInput(resizeFilter_hArg, 1);
            break;

        default:
            break;
        }

        // compute output dimensions based on filter type
        size_t outputWidth, outputHeight;
        switch (filter)
        {
        case FilterOption::Merge:
            if (params.resize_or_not == 1)
            {
                outputHeight = max(inputPhoto1->currentImage->height, inputPhoto2->currentImage->height);
                outputWidth  = max(inputPhoto1->currentImage->width,  inputPhoto2->currentImage->width);
            }
            else
            {
                outputHeight = min(inputPhoto1->currentImage->height, inputPhoto2->currentImage->height);
                outputWidth  = min(inputPhoto1->currentImage->width,  inputPhoto2->currentImage->width);
            }
            break;

        case FilterOption::Rotate:
            if (params.rotation_angle == 90 || params.rotation_angle == 270)
            {
                outputHeight = inputPhoto1->currentImage->width;
                outputWidth  = inputPhoto1->currentImage->height;
            }
            else
            {
                outputHeight = inputPhoto1->currentImage->height;
                outputWidth  = inputPhoto1->currentImage->width;
            }
            break;

        case FilterOption::Crop:
            outputHeight = params.crop_height;
            outputWidth  = params.crop_width;
            break;

        case FilterOption::Resize:
            outputHeight = params.resize_height;
            outputWidth  = params.resize_width;
            break;

        default:
            outputHeight = inputPhoto1->currentImage->height;
            outputWidth  = inputPhoto1->currentImage->width;
            break;
        }

        outputPhoto = new Photo(new Image(outputWidth, outputHeight));

        cout << "\nApplying the filter...\n";
        applyFilter(filter, inputPhoto1, inputPhoto2, outputPhoto, params, color);
        cout << "\n";

        if (filter == FilterOption::Merge)
            Utilities::exportIMG(*outputPhoto->currentImage);
        else
            Utilities::exportIMG(*inputPhoto1->currentImage);

        cout << "\n----------------------------------------------------------\n";

        cleanUp();
    }
}

void UI::Run()
{
    while (true)
    {
        int mainChoice = Utilities::Validations::v_numericalInput(
            FilterConstants::MAIN_MENU, 1, 3);
        MainMenuOption option = static_cast<MainMenuOption>(mainChoice);

        switch (option)
        {
        case MainMenuOption::ApplyFilter:
            runFilterLoop();
            break;

        case MainMenuOption::Quit:
            cout << "\nGoodbye!\n";
            return;
        }
    }
}

UI::~UI()
{
    cleanUp();
}

void UI::cleanUp()
{
    if (inputPhoto1)
    {
        delete inputPhoto1;
        inputPhoto1 = nullptr;
    }
    if (inputPhoto2)
    {
        delete inputPhoto2;
        inputPhoto2 = nullptr;
    }
    if (outputPhoto)
    {
        delete outputPhoto;
        outputPhoto = nullptr;
    }
}