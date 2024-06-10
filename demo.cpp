#include "target_position_sdk.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "cout_log.h"

std::vector<void *> get_handle(const std::string &_config_file)
{
    std::vector<void *> handles;

    cv::FileStorage fs(_config_file, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        CLOGERR << "can't open " << _config_file;
        return handles;
    }

    cv::FileNode objects = fs["object"];

    // iterate through a sequence using FileNodeIterator
    cv::FileNodeIterator it = objects.begin(), it_end = objects.end();
    for (int idx = 0; it != it_end; ++it, idx++)
    {
        std::string method = (*it)["method"];
        void *handle = create_target_position(method);
        std::string model_path = (*it)["model_path"];
        init_target_position(handle, model_path);
        handles.push_back(handle);
    }
    fs.release();

    return handles;
}

void release_handle(std::vector<void *> &_handles)
{
    for (auto handle : _handles)
    {
        release_target_position(handle);
    }
    return;
}

int test_image(const std::string &_file)
{
    cv::Mat image = cv::imread(_file); // 读取图像
    if (image.empty())
    {
        CLOGERR << "can't read " << _file;
        return -1;
    }
    cv::Mat img;

    cv::cvtColor(image, img, cv::COLOR_BGR2GRAY);
    ImageData image_data;
    image_data.channel = image.channels();
    image_data.data = image.data;
    image_data.height = image.rows;
    image_data.width = image.cols;

    std::vector<void *> handles = get_handle("config.yaml");

    // iterate through a sequence using FileNodeIterator
    for (auto &handle : handles)
    {
        std::vector<TargetRect> rects;
        get_target_position(handle, image_data, rects);

        for (size_t i = 0; i < rects.size(); i++)
        {
            cv::Rect box(rects[i].x - rects[i].w / 2, rects[i].y - rects[i].h / 2, rects[i].w, rects[i].h);
            cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(255, 255, 255), 7);
            cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(0, 0, 255), 3);
            // cv::putText(image, rects[i].id, cv::Point(box.x, box.y), 1, 2, cv::Scalar(0, 255, 0), 3);
            cv::putText(image, rects[i].id, cv::Point(box.x, box.y), cv::Scalar(0, 255, 0), cv::FontFace("C:/Windows/Fonts/simsun.ttc"), 20);
            std::cout << rects[i].id << "-id\n";
        }
        if (rects.size() == 0)
        {
            cv::putText(image, "empty", cv::Point(image.cols / 2, image.rows / 2), 2, 3, cv::Scalar(0, 0, 255), 3);
        }
    }
    release_handle(handles);

    cv::namedWindow("TargetResult", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO); // 窗口
    cv::resizeWindow("TargetResult", 1280, 720);
    cv::imshow("TargetResult", image);
    cv::waitKey();
    CLOGINFO << "exit";
    return 0;
}

int test_video(const std::string &_file)
{
    cv::VideoCapture cap(_file);
    if (!cap.isOpened())
    {
        CLOGERR << "can't read " << _file;
        return -1;
    }

    std::vector<void *> handles = get_handle("./config.yaml");
    cv::namedWindow("TargetResult", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO); // 窗口
    cv::resizeWindow("TargetResult", 1280, 720);
    cv::Mat frame;
    int index = 0;
    while (true)
    {
        cap >> frame;      // 等价于cap.read(frame);
        if (frame.empty()) // 如果某帧为空则退出循环
            break;
        if (index++ % 10 != 0)
            continue;
        cv::Mat img;

        cv::cvtColor(frame, img, cv::COLOR_BGR2GRAY);
        ImageData image_data;
        image_data.channel = frame.channels();
        image_data.data = frame.data;
        image_data.height = frame.rows;
        image_data.width = frame.cols;

        cv::Mat image = frame.clone();

        for (auto &handle : handles)
        {
            std::vector<TargetRect> rects;
            get_target_position(handle, image_data, rects);

            for (size_t i = 0; i < rects.size(); i++)
            {
                cv::Rect box(rects[i].x - rects[i].w / 2, rects[i].y - rects[i].h / 2, rects[i].w, rects[i].h);

                cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(255, 255, 255), 7);
                cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(0, 0, 255), 3);
                // cv::putText(image, rects[i].id, cv::Point(box.x, box.y), 1, 2, cv::Scalar(0, 255, 0), 3);
                cv::putText(image, rects[i].id, cv::Point(box.x, box.y), cv::Scalar(0, 255, 0), cv::FontFace("C:/Windows/Fonts/simsun.ttc"), 20);
            }
            if (rects.size() == 0)
            {
                cv::putText(image, "empty", cv::Point(image.cols / 2, image.rows / 2), 2, 3, cv::Scalar(0, 0, 255), 3);
            }
        }
        cv::imshow("TargetResult", image);
        if (cv::waitKey(10) == 27)
        {          // 检测是否有按下退出键
            break; // 退出程序
        }
        if (cv::getWindowProperty("TargetResult", cv::WND_PROP_AUTOSIZE) == -1)
        {
            cv::destroyAllWindows();
            break;
        }
    }
    cap.release(); // 释放资源

    release_handle(handles);
    CLOGINFO << "exit";
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        CLOGERR << "input error";
        CLOGINFO << "Usage: path_image_or_video";
        return -1;
    }
    SetConsoleOutputCP(CP_UTF8);
    std::string file = argv[1];
    std::string fileExtension = std::filesystem::path(file).extension().string();
    if (fileExtension == ".png" || fileExtension == ".jpg")
    {
        CLOGINFO << "test image";
        return test_image(file);
    }
    else
    {
        CLOGINFO << "test video";
        return test_video(file);
    }

    return 0;
}
