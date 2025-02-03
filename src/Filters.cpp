#include "Filters.h"

using namespace std;

void Filters::grayScale(Image &inputImage, Image &outputImage) {
    for (int i = 0; i < inputImage.width; i++) // Loop through each pixel in width
    {
        Utilities::displayProgressBar(i, inputImage.width - 1); // Display progress bar

        for (int j = 0; j < inputImage.height; j++) // Loop through each pixel in height
        {
            U8 avg = 0; // Initialize average value for grayScale calculation
            for (U8 k = 0; k < 3; k++) // Loop through RGB channels
            {
                avg += inputImage(i, j, k); // Sum RGB values
            }
            avg /= 3; // Calculate average RGB value

            for(U8 k = 0; k < 3; k++) // Assign average value to each RGB channel in output image
            {
                outputImage(i, j, k) = avg;
            }
        }
    }
}

void Filters::Black_White(Image &inputImage, Image &outputImage) {
    for(int i = 0; i < inputImage.width; i++) // Loop through each pixel in width
    {
        Utilities::displayProgressBar(i, inputImage.width - 1); // Display progress bar

        for (int j = 0; j < inputImage.height; j++) // Loop through each pixel in height
        {
            U8 avg = 0; // Initialize average value for black and white calculation

            for (U8 k = 0; k < inputImage.channels; k++) // Loop through channels
            {
                avg += inputImage(i, j, k); // Sum channel values
            }

            avg /= 3; // Calculate average value

            // Convert to black or white based on average value
            (avg > 127) ? avg = 255 : avg = 0;

            for (U8 k = 0; k < 3; k++) // Assign black or white value to each RGB channel in output image
            {
                outputImage(i, j, k) = avg;
            }
        }
    }
}

void Filters::invert(Image &inputImage, Image &outputImage) {
    for (int i = 0; i < inputImage.height; i++) // Loop through each pixel in height
    {
        Utilities::displayProgressBar(i, inputImage.height - 1); // Display progress bar

        for (int j = 0; j < inputImage.width; j++) // Loop through each pixel in width
        {
            for (U8 k = 0; k < 3; k++) // Loop through RGB channels
            {
                outputImage(j, i, k) = 255 - inputImage(j, i, k); // Invert color values
            }
        }
    }
}

void Filters::merge(Image &inputImage1, Image &inputImage2, Image &outputImage, U8 resize_or_not) {
    if (resize_or_not == 0) // If resizing is not required
    {
        for (int i = 0; i < outputImage.width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, outputImage.width - 1); // Display progress bar
            for (int j = 0; j < outputImage.height; ++j) // Loop through each pixel in height
            {
                for (U8 k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    U8 avg = (inputImage1(i, j, k) + inputImage2(i, j, k)) / 2; // Calculate average of corresponding pixels
                    outputImage(i, j, k) = avg; // Assign average value to output image
                }
            }
        }
    }
    else // If resizing is required
    {
        // Resize the first image
        Image image_1_resized(outputImage.width, outputImage.height);
        cout << "--------------------------" << endl;
        cout << "Resizing the first image.. " << endl;
        image_1_resized = resize(inputImage1, image_1_resized);

        // Resize the second image
        Image image_2_resized(outputImage.width, outputImage.height);
        cout << endl << "Resizing the second image.. " << endl;
        image_2_resized = resize(inputImage2, image_2_resized);

        cout << endl << "Merging.. " << endl;
        for (int i = 0; i < outputImage.width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, outputImage.width - 1); // Display progress bar
            for (int j = 0; j < outputImage.height; ++j) // Loop through each pixel in height
            {
                for (U8 k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    U8 avg = (image_1_resized(i, j, k) + image_2_resized(i, j, k)) / 2; // Calculate average of corresponding pixels
                    outputImage(i, j, k) = avg; // Assign average value to output image
                }
            }
        }
    }
}

