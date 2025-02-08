#include "Filters.h"

using namespace std;

void Filters::grayScale(Image *inputImage)
{
    for (size_t i = 0; i < inputImage->width; i++) // Loop through each pixel in width
    {
        Utilities::displayProgressBar(i, inputImage->width - 1); // Display progress bar

        for (size_t j = 0; j < inputImage->height; j++) // Loop through each pixel in height
        {
            U16 avg = 0;               // Initialize average value for grayScale calculation
            for (U8 k = 0; k < 3; k++) // Loop through RGB channels
            {
                avg += inputImage->getPixel(i, j, k); // Sum RGB values
            }
            avg /= 3; // Calculate average RGB value

            for (U8 k = 0; k < 3; k++) // Assign average value to each RGB channel in output image
            {
                inputImage->setPixel(i, j, k, avg);
            }
        }
    }
}

void Filters::Black_White(Image *inputImage)
{
    for (size_t i = 0; i < inputImage->width; i++) // Loop through each pixel in width
    {
        Utilities::displayProgressBar(i, inputImage->width - 1); // Display progress bar

        for (size_t j = 0; j < inputImage->height; j++) // Loop through each pixel in height
        {
            U16 avg = 0; // Initialize average value for black and white calculation

            for (U8 k = 0; k < inputImage->channels; k++) // Loop through channels
            {
                avg += inputImage->getPixel(i, j, k); // Sum channel values
            }

            avg /= 3; // Calculate average value

            // Convert to black or white based on average value
            (avg > 127) ? avg = 255 : avg = 0;

            for (U8 k = 0; k < 3; k++) // Assign black or white value to each RGB channel in output image
            {
                inputImage->setPixel(i, j, k, avg);
            }
        }
    }
}

void Filters::invert(Image *inputImage)
{
    for (size_t i = 0; i < inputImage->height; i++) // Loop through each pixel in height
    {
        Utilities::displayProgressBar(i, inputImage->height - 1); // Display progress bar

        for (size_t j = 0; j < inputImage->width; j++) // Loop through each pixel in width
        {
            for (U8 k = 0; k < 3; k++) // Loop through RGB channels
            {
                U8 invert = 0xFF - inputImage->getPixel(j, i, k); // Invert color values
                inputImage->setPixel(j, i, k, invert);
            }
        }
    }
}

