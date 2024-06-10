#include "target_position_ocrpp.hpp"
#include <include/args.h>
#include <include/paddleocr.h>
#include <include/paddlestructure.h>
#include <iostream>

int ITPOCRPP::init(const std::string &_model_path)
{
  path_det_ = _model_path + "/ch_PP-OCRv4_det_infer";

  path_rec_ = _model_path + "/ch_PP-OCRv4_rec_infer";

  FLAGS_det_model_dir = path_det_;
  FLAGS_rec_model_dir = path_rec_;
  FLAGS_rec_char_dict_path = _model_path + "/ppocr_keys_v1.txt";
  ocr_ = new PaddleOCR::PPOCR();
  return 0;
}

int ITPOCRPP::getPosition(double _timestamp, const cv::Mat &_image, std::vector<TargetPosition> &_targets)
{

  std::vector<PaddleOCR::OCRPredictResult> ocr_results =
      ((PaddleOCR::PPOCR *)ocr_)->ocr(_image, true, true, false);
  // PaddleOCR::Utility::print_result(ocr_results);
  // PaddleOCR::Utility::VisualizeBboxes(_image, ocr_results, "BB.PNG");

  for (int i = 0; i < ocr_results.size(); i++)
  {
    // det
    std::vector<std::vector<int>> boxes = ocr_results[i].box;
    float center_x = 0.0;
    float center_y = 0.0;
    float width = 0.0;
    float height = 0.0;
    if (boxes.size() == 4)
    {
      for (int n = 0; n < boxes.size(); n++)
      {
        center_x += boxes[n][0];
        center_y += boxes[n][1];
      }
      center_x /= boxes.size();
      center_y /= boxes.size();
      width += (boxes[1][0] - boxes[0][0]);
      width += (boxes[2][0] - boxes[3][0]);
      width /= 2.0;
      height += (boxes[3][1] - boxes[0][1]);
      height += (boxes[2][1] - boxes[1][1]);
      height /= 2.0;
    }
    else
    {
      continue;
    }
    // rec
    if (ocr_results[i].score > 0.8)
    {
      TargetPosition target;

      target.id = ocr_results[i].text;
      target.timestamp = _timestamp;
      target.x = center_x;
      target.y = center_y;
      target.h = height;
      target.w = width;
      _targets.push_back(target);
    }
  }
  return 0;
}

ITPOCRPP::ITPOCRPP()
{
}
ITPOCRPP::~ITPOCRPP()
{
  delete ((PaddleOCR::PPOCR *)ocr_);
}
