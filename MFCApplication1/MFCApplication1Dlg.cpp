
// MFCApplication1Dlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <vector>

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CMFCApplication1Dlg



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// Обработчики сообщений CMFCApplication1Dlg

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* po = GetDlgItem(IDC_STATIC);
	if (po == nullptr)
	{
		MessageBoxA(0, "picture control is bad", "error", MB_OK);
		exit(1);
	}
	pictureControl = (CStatic*)(po);

	po = GetDlgItem(IDC_SLIDER1);
	if (po == nullptr)
	{
		MessageBoxA(0, "slider brightness is bad", "error", MB_OK);
		exit(1);
	}
	sliderBright = (CSliderCtrl*)po;
	sliderBright->SetPos(50);

	po = GetDlgItem(IDC_SLIDER2);
	if (po == nullptr)
	{
		MessageBoxA(0, "slider contrast is bad", "error", MB_OK);
		exit(1);
	}
	sliderContr = (CSliderCtrl*)po;
	sliderContr->SetPos(50);

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
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



	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int normalizeRGB(int value)
{
	if (value > 255)
		return 255;
	if (value < 0)
		return 0;
	return value;
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	CFileDialog fileDialog(true,NULL,NULL,NULL, L".bmp|*.bmp");//
	int result = fileDialog.DoModal();
	if (result == IDOK)
	{
		CString fileName = fileDialog.GetPathName();
		image.Destroy();
		CImage input;
		HRESULT result= input.Load(fileName);
		if (result == S_OK)
		{

			CDC *screenDC = GetDC();
			CDC *pMDC = new CDC;
			pMDC->CreateCompatibleDC(screenDC);

			CBitmap *pb = new CBitmap;
			pb->CreateCompatibleBitmap(screenDC, 300, 250);
			CBitmap *pob = pMDC->SelectObject(pb);
			int w = input.GetWidth();
			int h = input.GetHeight();
			SetStretchBltMode(pMDC->m_hDC, COLORONCOLOR);
			input.StretchBlt(pMDC->m_hDC, 0, 0, 300, 250, 0, 0, w, h, SRCCOPY);
			pMDC->SelectObject(pob);
			image.Attach((HBITMAP)(*pb));

			pictureControl->SetBitmap((HBITMAP)image);

				
			vec.erase(vec.begin(), vec.end());

			width = image.GetWidth();
			height = image.GetHeight();
			num = width*height;
			/*int nClrCount = image.GetBPP() / 8;
			BYTE * pImageData = (BYTE *)image.GetBits();
			int Pitch = image.GetPitch();*/

			

			//COLORREF a = image.GetPixel(0,273);

			for (int i = 0; i < num; i++)
				vec.push_back(image.GetPixel(i%width, i/width));

			std::vector<COLORREF> array;

			/*const int nPitch = image.GetPitch();

			const unsigned int nWidth = image.GetWidth();
			const unsigned int nHeight = image.GetHeight();
			register BYTE * pxPixel = reinterpret_cast<BYTE*>(image.GetBits());

			CBitmap* map = CBitmap::FromHandle(image);
			/*if (nClrCount == 3)
			{
				for (int i = 0; i < num-2; i++)
				{
					WORD hue, luminance, saturation;
					BYTE bRed, bGreen, bBlue;
					BYTE * p = pImageData + i;
					bRed = *p;
					bGreen = *(p + 1);
					bBlue = *(p + 2);
					COLORREF color = RGB(bRed, bGreen, bBlue);
					array.push_back(color);
					//ColorRGBToHLS(color, &hue, &luminance, &saturation);
				}
			}*/

			/*for (int i = 0; i < num; i += 2)
				vec128.push_back(get_m128i(i));
			one = _mm_set1_epi16(40);
			/*if((width>image.GetWidth())&&(height>image.GetHeight()))
				for (int i = 0; i < image.GetHeight(); i++)
					for (int j = 0; j < image.GetWidth(); j++)
						matrix[i][j] = image.GetPixel(i, j);
			else
			{
				if (matrix != nullptr)
				{
					for (int i = 0; i < height; i++)
						delete[] matrix[i];
					delete matrix;
				}

				matrix = new COLORREF*[image.GetHeight()];
					for (int i = 0; i < image.GetHeight(); i++)
						matrix[i] = new COLORREF[image.GetWidth()];

					for (int i = 0; i < image.GetHeight(); i++)
						for (int j = 0; j < image.GetWidth(); j++)
							matrix[i][j] = image.GetPixel(i, j);	
			}*/
			sliderBright->SetPos(50);
			sliderContr->SetPos(50);
			ready = true;
		}
	}
}


