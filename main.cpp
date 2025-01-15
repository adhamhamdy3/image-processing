// Program Discription : This program do the Hero level and 15 filters of baby photoshop in assignment 3

// Last Modification Date : 10/4/2024

// Author1 : Adham Hamdy
// Author2 : Eslam Saeed
// Author3 : Andrew Wafae

// Eslam Saeed :  2, 5, 8, 11
// Adham Hamdy :  1, 4, 7, 10, 13
// Andrew Wafae : 3, 6, 9, 12, 14, 15

// GUI Made by: Adham Hamdy

// System Diagram
// https://bit.ly/3Uj0Uda

#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <regex>
#include <iomanip>
#include <random>
#include <algorithm>
#include "Image_Class.h"
using namespace std;


Image load_image(const string& filename);
void save_image(Image& output_image);

int get_int(const string& prompt, int l = INT_MIN, int r = INT_MAX);
int get_int(const string& prompt, vector <int> value_list);
string get_valid_image_filename(const string& prompt, bool existing = false);

void display_progress_bar(int current_index, int max_index, string optional_title = "");

void grayscale(Image& input_image, Image& output_image);
void black_and_white(Image& input_image, Image& output_image);
void invert(Image& input_image, Image& output_image);
void merge(Image& input_image_1, Image& input_image_2, Image& output_image, const int& resize_or_not);
void flip(Image& input_image, Image& output_image, const int& horizontal_or_vertical);
void rotate(Image& input_image, Image& output_image, const int& rotation_angle);
void lighten_or_darken(Image& input_image, Image& output_image, const int& lighten);
void crop(Image& input_image, Image& output_image, const int& vertex_row_no, const int& vertex_col_no);
void frame(Image& input_image, Image& output_image, const int& fancy, const int& color, const map < int, vector <int> >& color_to_rgb);
void edges(Image& input_image, Image& output_image);
Image& resize(Image& input_image, Image& output_image);
void blur(Image& input_image, Image& output_image, const int& blur_radius, const int& call_no);
void sunlight(Image& input_image, Image& output_image);
void oil_painting(Image& input_image, Image& output_image);
void old_tv(Image& input_image, Image& output_image);

