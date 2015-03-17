
// test2Dlg.cpp : 实现文件

#include "stdafx.h"
#include "test2.h"
#include "test2Dlg.h"
#include "afxdialogex.h"
#include<iostream>
#include <fstream>
#include <iomanip>
#include"CvvImage.h"
#include "highgui.h"
#include "cv.h"
#include <afxpriv.h>
#include <windows.h>
#include <string.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <io.h>

#pragma comment(lib, "User32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PICWIDTH 525
#define PICHEIGHT 350
#define SLEEPTIME 100
ofstream outFile;
CvCapture* capture;
char* window_nam = "Logo标注";

char fname[225];	// 视频/图片名称
char ext[30];		// 视频/图片扩展名
//int nPos = 0;	// 滑动条百分数
char imageLpng[225];
char imageLpath[225];		// 需要遍历的文件夹（名）
char fnameext[200];			// 视频名称+扩展名
double leftX = 0, leftY = 0, rightX = 0, rightY = 0;
double numFrames = 0, widFrames = PICWIDTH, heigFrames = PICHEIGHT;
double rate = 0, rateX = 0, rateY = 0;
double vertWIDTH = 0, vertHEIGHT = 0;
//int width = 0, height = 0;
IplImage* videoframe = NULL;
IplImage* image_l = NULL;
int frameNum = 0;	// 图片序列的当前序号
char imageLname[255];	// 图片序列的图片名称(路径+名称)


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	void ShowImage(IplImage* img, UINT ID);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Ctest2Dlg 对话框
Ctest2Dlg::Ctest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctest2Dlg::IDD, pParent)
	, m_radiobtngroup1(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_LogoI = 0;
	m_leftX = 0;
	m_leftY = 0;
	//m_image = 0;
	m_Dragging = 0;
	m_LogoID = 0;
	m_numFrames = 0;
	//m_widFrames = PICWIDTH;
	//m_heigFrames = PICHEIGHT;
	m_widFrames = 0;
	m_heigFrames = 0;
	m_posFrames = 0;	// 自定义变量，全局
	m_posF = 0;			// IDC_EDIT11对应变量（当前帧显示），全局
	m_startF = 0;
	m_endF = 0;
	m_slider = 0;
	//m_skewstep = 0;
	m_rightX = 0;
	m_rightY = 0;
	m_width = 0;
	m_height = 0;
	m_Vname = _T("");
	m_nPos = 0;			// 滑动条百分数
	m_PointOrigin.x = 0;
	m_PointOrigin.y = 0;
	m_PointOld.x = 0;
	m_PointOld.y = 0;
}

void Ctest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	//这个函数是控件与类成员交换数据用的
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT2, m_LogoI);
	DDX_Text(pDX, IDC_EDIT5, m_leftX);
	DDX_Text(pDX, IDC_EDIT6, m_leftY);
	//DDX_Text(pDX, IDC_EDIT7, m_rightX);
	//DDX_Text(pDX, IDC_EDIT8, m_rightY);
	//DDX_Slider(pDX, IDC_SLIDER1, m_image);
	DDX_Control(pDX, IDC_STATIC_PIC, m_ctrlPic);
	DDX_Control(pDX, IDC_EDIT2, m_Logo);
	DDX_Text(pDX, IDC_EDIT12, m_numFrames);
	DDX_Text(pDX, IDC_EDIT13, m_widFrames);
	DDX_Text(pDX, IDC_EDIT14, m_heigFrames);
	//  DDX_Text(pDX, IDC_EDIT11, m_posFrames);
	DDX_Text(pDX, IDC_EDIT11, m_posF);
	DDX_Text(pDX, IDC_EDIT3, m_startF);
	DDX_Text(pDX, IDC_EDIT4, m_endF);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSlider);			//滑块专用
	DDX_Slider(pDX, IDC_SLIDER1, m_slider);
	//DDX_Text(pDX, IDC_EDIT16, m_skewstep);
	DDX_Text(pDX, IDC_EDIT7, m_width);
	DDX_Text(pDX, IDC_EDIT8, m_height);
	DDX_Text(pDX, IDC_EDIT1, m_Vname);
	DDX_Text(pDX, IDC_EDIT9, m_nPos);
	DDX_Radio(pDX, IDC_RADIO1, m_radiobtngroup1);
}

BEGIN_MESSAGE_MAP(Ctest2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Ctest2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Ctest2Dlg::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON6, &Ctest2Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON3, &Ctest2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON8, &Ctest2Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &Ctest2Dlg::OnBnClickedButton7)
	ON_EN_CHANGE(IDC_EDIT2, &Ctest2Dlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON9, &Ctest2Dlg::OnBnClickedButton9)
	ON_EN_CHANGE(IDC_EDIT5, &Ctest2Dlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &Ctest2Dlg::OnEnChangeEdit6)
	ON_STN_CLICKED(IDC_STATIC_PIC, &Ctest2Dlg::OnStnClickedStaticPic)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT1, &Ctest2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON5, &Ctest2Dlg::OnBnClickedButton5)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT3, &Ctest2Dlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &Ctest2Dlg::OnEnChangeEdit4)
	//ON_BN_CLICKED(IDC_CHECK1, &Ctest2Dlg::OnBnClickedCheck1)
	//ON_LBN_SELCHANGE(IDC_LIST3, &Ctest2Dlg::OnLbnSelchangeList3)
	ON_EN_CHANGE(IDC_EDIT15, &Ctest2Dlg::OnEnChangeEdit15)
	ON_BN_CLICKED(IDC_CHECK6, &Ctest2Dlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK5, &Ctest2Dlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK4, &Ctest2Dlg::OnBnClickedCheck4)
	//ON_LBN_SELCHANGE(IDC_LIST4, &Ctest2Dlg::OnLbnSelchangeList4)
	ON_EN_CHANGE(IDC_EDIT11, &Ctest2Dlg::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT10, &Ctest2Dlg::OnEnChangeEdit10)
	ON_BN_CLICKED(IDC_BUTTON4, &Ctest2Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON14, &Ctest2Dlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &Ctest2Dlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_ENLARGE_WINSIZE, &Ctest2Dlg::OnBnClickedEnlargeWinsize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &Ctest2Dlg::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT14, &Ctest2Dlg::OnEnChangeEdit14)
	ON_BN_CLICKED(IDC_BUTTON10, &Ctest2Dlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &Ctest2Dlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON13, &Ctest2Dlg::OnBnClickedButton13)
	ON_EN_CHANGE(IDC_EDIT16, &Ctest2Dlg::OnEnChangeEdit16)
	ON_BN_CLICKED(IDC_RADIO1, &Ctest2Dlg::OnRadioBtnGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO2, &Ctest2Dlg::OnRadioBtnGroup1Clicked)
	ON_BN_CLICKED(IDC_RADIO3, &Ctest2Dlg::OnRadioBtnGroup1Clicked)
	//ON_BN_CLICKED(IDC_RADIO3, &Ctest2Dlg::OnBnClickedRadio3)
//	ON_NOTIFY(BCN_DROPDOWN, IDC_RADIO1, &Ctest2Dlg::OnRadioBtnGroup1Clicked)
ON_BN_CLICKED(IDC_BUTTON12, &Ctest2Dlg::OnBnClickedButton12)
ON_BN_CLICKED(IDC_BUTTON16, &Ctest2Dlg::OnBnClickedButton16)
ON_BN_CLICKED(IDC_BUTTON17, &Ctest2Dlg::OnBnClickedButton17)
ON_EN_CHANGE(IDC_EDIT12, &Ctest2Dlg::OnEnChangeEdit12)
END_MESSAGE_MAP()


