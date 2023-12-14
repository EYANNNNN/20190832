// COpenCVDlg.cpp: 구현 파일
//

#include "pch.h"
#include "imgProc2_20190832.h"
#include "afxdialogex.h"
#include "COpenCVDlg.h"

#include <opencv2/opencv.hpp>

// COpenCVDlg 대화 상자

IMPLEMENT_DYNAMIC(COpenCVDlg, CDialogEx)

COpenCVDlg::COpenCVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(NCV, pParent)
{

}

COpenCVDlg::~COpenCVDlg()
{
}

void COpenCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COpenCVDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECTFILE, &COpenCVDlg::OnBnClickedButtonFileSelect)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO, &COpenCVDlg::OnBnClickedButtonVideo)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA, &COpenCVDlg::OnBnClickedButtonCamera)
END_MESSAGE_MAP()


// COpenCVDlg 메시지 처리기


void COpenCVDlg::OnBnClickedButtonFileSelect()
{
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"JPG화일(*.jpg)|*.jpg|모든화일(*.*)|*.*");

	if(dlg.DoModal() == IDOK)
	{
		cv::Mat frame;
		frame = cv::imread((LPSTR)(LPCTSTR)dlg.GetPathName(), 1);//문자열을 넘기는 방식의 차이

		cv::imshow("Image", frame);
		cv::waitKey(0);

	}
}


void COpenCVDlg::OnBnClickedButtonVideo()
{
	CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"AVI화일(*.avi)|*.avi|모든화일(*.*)|*.*");

	if (dlg.DoModal() == IDOK)
	{
		cv::VideoCapture cap((LPSTR)(LPCTSTR)dlg.GetPathName());

		cap.open((LPSTR)(LPCTSTR)dlg.GetPathName());

		cv::Mat frame;
		while (1)
		{
			cap.read(frame);//한 프레임 읽기
			if (frame.empty())
				break;

			cv::imshow("Video", frame);   // 한 프레임 출력
			if (cv::waitKey(30) == 'q')
			break;
		}
		cv::destroyWindow("Video");// 창 끄기
		
	}
}


void COpenCVDlg::OnBnClickedButtonCamera()
{
	cv::VideoCapture cap(0);

	cv::Mat frame;

	while (1) {
		cap.read(frame);   // 한 프레임 읽기
		if (frame.empty()) 
			break;

		cv::imshow("Camera", frame);   // 한 프레임 출력
		if (cv::waitKey(30) == 'q') 
			break;
	}
	cv::destroyWindow("Camera");   // 창 끄기
}