int main()
{
    // Infinite loop for menu-driven program
    while (true)
    {
        int choice;
        // Menu prompt for filter selection
        string filter_prompt = "\nAvailable Filters: \n"
                               " 1)  Grayscale \n 2)  Black-and-White \n 3)  Invert \n 4)  Merge \n 5)  Flip \n 6)  Rotate \n"
                               " 7)  Lighten/Darken \n 8)  Crop \n 9)  Frame \n 10) Detect Edges \n 11) Resize \n 12) Blur \n"
                               " 13) Sunlight \n 14) Oil Painting \n 15) Old TV \n 16) Quit \n\n"
                               "Choose a filter, or quit (1-16): ";
        // Get user choice
        choice = get_int(filter_prompt, 1, 16);

        if (choice == 16) return 0; // Quit program if choice is 16

        // Get input image filenames based on choice
        string image_1_prompt = "Please enter the filename (NOT FILEPATH, including extension) of the " + string(choice == 4 ? "first " : "") + "image (must exist in this directory) " + "\n" + "(Available formats: .jpg, .png, .bmp, .tga): ";
        string image_1_filename = get_valid_image_filename(image_1_prompt, true);
        Image input_image_1 = load_image(image_1_filename);

        // Get the second image for merge filter
        Image empty_image_object;
        Image input_image_2 = empty_image_object;
        if (choice == 4)
        {
            string image_2_prompt = "Please enter the filename (NOT FILEPATH, including extension) of the second image (must exist in this directory) \n(Available formats: .jpg, .png, .bmp, .tga): ";
            string image_2_filename = get_valid_image_filename(image_2_prompt, true);
            input_image_2 = load_image(image_2_filename);
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
                resize_or_not = get_int(filter_4_arg_prompt, 0, 1);
                break;
            }
            case 5:
            {
                // Get user choice for flip filter
                string filter_5_arg_prompt = "Choose whether to flip the image (1: Horizontally, 2: Vertically): ";
                horizontal_or_vertical = get_int(filter_5_arg_prompt, 1, 2);
                break;
            }
            case 6:
            {
                // Get user choice for rotation angle
                string filter_6_arg_prompt = "Choose a CLOCKWISE rotation angle (90/180/270): ";
                rotation_angle = get_int(filter_6_arg_prompt, vector <int> {90, 180, 270});
                break;
            }
            case 7:
            {
                // Get user choice for lighten/darken filter
                string filter_7_arg_prompt = "Choose whether to lighten or darken the image (1 to lighten, 0 to darken): ";
                lighten = get_int(filter_7_arg_prompt, 0, 1);
                break;
            }
            case 8: {
                // Get user choice for crop filter
                string filter_8_row_prompt = "Choose the row number (top-to-bottom) to start cropping at (1-" + to_string(input_image_1.height) + ") : ";
                vertex_row_no = get_int(filter_8_row_prompt, 1, input_image_1.height) - 1;
                string filter_8_column_prompt = "Choose the column number (left-to-right) to start cropping at (1-" + to_string(input_image_1.width) + ") : ";
                vertex_col_no = get_int(filter_8_column_prompt, 1, input_image_1.width) - 1;
                string filter_8_height_prompt = "Choose the height of the cropped image (1-" + to_string(input_image_1.height - (vertex_row_no)) + ") : ";
                crop_height = get_int(filter_8_height_prompt, 1, input_image_1.height - (vertex_row_no));
                string filter_8_width_prompt = "Choose the width of the cropped image (1-" + to_string(input_image_1.width - (vertex_col_no)) + ") : ";
                crop_width = get_int(filter_8_width_prompt, 1, input_image_1.width - (vertex_col_no));
                break;
            }
            case 9:
            {
                // Get user choice for frame filter
                string filter_9_arg_prompt = "Choose whether to apply a simple or fancy frame (0 for simple, 1 for fancy): ";
                fancy = get_int(filter_9_arg_prompt, 0, 1);
                string filter_9_color_prompt = "Available Colors: \n"
                                               " 1) Black \n 2) White \n 3) Brown \n 4) Red \n 5) Green \n 6) Blue \n"
                                               " 7) Yellow \n\nChoose a color (1-7): ";
                color = get_int(filter_9_color_prompt, 1, 7);
                break;
            }
            case 11:
            {
                // Get user choice for resize filter
                string filter_18_width_prompt = "Choose the width of the rescaled image: ";
                resize_width = get_int(filter_18_width_prompt, 1);
                string filter_18_height_prompt = "Choose the height of the rescaled image: ";
                resize_height = get_int(filter_18_height_prompt, 1);
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
                grayscale(input_image_1, output_image);
                break;
            case 2:
                black_and_white(input_image_1, output_image);
                break;
            case 3:
                invert(input_image_1, output_image);
                break;
            case 4:
                merge(input_image_1, input_image_2, output_image, resize_or_not);
                break;
            case 5:
                flip(input_image_1, output_image, horizontal_or_vertical);
                break;
            case 6:
                rotate(input_image_1, output_image, rotation_angle);
                break;
            case 7:
                lighten_or_darken(input_image_1, output_image, lighten);
                break;
            case 8:
                crop(input_image_1, output_image, vertex_row_no, vertex_col_no);
                break;
            case 9:
                frame(input_image_1, output_image, fancy, color, color_to_rgb);
                break;
            case 10:
                edges(input_image_1, output_image);
                break;
            case 11:
                resize(input_image_1, output_image);
                break;
            case 12:
                blur(input_image_1, output_image, sqrt((input_image_1.height * input_image_1.width))/160, 1);
                blur(output_image, output_image, sqrt((input_image_1.height * input_image_1.width))/80, 2);
                break;
            case 13:
                sunlight(input_image_1, output_image);
                break;
            case 14:
                oil_painting(input_image_1, output_image);
                break;
            case 15:
                old_tv(input_image_1, output_image);
                break;
        }
        cout << endl;

        // Save the output image
        save_image(output_image);

        cout << endl << "---------------------------------------------------------------------------------------------------------"
             << endl << "---------------------------------------------------------------------------------------------------------";
    }
}