// Ctest2Dlg 消息处理程序

BOOL Ctest2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	String DocName = "MarkResult.txt";
	outFile.open(DocName,ios::out);
	if(!outFile)	{cerr<<"open error!"<<endl;exit(1);}
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	m_ctrlSlider.SetRange(0,100); //设置滑块位置的最大值和最小值
	m_ctrlSlider.SetPos(0);          //设置滑块的默认当前位置

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Ctest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void Ctest2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR Ctest2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*int LogoID = 0;
IplImage* src=0;
CvPoint temPt;
void on_mouse( int event, int x, int y, int flags, void* ustc)  
{  
    CvFont font;  
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);  
          
    if( event == CV_EVENT_LBUTTONDOWN )  
    {    
		CvPoint pt = cvPoint(x,y);
        char temp[16];  
        sprintf(temp,"(%d,%d)",pt.x,pt.y);
		temPt = pt;
        cvPutText(src,temp, pt, &font, cvScalar(255, 255, 255, 0));  
        cvCircle( src, pt, 2,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );  
        cvShowImage( window_nam, src );  
    }   
} */ 

int Ctest2Dlg::ShowImage(IplImage* img, UINT ID)		// 用于在控件 ID上显示图片 img
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	//GetDlgItem(ID)->GetClientRect(&rect);
	
	rateX = PICWIDTH / widFrames;
	rateY = PICHEIGHT / heigFrames;
	if( rateX <= rateY ){
		rate = rateX;
		vertWIDTH = widFrames*rate;
		vertHEIGHT = heigFrames*rate;
		rect = CRect( 0, 0, (int) vertWIDTH, (int)vertHEIGHT );
		
	}
	else{
		rate = rateY;
		vertWIDTH = widFrames*rate;
		vertHEIGHT = heigFrames*rate;
		rect = CRect( 0, 0, (int) vertWIDTH, (int)vertHEIGHT );
	}
	
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC,&rect);
	//Sleep(30);

	ReleaseDC(pDC);
	return 0;
}

void Ctest2Dlg::OnBnClickedButton12()			// 保存帧...
{
	// TODO: 在此添加控件通知处理程序代码
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	//InvalidateRect(hwnd, false);//重画时擦除背景
	//UpdateWindow();

	CFileDialog dlg(
        FALSE, _T("*.jpg"), NULL,
        OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		//_T("video files(*.mp4; *.3gp) |*.mp4; *.3gp | All Files (*.*) |*.*||"), NULL
        _T(" All Files (*.*)|*.*| video files(*.mp4; *.3gp)|*.mp4; *.3gp | image files(*.jpg; *.png)|*.jpg; *.png ||"), NULL
        );                                        // 选项视频的约定
    dlg.m_ofn.lpstrTitle = _T("Save Frame");    // 打开文件对话框的标题名
    if( dlg.DoModal() != IDOK )                    // 判断是否获得...(视频?)
        return;
    
    CString mPath = dlg.GetPathName();// 获取视频路径
	mPath.Format(_T("%s"), dlg.GetPathName());
	USES_CONVERSION;
	char* dirFrame = T2A(mPath);
	cvSaveImage(dirFrame, videoframe, 0);

	/*char fnameMiss[100];			// 可用于不出选项框 的保存
	sprintf(fnameMiss,"%s%06d.jpg",dirFrame, m_posF);
	cvSaveImage(fnameMiss, frame, 0);*/
}


void Ctest2Dlg::OnBnClickedButton1()			// “打开...”按钮
{
	// TODO: 在此添加控件通知处理程序代码								 ///////////////////////////////////////////////////////	
	LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	InvalidateRect(hwnd, true);//重画时擦除背景
	UpdateWindow();
	/*CPoint point;
	point.x = 0;
	point.y = 0;*/
	m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;
	string window_name = "Logo标注工具";

	CFileDialog dlg(
        TRUE, _T("*.mp4"), NULL,
        OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		//_T("video files(*.mp4; *.3gp) |*.mp4; *.3gp | All Files (*.*) |*.*||"), NULL
        _T(" All Files (*.*)|*.*| video files(*.mp4; *.3gp)|*.mp4; *.3gp | image files(*.jpg; *.png)|*.jpg; *.png ||"), NULL
        );                                        // 选项视频的约定
    dlg.m_ofn.lpstrTitle = _T("Open Video");    // 打开文件对话框的标题名
    if( dlg.DoModal() != IDOK )                    // 判断是否获得视频
        return;
    
    CString mPath = dlg.GetPathName();// 获取视频路径
	mPath.Format(_T("%s"), dlg.GetPathName());
	USES_CONVERSION;
	char* Video_name = T2A(mPath);
	//char* Video_name = "D:/Dufy/00视频特定内容认证与检索平台/video_taibiao/CCTV4/cctv4-2014-10-18-00-003.mp4";
	//char* Video_name = "1234.mp4"

	_finddata_t fileInfo;
	int pngSum = 0;
	char drive[100];
	char dir[255];
	_splitpath( Video_name, drive, dir, fname, ext );
	sprintf(imageLpng, "%s%s*.png", drive, dir);		// 需要遍历的文件夹里的文件（名）；（*.png或改为*.*或改为*.jpg)
	sprintf(imageLpath, "%s%s", drive, dir);			// 需要遍历的文件夹名
	sprintf(fnameext, "%s%s", fname, ext);				// 视频名称+扩展名
	CString Vname;
	Vname = A2T(fnameext);
	//Vname.Format(_T("%s%s"), fname, ext);
	m_Vname = Vname;
	UpdateData(FALSE);
	//GetDlgItem(IDC_EDIT1)->SetWindowText( aaa );

	IplImage* image = NULL;
	long handle = _findfirst(imageLpng, &fileInfo);
	UpdateData(TRUE);
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//AfxMessageBox(_T("1 被选中"));
			capture = cvCaptureFromFile(Video_name);
			numFrames = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);		// 帧数	// double类型
			widFrames = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);		// 帧宽
			heigFrames = cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);	// 帧高
			m_numFrames = (int)numFrames;	m_widFrames = (int)widFrames;	m_heigFrames = (int)heigFrames;		// int类型
			UpdateData(FALSE);

			//SetTimer(1,40,NULL);			
			videoframe = cvQueryFrame( capture );
			if( videoframe !=NULL ){ 	  
				ShowImage(videoframe, IDC_STATIC_PIC);	 		
				m_posF = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
				m_slider = 100*m_posF/numFrames;
				m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				UpdateData(FALSE);   
			}
			else{
				AfxMessageBox(_T("--(!) No captured frame -- Break!"));
			}
			InvalidateRect(hwnd, true);//重画时不擦除背景
			UpdateWindow();
			break;

		case 1:				// 图片序列
			//AfxMessageBox(_T("2 被选中"));			
			image_l = cvLoadImage( Video_name, -1 );
			if (handle == -1L){
				cerr << "failed to transfer files" << endl;
				break;
			}
			while (_findnext(handle, &fileInfo) == 0) {
				pngSum ++;
				//cout << fileInfo.name <<endl;
			}
			m_numFrames = pngSum + 1;			// 帧总数

			widFrames = image_l->width;		// 帧宽
			heigFrames = image_l->height;		// 帧高
			m_widFrames = (int)widFrames;	m_heigFrames = (int)heigFrames;		// int类型
			UpdateData(FALSE);

			//InvalidateRect(hwnd, false);//重画时不擦除背景
			//UpdateWindow();
			if( image_l !=NULL ){ 	  
				ShowImage(image_l, IDC_STATIC_PIC);	
				sscanf(fname, "frame%5d", &frameNum);
				sprintf( imageLname, "%sframe%05d.png", imageLpath, frameNum);
				m_posF = frameNum;
				m_slider = 100*m_posF/(double)m_numFrames;
				m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				UpdateData(FALSE);   
			}
			else{
				AfxMessageBox(_T("--(!) No captured frame -- Break!"));
			}
			break;

		case 2:				// 图片
			//AfxMessageBox(_T("3 被选中"));
			image = cvLoadImage( Video_name, -1 );
			widFrames = image->width;		// 帧宽
			heigFrames = image->height;		// 帧高
			m_numFrames = 1;	m_widFrames = (int)widFrames;	m_heigFrames = (int)heigFrames;		// int类型
			m_posF = 1;
			UpdateData(FALSE);

			if( image !=NULL ){ 	  
				ShowImage(image, IDC_STATIC_PIC);	 				
				UpdateData(FALSE);   				
			}
			else{
				AfxMessageBox(_T("--(!) No Picture Load! -- Break!"));
			}
			//InvalidateRect(hwnd, false);//重画时不擦除背景
			//UpdateWindow();
			break;
	}	
}

