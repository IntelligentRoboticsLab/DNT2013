#include "Tools/ImageProcessing/ImagePrimitives.h"

#include "ColorDiscretizer.h"
/*
*/
bool ColorDiscretizer::initializeColorModel(cv::InputArrayOfArrays images, int clusters)
{
    if (!this->checkClusters(clusters)) {
        return false;
    }

    int sampleSize = 0, sampleCount = 0;
    const int imageCount = images.size().area();

    for (int i = 0; i < imageCount; i++)
    {
        sampleSize += images.getMat(i).size().area();
    }

    cv::Mat samples(sampleSize, 3, CV_32F);
    cv::Mat image;
    cv::Vec3b* row;

    for (int i = 0; i < imageCount; i++)
    {
        image = images.getMat(i);
        for (int y = 0; y < image.rows; y++)
        {
            row = image.ptr<cv::Vec3b>(y);
            for (int x = 0; x < image.cols; x++)
            {
                samples.at<float>(sampleCount, 0) = row[x][0];
                samples.at<float>(sampleCount, 1) = row[x][1];
                samples.at<float>(sampleCount, 2) = row[x][2];
                sampleCount++;
            }
        }
    }

    this->generateClusterIndex(samples);
    return true;
}

bool ColorDiscretizer::initializeColorModel(std::vector<naoth::Image> images, int clusters)
{
    if (!this->checkClusters(clusters)) {
        return false;
    }

    int sampleSize = 0, sampleCount = 0;

    for (std::vector<naoth::Image>::iterator image = images.begin() ; image != images.end(); ++image)
    {
        sampleSize += image->width() * image->height();
    }

    cv::Mat samples(sampleSize, 3, CV_32F);
    Pixel pixel;

    for (std::vector<naoth::Image>::iterator image = images.begin() ; image != images.end(); ++image)
    {
        for (unsigned int y = 0; y < image->width(); y++)
        {
            for (unsigned int x = 0; x < image->height(); x++)
            {
                image->get(x, y, pixel);
                samples.at<float>(sampleCount, 0) = pixel.channels[0];
                samples.at<float>(sampleCount, 1) = pixel.channels[1];
                samples.at<float>(sampleCount, 2) = pixel.channels[2];
                sampleCount++;
            }
        }
    }

    this->generateClusterIndex(samples);
    return true;
}

bool ColorDiscretizer::initializeColorModel(std::vector<Pixel> pixels, int clusters)
{
    if (!this->checkClusters(clusters)) {
        return false;
    }

    int sampleCount = 0;
    cv::Mat samples(pixels.size(), 3, CV_32F);
    for (unsigned int i = 0; i < pixels.size(); i++)
    {
        samples.at<float>(sampleCount, 0) = pixels[i].channels[0];
        samples.at<float>(sampleCount, 1) = pixels[i].channels[1];
        samples.at<float>(sampleCount, 2) = pixels[i].channels[2];
        sampleCount++;
    }

    this->generateClusterIndex(samples);
    return true;
}

void ColorDiscretizer::discretize(cv::InputArray _image, cv::OutputArray _discretizedImage)
{
    if (!this->isClustersIndexed)
    {
        return;
    }

    cv::Mat image = _image.getMat();
    _discretizedImage.create(image.size(), CV_32SC1);
    cv::Mat discretizedImage = _discretizedImage.getMat();

    cv::MatConstIterator_<cv::Vec3i> src = image.begin<cv::Vec3i>(), src_end = image.end<cv::Vec3i>();
    cv::MatIterator_<int> dst = discretizedImage.begin<int>(), dst_end = discretizedImage.end<int>();

    std::vector<float> query(3), distances;
    std::vector<int> indices(1);
    const cvflann::SearchParams params;

    for (; src != src_end && dst != dst_end; ++src, ++dst)
    {
        query[0] = (*src)[0];
        query[1] = (*src)[1];
        query[2] = (*src)[2];

        this->knnIndex->knnSearch(query, indices, distances, 1, params);
        *dst = indices[0];
    }
}

void ColorDiscretizer::discretize(naoth::Image image, std::vector<std::vector<int> > &discretizedImage)
{
    if (!this->isClustersIndexed)
    {
        return;
    }

    discretizedImage = std::vector<std::vector<int> >(image.height());

    std::vector<int> *row, indices(1);
    std::vector<float> query(3), distances;
    const cvflann::SearchParams params;
    unsigned int x, y, yOffset;

    for (y = 0; y < image.height(); ++y)
    {
        row = &(discretizedImage[y] = std::vector<int>(image.width()));
        for (x = 0; x < image.width(); ++x)
        {
            yOffset = 2 * (y * image.cameraInfo.resolutionWidth + x);

            query[0] = image.yuv422[yOffset];
            query[1] = image.yuv422[yOffset+1-((x & 1)<<1)];
            query[2] = image.yuv422[yOffset+3-((x & 1)<<1)];

            this->knnIndex->knnSearch(query, indices, distances, 1, params);
            (*row)[x] = indices[0];
        }
    }
}

void ColorDiscretizer::discretize(std::vector<Pixel> pixels, std::vector<int> &discretizedPixels)
{
    if (!this->isClustersIndexed)
    {
        return;
    }

    Pixel* pixel;
    discretizedPixels = std::vector<int>(pixels.size());

    std::vector<float> query(3), distances;
    std::vector<int> indices(1);
    const cvflann::SearchParams params;

    for (unsigned int i = 0; i < pixels.size(); ++i)
    {
        pixel = &(pixels[i]);
        query[0] = pixel->channels[0];
        query[1] = pixel->channels[1];
        query[2] = pixel->channels[2];

        this->knnIndex->knnSearch(query, indices, distances, 1, params);
        discretizedPixels[i] = indices[0];
    }
}

void ColorDiscretizer::generateClusterIndex(const cv::Mat &samples,
                                            const cvflann::IndexParams &indexingParams,
                                            const cv::TermCriteria &criteria,
                                            int attempts,
                                            int flags)
{
    cv::kmeans(samples, this->clusters, cv::noArray(), criteria, attempts, flags, this->clusterColors);
}
