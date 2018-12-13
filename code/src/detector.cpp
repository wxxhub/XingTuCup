#include "detector.h"

#include <stdio.h>
#define ONE_TEST

namespace human
{

Detector::Detector(std::string net_prototxt_file, std::string module_file)
    : confidence_threshold_(0.5),
      mean_value_(127.5)
{
    Caffe::set_mode(Caffe::GPU);
    net_ = new caffe::Net<float>(net_prototxt_file, caffe::Phase::TEST);
    net_->CopyTrainedLayersFrom(module_file);
    reciprocal_mean_value_ = 1/mean_value_;

    Blob<float>* input_layer = net_->input_blobs()[0];
    num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1)
        << "Input layer should have 1 or 3 channels.";
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());
}

void Detector::WrapInputLayer(std::vector<cv::Mat>* input_channels) 
{
    Blob<float>* input_layer = net_->input_blobs()[0];

    int width = input_layer->width();
    int height = input_layer->height();
    float* input_data = input_layer->mutable_cpu_data();
    for (int i = 0; i < input_layer->channels(); ++i) {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }
}

void Detector::Preprocess(const cv::Mat& img, std::vector<cv::Mat>* input_channels) 
{
    /* Convert the input image to the input image format of the network. */
    // change img_channels to network_channels
    cv::Mat sample;
    if (img.channels() == 3 && num_channels_ == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
    else if (img.channels() == 4 && num_channels_ == 1)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
    else if (img.channels() == 4 && num_channels_ == 3)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
    else if (img.channels() == 1 && num_channels_ == 3)
        cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
    else
        sample = img;

    // change img_size to network_size
    cv::Mat sample_resized;
    if (sample.size() != input_geometry_)
        cv::resize(sample, sample_resized, input_geometry_);
    else
        sample_resized = sample;

    cv::Mat sample_float;
    if (num_channels_ == 3)
        sample_resized.convertTo(sample_float, CV_32FC3, reciprocal_mean_value_, -1);
    else
        sample_resized.convertTo(sample_float, CV_32FC1, reciprocal_mean_value_, -1);
        
    cv::split(sample_float, *input_channels);
}

void Detector::detect(Mat img)
{
    detections_.clear();
    Blob<float>* input_layer = net_->input_blobs()[0];
    input_layer->Reshape(1, num_channels_,
                        input_geometry_.height, input_geometry_.width);
    /* Forward dimension change to all layers. */
    net_->Reshape();

    std::vector<cv::Mat> input_channels;
    WrapInputLayer(&input_channels);

    Preprocess(img, &input_channels);

    net_->Forward();

    /* Copy the output layer to a std::vector */
    Blob<float>* result_blob = net_->output_blobs()[0];
    const float* result = result_blob->cpu_data();
    const int num_det = result_blob->height();
    for (int k = 0; k < num_det; ++k) {
        if (result[0] == -1) {
        // Skip invalid detection.
        result += 7;
        continue;
        }
        vector<float> detection(result, result + 7);
        detections_.push_back(detection);
        result += 7;
    }    
}

void Detector::showResult(Mat img, int waitkey, bool show_all)
{
    if(!show_all)
    {
        const vector<float> d = detections_[0]; // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
        CvPoint up_p;
        CvPoint down_p;
        char score[5];
        int id = static_cast<int>(d[1]);
        sprintf(score, "%0.2f", d[2]);
        up_p.x = static_cast<int>(d[3] * img.cols);
        up_p.y = static_cast<int>(d[4] * img.rows);
        down_p.x = static_cast<int>(d[5] * img.cols);
        down_p.y = static_cast<int>(d[6] * img.rows);
        int result = judge_[0].getJudgeResult(up_p.x, up_p.y, down_p.x, down_p.y, img.rows);
        std::string text = label_[id]+score;
        putText(img, text, up_p, cv::FONT_HERSHEY_COMPLEX, 0.6, cv::Scalar(0, 255, 255));
        if (result == FALLDOWN)
            rectangle(img,up_p ,down_p,Scalar(0,255,255));
        else
            rectangle(img,up_p ,down_p,Scalar(255,0,0));
    }
    else
    {
        for (int i = 0; i < detections_.size(); ++i) 
        {
            if (detections_.size()<=0)
                break;

            const vector<float>& d = detections_[i]; // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
            if(d[2] <= confidence_threshold_)
                break;
            CvPoint up_p;
            CvPoint down_p;
            char score[5];
            up_p.x = static_cast<int>(d[3] * img.cols);
            up_p.y = static_cast<int>(d[4] * img.rows);
            down_p.x = static_cast<int>(d[5] * img.cols);
            down_p.y = static_cast<int>(d[6] * img.rows);
            
            locking_id_.addObject(up_p.x, up_p.y, down_p.x, down_p.y, i);
        }

        locking_id_.matchID();
// get result by detections_.size()
/*
        for (int i = 0; i < detections_.size(); i++)
        {
            const vector<float>& d = detections_[i]; // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
            if (d[2] <= confidence_threshold_)
                break;
            
            int id = locking_id_.getID(i);
            if (id < 0 || id > CACHE_SIZE)
            {
                // printf("detections_.size(): %d\n", detections_.size());
                printf("result_id: %d\n", id);
                continue;
            }

            CvPoint up_p;
            CvPoint down_p;
            char score[20];
            int label_id = static_cast<int>(d[1]);
            up_p.x = static_cast<int>(d[3] * img.cols);
            up_p.y = static_cast<int>(d[4] * img.rows);
            down_p.x = static_cast<int>(d[5] * img.cols);
            down_p.y = static_cast<int>(d[6] * img.rows);
            int result = judge_[id].getJudgeResult(up_p.x, up_p.y, down_p.x, down_p.y, img.rows);
            sprintf(score, "%0.2f ID:%d", d[2], id);
            std::string text = label_[label_id]+score;
            putText(img, text, up_p, cv::FONT_HERSHEY_COMPLEX, 0.6, cv::Scalar(0, 255, 255));
            if (result == FALLDOWN)
                rectangle(img,up_p ,down_p,Scalar(0,255,255));
            else
                rectangle(img,up_p ,down_p,Scalar(255,0,0));
        }
*/

// get result by CACHE_SIZE
        for (int i = 0; i < CACHE_SIZE; i++)
        {
            int up_x, up_y, down_x, down_y;
            int id_result = locking_id_.getResult(i, up_x, up_y, down_x, down_y);

            if (id_result != MATCH_ID)
            {
                // printf("id_result: %d\n", id_result);
                if (id_result == CLEANER_ID)
                    judge_[i].cleanJudgeList();
                continue;
            }
            // printf("have id: %d\n", i);
            int judge_result = judge_[i].getJudgeResult(up_x, up_y, down_x, down_y, img.rows);
            char text[10];
            sprintf(text, "ID:%d", i);
            putText(img, text, CvPoint(up_x, up_y), cv::FONT_HERSHEY_COMPLEX, 0.6, cv::Scalar(0, 255, 255));
            if (judge_result == FALLDOWN)
                rectangle(img, CvPoint(up_x, up_y), CvPoint(down_x, down_y), Scalar(0,255,255));
            else
                rectangle(img, CvPoint(up_x, up_y), CvPoint(down_x, down_y), Scalar(255,0,0));   
        }

    }

    imshow("result", img);
    cvWaitKey(waitkey);
}

void Detector::setFPS(int fps)
{
    for (int i = 0; i < MAX_DETECTIONS; i++)
        judge_[i].setFPS(fps);
}
}