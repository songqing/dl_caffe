
// test2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once 
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream> 

using namespace std;  
using namespace cv; 

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctest2App:
// �йش����ʵ�֣������ test2.cpp
//

class Ctest2App : public CWinApp
{
public:
	Ctest2App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctest2App theApp;