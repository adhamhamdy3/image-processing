#include "Utilities.h"

using namespace std;

void Utilities::displayProgressBar(int currentIndex, int maxIndex, string optionalTitle) {
    int percentage = 100 * (currentIndex + 1) / (maxIndex + 1); // Calculate percentage
    cout << left << "\r" << optionalTitle << setfill('#') << setw(percentage / 5) << "" << setfill(' ') <<
         setw(20 - percentage / 5) << "" << "| " << (percentage) << "%"; // Display progress bar
    cout.flush(); // Flush output
}

int Utilities::Validations::v_numericalInput(const std::string &prompt, int l, int r) {
    U8 num; // Variable to store the integer
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

int Utilities::Validations::v_numericalInput(const std::string &prompt, std::vector<int> valueList) {
    U8 num; // Variable to store the integer
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

            if (find(valueList.begin(), valueList.end(), num) == valueList.end()) // Check if input is not in the list
            {
                cout << "Please enter a number from (" ;
                for (int i = 0; i < valueList.size() - 1; i++) cout << valueList[i] << ", ";
                cout << valueList[valueList.size() - 1] << ")" << "\n"; // Display list error message
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

string Utilities::Validations::v_ImgName(const string &prompt, bool existing) {
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

bool Utilities::Validations::v_inRectFrame(int i, int j, int centerRow, int centerColumn, int halfOfOuterHeight,
                              int halfOfOuter_W, int thickness) {
    int allowed_area_height = (2 * halfOfOuterHeight - thickness);
    int allowed_area_width = (2 * halfOfOuter_W - thickness);

    if ( ((halfOfOuterHeight - thickness <= abs(centerRow - i) && abs(centerRow - i) <= halfOfOuterHeight) ||
          (halfOfOuter_W - thickness <= abs(centerColumn - j) && abs(centerColumn - j) <= halfOfOuter_W))
         && ( (i < allowed_area_height) )
         && ( (j < allowed_area_width) )
            )
    {
        return true; // Point is within the rectangular frame
    }
    return false; // Point is outside the rectangular frame
}

int Utilities::radialDistance(int i_1, int j_1, int i_2, int j_2) {
    return sqrt(pow((i_1-i_2), 2) + pow((j_1-j_2), 2)); // Calculate and return radial distance
}

Image Utilities::importIMG(const std::string &fileName) {
    Image input_image(fileName); // Create an image object from the fileName
    return input_image; // Return the loaded image
}

void Utilities::exportIMG(Image & outputImg) {
    string output_image_prompt = "Please enter the filename (NOT FILEPATH, including extension) to store the edited image "
                                 "\n(Available formats: .jpg, .png, .bmp, .tga): ";
    string output_filename = Utilities::Validations::v_ImgName(output_image_prompt); // Get a valid output filename
    cout << endl << "Saving.. " << endl;
    outputImg.saveImage(output_filename); // Save the image
}