void Filters::merge(Image &inputImage1, Image &inputImage2, Image &outputImage, U8 resize_or_not)
{
    if (!resize_or_not) // If resizing is not required
    {
        for (int i = 0; i < outputImage.width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, outputImage.width - 1); // Display progress bar
            for (int j = 0; j < outputImage.height; ++j)             // Loop through each pixel in height
            {
                for (U8 k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    U16 avg = (inputImage1(i, j, k) + inputImage2(i, j, k)) / 2; // Calculate average of corresponding pixels
                    outputImage(i, j, k) = avg;                                  // Assign average value to output image
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
        cout << endl
             << "Resizing the second image.. " << endl;
        image_2_resized = resize(inputImage2, image_2_resized);

        cout << endl
             << "Merging.. " << endl;
        for (int i = 0; i < outputImage.width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, outputImage.width - 1); // Display progress bar
            for (int j = 0; j < outputImage.height; ++j)             // Loop through each pixel in height
            {
                for (U8 k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    U16 avg = (image_1_resized(i, j, k) + image_2_resized(i, j, k)) / 2; // Calculate average of corresponding pixels
                    outputImage(i, j, k) = avg;                                          // Assign average value to output image
                }
            }
        }
    }
}

void Filters::flip(Image *inputImage, U8 horizontal_or_vertical)
{
    H_V choice = static_cast<H_V>(horizontal_or_vertical);
    Image tempImage(*inputImage); // Temporary image for safe modification

    switch (choice)
    {
    case H_V::H: // Horizontal flip
    {
        const int width = static_cast<int>(inputImage->width);
        for (int x = 0; x < width; x++)
        {
            Utilities::displayProgressBar(x, width - 1);
            for (size_t y = 0; y < inputImage->height; y++)
            {
                for (U8 channel = 0; channel < 3; channel++)
                {
                    const int mirrored_x = width - 1 - x;
                    tempImage.setPixel(mirrored_x, y, channel,
                                       inputImage->getPixel(x, y, channel));
                }
            }
        }
        break;
    }
    case H_V::V: // Vertical flip
    {
        const int height = static_cast<int>(inputImage->height);
        for (size_t x = 0; x < inputImage->width; x++)
        {
            Utilities::displayProgressBar(x, inputImage->width - 1);
            for (int y = 0; y < height; y++)
            {
                for (U8 channel = 0; channel < 3; channel++)
                {
                    const int mirrored_y = height - 1 - y;
                    tempImage.setPixel(x, mirrored_y, channel,
                                       inputImage->getPixel(x, y, channel));
                }
            }
        }
        break;
    }
    }

    *inputImage = tempImage; // Apply final result
}

void Filters::rotate(Image *inputImage, U16 rotationAngle)
{
    ANGLE rAngle = (ANGLE)rotationAngle;

    // Original dimensions
    size_t origWidth = inputImage->width;
    size_t origHeight = inputImage->height;
    size_t newWidth, newHeight;

    // Determine new dimensions based on rotation angle
    switch (rAngle)
    {
    case ANGLE::RIGHT:
    case ANGLE::OBTUSE:
        newWidth = origHeight;
        newHeight = origWidth;
        break;
    case ANGLE::STRAIGHT:
        newWidth = origWidth;
        newHeight = origHeight;
        break;
    default:
        return; // Invalid angle
    }

    // Create temporary pixel buffer
    U8 *tempData = new U8[newWidth * newHeight * 3]; // 3 channels (RGB)

    for (size_t y = 0; y < origHeight; y++)
    {
        Utilities::displayProgressBar(y, origHeight - 1);
        for (size_t x = 0; x < origWidth; x++)
        {
            for (U8 channel = 0; channel < 3; channel++)
            {
                U8 value = inputImage->getPixel(x, y, channel);
                int newX, newY;

                // Calculate new coordinates
                switch (rAngle)
                {
                case ANGLE::RIGHT: // 90° clockwise
                    newX = origHeight - 1 - y;
                    newY = x;
                    break;
                case ANGLE::STRAIGHT: // 180°
                    newX = origWidth - 1 - x;
                    newY = origHeight - 1 - y;
                    break;
                case ANGLE::OBTUSE: // 270° clockwise
                    newX = y;
                    newY = origWidth - 1 - x;
                    break;
                default:
                    newX = x;
                    newY = y;
                }

                // Calculate index in temporary buffer
                int tempIndex = (newY * newWidth + newX) * 3 + channel;
                tempData[tempIndex] = value;
            }
        }
    }

    // Update inputImage with rotated data
    delete[] inputImage->imageData; // Assume data is stored as U8* data
    inputImage->width = newWidth;
    inputImage->height = newHeight;
    inputImage->imageData = tempData;
}

void Filters::exposure(Image *inputImage, bool lighten)
{
    if (lighten) // Lighten image
    {
        for (size_t i = 0; i < inputImage->width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, inputImage->width - 1); // Display progress bar
            for (size_t j = 0; j < inputImage->height; ++j)          // Loop through each pixel in height
            {
                for (U8 k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    U8 light = inputImage->getPixel(i, j, k) * 0.5;                                 // Calculate lightening factor
                    inputImage->setPixel(i, j, k, min(inputImage->getPixel(i, j, k) + light, 255)); // Apply lightening effect
                }
            }
        }
    }
    else // Darken image
    {
        for (size_t i = 0; i < inputImage->width; ++i) // Loop through each pixel in width
        {
            Utilities::displayProgressBar(i, inputImage->width - 1); // Display progress bar
            for (size_t j = 0; j < inputImage->height; ++j)          // Loop through each pixel in height
            {
                for (U8 k = 0; k < 3; ++k) // Loop through RGB channels
                {
                    U8 dark = inputImage->getPixel(i, j, k) * 0.5;                                 // Calculate darkening factor
                    inputImage->setPixel(i, j, k, min(inputImage->getPixel(i, j, k) - dark, 255)); // Apply darkening effect
                }
            }
        }
    }
}

