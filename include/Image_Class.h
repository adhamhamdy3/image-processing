/**
 * @File  : Image_Class.h
 * @brief : This file contains the declaration of the Image class.
 *
 * @authors : Shehab Diab, Youssef Mohamed , Nada Ahmed.
 *                       Dr Mohamed El-Ramely ,
 * @copyright : FCAI Cairo University
 * @date      : 18/3/2024
 */


// stb_image header definitions
#ifndef _IMAGE_CLASS_H
#define _IMAGE_CLASS_H


#define ll long long
#define PNG_TYPE 1
#define BMP_TYPE 2
#define TGA_TYPE 3
#define JPG_TYPE 4
#define UNSUPPORTED_TYPE -1


// stb_image header definitions
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define pixel unsigned int

#include <iostream>
#include <exception>


/**
 * @class Image
 * @brief Represents an image with functionalities for loading, saving, and manipulating pixels.
 */
class Image {
private:
    /**
     * @brief Checks if the given filename has a valid extension.
     *
     * @param filename The filename to check.
     * @return True if the filename has a valid extension, false otherwise.
     */
    bool isValidFilename(const std::string& filename) {
        const char* extension = strrchr(filename.c_str(), '.');
        if (extension == nullptr) {
            std::cerr << "Invalid filename: " << filename << std::endl;
            return false;
        }

        return true;
    }

    /**
     * @brief Determines the image type based on the file extension.
     *
     * @param extension The file extension to determine the type.
     * @return The type of image format.
     */

    short getExtensionType(const char* extension) {
        if (strcmp(extension, ".png") == 0) {
            return PNG_TYPE;
        }
        if (strcmp(extension, ".bmp") == 0) {
            return BMP_TYPE;
        }
        if (strcmp(extension, ".tga") == 0) {
            return TGA_TYPE;
        }
        if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
            return JPG_TYPE;
        }

        std::cerr << "Unsupported image format: " << extension << std::endl;
        return UNSUPPORTED_TYPE;
    }

private:
    std::string filename; ///< Stores the filename of the image.