void Filters::flip(Image &inputImage, Image &outputImage, U8 horizontal_or_vertical) {
    H_V choice = (H_V) horizontal_or_vertical;
    switch(choice)
    {
        case H_V::H: // Horizontal flip
            for(int i = inputImage.width - 1; i >= 0; i--) // Loop through each pixel in reversed width
            {
                Utilities::displayProgressBar((inputImage.width - 1) - i, inputImage.width - 1); // Display progress bar
                for(int j = 0; j < inputImage.height; j++) // Loop through each pixel in height
                {
                    for (int k = 0; k < 3; k++) // Loop through RGB channels
                    {
                        outputImage(inputImage.width - 1 - i, j, k) = inputImage(i, j, k); // Perform horizontal flip
                    }
                }
            }
            break;

        case H_V::V: // Vertical flip
            for(int i = 0; i < inputImage.width; i++) // Loop through each pixel in width
            {
                Utilities::displayProgressBar(i, inputImage.width - 1); // Display progress bar
                for(int j = inputImage.height - 1; j >= 0; j--) // Loop through each pixel in reversed height
                {
                    for(int k = 0; k < 3; k++) // Loop through RGB channels
                    {
                        outputImage(i, inputImage.height - 1 - j, k) = inputImage(i, j, k); // Perform vertical flip
                    }
                }
            }
            break;
    }
}

void Filters::rotate(Image &inputImage, Image &outputImage, int rotationAngle) {
    ANGLE rAngle = (ANGLE) rotationAngle;
    for (int i = 0; i < inputImage.height; i++) // Loop through each pixel in height
    {
        Utilities::displayProgressBar(i, inputImage.height - 1); // Display progress bar
        for (int j = 0; j < inputImage.width; j++) // Loop through each pixel in width
        {
            for (int k = 0; k < 3; k++) // Loop through RGB channels
            {
                switch (rAngle) // Perform rotation based on specified angle
                {
                    case ANGLE::RIGHT:
                        outputImage(outputImage.width - i - 1, j, k) = inputImage(j, i, k); // Rotate 90 degrees clockwise
                        break;
                    case ANGLE::STRAIGHT:
                        outputImage(outputImage.width - j - 1, outputImage.height - i - 1, k) = inputImage(j, i, k); // Rotate 180 degrees clockwise
                        break;
                    case ANGLE::OBTUSE:
                        outputImage(i, outputImage.height - j - 1, k) = inputImage(j, i, k); // Rotate 270 degrees clockwise
                        break;
                }
            }
        }
    }
}

void Filters::exposure(Image &inputImage, Image &outputImage, bool lighten) {
    if (lighten) // Lighten image
    {
        for (int i = 0; i < inputImage.width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, inputImage.width - 1); // Display progress bar
            for (int j = 0; j < inputImage.height; ++j) // Loop through each pixel in height
            {
                for (int k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    int light = inputImage(i, j, k) * 0.5; // Calculate lightening factor
                    outputImage (i, j, k) = min(inputImage(i, j, k) + light, 255); // Apply lightening effect
                }
            }
        }
    }
    else // Darken image
    {
        for (int i = 0; i < inputImage.width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, inputImage.width - 1); // Display progress bar
            for (int j = 0; j < inputImage.height; ++j) // Loop through each pixel in height
            {
                for (int k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    int dark = inputImage(i, j, k) * 0.5; // Calculate darkening factor
                    outputImage (i, j, k) = min(inputImage(i, j, k) - dark, 255); // Apply darkening effect
                }
            }
        }
    }
}

void Filters::crop(Image &inputImage, Image &outputImage, const int &vertexRow_Num, const int &vertexCol_Num) {
    for (int i = 0; i < outputImage.height; i++) // Loop through each pixel in output image height
    {
        Utilities::displayProgressBar(i, outputImage.height - 1); // Display progress bar
        for (int j = 0; j < outputImage.width; j++) // Loop through each pixel in output image width
        {
            for (U8 k = 0; k < 3; k++) // Loop through RGB channels
            {
                outputImage(j, i, k) = inputImage(j + vertexCol_Num, i + vertexRow_Num, k); // Crop image based on specified vertices
            }
        }
    }
}

