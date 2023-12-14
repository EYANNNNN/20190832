
// imgProc2_20190832View.cpp: CimgProc220190832View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imgProc2_20190832.h"
#endif

#include "imgProc2_20190832Doc.h"
#include "imgProc2_20190832View.h"
#include "COpenCVDlg.h"
#include "CAngleinputDialog.h"

#include <vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CimgProc220190832View

IMPLEMENT_DYNCREATE(CimgProc220190832View, CScrollView)

BEGIN_MESSAGE_MAP(CimgProc220190832View, CScrollView)
	ON_COMMAND(ID_PIXEL_ADD, &CimgProc220190832View::OnPixelAdd)
	ON_COMMAND(ID_PIXEL_SUB, &CimgProc220190832View::OnPixelSub)
	ON_COMMAND(ID_PIXEL_MUL, &CimgProc220190832View::OnPixelMul)
	ON_COMMAND(ID_PIXEL_DIV, &CimgProc220190832View::OnPixelDiv)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CimgProc220190832View::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_CONTRAST_STRETCHING, &CimgProc220190832View::OnPixelContrastStretching)
	ON_COMMAND(ID_PIXEL_BINARIZATION, &CimgProc220190832View::OnPixelBinarization)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &CimgProc220190832View::OnPixelTwoImageAdd)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_SUB, &CimgProc220190832View::OnPixelTwoImageSub)
	ON_COMMAND(ID_REGION_SHARPENING, &CimgProc220190832View::OnRegionSharpening)
	ON_COMMAND(ID_REGION_SMOOTHING, &CimgProc220190832View::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_EMBOSSING, &CimgProc220190832View::OnRegionEmbossing)
	ON_COMMAND(ID_REGION_PREWITT, &CimgProc220190832View::OnRegionPrewitt)
	ON_COMMAND(ID_REGION_ROBERTS, &CimgProc220190832View::OnRegionRoberts)
	ON_COMMAND(ID_REGION_SOBEL, &CimgProc220190832View::OnRegionSobel)
	ON_COMMAND(ID_REGION_AVERAGE_FILTERING, &CimgProc220190832View::OnRegionAverageFiltering)
	ON_COMMAND(ID_REGION_MEDIAN_FILTERING, &CimgProc220190832View::OnRegionMedianFiltering)
	ON_COMMAND(ID_MOPOLOGY_COLOR_TO_GRAY, &CimgProc220190832View::OnMopologyColorToGray)
	ON_COMMAND(ID_MOPOLOGY_BINARIZATION, &CimgProc220190832View::OnMopologyBinarization)
	ON_COMMAND(ID_MOPOLOGY_EROSION, &CimgProc220190832View::OnMopologyErosion)
	ON_COMMAND(ID_MOPOLOGY_DILATION, &CimgProc220190832View::OnMopologyDilation)
	ON_COMMAND(ID_MOPOLOGY_OPENING, &CimgProc220190832View::OnMopologyOpening)
	ON_COMMAND(ID_MOPOLOGY_CLOSING, &CimgProc220190832View::OnMopologyClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CimgProc220190832View::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_INTERPOLATION, &CimgProc220190832View::OnGeometryZoominInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CimgProc220190832View::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEANSUB, &CimgProc220190832View::OnGeometryZoomoutMeansub)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVGSAMPLING, &CimgProc220190832View::OnGeometryZoomoutAvgsampling)
	ON_COMMAND(ID_GEOMETRY_ROTATION, &CimgProc220190832View::OnGeometryRotation)
	ON_COMMAND(ID_GEOMETRY_MIRROR, &CimgProc220190832View::OnGeometryMirror)
	ON_COMMAND(ID_GEOMETRY_FLIP, &CimgProc220190832View::OnGeometryFlip)
	ON_COMMAND(ID_GEOMETRY_WARPING, &CimgProc220190832View::OnGeometryWarping)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_AVI_VIEW, &CimgProc220190832View::OnAviView)
	ON_COMMAND(ID_OPENCV_VIEW, &CimgProc220190832View::OnOpencvView)
	ON_COMMAND(ID_GEOMETRY_MORPHING, &CimgProc220190832View::OnGeometryMorphing)
END_MESSAGE_MAP()

// CimgProc220190832View 생성/소멸

CimgProc220190832View::CimgProc220190832View() noexcept
{
	bAviMode = false;

}

CimgProc220190832View::~CimgProc220190832View()
{
}

BOOL CimgProc220190832View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CimgProc220190832View 그리기

void CimgProc220190832View::OnDraw(CDC* pDC)
{
	CimgProc220190832Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (bAviMode)
	{
		//avi화일재생
		LoadAviFile(pDC);
		bAviMode = false;
		return;
	}

	int x, y;
	if (pDoc->InputIMG != NULL) {

		if (pDoc->depth == 1) {

			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x, y, RGB(pDoc->InputIMG[y][x], pDoc->InputIMG[y][x], pDoc->InputIMG[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(x, y, RGB(pDoc->InputIMG[y][3 * x + 0], pDoc->InputIMG[y][3 * x + 1], pDoc->InputIMG[y][3 * x + 2]));
				}
			}
		}
	}
	if (pDoc->ResultIMG != NULL) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(pDoc->imageWidth + 20 + x, y, RGB(pDoc->ResultIMG[y][x], pDoc->ResultIMG[y][x], pDoc->ResultIMG[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(pDoc->imageWidth + 20 + x, y, RGB(pDoc->ResultIMG[y][3 * x + 0], pDoc->ResultIMG[y][3 * x + 1], pDoc->ResultIMG[y][3 * x + 2]));
				}
			}
		}
	}
	
	if (pDoc->InputIMG2 != NULL) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(2 * (pDoc->imageWidth + 20) + x, y, RGB(pDoc->InputIMG2[y][x], pDoc->InputIMG2[y][x], pDoc->InputIMG2[y][x]));

				}
			}
		}
		else {
			for (y = 0; y < pDoc->imageHeight; y++) {
				for (x = 0; x < pDoc->imageWidth; x++) {
					pDC->SetPixel(2 * (pDoc->imageWidth + 20) + x, y, RGB(pDoc->InputIMG2[y][3 * x + 0], pDoc->InputIMG2[y][3 * x + 1], pDoc->InputIMG2[y][3 * x + 2]));
				}
			}
		}
	}
	
	if (pDoc->gResultIMG != NULL) {
		if (pDoc->depth == 1) {
			for (y = 0; y < pDoc->gimageHeight; y++) {
				for (x = 0; x < pDoc->gimageWidth; x++) {
					pDC->SetPixel(x, pDoc->imageHeight + 20 + y, RGB(pDoc->gResultIMG[y][x], pDoc->gResultIMG[y][x], pDoc->gResultIMG[y][x]));
				}
			}
		}
		else {
			for (y = 0; y < pDoc->gimageHeight; y++) {
				for (x = 0; x < pDoc->gimageWidth; x++) {
					pDC->SetPixel(x, pDoc->imageHeight + 20 + y, RGB(pDoc->gResultIMG[y][3 * x + 0], pDoc->gResultIMG[y][3 * x + 1], pDoc->gResultIMG[y][3 * x + 2]));
				}
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		if (pDoc->morphedImg[i] != NULL) {

			for (int y = 0; y < pDoc->imageHeight; y++)       // 모핑 결과 출력 
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + 20 + pDoc->imageWidth, y,
						RGB(pDoc->morphedImg[i][y][x],
							pDoc->morphedImg[i][y][x],
							pDoc->morphedImg[i][y][x]));
		}
	}

}
	

void CimgProc220190832View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 2048;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CimgProc220190832View 진단

#ifdef _DEBUG
void CimgProc220190832View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CimgProc220190832View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CimgProc220190832Doc* CimgProc220190832View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimgProc220190832Doc)));
	return (CimgProc220190832Doc*)m_pDocument;
}
#endif //_DEBUG


// CimgProc220190832View 메시지 처리기


void CimgProc220190832View::OnPixelAdd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CimgProc220190832Doc* pDoc = GetDocument();
	if (pDoc->InputIMG == NULL)return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = pDoc->InputIMG[y][x] + 100; // 밝기 조절
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][x] = value;
			}
			else
			{
				value = pDoc->InputIMG[y][3 * x + 0] + 100; // 밝기 조절
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][3 * x + 0] = value;
				//RED

				value = pDoc->InputIMG[y][3 * x + 1] + 100;
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][3 * x + 1] = value;
				//GREEN

				value = pDoc->InputIMG[y][3 * x + 2] + 100;
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][3 * x + 2] = value;
				//BLUE


			}

		}
	Invalidate(); // 화면 다시 호출

}

   