void Ctest2Dlg::OnTimer(UINT_PTR nIDEvent)			// 定时器：show视频每一帧 /////////////////////////////////////////////////////////////////////////////////
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	//IplImage* frame = NULL;
	
	//ofstream test;		// 要删
	//test.open("test.txt");

	UpdateData(TRUE);	
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			if( capture )
			{
				videoframe = cvQueryFrame( capture );	// frame = cvCreateFileCapture( capture );			
				//IplImage* src = NULL;
				//IplImage iplimage = frame;
				//src = &iplimage;
				if( videoframe != NULL ){ 	  						
					ShowImage(videoframe, IDC_STATIC_PIC);	 		
					m_posF = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
					m_slider = 100*m_posF/numFrames;
					m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
					UpdateData(FALSE);  
				}
			}
			break;
		
		case 1:				// 图片序列
			sscanf(fname, "frame%5d", &frameNum);
			frameNum ++;
			sprintf(fname, "frame%05d", frameNum);
			sprintf(imageLname, "%sframe%05d.png", imageLpath, frameNum);
			//test<<imageLname<<endl<<imageLpath<<endl<<frameNum<<endl<<imageLpng<<endl;
			//test.close();

			image_l = cvLoadImage( imageLname, -1 );
			if( image_l !=NULL ){ 	  
				ShowImage(image_l, IDC_STATIC_PIC);	
				m_posF = frameNum;
				m_slider = 100*m_posF/(double)m_numFrames;
				m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				UpdateData(FALSE);   
			}
			break;
		
		case 2:				// 图片
			//AfxMessageBox(_T("图片没有‘暂停/播放’功能"));
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

int state = 0;	// state=0，播放
void Ctest2Dlg::OnBnClickedButton3()		// 快退
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	/*m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;*/
	CString str;	
	GetDlgItem(IDC_EDIT10)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int step = 50;	//操作步长默认为50
	//GetDlgItem(IDC_EDIT10)->SetWindowText(_T("50"));
	if(intStr)
		step = intStr;
		//GetDlgItem(IDC_EDIT10)->SetWindowText(str);
	
	double posFDouble;
	IplImage* frame;
	UpdateData(TRUE);
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//AfxMessageBox(_T("1 被选中"));
			posFDouble = cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
			posFDouble = posFDouble - step - 0.5;  
			m_posF = (int)posFDouble;
			m_slider = 100*posFDouble/numFrames;  	
			m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
			UpdateData(FALSE);

			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, posFDouble);	// 快退
			state = 0;			// state=0，播放
			//SetTimer(1,40,NULL);
			frame = cvQueryFrame( capture );
			if( frame !=NULL ){ 	  
				ShowImage(frame, IDC_STATIC_PIC);	 		  
			}
			else{
				AfxMessageBox(_T("--(!) No captured frame -- Break!")); 
			}
			break;

		case 1:				// 图片序列
			//AfxMessageBox(_T("2 被选中"));
			sscanf(fname, "frame%5d", &frameNum);
			frameNum -= step;
			if( frameNum >= 1 && frameNum <= m_numFrames ){
				sprintf(fname, "frame%05d", frameNum);
				sprintf(imageLname, "%sframe%05d.png", imageLpath, frameNum);
				m_posF = frameNum;
				m_slider = 100*m_posF/(double)m_numFrames;
				m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				UpdateData(FALSE);

				image_l = cvLoadImage( imageLname, -1 );
				if( image_l !=NULL ){ 	  
					ShowImage(image_l, IDC_STATIC_PIC);	  
				}
			}
			else{
				frameNum += step;
				m_posF = frameNum;
				UpdateData(FALSE);
				AfxMessageBox(_T("已超出‘图片序列’范围！"));
			}
			break;

		case 2:				// 图片
			AfxMessageBox(_T("图片没有‘快退’功能"));
			break;
	}			
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);// 重画，保证鼠标点下时上一个矩形就消失
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();	
}


void Ctest2Dlg::OnBnClickedButton16()		// 退一帧
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	/*m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;*/

	double posFDouble;
	IplImage* frame;
	UpdateData(TRUE);
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//AfxMessageBox(_T("1 被选中"));
			posFDouble = cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
			posFDouble = posFDouble - 1.5;  
			m_posF = (int)posFDouble;
			m_slider = 100*posFDouble/numFrames;
			m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
			UpdateData(FALSE);

			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, posFDouble);	// 退一帧
			state = 0;			// state=0，播放
			//SetTimer(1,40,NULL);
			frame = cvQueryFrame( capture );
			if( frame !=NULL ){ 	  
				ShowImage(frame, IDC_STATIC_PIC);	 		  
			}
			else{
				AfxMessageBox(_T("--(!) No captured frame -- Break!")); 
			}	
			break;

		case 1:				// 图片序列
			//AfxMessageBox(_T("2 被选中"));
			sscanf(fname, "frame%5d", &frameNum);
			frameNum --;
			if( frameNum >= 1 && frameNum <= m_numFrames ){
				sprintf(fname, "frame%05d", frameNum);
				sprintf(imageLname, "%sframe%05d.png", imageLpath, frameNum);
				m_posF = frameNum;
				m_slider = 100*m_posF/(double)m_numFrames;
				m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				UpdateData(FALSE);

				image_l = cvLoadImage( imageLname, -1 );
				if( image_l !=NULL ){ 	  
					ShowImage(image_l, IDC_STATIC_PIC);	  
				}
			}
			else{
				frameNum ++;
				m_posF = frameNum;
				UpdateData(FALSE);
				AfxMessageBox(_T("已超出‘图片序列’范围！"));
			}
			break;

		case 2:				// 图片
			AfxMessageBox(_T("图片没有‘退一帧’功能"));
			break;
	}	
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);// 重画，保证鼠标点下时上一个矩形就消失
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
}