void Filters::frame(Image &inputImage, Image &outputImage, const int &fancy, const int &color,
                    const map<int, vector<int>> &color_to_rgb) {
    int corner_size = min(inputImage.height, inputImage.width) / 20; // Calculate corner size for the frame

    if (fancy) // Apply fancy frame
    {
        // Fancy Design
        for (int i = 0; i < inputImage.height; i++) // Loop through each pixel in height
        {
            Utilities::displayProgressBar(i, inputImage.height - 1); // Display progress bar
            for (int j = 0; j < inputImage.width; j++) // Loop through each pixel in width
            {
                // Calculate equivalent coordinates for symmetry
                int equivalent_i = (i < inputImage.height / 2 ? i : inputImage.height - i - 1);
                int equivalent_j = (j < inputImage.width / 2 ? j : inputImage.width - j - 1);

                // Calculate radial distances for different frame components
                int dist_1 = Utilities::radialDistance(equivalent_i, equivalent_j, (0.1 + 0.3) * corner_size,
                                                       (0.1 + 2 * 0.2 + 0.3 + 0.025 + 0.2 + 0.35 + 0.3 + 0.1) *
                                                       corner_size);

                int dist_2 = Utilities::radialDistance(equivalent_i, equivalent_j,
                                                       (0.1 + 2 * 0.2 + 0.3 + 0.025 + 0.2 + 0.35 + 0.3 + 0.1) *
                                                       corner_size,
                                                       (0.1 + 0.3) * corner_size);

                // Apply frame color based on radial distance and rectangular frames
                if ( ((0.3-0.08)*corner_size < dist_1 && dist_1 < 0.3*corner_size) ||
                     ((0.3-0.08)*corner_size < dist_2 && dist_2 < 0.3*corner_size) )
                {
                    for (int k = 0; k < 3; k++) outputImage(j, i, k) = color_to_rgb.at(color)[k];
                }
                else if (Utilities::Validations::v_inRectFrame(equivalent_i, equivalent_j,
                                                  inputImage.height / 2, inputImage.width / 2,
                                                  inputImage.height / 2, inputImage.width / 2,
                                                  0.1 * corner_size))
                {
                    for (int k = 0; k < 3; k++) outputImage(j, i, k) = color_to_rgb.at(color)[k];
                }
                    // Add more conditions for other rectangular frames or custom designs if needed
                else
                {
                    for (int k = 0; k < 3; k++) outputImage(j, i, k) = inputImage(j, i, k); // Copy original pixel if not in frame
                }
            }
        }
    }
    else
    {
        // Simple Frame
        for (int i = 0; i < inputImage.height; i++) // Loop through each pixel in height
        {
            Utilities::displayProgressBar(i, inputImage.height - 1); // Display progress bar
            for (int j = 0; j < inputImage.width; j++) // Loop through each pixel in width
            {
                // Check if pixel is outside the simple frame boundary
                if ((abs(inputImage.height / 2 - i) > inputImage.height / 2 - 0.3 * corner_size) ||
                    (abs(inputImage.width / 2 - j) > inputImage.width / 2 - 0.3 * corner_size)   )
                {
                    for (int k = 0; k < 3; k++) // Apply frame color to pixel
                    {
                        outputImage(j, i, k) = color_to_rgb.at(color)[k];
                    }
                }
                else // Copy original pixel if within frame boundary
                {
                    for (int k = 0; k < 3; k++)
                    {
                        outputImage(j, i, k) = inputImage(j, i, k);
                    }
                }
            }
        }
    }

}

