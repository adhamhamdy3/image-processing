#include "Utilities.h"

using namespace std;

void Utilities::display_progress_bar(int current_index, int max_index, string optional_title) {
    int percentage = 100 * (current_index + 1) / (max_index + 1); // Calculate percentage
    cout << left << "\r" << optional_title << setfill('#') << setw(percentage / 5) << "" << setfill(' ') <<
    setw(20 - percentage / 5) << "" << "| " << (percentage) << "%"; // Display progress bar
    cout.flush(); // Flush output
}

int Utilities::get_int(const string &prompt, int l, int r) {
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

int Utilities::get_int(const string &prompt, vector<int> value_list) {
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

string Utilities::get_valid_image_filename(const string &prompt, bool existing) {
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
            cerr << "Invalid filename. Please enter a valid filename with one of the following extensions: .jpg, .png, "
                    ".bmp, .tga" << endl; // Display error message
        }
    }

    freopen ("/dev/tty","w",stderr); // Redirect standard error back to console

    return filename; // Return valid filename
}

bool Utilities::in_rectangular_frame(int i, int j, int center_row, int center_column, int half_of_outer_height,
                                     int half_of_outer_width, int thickness, int outer_margin_with_overlap,
                                     int image_height, int image_width) {
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

int Utilities::radial_distance(int i_1, int j_1, int i_2, int j_2, int height, int width) {
    return sqrt(pow((i_1-i_2), 2) + pow((j_1-j_2), 2)); // Calculate and return radial distance
}