void Ctest2Dlg::OnBnClickedButton17()		// 进一帧
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	/*m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;*/

	double posFDouble;
	IplImage* frame;
	UpdateData(TRUE);
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//AfxMessageBox(_T("1 被选中"));
			posFDouble = cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
			posFDouble = posFDouble + 0.5;
			m_posF = (int)posFDouble;   	
			m_slider = 100*posFDouble/numFrames;
			m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
			UpdateData(FALSE);

			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, posFDouble);	// 进一帧
			state = 0;			// state=0，播放
			//SetTimer(1,40,NULL);
			frame = cvQueryFrame( capture );
			if( frame !=NULL ){ 	  
				ShowImage(frame, IDC_STATIC_PIC);	 		  
			}
			else{
				AfxMessageBox(_T("--(!) No captured frame -- Break!"));
			}	
			break;

		case 1:				// 图片序列
			//AfxMessageBox(_T("2 被选中"));
			sscanf(fname, "frame%5d", &frameNum);
			frameNum ++;
			if( frameNum >= 1 && frameNum <= m_numFrames ){
				sprintf(fname, "frame%05d", frameNum);
				sprintf(imageLname, "%sframe%05d.png", imageLpath, frameNum);
				m_posF = frameNum;
				m_slider = 100*m_posF/(double)m_numFrames;
				m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				UpdateData(FALSE);

				image_l = cvLoadImage( imageLname, -1 );
				if( image_l !=NULL ){ 	  
					ShowImage(image_l, IDC_STATIC_PIC);	  
				}
			}
			else{
				frameNum --;
				m_posF = frameNum;
				UpdateData(FALSE);
				AfxMessageBox(_T("已超出‘图片序列’范围！"));
			}
			break;

		case 2:				// 图片
			AfxMessageBox(_T("图片没有‘进一帧’功能"));
			break;
	}		
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);// 重画，保证鼠标点下时上一个矩形就消失
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
}


void Ctest2Dlg::OnBnClickedButton2()		// 快进
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	/*m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;*/

	CString str;	
	GetDlgItem(IDC_EDIT10)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int step = 50;	//操作步长默认为50
	if(intStr)
		step = intStr;

	double posFDouble;
	IplImage* frame;
	UpdateData(TRUE);
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//AfxMessageBox(_T("1 被选中"));
			posFDouble = cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
			posFDouble = posFDouble + step - 0.5;  
			m_posF = (int)posFDouble;
			m_slider = 100*posFDouble/numFrames;  	
			m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
			UpdateData(FALSE);

			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, posFDouble);	// 快进
			state = 0;			// state=0，播放
			//SetTimer(1,40,NULL);
			frame = cvQueryFrame( capture );
			if( frame !=NULL ){ 	  
				ShowImage(frame, IDC_STATIC_PIC);	 				
			}
			else{
				AfxMessageBox(_T("--(!) No captured frame -- Break!")); 
			}
			break;

		case 1:				// 图片序列
			//AfxMessageBox(_T("2 被选中"));
			sscanf(fname, "frame%5d", &frameNum);
			frameNum += step;
			if( frameNum >= 1 && frameNum <= m_numFrames ){
				sprintf(fname, "frame%05d", frameNum);
				sprintf(imageLname, "%sframe%05d.png", imageLpath, frameNum);
				m_posF = frameNum;
				m_slider = 100*m_posF/(double)m_numFrames;
				m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				UpdateData(FALSE);

				image_l = cvLoadImage( imageLname, -1 );
				if( image_l !=NULL ){ 	  
					ShowImage(image_l, IDC_STATIC_PIC);	  
				}
			}
			else{
				frameNum -= step;
				m_posF = frameNum;
				UpdateData(FALSE);
				AfxMessageBox(_T("已超出‘图片序列’范围！"));
			}
			break;

		case 2:				// 图片
			AfxMessageBox(_T("图片没有‘快进’功能"));
			break;
	}
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);// 重画，保证鼠标点下时上一个矩形就消失
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
}

void Ctest2Dlg::OnBnClickedButton5()		// 暂停/播放
{
	// TODO: 在此添加控件通知处理程序代码
	/*m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;*/

	switch(m_radiobtngroup1)
	{
		case 0:				// 视频		
		case 1:				// 图片序列
			if(state){				// state=1，暂停
				KillTimer(1);
				//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);// 重画，保证鼠标点下时上一个矩形就消失
				//InvalidateRect(hwnd, true);//重画时不擦除背景
				//UpdateWindow();

				//double posFDouble;
				//posFDouble = cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
				//m_posF = (int)posFDouble;
				//m_slider = 100*posFDouble/numFrames;  	
				//m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
				//UpdateData(FALSE);

				//cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, posFDouble);	// 快退
				//state = 0;			// state=0，播放
				////SetTimer(1,40,NULL);
				//IplImage* frame = cvQueryFrame( capture );
				//if( frame !=NULL ){ 	  
				//	ShowImage(frame, IDC_STATIC_PIC);	 				
				//}
				state = 0;
			}
			else{					// state=0，播放
				SetTimer(1,40,NULL);
				state = 1;
			}
			break;
		
		case 2:				// 图片
			AfxMessageBox(_T("图片没有‘暂停/播放’功能"));
			break;
	}
}

void Ctest2Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)			// 滑动条控制
{
	// TODO: 在此添加控件通知处理程序代码	
	KillTimer(1);	
	/*m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;*/
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
	m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置,百分数
	m_slider = m_nPos;	
	UpdateData(FALSE);
	//CString str,str1;
	//str.Format(_T("%d%%"),m_nPos);
	//SetDlgItemText(IDC_EDIT9,str);		// 显示所调节的数据,另外做一个编辑框
	//str1.Format(_T("%d"),m_posF);
	//SetDlgItemText(IDC_EDIT11,str1);

	UpdateData(TRUE);
	IplImage* frame;
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//AfxMessageBox(_T("1 被选中"));
			m_posF = (int)(numFrames * m_nPos / 100);// 当前帧位置
			UpdateData(FALSE);

			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, m_posF);	
			state = 0;			// state=0，播放
			//SetTimer(1,40,NULL);
			frame = cvQueryFrame( capture );
			if( frame !=NULL ){ 	  
				ShowImage(frame, IDC_STATIC_PIC);	 						   
			}
			break;

		case 1:				// 图片序列
			//AfxMessageBox(_T("2 被选中"));
			m_posF = (int)(m_numFrames * m_nPos / 100);// 当前帧位置
			UpdateData(FALSE);   
			frameNum = m_posF;
			sprintf(fname, "frame%05d", frameNum);
			sprintf(imageLname, "%sframe%05d.png", imageLpath, frameNum);

			image_l = cvLoadImage( imageLname, -1 );
			if( image_l !=NULL ){ 	  
				ShowImage(image_l, IDC_STATIC_PIC);	
			}
			break;

		case 2:				// 图片
			//AfxMessageBox(_T("3 被选中"));
			break;
	}
}

void Ctest2Dlg::OnBnClickedButton4()			// 结束
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);
	/*m_PointOrigin.x = 0;	m_PointOrigin.y = 0;
	m_PointOld.x = 0;	m_PointOld.y = 0;*/
	//IplImage* kong = NULL;
	//ShowImage(kong, IDC_STATIC_PIC);
	LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	InvalidateRect(hwnd, true);//重画时擦除背景
	UpdateWindow();
	//GetDlgItem(IDC_STATIC_PIC)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_STATIC_PIC)->ShowWindow(SW_SHOW);
	m_leftX = 0;
	m_leftY = 0;
	m_width = 0;
	m_height = 0;
	m_startF = 0;
	m_endF = 0;

	m_posF = 0;	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
	m_slider = 0;
	m_nPos = 0; //获得滑块的当前位置
	m_numFrames = 0;
	m_widFrames = 0;
	m_heigFrames = 0;
	m_Vname.Format(_T(" "));
	//m_Vname = NULL;

	UpdateData(FALSE);
	UpdateData(TRUE);
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, m_numFrames - 1);	// 显示最后一帧
			//m_posF = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES );	//单位为帧数的位置	 //将被下一步解压/获取的帧索引，以0为起点
			//m_slider = 100*m_posF/numFrames;
			//m_nPos = m_ctrlSlider.GetPos(); //获得滑块的当前位置
			//UpdateData(FALSE);			
			capture = NULL;
			videoframe = NULL;			
			break;

		case 1:				// 图片序列
		case 2:				// 图片
			image_l = NULL;		
			break;
	}
	InvalidateRect(hwnd, true);//重画时擦除背景
	UpdateWindow();
}

