#ifndef __ICNP_SDK_H__
#define __ICNP_SDK_H__

#ifdef WINDOWS
#define ICNP_EXPORT extern "C" __declspec(dllexport)
// for user
// #define ICNP_EXPORT extern "C" __declspec(dllimport)
#else // linux or android
#define ICNP_EXPORT extern "C"
#endif

#include <string>
#include <vector>

struct IconRect
{
    double timestamp;
    double x;
    double y;
    double w;
    double h;
    std::string id;
    std::string reserve;
};

struct ImageData
{
    double timestamp;
    int width;
    int height;
    int channel;
    unsigned char *data;
};

ICNP_EXPORT int init_icon_position(const std::string &pretraining_folder);
ICNP_EXPORT int get_icon_position(const ImageData &image, std::vector<IconRect> &rects);

#endif