void Filters::crop(Image *inputImage, size_t vertexRow_Num, size_t vertexCol_Num, size_t width, size_t height)
{
    Image croppedImage(width, height);
    for (size_t i = vertexRow_Num; i < vertexRow_Num + width; i++)
    {
        Utilities::displayProgressBar(i, vertexRow_Num + width - 1);

        for (size_t j = vertexCol_Num; j < vertexCol_Num + height; j++)
        {
            for (U8 k = 0; k < inputImage->channels; k++)
            {
                croppedImage(i - vertexRow_Num, j - vertexCol_Num, k) = inputImage->getPixel(i, j, k);
            }
        }
    }
    swap(inputImage->imageData, croppedImage.imageData);
    inputImage->width = width;
    inputImage->height = height;
}

void Filters::frame(Image *inputImage, int fancy, int color,
                    const unordered_map<U8, vector<U16>> &color_to_rgb)
{
    size_t cornerSize = min(inputImage->height, inputImage->width) / 20; // Calculate corner size for the frame

    if (fancy) // Apply fancy frame
    {
        // Fancy Design
        for (size_t i = 0; i < inputImage->height; i++) // Loop through each pixel in height
        {
            Utilities::displayProgressBar(i, inputImage->height - 1); // Display progress bar
            for (size_t j = 0; j < inputImage->width; j++)            // Loop through each pixel in width
            {
                // Calculate equivalent coordinates for symmetry
                size_t equivalent_i = (i < inputImage->height / 2 ? i : inputImage->height - i - 1);
                size_t equivalent_j = (j < inputImage->width / 2 ? j : inputImage->width - j - 1);

                // Calculate radial distances for different frame components
                float dist_1 = Utilities::radialDistance(equivalent_i, equivalent_j, (0.4f) * cornerSize,
                                                         (1.775f) *
                                                             cornerSize);

                float dist_2 = Utilities::radialDistance(equivalent_i, equivalent_j,
                                                         (1.775f) *
                                                             cornerSize,
                                                         (0.4f) * cornerSize);

                // Apply frame color based on radial distance and rectangular frames
                if (((0.22f) * cornerSize < dist_1 && dist_1 < 0.3f * cornerSize) ||
                    ((0.22f) * cornerSize < dist_2 && dist_2 < 0.3f * cornerSize))
                {
                    for (U8 k = 0; k < 3; k++)
                        inputImage->setPixel(j, i, k, color_to_rgb.at(color)[k]);
                }
                else if (Utilities::Validations::v_inRectFrame(equivalent_i, equivalent_j,
                                                               inputImage->height / 2, inputImage->width / 2,
                                                               inputImage->height / 2, inputImage->width / 2,
                                                               0.1 * cornerSize))
                {
                    for (U8 k = 0; k < 3; k++)
                        inputImage->setPixel(j, i, k, color_to_rgb.at(color)[k]);
                }
                // Add more conditions for other rectangular frames or custom designs if needed
                else
                {
                    for (U8 k = 0; k < 3; k++)
                        inputImage->setPixel(j, i, k, inputImage->getPixel(j, i, k)); // Copy original pixel if not in frame
                }
            }
        }
    }
    else
    {
        // Simple Frame
        for (size_t i = 0; i < inputImage->height; i++)
        {
            Utilities::displayProgressBar(i, inputImage->height - 1);
            for (size_t j = 0; j < inputImage->width; j++)
            {
                // Convert to signed integers to avoid underflow and ambiguity
                int center_i = static_cast<int>(inputImage->height / 2);
                int center_j = static_cast<int>(inputImage->width / 2);
                int signed_i = static_cast<int>(i);
                int signed_j = static_cast<int>(j);

                // Calculate absolute differences
                int di = std::abs(signed_i - center_i);
                int dj = std::abs(signed_j - center_j);

                // Threshold as integer to match type
                int threshold = static_cast<int>(0.3f * cornerSize);

                // Check if pixel is outside the frame boundary
                if (di > (center_i - threshold) || dj > (center_j - threshold))
                {
                    for (U8 k = 0; k < 3; k++)
                    {
                        inputImage->setPixel(j, i, k, color_to_rgb.at(color)[k]);
                    }
                }
            }
        }
    }
}