void Ctest2Dlg::OnBnClickedButton14()					// 开始帧位置
{
	// TODO: 在此添加控件通知处理程序代码
	m_startF = m_posF;
	UpdateData(FALSE);
}

void Ctest2Dlg::OnBnClickedButton15()					// 结束帧位置
{
	// TODO: 在此添加控件通知处理程序代码
	m_endF = m_posF;
	UpdateData(FALSE);
}

void Ctest2Dlg::OnBnClickedButton7()					// 保存信息
{
	// TODO: 在此添加控件通知处理程序代码
	int length = 0;
	//String DocName = "MarkResult.txt";
	//outFile.open(DocName,ios::out);
	//if(!outFile)	{cerr<<"open error!"<<endl;exit(1);}

	CString str;
	GetDlgItem(IDC_EDIT2)->GetWindowText(str);
	int LogoI = _tstoi(str);	// string转为int

	/*UpdateData(TRUE);	
	leftX = m_leftX;
	leftY = m_leftY;
	rightX = m_rightX;
	rightY = m_rightY;
	UpdateData(FALSE);*/

	length = (m_rightX-m_leftX)>(m_rightY-m_leftY) ? (m_rightX-m_leftX) : (m_rightY-m_leftY);
	/*int width = 0, height = 0;
	width = m_rightX-m_leftX;
	height = m_rightY-m_leftY;
	if(width >= height)
		height = width;
	else 
		width = height;*/
	outFile<<setfill('0')<<setw(2)<<LogoI<<" ";
	outFile<<setfill('0')<<setw(5)<<m_startF<<" "<<setfill('0')<<setw(5)<<m_endF<<" ";
	outFile<<setfill('0')<<setw(4)<<m_leftX<<" "<<setfill('0')<<setw(4)<<m_leftY<<" ";
	outFile<<setfill('0')<<setw(3)<<m_width<<" "<<setfill('0')<<setw(3)<<m_height<<"   ";
	outFile<<fname<<ext<<" ";
	outFile<<endl;

	CString strPos; 
	strPos.Format(_T("保存成功！")); 
	AfxMessageBox(strPos);

	//outFile.close();
}


CPoint leftDown, leftUp, rightUp, rightDown;
void Ctest2Dlg::OnLButtonDown(UINT nFlags , CPoint point)	/////////////////////////////////////////////////////////////////////////////////////////////
{
	//Invalidate();UpdateWindow();
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);// 重画，保证鼠标点下时上一个矩形就消失
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
	CClientDC dc(this);
	dc.SetROP2 (R2_NOT);   // 使中间出现的矩形都消失 // 跟背景色相反
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;
	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);	

	::SetCursor(m_HCross);   
	m_Dragging=1;
	m_PointOrigin=point;   
	m_PointOld=point;
	SetCapture();    
	RECT Rect;
	//Rect = CRect(0, 0, PICWIDTH, PICHEIGHT);
	GetClientRect(&Rect);    
	ClientToScreen(&Rect);   
	::ClipCursor (&Rect);    

	leftX = (double)m_PointOrigin.x;
	leftY = (double)m_PointOrigin.y;
	//m_leftX = (int) leftX * widFrames / PICWIDTH;
	//m_leftY = (int) leftY * heigFrames / PICHEIGHT;
	//m_leftX = (int) (leftX * widFrames) / (PICWIDTH * rate);
	//m_leftY = (int) (leftY * heigFrames) / (PICHEIGHT * rate);
	m_leftX = (int) leftX * widFrames / vertWIDTH;
	m_leftY = (int) leftY * heigFrames / vertHEIGHT;
	UpdateData(FALSE);
	CDialogEx::OnLButtonDown(nFlags, point);
}

//然后，使用OnMouseMove（）函数获取鼠标在Picture控件中的坐标：
void Ctest2Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
//CRect rect_ctr;
//(this->GetDlgItem(IDC_STATIC))->GetWindowRect(&rect_ctr);//获取Picture控件相对屏幕左上角的坐标，存储到rect_ctr中
//
//ScreenToClient(rect_ctr);//获取Picture控件相对对话框客户区左上角的坐标
//point.x -=  rect_ctr.left;//point获取的是鼠标相对对话框客户区左上角的坐标，减去rect_ctr.left和
//point.y -=  rect_ctr.top;//rect_ctr.top后，即为鼠标相对Picture控件左上角的坐标
//
////获取状态栏上的文字，并将鼠标的坐标显示在状态栏上
//char	szString[100]={0};
//char	szText[512];
//
//HWND hWnd = ::GetActiveWindow();
//::GetWindowText(hWnd,(LPWSTR)szText,200);
//memset(szText+strlen(szText),' ',20);
//
////如果鼠标位于Picture控件之外，在对话框状态栏上不显示其坐标
//if( point.x>(rect_ctr.right-rect_ctr.left) || point.y>(rect_ctr.bottom-rect_ctr.top )|| point.x<0 || point.y<0 )
//	;//sprintf(szString,"(%4i,%4i)",point.x,point.y);
//else
//	sprintf(szString,"(%4i,%4i)",point.x,point.y);
//	//将鼠标坐标显示到对话框状态栏上
//	
//	/*m_pt = point;
//	m_leftX = m_pt.x;
//	m_leftY = m_pt.y;
//	UpdateData(FALSE);*/
//
//	strcpy(szText+20,szString);
//	::SetWindowText(hWnd,(LPWSTR)szText);

	if(m_Dragging)
	{
		CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
		CClientDC dc(this);
		CPen *pOldPen=dc.SelectObject(&pen);
		dc.SetROP2 (R2_NOT);   // 使中间出现的矩形都消失 // 跟背景色相反

		//if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框			
		//	m_PointOld = point;
		//	int width, height, mend;		
		//	width = m_PointOld.x - m_PointOrigin.x;
		//	height = m_PointOld.y - m_PointOrigin.y;
		//	if(width > height){
		//		//mend = (width - height)/2;
		//		//m_PointOrigin.y -= mend;
		//		mend = width - height;
		//		m_PointOld.y += mend;
		//	}
		//	else{
		//		//mend = (height - width)/2;
		//		//m_PointOrigin.x -= mend;
		//		mend = height - width;
		//		m_PointOld.x += mend;
		//	}
		//}
		dc.Rectangle(m_PointOrigin.x,m_PointOrigin.y,m_PointOld.x,m_PointOld.y);// 作用是使上面OnMouseMove画的正方形消失
		
		m_PointOld = point;
		
		if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框			
			m_PointOld = point;
			int width, height;
			int mend;		
			width = m_PointOld.x - m_PointOrigin.x;
			height = m_PointOld.y - m_PointOrigin.y;
			if(width > height){
				mend = width - height;
				m_PointOld.y += mend;
			}
			else{
				mend = height - width;
				m_PointOld.x += mend;
			}
		}
		dc.Rectangle(m_PointOrigin.x,m_PointOrigin.y,m_PointOld.x,m_PointOld.y);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void Ctest2Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
 
	if(m_Dragging){
		m_Dragging=0;
		::ReleaseCapture ();  
		::ClipCursor (NULL);  
		dc.SetROP2 (R2_NOT);	//使最后的矩形消失 // 跟背景色相反 // 若无此句，整个矩形框内变白
		//dc.SetROP2 (R2_NOTCOPYPEN);	 //不会使最后的矩形消失 // 故边线和pen相反色，矩形内跟背景相反色
		//dc.Rectangle(m_PointOrigin.x,m_PointOrigin.y,m_PointOld.x,m_PointOld.y); // 虽然会使上一个矩形消失，下一条命令画矩形，但改矩形整个框内都使颜色相反
		m_PointOld=point;
		if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框			
			m_PointOld = point;
			int width, height;
			int mend;		
			width = m_PointOld.x - m_PointOrigin.x;
			height = m_PointOld.y - m_PointOrigin.y;
			if(width > height){
				mend = width - height;
				m_PointOld.y += mend;
			}
			else{
				mend = height - width;
				m_PointOld.x += mend;
			}			
		}
		dc.Rectangle(m_PointOrigin.x,m_PointOrigin.y,m_PointOld.x,m_PointOld.y);	// 作用是使上面OnMouseMove画的正方形消失
	}
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;
	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);	

	rightX = (double)m_PointOld.x;
	rightY = (double)m_PointOld.y;
	//m_rightX = (int) (rightX * widFrames) / (PICWIDTH * rate);
	//m_rightY = (int) (rightY * heigFrames) / (PICHEIGHT * rate);
	m_rightX = (int) rightX * widFrames / vertWIDTH;
	m_rightY = (int) rightY * heigFrames / vertHEIGHT;
	m_width = m_rightX - m_leftX;
	m_height = m_rightY - m_leftY;
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框	
		if(m_width >= m_height)
			m_height = m_width;
		else 
			m_width = m_height;
	}
	UpdateData(FALSE);
	
	CDialogEx::OnLButtonUp(nFlags, point);
}