// Function to load an image from file
Image load_image(const string& input_filename)
{
    Image input_image(input_filename); // Create an image object from the filename
    return input_image; // Return the loaded image
}

// Function to save an image to file
void save_image(Image& output_image)
{
    string output_image_prompt = "Please enter the filename (NOT FILEPATH, including extension) to store the edited image \n(Available formats: .jpg, .png, .bmp, .tga): ";
    string output_filename = get_valid_image_filename(output_image_prompt); // Get a valid output filename
    cout << endl << "Saving.. " << endl;
    output_image.saveImage(output_filename); // Save the image
}

// Function to get a valid image filename
string get_valid_image_filename(const string& prompt, bool existing)
{
    freopen ("cerr_output.txt","w",stderr); // Redirect standard error to a file

    string filename;
    while (true)
    {
        cout << endl;
        cout << prompt; // Display the prompt
        string input; cin >> input; // Get user input
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        regex valid_image_filename (R"(\s*([^.\/]+\.(?:jpg|png|bmp|tga))\s*)"); // Define regex pattern for valid filename
        smatch matches;
        regex_match(input, matches, valid_image_filename); // Match input against pattern

        bool file_exists = true;
        if (existing)
        {
            try
            {
                Image test(matches[1].str()); // Attempt to load the image
            }
            catch(...)
            {
                file_exists = false; // Image file doesn't exist
            }
        }

        if (!matches.empty() && file_exists) // If input matches pattern and file exists
        {
            filename = matches[1].str(); // Extract filename
            break; // Exit loop
        }
        else
        {
            cerr << "Invalid filename. Please enter a valid filename with one of the following extensions: .jpg, .png, .bmp, .tga" << endl; // Display error message
        }
    }

    freopen ("/dev/tty","w",stderr); // Redirect standard error back to console

    return filename; // Return valid filename
}

// Function to get an integer within a specified range
int get_int(const string& prompt, int l, int r)
{
    int num; // Variable to store the integer
    while (true) // Continue until a valid input is received
    {
        try
        {
            cout << endl << prompt; // Display the input prompt
            string input; cin >> input; // Get user input
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

            long double num_d = stold(input); // Convert input to long double

            if (num_d != (long long)(num_d)) // Check if input is not an integer
            {
                cout << "Not an integer, please try again." << "\n"; // Display error message
                continue; // Continue to next iteration
            }
            else if (!(l <= num_d && num_d <= r)) // Check if input is outside the specified range
            {
                cout << "Please enter a number from " << l << " to " << r << " inclusive." << "\n"; // Display range error message
                continue; // Continue to next iteration
            }
            else
            {
                num = (int) num_d; // Convert long double to integer
                break; // Exit loop
            }
        }
        catch(...)
        {
            cout << "Not a number, please try again." << "\n"; // Display general error message
        }
    }
    return num; // Return the valid integer
}

// Function to get an integer from a list of values
int get_int(const string& prompt, vector<int> value_list)
{
    int num; // Variable to store the integer
    while (true) // Continue until a valid input is received
    {
        try
        {
            cout << endl << prompt; // Display the input prompt
            string input; cin >> input; // Get user input
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

            long double num_d = stold(input); // Convert input to long double

            if (num_d != (long long)(num_d)) // Check if input is not an integer
            {
                cout << "Not an integer, please try again." << "\n"; // Display error message
                continue; // Continue to next iteration
            }

            num = (int) num_d; // Convert long double to integer

            if (find(value_list.begin(), value_list.end(), num) == value_list.end()) // Check if input is not in the list
            {
                cout << "Please enter a number from (" ;
                for (int i = 0; i < value_list.size() - 1; i++) cout << value_list[i] << ", ";
                cout << value_list[value_list.size() - 1] << ")" << "\n"; // Display list error message
                continue; // Continue to next iteration
            }
            else
            {
                break; // Exit loop
            }
        }
        catch(...)
        {
            cout << "Not a number, please try again." << "\n"; // Display general error message
        }
    }
    return num; // Return the valid integer
}

