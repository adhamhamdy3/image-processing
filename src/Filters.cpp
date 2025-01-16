#include "Filters.h"

void Filters::grayscale(Image &input_image, Image &output_image) {
    for (int i = 0; i < input_image.width; i++) // Loop through each pixel in width
    {
        Utilities::display_progress_bar(i, input_image.width - 1); // Display progress bar

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

void Filters::black_and_white(Image &input_image, Image &output_image) {
    for(int i = 0; i < input_image.width; i++) // Loop through each pixel in width
    {
        Utilities::display_progress_bar(i, input_image.width - 1); // Display progress bar

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

void Filters::invert(Image &input_image, Image &output_image) {
    for (int i = 0; i < input_image.height; i++) // Loop through each pixel in height
    {
        Utilities::display_progress_bar(i, input_image.height - 1); // Display progress bar

        for (int j = 0; j < input_image.width; j++) // Loop through each pixel in width
        {
            for (int k = 0; k < 3; k++) // Loop through RGB channels
            {
                output_image(j, i, k) = 255 - input_image(j, i, k); // Invert color values
            }
        }
    }
}

void Filters::merge(Image &input_image_1, Image &input_image_2, Image &output_image, const int &resize_or_not) {
    if (resize_or_not == 0) // If resizing is not required
    {
        for (int i = 0; i < output_image.width; ++i) // Loop through each pixel in width
        {
            Utilities::display_progress_bar(i, output_image.width - 1); // Display progress bar
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
            Utilities::display_progress_bar(i, output_image.width - 1); // Display progress bar
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

void Filters::flip(Image &input_image, Image &output_image, const int &horizontal_or_vertical) {
    switch(horizontal_or_vertical)
    {
        case 1: // Horizontal flip
            for(int i = input_image.width-1; i >= 0; i--) // Loop through each pixel in reversed width
            {
                Utilities::display_progress_bar((input_image.width-1) - i, input_image.width-1); // Display progress bar
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
                Utilities::display_progress_bar(i, input_image.width - 1); // Display progress bar
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

void Filters::rotate(Image &input_image, Image &output_image, const int &rotation_angle) {
    for (int i = 0; i < input_image.height; i++) // Loop through each pixel in height
    {
        Utilities::display_progress_bar(i, input_image.height - 1); // Display progress bar
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

void Filters::lighten_or_darken(Image &input_image, Image &output_image, const int &lighten) {
    if (lighten) // Lighten image
    {
        for (int i = 0; i < input_image.width; ++i) // Loop through each pixel in width
        {
            Utilities::display_progress_bar(i, input_image.width - 1); // Display progress bar
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
            Utilities::display_progress_bar(i, input_image.width - 1); // Display progress bar
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

void Filters::crop(Image &input_image, Image &output_image, const int &vertex_row_no, const int &vertex_col_no) {
    for (int i = 0; i < output_image.height; i++) // Loop through each pixel in output image height
    {
        Utilities::display_progress_bar(i, output_image.height - 1); // Display progress bar
        for (int j = 0; j < output_image.width; j++) // Loop through each pixel in output image width
        {
            for (int k = 0; k < 3; k++) // Loop through RGB channels
            {
                output_image(j, i, k) = input_image(j + vertex_col_no, i + vertex_row_no, k); // Crop image based on specified vertices
            }
        }
    }
}

void Filters::frame(Image &input_image, Image &output_image, const int &fancy, const int &color,
                    const map<int, vector<int>> &color_to_rgb) {
    int corner_size = min(input_image.height, input_image.width) / 20; // Calculate corner size for the frame

    if (fancy) // Apply fancy frame
    {
        // Fancy Design
        for (int i = 0; i < input_image.height; i++) // Loop through each pixel in height
        {
            Utilities::display_progress_bar(i, input_image.height - 1); // Display progress bar
            for (int j = 0; j < input_image.width; j++) // Loop through each pixel in width
            {
                // Calculate equivalent coordinates for symmetry
                int equivalent_i = (i < input_image.height/2 ? i : input_image.height-i-1);
                int equivalent_j = (j < input_image.width/2 ? j : input_image.width-j-1);

                // Calculate radial distances for different frame components
                int dist_1 = Utilities::radial_distance(equivalent_i, equivalent_j, (0.1+0.3)*corner_size,
                                                        (0.1+2*0.2+0.3+0.025+0.2+0.35+0.3  +  0.1)*corner_size,
                                                        input_image.height, input_image.width);

                int dist_2 = Utilities::radial_distance(equivalent_i, equivalent_j, (0.1+2*0.2+0.3+0.025+0.2+0.35+0.3  +  0.1)
                *corner_size, (0.1+0.3)*corner_size, input_image.height, input_image.width);

                // Apply frame color based on radial distance and rectangular frames
                if ( ((0.3-0.08)*corner_size < dist_1 && dist_1 < 0.3*corner_size) ||
                     ((0.3-0.08)*corner_size < dist_2 && dist_2 < 0.3*corner_size) )
                {
                    for (int k = 0; k < 3; k++)  output_image(j, i, k) = color_to_rgb.at(color)[k];
                }
                else if (Utilities::in_rectangular_frame(equivalent_i, equivalent_j,
                                                         input_image.height/2, input_image.width/2,
                                                         input_image.height/2, input_image.width/2,
                                                         0.1*corner_size, 0,
                                                         input_image.height, input_image.width))
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
            Utilities::display_progress_bar(i, input_image.height - 1); // Display progress bar
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

void Filters::edges(Image &input_image, Image &output_image) {
    int sobelX[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};

    int sobelY[3][3] = {{-1, -2, -1},
                        {0, 0, 0},
                        {1, 2, 1}};

    for (int y = 1; y+1 < input_image.height; ++y) { // Loop through each pixel in height
        Utilities::display_progress_bar(y, input_image.height - 2); // Display progress bar

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

Image &Filters::resize(Image &input_image, Image &output_image) {
    double round_height = double(input_image.height) / double(output_image.height);
    double round_width = double(input_image.width) / double(output_image.width);

    for (int i = 0; i < output_image.width; i++) {
        Utilities::display_progress_bar(i, output_image.width - 1);
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

void Filters::blur(Image &input_image, Image &output_image, const int &blur_radius, const int &call_no) {
// Precompute blur box sums
    vector<vector<vector<long long>>> blur_box_sums(input_image.height, vector<vector<long long>>(input_image.width, vector<long long>(3, 0)));
    for (int i = 0; i < input_image.height; ++i) {

        switch (call_no)
        {
            case 1:
            {
                Utilities::display_progress_bar(0.25 * i, (input_image.height - 1));
                break;
            }
            case 2:
            {
                Utilities::display_progress_bar(0.25 * i + 0.5 * input_image.height, (input_image.height - 1));
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
                Utilities::display_progress_bar(0.25 * i + 0.25 * input_image.height, (input_image.height - 1));
                break;
            }
            case 2:
            {
                Utilities::display_progress_bar(float(0.25 * i) + float(0.75 * input_image.height), (input_image.height - 1));
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

void Filters::sunlight(Image &input_image, Image &output_image) {
    // Apply sunlight effect by reducing intensity of blue channel
    for (int i = 0; i < input_image.width; ++i) {
        Utilities::display_progress_bar(i, input_image.width - 1);
        for (int j = 0; j < input_image.height; ++j) {
            // Keep red and green channels unchanged, reduce blue channel intensity
            output_image(i, j, 0) = input_image(i, j, 0);
            output_image(i, j, 1) = input_image(i, j, 1);
            output_image(i, j, 2) = input_image(i, j, 2) - input_image(i, j, 2) / 4;
        }
    }
}

void Filters::oil_painting(Image &input_image, Image &output_image) {
// Apply oil painting effect based on intensity levels and neighborhood pixels
    int RADIUS = min(5, (((input_image.height * input_image.width) / 36000) * 2));
    if (RADIUS < 2) RADIUS = 2;

    const int LEVEL_SIZE = 2;

    for (int i = 0; i < input_image.height; i++)
    {
        Utilities::display_progress_bar(i, input_image.height - 1);
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

void Filters::old_tv(Image &input_image, Image &output_image) {
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
        Utilities::display_progress_bar(i, input_image.height - 1);
        for (int j = 0; j < input_image.width; j++)
        {
            double noise = distribution(gen);  // Generate random noise value

            // Apply noise with different intensities based on line thickness
            if ((i % total_thickness) < total_thickness * 0.65)
            {
                for (int k = 0; k < input_image.channels; k++) {
                    double pixel_value = input_image(j, i, k) + 3 * noise + (k == 2 ? 110 : 90);  // Add noise and adjust blue channel
                    output_image(j, i, k) = Utilities::clamp(pixel_value, 0.0, 255.0);  // Clamp pixel value within valid range
                }
            }
            else
            {
                for (int k = 0; k < input_image.channels; k++)
                {
                    double pixel_value = input_image(j, i, k) + 2 * noise + (k == 2 ? 25 : 20);  // Add noise with different intensity
                    output_image(j, i, k) = Utilities::clamp(pixel_value, 0.0, 255.0);  // Clamp pixel value within valid range
                }
            }
        }
    }
}
