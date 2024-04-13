#include "target_position_sdk.h"

int main(int argc,char** argv)
{
    init_target_position("");
    ImageData image;
    std::vector<TargetRect> rects;
    get_target_position(image,rects);
    return 0;
}