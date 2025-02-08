#ifndef _UTILITIES_H
#define _UTILITIES_H

// v_<function> --> validation

#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <algorithm>
#include <regex>
#include <random>
#include "Image.h"

namespace Utilities
{
    namespace Validations
    {
        template <typename T>
        static constexpr const T &clamp(const T &value, const T &lower, const T &upper)
        {
            return (value < lower) ? lower : (value > upper) ? upper
                                                             : value;
        }
        int v_numericalInput(const std::string &prompt, int l = INT_MIN, int r = INT_MAX);
        int v_numericalInput(const std::string &prompt, std::vector<int> valueList);
        std::string v_ImgName(const std::string &prompt, bool existing = false);
        bool v_inRectFrame(int i, int j, int centerRow, int centerColumn, int halfOfOuter_H,
                           int halfOfOuter_W, int thickness);
    }

    void displayProgressBar(size_t currentIndex, size_t maxIndex, const std::string &optionalTitle = "");
    float radialDistance(size_t i_1, size_t j_1, float i_2, float j_2);

    Image importIMG(const std::string &fileName);
    void exportIMG(Image &outputImg);
}

#endif //_UTILITIES_H
