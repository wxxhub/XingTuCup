#ifndef _CAFFE_DETECTOR_
#define _CAFFE_DETECTOR_

#include <string>
#include <vector>
#include <caffe/caffe.hpp>
#include "caffe/layer.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "judge_state.h"
#include "locking_id.h"

using namespace caffe;
using namespace cv;

#define MAX_DETECTIONS 2

namespace human
{

class Detector
{
public:
    Detector(std::string net_prototxt_file, std::string module_file);
    void detect(Mat img);
    void showResult(Mat img, int waitkey = 1, bool show_all = false);
    void setFPS(int fps);

private:
    caffe::Net<float>* net_;
    const std::string label_[2] = {"background", "person"};
    cv::Size input_geometry_;
    int num_channels_;
    double mean_value_;
    double reciprocal_mean_value_;
    float confidence_threshold_;
    
    vector<vector<float> > detections_;

    JudgeState judge_[CACHE_SIZE];
    void convertImg(Mat img);
    void WrapInputLayer(std::vector<cv::Mat>* input_channels);
    void Preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels);

    LockingID locking_id_;
};

}

#endif /* _CAFFE_DETECTOR_ */