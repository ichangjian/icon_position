#include "target_position_sdk.h"

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int g_nCount = 2;
Rect box;         // 矩形对象
bool drawing_box; // 记录是否在画矩形对象

void onmouse(int event, int x, int y, int flag, void *img) // 鼠标事件回调函数，鼠标点击后执行的内容应在此
{
    cv::Mat &image = *(cv::Mat *)img;
    switch (event)
    {
    case EVENT_LBUTTONDOWN:     // 鼠标左键按下事件
        drawing_box = true;     // 标志在画框
        box = Rect(x, y, 0, 0); // 记录矩形的开始的点
        break;
    case EVENT_MOUSEMOVE: // 鼠标移动事件
        if (drawing_box)
        { // 如果左键一直按着，则表明在画矩形
            box.width = x - box.x;
            box.height = y - box.y; // 更新长宽
        }
        break;
    case EVENT_LBUTTONUP: // 鼠标左键松开事件
    {
        drawing_box = false; // 不在画矩形
                             // 这里好像没作用
        if (box.width < 0)
        {                               // 排除宽为负的情况，在这里判断是为了优化计算，不用再移动时每次更新都要计算长宽的绝对值
            box.x = box.x + box.width;  // 更新原点位置，使之始终符合左上角为原点
            box.width = -1 * box.width; // 宽度取正
        }
        if (box.height < 0)
        { // 同上
            box.y = box.y + box.height;
            box.height = -1 * box.width;
        }
        g_nCount++;
        // cv::Mat dst = image(box);
        // std::string str_save_name = std::to_string(g_nCount) + ".jpg";
        // cv::imwrite(str_save_name.c_str(), dst);
    }

    break;
    default:
        break;
    }
}
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "input error\n";
        std::cout << "Usage: path_image target_tag\n";
        return -1;
    }
    cv::Mat image = cv::imread(argv[1]); // 读取图像
    cv::Mat img;
    cv::cvtColor(image, img, cv::COLOR_BGR2GRAY);
    cv::Mat temp;
    cv::namedWindow("TargetRect", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO); // 窗口
    cv::resizeWindow("TargetRect", 1280, 720);
    setMouseCallback("TargetRect", onmouse, &image); // 注册鼠标事件到“TargetRect”窗口，即使在该窗口下出现鼠标事件就执行onmouse函数的内容,最后一个参数为传入的数据。这里其实没有用到
    imshow("TargetRect", image);

    box = cv::Rect(0, 0, 0, 0);
    bool flag_save = false;
    while (1)
    {
          if (drawing_box)
        {
            // 不断更新正在画的矩形
            image.copyTo(temp); 
            rectangle(temp, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(0, 0, 255),3);

            imshow("TargetRect", temp); // 显示
        }
        // 检测是否有按下退出键
        int key = waitKey(30);
        if (key == 27)
        {          
            break; // 退出程序
        }
        else if (key == 115)
        {
            flag_save = true;
            cv::destroyAllWindows();
            break;
        }

        if (cv::getWindowProperty("TargetRect", cv::WND_PROP_AUTOSIZE) == -1)
        {
            cv::destroyAllWindows();
            break;
        }
    }
    if (flag_save && box.width > 10 && box.height > 10)
    {
        init_train_target("");
        ImageData image_data;
        std::vector<TargetRect> rects;

        image_data.channel = img.channels();
        image_data.data = img.data;
        image_data.height = img.rows;
        image_data.width = img.cols;
        TargetRect rect;
        rect.id = argv[2];
        rect.x = box.x + box.width / 2;
        rect.y = box.y + box.height / 2;
        rect.h = box.height;
        rect.w = box.width;
        rects.push_back(rect);
        add_train_target(image_data, rects);
        save_train_target("./model/");
    }

    return 0;
}