void Filters::detectEdges(Image *inputImage)
{
    const int sobelX[3][3] = {{-1, 0, 1},
                              {-2, 0, 2},
                              {-1, 0, 1}};

    const int sobelY[3][3] = {{-1, -2, -1},
                              {0, 0, 0},
                              {1, 2, 1}};

    for (int y = 1; y + 1 < inputImage->height; ++y)
    {                                                             // Loop through each pixel in height
        Utilities::displayProgressBar(y, inputImage->height - 2); // Display progress bar

        for (int x = 1; x + 1 < inputImage->width; ++x)
        { // Loop through each pixel in width
            float gradientX = 0.0;
            float gradientY = 0.0;

            for (int j = -1; j <= 1; ++j)
            {
                for (int i = -1; i <= 1; ++i)
                {
                    // Compute gradients using Sobel operator
                    gradientX += inputImage->getPixel(x + i, y + j, 0) * sobelX[j + 1][i + 1];
                    gradientY += inputImage->getPixel(x + i, y + j, 0) * sobelY[j + 1][i + 1];
                }
            }
            float magnitude = sqrt(gradientX * gradientX + gradientY * gradientY); // Calculate magnitude
            magnitude = 255 - magnitude;                                           // Invert magnitude for edge detection

            // Set output pixel values based on magnitude
            inputImage->setPixel(x, y, 0, magnitude);
            inputImage->setPixel(x, y, 1, magnitude);
            inputImage->setPixel(x, y, 2, magnitude);
        }
    }
}

Image &Filters::resize(Image &inputImage, Image &outputImage)
{
    double round_height = double(inputImage.height) / double(outputImage.height);
    double round_width = double(inputImage.width) / double(outputImage.width);

    for (int i = 0; i < outputImage.width; i++)
    {
        Utilities::displayProgressBar(i, outputImage.width - 1);
        for (int j = 0; j < outputImage.height; j++)
        {
            for (U8 k = 0; k < 3; k++)
            {
                if (round(round_width * double(i)) < inputImage.width && round(round_height * double(j)) < inputImage.height)
                {
                    outputImage(i, j, k) = inputImage(round(round_width * double(i)),
                                                      round(round_height * double(j)), k);
                }
            }
        }
    }

    return outputImage;
}

void Filters::blur(Image &inputImage, Image &outputImage, int blurRadius, int call_Num)
{
    // Precompute blur box sums
    vector<vector<vector<long long>>> blur_box_sums(inputImage.height,
                                                    vector<vector<long long>>(inputImage.width,
                                                                              vector<long long>(3, 0)));
    for (int i = 0; i < inputImage.height; ++i)
    {

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

        for (int j = 0; j < inputImage.width; ++j)
        {
            for (U8 k = 0; k < 3; ++k)
            {
                blur_box_sums[i][j][k] = inputImage(j, i, k);
                if (i > 0)
                    blur_box_sums[i][j][k] += blur_box_sums[i - 1][j][k];
                if (j > 0)
                    blur_box_sums[i][j][k] += blur_box_sums[i][j - 1][k];
                if (i > 0 && j > 0)
                    blur_box_sums[i][j][k] -= blur_box_sums[i - 1][j - 1][k];
            }
        }
    }

    // Blur the image
    for (int i = 0; i < inputImage.height; ++i)
    {

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

        for (int j = 0; j < inputImage.width; ++j)
        {
            int x_1 = max(0, j - blurRadius);
            int y_1 = max(0, i - blurRadius);
            int x_2 = min(inputImage.width - 1, j + blurRadius);
            int y_2 = min(inputImage.height - 1, i + blurRadius);

            int count = (x_2 - x_1 + 1) * (y_2 - y_1 + 1);

            for (U8 k = 0; k < 3; ++k)
            {
                size_t sum = blur_box_sums[y_2][x_2][k];
                if (x_1 > 0)
                    sum -= blur_box_sums[y_2][x_1 - 1][k];
                if (y_1 > 0)
                    sum -= blur_box_sums[y_1 - 1][x_2][k];
                if (x_1 > 0 && y_1 > 0)
                    sum += blur_box_sums[y_1 - 1][x_1 - 1][k];
                outputImage(j, i, k) = round(sum / float(count));
            }
        }
    }
}

