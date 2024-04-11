#include "icon_position_sdk.h"

int main(int argc,char** argv)
{
    init_icon_position("");
    ImageData image;
    std::vector<IconRect> rects;
    get_icon_position(image,rects);
    return 0;
}