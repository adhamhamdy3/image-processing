#ifndef _FILTERS_H
#define _FILTERS_H

#include "Utilities.h"
#include "Image.h"

enum class H_V : uint8_t
{
    H = 1, V
};

enum class ANGLE : int
{
    RIGHT = 90, STRAIGHT = 180, OBTUSE = 270
};

typedef uint8_t U8;

namespace Filters
{
    void grayScale(Image& inputImage, Image& outputImage);
    void Black_White(Image & inputImage, Image & outputImage);
    void invert(Image& inputImage, Image& outputImage);
    void merge(Image& inputImage1, Image& inputImage2, Image& outputImage, U8 resize_or_not);
    void flip(Image& inputImage, Image& outputImage, U8 horizontal_or_vertical);
    void rotate(Image& inputImage, Image& outputImage, int rotationAngle);
    void exposure(Image & inputImage, Image & outputImage, bool lighten);
    void crop(Image& inputImage, Image& outputImage, const int& vertexRow_Num, const int& vertexCol_Num);
    void frame(Image& inputImage, Image& outputImage, const int& fancy, const int& color, const std::map <int,
               std::vector <int>>& color_to_rgb);

    void edges(Image& inputImage, Image& outputImage);
    Image& resize(Image& inputImage, Image& outputImage);
    void blur(Image& inputImage, Image& outputImage, const int& blurRadius, const int& call_Num);
    void sunlight(Image& inputImage, Image& outputImage);
    void oilPainting(Image & inputImage, Image & outputImage);
    void oldTV(Image & inputImage, Image & outputImage);
};


#endif //_FILTERS_H
