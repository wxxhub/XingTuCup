// int main()
// {
//   Caffe::set_mode(Caffe::GPU);
//   std::string net_prototxt = "../example/MobileNetSSD_deploy.prototxt";
//   caffe::Net<float>* net = new caffe::Net<float>(net_prototxt, caffe::Phase::TEST);
//   net->CopyTrainedLayersFrom("../snapshot/r1/mobilenet_iter_10000.caffemodel");
//   std::string img_file = "/home/wxx/testimg/02563.jpg";
//   cv::Mat im = cv::imread(img_file);

//   im.convertTo(im, CV_32F, 1/127.5, -1);
//   Blob<float>* input = net->input_blobs()[0];
//   input->Reshape(1, 3, im.rows, im.cols);
//   net->Reshape();

//   Mat ms[3];
//   float* ptr = input->mutable_cpu_data();
//   for (int i = 0; i < 3; ++i){
//       ms[i] = Mat(input->height(), input->width(), CV_32F, ptr);
//       ptr += input->width()*input->height();
//   }
//   split(im, ms);
//   net->Forward();

//   Blob<float>* out = net->output_blobs()[0];
//   const float* result = out->cpu_data();
//   const int num_det = out->height();
//   vector<vector<float> > detections;
//   for (int k = 0; k < num_det; ++k) {
//     if (result[0] == -1) {
//       // Skip invalid detection.
//       result += 7;
//       continue;
//     }
//     vector<float> detection(result, result + 7);
//     detections.push_back(detection);
//     result += 7;
//   }
//   for (int i = 0; i < detections.size(); ++i) {
//     const vector<float>& d = detections[i];
//     // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
//     CHECK_EQ(d.size(), 7);
//     const float score = d[2];
//     if (score >= 0) {
//       std::cout << static_cast<int>(d[1]) << " ";
//       std::cout << score << " ";
//       std::cout << static_cast<int>(d[3] * im.cols) << " ";
//       std::cout << static_cast<int>(d[4] * im.rows) << " ";
//       std::cout << static_cast<int>(d[5] * im.cols) << " ";
//       std::cout << static_cast<int>(d[6] * im.rows) << std::endl;
//     }
//   }
//   return 0;
// }

/*
void Detector::detect(Mat img)
{
    detections_.clear();

    cv::Mat sample;
    cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);

    cv::Mat sample_resized;
    cv::Size input_geometry_ = cv::Size(300, 300);
    if (sample.size() != input_geometry_)
        cv::resize(sample, sample_resized, input_geometry_);
    else
        sample_resized = sample;

    cv::Mat sample_float;
    sample_resized.convertTo(sample_float, CV_32FC3);

    // img.convertTo(img, CV_32FC3, 1/127.5, -1);

    Blob<float>* input = net_->input_blobs()[0];
    input->Reshape(1, 3, 300, 300);
    net_->Reshape();
    
    Mat ms[3];
    float* ptr = input->mutable_cpu_data();
    for (int i = 0; i < 3; ++i)
    {
        ms[i] = Mat(300, 300, CV_32FC3, ptr);
        ptr += 90000;
    }
    split(sample_float, ms);
#ifdef TIME_TEST
    ti_.reset();
#endif /* TIME_TEST *

    net_->Forward();

#ifdef TIME_TEST
    ti_.stop();
    std::cout<<"Forward time: "<<ti_.elapsed()<<"ms"<<std::endl;
#endif /* TIME_TEST *
    Blob<float>* out = net_->output_blobs()[0];
    const float* result = out->cpu_data();
    const int num_det = out->height();

    for (int k = 0; k < num_det; ++k) 
    {
        if (result[0] == -1) 
        {
        // Skip invalid detection.
        result += 7;
        continue;
        }
        vector<float> detection(result, result + 7);
        detections_.push_back(detection);
        result += 7;
    }
}
*/