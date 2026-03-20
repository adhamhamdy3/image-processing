#ifndef _FILTERS_H
#define _FILTERS_H

#include "Utilities.h"
#include "Photo.h"
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
    void Grayscale(Photo *photo);
    void Black_White(Photo *photo);
    void Invert(Photo *photo);
    void Merge(Photo *photo1, Photo *photo2, Photo *outputPhoto, U8 resize_or_not);
    void Flip(Photo *photo, U8 horizontal_or_vertical);
    void Rotate(Photo *photo, U16 rotationAngle);
    void Exposure(Photo *photo, bool lighten);
    void Crop(Photo *photo, size_t vertexRow_Num, size_t vertexCol_Num, size_t width, size_t height);
    void Frame(Photo *photo, int fancy, int color, const std::unordered_map<U8, std::vector<U16>> &color_to_rgb);
    void DetectEdges(Photo *photo);
    Image &Resize(Photo *photo, size_t w, size_t h);
    void Blur(Photo *photo, U8 blurLevel);
    void Sunlight(Photo *photo);
    void OilPainting(Photo *photo);
    void OldTV(Photo *photo);
};

#endif //_FILTERS_H
