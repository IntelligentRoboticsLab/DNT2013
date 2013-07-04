#include "ColorDiscretizer.h"
/*
*/
ColorDiscretizer::ColorDiscretizer()
{
}

ColorDiscretizer::~ColorDiscretizer()
{
}

void ColorDiscretizer::setClusters(int num_clusters)
{
    this->clusters = num_clusters;
}

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

    cv::Mat samples(sampleSize, 3, CV_32FC3);
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

bool ColorDiscretizer::initializeColorModel(const std::vector<naoth::Image> &images, int clusters)
{
    if (!this->checkClusters(clusters)) {
        return false;
    }

    int sampleSize = 0, sampleCount = 0;

    for (std::vector<naoth::Image>::const_iterator image = images.begin() ; image != images.end(); ++image)
    {
        sampleSize += image->width() * image->height();
    }

    cv::Mat samples(sampleSize, 3, CV_32F);
    Pixel pixel;

    for (std::vector<naoth::Image>::const_iterator image = images.begin() ; image != images.end(); ++image)
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

bool ColorDiscretizer::initializeColorModel(const std::vector<Pixel> &pixels, int clusters)
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

    for (; src != src_end && dst != dst_end; ++src, ++dst)
    {
        *dst = this->nearestNeighborIndex((*src)[0], (*src)[1], (*src)[2]);
    }
}

void ColorDiscretizer::discretize(const naoth::Image &image, std::vector<std::vector<int> > &discretizedImage)
{
    if (!this->isClustersIndexed)
    {
        return;
    }

    discretizedImage = std::vector<std::vector<int> >(image.height());

    std::vector<int> *row;
    unsigned int x, y, yOffset;

    for (y = 0; y < image.height(); ++y)
    {
        row = &(discretizedImage[y] = std::vector<int>(image.width()));
        for (x = 0; x < image.width(); ++x)
        {
            yOffset = 2 * (y * image.cameraInfo.resolutionWidth + x);
            // Copied yuv422 indices from Representations/Infrastructure/Image.h, l.125--129
            (*row)[x] = this->nearestNeighborIndex(image.yuv422[yOffset],
                                                   image.yuv422[yOffset+1-((x & 1)<<1)],
                    image.yuv422[yOffset+3-((x & 1) <<1)]);
        }
    }
}

void ColorDiscretizer::discretize(const std::vector<Pixel> &pixels, std::vector<int> &discretizedPixels)
{
    if (!this->isClustersIndexed)
    {
        return;
    }

    const Pixel* pixel;
    discretizedPixels = std::vector<int>(pixels.size());

    for (unsigned int i = 0; i < pixels.size(); ++i)
    {
        pixel = &pixels[i];
        discretizedPixels[i] = this->nearestNeighborIndex(pixel->channels[0], pixel->channels[1], pixel->channels[2]);
    }
}

void ColorDiscretizer::generateClusterIndex(const cv::Mat &samples,
                                            const cv::TermCriteria &criteria,
                                            int attempts,
                                            int flags)
{
    cv::Mat labels;
    cv::kmeans(samples, this->clusters, labels, criteria, attempts, flags, this->clusterColors);
}

unsigned int ColorDiscretizer::nearestNeighborIndex(float channel1, float channel2, float channel3)
{
    float minDist = INFINITY, dist;
    unsigned int minIndex = this->clusters, i = 0;
    cv::Vec3f *row;

    for (; i < this->clusters; i++)
    {
        row = this->clusterColors.ptr<cv::Vec3f>(i);
        dist = (channel1 - (*row)[0]) * (channel1 - (*row)[0]);
        dist += (channel2 - (*row)[1]) * (channel2 - (*row)[1]);
        dist += (channel3 - (*row)[2]) * (channel3 - (*row)[2]);

        if (dist < minDist)
        {
            minDist = dist;
            minIndex = i;
        }
    }
    return minIndex;
}

void ColorDiscretizer::saveClusters(const char* fileName)
{
    if (!this->isClustersIndexed) return;
    std::ofstream outBinFile;
    outBinFile.open(fileName, std::ios::out | std::ios::binary);
    for (int i = 0; i < this->getClusters(); i++){
        outBinFile.write(reinterpret_cast<char*> (this->clusterColors.ptr<cv::Vec3f>(i)), sizeof(cv::Vec3f));
    }
    outBinFile.close();
    return;
}

void ColorDiscretizer::readClusters(const char *fileName)
{
    std::ifstream inBinFile;
    inBinFile.open(fileName, std::ios::in | std::ios::binary);
    for (int i = 0; i < this->getClusters(); i++){
        inBinFile.read(reinterpret_cast<char*> (this->clusterColors.ptr<cv::Vec3f>(i)), sizeof(cv::Vec3f));
    }
    inBinFile.close();
    return;
}
