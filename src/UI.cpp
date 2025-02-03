#include "UI.h"
#include "Utilities.h"
#include <unordered_map>

using namespace std;

void UI::Run()
{
    while (true)
    {
        int choice;
        // Menu prompt for filter selection
        string filter_prompt = "\nAvailable Filters: \n"
                               " 1)  Gray Scale \n 2)  Black-and-White \n 3)  Invert \n 4)  Merge \n 5)  Flip \n 6)  Rotate \n"
                               " 7)  Lighten/Darken \n 8)  Crop \n 9)  Frame \n 10) Detect Edges \n 11) Resize \n 12) Blur \n"
                               " 13) Sunlight \n 14) Oil Painting \n 15) Old TV \n 16) Quit \n\n"
                               "Choose a filter, or quit (1-16): ";
        // Get user choice
        choice = Utilities::Validations::v_numericalInput(filter_prompt, 1, 16);
        if (choice == 16) break; // Quit program if choice is 16
        // Get input image filenames based on choice
        string image_1_prompt = "Please enter the filename (NOT FILEPATH, including extension) of the " + string(choice == 4 ? "first " : "") + "image (must exist in this directory) " + "\n" + "(Available formats: .jpg, .png, .bmp, .tga): ";
        string image_1_filename = Utilities::Validations::v_ImgName(image_1_prompt, true);
        Image input_image_1 = Utilities::importIMG(image_1_filename);
        // Get the second image for merge filter
        Image empty_image_object;
        Image input_image_2 = empty_image_object;
        if (choice == 4)
        {
            string image_2_prompt = "Please enter the filename (NOT FILEPATH, including extension) of the second image (must exist in this directory) \n(Available formats: .jpg, .png, .bmp, .tga): ";
            string image_2_filename = Utilities::Validations::v_ImgName(image_2_prompt, true);
            input_image_2 = Utilities::importIMG(image_2_filename);
        }
        // Get additional inputs based on filter choice
        int resize_or_not;
        int horizontal_or_vertical;
        int rotation_angle;
        int lighten;
        int vertex_row_no, vertex_col_no, crop_width, crop_height;
        int fancy;
        int resize_width, resize_height;
        // Frame Colors (in order): black, white, brown, red, green, blue, yellow
        map < int, vector <int> > color_to_rgb = { {1, {0, 0, 0}}, {2, {255, 255, 255}}, {3, {80, 40, 0}}, {4, {255, 0, 0}}, {5, {0, 255, 0}}, {6, {0, 0, 255}}, {7, {255, 255, 0}} };
        int color;
        switch (choice)
        {
            // Cases for different filters
            case 4:
            {
                // Get user choice for merge filter
                string filter_4_arg_prompt = "Choose whether to crop and merge the common part of the images" + string("\n") + "or resize them to the same size (0: Crop, 1: Resize): ";
                resize_or_not = Utilities::Validations::v_numericalInput(filter_4_arg_prompt, 0, 1);
                break;
            }
            case 5:
            {
                // Get user choice for flip filter
                string filter_5_arg_prompt = "Choose whether to flip the image (1: Horizontally, 2: Vertically): ";
                horizontal_or_vertical = Utilities::Validations::v_numericalInput(filter_5_arg_prompt, 1, 2);
                break;
            }
            case 6:
            {
                // Get user choice for rotation angle
                string filter_6_arg_prompt = "Choose a CLOCKWISE rotation angle (90/180/270): ";
                rotation_angle = Utilities::Validations::v_numericalInput(filter_6_arg_prompt, vector <int> {90, 180, 270});
                break;
            }
            case 7:
            {
                // Get user choice for lighten/darken filter
                string filter_7_arg_prompt = "Choose whether to lighten or darken the image (1 to lighten, 0 to darken): ";
                lighten = Utilities::Validations::v_numericalInput(filter_7_arg_prompt, 0, 1);
                break;
            }
            case 8: {
                // Get user choice for crop filter
                string filter_8_row_prompt = "Choose the row number (top-to-bottom) to start cropping at (1-" + to_string(input_image_1.height) + ") : ";
                vertex_row_no = Utilities::Validations::v_numericalInput(filter_8_row_prompt, 1, input_image_1.height) - 1;
                string filter_8_column_prompt = "Choose the column number (left-to-right) to start cropping at (1-" + to_string(input_image_1.width) + ") : ";
                vertex_col_no = Utilities::Validations::v_numericalInput(filter_8_column_prompt, 1, input_image_1.width) - 1;
                string filter_8_height_prompt = "Choose the height of the cropped image (1-" + to_string(input_image_1.height - (vertex_row_no)) + ") : ";
                crop_height = Utilities::Validations::v_numericalInput(filter_8_height_prompt, 1, input_image_1.height - (vertex_row_no));
                string filter_8_width_prompt = "Choose the width of the cropped image (1-" + to_string(input_image_1.width - (vertex_col_no)) + ") : ";
                crop_width = Utilities::Validations::v_numericalInput(filter_8_width_prompt, 1, input_image_1.width - (vertex_col_no));
                break;
            }
            case 9:
            {
                // Get user choice for frame filter
                string filter_9_arg_prompt = "Choose whether to apply a simple or fancy frame (0 for simple, 1 for fancy): ";
                fancy = Utilities::Validations::v_numericalInput(filter_9_arg_prompt, 0, 1);
                string filter_9_color_prompt = "Available Colors: \n"
                                               " 1) Black \n 2) White \n 3) Brown \n 4) Red \n 5) Green \n 6) Blue \n"
                                               " 7) Yellow \n\nChoose a color (1-7): ";
                color = Utilities::Validations::v_numericalInput(filter_9_color_prompt, 1, 7);
                break;
            }
            case 11:
            {
                // Get user choice for resize filter
                string filter_18_width_prompt = "Choose the width of the rescaled image: ";
                resize_width = Utilities::Validations::v_numericalInput(filter_18_width_prompt, 1);
                string filter_18_height_prompt = "Choose the height of the rescaled image: ";
                resize_height = Utilities::Validations::v_numericalInput(filter_18_height_prompt, 1);
                break;
            }
        }
        // Determine output dimensions based on filter choice
        int output_width, output_height;
        switch (choice)
        {
            case 4:
            {
                // Calculate output dimensions for merge filter
                if (resize_or_not == 1)
                {
                    output_height = max(input_image_1.height, input_image_2.height);
                    output_width = max(input_image_1.width, input_image_2.width);
                }
                else
                {
                    output_height = min(input_image_1.height, input_image_2.height);
                    output_width = min(input_image_1.width, input_image_2.width);
                }
                break;
            }
            case 6:
            {
                // Calculate output dimensions for rotation filter
                if (rotation_angle == 90 || rotation_angle == 270)
                {
                    output_height = input_image_1.width;
                    output_width = input_image_1.height;
                }
                else
                {
                    output_height = input_image_1.height;
                    output_width = input_image_1.width;
                }
                break;
            }
            case 8:
                // Set output dimensions for crop filter
                output_height = crop_height;
                output_width = crop_width;
                break;
            case 11:
                // Set output dimensions for resize filter
                output_height = resize_height;
                output_width = resize_width;
                break;
            default:
                // Default output dimensions
                output_height = input_image_1.height;
                output_width = input_image_1.width;
                break;
        }
        // Create output image object
        Image output_image(output_width, output_height);
        // Apply selected filter
        cout << endl << "Applying the filter.. " << endl;
        switch (choice)
        {
            case 1:
                Filters::grayScale(input_image_1, output_image);
                break;
            case 2:
                Filters::Black_White(input_image_1, output_image);
                break;
            case 3:
                Filters::invert(input_image_1, output_image);
                break;
            case 4:
                Filters::merge(input_image_1, input_image_2, output_image, resize_or_not);
                break;
            case 5:
                Filters::flip(input_image_1, output_image, horizontal_or_vertical);
                break;
            case 6:
                Filters::rotate(input_image_1, output_image, rotation_angle);
                break;
            case 7:
                Filters::exposure(input_image_1, output_image, lighten);
                break;
            case 8:
                Filters::crop(input_image_1, output_image, vertex_row_no, vertex_col_no);
                break;
            case 9:
                Filters::frame(input_image_1, output_image, fancy, color, color_to_rgb);
                break;
            case 10:
                Filters::edges(input_image_1, output_image);
                break;
            case 11:
                Filters::resize(input_image_1, output_image);
                break;
            case 12:
                Filters::blur(input_image_1, output_image, sqrt((input_image_1.height * input_image_1.width))/160, 1);
                Filters::blur(output_image, output_image, sqrt((input_image_1.height * input_image_1.width))/80, 2);
                break;
            case 13:
                Filters::sunlight(input_image_1, output_image);
                break;
            case 14:
                Filters::oilPainting(input_image_1, output_image);
                break;
            case 15:
                Filters::oldTV(input_image_1, output_image);
                break;
        }
        cout << endl;
        // Save the output image
        Utilities::exportIMG(output_image);
        cout << endl << "---------------------------------------------------------------------------------------------------------"
             << endl << "---------------------------------------------------------------------------------------------------------";
    }
}