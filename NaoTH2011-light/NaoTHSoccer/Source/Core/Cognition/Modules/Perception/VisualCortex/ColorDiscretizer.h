#ifndef COLORDISCRETIZER_H
#define COLORDISCRETIZER_H

#include <opencv2/ml/ml.hpp>

#include "Representations/Infrastructure/Image.h"

class ColorDiscretizer
{
public:
    ColorDiscretizer();
    ~ColorDiscretizer(){}
    bool initializeColorModel(cv::InputArrayOfArrays images, int colorClasses);
    bool initializeColorModel(std::vector<naoth::Image> images, int colorClasses);
    void discretize(cv::InputArray image, cv::OutputArray discretizedImage);
    void discretize(std::vector<Pixel> pixels, std::vector<int> &discretizedPixels);
private:
    CvEM* colorModel;
    bool isColorModelTrained;
};

#endif // COLORDISCRETIZER_H
