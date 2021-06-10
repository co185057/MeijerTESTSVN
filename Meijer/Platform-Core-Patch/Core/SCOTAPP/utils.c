/**************************************************************************

    UTILS.C - useful functions for WinG Sample Apps

 **************************************************************************/
/**************************************************************************

    (C) Copyright 1994 Microsoft Corp.  All rights reserved.

    You have a royalty-free right to use, modify, reproduce and 
    distribute the Sample Files (and/or any modified version) in 
    any way you find useful, provided that you agree that 
    Microsoft has no warranty obligations or liability for any 
    Sample Application Files which are modified. 

 **************************************************************************/

/*----------------------------------------------------------------------------*\
|   Functions for handling Device Independent Bitmaps and clearing the         | 
|   System Palette.                                                            |
\*----------------------------------------------------------------------------*/

#include <windows.h>
#include <windowsx.h>
#include "Scotapputils.h"

#include <memory.h>             // for _fmemcpy()
#define _huge

#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

/* flags for _lseek */
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0


/*
 *  Clear the System Palette so that we can ensure an identity palette 
 *  mapping for fast performance.
 */

void ClearSystemPalette(void)
{
  //*** A dummy palette setup
  struct
  {
    WORD Version;
    WORD NumberOfEntries;
    PALETTEENTRY aEntries[256];
  } Palette =
  {
    0x300,
    256
  };

  HPALETTE ScreenPalette = 0;
  HDC ScreenDC;
  int Counter;
  UINT nMapped = 0;
  BOOL bOK = FALSE;
  int  nOK = 0;
  
  //*** Reset everything in the system palette to black
  for(Counter = 0; Counter < 256; Counter++)
  {
    Palette.aEntries[Counter].peRed = 0;
    Palette.aEntries[Counter].peGreen = 0;
    Palette.aEntries[Counter].peBlue = 0;
    Palette.aEntries[Counter].peFlags = PC_NOCOLLAPSE;
  }

  //*** Create, select, realize, deselect, and delete the palette
  ScreenDC = GetDC(NULL);
  ScreenPalette = CreatePalette((LOGPALETTE *)&Palette);

  if (ScreenPalette)
  {
    ScreenPalette = SelectPalette(ScreenDC,ScreenPalette,FALSE);
    nMapped = RealizePalette(ScreenDC);
    ScreenPalette = SelectPalette(ScreenDC,ScreenPalette,FALSE);
    bOK = DeleteObject(ScreenPalette);
  }

  nOK = ReleaseDC(NULL, ScreenDC);

  return;
}


/*
 *   Open a DIB file and return a MEMORY DIB, a memory handle containing..
 *
 *   BITMAP INFO    bi
 *   palette data
 *   bits....
 */

PDIB DibOpenFile(LPCTSTR szFile)
{
    HFILE               fh;
    DWORD               dwLen;
    DWORD               dwBits;
    PDIB                pdib;
    LPVOID              p;
    OFSTRUCT            of;


//    fh = OpenFile(szFile, &of, OF_READ);  
    fh = CreateFile(szFile, 
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);

    if (fh == -1)
    {
        HRSRC h;

        h = FindResource(GetModuleHandle(NULL), szFile, RT_BITMAP);

        //!!! can we call GlobalFree() on this? is it the right format.
        //!!! can we write to this resource?
        if (h)
            return (PDIB)LockResource(LoadResource(GetModuleHandle(NULL), h));
    }

    if (fh == -1)
        return NULL;

    pdib = DibReadBitmapInfo(fh);

    if (!pdib)
        return NULL;

    /* How much memory do we need to hold the DIB */

    dwBits = pdib->biSizeImage;
    dwLen  = pdib->biSize + DibPaletteSize(pdib) + dwBits;

    /* Can we get more memory? */

    p = GlobalReAllocPtr(pdib,dwLen,0);

    if (!p)
    {
        GlobalFreePtr(pdib);
        pdib = NULL;
    }
    else
    {
        pdib = (PDIB)p;
    }

    if (pdib)
    {
        /* read in the bits */
        _hread(fh, (LPBYTE)pdib + (UINT)pdib->biSize + DibPaletteSize(pdib), dwBits);
    }

    _lclose(fh);

    return pdib;
}