BOOL Ctest2Dlg::PreTranslateMessage(MSG* pMsg)  
{
	if (GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd() == pMsg->hwnd && pMsg->message == WM_LBUTTONDOWN)
	{
		OnLButtonDown(MK_LBUTTON, pMsg->pt);    //在此传递点击部位在对话框中的坐标	
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//BOOL Ctest2Dlg::PreTranslateMessage(MSG* pMsg)  
//{
//	if (GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd() == pMsg->hwnd && pMsg->message == WM_MOUSEMOVE)
//	{
//		OnMouseMove(MK_MBUTTON, pMsg->pt); //在此传递点击部位在对话框中的坐标			
//		return TRUE;
//	}
//	return CDialog::PreTranslateMessage(pMsg);
//}


void Ctest2Dlg::OnBnClickedButton8()					// 按钮：上
{
	// TODO: 在此添加控件通知处理程序代码
	//Invalidate();UpdateWindow();
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;

	//CClientDC dc1(this);
	//dc1.SetROP2 (R2_NOT);   // 使中间出现的矩形都消失 // 跟背景色相反
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
	dc.SetROP2 (R2_NOT);	//使最后的矩形消失 // 跟背景色相反

	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);				// 目的：擦除上一个矩形
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);

	CString str;	
	GetDlgItem(IDC_EDIT16)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int stewstep = 5;	//偏移步长默认为5
	if(intStr)
		stewstep = intStr;
	//m_skewstep
	leftUp.x = m_PointOrigin.x;
	leftUp.y = m_PointOrigin.y - stewstep;
	leftDown.x = m_PointOrigin.x;
	leftDown.y = m_PointOld.y - stewstep;
	rightUp.x = m_PointOld.x;
	rightUp.y = m_PointOrigin.y - stewstep;
	rightDown.x = m_PointOld.x;
	rightDown.y = m_PointOld.y - stewstep;

	dc.MoveTo (leftUp);	dc.LineTo (leftDown);
	dc.MoveTo (leftDown); dc.LineTo (rightDown);
	dc.MoveTo (rightDown); dc.LineTo (rightUp);
	dc.MoveTo (rightUp); dc.LineTo (leftUp);	
	//dc.SelectObject(&pOldPen);		//恢复旧的画笔

	m_PointOrigin = leftUp;
	m_PointOld = rightDown;
	leftX = leftUp.x;
	leftY = leftUp.y;
	rightX = rightDown.x;
	rightY = rightDown.y;
	//m_leftX = (int) leftX * widFrames / PICWIDTH;
	//m_leftY = (int) leftY * heigFrames / PICHEIGHT;
	//m_rightX = (int) rightX * widFrames / PICWIDTH;
	//m_rightY = (int) rightY * heigFrames / PICHEIGHT;
	//m_leftX = (int) (leftX * widFrames) / (PICWIDTH * rate);
	//m_leftY = (int) (leftY * heigFrames) / (PICHEIGHT * rate);
	//m_rightX = (int) (rightX * widFrames) / (PICWIDTH * rate);
	//m_rightY = (int) (rightY * heigFrames) / (PICHEIGHT * rate);
	m_leftX = (int) leftX * widFrames / vertWIDTH;
	m_leftY = (int) leftY * heigFrames / vertHEIGHT;
	m_rightX = (int) rightX * widFrames / vertWIDTH;
	m_rightY = (int) rightY * heigFrames / vertHEIGHT;
	m_width = m_rightX - m_leftX;
	m_height = m_rightY - m_leftY;
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框	
		if(m_width >= m_height)
			m_height = m_width;
		else 
			m_width = m_height;
	}
	UpdateData(FALSE);
}

void Ctest2Dlg::OnBnClickedButton11()					// 按钮：下
{
	// TODO: 在此添加控件通知处理程序代码
	//Invalidate();UpdateWindow();
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
	dc.SetROP2 (R2_NOT);	//使最后的矩形消失 // 跟背景色相反

	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);		// 目的：擦除上一个矩形
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);

	CString str;	
	GetDlgItem(IDC_EDIT16)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int stewstep = 5;	//偏移步长默认为5
	if(intStr)
		stewstep = intStr;
	//m_skewstep
	leftUp.x = m_PointOrigin.x;
	leftUp.y = m_PointOrigin.y + stewstep;
	leftDown.x = m_PointOrigin.x;
	leftDown.y = m_PointOld.y + stewstep;
	rightUp.x = m_PointOld.x;
	rightUp.y = m_PointOrigin.y + stewstep;
	rightDown.x = m_PointOld.x;
	rightDown.y = m_PointOld.y + stewstep;

	dc.MoveTo (leftUp);dc.LineTo (leftDown);
	dc.MoveTo (leftDown);dc.LineTo (rightDown);
	dc.MoveTo (rightDown);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (leftUp);	

	m_PointOrigin = leftUp;
	m_PointOld = rightDown;
	leftX = leftUp.x;
	leftY = leftUp.y;
	rightX = rightDown.x;
	rightY = rightDown.y;
	m_leftX = (int) leftX * widFrames / vertWIDTH;
	m_leftY = (int) leftY * heigFrames / vertHEIGHT;
	m_rightX = (int) rightX * widFrames / vertWIDTH;
	m_rightY = (int) rightY * heigFrames / vertHEIGHT;
	m_width = m_rightX - m_leftX;
	m_height = m_rightY - m_leftY;
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框	
		if(m_width >= m_height)
			m_height = m_width;
		else 
			m_width = m_height;
	}
	UpdateData(FALSE);
}

