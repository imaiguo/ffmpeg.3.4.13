
#include "FileDecode.h"
#include "UtilTools.h"

void FileDecode::Init(){
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
}

FileDecode::FileDecode(std::string filepath){
	filename = filepath;
	LOG4CPLUS_TRACE(logger, "read to open file: [ " <<  filename.c_str() << "].");
}

void FileDecode::start(){
	int videoStream = -1;
	int ret = 0;
	int angle = -1;
	bool rotate = true;
	bool writepic = true;
	
	AVFormatContext *pFormatCtx = NULL;
	AVCodecContext *pCodecCtx = NULL;
	const AVCodec *pCodec = NULL;
	AVFrame *pFrame	= NULL;
	AVPacket packet;

	do{
		pFormatCtx = avformat_alloc_context();
		if(avformat_open_input(&pFormatCtx, filename.c_str(), NULL,NULL)!=0){
			LOG4CPLUS_ERROR(logger, "open file:[" << filename.c_str() << "] error");
			break;
		}

		if(avformat_find_stream_info(pFormatCtx,NULL)<0){
			LOG4CPLUS_ERROR(logger, "Could't find stream information");
			break;
		}

		for(int i =0;i < pFormatCtx->nb_streams;i++){
			if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
				videoStream = i;
			}
		}

		if(-1 == videoStream){
			LOG4CPLUS_ERROR(logger, "no find vedio stream.");
			break;
		}

		angle = getRotate(pFormatCtx->streams[videoStream]);
		LOG4CPLUS_INFO(logger, "video rotate angle.");

		int width= pFormatCtx->streams[videoStream]->codecpar->width;
		int heigh = pFormatCtx->streams[videoStream]->codecpar->height;

		pCodecCtx = avcodec_alloc_context3(NULL);

		if(!pCodecCtx){
			LOG4CPLUS_ERROR(logger, "avcodec_alloc_context3 error.");
			break;
		}

		ret = avcodec_parameters_to_context(pCodecCtx,pFormatCtx->streams[videoStream]->codecpar);

		if(ret < 0){
			LOG4CPLUS_ERROR(logger, "avcodec_parameters_to_context error.");
			break;
		}

		// av_codec_set_pkt_timebase(pCodecCtx, pFormatCtx->streams[videoStream]->time_base);

		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

		if(pCodec == NULL){
			LOG4CPLUS_ERROR(logger, "avcodec_find_decoder error.");
			break;
		}

		pCodecCtx->thread_count = 4;

		if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
			LOG4CPLUS_ERROR(logger, "avcodec_open2 error.");
			break;
		}

		pFrame = av_frame_alloc();

		av_init_packet(&packet);

		int counts = 0;
		while(av_read_frame(pFormatCtx,&packet) >= 0){
			if(packet.stream_index == videoStream){
				ret =avcodec_send_packet(pCodecCtx,&packet);
				if(ret < 0){
					LOG4CPLUS_ERROR(logger, "avcodec_send_packet.");
					break;
				}

				LOG4CPLUS_TRACE(logger, "packet.pts:[" << packet.pts << "].");
				LOG4CPLUS_TRACE(logger, "packet.dts:[" << packet.dts << "].");
				LOG4CPLUS_TRACE(logger, "packet.pos:[" << packet.pos << "].");
				LOG4CPLUS_TRACE(logger, "packet.duration:[" << packet.duration << "].");
				while(0 == avcodec_receive_frame(pCodecCtx, pFrame)){
					if(rotate){
						cv::Mat mat;
						AVFrame2Img(pFrame, mat);
						rotateMat(mat,angle);

						if(writepic){
							std::string name = "picffmpeg//";
							name += std::to_string(++counts);
							name += ".jpg";
							LOG4CPLUS_TRACE(logger, "file saved to [" << name.c_str() << "].");
							// sprintf(szFilename,"picffmpeg//%s_%d.jpg",filename.c_str(),counts);
							cv::imwrite(name, mat);
						}
					}

					av_packet_unref(&packet);
				}
			}
			av_packet_unref(&packet);
		}
	}while(0);

	av_packet_unref(&packet);

	if(NULL != pFrame){
		av_frame_unref(pFrame);
		av_frame_free(&pFrame);
	}

	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
}