// Function to display a progress bar
void display_progress_bar(int current_index, int max_index, string optional_title)
{
    int percentage = 100 * (current_index + 1) / (max_index + 1); // Calculate percentage
    cout << left << "\r" << optional_title << setfill('#') << setw(percentage / 5) << "" << setfill(' ') << setw(20 - percentage / 5) << "" << "| " << (percentage) << "%"; // Display progress bar
    cout.flush(); // Flush output
}

// Function to convert an image to grayscale
void grayscale(Image& input_image, Image& output_image)
{
    for (int i = 0; i < input_image.width; i++) // Loop through each pixel in width
    {
        display_progress_bar(i, input_image.width - 1); // Display progress bar

        for (int j = 0; j < input_image.height; j++) // Loop through each pixel in height
        {
            int avg = 0; // Initialize average value for grayscale calculation
            for (int k = 0; k < 3; k++) // Loop through RGB channels
            {
                avg += input_image(i, j, k); // Sum RGB values
            }
            avg /= 3; // Calculate average RGB value

            for(int k = 0; k < 3; k++) // Assign average value to each RGB channel in output image
            {
                output_image(i, j, k) = avg;
            }
        }
    }
}

// Function to convert an image to black and white
void black_and_white(Image& input_image, Image& output_image)
{
    for(int i = 0; i < input_image.width; i++) // Loop through each pixel in width
    {
        display_progress_bar(i, input_image.width - 1); // Display progress bar

        for (int j = 0; j < input_image.height; j++) // Loop through each pixel in height
        {
            int avg = 0; // Initialize average value for black and white calculation

            for (int k = 0; k < input_image.channels; k++) // Loop through channels
            {
                avg += input_image(i, j, k); // Sum channel values
            }

            avg /= 3; // Calculate average value

            // Convert to black or white based on average value
            (avg > 127) ? avg = 255 : avg = 0;

            for (int k = 0; k < 3; k++) // Assign black or white value to each RGB channel in output image
            {
                output_image(i, j, k) = avg;
            }
        }
    }
}

// Function to invert colors of an image
void invert(Image& input_image, Image& output_image)
{
    for (int i = 0; i < input_image.height; i++) // Loop through each pixel in height
    {
        display_progress_bar(i, input_image.height - 1); // Display progress bar

        for (int j = 0; j < input_image.width; j++) // Loop through each pixel in width
        {
            for (int k = 0; k < 3; k++) // Loop through RGB channels
            {
                output_image(j, i, k) = 255 - input_image(j, i, k); // Invert color values
            }
        }
    }
}

// Function to merge two images
void merge(Image& input_image_1, Image& input_image_2, Image& output_image, const int& resize_or_not)
{

    if (resize_or_not == 0) // If resizing is not required
    {
        for (int i = 0; i < output_image.width; ++i) // Loop through each pixel in width
        {
            display_progress_bar(i, output_image.width - 1); // Display progress bar
            for (int j = 0; j < output_image.height; ++j) // Loop through each pixel in height
            {
                for (int k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    unsigned int avg = (input_image_1(i, j, k) + input_image_2(i, j, k)) / 2; // Calculate average of corresponding pixels
                    output_image(i, j, k) = avg; // Assign average value to output image
                }
            }
        }
    }
    else // If resizing is required
    {
        // Resize the first image
        Image image_1_resized(output_image.width, output_image.height);
        cout << "--------------------------" << endl;
        cout << "Resizing the first image.. " << endl;
        image_1_resized = resize(input_image_1, image_1_resized);

        // Resize the second image
        Image image_2_resized(output_image.width, output_image.height);
        cout << endl << "Resizing the second image.. " << endl;
        image_2_resized = resize(input_image_2, image_2_resized);

        cout << endl << "Merging.. " << endl;
        for (int i = 0; i < output_image.width; ++i) // Loop through each pixel in width
        {
            display_progress_bar(i, output_image.width - 1); // Display progress bar
            for (int j = 0; j < output_image.height; ++j) // Loop through each pixel in height
            {
                for (int k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    unsigned int avg = (image_1_resized(i, j, k) + image_2_resized(i, j, k)) / 2; // Calculate average of corresponding pixels
                    output_image(i, j, k) = avg; // Assign average value to output image
                }
            }
        }
    }
}


