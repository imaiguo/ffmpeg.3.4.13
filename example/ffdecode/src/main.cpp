

#include <string>

#include <log4cplus/loggingmacros.h>
#include <log4cplus/logger.h>

#include "UtilTools.h"
#include "Log4cplusInit.h"
#include "FileDecode.h"

int main(int argc,char **argv) {

	FileDecode::Init();
	Log4cplusInitFunc(argv[0]);
	log4cplus::Logger logger = log4cplus::Logger::getRoot();
	LOG4CPLUS_TRACE(logger, "main start");

	// FileDecode decode("D:/video/01.mp4");
	// FileDecode decode("rtsp://user:passwd@192.168.0.2:554/h264/ch1/sub/av_stream");
	FileDecode decode("rtmp://localhost:1935/live/test");
	decode.StartRtsp();

	return 0;
}