void Filters::edges(Image &inputImage, Image &outputImage) {
    int sobelX[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};

    int sobelY[3][3] = {{-1, -2, -1},
                        {0, 0, 0},
                        {1, 2, 1}};

    for (int y = 1; y+1 < inputImage.height; ++y) { // Loop through each pixel in height
        Utilities::displayProgressBar(y, inputImage.height - 2); // Display progress bar

        for (int x = 1; x+1 < inputImage.width; ++x) { // Loop through each pixel in width
            float gradientX = 0.0;
            float gradientY = 0.0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    // Compute gradients using Sobel operator
                    gradientX += inputImage(x + i, y + j, 0) * sobelX[j + 1][i + 1];
                    gradientY += inputImage(x + i, y + j, 0) * sobelY[j + 1][i + 1];
                }
            }
            float magnitude = sqrt(gradientX * gradientX + gradientY * gradientY); // Calculate magnitude
            magnitude = 255 - magnitude; // Invert magnitude for edge detection

            // Set output pixel values based on magnitude
            outputImage(x, y, 0) = magnitude;
            outputImage(x, y, 1) = magnitude;
            outputImage(x, y, 2) = magnitude;
        }
    }
}

Image &Filters::resize(Image &inputImage, Image &outputImage) {
    double round_height = double(inputImage.height) / double(outputImage.height);
    double round_width = double(inputImage.width) / double(outputImage.width);

    for (int i = 0; i < outputImage.width; i++) {
        Utilities::displayProgressBar(i, outputImage.width - 1);
        for (int j = 0; j < outputImage.height; j++) {
            for (U8 k = 0; k < 3; k++) {
                if (round(round_width * double(i)) < inputImage.width && round(round_height * double(j)) < inputImage.height) {
                    outputImage(i, j, k) = inputImage(round(round_width * double(i)),
                                                      round(round_height * double(j)), k);
                }
            }
        }
    }

    return outputImage;
}

void Filters::blur(Image &inputImage, Image &outputImage, const int &blurRadius, const int &call_Num) {
// Precompute blur box sums
    vector<vector<vector<long long>>> blur_box_sums(inputImage.height,
                                                    vector<vector<long long>>(inputImage.width,
                                                                              vector<long long>(3, 0)));
    for (int i = 0; i < inputImage.height; ++i) {

        switch (call_Num)
        {
            case 1:
            {
                Utilities::displayProgressBar(0.25 * i, (inputImage.height - 1));
                break;
            }
            case 2:
            {
                Utilities::displayProgressBar(0.25 * i + 0.5 * inputImage.height,
                                              (inputImage.height - 1));
                break;
            }
        }

        for (int j = 0; j < inputImage.width; ++j) {
            for (U8 k = 0; k < 3; ++k) {
                blur_box_sums[i][j][k] = inputImage(j, i, k);
                if (i > 0) blur_box_sums[i][j][k] += blur_box_sums[i - 1][j][k];
                if (j > 0) blur_box_sums[i][j][k] += blur_box_sums[i][j - 1][k];
                if (i > 0 && j > 0) blur_box_sums[i][j][k] -= blur_box_sums[i - 1][j - 1][k];
            }
        }
    }

    // Blur the image
    for (int i = 0; i < inputImage.height; ++i) {

        switch (call_Num)
        {
            case 1:
            {
                Utilities::displayProgressBar(0.25 * i + 0.25 * inputImage.height,
                                              (inputImage.height - 1));
                break;
            }
            case 2:
            {
                Utilities::displayProgressBar(float(0.25 * i) + float(0.75 * inputImage.height),
                                              (inputImage.height - 1));
                break;
            }
        }

        for (int j = 0; j < inputImage.width; ++j) {
            int x_1 = max(0, j - blurRadius);
            int y_1 = max(0, i - blurRadius);
            int x_2 = min(inputImage.width - 1, j + blurRadius);
            int y_2 = min(inputImage.height - 1, i + blurRadius);

            int count = (x_2 - x_1 + 1) * (y_2 - y_1 + 1);

            for (U8 k = 0; k < 3; ++k) {
                size_t sum = blur_box_sums[y_2][x_2][k];
                if (x_1 > 0) sum -= blur_box_sums[y_2][x_1 - 1][k];
                if (y_1 > 0) sum -= blur_box_sums[y_1 - 1][x_2][k];
                if (x_1 > 0 && y_1 > 0) sum += blur_box_sums[y_1 - 1][x_1 - 1][k];
                outputImage(j, i, k) = round(sum / float(count));
            }
        }
    }
}