/*
 *  ReadDibBitmapInfo()
 *
 *  Will read a file in DIB format and return a global HANDLE to its
 *  BITMAPINFO.  This function will work with both "old" and "new"
 *  bitmap formats, but will always return a "new" BITMAPINFO.
 */

PDIB DibReadBitmapInfo(HFILE fh)
{
    DWORD     off;
    HANDLE    hbi = NULL;
    int       size;
    int       i;
    int       nNumColors;

    RGBQUAD FAR       *pRgb;
    BITMAPINFOHEADER   bi;
    BITMAPCOREHEADER   bc;
    BITMAPFILEHEADER   bf;
    PDIB               pdib;

    if (fh == -1)
        return NULL;

    off = _llseek(fh,0L,SEEK_CUR);

    if (sizeof(bf) != _lread(fh,(LPTSTR)&bf,sizeof(bf)))
        return FALSE;

    /*
     *  do we have a RC HEADER?
     */
    if (bf.bfType != BFT_BITMAP)
    {
        bf.bfOffBits = 0L;
        _llseek(fh,off,SEEK_SET);
    }

    if (sizeof(bi) != _lread(fh,(LPTSTR)&bi,sizeof(bi)))
        return FALSE;

    /*
     *  what type of bitmap info is this?
     */
    switch (size = (int)bi.biSize)
    {
        default:
        case sizeof(BITMAPINFOHEADER):
            break;

        case sizeof(BITMAPCOREHEADER):
            bc = *(BITMAPCOREHEADER*)&bi;
            bi.biSize               = sizeof(BITMAPINFOHEADER);
            bi.biWidth              = (DWORD)bc.bcWidth;
            bi.biHeight             = (DWORD)bc.bcHeight;
            bi.biPlanes             =  (UINT)bc.bcPlanes;
            bi.biBitCount           =  (UINT)bc.bcBitCount;
            bi.biCompression        = BI_RGB;
            bi.biSizeImage          = 0;
            bi.biXPelsPerMeter      = 0;
            bi.biYPelsPerMeter      = 0;
            bi.biClrUsed            = 0;
            bi.biClrImportant       = 0;

            _llseek(fh,(LONG)sizeof(BITMAPCOREHEADER)-sizeof(BITMAPINFOHEADER),SEEK_CUR);

            break;
    }

    nNumColors = DibNumColors(&bi);

#if 0
    if (bi.biSizeImage == 0)
        bi.biSizeImage = DibSizeImage(&bi);

    if (bi.biClrUsed == 0)
        bi.biClrUsed = DibNumColors(&bi);
#else
    FixBitmapInfo(&bi);
#endif

    pdib = (PDIB)GlobalAllocPtr(GMEM_MOVEABLE,(LONG)bi.biSize + nNumColors * sizeof(RGBQUAD));

    if (!pdib)
        return NULL;

    *pdib = bi;

    pRgb = DibColors(pdib);

    if (nNumColors)
    {
        if (size == sizeof(BITMAPCOREHEADER))
        {
            /*
             * convert a old color table (3 byte entries) to a new
             * color table (4 byte entries)
             */
            _lread(fh,(LPVOID)pRgb,nNumColors * sizeof(RGBTRIPLE));

            for (i=nNumColors-1; i>=0; i--)
            {
                RGBQUAD rgb;

                rgb.rgbRed      = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
                rgb.rgbBlue     = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
                rgb.rgbGreen    = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
                rgb.rgbReserved = (BYTE)0;

                pRgb[i] = rgb;
            }
        }
        else
        {
            _lread(fh,(LPVOID)pRgb,nNumColors * sizeof(RGBQUAD));
        }
    }

    if (bf.bfOffBits != 0L)
        _llseek(fh,off + bf.bfOffBits,SEEK_SET);

    return pdib;
}

