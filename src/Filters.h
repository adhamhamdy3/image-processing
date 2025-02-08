#ifndef _FILTERS_H
#define _FILTERS_H

#include "Utilities.h"
#include "Image.h"
#include <unordered_map>

enum class H_V : uint8_t
{
    H = 1, V
};

enum class ANGLE : int
{
    RIGHT = 90, STRAIGHT = 180, OBTUSE = 270
};

namespace Filters
{
    void grayScale(Image *inputImage);
    void Black_White(Image *inputImage);
    void invert(Image *inputImage);
    void merge(Image &inputImage1, Image &inputImage2, Image &outputImage, U8 resize_or_not);

    void flip(Image *inputImage, int horizontal_or_vertical);
    void rotate(Image *inputImage, int rotationAngle);
    void exposure(Image &inputImage, Image &outputImage, bool lighten);
    void crop(Image &inputImage, Image &outputImage, int vertexRow_Num, int vertexCol_Num);
    void frame(Image &inputImage, Image &outputImage, int fancy, int color, const std::unordered_map <int,
               std::vector <int>>& color_to_rgb);

    void edges(Image &inputImage, Image &outputImage);
    Image& resize(Image &inputImage, Image &outputImage);
    void blur(Image &inputImage, Image &outputImage, int blurRadius, int call_Num);
    void sunlight(Image &inputImage, Image &outputImage);
    void oilPainting(Image &inputImage, Image &outputImage);
    void oldTV(Image &inputImage, Image &outputImage);
};


#endif //_FILTERS_H
