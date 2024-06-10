#pragma once
#include "target_position.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class TGT_EXPORT ITPOCRPP : public InferenceTargetPosition
{
    private:
    std::string path_det_;
    std::string path_rec_;
    void * ocr_;
public:
    ITPOCRPP();
    ~ITPOCRPP();
    int init(const std::string &_model_path);
    int getPosition(double _timestamp, const cv::Mat &_image, std::vector<TargetPosition> &_targets);
};

class TGT_EXPORT TTPOCRPP : public TrainTargetPosition
{
public:
    int init(const std::string &_model_path)
    {
        return -1;
    }

    int addTarget(const std::string &_tag, const cv::Mat &_image) {
        return -1;
    }

    int train(const std::string &_save_path) {
        return -1;
    }
};
