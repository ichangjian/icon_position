#pragma once

#define TGT_EXPORT  __declspec(dllexport)

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
struct TGT_EXPORT TargetPosition
{
    double timestamp; // 识别图的时间戳
    double x; // 识别到的目标物中心点在识别图x轴上的位置
    double y; // 识别到的目标物中心点在识别图y轴上的位置
    double w; // 识别到的目标物在识别图上的宽度
    double h; // 识别到的目标物在识别图上的高度
    cv::Mat data;  // for train  
    std::string id; // 识别到的目标物标识
    std::string reserve; //预留变量
};

 class TGT_EXPORT InferenceTargetPosition
{
public:
    virtual int init(const std::string &_model_path) = 0;
    virtual int getPosition(double _timestamp, const cv::Mat &_image, std::vector<TargetPosition>& _targets) = 0;
};

 class TGT_EXPORT TrainTargetPosition
{
public:
    virtual int init(const std::string &_model_path) = 0;
    virtual int addTarget(const std::string & _tag, const cv::Mat &_image) = 0;
    virtual int train(const std::string & _save_path) = 0;
};