// Function to flip an image horizontally or vertically
void flip(Image& input_image, Image& output_image, const int& horizontal_or_vertical)
{
    switch(horizontal_or_vertical)
    {
        case 1: // Horizontal flip
            for(int i = input_image.width-1; i >= 0; i--) // Loop through each pixel in reversed width
            {
                display_progress_bar((input_image.width-1) - i, input_image.width-1); // Display progress bar
                for(int j = 0; j < input_image.height; j++) // Loop through each pixel in height
                {
                    for (int k = 0; k < 3; k++) // Loop through RGB channels
                    {
                        output_image(input_image.width - 1 - i,j,k) = input_image(i,j,k); // Perform horizontal flip
                    }
                }
            }
            break;

        case 2: // Vertical flip
            for(int i = 0; i < input_image.width; i++) // Loop through each pixel in width
            {
                display_progress_bar(i, input_image.width - 1); // Display progress bar
                for(int j = input_image.height - 1; j >= 0; j--) // Loop through each pixel in reversed height
                {
                    for(int k = 0; k < 3; k++) // Loop through RGB channels
                    {
                        output_image(i,input_image.height - 1 - j,k) = input_image(i,j,k); // Perform vertical flip
                    }
                }
            }
            break;
    }
}

// Function to rotate an image by a specified angle
void rotate(Image& input_image, Image& output_image, const int& rotation_angle)
{
    for (int i = 0; i < input_image.height; i++) // Loop through each pixel in height
    {
        display_progress_bar(i, input_image.height - 1); // Display progress bar
        for (int j = 0; j < input_image.width; j++) // Loop through each pixel in width
        {
            for (int k = 0; k < 3; k++) // Loop through RGB channels
            {
                switch (rotation_angle) // Perform rotation based on specified angle
                {
                    case 90:
                        output_image(output_image.width - i - 1, j, k) = input_image(j, i, k); // Rotate 90 degrees clockwise
                        break;
                    case 180:
                        output_image(output_image.width - j - 1, output_image.height - i - 1, k) = input_image(j, i, k); // Rotate 180 degrees clockwise
                        break;
                    case 270:
                        output_image(i, output_image.height - j - 1, k) = input_image(j, i, k); // Rotate 270 degrees clockwise
                        break;
                }
            }
        }
    }
}

// Function to lighten or darken an image
void lighten_or_darken(Image& input_image, Image& output_image, const int& lighten)
{
    if (lighten) // Lighten image
    {
        for (int i = 0; i < input_image.width; ++i) // Loop through each pixel in width
        {
            display_progress_bar(i, input_image.width - 1); // Display progress bar
            for (int j = 0; j < input_image.height; ++j) // Loop through each pixel in height
            {
                for (int k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    int light = input_image(i, j, k) * 0.5; // Calculate lightening factor
                    output_image (i, j, k) = min(input_image(i, j, k) + light, 255); // Apply lightening effect
                }
            }
        }
    }
    else // Darken image
    {
        for (int i = 0; i < input_image.width; ++i) // Loop through each pixel in width
        {
            display_progress_bar(i, input_image.width - 1); // Display progress bar
            for (int j = 0; j < input_image.height; ++j) // Loop through each pixel in height
            {
                for (int k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    int dark = input_image(i, j, k) * 0.5; // Calculate darkening factor
                    output_image (i, j, k) = min(input_image(i, j, k) - dark, 255); // Apply darkening effect
                }
            }
        }
    }
}

// Function to crop an image based on specified vertices
void crop(Image& input_image, Image& output_image, const int& vertex_row_no, const int& vertex_col_no)
{
    for (int i = 0; i < output_image.height; i++) // Loop through each pixel in output image height
    {
        display_progress_bar(i, output_image.height - 1); // Display progress bar
        for (int j = 0; j < output_image.width; j++) // Loop through each pixel in output image width
        {
            for (int k = 0; k < 3; k++) // Loop through RGB channels
            {
                output_image(j, i, k) = input_image(j + vertex_col_no, i + vertex_row_no, k); // Crop image based on specified vertices
            }
        }
    }
}

