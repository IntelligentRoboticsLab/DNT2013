#include "Tools/ImageProcessing/ImagePrimitives.h"

#include "ColorDiscretizer.h"


ColorDiscretizer::ColorDiscretizer()
{
}

bool ColorDiscretizer::initializeColorModel(cv::InputArrayOfArrays images, int colorClasses)
{
    int sampleSize = 0, sampleCount = 0;
    const int imageCount = images.size().area();

    for (int i = 0; i < imageCount; i++)
    {
        sampleSize += images.getMat(i).size().area();
    }

    cv::Mat samples(sampleSize, 3, images.getMat(0).type());
    cv::Mat image;
    cv::Vec3f* row;

    for (int i = 0; i < imageCount; i++)
    {
        image = images.getMat(i);
        for (int y = 0; y < image.rows; y++)
        {
            row = image.ptr<cv::Vec3f>(y);
            for (int x = 0; x < image.cols; x++)
            {
                samples.at<cv::Vec3f>(sampleCount++, 0) = row[x];
            }
        }
    }
    CvEMParams emParams(colorClasses);
    this->colorModel = new CvEM();
    return this->colorModel->train(samples, cv::Mat(), emParams);
}

bool ColorDiscretizer::initializeColorModel(std::vector<naoth::Image> images, int colorClasses)
{
    int sampleSize = 0, sampleCount = 0;

    for (std::vector<naoth::Image>::iterator image = images.begin() ; image != images.end(); ++image)
    {
        sampleSize += image->width() * image->height();
    }

    cv::Mat samples(sampleSize, 3, CV_32SC1);
    Pixel pixel;
    cv::Vec3b _pixel;

    for (std::vector<naoth::Image>::iterator image = images.begin() ; image != images.end(); ++image)
    {
        for (unsigned int y = 0; y < image->width(); y++)
        {
            for (unsigned int x = 0; x < image->height(); x++)
            {
                image->get(x, y, pixel);
                _pixel = cv::Vec3b();
                _pixel[0] = pixel.channels[0];
                _pixel[1] = pixel.channels[1];
                _pixel[2] = pixel.channels[2];
                samples.at<cv::Vec3b>(sampleCount++, 0) = _pixel;
            }
        }
    }
    CvEMParams emParams(colorClasses);
    this->colorModel = new CvEM();
    this->isColorModelTrained = this->colorModel->train(samples, cv::Mat(), emParams);
    return this->isColorModelTrained;
}

void ColorDiscretizer::discretize(cv::InputArray _image, cv::OutputArray _discretizedImage)
{
    if (this->colorModel == NULL || !this->isColorModelTrained)
    {
        return;
    }

    cv::Mat image = _image.getMat();
    _discretizedImage.create(image.size(), CV_32SC1);
    cv::Mat discretizedImage = _discretizedImage.getMat();

    cv::Mat row;

    for (int y = 0; y < image.rows; y++)
    {
        row = image.at<cv::Mat>(y);
        for (int x = 0; x < image.cols; x++)
        {
            discretizedImage.at<int>(x, y) = (int) this->colorModel->predict(row.at<cv::Mat>(x));
        }
    }
}

void ColorDiscretizer::discretize(std::vector<Pixel> pixels, std::vector<int> &discretizedPixels)
{
    if (this->colorModel == NULL || !this->isColorModelTrained)
    {
        return;
    }

    discretizedPixels = std::vector<int>(pixels.size());
    cv::Vec3f tmp;
    Pixel pixel;

    for (unsigned int i = 0; i < pixels.size(); ++i)
    {
        pixel = pixels[i];
        tmp = cv::Vec3f(3);
        tmp[0] = pixel.channels[0];
        tmp[1] = pixel.channels[1];
        tmp[2] = pixel.channels[2];

        discretizedPixels[i] = (int) this->colorModel->predict(cv::Mat(tmp));
    }
}
