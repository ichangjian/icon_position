#include "target_position_template.hpp"
#include "target_position_sdk.h"
#include <opencv2/opencv.hpp>
#include "cout_log.h"
void *create_target_position()
{
    CLOGINFO << "enter create_target_position";
    ITPTemplate *ITPT = new ITPTemplate();
    CLOGINFO << "leave create_target_position";
    return ITPT;
}
int init_target_position(void *handle, const std::string &pretraining_folder)
{
    CLOGINFO << "enter init_target_position";
    int ret = ((ITPTemplate *)handle)->init(pretraining_folder);
    CLOGINFO << "leave init_target_position";
    return ret;
}

int get_target_position(void *handle, const ImageData &image_data, std::vector<TargetRect> &rects)
{
    CLOGINFO << "enter get_target_position";
    cv::Mat img(image_data.height, image_data.width, CV_8UC1, image_data.data);
    std::vector<TargetPosition> targets;
    ((ITPTemplate *)handle)->getPosition(image_data.timestamp, img, targets);
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
    CLOGINFO << "leave get_target_position";
    return 0;
}
int release_target_position(void *handle)
{
    CLOGINFO << "enter release_target_position";
    delete (ITPTemplate *)handle;
    CLOGINFO << "leave release_target_position";
    return 0;
}
TTPTemplate TTPT;
int init_train_target(const std::string &config_folder)
{
    CLOGINFO << "enter init_train_target";
    TTPT.init(config_folder);
    CLOGINFO << "leave init_train_target";
    return 0;
}
int add_train_target(const ImageData &image_data, const std::vector<TargetRect> &rects)
{
    CLOGINFO << "enter add_train_target";
    cv::Mat img(image_data.height, image_data.width, CV_8UC1, image_data.data);
    for (size_t i = 0; i < rects.size(); i++)
    {
        TTPT.addTarget(rects[i].id, img(cv::Rect(rects[i].x - rects[i].w / 2, rects[i].y - rects[i].h / 2, rects[i].w, rects[i].h)));
    }

    CLOGINFO << "leave add_train_target";
    return 0;
}
int save_train_target(const std::string &pretraining_folder)
{
    CLOGINFO << "enter save_train_target";
    TTPT.train(pretraining_folder);
    CLOGINFO << "leave save_train_target";
    return 0;
}