
// test2Dlg.h : 头文件
//

#pragma once
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream> 

using namespace std;  
using namespace cv; 

// Ctest2Dlg 对话框
class Ctest2Dlg : public CDialogEx
{
// 构造
public:
	Ctest2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton6();
private:
	
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton9();
//	int m_LogoI;
	int m_leftX;
	int m_leftY;
	//int m_rightX;
	//int m_rightY;
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	int m_image;
	int ShowImage(IplImage* img, UINT ID);
	CStatic m_ctrlPic;
	afx_msg void OnStnClickedStaticPic();
	CPoint m_pt;
	BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	// “类向导”——“消息”中添加
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	HCURSOR m_HCross;
	int m_Dragging;
	CPoint m_PointOrigin;
	CPoint m_PointOld;
	int m_LogoID;
	CEdit m_Logo;
	afx_msg void OnEnChangeEdit1();
	//void OnTimer(void);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnEnChangeEdit15();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnLbnSelchangeList4();
	int m_numFrames;
	int m_widFrames;
	int m_heigFrames;
	afx_msg void OnEnChangeEdit11();
	int m_posFrames;
	int m_posF;
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	int m_startF;
	int m_endF;
	afx_msg void OnBnClickedEnlargeWinsize();
	afx_msg void OnRadioBtnGroup1Clicked();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);

	//CSliderCtrl m_ctrlSlider;
	CSliderCtrl m_ctrlSlider;
	afx_msg void OnEnChangeEdit14();
	int m_slider;
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton13();
//	int m_skewstep;
	afx_msg void OnEnChangeEdit16();
	int m_rightX;
	int m_rightY;
	int m_width;
	int m_height;
	CString m_Vname;
	int m_nPos;
	int m_radiobtngroup1;
//	afx_msg void OnRadioBtnGroup1Clicked(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnEnChangeEdit12();
};