/*
 *  DibSetUsage(hdib,hpal,wUsage)
 *
 *  Modifies the color table of the passed DIB for use with the wUsage
 *  parameter specifed.
 *
 *  if wUsage is DIB_PAL_COLORS the DIB color table is set to 0-256
 *  if wUsage is DIB_RGB_COLORS the DIB color table is set to the RGB values
 *      in the passed palette
 */

BOOL DibSetUsage(PDIB pdib, HPALETTE hpal,UINT wUsage)
{
    PALETTEENTRY       ape[256];
    RGBQUAD FAR *      pRgb;
    WORD FAR *         pw;
    int                nColors;
    int                n;

    if (hpal == NULL)
        hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

    if (!pdib)
        return FALSE;

    nColors = DibNumColors(pdib);
    
    if (nColors == 3 && DibCompression(pdib) == BI_BITFIELDS)
        nColors = 0;

    if (nColors > 0)
    {
        pRgb = DibColors(pdib);

        switch (wUsage)
        {
            //
            // Set the DIB color table to palette indexes
            //
            case DIB_PAL_COLORS:
                for (pw = (WORD FAR*)pRgb,n=0; n<nColors; n++,pw++)
                    *pw = n;
                break;

            //
            // Set the DIB color table to RGBQUADS
            //
            default:
            case DIB_RGB_COLORS:
                nColors = min(nColors,256);

                GetPaletteEntries(hpal,0,nColors,ape);

                for (n=0; n<nColors; n++)
                {
                    pRgb[n].rgbRed      = ape[n].peRed;
                    pRgb[n].rgbGreen    = ape[n].peGreen;
                    pRgb[n].rgbBlue     = ape[n].peBlue;
                    pRgb[n].rgbReserved = 0;
                }
                break;
        }
    }
    return TRUE;
}

/*
 *  DibCreate(bits, dx, dy)
 *
 *  Creates a new packed DIB with the given dimensions and the
 *  given number of bits per pixel
 */

PDIB DibCreate(int bits, int dx, int dy)
{
    LPBITMAPINFOHEADER lpbi ;
    DWORD       dwSizeImage;
    int         i;
    DWORD FAR  *pdw;

    dwSizeImage = dy*(DWORD)((dx*bits/8+3)&~3);

    lpbi = (PDIB)GlobalAllocPtr(GHND,sizeof(BITMAPINFOHEADER)+dwSizeImage + 1024);

    if (lpbi == NULL)
        return NULL;

    lpbi->biSize            = sizeof(BITMAPINFOHEADER) ;
    lpbi->biWidth           = dx;
    lpbi->biHeight          = dy;
    lpbi->biPlanes          = 1;
    lpbi->biBitCount        = bits ;
    lpbi->biCompression     = BI_RGB ;
    lpbi->biSizeImage       = dwSizeImage;
    lpbi->biXPelsPerMeter   = 0 ;
    lpbi->biYPelsPerMeter   = 0 ;
    lpbi->biClrUsed         = 0 ;
    lpbi->biClrImportant    = 0 ;

    if (bits == 4)
        lpbi->biClrUsed = 16;

    else if (bits == 8)
        lpbi->biClrUsed = 256;

    pdw = (DWORD FAR *)((LPBYTE)lpbi+(int)lpbi->biSize);

    for (i=0; i<(int)lpbi->biClrUsed/16; i++)
    {
        *pdw++ = 0x00000000;    // 0000  black
        *pdw++ = 0x00800000;    // 0001  dark red
        *pdw++ = 0x00008000;    // 0010  dark green
        *pdw++ = 0x00808000;    // 0011  mustard
        *pdw++ = 0x00000080;    // 0100  dark blue
        *pdw++ = 0x00800080;    // 0101  purple
        *pdw++ = 0x00008080;    // 0110  dark turquoise
        *pdw++ = 0x00C0C0C0;    // 1000  gray
        *pdw++ = 0x00808080;    // 0111  dark gray
        *pdw++ = 0x00FF0000;    // 1001  red
        *pdw++ = 0x0000FF00;    // 1010  green
        *pdw++ = 0x00FFFF00;    // 1011  yellow
        *pdw++ = 0x000000FF;    // 1100  blue
        *pdw++ = 0x00FF00FF;    // 1101  pink (magenta)
        *pdw++ = 0x0000FFFF;    // 1110  cyan
        *pdw++ = 0x00FFFFFF;    // 1111  white
    }

    return (PDIB)lpbi;
}

