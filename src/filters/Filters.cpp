#include "Filters.h"

using namespace std;

void Filters::Grayscale(Photo *photo)
{
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    for (size_t i = 0; i < width; i++)  // Loop through each pixel in width
    {
        Utilities::displayProgressBar(i, width - 1); // Display progress bar
        for (size_t j = 0; j < height; j++) // Loop through each pixel in height
        {
            U16 avg = 0;    // Initialize average value for Grayscale calculation
            for (U8 k = 0; k < 3; k++)// Loop through RGB channels
            {
                avg += photo->currentImage->getPixel(i, j, k); // Sum RGB values
            }
            avg /= 3;   // Calculate average RGB value

            for (U8 k = 0; k < 3; k++)  // Assign average value to each RGB channel in output image
                photo->currentImage->setPixel(i, j, k, avg);
        }
    }
    photo->pushChanges();
}

void Filters::Black_White(Photo* photo){
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    for (size_t i = 0; i < width; i++) // Loop through each pixel in width
    {
        Utilities::displayProgressBar(i, width - 1); // Display progress bar

        for (size_t j = 0; j < height; j++) // Loop through each pixel in height
        {
            U16 avg = 0; // Initialize average value for black and white calculation

            for (U8 k = 0; k < 3; k++) // Loop through channels
            {
                avg += photo->currentImage->getPixel(i, j, k); // Sum channel values
            }

            avg /= 3; // Calculate average value

            // Convert to black or white based on average value
            (avg > 127) ? avg = 255 : avg = 0;

            for (U8 k = 0; k < 3; k++) // Assign black or white value to each RGB channel in output image
            {
                photo->currentImage->setPixel(i, j, k, avg);
            }
        }
    }
    photo->pushChanges();
}

void Filters::Invert(Photo *photo)
{
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    for (size_t i = 0; i < height; i++) // Loop through each pixel in height
    {
        Utilities::displayProgressBar(i, height - 1); // Display progress bar

        for (size_t j = 0; j < width; j++) // Loop through each pixel in width
        {
            for (U8 k = 0; k < 3; k++) // Loop through RGB channels
            {
                U8 invert = 255 - photo->currentImage->getPixel(j, i, k); // Invert color values
                photo->currentImage->setPixel(j, i, k, invert);
            }
        }
    }
    photo->pushChanges();
}

void Filters::Merge(Photo *photo1, Photo *photo2, Photo *outputPhoto, U8 resize_or_not)
{
    Image &img1 = *photo1->currentImage;
    Image &img2 = *photo2->currentImage;
    Image &out  = *outputPhoto->currentImage;

    if (!resize_or_not)
    {
        // Merge without resizing (common part)
        for (int i = 0; i < out.width; ++i)
        {
            Utilities::displayProgressBar(i, out.width - 1);
            for (int j = 0; j < out.height; ++j)
            {
                for (U8 k = 0; k < 3; ++k)
                {
                    U8 avg = (img1.getPixel(i, j, k) + img2.getPixel(i, j, k)) / 2;
                    out.setPixel(i, j, k, avg);
                }
            }
        }
    }
    else
    {
        // Resize both to match output dimensions
        Photo temp1(new Image(img1));
        Photo temp2(new Image(img2));

        Filters::Resize(&temp1, out.width, out.height);
        Filters::Resize(&temp2, out.width, out.height);

        for (int i = 0; i < out.width; ++i)
        {
            Utilities::displayProgressBar(i, out.width - 1);
            for (int j = 0; j < out.height; ++j)
            {
                for (U8 k = 0; k < 3; ++k)
                {
                    U8 avg = (temp1.currentImage->getPixel(i, j, k) + temp2.currentImage->getPixel(i, j, k)) / 2;
                    out.setPixel(i, j, k, avg);
                }
            }
        }
    }
    outputPhoto->pushChanges();
}

