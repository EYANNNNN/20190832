
// imgProc2_20190832Doc.cpp: CimgProc220190832Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "imgProc2_20190832.h"
#endif

#include "imgProc2_20190832Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define NUM_FRAMES 10

// CimgProc220190832Doc

IMPLEMENT_DYNCREATE(CimgProc220190832Doc, CDocument)

BEGIN_MESSAGE_MAP(CimgProc220190832Doc, CDocument)
END_MESSAGE_MAP()


// CimgProc220190832Doc 생성/소멸

CimgProc220190832Doc::CimgProc220190832Doc() noexcept
{
	InputIMG = NULL;
	InputIMG2 = NULL;
	ResultIMG = NULL;

	gResultIMG = NULL;
	for (int i = 0; i < NUM_FRAMES; i++)morphedImg[i] = NULL;
}

CimgProc220190832Doc::~CimgProc220190832Doc()
{
	int i;

	if (InputIMG != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(InputIMG[i]);
		free(InputIMG);
	}
	if (InputIMG2 != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(InputIMG2[i]);
		free(InputIMG2);
	}
	if (ResultIMG != NULL)
	{
		for (i = 0; i < imageHeight; i++)
			free(ResultIMG[i]);
		free(ResultIMG);
	}
		if (gResultIMG != NULL)
		{
			for (i = 0; i < gimageHeight; i++)
				free(gResultIMG[i]);
			free(gResultIMG);
		}
		for (i = 0; i < NUM_FRAMES; i++) {
			if (morphedImg[i] != NULL) {
				int j;
				for (j = 0; j < imageHeight; j++)
					free(morphedImg[i][j]);
				free(morphedImg[i]);
			}
		}
	}

BOOL CimgProc220190832Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CimgProc220190832Doc serialization

void CimgProc220190832Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		LoadImageFile(ar);
		// TODO: 여기에 로딩 코드를 추가합니다.
		
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CimgProc220190832Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CimgProc220190832Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CimgProc220190832Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CimgProc220190832Doc 진단

#ifdef _DEBUG
void CimgProc220190832Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CimgProc220190832Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CimgProc220190832Doc 명령


