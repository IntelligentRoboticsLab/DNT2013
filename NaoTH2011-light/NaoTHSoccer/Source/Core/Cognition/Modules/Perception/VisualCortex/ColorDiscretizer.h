#ifndef COLORDISCRETIZER_H
#define COLORDISCRETIZER_H

#include <opencv2/core/core.hpp>
#include <opencv2/flann/flann_base.hpp>
#include <opencv2/flann/flann.hpp>

#include "Representations/Infrastructure/Image.h"

class ColorDiscretizer
{
public:
    ColorDiscretizer(int clusters): clusters(clusters) {}
    ~ColorDiscretizer(){}
    /*
    */
    bool initializeColorModel(cv::InputArrayOfArrays images, int clusters=-1);
    bool initializeColorModel(std::vector<naoth::Image> images, int clusters=-1);
    bool initializeColorModel(std::vector<Pixel> pixels, int clusters=-1);
    void discretize(cv::InputArray image, cv::OutputArray discretizedImage);
    void discretize(naoth::Image image, std::vector<std::vector<int> > &discretizedImage);
    void discretize(std::vector<Pixel> pixels, std::vector<int> &discretizedPixels);
    inline int getClusters() const { return this->clusters; }
    inline cv::Mat getClusterColors() const { return this->clusterColors; }
private:
    int clusters;
    cv::Mat clusterColors;
    cv::flann::GenericIndex<cvflann::L1<float> > *knnIndex;
    //cv::flann::Index_<float> *knnIndex;
    bool isClustersIndexed;
    /*
    */
    inline bool checkClusters(int clusters) { return (clusters > 0) && (this->clusters = clusters); }
    void generateClusterIndex(const cv::Mat &samples,
                              const cvflann::IndexParams &indexingParams=cvflann::LinearIndexParams(),
                              const cv::TermCriteria &criteria=cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 5, 1.0),
                              int attempts=3,
                              int flags=cv::KMEANS_PP_CENTERS);
};

#endif // COLORDISCRETIZER_H
