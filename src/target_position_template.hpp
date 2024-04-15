#pragma once
#include "target_position.hpp"

class ITPTemplate : public InferenceTargetPosition
{
private:
    std::vector<TargetPosition> templates_;
public:
    int init(const std::string &_model_path);
    int getPosition(double _timestamp, const cv::Mat &_image, std::vector<TargetPosition> &_targets);
    ITPTemplate(/* args */);
    ~ITPTemplate();
};

class TTPTemplate : public TrainTargetPosition
{
private:
std::vector<TargetPosition> targets_;
public:
    int init(const std::string &_model_path);
    int addTarget(const std::string &_tag, const cv::Mat &_image);
    int train(const std::string &_save_path);
};
