/**
 * @File  : Image_Class.h
 * @brief : This file contains the declaration of the Image class.
 *
 * @authors : Shehab Diab, Youssef Mohamed , Nada Ahmed.
 *                       Dr Mohamed El-Ramly ,
 * @copyright : FCAI Cairo University
 * @date      : 18/3/2024
 */

// stb_image header definitions
#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <exception>
#include <cstdint>

typedef uint16_t U16;
typedef uint8_t U8;

/**
 * @class Image
 * @brief Represents an image with functionalities for loading, saving, and manipulating pixels.
 */

class Image
{
private:
    /**
     * @brief Checks if the given filename has a valid extension.
     *
     * @param filename The filename to check.
     * @return True if the filename has a valid extension, false otherwise.
     */
    bool isValidFilename(const std::string &filename);

    /**
     * @brief Determines the image type based on the file extension.
     *
     * @param extension The file extension to determine the type.
     * @return The type of image format.
     */
    short getExtensionType(const char *extension);

private:
    std::string filename; ///< Stores the filename of the image.

public:
    int width = 0;                      ///< Width of the image.
    int height = 0;                     ///< Height of the image.
    int channels = 3;                   ///< Number of color channels in the image.
    unsigned char *imageData = nullptr; ///< Pointer to the image data.

    /**
     * @brief Default constructor for the Image class.
     */
    Image() = default;

    /**
     * @brief Constructor that loads an image from the specified filename.
     *
     * @param filename The filename of the image to load.
     */
    Image(std::string filename);

    /**
     * @brief Constructor that creates an image with the specified dimensions.
     *
     * @param mWidth The width of the image.
     * @param mHeight The height of the image.
     */
    Image(int mWidth, int mHeight);

    /**
     * @brief Constructor that creates an image by copying another image.
     *
     * @param other The Image we want to copy.
     */
    Image(const Image &other);

    /**
     * @brief Overloading the assignment operator.
     *
     * @param image The Image we want to copy.
     *
     * @return *this after copying data.
     */
    Image &operator=(const Image &image);

    /**
     * @brief Destructor for the Image class.
     */
    ~Image();

    /**
     * @brief Loads a new image from the specified filename.
     *
     * @param filename The filename of the image to load.
     * @return True if the image is loaded successfully, false otherwise.
     * @throws std::invalid_argument If the filename or file format is invalid.
     */
    bool loadNewImage(const std::string &filename);

    /**
     * @brief Saves the image to the specified output filename.
     *
     * @param outputFilename The filename to save the image.
     * @return True if the image is saved successfully, false otherwise.
     * @throws std::invalid_argument If the output filename or file format is invalid.
     */
    bool saveImage(const std::string &outputFilename);

    /**
     * @brief Gets the pixel value at the specified position and channel.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param c The color channel index (0 for red, 1 for green, 2 for blue).
     * @return Reference to the pixel value.
     * @throws std::out_of_range If the coordinates or channel index is out of bounds.
     */
    unsigned char &getPixel(int x, int y, int c);

    const unsigned char &getPixel(int x, int y, int c) const;
    /**
     * @brief Sets the pixel value at the specified position and channel.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param c The color channel index (0 for red, 1 for green, 2 for blue).
     * @param value The new value to set.
     * @throws std::out_of_range If the coordinates or channel index is out of bounds.
     */
    void setPixel(int x, int y, int c, unsigned char value);

    /**
     * @brief Overloaded function call operator to access pixels.
     *
     * @param row The row index of the pixel.
     * @param col The column index of the pixel.
     * @param channel The color channel index (0 for red, 1 for green, 2 for blue).
     * @return Reference to the pixel value.
     */
    const unsigned char &operator()(int row, int col, int channel) const;

    unsigned char &operator()(int row, int col, int channel);

    bool operator==(const Image &other) const;
};

#endif // _IMAGE_H