// Function to calculate the radial distance between two points
int radial_distance(int i_1, int j_1, int i_2, int j_2, int height, int width)
{
    return sqrt(pow((i_1-i_2), 2) + pow((j_1-j_2), 2)); // Calculate and return radial distance
}

// Function to check if a point is within a rectangular frame with specified parameters
bool in_rectangular_frame (int i, int j, int center_row, int center_column, int half_of_outer_height, int half_of_outer_width, int thickness, int outer_margin_with_overlap, int image_height, int image_width)
{
    int allowed_area_height = (2*half_of_outer_height - thickness);
    int allowed_area_width = (2*half_of_outer_width - thickness);

    if ( ((half_of_outer_height - thickness <= abs(center_row - i) && abs(center_row - i) <= half_of_outer_height) ||
          (half_of_outer_width - thickness <= abs(center_column - j) && abs(center_column - j) <= half_of_outer_width))
         && ( (i < allowed_area_height) )
         && ( (j < allowed_area_width) )
            )
    {
        return true; // Point is within the rectangular frame
    }
    return false; // Point is outside the rectangular frame
}

// Function to apply a frame to an image, either simple or fancy
void frame(Image& input_image, Image& output_image, const int& fancy, const int& color, const map < int, vector <int> >& color_to_rgb)
{
    int corner_size = min(input_image.height, input_image.width) / 20; // Calculate corner size for the frame

    if (fancy) // Apply fancy frame
    {
        // Fancy Design
        for (int i = 0; i < input_image.height; i++) // Loop through each pixel in height
        {
            display_progress_bar(i, input_image.height - 1); // Display progress bar
            for (int j = 0; j < input_image.width; j++) // Loop through each pixel in width
            {
                // Calculate equivalent coordinates for symmetry
                int equivalent_i = (i < input_image.height/2 ? i : input_image.height-i-1);
                int equivalent_j = (j < input_image.width/2 ? j : input_image.width-j-1);

                // Calculate radial distances for different frame components
                int dist_1 = radial_distance(equivalent_i, equivalent_j, (0.1+0.3)*corner_size, (0.1+2*0.2+0.3+0.025+0.2+0.35+0.3  +  0.1)*corner_size, input_image.height, input_image.width);
                int dist_2 = radial_distance(equivalent_i, equivalent_j, (0.1+2*0.2+0.3+0.025+0.2+0.35+0.3  +  0.1)*corner_size, (0.1+0.3)*corner_size, input_image.height, input_image.width);

                // Apply frame color based on radial distance and rectangular frames
                if ( ((0.3-0.08)*corner_size < dist_1 && dist_1 < 0.3*corner_size) ||
                     ((0.3-0.08)*corner_size < dist_2 && dist_2 < 0.3*corner_size) )
                {
                    for (int k = 0; k < 3; k++)  output_image(j, i, k) = color_to_rgb.at(color)[k];
                }
                else if (in_rectangular_frame(equivalent_i, equivalent_j, input_image.height/2, input_image.width/2, input_image.height/2, input_image.width/2, 0.1*corner_size, 0, input_image.height, input_image.width))
                {
                    for (int k = 0; k < 3; k++)  output_image(j, i, k) = color_to_rgb.at(color)[k];
                }
                    // Add more conditions for other rectangular frames or custom designs if needed
                else
                {
                    for (int k = 0; k < 3; k++)  output_image(j, i, k) = input_image(j, i, k); // Copy original pixel if not in frame
                }
            }
        }
    }
    else
    {
        // Simple Frame
        for (int i = 0; i < input_image.height; i++) // Loop through each pixel in height
        {
            display_progress_bar(i, input_image.height - 1); // Display progress bar
            for (int j = 0; j < input_image.width; j++) // Loop through each pixel in width
            {
                // Check if pixel is outside the simple frame boundary
                if ( (abs(input_image.height/2 - i) > input_image.height/2 - 0.3*corner_size) ||
                     (abs(input_image.width/2 - j) > input_image.width/2 - 0.3*corner_size)   )
                {
                    for (int k = 0; k < 3; k++) // Apply frame color to pixel
                    {
                        output_image(j, i, k) = color_to_rgb.at(color)[k];
                    }
                }
                else // Copy original pixel if within frame boundary
                {
                    for (int k = 0; k < 3; k++)
                    {
                        output_image(j, i, k) = input_image(j, i, k);
                    }
                }
            }
        }
    }
}