void Filters::sunlight(Image &inputImage, Image &outputImage) {
    // Apply sunlight effect by reducing intensity of blue channel
    for (int i = 0; i < inputImage.width; ++i) {
        Utilities::displayProgressBar(i, inputImage.width - 1);
        for (int j = 0; j < inputImage.height; ++j) {
            // Keep red and green channels unchanged, reduce blue channel intensity
            outputImage(i, j, 0) = inputImage(i, j, 0);
            outputImage(i, j, 1) = inputImage(i, j, 1);
            outputImage(i, j, 2) = inputImage(i, j, 2) - inputImage(i, j, 2) / 4;
        }
    }
}

void Filters::oilPainting(Image &inputImage, Image &outputImage) {
// Apply oil painting effect based on intensity levels and neighborhood pixels
    U8 RADIUS = min(5, (((inputImage.height * inputImage.width) / 36000) * 2));
    if (RADIUS < 2) RADIUS = 2;

    const U8 LEVEL_SIZE = 2;

    for (int i = 0; i < inputImage.height; i++)
    {
        Utilities::displayProgressBar(i, inputImage.height - 1);
        for (int j = 0; j < inputImage.width; j++)
        {
            vector<int> intensity_count(LEVEL_SIZE + 1, 0);
            vector<vector<int>> color_sums(LEVEL_SIZE + 1, vector<int>(3, 0));

            // Calculate intensity levels and accumulate color sums within neighborhood
            for (int m = i - RADIUS; m <= i + RADIUS; m++)
            {
                for (int n = j - RADIUS; n <= j + RADIUS; n++)
                {
                    if (m < 0 || n < 0 || m >= inputImage.height || n >= inputImage.width) continue;

                    int current_intensity = ((inputImage(n, m, 0) + inputImage(n, m, 1) +
                                              inputImage(n, m, 2)) / (255.0 * 3.0)) * LEVEL_SIZE;
                    intensity_count[current_intensity]++;
                    for (int k = 0; k < 3; k++) color_sums[current_intensity][k] += inputImage(n, m, k);
                }
            }

            auto max_index = max_element(intensity_count.begin(), intensity_count.end());

            // Assign pixel color based on dominant intensity level in neighborhood
            for (U8 k = 0; k < 3; k++) outputImage(j, i, k) = color_sums[max_index -
                                                                          intensity_count.begin()][k] / *max_index;
        }
    }
}

void Filters::oldTV(Image &inputImage, Image &outputImage) {
// Initialize random number generator for noise
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> distribution(0, 25);

    // Calculate total thickness of noise lines based on image size
    int total_thickness = inputImage.height / 30;
    if (min(inputImage.height, inputImage.width) < 500) total_thickness *= 3;
    else if (min(inputImage.height, inputImage.width) < 1000) total_thickness *= 2;

    for (int i = 0; i < inputImage.height; i++)
    {
        Utilities::displayProgressBar(i, inputImage.height - 1);
        for (int j = 0; j < inputImage.width; j++)
        {
            double noise = distribution(gen);  // Generate random noise value

            // Apply noise with different intensities based on line thickness
            if ((i % total_thickness) < total_thickness * 0.65)
            {
                for (int k = 0; k < inputImage.channels; k++) {
                    double pixel_value = inputImage(j, i, k) + 3 * noise + (k == 2 ? 110 : 90);  // Add noise and adjust blue channel
                    outputImage(j, i, k) = Utilities::Validations::clamp(pixel_value, 0.0, 255.0);  // Clamp pixel value within valid range
                }
            }
            else
            {
                for (int k = 0; k < inputImage.channels; k++)
                {
                    double pixel_value = inputImage(j, i, k) + 2 * noise + (k == 2 ? 25 : 20);  // Add noise with different intensity
                    outputImage(j, i, k) = Utilities::Validations::clamp(pixel_value, 0.0, 255.0);  // Clamp pixel value within valid range
                }
            }
        }
    }
}
