
// imgProc2_20190832Doc.h: CimgProc220190832Doc 클래스의 인터페이스
//


#pragma once


class CimgProc220190832Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CimgProc220190832Doc() noexcept;
	DECLARE_DYNCREATE(CimgProc220190832Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CimgProc220190832Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char **InputIMG; // 입력 영상의 기억장소에 대한 포인터 변수
	unsigned char **InputIMG2; //출력 영상의 기억장소에 대한 포인터 변수
	unsigned char **ResultIMG; // 결과 값
	int imageWidth; //영상의 가로 크기
	int imageHeight; // 영상의 세로 크기
	int depth; //1= 흑백,3=컬러

	int gimageWidth; //영상의 가로 크기
	int gimageHeight; // 영상의 세로 크기
	//출력이미지
	unsigned char** gResultIMG; //[y][x]

	void LoadImageFile(CArchive& ar);
	void LoadSecondImageFile(CArchive& ar);

	unsigned char** morphedImg[10];
};