void Filters::sunlight(Image &inputImage, Image &outputImage)
{
    // Apply sunlight effect by reducing intensity of blue channel
    for (int i = 0; i < inputImage.width; ++i)
    {
        Utilities::displayProgressBar(i, inputImage.width - 1);
        for (int j = 0; j < inputImage.height; ++j)
        {
            // Keep red and green channels unchanged, reduce blue channel intensity
            outputImage(i, j, 0) = inputImage(i, j, 0);
            outputImage(i, j, 1) = inputImage(i, j, 1);
            outputImage(i, j, 2) = inputImage(i, j, 2) - inputImage(i, j, 2) / 4;
        }
    }
}

void Filters::oilPainting(Image &inputImage, Image &outputImage)
{
    // Apply oil painting effect based on intensity levels and neighborhood pixels
    U8 RADIUS = min(5, (((inputImage.height * inputImage.width) / 36000) * 2));
    if (RADIUS < 2)
        RADIUS = 2;

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
                    if (m < 0 || n < 0 || m >= inputImage.height || n >= inputImage.width)
                        continue;

                    int current_intensity = ((inputImage(n, m, 0) + inputImage(n, m, 1) +
                                              inputImage(n, m, 2)) /
                                             (255.0 * 3.0)) *
                                            LEVEL_SIZE;
                    intensity_count[current_intensity]++;
                    for (U8 k = 0; k < 3; k++)
                        color_sums[current_intensity][k] += inputImage(n, m, k);
                }
            }

            auto max_index = max_element(intensity_count.begin(), intensity_count.end());

            // Assign pixel color based on dominant intensity level in neighborhood
            for (U8 k = 0; k < 3; k++)
                outputImage(j, i, k) = color_sums[max_index -
                                                  intensity_count.begin()][k] /
                                       *max_index;
        }
    }
}

void Filters::oldTV(Image &inputImage, Image &outputImage)
{
    // Initialize random number generator for noise
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> distribution(0, 25);

    // Calculate total thickness of noise lines based on image size
    int total_thickness = inputImage.height / 30;
    if (min(inputImage.height, inputImage.width) < 500)
        total_thickness *= 3;
    else if (min(inputImage.height, inputImage.width) < 1000)
        total_thickness *= 2;

    for (int i = 0; i < inputImage.height; i++)
    {
        Utilities::displayProgressBar(i, inputImage.height - 1);
        for (int j = 0; j < inputImage.width; j++)
        {
            double noise = distribution(gen); // Generate random noise value

            // Apply noise with different intensities based on line thickness
            if ((i % total_thickness) < total_thickness * 0.65)
            {
                for (U8 k = 0; k < inputImage.channels; k++)
                {
                    double pixel_value = inputImage(j, i, k) + 3 * noise + (k == 2 ? 110 : 90);    // Add noise and adjust blue channel
                    outputImage(j, i, k) = Utilities::Validations::clamp(pixel_value, 0.0, 255.0); // Clamp pixel value within valid range
                }
            }
            else
            {
                for (U8 k = 0; k < inputImage.channels; k++)
                {
                    double pixel_value = inputImage(j, i, k) + 2 * noise + (k == 2 ? 25 : 20);     // Add noise with different intensity
                    outputImage(j, i, k) = Utilities::Validations::clamp(pixel_value, 0.0, 255.0); // Clamp pixel value within valid range
                }
            }
        }
    }
}