void Ctest2Dlg::OnBnClickedButton9()					// 按钮：左
{
	// TODO: 在此添加控件通知处理程序代码
	//Invalidate();UpdateWindow();
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
	dc.SetROP2 (R2_NOT);	//使最后的矩形消失 // 跟背景色相反

	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);		// 目的：擦除上一个矩形
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);

	CString str;	
	GetDlgItem(IDC_EDIT16)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int stewstep = 5;	//偏移步长默认为5
	if(intStr)
		stewstep = intStr;
	//m_skewstep
	leftUp.x = m_PointOrigin.x - stewstep;
	leftUp.y = m_PointOrigin.y;
	leftDown.x = m_PointOrigin.x - stewstep;
	leftDown.y = m_PointOld.y;
	rightUp.x = m_PointOld.x - stewstep;
	rightUp.y = m_PointOrigin.y;
	rightDown.x = m_PointOld.x - stewstep;
	rightDown.y = m_PointOld.y;

	dc.MoveTo (leftUp);dc.LineTo (leftDown);
	dc.MoveTo (leftDown);dc.LineTo (rightDown);
	dc.MoveTo (rightDown);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (leftUp);	

	m_PointOrigin = leftUp;
	m_PointOld = rightDown;
	leftX = leftUp.x;
	leftY = leftUp.y;
	rightX = rightDown.x;
	rightY = rightDown.y;
	m_leftX = (int) leftX * widFrames / vertWIDTH;
	m_leftY = (int) leftY * heigFrames / vertHEIGHT;
	m_rightX = (int) rightX * widFrames / vertWIDTH;
	m_rightY = (int) rightY * heigFrames / vertHEIGHT;
	m_width = m_rightX - m_leftX;
	m_height = m_rightY - m_leftY;
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框	
		if(m_width >= m_height)
			m_height = m_width;
		else 
			m_width = m_height;
	}
	UpdateData(FALSE);
	
}

void Ctest2Dlg::OnBnClickedButton10()					// 按钮：右
{
	// TODO: 在此添加控件通知处理程序代码
	//Invalidate();UpdateWindow();
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
	dc.SetROP2 (R2_NOT);	//使最后的矩形消失 // 跟背景色相反

	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);		// 目的：擦除上一个矩形
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);

	CString str;	
	GetDlgItem(IDC_EDIT16)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int stewstep = 5;	//偏移步长默认为5
	if(intStr)
		stewstep = intStr;
	//m_skewstep
	leftUp.x = m_PointOrigin.x + stewstep;
	leftUp.y = m_PointOrigin.y;
	leftDown.x = m_PointOrigin.x + stewstep;
	leftDown.y = m_PointOld.y;
	rightUp.x = m_PointOld.x + stewstep;
	rightUp.y = m_PointOrigin.y;
	rightDown.x = m_PointOld.x + stewstep;
	rightDown.y = m_PointOld.y;

	dc.MoveTo (leftUp);dc.LineTo (leftDown);
	dc.MoveTo (leftDown);dc.LineTo (rightDown);
	dc.MoveTo (rightDown);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (leftUp);	

	m_PointOrigin = leftUp;
	m_PointOld = rightDown;
	leftX = leftUp.x;
	leftY = leftUp.y;
	rightX = rightDown.x;
	rightY = rightDown.y;
	m_leftX = (int) leftX * widFrames / vertWIDTH;
	m_leftY = (int) leftY * heigFrames / vertHEIGHT;
	m_rightX = (int) rightX * widFrames / vertWIDTH;
	m_rightY = (int) rightY * heigFrames / vertHEIGHT;
	m_width = m_rightX - m_leftX;
	m_height = m_rightY - m_leftY;
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框	
		if(m_width >= m_height)
			m_height = m_width;
		else 
			m_width = m_height;
	}
	UpdateData(FALSE);
}


void Ctest2Dlg::OnBnClickedEnlargeWinsize()				// 按钮：扩大
{
	// TODO: 在此添加控件通知处理程序代码
	//Invalidate();UpdateWindow();
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
	dc.SetROP2 (R2_NOT);	//使最后的矩形消失 // 跟背景色相反

	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);		// 目的：擦除上一个矩形
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);

	CString str;	
	GetDlgItem(IDC_EDIT15)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int zoomstep = 5;	//缩放步长默认为5
	if(intStr)
		zoomstep = intStr;
	//m_skewstep

	if ( (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5)) && (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK6)) ){		// "调宽"和"调高"都勾选
		leftUp.x = m_PointOrigin.x - zoomstep;
		leftDown.x = m_PointOrigin.x - zoomstep;
		leftUp.y = m_PointOrigin.y - zoomstep;
		rightUp.y = m_PointOrigin.y - zoomstep;
		rightUp.x = m_PointOld.x + zoomstep;
		rightDown.x = m_PointOld.x + zoomstep;
		rightDown.y = m_PointOld.y + zoomstep;
		leftDown.y = m_PointOld.y + zoomstep;
	}
	else if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK5 ) ){		// 勾选"调宽"框	
		leftUp.x = m_PointOrigin.x - zoomstep;
		leftDown.x = m_PointOrigin.x - zoomstep;
		leftUp.y = m_PointOrigin.y;
		rightUp.y = m_PointOrigin.y;
		rightUp.x = m_PointOld.x + zoomstep;
		rightDown.x = m_PointOld.x + zoomstep;
		rightDown.y = m_PointOld.y;
		leftDown.y = m_PointOld.y;
	}
	else if( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK6 ) ){			// 勾选"调高"框	
		leftUp.x = m_PointOrigin.x;
		leftDown.x = m_PointOrigin.x;
		leftUp.y = m_PointOrigin.y - zoomstep;
		rightUp.y = m_PointOrigin.y - zoomstep;
		rightUp.x = m_PointOld.x;
		rightDown.x = m_PointOld.x;
		rightDown.y = m_PointOld.y + zoomstep;
		leftDown.y = m_PointOld.y + zoomstep;
	}
	else{																// 不勾选
		leftUp.x = m_PointOrigin.x - zoomstep;
		leftDown.x = m_PointOrigin.x - zoomstep;
		leftUp.y = m_PointOrigin.y - zoomstep;
		rightUp.y = m_PointOrigin.y - zoomstep;
		rightUp.x = m_PointOld.x + zoomstep;
		rightDown.x = m_PointOld.x + zoomstep;
		rightDown.y = m_PointOld.y + zoomstep;
		leftDown.y = m_PointOld.y + zoomstep;		
	}

	dc.MoveTo (leftUp);dc.LineTo (leftDown);
	dc.MoveTo (leftDown);dc.LineTo (rightDown);
	dc.MoveTo (rightDown);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (leftUp);

	m_PointOrigin = leftUp;
	m_PointOld = rightDown;
	leftX = leftUp.x;
	leftY = leftUp.y;
	rightX = rightDown.x;
	rightY = rightDown.y;
	m_leftX = (int) leftX * widFrames / vertWIDTH;
	m_leftY = (int) leftY * heigFrames / vertHEIGHT;
	m_rightX = (int) rightX * widFrames / vertWIDTH;
	m_rightY = (int) rightY * heigFrames / vertHEIGHT;
	m_width = m_rightX - m_leftX;
	m_height = m_rightY - m_leftY;
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框	
		if(m_width >= m_height)
			m_height = m_width;
		else 
			m_width = m_height;
	}
	UpdateData(FALSE);
}

