#include "GDI.h"

extern unsigned WindowWidth;
extern unsigned WindowHeight;
extern FLOAT m_dpi, beforeDpi;
//extern FLOAT m_dpiY;

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR pGdiToken;

extern unsigned shadowWidth;
unsigned inRoundD = 28;
unsigned outRoundD = 50;
unsigned nWidth;
unsigned nHeight;

unsigned LogicalToPhysical(unsigned pixel,
    FLOAT const dpi, FLOAT const beforeDpi)
{
    return (unsigned)pixel * dpi / beforeDpi;
}

void AddRoundRectange(Gdiplus::GraphicsPath& path, float left, float top, float right, float bottom, float diameterX, float diameterY)
{
    // ��Բ��
    //
    // ������Բ��
    path.AddArc(left, top, diameterX, diameterY, 180, 90);
    // ������Բ��
    path.AddArc(right - diameterX, top, diameterX, diameterY, 270, 90);
    // ������Բ��
    path.AddArc(right - diameterX, bottom - diameterY, diameterX, diameterY, 0, 90);
    // ������Բ��
    path.AddArc(left, bottom - diameterY, diameterX, diameterY, 90, 90);

    //
    // ��ֱ�ߣ�����4��Բ�ǣ�
    //�Զ�����������
    // ����������
    //path.AddLine(left + diameterX / 2, top, right - diameterX / 2, top);
    // ���Ҳ�����
    //path.AddLine(right, top + diameterY / 2, right, bottom - diameterY / 2);
    // ���ײ�����
    //path.AddLine(right - diameterX / 2, bottom, left + diameterX / 2, bottom);
    // ���������
    path.AddLine(left, bottom - diameterY / 2, left, top + diameterY / 2);
}

void gdiDrawShadow(Graphics& pGraphics)
{
    //����Graphics�����굥λΪ����
    pGraphics.SetPageUnit(UnitPixel);
    Gdiplus::GraphicsPath ShadowPath, WindowPath;       //��������·��

    AddRoundRectange(ShadowPath, 0, 0, nWidth, nHeight, outRoundD, outRoundD);
    //ShadowPath.SetFillMode(FillModeWinding);
    AddRoundRectange(WindowPath, shadowWidth + 1, shadowWidth + 1, nWidth - shadowWidth - 2, nHeight - shadowWidth - 2, inRoundD, inRoundD);

    //pGraphics.DrawPath(new Pen(Color(0xFF, 0xFF, 0x00, 0x00), 2), &ShadowPath);
    //������Ӱ������
    Gdiplus::Region ShadowRegion(&ShadowPath);
    Gdiplus::Region WindowRegion(&WindowPath);
    ShadowRegion.Exclude(&WindowRegion);      //�������

    // ��ʼ�����仭ˢ
    Gdiplus::PathGradientBrush pathBrush(&ShadowPath);
    pathBrush.SetCenterColor(Color(0xFF, 0x00, 0x00, 0x00)); // �������õ���·�����仭ˢ
    Color colors[] = { Color(0x00, 0x00, 0x00, 0x00) };
    int count = 1;
    pathBrush.SetSurroundColors(colors, &count);

    REAL fac[] = {
        0.0f,
        0.16f,     //SurroundColors to CenterColor
        0.9f,     //SurroundColors to CenterColor
        1.0f };
    REAL pos[] = {
        0.0f,
        0.3f,   //the boundary to the center
        0.7f,   //the boundary to the center
        1.0f };
    pathBrush.SetBlend(fac, pos, 4);

    //�Խ���Ч�����е�����ʹ�������Ȼ������ʵ�������ǶԽ���Ч���������š������Ǻ�����������������ű�����
    //pathBrush.SetFocusScales(0.55f, 0.55f);

    pGraphics.FillRegion(&pathBrush, &ShadowRegion);

    //delete& WindowPath; 
    //delete& ShadowPath;
}

void gdiPlusDraw(HWND hwnd)
{
    outRoundD = LogicalToPhysical(outRoundD, m_dpi, beforeDpi);
    inRoundD = LogicalToPhysical(inRoundD, m_dpi, beforeDpi);

    nWidth = WindowWidth + shadowWidth * 2;
    nHeight = WindowHeight + shadowWidth * 2;
    
    Gdiplus::RectF gdiRc(0, 0, nWidth, nHeight);

    HDC hdc = GetDC(hwnd);
    HDC memDC = CreateCompatibleDC(hdc);

    BITMAPINFO bitmapinfo;
    bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapinfo.bmiHeader.biBitCount = 32;
    bitmapinfo.bmiHeader.biHeight = nHeight;
    bitmapinfo.bmiHeader.biWidth = nWidth;
    bitmapinfo.bmiHeader.biPlanes = 1;
    bitmapinfo.bmiHeader.biCompression = BI_RGB;
    bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
    bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
    bitmapinfo.bmiHeader.biClrUsed = 0;
    bitmapinfo.bmiHeader.biClrImportant = 0;
    bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;

    HBITMAP hBmp = CreateDIBSection(hdc, &bitmapinfo, DIB_RGB_COLORS, NULL, NULL, 0);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);

    Gdiplus::Graphics pGraphics(memDC);

    pGraphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);      //ָ��������ݵĳ���

    gdiDrawShadow(pGraphics);

    BLENDFUNCTION blend;
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;

    SIZE sizeWnd = { nWidth, nHeight };
    POINT ptSrc = { 0, 0 };

    UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, memDC, &ptSrc, 0, &blend, ULW_ALPHA);
    //ShowWindow(hwnd, SW_SHOW);

    SelectObject(memDC, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(memDC);
    ReleaseDC(hwnd, hdc);
}