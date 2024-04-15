#include "target_position_template.hpp"
#include <filesystem>
ITPTemplate::ITPTemplate(/* args */)
{
}

ITPTemplate::~ITPTemplate()
{
}

int ITPTemplate::init(const std::string &_model_path)
{
    std::cout << _model_path << std::endl;

    std::filesystem::path model_path(_model_path);
    if (!exists(model_path)) // 必须先检测目录是否存在才能使用文件入口.
    {
        std::cout << "error" << std::endl;
        return -1;
    }

    std::filesystem::directory_entry entry(model_path);                 // 文件入口
    if (entry.status().type() != std::filesystem::file_type::directory) // 这里用了C++11的强枚举类型
    {
        std::cout << "error" << std::endl;
        return -1;
    }
    std::filesystem::directory_iterator list(model_path); // 文件入口容器
    for (auto &it : list)
    {
        std::cout << it.path().filename() << std::endl; // 通过文件入口（it）获取path对象，再得到path对象的文件名，将之输出

        std::string inputFile = it.path().string(); // 替换为你的文件路径

        // 获取文件名但不包括后缀
        std::string inputFileName = std::filesystem::path(inputFile).filename().stem().string();
        // 获取文件扩展名
        std::string fileExtension = std::filesystem::path(inputFile).extension().string();
        if (fileExtension == ".png")
        {
            std::cout << inputFile << std::endl;

            cv::Mat img = cv::imread(inputFile, 0);
            TargetPosition aa;
            aa.id = inputFileName;
            aa.data = img;
            templates_.push_back(aa);
        }
    }
    return 0;
}
int ITPTemplate::getPosition(double _timestamp, const cv::Mat &_image, std::vector<TargetPosition> &_targets)
{
    for (size_t i = 0; i < templates_.size(); i++)
    {
        cv::Mat result;
        cv::matchTemplate(_image, templates_[i].data, result, 5);
        double minVal;
        double maxVal;
        cv::Point minLoc;
        cv::Point maxLoc;
        cv::Point matchLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

        // matchLoc = maxLoc;
        if (maxVal > 0.8)
        {
            std::cout << maxLoc << "\n";
            TargetPosition target;
            target.id = templates_[i].id;
            target.x = maxLoc.x + templates_[i].data.cols / 2;
            target.y = maxLoc.y + templates_[i].data.rows / 2;
            target.w = templates_[i].data.cols;
            target.h = templates_[i].data.rows;

            _targets.push_back(target);
        }

        std::cout << "maxVal" << maxVal << std::endl;
        ;
    }

    return 0;
}

int TTPTemplate::init(const std::string &_model_path)
{
    return 0;
}

int TTPTemplate::addTarget(const std::string &_tag, const cv::Mat &_image)
{
    TargetPosition target;
    target.id = _tag;
    target.data = _image;
    targets_.push_back(target);
    return 0;
}

int TTPTemplate::train(const std::string &_save_path)
{
    std::filesystem::path save_path(_save_path);

    if (!std::filesystem::exists(save_path)) 
    {
        try 
        {
            std::filesystem::create_directories(save_path);
        } 
        catch (const std::filesystem::filesystem_error& ex) 
        {
            std::cerr << "Error creating directory: " << ex.what() << std::endl;
            return -1; // 创建目录失败
        }
    } 
    for (size_t i = 0; i < targets_.size(); i++)
    {
        cv::imwrite(_save_path + "/" + targets_[i].id + ".png", targets_[i].data);
    }

    return 0;
}
