#include "target_position_template.hpp"
#include "target_position_sdk.h"
#include <opencv2/opencv.hpp>

void *create_target_position()
{
    ITPTemplate * ITPT =new ITPTemplate();
    return ITPT;
}
int init_target_position(void *handle, const std::string &pretraining_folder)
{
    return ((ITPTemplate *)handle)->init(pretraining_folder);
}

int get_target_position(void *handle, const ImageData &image_data, std::vector<TargetRect> &rects)
{
    cv::Mat img(image_data.height,image_data.width,CV_8UC1,image_data.data);
    std::vector<TargetPosition> targets;
    ((ITPTemplate *)handle)->getPosition(image_data.timestamp,img,targets);
    for (size_t i = 0; i < targets.size(); i++)
    {
        TargetRect rect;
        rect.timestamp = targets[i].timestamp;
        rect.x = targets[i].x;
        rect.y = targets[i].y;
        rect.w = targets[i].w;
        rect.h = targets[i].h;
        rect.id = targets[i].id;
        rect.reserve = targets[i].reserve;
        rects.push_back(rect);
    }
    
    return 0;
}
int release_target_position(void *handle)
{
    delete (ITPTemplate *)handle;
    return 0;
}
TTPTemplate TTPT; 
int init_train_target(const std::string &config_folder)
{
    TTPT.init(config_folder);
    return 0;
}
int add_train_target(const ImageData &image_data, const std::vector<TargetRect> &rects)
{
    cv::Mat img(image_data.height,image_data.width,CV_8UC1,image_data.data);
    for (size_t i = 0; i < rects.size(); i++)
    {
    TTPT.addTarget(rects[i].id,img(cv::Rect(rects[i].x-rects[i].w/2,rects[i].y-rects[i].h/2,rects[i].w,rects[i].h)));
    }
    
    return 0;
}
int save_train_target(const std::string &pretraining_folder)
{
    TTPT.train(pretraining_folder);
    return 0;
}