void Filters::Flip(Photo *photo, U8 horizontal_or_vertical)
{
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    H_V choice = (H_V) (horizontal_or_vertical);
    Image tempImage(*photo->currentImage); // Temporary image for safe modification

    switch (choice)
    {
    case H_V::H: // Horizontal Flip
    {
        for (size_t x = 0; x < width; x++)
        {
            Utilities::displayProgressBar(x, width - 1);
            for (size_t y = 0; y < height; y++)
            {
                for (U8 channel = 0; channel < 3; channel++)
                {
                    const size_t mirrored_x = width - 1 - x;
                    tempImage.setPixel(mirrored_x, y, channel,
                                       photo->currentImage->getPixel(x, y, channel));
                }
            }
        }
        break;
    }
    case H_V::V: // Vertical Flip
    {
        for (size_t x = 0; x < width; x++)
        {
            Utilities::displayProgressBar(x, width - 1);
            for (size_t y = 0; y < height; y++)
            {
                for (U8 channel = 0; channel < 3; channel++)
                {
                    const size_t mirrored_y = height - 1 - y;
                    tempImage.setPixel(x, mirrored_y, channel,
                                       photo->currentImage->getPixel(x, y, channel));
                }
            }
        }
        break;
    }
    }

    *photo->currentImage = tempImage; // Apply final result
    photo->pushChanges();
}

void Filters::Rotate(Photo *photo, U16 rotationAngle)
{
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    ANGLE rAngle = (ANGLE)rotationAngle;

    // Original dimensions
    size_t origWidth = width;
    size_t origHeight = height;
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
                U8 value = photo->currentImage->getPixel(x, y, channel);
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
    delete[] photo->currentImage->imageData; // Assume data is stored as U8* data
    photo->currentImage->width = newWidth;
    photo->currentImage->height = newHeight;
    photo->currentImage->imageData = tempData;
    photo->pushChanges();
}

void Filters::Exposure(Photo *photo, bool lighten)
{
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    for (size_t i = 0; i < width; ++i)
    {
        Utilities::displayProgressBar(i, width - 1);
        for (size_t j = 0; j < height; ++j)
        {
            for (U8 k = 0; k < 3; ++k)
            {
                U8 pixel = photo->currentImage->getPixel(i, j, k);
                U8 delta = (U8) (pixel * 0.5);
                int adjusted = lighten ? (pixel + delta) : (pixel - delta);
                U8 value = (U8)(std::min(adjusted, 255));
                photo->currentImage->setPixel(i, j, k, value);
            }
        }
    }
    photo->pushChanges();
}

void Filters::Crop(Photo *photo, size_t vertexRow_Num, size_t vertexCol_Num, size_t width, size_t height)
{
    Image croppedImage(width, height);
    for (size_t i = vertexRow_Num; i < vertexRow_Num + width; i++)
    {
        Utilities::displayProgressBar(i, vertexRow_Num + width - 1);

        for (size_t j = vertexCol_Num; j < vertexCol_Num + height; j++)
        {
            for (U8 k = 0; k < 3; k++)
                croppedImage(i - vertexRow_Num, j - vertexCol_Num, k) = photo->currentImage->getPixel(i, j, k);
        }
    }
    swap(photo->currentImage->imageData, croppedImage.imageData);
    photo->currentImage->width = width;
    photo->currentImage->height = height;
    photo->pushChanges();
}

