
// test2.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once 
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream> 

using namespace std;  
using namespace cv; 

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// Ctest2App:
// 有关此类的实现，请参阅 test2.cpp
//

class Ctest2App : public CWinApp
{
public:
	Ctest2App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern Ctest2App theApp;