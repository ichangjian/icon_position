#ifndef __TGT_SDK_H__
#define __TGT_SDK_H__

#ifdef WINDOWS
#define TGT_EXPORT extern "C" __declspec(dllexport)
// for user
// #define TGT_EXPORT extern "C" __declspec(dllimport)
#else // linux or android
#define TGT_EXPORT extern "C"
#endif

#include <string>
#include <vector>

/// @brief 图像数据，用于传递识别图
struct ImageData
{
    double timestamp; // 图像时间戳，单位为秒
    int width; // 图像的宽
    int height; // 图像的高
    int channel; // 图像的通道数
    unsigned char *data; // // 图像的数据所在内存的首地址
};

/// @brief 目标物所在的位置
struct TargetRect
{
    double timestamp; // 识别图的时间戳
    double x; // 识别到的目标物中心点在识别图x轴上的位置
    double y; // 识别到的目标物中心点在识别图y轴上的位置
    double w; // 识别到的目标物在识别图上的宽度
    double h; // 识别到的目标物在识别图上的高度
    std::string id; // 识别到的目标物标识
    std::string reserve; //预留变量
};

/// @brief 创建target position 对象
/// @return  运行成功返回target position 对象的指针，出错返回 NULL
TGT_EXPORT void * create_target_position();

/// @brief 初始化程序
/// @param handle 是 target position 对象的指针
/// @param pretraining_folder 配置文件和模型文件所在路径
/// @return 运行成功返回 0，出错返回 -1
TGT_EXPORT int init_target_position(void* handle,const std::string &pretraining_folder);

/// @brief 获取目标物在识别图上的位置
/// @param handle 是 target position 对象的指针
/// @param image_data 识别图的图像数据 @ImageData
/// @param rects 识别到所有目标物在识别图上的位置 @TargetRect
/// @return 运行成功返回 0，出错返回 -1
TGT_EXPORT int get_target_position(void* handle,const ImageData &image_data, std::vector<TargetRect> &rects);

/// @brief 释放对象资源
/// @param handle 是 target position 对象的指针
/// @return 运行成功返回 0，出错返回 -1
TGT_EXPORT int release_target_position(void* handle);


TGT_EXPORT int init_train_target(const std::string &config_folder);
TGT_EXPORT int add_train_target(const ImageData &image_data, const std::vector<TargetRect> &rects);
TGT_EXPORT int save_train_target(const std::string &pretraining_folder);

#endif