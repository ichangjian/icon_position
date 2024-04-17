#include "target_position_template.hpp"
#include <fstream>
#include <filesystem>
#include "cout_log.h"

template <typename T>
T ReadBinaryLittleEndian(std::istream *stream)
{
    T data_little_endian;
    stream->read(reinterpret_cast<char *>(&data_little_endian), sizeof(T));
    return data_little_endian;
}

ITPTemplate::ITPTemplate(/* args */)
{
}

ITPTemplate::~ITPTemplate()
{
}

int ITPTemplate::init(const std::string &_model_path)
{
    CLOGINFO << _model_path ;

    std::filesystem::path model_path(_model_path);
    if (!exists(model_path)) // 必须先检测目录是否存在才能使用文件入口.
    {
        CLOGERR << "model path isnt exist";
        return -1;
    }

    std::filesystem::directory_entry entry(model_path);                 // 文件入口
    if (entry.status().type() != std::filesystem::file_type::directory) // 这里用了C++11的强枚举类型
    {
        CLOGERR << "model path isnt directory";
        return -1;
    }
    std::filesystem::directory_iterator list(model_path); // 文件入口容器
    for (auto &it : list)
    {
        // std::cout << it.path().filename() << std::endl; // 通过文件入口（it）获取path对象，再得到path对象的文件名，将之输出

        std::string inputFile = it.path().string(); // 替换为你的文件路径

        // 获取文件名但不包括后缀
        std::string inputFileName = std::filesystem::path(inputFile).filename().stem().string();
        // 获取文件扩展名
        std::string fileExtension = std::filesystem::path(inputFile).extension().string();
        if (fileExtension == ".png")
        {
            CLOGINFO << "load "<< inputFile;

            cv::Mat img = cv::imread(inputFile, 0);
            if (img.empty())
            {
        CLOGERR << "can't load";
                continue;
            }
            TargetPosition aa;
            aa.id = inputFileName;
            aa.data = img;
            templates_.push_back(aa);
        }
        if (fileExtension == ".tmp")
        {
            CLOGINFO << "load " << inputFile;

            cv::Mat img = loadTMP(inputFile);
            if (img.empty())
            {
        CLOGERR << "can't load";
                continue;
            }
            
            TargetPosition aa;
            aa.id = inputFileName;
            aa.data = img;
            templates_.push_back(aa);
        }
    }
    if (templates_.size()==0)
    {
        CLOGERR << "can't init";
        return -1;
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
            TargetPosition target;
            target.id = templates_[i].id;
            target.x = maxLoc.x + templates_[i].data.cols / 2;
            target.y = maxLoc.y + templates_[i].data.rows / 2;
            target.w = templates_[i].data.cols;
            target.h = templates_[i].data.rows;

            CLOGINFO << "tag " << target.id << " value "
                     << maxVal << " " << target.x << "-" << target.y;
            _targets.push_back(target);
        }

    }

    return 0;
}

cv::Mat ITPTemplate::loadTMP(const std::string &_file)
{
    std::ifstream in_file(_file, std::ios::binary);
    if (!in_file.is_open())
    {
        return cv::Mat();
    }

    int height=ReadBinaryLittleEndian<int>(&in_file);
    int width=ReadBinaryLittleEndian<int>(&in_file);

    cv::Mat target(height, width, CV_8UC1);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            uchar pixel = ReadBinaryLittleEndian<uchar>(&in_file);
            target.at<uchar>(i, j) = pixel;
        }
    }
    // cv::imwrite("tmp.png", target);
    return target;
}
//=================================================================
int TTPTemplate::init(const std::string &_model_path)
{
    flag_use_tmp_ = true;
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
        catch (const std::filesystem::filesystem_error &ex)
        {
            std::cerr << "Error creating directory: " << ex.what() << std::endl;
            return -1; // 创建目录失败
        }
    }
    for (size_t i = 0; i < targets_.size(); i++)
    {
        if (flag_use_tmp_)
        {
            saveTMP(_save_path + "/" + targets_[i].id + ".tmp", targets_[i].data);
        }
        else
        {
            cv::imwrite(_save_path + "/" + targets_[i].id + ".png", targets_[i].data);
        }
    }

    return 0;
}

int TTPTemplate::saveTMP(const std::string &_file, const cv::Mat &_target)
{
    std::ofstream out_file(_file, std::ios::binary);
    if (!out_file.is_open())
    {
        CLOGERR << "can't save "<< _file;
        return -1;
    }
    out_file.write((char*)&_target.rows,sizeof(int));
    out_file.write((char*)&_target.cols,sizeof(int));

    for (size_t i = 0; i < _target.rows; i++)
    {
        for (size_t j = 0; j < _target.cols; j++)
        {
            uchar pixel= _target.at<uchar>(i, j);
            out_file.write((char*)&pixel,sizeof(uchar));
        }
    }
    out_file.close();

    return 0;
}