void Filters::Frame(Photo *photo, int fancy, int color,
                    const std::unordered_map<U8, std::vector<U16>> &color_to_rgb)
{
    using namespace Utilities;
    using namespace Validations;

    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    size_t cornerSize = min(height, width) / 20; // Calculate corner size for the Frame

    if (fancy) // Apply fancy Frame
    {
        // Fancy Design
        for (size_t i = 0; i < height; i++) // Loop through each pixel in height
        {
            Utilities::displayProgressBar(i, height - 1); // Display progress bar
            for (size_t j = 0; j < width; j++)            // Loop through each pixel in width
            {
                // Calculate equivalent coordinates for symmetry
                size_t equivalent_i = (i < height / 2 ? i : height - i - 1);
                size_t equivalent_j = (j < width / 2 ? j : width - j - 1);

                // Calculate radial distances for different Frame components
                float dist_1 = radialDistance(equivalent_i, equivalent_j, (0.4f) * cornerSize,
                                                         (1.775f) *
                                                             cornerSize);

                float dist_2 = radialDistance(equivalent_i, equivalent_j,
                                                         (1.775f) *
                                                             cornerSize,
                                                         (0.4f) * cornerSize);

                // Apply Frame color based on radial distance and rectangular frames
                if (((0.22f) * cornerSize < dist_1 && dist_1 < 0.3f * cornerSize) ||
                    ((0.22f) * cornerSize < dist_2 && dist_2 < 0.3f * cornerSize))
                {
                    for (U8 k = 0; k < 3; k++)
                        photo->currentImage->setPixel(j, i, k, color_to_rgb.at(color)[k]);
                }
                else if (v_inRectFrame(equivalent_i, equivalent_j,height / 2, width / 2,height / 2, width / 2,0.1 * cornerSize))
                {
                    for (U8 k = 0; k < 3; k++)
                        photo->currentImage->setPixel(j, i, k, color_to_rgb.at(color)[k]);
                }
                // Add more conditions for other rectangular frames or custom designs if needed
                else
                {
                    for (U8 k = 0; k < 3; k++)
                        photo->currentImage->setPixel(j, i, k, photo->currentImage->getPixel(j, i, k)); // Copy original pixel if not in Frame
                }
            }
        }
    }
    else
    {
        // Simple Frame
        for (size_t i = 0; i < height; i++)
        {
            Utilities::displayProgressBar(i, height - 1);
            for (size_t j = 0; j < width; j++)
            {
                // Convert to signed integers to avoid underflow and ambiguity
                int center_i = static_cast<int>(height / 2);
                int center_j = static_cast<int>(width / 2);
                int signed_i = static_cast<int>(i);
                int signed_j = static_cast<int>(j);

                // Calculate absolute differences
                int di = std::abs(signed_i - center_i);
                int dj = std::abs(signed_j - center_j);

                // Threshold as integer to match type
                int threshold = static_cast<int>(0.3f * cornerSize);

                // Check if pixel is outside the Frame boundary
                if (di > (center_i - threshold) || dj > (center_j - threshold))
                {
                    for (U8 k = 0; k < 3; k++)
                    {
                        photo->currentImage->setPixel(j, i, k, color_to_rgb.at(color)[k]);
                    }
                }
            }
        }
    }
    photo->pushChanges();
}

void Filters::DetectEdges(Photo *photo)
{
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    Image outputImage(width, height);

    const int sobelX[3][3] = {{-1, 0, 1},
                              {-2, 0, 2},
                              {-1, 0, 1}};
    const int sobelY[3][3] = {{-1, -2, -1},
                              {0, 0, 0},
                              {1, 2, 1}};
    for (size_t y = 1; y + 1 < height; ++y) // Loop through each pixel in height
    {
        Utilities::displayProgressBar(y, height - 2); // Display progress bar
        for (size_t x = 1; x + 1 < width; ++x) // Loop through each pixel in width
        {
            float gradientX = 0.0;
            float gradientY = 0.0;
            for (int j = -1; j <= 1; ++j)
            {
                for (int i = -1; i <= 1; ++i)
                {
                    // Compute gradients using Sobel operator
                    gradientX += photo->currentImage->getPixel(x + i, y + j, 0) * sobelX[j + 1][i + 1];
                    gradientY += photo->currentImage->getPixel(x + i, y + j, 0) * sobelY[j + 1][i + 1];
                }
            }
            float magnitude = sqrt(gradientX * gradientX + gradientY * gradientY); // Calculate magnitude
            magnitude = 255 - magnitude;                                           // Invert magnitude for edge detection
            // Set output pixel values based on magnitude
            outputImage(x, y, 0) = magnitude;
            outputImage(x, y, 1) = magnitude;
            outputImage(x, y, 2) = magnitude;
        }
    }
    swap(photo->currentImage->imageData, outputImage.imageData);
    photo->pushChanges();
}