void FileDecode::StartRtsp(){
	int videoStream = -1;
	int ret = 0;
	int angle = -1;
	bool rotate = true;
	bool writepic = true;

	AVFormatContext *pFormatCtx = NULL;
	AVCodecContext *pCodecCtx = NULL;
	const AVCodec *pCodec = NULL;
	AVFrame *pFrame	= NULL;
	AVPacket packet;

	do{
		pFormatCtx = avformat_alloc_context();

		AVDictionary *options = NULL;
		av_dict_set(&options, "buffer_size", "4096000", 0); //设置缓存大小
		av_dict_set(&options, "rtsp_transport", "tcp", 0);  //以tcp的方式打开,
		av_dict_set(&options, "stimeout", "5000000", 0);    //设置超时断开链接时间，单位us,   5s
		av_dict_set(&options, "max_delay", "500000", 0);    //设置最大时延

		if(avformat_open_input(&pFormatCtx, filename.c_str(), NULL, &options)!=0){
			LOG4CPLUS_ERROR(logger, "open file:[" << filename.c_str() << "] error");
			break;
		}
		
		if(avformat_find_stream_info(pFormatCtx,NULL)<0){
			LOG4CPLUS_ERROR(logger, "Could't find stream information");
			break;
		}
		
		for(int i =0;i < pFormatCtx->nb_streams;i++){
			if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
				videoStream = i;
			}
		}
		
		if(-1 == videoStream){
			LOG4CPLUS_ERROR(logger, "no find vedio stream.");
			break;
		}
		
		angle = getRotate(pFormatCtx->streams[videoStream]);
		LOG4CPLUS_INFO(logger, "video rotate angle.");
		
		int width= pFormatCtx->streams[videoStream]->codecpar->width;
		int heigh = pFormatCtx->streams[videoStream]->codecpar->height;
				
		pCodecCtx = avcodec_alloc_context3(NULL);

		if(!pCodecCtx){
			LOG4CPLUS_ERROR(logger, "avcodec_alloc_context3 error.");
			break;
		}
		
		ret = avcodec_parameters_to_context(pCodecCtx,pFormatCtx->streams[videoStream]->codecpar);
		
		if(ret < 0){
			LOG4CPLUS_ERROR(logger, "avcodec_parameters_to_context error.");
			break;
		}
		
		// av_codec_set_pkt_timebase(pCodecCtx, pFormatCtx->streams[videoStream]->time_base);

		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
		
		if(pCodec == NULL){
			LOG4CPLUS_ERROR(logger, "avcodec_find_decoder error.");
			break;
		}

		pCodecCtx->thread_count = 4;

		if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
			LOG4CPLUS_ERROR(logger, "avcodec_open2 error.");
			break;
		}
		
		pFrame = av_frame_alloc();
		
		av_init_packet(&packet);

		int counts = 0;

		LOG4CPLUS_TRACE(logger, "packet.pts:[" << packet.pts << "].");
		LOG4CPLUS_TRACE(logger, "packet.dts:[" << packet.dts << "].");
		LOG4CPLUS_TRACE(logger, "packet.pos:[" << packet.pos << "].");
		LOG4CPLUS_TRACE(logger, "packet.duration:[" << packet.duration << "].");


		while(av_read_frame(pFormatCtx, &packet) >= 0){
			if(packet.stream_index == videoStream){
				ret =avcodec_send_packet(pCodecCtx, &packet);
				if(ret < 0){
					LOG4CPLUS_ERROR(logger, "avcodec_send_packet.");
					break;
				}

				while(0 == avcodec_receive_frame(pCodecCtx, pFrame)){

					LOG4CPLUS_TRACE(logger, "packet.pts:[" << packet.pts << "].");
					LOG4CPLUS_TRACE(logger, "packet.dts:[" << packet.dts << "].");
					LOG4CPLUS_TRACE(logger, "packet.pos:[" << packet.pos << "].");
					LOG4CPLUS_TRACE(logger, "packet.duration:[" << packet.duration << "].");

					if(rotate){
						cv::Mat mat;
						AVFrame2Img(pFrame, mat);
						rotateMat(mat,angle);

						if(writepic){
							std::string name = "picffmpeg//";
							name += std::to_string(++counts);
							name += ".jpg";
							LOG4CPLUS_TRACE(logger, "file saved to [" << name.c_str() << "].");
							cv::imwrite(name, mat);
						}
					}

					av_packet_unref(&packet);
				}
			}
			av_packet_unref(&packet);
		}
	}while(0);

	av_packet_unref(&packet);

	if(NULL != pFrame){
		av_frame_unref(pFrame);
		av_frame_free(&pFrame);
	}

	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
}