void CimgProc220190832Doc::LoadImageFile(CArchive& ar)
{
	int maxValue;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();
	bool isbmp = false;

	if (strcmp(strchr(fname, '.'), ".ppm") == 0 || strcmp(strchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strchr(fname, '.'), ".pgm") == 0 || strcmp(strchr(fname, '.'), ".PGM") == 0)
	{
		ar.ReadString(type, 15); //P5
		do {
			ar.ReadString(buf, 256);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		do {
			ar.ReadString(buf, 256);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) depth = 1;
		else                         depth = 3;
	}
	else if (strcmp(strchr(fname, '.'), ".bmp") == 0 || strcmp(strchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		//bmp화일임을 나타내는 "BM"마커가 있는지 확인
		if (bmfh.bfType != (WORD)(('M' << 8) | 'B'))  return;//상위 아스키코드 M 하위 아스키코드 B 

		//bitmap info header읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		imageWidth = bih.biWidth;
		imageHeight = bih.biHeight;
		depth = bih.biBitCount / 8;

		//palette 처리
		if (depth == 1)
		{//palette존재
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);

		}
		isbmp = true;
	}
	else if (strcmp(strchr(fname, '.'), ".raw") == 0 || strcmp(strchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256)
		{
			AfxMessageBox("256*256크기의 raw화일만 읽을수 있습니다.");
			return;
		}

		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}

	InputIMG = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*)); //기억 장소 할당
	ResultIMG = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));//기억 장소 할당

	for (int i = 0; i < imageHeight; i++)
	{
		InputIMG[i] = (unsigned char*)malloc(imageWidth * depth);
		ResultIMG[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	if (!isbmp)
	{
		for (int i = 0; i < imageHeight; i++)
			ar.Read(InputIMG[i], imageWidth * depth);
	}
	else
	{
		//파일에서 읽어서 저장
		BYTE nu[4*3];
		int widthfile;
		widthfile = (imageWidth * 8 + 31) / 32 * 4;
		for (int i = 0; i < imageHeight; i++)
		{
			if(depth ==1)
			   ar.Read(InputIMG[imageHeight-1-i],imageWidth * depth);
			else
			{
				//BGR => RGB
				BYTE r, g, b;
				for (int j = 0; j < imageWidth; j++)
				{
					ar.Read(&b, 1); ar.Read(&g, 1); ar.Read(&r, 1);
					InputIMG[imageHeight - 1 - i][3 * j + 0] = r;
					InputIMG[imageHeight - 1 - i][3 * j + 1] = g;
					InputIMG[imageHeight - 1 - i][3 * j + 2] = b;
				}

			}

			if ((widthfile - imageWidth) != 0)
				ar.Read(nu, (widthfile - imageWidth) * depth);
		}
	}

}

void CimgProc220190832Doc::LoadSecondImageFile(CArchive& ar)
{
	int i, maxValue;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();   // CStrung - 문자열을 다루는 여러 기능 포함// fp-> 함수 이름 넘김
	bool isbmp = false;


	int imgw, imgh, imgd;

	// strcmp : 문자열 비교 / strrchr : 
	if (strcmp(strrchr(fname, '.'), ".ppm") == 0 || strcmp(strrchr(fname, '.'), ".PPM") == 0 ||
		strcmp(strrchr(fname, '.'), ".pgm") == 0 || strcmp(strrchr(fname, '.'), ".PGM") == 0)
	{
		ar.ReadString(type, 15);   // type 변수 15글자 읽음 마지막 하나는 null값
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');   // #으로 시작하면 버림
		sscanf_s(buf, "%d %d", &imgw, &imgh);   // sscanf 문자열(버퍼)에 들어간걸 처리 (저장된 변수, 숫자로 바꿈, 숫자

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');   // #으로 시작하면 버림
		sscanf_s(buf, "%d", &maxValue);

		if (strcmp(type, "P5") == 0) imgd = 1;   // 흑백
		else                         imgd = 3;   // 컬러
	}

	else if (strcmp(strrchr(fname, '.'), ".bmp") == 0 || strcmp(strrchr(fname, '.'), ".BMP") == 0)
	{
		//bitmap file header읽기
		BITMAPFILEHEADER bmfh;
		ar.Read((LPSTR)&bmfh, sizeof(bmfh));
		if (bmfh.bfType != (WORD)('B' | ('M' << 8)))
			return;

		//bitmap info header 읽기
		BITMAPINFOHEADER bih;
		ar.Read((LPSTR)&bih, sizeof(bih));
		imgw = bih.biWidth;
		imgh = bih.biHeight;
		imgd = bih.biBitCount / 8;


		if (imgd == 1)
		{
			// palette 존재함
			BYTE palette[256 * 4];
			ar.Read(palette, 256 * 4);
		}
		isbmp = true;

	}



	else if (strcmp(strrchr(fname, '.'), ".raw") == 0 || strcmp(strrchr(fname, '.'), ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256x256 크기의 파일만 가능합니다.");
			return;
		}

		imgw = 256;
		imgh = 256;
		imgd = 1;
	}

	if (imgw != imageWidth || imgh != imageHeight || imgd != depth) {
		AfxMessageBox("동일한 크기의 화일만 읽어들일 수 있습니다.");
		return;
	}

	// 메모리 할당
	//메모리 얼로케이션?
	//inputImg : unsigned char 포인터의 포인터
	InputIMG2 = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
	for (int i = 0; i < imageHeight; i++) {
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		InputIMG2[i] = (unsigned char*)malloc(imageWidth * depth);
	}

	// 영상데이터 읽기
	//for (int i = 0; i < imageHeight; i++) {
	//   ar.Read(inputImg2[i], imageWidth * depth);   //ar.Read는 아스키 코드 문자열을 읽음
	//}

	   // 영상데이터 읽기
	if (!isbmp)
	{
		for (int i = 0; i < imageHeight; i++)
			ar.Read(InputIMG2[i], imageWidth * depth);   //ar.Read는 아스키 코드 문자열을 읽음
	}

	else
	{
		//파일에서 읽어서 저장
		BYTE nu[4 * 3];
		int widthfile;
		widthfile = (imageWidth * 8 + 32) / 32 * 4; // 4의 배수를 만들기 위해서 필요한 부분
		for (int j = 0; j < imageHeight; j++)
		{
			if (depth == 1)
				ar.Read(InputIMG2[imageHeight - 1 - j], imageWidth * depth); // 이 부분을 추가해야 사선이 사라짐   //ar.Read는 아스키 코드 문자열을 읽음

			else
			{
				for (int i = 0; i < imageWidth; i++)
				{
					BYTE r, g, b;
					ar.Read(&b, 1);
					ar.Read(&g, 1);
					ar.Read(&r, 1);

					InputIMG2[imageHeight - 1 - j][3 * i + 0] = r;
					InputIMG2[imageHeight - 1 - j][3 * i + 1] = g;
					InputIMG2[imageHeight - 1 - j][3 * i + 2] = b;

				}
			}

			if ((widthfile - imageHeight) != 0)
			{
				ar.Read(nu, (widthfile - imageWidth) * depth);
			}
		}
	}
}