Image &Filters::Resize(Photo *photo, size_t targetWidth, size_t targetHeight)
{
    const size_t width = photo->currentImage->width;
    const size_t height = photo->currentImage->height;

    // Create temporary image for result
    Image result(targetWidth, targetHeight);

    // Calculate scaling ratios (original/target for both directions)
    const float widthRatio = static_cast<float>(width) / targetWidth;
    const float heightRatio = static_cast<float>(height) / targetHeight;

    // Process all destination pixels using nearest-neighbor interpolation
    for (size_t dstY = 0; dstY < targetHeight; ++dstY)
    {
        Utilities::displayProgressBar(dstY, targetHeight - 1);
        for (size_t dstX = 0; dstX < targetWidth; ++dstX)
        {
            // Calculate source coordinates with clamping
            const int srcX = std::clamp(
                static_cast<int>(dstX * widthRatio),
                0,
                static_cast<int>(width) - 1);
            const int srcY = std::clamp(
                static_cast<int>(dstY * heightRatio),
                0,
                static_cast<int>(height) - 1);

            // Copy all color channels
            for (U8 channel = 0; channel < 3; ++channel)
            {
                const U8 pixelValue = photo->currentImage->getPixel(srcX, srcY, channel);
                result.setPixel(dstX, dstY, channel, pixelValue);
            }
        }
    }

    // Swap image data and update dimensions
    std::swap(photo->currentImage->imageData, result.imageData);
    photo->currentImage->width = targetWidth;
    photo->currentImage->height = targetHeight;

    photo->pushChanges();
    // Return reference to modified input image
    return *photo->currentImage;
}

void Filters::Blur(Photo *photo, U8 blurLevel)
{
    const int width = photo->currentImage->width;
    const int height = photo->currentImage->height;
    const int radius = std::clamp(static_cast<int>(blurLevel), 1, 15);  // Limit maximum radius

    Image tempImage(width, height);

    // Create integral images for each channel
    std::vector<std::vector<long long>> integrals(3,
                                                  std::vector<long long>(width * height, 0));

    // Build integral images
    for (int c = 0; c < 3; ++c)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                long long current = photo->currentImage->getPixel(x, y, c);
                if(x > 0) current += integrals[c][y*width + (x-1)];
                if(y > 0) current += integrals[c][(y-1)*width + x];
                if(x > 0 && y > 0) current -= integrals[c][(y-1)*width + (x-1)];
                integrals[c][y*width + x] = current;
            }
        }
    }

    // Apply Blur using integral images
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const int x1 = std::max(0, x - radius);
            const int y1 = std::max(0, y - radius);
            const int x2 = std::min(width-1, x + radius);
            const int y2 = std::min(height-1, y + radius);

            const int area = (x2 - x1 + 1) * (y2 - y1 + 1);

            for (int c = 0; c < 3; ++c)
            {
                long long sum = integrals[c][y2*width + x2];
                if(x1 > 0) sum -= integrals[c][y2*width + (x1-1)];
                if(y1 > 0) sum -= integrals[c][(y1-1)*width + x2];
                if(x1 > 0 && y1 > 0) sum += integrals[c][(y1-1)*width + (x1-1)];

                tempImage.setPixel(x, y, c, static_cast<U8>(sum / area));
            }
        }
    }

    std::swap(photo->currentImage->imageData, tempImage.imageData);
    photo->pushChanges();
}

void Filters::Sunlight(Photo *photo)
{
    const size_t width  = static_cast<size_t>(photo->currentImage->width);
    const size_t height = static_cast<size_t>(photo->currentImage->height);

    Image outputImage(width, height);
    // Apply Sunlight effect by reducing intensity of blue channel
    for (size_t i = 0; i < width; ++i)
    {
        Utilities::displayProgressBar(i, width - 1);
        for (size_t j = 0; j < height; ++j)
        {
            // Keep red and green channels unchanged, reduce blue channel intensity
            outputImage(i, j, 0) = photo->currentImage->getPixel(i, j, 0);
            outputImage(i, j, 1) = photo->currentImage->getPixel(i, j, 1);
            outputImage(i, j, 2) = photo->currentImage->getPixel(i, j, 2) - photo->currentImage->getPixel(i, j, 2) / 4;
        }
    }

    std::swap(photo->currentImage->imageData, outputImage.imageData);
    photo->pushChanges();
}

