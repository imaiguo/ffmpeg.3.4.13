
#include <fstream>

#include "UtilTools.h"

using namespace std;

#ifdef __linux__
void SetThreadCPU(int count){

	int cpus = 0;
	cpu_set_t mask;
	cpus = sysconf(_SC_NPROCESSORS_CONF);
	std::cout << "cpus: [" << cpus < <"]." << std::endl;
	
	CPU_ZERO(&mask);
	for(int i=0 ;i < cpus ;i++){
		CPU_SET(i,&mask);
		
		if(sched_setaffinity(0,sizeof(mask),&mask) == -1){

			cout<<"set CPU affinity failure,ERORR:"<<strerror(errno)<<endl;
		}
	}
}

int64_t getCurrentTime(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000+tv.tv_usec/1000;	
}

void listDir(const char *path, std::vector<string>&vpath) {

    //1.定义要扫描的文件后缀名
    vector<string> vback;
    vback.push_back(".mp4");
    vback.push_back(".MP4");
    vback.push_back(".MOV");
    vback.push_back(".m4v");
    vback.push_back(".flv");
    vback.push_back(".rm");
    vback.push_back(".rmvb");

	DIR	*pDir = NULL;
	struct dirent	*ent=NULL;
	char childpath[512];
	
	memset(childpath,0,sizeof(childpath));
	pDir = opendir(path);
	if(pDir == NULL){
		//cout<<"open path["<<path<<"]failed."<<endl;
		return;
	}
	
	//cout<<"find video file:"<<endl;
	while((ent = readdir(pDir)) !=NULL){
		string strtmp;
		strtmp.append(path);
		strtmp.append("/");
		strtmp.append(ent->d_name);
		if(ent->d_type & DT_DIR){
			if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
				continue;
			listDir(strtmp.c_str(),vpath);
		}else{
			if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
				continue;
			size_t pos = strtmp.rfind(".");
			
			if(string::npos != pos){
				string substr = strtmp.substr(pos);
				for(string str:vback){
					if(substr==str){
						//cout<<"\t"<<strtmp<<endl;
						vpath.push_back(strtmp);
						break;
					}
				}   
			}
		}
	}

	closedir(pDir);	
}

#endif

void rotateMat(cv::Mat & mat,int angle){
	if(angle < 1)
		return;
	
	cv::Mat imgtmp;
	if(1 == angle){		
		cv::transpose(mat,imgtmp);
		flip(imgtmp,mat,1);
		
	}else if(2 == angle){
		flip(mat,imgtmp,-1);
		mat = imgtmp;
	}else if(3 == angle){
		cv::transpose(mat,imgtmp);
		flip(imgtmp,mat,0);
	}
}

void AVFrame2Img(AVFrame *pFrame, cv::Mat& img){
	int frameHeight = pFrame->height;
	int frameWidth = pFrame->width;
	int channels = 3;
	
	//创建保存yuv数据的buffer
	uchar* pDecodedBuffer = (uchar*)malloc(frameHeight*frameWidth * sizeof(uchar)*channels);

	//从AVFrame中获取yuv420p数据，并保存到buffer
	int i, j, k;
	//拷贝y分量
	for (i = 0; i < frameHeight; i++){
		memcpy(pDecodedBuffer + frameWidth * i,
			pFrame->data[0] + pFrame->linesize[0] * i,
			frameWidth);
	}

	//拷贝u分量
	for (j = 0; j < frameHeight / 2; j++){
		memcpy(pDecodedBuffer + frameWidth * i + frameWidth / 2 * j,
			pFrame->data[1] + pFrame->linesize[1] * j,
			frameWidth / 2);
	}

	//拷贝v分量
	for (k = 0; k < frameHeight / 2; k++){
		memcpy(pDecodedBuffer + frameWidth * i + frameWidth / 2 * j + frameWidth / 2 * k,
			pFrame->data[2] + pFrame->linesize[2] * k,
			frameWidth / 2);
	}

     //mathod 1 : CV_YUV420p2RGB   快一点
    cv::Mat yuvtmp( frameHeight * 3/ 2, frameWidth, CV_8UC1, (unsigned char *) pDecodedBuffer);
    cvtColor(yuvtmp, img, CV_YUV420p2RGB);


    //mathod 2 : for  慢一些
	//将buffer中的yuv420p数据转换为RGB;
    // img = cv::Mat::zeros(frameHeight, frameWidth, CV_8UC3);
	// Yuv420p2Rgb32(pDecodedBuffer, img.data, frameWidth, frameHeight);

	free(pDecodedBuffer);
}

int getRotate(AVStream *avStream){
	AVDictionaryEntry *tag = NULL;
	int m_Rotate = 0;
	tag = av_dict_get(avStream->metadata,"rotate",tag,0);
	
	if(tag == NULL)
		m_Rotate=0;
	else{
		int angle = atoi(tag->value);
		angle %= 360;
		if(angle == 90)
			m_Rotate = 1;
		else if(angle == 180)
			m_Rotate = 2;
		else if(angle == 270)
			m_Rotate = 3;
	}

	return m_Rotate;
}

void prepareStream(const char *path,uint8_t **data,uint32_t &len){

	std::ifstream infile(path, std::ifstream::binary);
	infile.seekg(0,infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	
	len = size;
	if(size <=0){
		len =0;
		return;
	}

	*data = new uint8_t[size];
	if(*data){
		infile.read((char *)*data,size);
	}else{
		len =0;
	}

	infile.close();

}
