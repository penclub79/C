#include <Windows.h>

//int SetDIBitsToDevice(HDC hdc, int XDest, int YDest, DWORD dwWidth, DWORD dwHeight, int XSrc, int YSrc, UINT uStartScan, UINT cScanLines, CONST VOID *IpvBits, CONST BITMAPINFO *Ipbmi, UINT fuColorUse);
//
//
//BITMAPFILEHEADER *fh = NULL;
//BITMAPINFOHEADER *ih;
//int bx, by;
//BYTE *pRaster;
//
//void LoadDIB(char* Path)
//{
//	HANDLE hFile;
//	DWORD FileSize, dwRead;
//
//	hFile = CreateFile("UserImages.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile == INVALID_HANDLE_VALUE){ return; }
//	
//	FileSize = GetFileSize(hFile, NULL);
//	if (fh) { free(fh); }
//	fh = (BITMAPFILEHEADER *)malloc(GetFileSize);
//	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
//	CloseHandle(hFile);
//
//	pRaster = (PBYTE)fh + fh->bfOffBits;
//	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
//	bx = ih->biWidth;
//	by = ih->biHeight;
//
//}
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

//	HDC hdc;
//	PAINTSTRUCT ps;
//	OPENFILENAME OFN;
//	char lpstrFile[MAX_PATH] = "";
//
//	switch (iMessage) {
//	case WM_LBUTTONDOWN:
//		memset(&OFN, 0, sizeof(OPENFILENAME));
//		OFN.lStructSize = sizeof(OPENFILENAME);
//		OFN.hwndOwner = hWnd;
//		OFN.lpstrFilter = "Bitmap File (*.bmp) \0*.bmp\0";
//		OFN.lpstrFile = lpstrFile;
//		OFN.nMaxFile = 256;
//		if (GetOpenFileName(&OFN) != 0)
//		{
//			LoadDIB(lpstrFile);
//			InvalidateRect(hWnd, NULL, TRUE);
//		}
//		return 0;
//	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
//		if (fh)
//		{
//			SetDIBitsToDevice(hdc, 0, 0, bx, by, 0, 0, 0, by, pRaster, (BITMAPINFO *)ih, DIB_RGB_COLORS);
//		}
//		EndPaint(hWnd, &ps);
//		return 0;
//	case WM_DESTROY:
//		if (fh)
//			free(fh);
//		PostQuitMessage(0);
//		return 0;	
//	}
//
//	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
//
}

HBITMAP LoadBMP(void)
{
	HWND hwnd = GetDesktopWindow();
	LPVOID lpDIBits = 0;
	HANDLE hFile;
	hFile = CreateFile("bmpSample.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	BITMAPFILEHEADER bmFileHeader;
	LPBITMAPINFO lpBitmapInfo;
	LPBYTE pByte = 0;
	DWORD dwReadBytes;

	// 1. BITMAPFILEHEADER 구조체 만큼 파일에서 읽기
	ReadFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwReadBytes, NULL);

	// 2. DIB에 사용할 BITMAPINFO 구조체를 파일에서 읽기
	// BITMAPINFO는 BITMAPINFOHEADER + RGBQUAD의 구조체
	int iBitmapInfoSize = bmFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
	pByte = new BYTE[iBitmapInfoSize];
	ReadFile(hFile, pByte, iBitmapInfoSize, &dwReadBytes, NULL);

	// 3. pixel Data를 받아서 DIB에 복사
	HDC hdc = GetDC(hwnd);
	lpBitmapInfo = (LPBITMAPINFO)pByte;
	HBITMAP hDib = CreateDIBSection(hdc, lpBitmapInfo, DIB_RGB_COLORS, &lpDIBits, NULL, 0);
	ReadFile(hFile, lpDIBits, lpBitmapInfo->bmiHeader.biSizeImage, &dwReadBytes, NULL);
	ReleaseDC(hwnd, hdc);

	// 4. 마무리
	if (pByte) { delete[] pByte; pByte = 0; }
	if (hFile) { CloseHandle(hFile); hFile = 0; }

	return hDib;

}