public:
    int width = 0; ///< Width of the image.
    int height = 0; ///< Height of the image.
    int channels = 3; ///< Number of color channels in the image.
    unsigned char* imageData = nullptr; ///< Pointer to the image data.

    /**
     * @brief Default constructor for the Image class.
     */
    Image() = default;

    /**
     * @brief Constructor that loads an image from the specified filename.
     *
     * @param filename The filename of the image to load.
     */
    Image(std::string filename) : filename((filename)) {
        loadNewImage(this->filename);
    }

    /**
     * @brief Constructor that creates an image with the specified dimensions.
     *
     * @param mWidth The width of the image.
     * @param mHeight The height of the image.
     */
    Image(int mWidth, int mHeight) {
        this->width = mWidth;
        this->height = mHeight;
        this->imageData = (unsigned char*)malloc(mWidth * mHeight * this->channels);
    }


    /**
     * @brief Destructor for the Image class.
     */
    ~Image() {
        if (imageData != nullptr) {
            stbi_image_free(imageData);
        }
    }

    /**
     * @brief Loads a new image from the specified filename.
     *
     * @param filename The filename of the image to load.
     * @return True if the image is loaded successfully, false otherwise.
     * @throws std::invalid_argument If the filename or file format is invalid.
     */
    bool loadNewImage(const std::string& filename) {
        if (!isValidFilename(filename)) {
            std::cerr << "Couldn't Load Image" << '\n';
            throw std::invalid_argument("The file extension does not exist");
        }

        const char* extension = strrchr(filename.c_str(), '.');
        short extensionType = getExtensionType(extension);
        if (extensionType == UNSUPPORTED_TYPE) {
            std::cerr << "Unsupported File Format" << '\n';
            throw std::invalid_argument("File Extension is not supported, Only .JPG, JPEG, .BMP, .PNG, .TGA are supported");
        }
        if (imageData != nullptr){
            stbi_image_free(imageData);
        }

        imageData = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb);

        if (imageData == nullptr) {
            std::cerr << "File Doesn't Exist" << '\n';
            throw std::invalid_argument("Invalid filename, File Does not Exist");
        }

        return true;
    }

    /**
     * @brief Saves the image to the specified output filename.
     *
     * @param outputFilename The filename to save the image.
     * @return True if the image is saved successfully, false otherwise.
     * @throws std::invalid_argument If the output filename or file format is invalid.
     */

    bool saveImage(const std::string& outputFilename) {
        if (!isValidFilename(outputFilename)) {
            std::cerr << "Not Supported Format" << '\n';
            throw std::invalid_argument("The file extension does not exist");
        }

        // Determine image type based on filename extension
        const char* extension = strrchr(outputFilename.c_str(), '.');
        short extensionType = getExtensionType(extension);
        if (extensionType == UNSUPPORTED_TYPE) {
            std::cerr << "File Extension is not supported, Only .JPG, JPEG, .BMP, .PNG, .TGA are supported" << '\n';
            throw std::invalid_argument("File Extension is not supported, Only .JPG, JPEG, .BMP, .PNG, .TGA are supported");
        }

        if (extensionType == PNG_TYPE) {
            stbi_write_png(outputFilename.c_str(), width, height, STBI_rgb, imageData, width * 3);
        }
        else if (extensionType == BMP_TYPE) {
            stbi_write_bmp(outputFilename.c_str(), width, height, STBI_rgb, imageData);
        }
        else if (extensionType == TGA_TYPE) {
            stbi_write_tga(outputFilename.c_str(), width, height, STBI_rgb, imageData);
        }
        else if (extensionType == JPG_TYPE) {
            stbi_write_jpg(outputFilename.c_str(), width, height, STBI_rgb, imageData, 90);
        }

        return true;
    }

    /**
     * @brief Gets the pixel value at the specified position and channel.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param c The color channel index (0 for red, 1 for green, 2 for blue).
     * @return Reference to the pixel value.
     * @throws std::out_of_range If the coordinates or channel index is out of bounds.
     */
    unsigned char& getPixel(int x, int y, int c) {
        if (x > width || x < 0) {
            std::cerr << "Out of width bounds" << '\n';
            throw std::out_of_range("Out of bounds, Cannot exceed width value");
        }
        if (y > height || y < 0) {
            std::cerr << "Out of height bounds" << '\n';
            throw std::out_of_range("Out of bounds, Cannot exceed height value");
        }
        if (c < 0 || c > 2) {
            std::cerr << "Out of channels bounds" << '\n';
            throw std::out_of_range("Out of bounds, You only have 3 channels in RGB");
        }

        return imageData[(y * width + x) * channels + c];
    }

    /**
     * @brief Sets the pixel value at the specified position and channel.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param c The color channel index (0 for red, 1 for green, 2 for blue).
     * @param value The new value to set.
     * @throws std::out_of_range If the coordinates or channel index is out of bounds.
     */
    void setPixel(int x, int y, int c, unsigned char value) {
        if (x > width || x < 0) {
            std::cerr << "Out of width bounds" << '\n';
            throw std::out_of_range("Out of bounds, Cannot exceed width value");
        }
        if (y > height || y < 0) {
            std::cerr << "Out of height bounds" << '\n';
            throw std::out_of_range("Out of bounds, Cannot exceed height value");
        }
        if (c < 0 || c > 2) {
            std::cerr << "Out of channels bounds" << '\n';
            throw std::out_of_range("Out of bounds, You only have 3 channels in RGB");
        }

        imageData[(y * width + x) * channels + c] = value;
    }

    /**
     * @brief Overloaded function call operator to access pixels.
     *
     * @param row The row index of the pixel.
     * @param col The column index of the pixel.
     * @param channel The color channel index (0 for red, 1 for green, 2 for blue).
     * @return Reference to the pixel value.
     */
    unsigned char& operator()(int row, int col, int channel) {
        return getPixel(row, col, channel);
    }
};



#endif // _IMAGE_CLASS_H

