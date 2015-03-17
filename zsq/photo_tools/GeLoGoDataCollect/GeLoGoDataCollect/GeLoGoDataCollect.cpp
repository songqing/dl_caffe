/***********************************************************************************	
/*	该函数实现检测到Logo后输出错检、漏检、正确的信息到txt文档中，
/*	并保存错检和正确的图片（即正样本和负样本）	
/***********************************************************************************/

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <direct.h>
#include <stdio.h>

 using namespace std;
 using namespace cv;

int FNumber = 0;
int threshVa = 0;
int category = 1;
int tempCate = 1;

void IMSGMethodFrameTest( Mat frame, int LeftX, int LeftY, int length, int save_startF, int save_endF, char* dirVideoname_F, char* dirVideoname_L)
{
	FNumber++;
	threshVa++;
	std::vector<Rect> faces;

	// 从第 save_startF 帧开始采集，从第 save_endF 帧结束
	//if( FNumber>=save_startF && FNumber<=save_endF ){ 
	if( threshVa >=save_startF ){ 
		char fraName[255];
		sprintf(fraName,"%s\\%06d.png",dirVideoname_F, FNumber);
		imwrite(fraName,frame);				// 保存关键帧

		char LogoName[255];
		Rect rectInImage = Rect(LeftX, LeftY, length, length);
		Mat colorROI = frame( rectInImage );		
		sprintf(LogoName,"%s\\%06d.png",dirVideoname_L, FNumber);
		imwrite(LogoName,colorROI);			// 从图像orgImage中提取一块子图像（大小rectInImage、命名fname）
	}
 }

void IMSGMethodVideoTest(char* Video_name, int LeftX, int LeftY, int length, int save_startF, int save_endF, char* dirVideoname_F, char* dirVideoname_L)
 {
	CvCapture* capture;
	double posFDouble = 0.0;
	double numFrames = 0.0;
	Mat frame;	
	if( tempCate != category){
		tempCate = category;
		FNumber = 0;
	}

	capture = cvCaptureFromFile(Video_name);		// = cvCaptureFromCAM( -1 );
	numFrames = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);		// 帧数	// double类型
	if( capture ){
		while( true ){
			frame = cvQueryFrame( capture );
			if( !frame.empty() && ( posFDouble < numFrames ) ){ 	  
				IMSGMethodFrameTest( frame, LeftX, LeftY, length, save_startF, save_endF, dirVideoname_F, dirVideoname_L );
				//int c = waitKey(10);
				//if( (char)c == 'c' ) { break; }
			}
			else{
				printf(" --(!) No captured frame -- Break!\n\n"); break; 
			}

			posFDouble = cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );
			posFDouble = posFDouble + 10 - 0.5;
			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, posFDouble);
		}
	}
	else{
		cout<<"Failed to Capture Video !"<<endl;
	}
}

 int main( int argc, const char** argv )
 {
 	//String Logo_cascade_name = "Logo4_32x32_Haar_L08_Detector.xml";	
	int LeftX = 0, LeftY = 0, width = 0, height = 0, length = 0;
	long save_startF = 0, save_endF = 0;
	char shotBuf[255];
	char videoname[255];
	char* dirname_F = "D:\\Dufy\\TVFrame";
	char* dirname_L = "D:\\Dufy\\TVLogo";
	char* dirname_V = "D:\\Dufy\\video_taibiao";
	int videoNum = 0;

	ifstream inFile;
	String DocName = "MarkResult.txt";
	inFile.open(DocName,ios::out);
	if(!inFile.is_open())
	{
		printf("Cannot open %s !\n", DocName);
		return 0;
	}

	while (!inFile.eof()) 
	{
		inFile>>category>>save_startF>>save_endF>>LeftX>>LeftY>>width>>height>>videoname;
		cout<<category<<" "<<save_startF<<" "<<save_endF<<" "<<LeftX<<" "<<LeftY<<" "<<width<<" "<<height<<" "<<videoname<<endl;
		length = width;		
		if( tempCate != category)
			videoNum = 1;
		else
			videoNum++;

		// 关键帧Frame 保存的路径
		// 创建类别 category = 01 的文件夹
		char* dirTVname_F = NULL;	
		dirTVname_F = new char[16+strlen(videoname)+1];
		sprintf(dirTVname_F, "%s\\%02d", dirname_F, category);		// category = 01
		if (_mkdir(dirTVname_F) != 0)		{	}
		cout<<dirTVname_F<<endl;		//"D:\\Dufy\\TVFrame\\01"
		
		// 创建“视频名”命名的文件夹
		char* dirVideoname_F = NULL;		
		//dirVideoname_F = new char[strlen(dirTVname_F)+strlen(videoname)+20];		
		//sprintf(dirVideoname_F, "%s\\%s", dirTVname_F, videoname);		//"D:\\Dufy\\TVFrame\\01\\[视听港湾Du75.com]2014102394246.mp4"
		dirVideoname_F = new char[strlen(dirTVname_F)+20];		
		sprintf(dirVideoname_F, "%s\\%03d", dirTVname_F, videoNum);
		if (_mkdir(dirVideoname_F) != 0)	{cout<<"Make Directory Failed!"<<endl;}
		cout<<dirVideoname_F<<endl;		//"D:\\Dufy\\TVFrame\\01\\001"


		// Logo保存的路径
		// 创建类别 category = 01 的文件夹
		char* dirTVname_L = NULL;	
		dirTVname_L = new char[16+strlen(videoname)+1];
		sprintf(dirTVname_L, "%s\\%02d", dirname_L, category);		// category = 01
		if (_mkdir(dirTVname_L) != 0)		{	}
		cout<<dirTVname_L<<endl;		//"D:\\Dufy\\TVLogo\\01"
		
		// 创建“视频名”命名的文件夹
		char* dirVideoname_L = NULL;	
		//dirVideoname_L = new char[strlen(dirTVname_L)+strlen(videoname)+20];		
		//sprintf(dirVideoname_L, "%s\\%s", dirTVname_L, videoname);		//"D:\\Dufy\\TVLogo\\01\\[视听港湾Du75.com]2014102394246.mp4"
		dirVideoname_L = new char[strlen(dirTVname_L)+20];		
		sprintf(dirVideoname_L, "%s\\%03d", dirTVname_L, videoNum);
		if (_mkdir(dirVideoname_L) != 0)	{cout<<"Make Directory Failed!"<<endl;}
		cout<<dirVideoname_L<<endl;		//"D:\\Dufy\\TVLogo\\01\\001"


		// video所在的路径
		char* dirTVname_V = NULL;	
		dirTVname_V = new char[20+strlen(videoname)+1];
		sprintf(dirTVname_V, "%s\\%02d", dirname_V, category);
		cout<<dirTVname_V<<endl;	// "D:\\Dufy\\video_taibiao\\01"
		
		char* dirVideoname_V = NULL;	
		dirVideoname_V = new char[strlen(dirTVname_V)+strlen(videoname)+30];		
		sprintf(dirVideoname_V, "%s\\%s", dirTVname_V, videoname);
		cout<<dirVideoname_V<<endl;	// "D:\\Dufy\\video_taibiao\\01\\[视听港湾Du75.com]2014102394246.mp4"

		IMSGMethodVideoTest(dirVideoname_V, LeftX, LeftY, length, save_startF, save_endF, dirVideoname_F, dirVideoname_L);
		
		delete []dirTVname_L;
		delete []dirVideoname_L;
		delete []dirTVname_V;
		delete []dirVideoname_V;
	}
	inFile.close();	
	return 0;
 }