void CimgProc220190832View::OnPixelSub()
{
	CimgProc220190832Doc* pDoc = GetDocument();
	if (pDoc->InputIMG == NULL)return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				value = pDoc->InputIMG[y][x] - 100; // 밝기 조절
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][x] = value;
			}
			else
			{
				value = pDoc->InputIMG[y][3 * x + 0] - 100; // 밝기 조절
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][3 * x + 0] = value;
				//RED

				value = pDoc->InputIMG[y][3 * x + 1] - 100;
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][3 * x + 1] = value;
				//GREEN

				value = pDoc->InputIMG[y][3 * x + 2] - 100;
				if (value > 255) value = 255;
				else if (value < 0) value = 0;
				pDoc->ResultIMG[y][3 * x + 2] = value;
				//BLUE


			}
		}
			Invalidate();// 화면에 다시 호출
}


void CimgProc220190832View::OnPixelMul()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CimgProc220190832Doc* pDoc = GetDocument();

	if (pDoc->InputIMG == NULL) return;
	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{

				value = pDoc->InputIMG[y][x] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultIMG[y][x] = value;
			}
			else
			{
				value = pDoc->InputIMG[y][3 * x + 0] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultIMG[y][3 * x + 0] = value;

				value = pDoc->InputIMG[y][3 * x + 1] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultIMG[y][3 * x + 1] = value;

				value = pDoc->InputIMG[y][3 * x + 2] * 1.5;
				if (value > 255)value = 255;
				else if (value < 0) value = 0;

				pDoc->ResultIMG[y][3 * x + 2] = value;
			}
		}
	Invalidate();
}

	void CimgProc220190832View::OnPixelDiv()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		if (pDoc->InputIMG == NULL) return;
		int x, y, value;
		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				if (pDoc->depth == 1)
				{

					value = pDoc->InputIMG[y][x] / 1.5;
					if (value > 255)value = 255;
					else if (value < 0) value = 0;

					pDoc->ResultIMG[y][x] = value;
				}
				else
				{
					value = pDoc->InputIMG[y][3 * x + 0] / 1.5;
					if (value > 255)value = 255;
					else if (value < 0) value = 0;

					pDoc->ResultIMG[y][3 * x + 0] = value;

					value = pDoc->InputIMG[y][3 * x + 1] / 1.5;
					if (value > 255)value = 255;
					else if (value < 0) value = 0;

					pDoc->ResultIMG[y][3 * x + 1] = value;

					value = pDoc->InputIMG[y][3 * x + 2] / 1.5;
					if (value > 255)value = 255;
					else if (value < 0) value = 0;

					pDoc->ResultIMG[y][3 * x + 2] = value;
				}
			}
		Invalidate();
		
		
	}

	void CimgProc220190832View::OnPixelHistoEq()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
		int x, y, i, k;
		int acc_hist;
		float N = 256 * 256;
		int hist[256], sum[256];

		for (i = 0; i < 256; i++) hist[i] = 0;

		//히스토그램 구하기
		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				k = pDoc->InputIMG[y][x];
					hist[k]++;
			}
		//누적분포
		acc_hist = 0;
		for (i = 0; i < 256; i++)
		{
			acc_hist += hist[i];
			sum[i] = acc_hist;
		}
		//픽셀 밝기 변환
		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x<pDoc->imageWidth; x++)
			{
				k = pDoc->InputIMG[y][x];
				pDoc->ResultIMG[y][x] = sum[k] / N * 255;
			}
		Invalidate();

	}


	void CimgProc220190832View::OnPixelContrastStretching()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		int x, y;
		int minVal = 255;
		int maxVal = 0;

		// 입력 이미지에서 최소값과 최대값을 찾습니다.
		for (y = 0; y < pDoc->imageHeight; y++) {
			for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
				int pixelValue = pDoc->InputIMG[y][x];
				if (pixelValue < minVal) {
					minVal = pixelValue;
				}
				if (pixelValue > maxVal) {
					maxVal = pixelValue;
				}
			}
		}

		// 명암 대비 스트레칭을 적용하여 결과 이미지를 생성합니다.
		for (y = 0; y < pDoc->imageHeight; y++) {
			for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
				int pixelValue = pDoc->InputIMG[y][x];
				int stretchedValue = 255 * (pixelValue - minVal) / (maxVal - minVal);

				// 범위를 벗어난 값은 조정합니다.
				if (stretchedValue > 255) {
					stretchedValue = 255;
				}
				else if (stretchedValue < 0) {
					stretchedValue = 0;
				}
				pDoc->ResultIMG[y][x] = stretchedValue;
			}
		}
		Invalidate();
	}

	//이진화 처리
	void CimgProc220190832View::OnPixelBinarization()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
		int x, y, value;
		int threshold = 128;// 검은색이 50개 있고 흰색이 205개가 있으니깐 밝게 나온다.


		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++)
			{
				value = pDoc->InputIMG[y][x];
				if (value > threshold) pDoc->ResultIMG[y][x] = 255;//처리전밝기값:pDoc->Inputimg[y][x] 255흰색 0검은색
				else                                   pDoc->ResultIMG[y][x] = 0;
			}
		Invalidate();
	}


