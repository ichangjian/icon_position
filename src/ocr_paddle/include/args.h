// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// #include <gFLAGSs/gFLAGSs.h>
#include <string>
// common args
extern bool FLAGS_use_gpu;// = false;             //, "Infering with GPU or CPU.");
extern bool FLAGS_use_tensorrt;// = false;        //, "Whether use tensorrt.");
extern int FLAGS_gpu_id;// = 0;                   //, "Device id of GPU to execute.");
extern int FLAGS_gpu_mem;// = 4000;               //, "GPU id when infering with GPU.");
extern int FLAGS_cpu_threads;// = 10;             //, "Num of threads with CPU.");
extern bool FLAGS_enable_mkldnn;// = false;       //, "Whether use mkldnn with CPU.");
extern std::string FLAGS_precision;// = "fp32";   //, "Precision be one of fp32/fp16/int8FLAGS_");
extern bool FLAGS_benchmark;// = false;           //, "Whether use benchmark.");
extern std::string FLAGS_output;// = "./output/"; //, "Save benchmark log path.");
extern std::string FLAGS_image_dir;// = "";       //, "Dir of input image.");
extern std::string FLAGS_type;// = "ocr";         //, "Perform ocr or structure, the value is selected in ['ocr','structure'].");
// detection related
extern std::string FLAGS_det_model_dir;// = "";         //, "Path of det inference model.");
extern std::string FLAGS_limit_type;// = "max";         //, "limit_type of input image.");
extern int FLAGS_limit_side_len;// = 960;               //, "limit_side_len of input image.");
extern double FLAGS_det_db_thresh ;//= 0.3;             //, "Threshold of det_db_thresh.");
extern double FLAGS_det_db_box_thresh ;//= 0.6;         //, "Threshold of det_db_box_thresh.");
extern double FLAGS_det_db_unclip_ratio ;//= 1.5;       //, "Threshold of det_db_unclip_ratio.");
extern bool FLAGS_use_dilation;// = false;              //, "Whether use the dilation on output map.");
extern std::string FLAGS_det_db_score_mode;// = "slow"; //, "Whether use polygon score.");
extern bool FLAGS_visualize;// = true;                  //, "Whether show the detection results.");
// classification related
extern bool FLAGS_use_angle_cls;// = false;     //, "Whether use use_angle_cls.");
extern std::string FLAGS_cls_model_dir;// = ""; //, "Path of cls inference model.");
extern double FLAGS_cls_thresh ;//= 0.9;        //, "Threshold of cls_thresh.");
extern int FLAGS_cls_batch_num;// = 1;          //, "cls_batch_num.");
// recognition related
extern std::string FLAGS_rec_model_dir;// = "";                                         //, "Path of rec inference model.");
extern int FLAGS_rec_batch_num;// = 6;                                                  //, "rec_batch_num.");
extern std::string FLAGS_rec_char_dict_path;// = "../../ppocr/utils/ppocr_keys_v1.txt"; //, "Path of dictionary.");
extern int FLAGS_rec_img_h;// = 48;                                                     //, "rec image height");
extern int FLAGS_rec_img_w;// = 320;                                                    //, "rec image width");

// layout model related
extern std::string FLAGS_layout_model_dir;// = "";                                                             //, "Path of table layout inference model.");
extern std::string FLAGS_layout_dict_path;// = "../../ppocr/utils/dict/layout_dict/layout_publaynet_dict.txt"; //, "Path of dictionary.");
extern double FLAGS_layout_score_threshold ;//= 0.5;                                                           //, "Threshold of score.");
extern double FLAGS_layout_nms_threshold ;//= 0.5;                                                             //, "Threshold of nms.");
// structure model related
extern std::string FLAGS_table_model_dir;// = "";    //, "Path of table struture inference model.");
extern int FLAGS_table_max_len;// = 488;             //, "max len size of input image.");
extern int FLAGS_table_batch_num;// = 1;             //, "table_batch_num.");
extern bool FLAGS_merge_no_span_structure;// = true; //,"Whether merge <td> and </td> to <td></td>");
extern std::string FLAGS_table_char_dict_path;// =
    // "../../ppocr/utils/dict/table_structure_dict_ch.txt"; //,"Path of dictionary.");

// ocr forward related
extern bool FLAGS_det;// = true;     //, "Whether use det in forward.");
extern bool FLAGS_rec;// = true;     //, "Whether use rec in forward.");
extern bool FLAGS_cls;// = false;    //, "Whether use cls in forward.");
extern bool FLAGS_table;// = false;  //, "Whether use table structure in forward.");
extern bool FLAGS_layout;// = false; //, "Whether use layout analysis in forward.");
