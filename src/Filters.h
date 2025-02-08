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
    void grayScale(Image *inputImage);
    void Black_White(Image *inputImage);
    void invert(Image *inputImage);
    void merge(Image &inputImage1, Image &inputImage2, Image &outputImage, U8 resize_or_not); // TODO check if u can remove output
    void flip(Image *inputImage, U8 horizontal_or_vertical);
    void rotate(Image *inputImage, U16 rotationAngle);
    void exposure(Image *inputImage, bool lighten);
    void crop(Image *inputImage, size_t vertexRow_Num, size_t vertexCol_Num, size_t width, size_t height);
    void frame(Image *inputImage, int fancy, int color, const std::unordered_map<U8, std::vector<U16>> &color_to_rgb);

    void detectEdges(Image *inputImage);
    Image &resize(Image &inputImage, Image &outputImage);
    void blur(Image &inputImage, Image &outputImage, int blurRadius, int call_Num);
    void sunlight(Image &inputImage, Image &outputImage);
    void oilPainting(Image &inputImage, Image &outputImage);
    void oldTV(Image &inputImage, Image &outputImage);
};

#endif //_FILTERS_H