/*	void CimgProc220190832View::OnPixelTwoImageAdd()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
		CFileDialog dlg(TRUE);//파일 선택 대화상자 객체 선언
		                      // TRUE: 파일 열기
		                      // FALSE: 파일 저장

		if (dlg.DoModal() == IDOK)
		{
			CFile file;
			file.Open(dlg.GetPathName(), CFile::modeRead); //화일 열기
			file.Read(pDoc->InputIMG2, 256 * 256);
			file.Close();


			
			Invalidate();


		}
	}
*/





	void CimgProc220190832View::OnPixelTwoImageAdd()
	{

	
		CimgProc220190832Doc* pDoc = GetDocument();
		CFileDialog dlg(TRUE);

		LoadTwoImage();
		

		int x,y,value;


		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				value = 0.7 * pDoc->InputIMG[y][x] + 0.3 * pDoc->InputIMG2[y][x];
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->ResultIMG[y][x] = value;
			}
		Invalidate();
	}

	void  CimgProc220190832View::LoadTwoImage()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
		CFileDialog dlg(TRUE);

		if (dlg.DoModal() == IDOK) //파일 선택 대화 상자 실행
		{
			CFile file;
			file.Open(dlg.GetPathName(), CFile::modeRead); //파일열기
			CArchive ar(&file, CArchive::load);
			pDoc->LoadSecondImageFile(ar);
			file.Close(); //파일 닫기
		
			int x, y,value;

			for(y=0; y<pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
				{
					if (pDoc->depth == 1)
					{
						value = 0.5 * pDoc->InputIMG[y][x] + 0.5 * pDoc->InputIMG2[y][x];
						if (value > 255)value = 255;
						else if (value < 0)value = 0;
						pDoc->ResultIMG[y][x] = value;
					}
					else 
					{
						value = 0.5 * pDoc->InputIMG[y][3 * x + 0] + 0.5 * pDoc->InputIMG2[y][3 * x + 0];
						if (value > 255)value = 255;
						else if (value < 0)value = 0;
						pDoc->ResultIMG[y][3*x+0] = value;

						value = 0.5 * pDoc->InputIMG[y][3 * x + 1] + 0.5 * pDoc->InputIMG2[y][3 * x + 1];
						if (value > 255)value = 255;
						else if (value < 0)value = 0;
						pDoc->ResultIMG[y][3 * x + 1] = value;

						value = 0.5 * pDoc->InputIMG[y][3 * x + 2] + 0.5 * pDoc->InputIMG2[y][3 * x + 2];
						if (value > 255)value = 255;
						else if (value < 0)value = 0;
						pDoc->ResultIMG[y][3 * x + 2] = value;
					}
				}
				
			Invalidate();//화면을 다시 호출
		}
		
	}

	void CimgProc220190832View::OnPixelTwoImageSub()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		LoadTwoImage();

		int x, y, value;


		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				value = pDoc->InputIMG[y][x] - pDoc->InputIMG2[y][x];
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				if (value > 64) value = 255;
				else            value = 0;
				pDoc->ResultIMG[y][x] = value;

				for (y = 0; y < pDoc->imageHeight; y++)
					for (x = 0; x < pDoc->imageWidth; x++)
					{
						if (pDoc->depth == 1)
						{
							value = 0.5 * pDoc->InputIMG[y][x] - 0.5 * pDoc->InputIMG2[y][x];
							if (value > 255)value = 255;
							else if (value < 0)value = 0;
							pDoc->ResultIMG[y][x] = value;
						}
						else
						{
							value = 0.5 * pDoc->InputIMG[y][3 * x + 0] - 0.5 * pDoc->InputIMG2[y][3 * x + 0];
							if (value > 255)value = 255;
							else if (value < 0)value = 0;
							pDoc->ResultIMG[y][3 * x + 0] = value;

							value = 0.5 * pDoc->InputIMG[y][3 * x + 1] - 0.5 * pDoc->InputIMG2[y][3 * x + 1];
							if (value > 255)value = 255;
							else if (value < 0)value = 0;
							pDoc->ResultIMG[y][3 * x + 1] = value;

							value = 0.5 * pDoc->InputIMG[y][3 * x + 2] - 0.5 * pDoc->InputIMG2[y][3 * x + 2];
							if (value > 255)value = 255;
							else if (value < 0)value = 0;
							pDoc->ResultIMG[y][3 * x + 2] = value;
						}
					}
				Invalidate();
			}
	}
		
	


	void CimgProc220190832View::OnRegionSharpening()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		float kernel[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0} };

		convolve(pDoc->InputIMG, pDoc->ResultIMG, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);// 마스크의 형태

		Invalidate();
	}


	void CimgProc220190832View::convolve(unsigned char** inImg, unsigned char** outImg, int cols, int rows, float mask[][3], int bias, int depth)
	{
		int x, y;
		int i, j;
		int sum,rsum,gsum,bsum;

		for (y = 1; y < rows-1; y++)
			for (x = 1; x < cols-1; x++)
			{
				if (depth == 1)
				{
					sum = 0;
					for (j = 0; j < 3; j++)
						for (i = 0; i < 3; i++)
						{
							sum += (inImg[y + j - 1][x + i - 1] * mask[j][i]);
						}
					sum += bias;
					if (sum > 255) sum = 255;
					else if (sum < 0) sum = 0;
					outImg[y][x] = sum;
				}
				else
				{
					rsum = 0; gsum = 0; bsum = 0;
					for (j = 0; j < 3; j++)
						for (i = 0; i < 3; i++)
						{
							rsum += (inImg[y + j - 1][3 * (x + i - 1) + 0] * mask[j][i]);
							gsum += (inImg[y + j - 1][3 * (x + i - 1) + 1] * mask[j][i]);
							bsum += (inImg[y + j - 1][3 * (x + i - 1) + 2] * mask[j][i]);
						}
					rsum += bias;
					gsum += bias;
					bsum += bias;


					if (rsum > 255) rsum = 255;
					else if (rsum < 0) rsum = 0;
					if (gsum > 255) gsum = 255;
					else if (gsum < 0) gsum = 0;
					if (bsum > 255) bsum = 255;
					else if (bsum < 0) bsum = 0;

					outImg[y][3 * x + 0] = rsum;
					outImg[y][3 * x + 1] = gsum;
					outImg[y][3 * x + 2] = bsum;

				}
			}
	}


	void CimgProc220190832View::OnRegionSmoothing()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		float kernel[3][3] = { {1 / 9.0f,1 / 9.0f},{1 / 9.0f,1 / 9.0f},{1 / 9.0f,1 / 9.0f} };

		convolve(pDoc->InputIMG, pDoc->ResultIMG, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

		Invalidate();
		
	}


	void CimgProc220190832View::OnRegionEmbossing()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
	
		float kernel[3][3] = { { -1, -1, 0} ,{ -1, 0, 1},{0, 1, 1} };

		convolve(pDoc->InputIMG, pDoc->ResultIMG, pDoc->imageWidth, pDoc->imageHeight, kernel, 128, pDoc->depth);
		
		
		Invalidate();
		
	}


	void CimgProc220190832View::OnRegionPrewitt()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		float kernel_h[3][3] = { { -1, -1, -1},
			                    { 0, 0, 0},
			                    { 1, 1, 1}};
		float kernel_v[3][3] = { { -1, 0, 1},
						         { -1, 0, 1},
						         { -1, 0, 1} };
		int x, y,i;
		int value,rvalue,gvalue,bvalue;

		unsigned char** Er, ** Ec;

		//메모리할당
		Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
		Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

		for (i = 0; i < pDoc->imageHeight; i++)
		{
			Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
			Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		}

		convolve(pDoc->InputIMG, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_h, 128, pDoc->depth);
		convolve(pDoc->InputIMG, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_v, 128, pDoc->depth);

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
					if (value > 255) value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultIMG[y][x] = value;
				}
				else 
				{
					rvalue = sqrt((Er[y][3*x+0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));
					gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
					bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

					value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);
					if (value > 255) value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultIMG[y][3 * x + 0] = value;
					pDoc->ResultIMG[y][3 * x + 1] = value;
					pDoc->ResultIMG[y][3 * x + 2] = value;

				}
			}
		//메모리 삭제 free 함수 이용.
		for (i = 0; i < pDoc->imageHeight; i++)
		{
			free(Er[i]);
			free(Ec[i]);
		}
		free(Er);
		free(Ec);
	
		Invalidate();//화면 다시 호출

	}


	void CimgProc220190832View::OnRegionRoberts()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		float kernel_h[3][3] = { { -1, 0, 0},
								{ 0, 1, 0},
								{ 0, 0, 0} };
		float kernel_v[3][3] = { { 0, 0, -1},
								 { 0, 1, 0},
								 { 0, 0, 0} };
		int x, y, i;
		int value, rvalue, gvalue, bvalue;

		unsigned char** Er, ** Ec;

		//메모리할당
		Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
		Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

		for (i = 0; i < pDoc->imageHeight; i++)
		{
			Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
			Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		}

		convolve(pDoc->InputIMG, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_h, 128, pDoc->depth);
		convolve(pDoc->InputIMG, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_v, 128, pDoc->depth);

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
					if (value > 255) value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultIMG[y][x] = value;
				}
				else
				{
					rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));
					gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
					bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

					value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);
					if (value > 255) value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultIMG[y][3 * x + 0] = value;
					pDoc->ResultIMG[y][3 * x + 1] = value;
					pDoc->ResultIMG[y][3 * x + 2] = value;

				}
			}
		//메모리 삭제 free 함수 이용.
		for (i = 0; i < pDoc->imageHeight; i++)
		{
			free(Er[i]);
			free(Ec[i]);
		}
		free(Er);
		free(Ec);

		Invalidate();
	}


	void CimgProc220190832View::OnRegionSobel()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		float kernel_h[3][3] = { { -1, -2, -1},
								{ 0, 0, 0},
								{ 1, 2, 1} };
		float kernel_v[3][3] = { { -1, 0, 1},
								 { -2, 0, 2},
								 { -1, 0, 1} };
		int x, y, i;
		int value, rvalue, gvalue, bvalue;

		unsigned char** Er, ** Ec;

		//메모리할당
		Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
		Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

		for (i = 0; i < pDoc->imageHeight; i++)
		{
			Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
			Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		}

		convolve(pDoc->InputIMG, Er, pDoc->imageWidth, pDoc->imageHeight, kernel_h, 128, pDoc->depth);
		convolve(pDoc->InputIMG, Ec, pDoc->imageWidth, pDoc->imageHeight, kernel_v, 128, pDoc->depth);

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
					if (value > 255) value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultIMG[y][x] = value;
				}
				else
				{
					rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));
					gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
					bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

					value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);
					if (value > 255) value = 255;
					else if (value < 0) value = 0;
					pDoc->ResultIMG[y][3 * x + 0] = value;
					pDoc->ResultIMG[y][3 * x + 1] = value;
					pDoc->ResultIMG[y][3 * x + 2] = value;

				}
			}
		//메모리 삭제 free 함수 이용.
		for (i = 0; i < pDoc->imageHeight; i++)
		{
			free(Er[i]);
			free(Ec[i]);
		}
		free(Er);
		free(Ec);

		Invalidate();

	}


	void CimgProc220190832View::OnRegionAverageFiltering()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
		int x, y, i, j;
		int xpos, ypos;
		int sum,rsum,gsum,bsum;
		int pixelcount;

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++)//중심좌표
			{
				sum = 0; pixelcount = 0;   rsum = gsum = bsum = 0;
				for (j = -2; j <= 2; j++)//5X5		3X3-> j=-1 j<=1
					for (i = -2; i <= 2; i++)
					{
						xpos = x + i;
						ypos = y + j;
						if (xpos >= 0 && ypos <= pDoc->imageWidth - 1 && ypos >= 0 && ypos <= pDoc->imageHeight - 1)
						{
							if(pDoc->depth==1)//컬러
							sum += pDoc->InputIMG[y + j][x + i];
							else {
								rsum += pDoc->InputIMG[y + j][3 * (x + i) + 0];
								gsum += pDoc->InputIMG[y + j][3 * (x + i) + 1];
								bsum += pDoc->InputIMG[y + j][3 * (x + i) + 2];
							     }
							pixelcount++;
						}
					}
				if(pDoc->depth==1)//컬러
				pDoc->ResultIMG[y][x] = sum / pixelcount;
				else
				{
				pDoc->ResultIMG[y][3 * x + 0] = rsum / pixelcount;
				pDoc->ResultIMG[y][3 * x + 1] = gsum / pixelcount;
				pDoc->ResultIMG[y][3 * x + 2] = bsum / pixelcount;
				}
			}
		Invalidate();
	}

	void CimgProc220190832View::OnRegionMedianFiltering()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
		int x, y, i, j;
		int n[9], temp;


		for (y = 1; y < pDoc->imageHeight - 1; y++)
			for (x = 1; x < pDoc->imageWidth - 1; x++)
			{
				if (pDoc->depth == 1) {
					n[0] = pDoc->InputIMG[y - 1][x - 1];
					n[1] = pDoc->InputIMG[y - 1][x - 0];
					n[2] = pDoc->InputIMG[y - 1][x + 1];
					n[3] = pDoc->InputIMG[y - 0][x - 1];
					n[4] = pDoc->InputIMG[y - 0][x - 0];
					n[5] = pDoc->InputIMG[y - 0][x + 1];
					n[6] = pDoc->InputIMG[y + 1][x - 1];
					n[7] = pDoc->InputIMG[y + 1][x - 0];
					n[8] = pDoc->InputIMG[y + 1][x + 1];

					//sorting
					for (i = 8; i > 0; i--)
						for (j = 0; j < i; j++)
							if (n[j] > n[j + 1])
							{
								temp = n[j + 1];
								n[j + 1] = n[j];
								n[j] = temp;
							}
					pDoc->ResultIMG[y][x] = n[4];
				}
				else {
					n[0] = pDoc->InputIMG[y - 1][3 * (x - 1) + 0];
					n[1] = pDoc->InputIMG[y - 1][3 * (x - 0) + 0];
					n[2] = pDoc->InputIMG[y - 1][3 * (x + 1) + 0];
					n[3] = pDoc->InputIMG[y - 0][3 * (x - 1) + 0];
					n[4] = pDoc->InputIMG[y - 0][3 * (x - 0) + 0];
					n[5] = pDoc->InputIMG[y - 0][3 * (x + 1) + 0];
					n[6] = pDoc->InputIMG[y + 1][3 * (x - 1) + 0];
					n[7] = pDoc->InputIMG[y + 1][3 * (x - 0) + 0];
					n[8] = pDoc->InputIMG[y + 1][3 * (x + 1) + 0];

					//sorting
					for (i = 8; i > 0; i--)
						for (j = 0; j < i; j++)
							if (n[j] > n[j + 1])
							{
								temp = n[j + 1];
								n[j + 1] = n[j];
								n[j] = temp;
							}
					pDoc->ResultIMG[y][3 * x + 0] = n[4];

					n[0] = pDoc->InputIMG[y - 1][3 * (x - 1) + 1];
					n[1] = pDoc->InputIMG[y - 1][3 * (x - 0) + 1];
					n[2] = pDoc->InputIMG[y - 1][3 * (x + 1) + 1];
					n[3] = pDoc->InputIMG[y - 1][3 * (x + 1) + 1];
					n[4] = pDoc->InputIMG[y - 1][3 * (x + 1) + 1];
					n[5] = pDoc->InputIMG[y - 1][3 * (x + 1) + 1];
					n[6] = pDoc->InputIMG[y - 1][3 * (x + 1) + 1];
					n[7] = pDoc->InputIMG[y - 1][3 * (x + 1) + 1];
					n[8] = pDoc->InputIMG[y - 1][3 * (x + 1) + 1];

					//sorting
					for (i = 8; i > 0; i--)
						for (j = 0; j < i; j++)
							if (n[j] > n[j + 1])
							{
								temp = n[j + 1];
								n[j + 1] = n[j];
								n[j] = temp;
							}
					pDoc->ResultIMG[y][3 * x + 1] = n[4];

					n[0] = pDoc->InputIMG[y - 1][3 * (x - 1) + 2];
					n[1] = pDoc->InputIMG[y - 1][3 * (x - 0) + 2];
					n[2] = pDoc->InputIMG[y - 1][3 * (x + 1) + 2];
					n[3] = pDoc->InputIMG[y - 0][3 * (x - 1) + 2];
					n[4] = pDoc->InputIMG[y - 0][3 * (x - 0) + 2];
					n[5] = pDoc->InputIMG[y - 0][3 * (x + 1) + 2];
					n[6] = pDoc->InputIMG[y + 1][3 * (x - 1) + 2];
					n[7] = pDoc->InputIMG[y + 1][3 * (x - 0) + 2];
					n[8] = pDoc->InputIMG[y + 1][3 * (x + 1) + 2];

					//sorting
					for (i = 8; i > 0; i--)
						for (j = 0; j < i; j++)
							if (n[j] > n[j + 1])
							{
								temp = n[j + 1];
								n[j + 1] = n[j];
								n[j] = temp;
							}
					pDoc->ResultIMG[y][3 * x + 2] = n[4];
				}

			}
		Invalidate();
	}



	void CimgProc220190832View::OnMopologyColorToGray()
	{
		CimgProc220190832Doc* pDoc = GetDocument();
		if (pDoc->depth == 1) return;

		int x, y;
		int gray;

		for (y = 1; y < pDoc->imageHeight; y++)
			for (x = 1; x < pDoc->imageWidth; x++)
			{
				gray = (pDoc->InputIMG[y][3 * x + 0] + pDoc->InputIMG[y][3 * x + 1] + pDoc->InputIMG[y][3 * x + 2]) / 3;
				pDoc->InputIMG[y][3 * x + 0] = gray;
				pDoc->InputIMG[y][3 * x + 1] = gray;
				pDoc->InputIMG[y][3 * x + 2] = gray;

			}
		Invalidate();

	}


	void CimgProc220190832View::OnMopologyBinarization()
	{
		CimgProc220190832Doc* pDoc = GetDocument();

		int x, y;
		int gray, threshold = 100;

		for (y = 1; y < pDoc->imageHeight; y++)
			for (x = 1; x < pDoc->imageWidth; x++)
			{
				if (pDoc->depth == 1)
				{
					if (pDoc->InputIMG[y][x] > threshold) pDoc->InputIMG[y][x] = 255;//값이 크면 흰색
					else                                  pDoc->InputIMG[y][x] = 0;//값이 작으면 검은색
				}
				else
				{
					if ((pDoc->InputIMG[y][3 * x + 0] + pDoc->InputIMG[y][3 * x + 1] + pDoc->InputIMG[y][3 * x + 2]) / 3 > threshold)
					{
						pDoc->InputIMG[y][3 * x + 0] = 255;
						pDoc->InputIMG[y][3 * x + 1] = 255;
						pDoc->InputIMG[y][3 * x + 2] = 255;
					}
					else
					{
						pDoc->InputIMG[y][3 * x + 0] = 0;
						pDoc->InputIMG[y][3 * x + 1] = 0;
						pDoc->InputIMG[y][3 * x + 2] = 0;

					}
				}
			}
		Invalidate();
	}


		void CimgProc220190832View::OnMopologyErosion()
		{
			CimgProc220190832Doc* pDoc = GetDocument();

			int x, y,i,j;
			int min=255,rmin,gmin,bmin;

			for (y = 1; y < pDoc->imageHeight-1; y++)
				for (x = 1; x < pDoc->imageWidth-1; x++)
				{
					min = 255; rmin = 255; gmin = 255; bmin = 255;

				    for(j= -1; j<=1; j++)
						for (i = -1; i <= 1; i++)
						{
							if (pDoc->depth == 1)
							{
								if (pDoc->InputIMG[y + j][x + i] < min)
									min = pDoc->InputIMG[y + j][x + i];
							}
							else
							{
								if (pDoc->InputIMG[y + j][3 * (x + i) + 0] < rmin) rmin = pDoc->InputIMG[y + j][3 * (x + i) + 0];
								if (pDoc->InputIMG[y + j][3 * (x + i) + 1] < gmin) gmin = pDoc->InputIMG[y + j][3 * (x + i) + 1];
								if (pDoc->InputIMG[y + j][3 * (x + i) + 2] < bmin) bmin = pDoc->InputIMG[y + j][3 * (x + i) + 2];

							}
						}
					if(pDoc->depth==1)
					pDoc->ResultIMG[y][x] = min;
					else 
					{
						pDoc->ResultIMG[y][3 * x + 0] = rmin;
						pDoc->ResultIMG[y][3 * x + 1] = gmin;
						pDoc->ResultIMG[y][3 * x + 2] = bmin;
					}
				}
			Invalidate();
		}


		void CimgProc220190832View::OnMopologyDilation()
		{
			CimgProc220190832Doc* pDoc = GetDocument();

			int x, y, i, j;
			int max, rmax, gmax, bmax;

			for (y = 1; y < pDoc->imageHeight - 1; y++)
				for (x = 1; x < pDoc->imageWidth - 1; x++)
				{
					max = 0; rmax = 0; gmax = 0; bmax = 0;

					for (j = -1; j <= 1; j++)
						for (i = -1; i <= 1; i++)
						{
							if (pDoc->depth == 1)
							{
								if (pDoc->InputIMG[y + j][x + i] > max)
									max = pDoc->InputIMG[y + j][x + i];
							}
							else
							{
								if (pDoc->InputIMG[y + j][3 * (x + i) + 0] > rmax) rmax = pDoc->InputIMG[y + j][3 * (x + i) + 0];
								if (pDoc->InputIMG[y + j][3 * (x + i) + 1] > gmax) gmax = pDoc->InputIMG[y + j][3 * (x + i) + 1];
								if (pDoc->InputIMG[y + j][3 * (x + i) + 2] > bmax) bmax = pDoc->InputIMG[y + j][3 * (x + i) + 2];

							}
						}
					if (pDoc->depth == 1)
						pDoc->ResultIMG[y][x] = max;
					else 
					{
						pDoc->ResultIMG[y][3 * x + 0] = rmax;
						pDoc->ResultIMG[y][3 * x + 1] = gmax;
						pDoc->ResultIMG[y][3 * x + 2] = bmax;
					}
				}
			Invalidate();
		}


		void CimgProc220190832View::OnMopologyOpening()
		{
			OnMopologyErosion();//한 픽셀이 줄어드는 형태로 등장
			CopyResultTolinput();
			OnMopologyErosion();
			CopyResultTolinput();
			OnMopologyErosion();

			CopyResultTolinput();
			OnMopologyDilation();
			CopyResultTolinput();
			OnMopologyDilation();
			CopyResultTolinput();
			OnMopologyDilation();//ResuluIMG에 남는 형태
		}


		void CimgProc220190832View::CopyResultTolinput()
		{
			CimgProc220190832Doc* pDoc = GetDocument();
			int x, y;

			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth*pDoc->depth; x++)
				{
					pDoc->InputIMG[y][x] = pDoc->ResultIMG[y][x];//흑백으로 처리
				}
		
		}


		void CimgProc220190832View::OnMopologyClosing()
		{
		
			OnMopologyDilation();
			CopyResultTolinput();
			OnMopologyDilation();
			CopyResultTolinput();
			OnMopologyDilation();

			CopyResultTolinput();
			OnMopologyErosion();//한 픽셀이 줄어드는 형태로 등장
			CopyResultTolinput();
			OnMopologyErosion();
			CopyResultTolinput();
			OnMopologyErosion();

		
		}


		void CimgProc220190832View::OnGeometryZoominPixelCopy()
		{
			CimgProc220190832Doc* pDoc = GetDocument();
			int x, y, i;

			int xscale = 3;
			int yscale = 2;

			if (pDoc->gResultIMG != NULL)
			{
				for (i = 0; i < pDoc->gimageHeight; i++)
					free(pDoc->gResultIMG[i]);
				free(pDoc->gResultIMG);
			}

			pDoc->gimageWidth = pDoc->imageWidth * xscale;
			pDoc->gimageHeight = pDoc->imageHeight * yscale;
		
			//메모리할당
			pDoc->gResultIMG = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
				for(i = 0; i < pDoc->gimageHeight; i++) {
					pDoc->gResultIMG[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
				}

		/*		//전방향 사상
				for (y = 0; y < pDoc->imageHeight; y++)
					for (x = 0; x < pDoc->imageWidth; x++)
					{
					  pDoc->gResultIMG[y*yscale][x*xscale] = pDoc->InputIMG[y][x];
					
					}
					*/

            //역방향 사상
			for(y=0; y<pDoc->gimageHeight; y++)
				for (x = 0; x < pDoc->gimageWidth; x++)
				{
					if(pDoc->depth==1)
					pDoc->gResultIMG[y][x] = pDoc->InputIMG[y / yscale][x / xscale];
					else {
						pDoc->gResultIMG[y][3 * x + 0] = pDoc->InputIMG[y / yscale][3 * (x / xscale) + 0];
						pDoc->gResultIMG[y][3 * x + 1] = pDoc->InputIMG[y / yscale][3 * (x / xscale) + 1];
						pDoc->gResultIMG[y][3 * x + 2] = pDoc->InputIMG[y / yscale][3 * (x / xscale) + 2];
					}
				}
				
			Invalidate();
		}


		void CimgProc220190832View::OnGeometryZoominInterpolation()
		{
			CimgProc220190832Doc* pDoc = GetDocument();
			int x, y, i;

			float xscale = 2.1;
			float yscale = 1.5;
			float src_x, src_y;
			float alpha, beta;
			int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
			int E, F;

			if (pDoc->gResultIMG != NULL)
			{
				for (i = 0; i < pDoc->gimageHeight; i++)
					free(pDoc->gResultIMG[i]);
				free(pDoc->gResultIMG);
			}

			pDoc->gimageWidth = (pDoc->imageWidth) * xscale;
			pDoc->gimageHeight = (pDoc->imageHeight) * yscale;
			//메모리 할당
			pDoc->gResultIMG = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
			for (i = 0; i < pDoc->gimageHeight; i++) 
			{
				pDoc->gResultIMG[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
			}

			//역방향 사상
			for (y = 0; y < pDoc->gimageHeight; y++)
				for (x = 0; x < pDoc->gimageWidth; x++)
				{
					src_x = x / xscale;
					src_y = y / yscale;
					alpha = src_x - (int)src_x;
					beta = src_y - (int)src_y;

					Ax = (int)src_x;
					Ay = (int)src_y;
					Bx = Ax + 1;
					By = Ay;
					Cx = Ax;
					Cy = Ay + 1;
					Dx = Ax + 1;
					Dy = Ay + 1;

					if (Bx > pDoc->imageWidth - 1) Bx = pDoc->imageWidth - 1;
					if (Cy > pDoc->imageHeight - 1) Cy = pDoc->imageHeight - 1;
					if (Dx > pDoc->imageWidth - 1) Dx = pDoc->imageWidth - 1;
					if (Dy > pDoc->imageHeight - 1) Dy = pDoc->imageHeight - 1;



					if (pDoc->depth == 1)
					{
						E = (1 - alpha) * pDoc->InputIMG[Ay][Ax] + alpha * pDoc->InputIMG[By][Bx];
						F = (1 - alpha) * pDoc->InputIMG[Cy][Cx] + alpha * pDoc->InputIMG[Dy][Dx];

						pDoc->gResultIMG[y][x] = (1 - beta) * E + beta * F;
					}
					else
					{
						E = (1 - alpha) * pDoc->InputIMG[Ay][3 * Ax + 0] + alpha * pDoc->InputIMG[By][3 * Bx + 0];
						F = (1 - alpha) * pDoc->InputIMG[Cy][3 * Cx + 0] + alpha * pDoc->InputIMG[Dy][3 * Dx + 0];
						pDoc->gResultIMG[y][3 * x + 0] = (1 - beta) * E + beta * F;

						E = (1 - alpha) * pDoc->InputIMG[Ay][3 * Ax + 1] + alpha * pDoc->InputIMG[By][3 * Bx + 1];
						F = (1 - alpha) * pDoc->InputIMG[Cy][3 * Cx + 1] + alpha * pDoc->InputIMG[Dy][3 * Dx + 1];
						pDoc->gResultIMG[y][3 * x + 1] = (1 - beta) * E + beta * F;

						E = (1 - alpha) * pDoc->InputIMG[Ay][3 * Ax + 2] + alpha * pDoc->InputIMG[By][3 * Bx + 2];
						F = (1 - alpha) * pDoc->InputIMG[Cy][3 * Cx + 2] + alpha * pDoc->InputIMG[Dy][3 * Dx + 2];
						pDoc->gResultIMG[y][3 * x + 2] = (1 - beta) * E + beta * F;
					}

				}

			Invalidate();
		
		}

		
		void CimgProc220190832View::OnGeometryZoomoutSubsampling()
		{
			CimgProc220190832Doc* pDoc = GetDocument();
			int x, y, i;
			int xscale = 3;		//	1/3
			int yscale = 2;		//	1/2
			if (pDoc->gResultIMG != NULL)
			{
				for (int i = 0; i < pDoc->gimageHeight; i++)
					free(pDoc->gResultIMG[i]);
				free(pDoc->gResultIMG);

			}
			pDoc->gimageWidth = pDoc->imageWidth / xscale;
			pDoc->gimageHeight = pDoc->imageHeight / yscale;

			//메모리 할당
			pDoc->gResultIMG = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
			for (int i = 0; i < pDoc->gimageHeight; i++)
			{
				pDoc->gResultIMG[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
			}
			//역방향사상
			for (y = 0; y < pDoc->gimageHeight; y++)
				for (x = 0; x < pDoc->gimageWidth; x++)
				{
					if (pDoc->depth == 1)
						pDoc->gResultIMG[y][x] = pDoc->InputIMG[y * yscale][x * xscale];
					else
					{
						pDoc->gResultIMG[y][3 * x + 0] = pDoc->InputIMG[y * yscale][3 * (x * xscale) + 0];
						pDoc->gResultIMG[y][3 * x + 1] = pDoc->InputIMG[y * yscale][3 * (x * xscale) + 1];
						pDoc->gResultIMG[y][3 * x + 2] = pDoc->InputIMG[y * yscale][3 * (x * xscale) + 2];

					}
				}
			Invalidate();
		}

		void CimgProc220190832View::OnGeometryZoomoutMeansub()
		{
			OnRegionSmoothing();
			CopyResultTolinput();
			OnGeometryZoomoutSubsampling();
		}





		void CimgProc220190832View::OnGeometryZoomoutAvgsampling()
		{
			CimgProc220190832Doc* pDoc = GetDocument();

			int xscale = 3;
			int yscale = 2;
			int x, y, j, i;
			int sum, rsum, gsum, bsum;
			int src_x, src_y;

			if (pDoc->gResultIMG != NULL) {
				for (int i = 0; i < pDoc->gimageHeight; i++)
					free(pDoc->gResultIMG[i]);
				free(pDoc->gResultIMG);
			}

			pDoc->gimageWidth = pDoc->imageWidth / xscale;      // 줄어들었을 때 이미지 크기
			pDoc->gimageHeight = pDoc->imageHeight / yscale;

			pDoc->gResultIMG = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
			for (int i = 0; i < pDoc->gimageHeight; i++)
				pDoc->gResultIMG[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);

			// 전방향 사상
			for (y = 0; y < pDoc->imageHeight - yscale; y += yscale)
				for (x = 0; x < pDoc->imageWidth - xscale; x += xscale) {
					if (pDoc->depth == 1) {
						sum = 0;
						for (j = 0; j < yscale; j++)
							for (i = 0; i < xscale; i++) {
								src_x = x + i;
								src_y = y + j;
								sum += pDoc->InputIMG[src_y][src_x];
							}
						pDoc->gResultIMG[y / yscale][x / xscale] = sum / (xscale * yscale);
					}
					else {
						rsum = gsum = bsum = 0;
						for (j = 0; j < yscale; j++)
							for (i = 0; i < xscale; i++) {
								src_x = x + i;
								src_y = y + j;
								rsum += pDoc->InputIMG[src_y][3 * src_x + 0];
								gsum += pDoc->InputIMG[src_y][3 * src_x + 1];
								bsum += pDoc->InputIMG[src_y][3 * src_x + 2];
							}
						pDoc->gResultIMG[y / yscale][3 * (x / xscale) + 0] = rsum / (xscale * yscale);
						pDoc->gResultIMG[y / yscale][3 * (x / xscale) + 1] = gsum / (xscale * yscale);
						pDoc->gResultIMG[y / yscale][3 * (x / xscale) + 2] = bsum / (xscale * yscale);
					}
				}
			Invalidate();
		}
#define PI 3.1415926521
		
#include "CAngleinputDialog.h"
		void CimgProc220190832View::OnGeometryRotation()
		{
			CimgProc220190832Doc* pDoc = GetDocument();
			CAngleinputDialog dlg;

			int angle = -45;	// degree
			float radian;
			int Hy;
			int Cx, Cy;		// 영상의 가운데 좌표값
			int xdiff, ydiff;
			int x, y;
			int x_source, y_source;

			dlg.m_iAngle = angle;
			if (dlg.DoModal() == IDCANCEL)		return;
			angle = dlg.m_iAngle;

			radian = PI / 180 * angle;

			// y의 마지막 좌표값		(좌표랑 관련된 파라미터)
			Hy = pDoc->imageHeight - 1;
			// 중심점
			Cx = pDoc->imageWidth / 2;
			Cy = pDoc->imageHeight / 2;

			if (pDoc->gResultIMG != NULL)
			{
				for (int i = 0; i < pDoc->gimageHeight; i++)
					free(pDoc->gResultIMG[i]);
				free(pDoc->gResultIMG);
			}


			// 이미지 가로세로 크기구하기		fabs : 크기 음수되는 거를 막음
			pDoc->gimageWidth = pDoc->imageWidth * fabs(cos(PI / 2 - radian)) + pDoc->imageWidth * fabs(cos(radian));		// 90도
			pDoc->gimageHeight = pDoc->imageHeight * fabs(cos(radian)) + pDoc->imageWidth * fabs(cos(PI / 2 - radian));

			//메모리 할당
			pDoc->gResultIMG= (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
			for (int i = 0; i < pDoc->gimageHeight; i++)
			{
				//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
				pDoc->gResultIMG[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
			}

			xdiff = (pDoc->gimageWidth - pDoc->imageWidth) / 2;
			ydiff = (pDoc->gimageHeight - pDoc->imageHeight) / 2;

			for (y = -ydiff; y < pDoc->gimageHeight - ydiff; y++)
				for (x = -xdiff; x < pDoc->gimageWidth - xdiff; x++)
				{
					x_source = (Hy - y - Cx) * sin(radian) + (x - Cx) * cos(radian) + Cx;
					y_source = Hy - ((Hy - y - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);
					if (pDoc->depth == 1)
					{
						if (x_source<0 || x_source>pDoc->imageWidth - 1 ||
							y_source <0 || y_source>pDoc->imageHeight - 1)	//-1 : 크면 가져올 영역을 넘어선거임
							pDoc->gResultIMG[y + ydiff][x + xdiff] = 0;		// 검은색
						else
							pDoc->gResultIMG[y + ydiff][x + xdiff] = pDoc->InputIMG[y_source][x_source];
					}
					else
					{
						if (x_source<0 || x_source>pDoc->imageWidth - 1 || y_source<0 || y_source>pDoc->imageHeight - 1) {
							pDoc->gResultIMG[y + ydiff][3 * (x + xdiff) + 0] = 255;
							pDoc->gResultIMG[y + ydiff][3 * (x + xdiff) + 1] = 255;
							pDoc->gResultIMG[y + ydiff][3 * (x + xdiff) + 2] = 255;
						}
						else {
							pDoc->gResultIMG[y + ydiff][3 * (x + xdiff) + 0] = pDoc->InputIMG[y_source][3 * x_source + 0];
							pDoc->gResultIMG[y + ydiff][3 * (x + xdiff) + 1] = pDoc->InputIMG[y_source][3 * x_source + 1];
							pDoc->gResultIMG[y + ydiff][3 * (x + xdiff) + 2] = pDoc->InputIMG[y_source][3 * x_source + 2];
						}
					}
				}
			Invalidate();
		}


		


		void CimgProc220190832View::OnGeometryMirror()
		{
			CimgProc220190832Doc* pDoc = GetDocument();
			int x, y;

			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
				{
					if(pDoc->depth==1)
					pDoc->ResultIMG[y][x] = pDoc->InputIMG[y][pDoc->imageWidth - 1 - x];
					else
					{
						pDoc->ResultIMG[y][3 * x + 0] = pDoc->InputIMG[y][3 * (pDoc->imageWidth - 1 - x) + 0];
						pDoc->ResultIMG[y][3 * x + 1] = pDoc->InputIMG[y][3 * (pDoc->imageWidth - 1 - x) + 1];
						pDoc->ResultIMG[y][3 * x + 2] = pDoc->InputIMG[y][3 * (pDoc->imageWidth - 1 - x) + 2];

					}


				}
			Invalidate();
		}


		void CimgProc220190832View::OnGeometryFlip()
		{
			CimgProc220190832Doc* pDoc = GetDocument();
			int x, y;
			
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
				{
					if (pDoc->depth == 1)
						pDoc->ResultIMG[y][x] = pDoc->InputIMG[pDoc->imageHeight - 1 - y][x];
					else
					{
						pDoc->ResultIMG[y][3 * x + 0] = pDoc->InputIMG[pDoc->imageHeight - 1 - y][3 * x + 0];
						pDoc->ResultIMG[y][3 * x + 1] = pDoc->InputIMG[pDoc->imageHeight - 1 - y][3 * x + 1];
						pDoc->ResultIMG[y][3 * x + 2] = pDoc->InputIMG[pDoc->imageHeight - 1 - y][3 * x + 2];
					}
					

				}
			Invalidate();
		}

		typedef struct 
		{
			int Px;
			int Py;
			int Qx;
			int Qy;

		}control_line;

		control_line mctrl_source = { 100,100,150,150 };
		control_line mctrl_dest = { 100,100,200,200 };

		void CimgProc220190832View::OnGeometryWarping()
		{
			CimgProc220190832Doc* pDoc = GetDocument();

			control_line source_lines[5] = { {100,100,150,150},
				{0,0,pDoc->imageWidth - 1,0},{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1},
				{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1},{0,pDoc->imageHeight - 1,0,0} };
			control_line dest_lines[5] = { {100,100,200,200},
				{0,0,pDoc->imageWidth - 1,0},{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1},
				{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1},{0,pDoc->imageHeight - 1,0,0} };

			source_lines[0] = mctrl_source;
			dest_lines[0] = mctrl_dest;
			int x, y;

			double u;
			double h;
			double d;
			double tx, ty;
			double xp, yp;

			double weight;
			double totalweight;
			double a = 0.01;
			double b = 2.0;
			double p = 0.75;

			int x1, x2, y1, y2;
			int src_x1, src_y1, src_x2, src_y2;
			double src_line_length, dest_line_length;

			int num_lines = 5;
			int line;
			int source_x, source_y;
			int last_row, last_col;

			last_row = pDoc->imageHeight - 1;
			last_col = pDoc->imageWidth - 1;

			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
				{
					tx = 0.0;
					ty = 0.0;
					totalweight = 0.0;
					//각제어선의 영향을 계산
					for (line = 0; line < num_lines; line++)
					{
						x1 = dest_lines[line].Px;
						y1 = dest_lines[line].Py;
						x2 = dest_lines[line].Qx;
						y2 = dest_lines[line].Qy;

						dest_line_length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

						u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
							(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

						h = ((y - y1)*(x2 - x1) - (x - x1)*(y2 - y1)) / dest_line_length;

						if (u < 0) d = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
						else if (u > 1) d = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
						else            d = fabs(h);

						src_x1 = source_lines[line].Px;
						src_y1 = source_lines[line].Py;
						src_x2 = source_lines[line].Qx;
						src_y2 = source_lines[line].Qy;

						src_line_length=sqrt((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));

						xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
						yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

						weight = pow(pow(dest_line_length, p) / (a + d), b);

						tx += (xp - x) * weight;
						ty += (yp - y) * weight;
						totalweight += weight;
					}
					source_x = x + (tx / totalweight);
					source_y = y + (ty / totalweight);

					if (source_x < 0) source_x = 0;
					if (source_x > last_col) source_x=last_col;

					if (source_y < 0) source_y = 0;
					if (source_y > last_row) source_y = last_row;

					if(pDoc->depth==1)
					pDoc->ResultIMG[y][x] = pDoc->InputIMG[source_y][source_x];
					else
					{
						pDoc->ResultIMG[y][3 * x + 0] = pDoc->InputIMG[source_y][3 * source_x + 0];
						pDoc->ResultIMG[y][3 * x + 1] = pDoc->InputIMG[source_y][3 * source_x + 1];
						pDoc->ResultIMG[y][3 * x + 2] = pDoc->InputIMG[source_y][3 * source_x + 2];
					}
				}
			Invalidate();
		}

		CPoint mpos_st, mpos_end;

		void CimgProc220190832View::OnLButtonDown(UINT nFlags, CPoint point)
		{
			mpos_st = point;

			CScrollView::OnLButtonDown(nFlags, point);
		}


		void CimgProc220190832View::OnLButtonUp(UINT nFlags, CPoint point)
		{
			mpos_end = point;

			CDC* pDC = GetDC();
			CPen rpen;
			rpen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
			pDC->SelectObject(&rpen);

			pDC->MoveTo(mpos_st);
			pDC->LineTo(mpos_end);
			ReleaseDC(pDC);

			int Ax, Ay, Bx, By;
			Ax = mpos_st.x;
			Ay = mpos_st.y;
			Bx = mpos_end.x;
			By = mpos_end.y;

			if (Ax < Bx) mctrl_source.Px = Ax - (Bx - Ax) / 2;
			else         mctrl_source.Px = Ax + (Ax - Bx) / 2;

			if (Ay < By) mctrl_source.Px = Ay - (By - Ay) / 2;
			else         mctrl_source.Px = Ay + (Ay - By) / 2;

			mctrl_dest.Px = mctrl_source.Px;
			mctrl_dest.Py = mctrl_source.Py;

			mctrl_source.Qx = mpos_st.x;
			mctrl_source.Qy = mpos_st.y;
			mctrl_dest.Qx = mpos_end.x;
			mctrl_dest.Qy = mpos_end.y;

			CScrollView::OnLButtonUp(nFlags, point);
		}


		void CimgProc220190832View::OnAviView()
		{
			CFileDialog dlg(true, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				"AVI화일(*.avi)|*.avi|모든화일|*.*|");

			if (dlg.DoModal() == IDOK)
			{
				AVIFileName = dlg.GetPathName();
				bAviMode = true;
				Invalidate();
			}
			
		}

		
		void CimgProc220190832View::LoadAviFile(CDC* pDC)
		{
			PAVIFILE pavi;
			AVIFILEINFO fi;
			int stm;
			PAVISTREAM pstm = NULL;
			AVISTREAMINFO si;
			PGETFRAME pfrm = NULL;
			int frame;
			LPBITMAPINFOHEADER pbmpih;
			unsigned char* image;
			int x, y;

			AVIFileInit();
			AVIFileOpen(&pavi, AVIFileName, OF_READ | OF_SHARE_DENY_NONE, NULL);
			AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));

			for (stm = 0; stm < fi.dwStreams; stm++)
			{
				AVIFileGetStream(pavi, &pstm, 0, stm);
				AVIStreamInfo(pstm, &si, sizeof(si));
				if (si.fccType == streamtypeVIDEO)
				{
					pfrm=AVIStreamGetFrameOpen(pstm, NULL);
					for (frame = 0; frame < 100; frame++) //si.dwLength
					{
						pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
						if (!pbmpih)continue;

						image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);
						/*
						for(y=0; y<fi.dwHeight; y++)
							for (x = 0; x < fi.dwWidth; x++)
							{
								pDC->SetPixel(x, fi.dwHeight - 1 - y,
									RGB(image[(y * fi.dwWidth + x) * 3 + 0],
										image[(y * fi.dwWidth + x) * 3 + 1],
										image[(y * fi.dwWidth + x) * 3 + 2])); 
							}
							*/
						::SetDIBitsToDevice(pDC->GetSafeHdc(),
							0, 0, fi.dwWidth, fi.dwHeight,
							0, 0, 0, fi.dwWidth,
							image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
						Sleep(30);
							
					}
				}
			}
		}

#include "COpenCVDlg.h"
		void CimgProc220190832View::OnOpencvView()
		{
			COpenCVDlg dlg;

			dlg.DoModal();
		}

#define MORPHING   8
#define NUM_FRAMES 10
void CimgProc220190832View::OnGeometryMorphing()
{
	CimgProc220190832Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	control_line source_line[23] =
	{ {116,7,207,5},{34,109,90,21},{55,249,30,128},{118,320,65,261},
	 {123,321,171,321},{179,319,240,264},{247,251,282,135},{281,114,228,8},
	 {78,106,123,109},{187,115,235,114},{72,142,99,128},{74,150,122,154},
	 {108,127,123,146},{182,152,213,132},{183,159,229,157},{219,131,240,154},
	 {80,246,117,212},{127,222,146,223},{154,227,174,221},{228,252,183,213},
	 {114,255,186,257},{109,258,143,277},{152,278,190,262} };
	control_line dest_line[23] =
	{ {120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},
	 {142,337,181,335},{192,335,232,280},{244,259,288,108},{285,92,212,13},
	 {96,135,136,118},{194,119,223,125},{105,145,124,134},{110,146,138,151},
	 {131,133,139,146},{188,146,198,134},{189,153,218,146},{204,133,221,140},
	 {91,268,122,202},{149,206,159,209},{170,209,181,204},{235,265,208,199},
	 {121,280,205,284},{112,286,160,301},{166,301,214,287} };

	double u;
	double h;
	double d;
	double tx, ty;
	double xp, yp;
	double weight;
	double totalWeight;
	double a = 0.001, b = 2.0, p = 0.75;
	unsigned char** warpedImg = NULL;
	unsigned char** warpedImg2 = NULL;
	int frame;
	double fweight;
	control_line warp_lines[23];
	double tx2, ty2, xp2, yp2;
	int dest_x1, dest_y1, dest_x2, dest_y2, source_x2, source_y2;
	int x1, y1, x2, y2, src_x1, src_x2, src_y1, src_y2;
	double src_line_length, dest_line_length;
	int i, j;
	int num_lines = 23;
	int line, x, y, source_x, source_y, last_row, last_col;

	//두 입력 영상을 읽어 들임
	LoadTwoImage2();

	// 중간 프레임의 위핑 결과를 저장

	if (warpedImg != NULL) {
		for (i = 0; i < pDoc->imageHeight; i++)
			free(warpedImg[i]);
		free(warpedImg);
	}
	warpedImg = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->imageHeight; i++) warpedImg[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);

	if (warpedImg2 != NULL) {
		for (i = 0; i < pDoc->imageHeight; i++)
			free(warpedImg2[i]);
		free(warpedImg2);
	}
	warpedImg2 = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->imageHeight; i++) warpedImg2[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);

	for (i = 0; i < NUM_FRAMES; i++) {
		if (pDoc->morphedImg[i] != NULL) {
			int  j;
			for (j = 0; j < pDoc->imageHeight; j++)
				free(pDoc->morphedImg[i][j]);
			free(pDoc->morphedImg[i]);
		}
	}
	for (i = 0; i < NUM_FRAMES; i++) {
		pDoc->morphedImg[i] = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
		for (j = 0; j < pDoc->imageHeight; j++) pDoc->morphedImg[i][j] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}


	last_col = pDoc->imageWidth - 1;
	last_row = pDoc->imageHeight - 1;

	for (frame = 1; frame <= NUM_FRAMES; frame++) {
		fweight = (double)(frame) / NUM_FRAMES;

		for (line = 0; line < num_lines; line++) {
			warp_lines[line].Px = (int)(source_line[line].Px + (dest_line[line].Px - source_line[line].Px) * fweight);
			warp_lines[line].Py = (int)(source_line[line].Py + (dest_line[line].Py - source_line[line].Py) * fweight);
			warp_lines[line].Qx = (int)(source_line[line].Qx + (dest_line[line].Qx - source_line[line].Qx) * fweight);
			warp_lines[line].Qy = (int)(source_line[line].Qy + (dest_line[line].Qy - source_line[line].Qy) * fweight);
		}
		for (y = 0; y < pDoc->imageHeight; y++) {
			for (x = 0; x < pDoc->imageHeight; x++) {
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;
				tx2 = 0.0;
				ty2 = 0.0;
				for (line = 0; line < num_lines; line++)
				{
					x1 = warp_lines[line].Px;
					y1 = warp_lines[line].Py;
					x2 = warp_lines[line].Qx;
					y2 = warp_lines[line].Qy;
					dest_line_length = sqrt((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
						(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					if (u < 0) d = sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));
					else if (u > 1) d = sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));
					else d = fabs(h);

					src_x1 = source_line[line].Px;
					src_y1 = source_line[line].Py;
					src_x2 = source_line[line].Qx;
					src_y2 = source_line[line].Qy;
					src_line_length = sqrt((double)(src_x2 - src_x1) * (src_x2 - src_x1) +
						(src_y2 - src_y1) * (src_y2 - src_y1));
					dest_x1 = dest_line[line].Px;
					dest_y1 = dest_line[line].Py;
					dest_x2 = dest_line[line].Qx;
					dest_y2 = dest_line[line].Qy;
					dest_line_length = sqrt((double)(dest_x2 - dest_x1) * (dest_x2 - dest_x1) +
						(dest_y2 - dest_y1) * (dest_y2 - dest_y1));

					xp = src_x1 + u * (src_x2 - src_x1) -
						h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) +
						h * (src_x2 - src_x1) / src_line_length;

					xp2 = dest_x1 + u * (dest_x2 - dest_x1) -
						h * (dest_y2 - dest_y1) / dest_line_length;
					yp2 = dest_y1 + u * (dest_y2 - dest_y1) +
						h * (dest_x2 - dest_x1) / dest_line_length;
					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					tx += (xp - x) * weight;
					ty += (yp - y) * weight;

					tx2 += (xp2 - x) * weight;
					ty2 += (yp2 - y) * weight;

					totalWeight += weight;
				}
				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				source_x2 = x + (int)(tx2 / totalWeight + 0.5);
				source_y2 = y + (int)(ty2 / totalWeight + 0.5);

				if (source_x < 0) source_x = 0;
				if (source_x > last_col) source_x = last_col;
				if (source_y < 0) source_y = 0;
				if (source_y > last_row) source_y = last_row;

				if (source_x2 < 0) source_x2 = 0;
				if (source_x2 > last_col) source_x2 = last_col;
				if (source_y2 < 0) source_y2 = 0;
				if (source_y2 > last_row) source_y2 = last_row;
				warpedImg[y][x] = pDoc->InputIMG[source_y][source_x];
				warpedImg2[y][x] = pDoc->InputIMG2[source_y2][source_x2];
			}
		}

		for (y = 0; y < pDoc->imageHeight; y++)
			for (x = 0; x < pDoc->imageWidth; x++) {
				int val = (int)((1.0 - fweight) * warpedImg[y][x] +
					fweight * warpedImg2[y][x]);
				if (val < 0) val = 0;
				if (val > 255) val = 255;
				pDoc->morphedImg[frame - 1][y][x] = val;
			}
	}
	Invalidate();
}

void CimgProc220190832View::LoadTwoImage2()
{
	CimgProc220190832Doc* pDoc = GetDocument();
	CFile file;
	CFileDialog dlg(TRUE);

	AfxMessageBox("Select the First Image");

	if (dlg.DoModal() == IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadImageFile(ar);
		file.Close();
	}
	AfxMessageBox("Select the Second Image");
	if (dlg.DoModal() == IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();
	}

}
