#pragma once

//功能开关
#define LOOP_NUM 1
//#define _WRITE_PIC_

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/video/video.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2\imgproc\types_c.h>

extern "C"{
	
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libswresample/swresample.h>

#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>

// #include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
}


#include <cstdlib>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/initializer.h>

#include <iomanip>


#include <chrono>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <string>

void AVFrame2Img(AVFrame *pFrame, cv::Mat& img);
int getRotate(AVStream *avStream);
void getVidaoMat(std::string path);
void rotateMat(cv::Mat & mat,int angle);
