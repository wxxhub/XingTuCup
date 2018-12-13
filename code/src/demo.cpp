#include <iostream>
#include <caffe/caffe.hpp>
#include "caffe/layer.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <vector>
#include "detector.h"

#include "timer.h"

#define VIDEO

// using namespace std;
using namespace caffe;
using namespace cv;

int main()
{
  std::string net_prototxt_file = "../example/MobileNetSSD_deploy.prototxt";
  std::string module_file = "../snapshot/r1/mobilenet_iter_10000.caffemodel";
  std::string output_video_path = "/home/wxx/develop/demo4/video/result/output39.avi";
  human::Detector detector(net_prototxt_file, module_file);
  timer ti;

  bool record = false;

#ifdef VIDEO
  // VideoCapture cap("/home/wxx/develop/demo3/video/test2.mp4");
  // VideoCapture cap("/home/wxx/develop/demo3/video/walking/test12.mp4");
  // VideoCapture cap("/home/wxx/develop/demo3/video/many_people/test20.mp4");
  VideoCapture cap("/home/wxx/develop/demo4/video/walk2/test35.mp4");
  Mat img;

  if(!cap.isOpened())
  {
    std::cout<<"open fail"<<std::endl;
    return -1;
  }

  VideoWriter output_video; //获取当前摄像头的视频信息
  cv::Size S = cv::Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)); //打开视频路劲，设置基本信息
  output_video.open(output_video_path, CV_FOURCC('X', 'V', 'I', 'D'), 30.0, S, true);

  int fps = cap.get(CAP_PROP_FPS);
  detector.setFPS(fps);
  while(cap.read(img))
  {
    // std::cout<<"processing ...."<<std::endl;
    // ti.reset();
    // flip(img, img, 0);
    detector.detect(img);
    // ti.stop();
    // std::cout<<"detector time: "<<ti.elapsed()<<"ms"<<std::endl;
    // ti.reset();
    detector.showResult(img, 1, true);
    // ti.stop();
    // std::cout<<"showResult time: "<<ti.elapsed()<<"ms"<<std::endl;
    if (record)
    {
      // std::cout<<"capting ......................"<<std::endl;
      output_video.write(img);
    }
  }
  output_video.release();
#else /* VIDEO */
  char img_file[100];
  for (int i = 0; i <= 2500; i++)
  {
    sprintf(img_file,"/home/wxx/develop/train/img/person_data/JPEGImages/%05d.jpg",i); 
    std::cout<<img_file<<std::endl;
    cv::Mat img = cv::imread(img_file);
    // ti.reset();
    detector.detect(img);
    // ti.stop();
    // std::cout<<"detector time: "<<ti.elapsed()<<"ms"<<std::endl;
    // ti.reset();
    detector.showResult(img, 0, true);
    // ti.stop();
    // std::cout<<"showResult time: "<<ti.elapsed()<<"ms"<<std::endl;
  }
#endif /* VIDEO */
  return 0;
}