void Ctest2Dlg::OnBnClickedButton13()					// 按钮：缩小
{
	// TODO: 在此添加控件通知处理程序代码
	//Invalidate();UpdateWindow();
	//LPCRECT  hwnd = (LPCRECT)cvGetWindowHandle(window_nam);
	//InvalidateRect(hwnd, false);//重画时不擦除背景
	//UpdateWindow();
	leftDown.x = m_PointOld.x;
	leftDown.y = m_PointOrigin.y;
	rightUp.x = m_PointOrigin.x;
	rightUp.y = m_PointOld.y;

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));  //红色画笔
	CClientDC dc(this);
	CPen *pOldPen=dc.SelectObject(&pen);
	dc.SetROP2 (R2_NOT);	//使最后的矩形消失 // 跟背景色相反

	dc.MoveTo (m_PointOrigin);dc.LineTo (leftDown);		// 目的：擦除上一个矩形
	dc.MoveTo (leftDown);dc.LineTo (m_PointOld);
	dc.MoveTo (m_PointOld);dc.LineTo (rightUp);
	dc.MoveTo (rightUp);dc.LineTo (m_PointOrigin);

	CString str;	
	GetDlgItem(IDC_EDIT15)->GetWindowText(str);
	int intStr = _tstoi(str);	// string转为int
	int zoomstep = 5;	//缩放步长默认为5
	if(intStr)
		zoomstep = intStr;
	//m_skewstep

	if ( (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5)) && (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK6)) ){		// "调宽"和"调高"都勾选
		leftUp.x = m_PointOrigin.x + zoomstep;
		leftDown.x = m_PointOrigin.x + zoomstep;
		leftUp.y = m_PointOrigin.y + zoomstep;
		rightUp.y = m_PointOrigin.y + zoomstep;
		rightUp.x = m_PointOld.x - zoomstep;
		rightDown.x = m_PointOld.x - zoomstep;
		rightDown.y = m_PointOld.y - zoomstep;
		leftDown.y = m_PointOld.y - zoomstep;
	}
	else if	( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK5 ) ){				// 勾选"调宽"框	
		leftUp.x = m_PointOrigin.x + zoomstep;
		leftDown.x = m_PointOrigin.x + zoomstep;
		leftUp.y = m_PointOrigin.y;
		rightUp.y = m_PointOrigin.y;
		rightUp.x = m_PointOld.x - zoomstep;
		rightDown.x = m_PointOld.x - zoomstep;
		rightDown.y = m_PointOld.y;
		leftDown.y = m_PointOld.y;	
	}
	else if( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK6 ) ){			// 勾选"调高"框				
		leftUp.x = m_PointOrigin.x;
		leftDown.x = m_PointOrigin.x;
		leftUp.y = m_PointOrigin.y + zoomstep;
		rightUp.y = m_PointOrigin.y + zoomstep;
		rightUp.x = m_PointOld.x;
		rightDown.x = m_PointOld.x;
		rightDown.y = m_PointOld.y - zoomstep;
		leftDown.y = m_PointOld.y - zoomstep;
	}
	else{																// 不勾选
		leftUp.x = m_PointOrigin.x + zoomstep;
		leftDown.x = m_PointOrigin.x + zoomstep;
		leftUp.y = m_PointOrigin.y + zoomstep;
		rightUp.y = m_PointOrigin.y + zoomstep;
		rightUp.x = m_PointOld.x - zoomstep;
		rightDown.x = m_PointOld.x - zoomstep;
		rightDown.y = m_PointOld.y - zoomstep;
		leftDown.y = m_PointOld.y - zoomstep;
	}

	if( rightDown.y>leftUp.y || rightDown.y>leftUp.y){
		dc.MoveTo (leftUp);dc.LineTo (leftDown);
		dc.MoveTo (leftDown);dc.LineTo (rightDown);
		dc.MoveTo (rightDown);dc.LineTo (rightUp);
		dc.MoveTo (rightUp);dc.LineTo (leftUp);	

		m_PointOrigin = leftUp;
		m_PointOld = rightDown;
		leftX = leftUp.x;
		leftY = leftUp.y;
		rightX = rightDown.x;
		rightY = rightDown.y;
		m_leftX = (int) leftX * widFrames / vertWIDTH;
		m_leftY = (int) leftY * heigFrames / vertHEIGHT;
		m_rightX = (int) rightX * widFrames / vertWIDTH;
		m_rightY = (int) rightY * heigFrames / vertHEIGHT;
		m_width = m_rightX - m_leftX;
		m_height = m_rightY - m_leftY;
		if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK4 ) ){			// 勾选"正方形"框	
			if(m_width >= m_height)
				m_height = m_width;
			else 
				m_width = m_height;
		}
		UpdateData(FALSE);
	}
	else {
		CString strPos; 
		strPos.Format(_T("警告：已缩至最小！")); 
		AfxMessageBox(strPos);
	}
}

void Ctest2Dlg::OnBnClickedButton6()						// 保存切分后的视频Logo
{
	// TODO: 在此添加控件通知处理程序代码




}

void Ctest2Dlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	
}

void Ctest2Dlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//m_leftX = temPt.x;
	//UpdateData(FALSE);
}

void Ctest2Dlg::OnEnChangeEdit6()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//m_leftY = temPt.y;
	//UpdateData(FALSE);
}

void Ctest2Dlg::OnStnClickedStaticPic()
{
	// TODO: 在此添加控件通知处理程序代码
}

void Ctest2Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnLbnSelchangeList3()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnEnChangeEdit15()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnLbnSelchangeList4()
{
	// TODO: 在此添加控件通知处理程序代码
}


void Ctest2Dlg::OnEnChangeEdit11()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


	void Ctest2Dlg::OnEnChangeEdit10()
	{
		// TODO:  如果该控件是 RICHEDIT 控件，它将不
		// 发送此通知，除非重写 CDialogEx::OnInitDialog()
		// 函数并调用 CRichEditCtrl().SetEventMask()，
		// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

		// TODO:  在此添加控件通知处理程序代码
	}

	void Ctest2Dlg::OnEnChangeEdit14()
	{
		// TODO:  如果该控件是 RICHEDIT 控件，它将不
		// 发送此通知，除非重写 CDialogEx::OnInitDialog()
		// 函数并调用 CRichEditCtrl().SetEventMask()，
		// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

		// TODO:  在此添加控件通知处理程序代码
	}

	void Ctest2Dlg::OnEnChangeEdit16()
	{
		// TODO:  如果该控件是 RICHEDIT 控件，它将不
		// 发送此通知，除非重写 CDialogEx::OnInitDialog()
		// 函数并调用 CRichEditCtrl().SetEventMask()，
		// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

		// TODO:  在此添加控件通知处理程序代码
	}


//	void Ctest2Dlg::OnRadioBtnGroup1Clicked(NMHDR *pNMHDR, LRESULT *pResult)
//	{
//		LPNMBCDROPDOWN pDropDown = reinterpret_cast<LPNMBCDROPDOWN>(pNMHDR);
//		// TODO: 在此添加控件通知处理程序代码
//		*pResult = 0;
//	}


void Ctest2Dlg::OnRadioBtnGroup1Clicked()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch(m_radiobtngroup1)
	{
		case 0:				// 视频
			//AfxMessageBox(_T("1 被选中"));
			break;
		case 1:				// 图片序列
			//AfxMessageBox(_T("2 被选中"));
			break;
		case 2:				// 图片
			//AfxMessageBox(_T("3 被选中"));
			break;
	}
}



void Ctest2Dlg::OnEnChangeEdit12()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