// Function to detect edges using Sobel operator
void edges(Image& input_image, Image& output_image)
{
    int sobelX[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};

    int sobelY[3][3] = {{-1, -2, -1},
                        {0, 0, 0},
                        {1, 2, 1}};

    for (int y = 1; y+1 < input_image.height; ++y) { // Loop through each pixel in height
        display_progress_bar(y, input_image.height - 2); // Display progress bar

        for (int x = 1; x+1 < input_image.width; ++x) { // Loop through each pixel in width
            float gradientX = 0.0;
            float gradientY = 0.0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    // Compute gradients using Sobel operator
                    gradientX += input_image(x + i, y + j, 0) * sobelX[j + 1][i + 1];
                    gradientY += input_image(x + i, y + j, 0) * sobelY[j + 1][i + 1];
                }
            }
            float magnitude = sqrt(gradientX * gradientX + gradientY * gradientY); // Calculate magnitude
            magnitude = 255 - magnitude; // Invert magnitude for edge detection

            // Set output pixel values based on magnitude
            output_image(x, y, 0) = magnitude;
            output_image(x, y, 1) = magnitude;
            output_image(x, y, 2) = magnitude;
        }
    }
}


// Function to resize an image using nearest-neighbor interpolation
Image& resize(Image& input_image, Image& output_image)
{
    double round_height = double(input_image.height) / double(output_image.height);
    double round_width = double(input_image.width) / double(output_image.width);

    for (int i = 0; i < output_image.width; i++) {
        display_progress_bar(i, output_image.width - 1);
        for (int j = 0; j < output_image.height; j++) {
            for (int k = 0; k < 3; k++) {
                if (round(round_width * double(i)) < input_image.width && round(round_height * double(j)) < input_image.height) {
                    output_image(i, j, k) = input_image(round(round_width * double(i)), round(round_height * double(j)), k);
                }
            }
        }
    }

    return output_image;
}

// 2D Prefix Blur
void blur(Image& input_image, Image& output_image, const int& blur_radius, const int& call_no)
{

    // Precompute blur box sums
    vector<vector<vector<long long>>> blur_box_sums(input_image.height, vector<vector<long long>>(input_image.width, vector<long long>(3, 0)));
    for (int i = 0; i < input_image.height; ++i) {

        switch (call_no)
        {
            case 1:
            {
                display_progress_bar(0.25 * i, (input_image.height - 1));
                break;
            }
            case 2:
            {
                display_progress_bar(0.25 * i + 0.5 * input_image.height, (input_image.height - 1));
                break;
            }
        }

        for (int j = 0; j < input_image.width; ++j) {
            for (int k = 0; k < 3; ++k) {
                blur_box_sums[i][j][k] = input_image(j, i, k);
                if (i > 0) blur_box_sums[i][j][k] += blur_box_sums[i - 1][j][k];
                if (j > 0) blur_box_sums[i][j][k] += blur_box_sums[i][j - 1][k];
                if (i > 0 && j > 0) blur_box_sums[i][j][k] -= blur_box_sums[i - 1][j - 1][k];
            }
        }
    }

    // Blur the image
    for (int i = 0; i < input_image.height; ++i) {

        switch (call_no)
        {
            case 1:
            {
                display_progress_bar(0.25 * i + 0.25 * input_image.height, (input_image.height - 1));
                break;
            }
            case 2:
            {
                display_progress_bar(float(0.25 * i) + float(0.75 * input_image.height), (input_image.height - 1));
                break;
            }
        }

        for (int j = 0; j < input_image.width; ++j) {
            int x_1 = max(0, j - blur_radius);
            int y_1 = max(0, i - blur_radius);
            int x_2 = min(input_image.width - 1, j + blur_radius);
            int y_2 = min(input_image.height - 1, i + blur_radius);

            int count = (x_2 - x_1 + 1) * (y_2 - y_1 + 1);

            for (int k = 0; k < 3; ++k) {
                long long sum = blur_box_sums[y_2][x_2][k];
                if (x_1 > 0) sum -= blur_box_sums[y_2][x_1 - 1][k];
                if (y_1 > 0) sum -= blur_box_sums[y_1 - 1][x_2][k];
                if (x_1 > 0 && y_1 > 0) sum += blur_box_sums[y_1 - 1][x_1 - 1][k];
                output_image(j, i, k) = round(sum / float(count));
            }
        }
    }
}