static void xlatClut8(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{
    DWORD dw;

#ifdef __cplusplus
    for (dw = 0; dw < dwSize; dw++, ((BYTE  *&)pb)++)
#else
    for (dw = 0; dw < dwSize; dw++, ((BYTE  *)pb)++)
#endif
        *pb = xlat[*pb];
}

static void xlatClut4(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{
    DWORD dw;

#ifdef __cplusplus
    for (dw = 0; dw < dwSize; dw++, ((BYTE  *&)pb)++)
#else
    for (dw = 0; dw < dwSize; dw++, ((BYTE  *)pb)++)
#endif
        *pb = (BYTE)(xlat[*pb & 0x0F] | (xlat[(*pb >> 4) & 0x0F] << 4));
}

#define RLE_ESCAPE  0
#define RLE_EOL     0
#define RLE_EOF     1
#define RLE_JMP     2

static void xlatRle8(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{
    BYTE    cnt;
    BYTE    b;
    BYTE  *prle = pb;

    for(;;)
    {
        cnt = *prle++;
        b   = *prle;

        if (cnt == RLE_ESCAPE)
        {
            prle++;

            switch (b)
            {
                case RLE_EOF:
                    return;

                case RLE_EOL:
                    break;

                case RLE_JMP:
                    prle++;     // skip dX
                    prle++;     // skip dY
                    break;


                default:
                    cnt = b;
                    for (b=0; b<cnt; b++,prle++)
                        *prle = xlat[*prle];

                    if (cnt & 1)
                        prle++;

                    break;
            }
        }
        else
        {
            *prle++ = xlat[b];
        }
    }
}

static void xlatRle4(BYTE FAR *pb, DWORD dwSize, BYTE FAR *xlat)
{
}

static void hmemmove(BYTE  *d, BYTE  *s, LONG len)
{
    d += len-1;
    s += len-1;

    while (len--)
        *d-- = *s--;
}

/*
 *  DibMapToPalette(pdib, hpal)
 *
 *  Map the colors of the DIB, using GetNearestPaletteIndex, to
 *  the colors of the given palette.
 */

void DibMapToPalette(PDIB pdib, HPALETTE hpal)
{
    LPBITMAPINFOHEADER  lpbi;
    PALETTEENTRY        pe;
    int                 n;
    int                 nDibColors;
    int                 nPalColors=0;
    BYTE FAR *          lpBits;
    RGBQUAD FAR *       lpRgb;
    BYTE                xlat[256];
    DWORD               SizeImage;

    if (!hpal || !pdib)
        return;

    lpbi   = (LPBITMAPINFOHEADER)pdib;
    lpRgb  = DibColors(pdib);

    GetObject(hpal,sizeof(int),(LPTSTR)&nPalColors);
    nDibColors = DibNumColors(pdib);

    if ((SizeImage = lpbi->biSizeImage) == 0)
        SizeImage = DibSizeImage(lpbi);

    //
    //  build a xlat table. from the current DIB colors to the given
    //  palette.
    //
    for (n=0; n<nDibColors; n++)
        xlat[n] = (BYTE)GetNearestPaletteIndex(hpal,RGB(lpRgb[n].rgbRed,lpRgb[n].rgbGreen,lpRgb[n].rgbBlue));

    lpBits = (LPBYTE)DibPtr(lpbi);
    lpbi->biClrUsed = nPalColors;

    //
    // re-size the DIB
    //
    if (nPalColors > nDibColors)
    {
        GlobalReAllocPtr(lpbi, lpbi->biSize + nPalColors*sizeof(RGBQUAD) + SizeImage, 0);
        hmemmove((BYTE  *)DibPtr(lpbi), (BYTE  *)lpBits, SizeImage);
        lpBits = (LPBYTE)DibPtr(lpbi);
    }
    else if (nPalColors < nDibColors)
    {
        memcpy(DibPtr(lpbi), lpBits, SizeImage);
        GlobalReAllocPtr(lpbi, lpbi->biSize + nPalColors*sizeof(RGBQUAD) + SizeImage, 0);
        lpBits = (LPBYTE)DibPtr(lpbi);
    }

    //
    // translate the DIB bits
    //
    switch (lpbi->biCompression)
    {
        case BI_RLE8:
            xlatRle8(lpBits, SizeImage, xlat);
            break;

        case BI_RLE4:
            xlatRle4(lpBits, SizeImage, xlat);
            break;

        case BI_RGB:
            if (lpbi->biBitCount == 8)
                xlatClut8(lpBits, SizeImage, xlat);
            else
                xlatClut4(lpBits, SizeImage, xlat);
            break;
    }

    //
    //  Now copy the RGBs in the logical palette to the dib color table
    //
    for (n=0; n<nPalColors; n++)
    {
        GetPaletteEntries(hpal,n,1,&pe);

        lpRgb[n].rgbRed      = pe.peRed;
        lpRgb[n].rgbGreen    = pe.peGreen;
        lpRgb[n].rgbBlue     = pe.peBlue;
        lpRgb[n].rgbReserved = (BYTE)0;
    }

    return;
}

/*<<<DibToScreenDdb>>>---------------------------------------------------------

 This is a new function added for NICE (i.e. it was not in the original sample
 code.  It takes a device-independent bitmap (DIB) and returns a handle to a
 device-dependent bitmap (DDB) that is compatible with the current display
 screen.  The caller is responsible for freeing the DDB bitmap by calling
 DeleteObject on the returned bitmap handle.

 Taken from NICEClient 1.3 NICECMN library
-----------------------------------------------------------------------------*/
HBITMAP DibToScreenDdb(PDIB lpDib)
{
    HWND hwndDesktop;
    HDC hdcScreen, hdcMemory;
    HBITMAP hBitmap;
    HBITMAP hbmpOld;
    UINT bmpWidth, bmpHeight;

    /* make sure that the DIB pointer is non-NULL and that at least the
     * first byte is readable */
    if (IsBadReadPtr(lpDib, 1))
        return NULL;

    /* get a DC that matches the current display screen */
    hwndDesktop = GetDesktopWindow();
    hdcScreen = GetWindowDC(hwndDesktop);
    if (!hdcScreen)
        /* couldn't get a window DC for the screen, so return failure */
        return NULL;
            
    /* create a memory DC to match the screen */
    hdcMemory = CreateCompatibleDC(hdcScreen);
    if (!hdcMemory)
    {
        /* couldn't create memory DC, so free the screen DC and return
         * failure */
        ReleaseDC(hwndDesktop, hdcScreen);
        return NULL;
    }
              
    /* create a bitmap to match the screen with the same size as the DIB */
    bmpWidth = DibWidth(lpDib);
    bmpHeight = DibHeight(lpDib);
    hBitmap = CreateCompatibleBitmap(hdcScreen, bmpWidth, bmpHeight);
    
    /* release the screen DC */
    ReleaseDC(hwndDesktop, hdcScreen);
            
    if (!hBitmap)
    {
        /* could not create the device-dependent bitmap, so free the memory
         * DC and return failure */
        DeleteDC(hdcMemory);
        return NULL;
    }
            
    /* select the device-dependent bitmap into the memory DC */
    hbmpOld = SelectObject(hdcMemory, hBitmap);
    
		/* set the DIB bits into the device-dependent bitmap */
    SetDIBitsToDevice(hdcMemory, 0, 0, bmpWidth, bmpHeight, 0, 0,
                      0, bmpHeight, DibPtr(lpDib),
                      (LPBITMAPINFO)lpDib, DIB_RGB_COLORS);
            
    /* free up the memory DC */
    SelectObject(hdcMemory, hbmpOld);
    DeleteDC(hdcMemory);

    /* return the handle of the new device-dependent bitmap */
    return hBitmap;
}

/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPTSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function _getts the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
 * style DIB).
 *
 ************************************************************************/


WORD  PaletteSize(LPTSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}


/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPTSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/


WORD DIBNumColors(LPTSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