void CMFCApplication1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (ready)
	{
		if (sliderBright == (CSliderCtrl*)pScrollBar)
			changeBrigth();
		if (sliderContr == (CSliderCtrl*)pScrollBar)
			changeContrast();
	}
	//CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMFCApplication1Dlg::changeBrigth()
{
	int position = sliderBright->GetPos();
	int adjust = (position - prevPositionBrigh);
	prevPositionBrigh = position;
	for (int i = 0; i < num; i++)
	{
		COLORREF re = vec.at(i);
		int r = normalizeRGB(GetRValue(re) + adjust);
		int g = normalizeRGB(GetGValue(re) + adjust);
		int b = normalizeRGB(GetBValue(re) + adjust);
		COLORREF ref = RGB(r, g, b);
		vec.at(i) = RGB(r, g, b);
		image.SetPixel(i%width, i / width, vec.at(i));
	}	
	/*int i = 0;
	if (adjust!=0)
		if(adjust>0)
			for (auto it : vec128)
			{
				it = _mm_add_epi16(it, one);
				setpix_m128i(it, i);
				i += 2;
			}
		else
			for (auto it : vec128)
			{
				it = _mm_sub_epi16(it, one);
				setpix_m128i(it, i);
				i += 2;
			}*/
	

	pictureControl->SetBitmap((HBITMAP)image);
}


void CMFCApplication1Dlg::changeContrast()
{
	int position = sliderContr->GetPos();
	int adjust = position - prevPositionContr;
	prevPositionContr = position;
	double factor = (259.0 * (adjust + 255.0)) / (255.0 * (259.0 - adjust));

	for (int i = 0; i < num; i++)
	{
		COLORREF re = vec.at(i);
		int r = normalizeRGB((GetRValue(re) - 128) * factor + 128);
		int g = normalizeRGB((GetGValue(re) - 128) * factor + 128);
		int b = normalizeRGB((GetBValue(re) - 128) * factor + 128);
		vec.at(i) = RGB(r, g, b);
		image.SetPixel(i%width, i / width, vec.at(i));
	}

	pictureControl->SetBitmap((HBITMAP)image);
}

int CMFCApplication1Dlg::normalizeJ(int value)
{
	if (value >= height)
		return height-1;
	if (value < 0)
		return 0;
	return value;
}

int CMFCApplication1Dlg::normalizeI(int value)
{
	if (value >= width)
		return width-1;
	if (value < 0)
		return 0;
	return value;
}

__m128i CMFCApplication1Dlg::get_m128i(int i0)
{
	COLORREF a = image.GetPixel(i0%width, normalizeJ(i0 / width)),
		b = image.GetPixel((i0 + 1) % width, normalizeJ((i0 + 1) / width)),
		c = image.GetPixel((i0 + 2) % width, normalizeJ((i0 + 2) / width)),
		//d = image.GetPixel(normalizeI((i0 + 3) / width), normalizeJ((i0 + 3) % width))
		d = image.GetPixel((i0 + 3) % width, normalizeJ((i0 + 3) / width)),
		e = image.GetPixel((i0 + 4) % width, normalizeJ((i0 + 4) / width));
	uint8_t a0 = GetRValue(a),
		a1 = GetGValue(a),
		a2 = GetBValue(a),
		a3 = GetRValue(b),
		a4 = GetGValue(b),
		a5 = GetBValue(b),
		a6 = GetRValue(c),
		a7 = GetGValue(c),
		a8 = GetBValue(c),
		a9 = GetRValue(d),
		a10 = GetGValue(d),
		a11 = GetBValue(d),
		a12 = GetRValue(e),
		a13 = GetGValue(e),
		a14 = GetBValue(e);
	//return _mm_setr_epi8(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14, 0);
	return _mm_setr_epi16(a0, a1, a2, a3, a4, a5, 0, 0);
	//return _mm_setr_epi32(a, b, c, d);
}

void CMFCApplication1Dlg::setpix_m128i(__m128i it,int i0)
{
	/*image.SetPixel((i0) % width, normalizeJ((i0) / width), RGB(it.m128i_i8[0], it.m128i_i8[1], it.m128i_i8[2]));
	image.SetPixel((i0 + 1) % width, normalizeJ((i0 + 1) / width), RGB(it.m128i_i8[3], it.m128i_i8[4], it.m128i_i8[5]));
	image.SetPixel((i0 + 2) % width, normalizeJ((i0 + 2) / width), RGB(it.m128i_i8[6], it.m128i_i8[7], it.m128i_i8[8]));
	image.SetPixel((i0 + 3) % width, normalizeJ((i0 + 3) / width), RGB(it.m128i_i8[9], it.m128i_i8[10], it.m128i_i8[11]));
	image.SetPixel((i0 + 3) % width, normalizeJ((i0 + 3) / width), RGB(it.m128i_i8[12], it.m128i_i8[13], it.m128i_i8[14]));*/
	__int16 g = it.m128i_i16[0];
	COLORREF r = RGB(normalizeRGB(it.m128i_i16[0]), normalizeRGB(it.m128i_i16[1]), normalizeRGB(it.m128i_i16[2])),
		r2= RGB(normalizeRGB(it.m128i_i16[3]), normalizeRGB(it.m128i_i16[4]), normalizeRGB(it.m128i_i16[5]));
	image.SetPixel((i0) % width, normalizeJ((i0) / width), r);
	image.SetPixel((i0 + 1) % width, normalizeJ((i0 + 1) / width), r2);
}