void sunlight(Image& input_image, Image& output_image)
{
    // Apply sunlight effect by reducing intensity of blue channel
    for (int i = 0; i < input_image.width; ++i) {
        display_progress_bar(i, input_image.width - 1);
        for (int j = 0; j < input_image.height; ++j) {
            // Keep red and green channels unchanged, reduce blue channel intensity
            output_image(i, j, 0) = input_image(i, j, 0);
            output_image(i, j, 1) = input_image(i, j, 1);
            output_image(i, j, 2) = input_image(i, j, 2) - input_image(i, j, 2) / 4;
        }
    }
}

void oil_painting(Image& input_image, Image& output_image)
{
    // Apply oil painting effect based on intensity levels and neighborhood pixels
    int RADIUS = min(5, (((input_image.height * input_image.width) / 36000) * 2));
    if (RADIUS < 2) RADIUS = 2;

    const int LEVEL_SIZE = 2;

    for (int i = 0; i < input_image.height; i++)
    {
        display_progress_bar(i, input_image.height - 1);
        for (int j = 0; j < input_image.width; j++)
        {
            vector<int> intensity_count(LEVEL_SIZE + 1, 0);
            vector<vector<int>> color_sums(LEVEL_SIZE + 1, vector<int>(3, 0));

            // Calculate intensity levels and accumulate color sums within neighborhood
            for (int m = i - RADIUS; m <= i + RADIUS; m++)
            {
                for (int n = j - RADIUS; n <= j + RADIUS; n++)
                {
                    if (m < 0 || n < 0 || m >= input_image.height || n >= input_image.width) continue;

                    int current_intensity = ((input_image(n, m, 0) + input_image(n, m, 1) + input_image(n, m, 2)) / (255.0 * 3.0)) * LEVEL_SIZE;
                    intensity_count[current_intensity]++;
                    for (int k = 0; k < 3; k++) color_sums[current_intensity][k] += input_image(n, m, k);
                }
            }

            auto max_index = max_element(intensity_count.begin(), intensity_count.end());

            // Assign pixel color based on dominant intensity level in neighborhood
            for (int k = 0; k < 3; k++) output_image(j, i, k) = color_sums[max_index - intensity_count.begin()][k] / *max_index;
        }
    }
}

void old_tv(Image& input_image, Image& output_image)
{
    // Initialize random number generator for noise
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> distribution(0, 25);

    // Calculate total thickness of noise lines based on image size
    int total_thickness = input_image.height / 30;
    if (min(input_image.height, input_image.width) < 500) total_thickness *= 3;
    else if (min(input_image.height, input_image.width) < 1000) total_thickness *= 2;

    for (int i = 0; i < input_image.height; i++)
    {
        display_progress_bar(i, input_image.height - 1);
        for (int j = 0; j < input_image.width; j++)
        {
            double noise = distribution(gen);  // Generate random noise value

            // Apply noise with different intensities based on line thickness
            if ((i % total_thickness) < total_thickness * 0.65)
            {
                for (int k = 0; k < input_image.channels; k++) {
                    double pixel_value = input_image(j, i, k) + 3 * noise + (k == 2 ? 110 : 90);  // Add noise and adjust blue channel
                    output_image(j, i, k) = clamp(pixel_value, 0.0, 255.0);  // Clamp pixel value within valid range
                }
            }
            else
            {
                for (int k = 0; k < input_image.channels; k++)
                {
                    double pixel_value = input_image(j, i, k) + 2 * noise + (k == 2 ? 25 : 20);  // Add noise with different intensity
                    output_image(j, i, k) = clamp(pixel_value, 0.0, 255.0);  // Clamp pixel value within valid range
                }
            }
        }
    }
}