void Filters::OilPainting(Photo *photo)
{
    const int width = photo->currentImage->width;
    const int height = photo->currentImage->height;

    Image outputImage(width, height);

    U8 RADIUS = min(5, (((height * width) / 36000) * 2));
    if (RADIUS < 2)
        RADIUS = 2;

    const int LEVEL_SIZE = 2;

    for (int i = 0; i < height; i++)
    {
        Utilities::displayProgressBar(i, height - 1);
        for (int j = 0; j < width; j++)
        {
            int intensity_count[LEVEL_SIZE + 1] = {0};
            int color_sums[LEVEL_SIZE + 1][3]   = {{0}};

            for (int m = i - RADIUS; m <= i + RADIUS; m++)
            {
                for (int n = j - RADIUS; n <= j + RADIUS; n++)
                {
                    if (m < 0 || n < 0 || m >= height || n >= width)
                        continue;

                    int current_intensity = ((photo->currentImage->getPixel(n, m, 0) +
                                              photo->currentImage->getPixel(n, m, 1) +
                                              photo->currentImage->getPixel(n, m, 2)) / (255.0 * 3.0)) * LEVEL_SIZE;

                    intensity_count[current_intensity]++;
                    for (U8 k = 0; k < 3; k++)
                        color_sums[current_intensity][k] += photo->currentImage->getPixel(n, m, k);
                }
            }

            // find dominant intensity bucket
            int max_val = 0, max_idx = 0;
            for (int b = 0; b <= LEVEL_SIZE; b++)
            {
                if (intensity_count[b] > max_val)
                {
                    max_val = intensity_count[b];
                    max_idx = b;
                }
            }

            // guard against empty neighborhood
            if (max_val == 0)
            {
                for (U8 k = 0; k < 3; k++)
                    outputImage(j, i, k) = photo->currentImage->getPixel(j, i, k);
                continue;
            }

            for (U8 k = 0; k < 3; k++)
                outputImage(j, i, k) = color_sums[max_idx][k] / max_val;
        }
    }

    std::swap(photo->currentImage->imageData, outputImage.imageData);
    photo->pushChanges();
}

void Filters::OldTV(Photo *photo)
{
    const int width = photo->currentImage->width;
    const int height = photo->currentImage->height;

    Image outputImage(width, height);
    // Initialize random number generator for noise
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> distribution(0, 25);

    // Calculate total thickness of noise lines based on image size
    int total_thickness = height / 30;
    if (min(height, width) < 500)
        total_thickness *= 3;
    else if (min(height, width) < 1000)
        total_thickness *= 2;

    for (int i = 0; i < height; i++)
    {
        Utilities::displayProgressBar(i, height - 1);
        for (int j = 0; j < width; j++)
        {
            double noise = distribution(gen); // Generate random noise value

            // Apply noise with different intensities based on line thickness
            if ((i % total_thickness) < total_thickness * 0.65)
            {
                for (U8 k = 0; k < 3; k++)
                {
                    double pixel_value = photo->currentImage->getPixel(j, i, k) + 3 * noise + (k == 2 ? 110 : 90);    // Add noise and adjust blue channel
                    outputImage(j, i, k) = std::clamp(pixel_value, 0.0, 255.0); // Clamp pixel value within valid range
                }
            }
            else
            {
                for (U8 k = 0; k < 3; k++)
                {
                    double pixel_value = photo->currentImage->getPixel(j, i, k) + 2 * noise + (k == 2 ? 25 : 20);     // Add noise with different intensity
                    outputImage(j, i, k) = std::clamp(pixel_value, 0.0, 255.0); // Clamp pixel value within valid range
                }
            }
        }
    }
    std::swap(photo->currentImage->imageData, outputImage.imageData);
    photo->pushChanges();
}
