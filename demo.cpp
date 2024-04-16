#include "target_position_sdk.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>

int test_image(const std::string &_file)
{
    cv::Mat image = cv::imread(_file); // 读取图像
    if (image.empty())
    {
        std::cout << "cant read " << _file << std::endl;
        return -1;
    }
    cv::Mat img;

    cv::cvtColor(image, img, cv::COLOR_BGR2GRAY);
    void *handle = create_target_position();
    init_target_position(handle, "./model/");

    ImageData image_data;
    image_data.channel = img.channels();
    image_data.data = img.data;
    image_data.height = img.rows;
    image_data.width = img.cols;
    std::vector<TargetRect> rects;
    get_target_position(handle, image_data, rects);

    for (size_t i = 0; i < rects.size(); i++)
    {
        cv::Rect box(rects[i].x - rects[i].w / 2, rects[i].y - rects[i].h / 2, rects[i].w, rects[i].h);
        std::cout << box << "\n";
        cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(255, 255, 255), 7);
        cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(0, 0, 255), 3);
        cv::putText(image, rects[i].id, cv::Point(box.x, box.y), 1, 2, cv::Scalar(0, 255, 0), 3);
    }
    if (rects.size() == 0)
    {
        cv::putText(image, "empty", cv::Point(image.cols / 2, image.rows / 2), 2, 3, cv::Scalar(0, 0, 255), 3);
    }

    cv::namedWindow("TargetResult", cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO); // 窗口
    cv::resizeWindow("TargetResult", 1280, 720);
    cv::imshow("TargetResult", image);
    cv::waitKey();
    release_target_position(handle);
    return 0;
}

int test_video(const std::string &_file)
{
    cv::VideoCapture cap(_file);
    if (!cap.isOpened())
    {
        std::cout << "cant read " << _file << std::endl;
        return -1;
    }

    void *handle = create_target_position();
    init_target_position(handle, "./model/");
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
        image_data.channel = img.channels();
        image_data.data = img.data;
        image_data.height = img.rows;
        image_data.width = img.cols;
        std::vector<TargetRect> rects;
        get_target_position(handle, image_data, rects);

        cv::Mat image = frame;
        for (size_t i = 0; i < rects.size(); i++)
        {
            cv::Rect box(rects[i].x - rects[i].w / 2, rects[i].y - rects[i].h / 2, rects[i].w, rects[i].h);
            std::cout << box << "\n";
            cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(255, 255, 255), 7);
            cv::rectangle(image, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(0, 0, 255), 3);
            cv::putText(image, rects[i].id, cv::Point(box.x, box.y), 1, 2, cv::Scalar(0, 255, 0), 3);
        }
        if (rects.size() == 0)
        {
            cv::putText(image, "empty", cv::Point(image.cols / 2, image.rows / 2), 2, 3, cv::Scalar(0, 0, 255), 3);
        }

        cv::imshow("TargetResult", image);
        if (cv::waitKey(1) == 27)
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

    release_target_position(handle);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "input error\n";
        std::cout << "Usage: path_image_or_video\n";
        return -1;
    }
    std::string file = argv[1];
    std::string fileExtension = std::filesystem::path(file).extension().string();
    if (fileExtension == ".png" || fileExtension == ".jpg")
    {
        return test_image(file);
    }
    else
    {
        return test_video(file);
    }

    return 0;
}

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
bool use_mask;
Mat img;
Mat templ;
Mat mask;
Mat result;
const char *image_window = "Source Image";
const char *result_window = "Result window";
int match_method;
int max_Trackbar = 5;
void MatchingMethod(int, void *);
const char *keys =
    "{ help h| | Print help message. }"
    "{ @input1 | Template_Matching_Original_Image.jpg | image_name }"
    "{ @input2 | Template_Matching_Template_Image.jpg | template_name }"
    "{ @input3 | | mask_name }";
int maina(int argc, char **argv)
{
    CommandLineParser parser(argc, argv, keys);

    img = imread((parser.get<String>("@input1")));
    templ = imread((parser.get<String>("@input2")), IMREAD_COLOR);
    if (argc > 3)
    {
        use_mask = true;
        mask = imread(samples::findFile(parser.get<String>("@input3")), IMREAD_COLOR);
    }
    if (img.empty() || templ.empty() || (use_mask && mask.empty()))
    {
        cout << "Can't read one of the images" << endl;
        return EXIT_FAILURE;
    }
    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);
    const char *trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
    MatchingMethod(0, 0);
    waitKey(0);
    return EXIT_SUCCESS;
}
void MatchingMethod(int, void *)
{
    Mat img_display;
    img.copyTo(img_display);
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
    if (use_mask && method_accepts_mask)
    {
        matchTemplate(img, templ, result, match_method, mask);
    }
    else
    {
        matchTemplate(img, templ, result, match_method);
    }
    // normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
    {
        matchLoc = minLoc;
        std::cout << "minVal" << minVal << std::endl;
        ;
    }
    else
    {
        matchLoc = maxLoc;

        std::cout << "maxVal" << maxVal << std::endl;
        ;
    }
    rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
    rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
    imshow(image_window, img_display);
    imshow(result_window, result);
    return;
}