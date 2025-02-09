#ifndef _FILTERS_H
#define _FILTERS_H

#include "Utilities.h"
#include "Image.h"
#include <unordered_map>

enum class H_V : U8
{
    H = 1,
    V
};

enum class ANGLE : U16
{
    RIGHT = 90,
    STRAIGHT = 180,
    OBTUSE = 270
};

namespace Filters
{
    void Grayscale(Image *inputImage);
    void Black_White(Image *inputImage);
    void Invert(Image * inputImage);
    void Merge(Image &inputImage1, Image &inputImage2, Image &outputImage, U8 resize_or_not); // TODO check if u can remove output
    void Flip(Image * inputImage, U8 horizontal_or_vertical);
    void Rotate(Image *inputImage, U16 rotationAngle);
    void exposure(Image *inputImage, bool lighten);
    void Crop(Image * inputImage, size_t vertexRow_Num, size_t vertexCol_Num, size_t width, size_t height);
    void Frame(Image *inputImage, int fancy, int color, const std::unordered_map<U8, std::vector<U16>> &color_to_rgb);
    void DetectEdges(Image *inputImage);
    Image &Resize(Image * inputImage, size_t w, size_t h);
    void Blur(Image *inputImage, U8 blurLevel);
    void Sunlight(Image *inputImage);
    void OilPainting(Image *inputImage);
    void OldTV(Image *inputImage);
};

#endif //_FILTERS_H
