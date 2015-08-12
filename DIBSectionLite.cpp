// CDIBSectionLite.cpp : implementation file
//
// General purpose DIBsection wrapper class for Win9x, NT 4.0, W2K and WinCE.
//
// Author      : Chris Maunder (cmaunder@mail.com)
// Date        : 17 May 1999
//
// Copyright © Dundas Software Ltd. 1999, All Rights Reserved                      
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage, in any form, caused
// by this code. Use it at your own risk and as with all code expect bugs!
// It's been tested but I'm not perfect.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file.
//
// History :  25 May 1999 - First release
//             4 Jun 1999 - Fixed SetBitmap bug
//             4 May 2000 - 16 or 32 bit compression bug fix (Jim Miller <jam@3dfx.com>)
//            20 Sep 2000 - Bug fix in Save() (saving 4 bytes too many - Tadeusz Dracz)
//                          Allowed lpBits to be NULL in SetBitmap - Don Grasberger
//                          Fixed NumColorEntries bug - Itay Szekely
//                          Fixed buffer overrun bug in Load() - Itay Szekely
//             5 Apr 2000 - Fix for 16/32 bpp bitmaps for PocketPC's (Dan Robbins - DCR)

#include "stdafx.h"
#include "DIBSectionLite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Use C++ exception handling instead of structured on non-CE platforms.
#ifndef _WIN32_WCE  
#undef TRY
#undef CATCH
#undef END_CATCH
#define TRY try
#define CATCH(ex_class, ex_object) catch(ex_class* ex_object)
#define END_CATCH
#endif  // _WIN32_WCE

// Spit out some messages as a sanity check for programmers
#ifdef DIBSECTION_NO_DITHER
#pragma message(" - CDIBSectionLite: No dithering")
#endif
#ifdef DIBSECTION_NO_MEMDC_REUSE
#pragma message(" - CDIBSectionLite: No memory DC reuse")
#endif
#ifdef DIBSECTION_NO_PALETTE
#pragma message(" - CDIBSectionLite: No palette support")
#endif

// Standard colors
RGBQUAD CDIBSectionLite::ms_StdColors[] = {
    { 0x00, 0x00, 0x00, 0 },  // System palette - first 10 colors
    { 0x80, 0x00, 0x00, 0 },
    { 0x00, 0x80, 0x00, 0 },
    { 0x80, 0x80, 0x00, 0 },
    { 0x00, 0x00, 0x80, 0 },
    { 0x80, 0x00, 0x80, 0 },
    { 0x00, 0x80, 0x80, 0 },
    { 0xC0, 0xC0, 0xC0, 0 },
    { 0xC0, 0xDC, 0xC0, 0 },
    { 0xA6, 0xCA, 0xF0, 0 },

    { 0x2C, 0x00, 0x00, 0 },
    { 0x56, 0x00, 0x00, 0 },
    { 0x87, 0x00, 0x00, 0 },
    { 0xC0, 0x00, 0x00, 0 },
    { 0xFF, 0x00, 0x00, 0 },
    { 0x00, 0x2C, 0x00, 0 },
    { 0x2C, 0x2C, 0x00, 0 },
    { 0x56, 0x2C, 0x00, 0 },
    { 0x87, 0x2C, 0x00, 0 },
    { 0xC0, 0x2C, 0x00, 0 },
    { 0xFF, 0x2C, 0x00, 0 },
    { 0x00, 0x56, 0x00, 0 },
    { 0x2C, 0x56, 0x00, 0 },
    { 0x56, 0x56, 0x00, 0 },
    { 0x87, 0x56, 0x00, 0 },
    { 0xC0, 0x56, 0x00, 0 },
    { 0xFF, 0x56, 0x00, 0 },
    { 0x00, 0x87, 0x00, 0 },
    { 0x2C, 0x87, 0x00, 0 },
    { 0x56, 0x87, 0x00, 0 },
    { 0x87, 0x87, 0x00, 0 },
    { 0xC0, 0x87, 0x00, 0 },
    { 0xFF, 0x87, 0x00, 0 },
    { 0x00, 0xC0, 0x00, 0 },
    { 0x2C, 0xC0, 0x00, 0 },
    { 0x56, 0xC0, 0x00, 0 },
    { 0x87, 0xC0, 0x00, 0 },
    { 0xC0, 0xC0, 0x00, 0 },
    { 0xFF, 0xC0, 0x00, 0 },
    { 0x00, 0xFF, 0x00, 0 },
    { 0x2C, 0xFF, 0x00, 0 },
    { 0x56, 0xFF, 0x00, 0 },
    { 0x87, 0xFF, 0x00, 0 },
    { 0xC0, 0xFF, 0x00, 0 },
    { 0xFF, 0xFF, 0x00, 0 },
    { 0x00, 0x00, 0x2C, 0 },
    { 0x2C, 0x00, 0x2C, 0 },
    { 0x56, 0x00, 0x2C, 0 },
    { 0x87, 0x00, 0x2C, 0 },
    { 0xC0, 0x00, 0x2C, 0 },
    { 0xFF, 0x00, 0x2C, 0 },
    { 0x00, 0x2C, 0x2C, 0 },
    { 0x2C, 0x2C, 0x2C, 0 },
    { 0x56, 0x2C, 0x2C, 0 },
    { 0x87, 0x2C, 0x2C, 0 },
    { 0xC0, 0x2C, 0x2C, 0 },
    { 0xFF, 0x2C, 0x2C, 0 },
    { 0x00, 0x56, 0x2C, 0 },
    { 0x2C, 0x56, 0x2C, 0 },
    { 0x56, 0x56, 0x2C, 0 },
    { 0x87, 0x56, 0x2C, 0 },
    { 0xC0, 0x56, 0x2C, 0 },
    { 0xFF, 0x56, 0x2C, 0 },
    { 0x00, 0x87, 0x2C, 0 },
    { 0x2C, 0x87, 0x2C, 0 },
    { 0x56, 0x87, 0x2C, 0 },
    { 0x87, 0x87, 0x2C, 0 },
    { 0xC0, 0x87, 0x2C, 0 },
    { 0xFF, 0x87, 0x2C, 0 },
    { 0x00, 0xC0, 0x2C, 0 },
    { 0x2C, 0xC0, 0x2C, 0 },
    { 0x56, 0xC0, 0x2C, 0 },
    { 0x87, 0xC0, 0x2C, 0 },
    { 0xC0, 0xC0, 0x2C, 0 },
    { 0xFF, 0xC0, 0x2C, 0 },
    { 0x00, 0xFF, 0x2C, 0 },
    { 0x2C, 0xFF, 0x2C, 0 },
    { 0x56, 0xFF, 0x2C, 0 },
    { 0x87, 0xFF, 0x2C, 0 },
    { 0xC0, 0xFF, 0x2C, 0 },
    { 0xFF, 0xFF, 0x2C, 0 },
    { 0x00, 0x00, 0x56, 0 },
    { 0x2C, 0x00, 0x56, 0 },
    { 0x56, 0x00, 0x56, 0 },
    { 0x87, 0x00, 0x56, 0 },
    { 0xC0, 0x00, 0x56, 0 },
    { 0xFF, 0x00, 0x56, 0 },
    { 0x00, 0x2C, 0x56, 0 },
    { 0x2C, 0x2C, 0x56, 0 },
    { 0x56, 0x2C, 0x56, 0 },
    { 0x87, 0x2C, 0x56, 0 },
    { 0xC0, 0x2C, 0x56, 0 },
    { 0xFF, 0x2C, 0x56, 0 },
    { 0x00, 0x56, 0x56, 0 },
    { 0x2C, 0x56, 0x56, 0 },
    { 0x56, 0x56, 0x56, 0 },
    { 0x87, 0x56, 0x56, 0 },
    { 0xC0, 0x56, 0x56, 0 },
    { 0xFF, 0x56, 0x56, 0 },
    { 0x00, 0x87, 0x56, 0 },
    { 0x2C, 0x87, 0x56, 0 },
    { 0x56, 0x87, 0x56, 0 },
    { 0x87, 0x87, 0x56, 0 },
    { 0xC0, 0x87, 0x56, 0 },
    { 0xFF, 0x87, 0x56, 0 },
    { 0x00, 0xC0, 0x56, 0 },
    { 0x2C, 0xC0, 0x56, 0 },
    { 0x56, 0xC0, 0x56, 0 },
    { 0x87, 0xC0, 0x56, 0 },
    { 0xC0, 0xC0, 0x56, 0 },
    { 0xFF, 0xC0, 0x56, 0 },
    { 0x00, 0xFF, 0x56, 0 },
    { 0x2C, 0xFF, 0x56, 0 },
    { 0x56, 0xFF, 0x56, 0 },
    { 0x87, 0xFF, 0x56, 0 },
    { 0xC0, 0xFF, 0x56, 0 },
    { 0xFF, 0xFF, 0x56, 0 },
    { 0x00, 0x00, 0x87, 0 },
    { 0x2C, 0x00, 0x87, 0 },
    { 0x56, 0x00, 0x87, 0 },
    { 0x87, 0x00, 0x87, 0 },
    { 0xC0, 0x00, 0x87, 0 },
    { 0xFF, 0x00, 0x87, 0 },
    { 0x00, 0x2C, 0x87, 0 },
    { 0x2C, 0x2C, 0x87, 0 },
    { 0x56, 0x2C, 0x87, 0 },
    { 0x87, 0x2C, 0x87, 0 },
    { 0xC0, 0x2C, 0x87, 0 },
    { 0xFF, 0x2C, 0x87, 0 },
    { 0x00, 0x56, 0x87, 0 },
    { 0x2C, 0x56, 0x87, 0 },
    { 0x56, 0x56, 0x87, 0 },
    { 0x87, 0x56, 0x87, 0 },
    { 0xC0, 0x56, 0x87, 0 },
    { 0xFF, 0x56, 0x87, 0 },
    { 0x00, 0x87, 0x87, 0 },
    { 0x2C, 0x87, 0x87, 0 },
    { 0x56, 0x87, 0x87, 0 },
    { 0x87, 0x87, 0x87, 0 },
    { 0xC0, 0x87, 0x87, 0 },
    { 0xFF, 0x87, 0x87, 0 },
    { 0x00, 0xC0, 0x87, 0 },
    { 0x2C, 0xC0, 0x87, 0 },
    { 0x56, 0xC0, 0x87, 0 },
    { 0x87, 0xC0, 0x87, 0 },
    { 0xC0, 0xC0, 0x87, 0 },
    { 0xFF, 0xC0, 0x87, 0 },
    { 0x00, 0xFF, 0x87, 0 },
    { 0x2C, 0xFF, 0x87, 0 },
    { 0x56, 0xFF, 0x87, 0 },
    { 0x87, 0xFF, 0x87, 0 },
    { 0xC0, 0xFF, 0x87, 0 },
    { 0xFF, 0xFF, 0x87, 0 },
    { 0x00, 0x00, 0xC0, 0 },
    { 0x2C, 0x00, 0xC0, 0 },
    { 0x56, 0x00, 0xC0, 0 },
    { 0x87, 0x00, 0xC0, 0 },
    { 0xC0, 0x00, 0xC0, 0 },
    { 0xFF, 0x00, 0xC0, 0 },
    { 0x00, 0x2C, 0xC0, 0 },
    { 0x2C, 0x2C, 0xC0, 0 },
    { 0x56, 0x2C, 0xC0, 0 },
    { 0x87, 0x2C, 0xC0, 0 },
    { 0xC0, 0x2C, 0xC0, 0 },
    { 0xFF, 0x2C, 0xC0, 0 },
    { 0x00, 0x56, 0xC0, 0 },
    { 0x2C, 0x56, 0xC0, 0 },
    { 0x56, 0x56, 0xC0, 0 },
    { 0x87, 0x56, 0xC0, 0 },
    { 0xC0, 0x56, 0xC0, 0 },
    { 0xFF, 0x56, 0xC0, 0 },
    { 0x00, 0x87, 0xC0, 0 },
    { 0x2C, 0x87, 0xC0, 0 },
    { 0x56, 0x87, 0xC0, 0 },
    { 0x87, 0x87, 0xC0, 0 },
    { 0xC0, 0x87, 0xC0, 0 },
    { 0xFF, 0x87, 0xC0, 0 },
    { 0x00, 0xC0, 0xC0, 0 },
    { 0x2C, 0xC0, 0xC0, 0 },
    { 0x56, 0xC0, 0xC0, 0 },
    { 0x87, 0xC0, 0xC0, 0 },
    { 0xFF, 0xC0, 0xC0, 0 },
    { 0x00, 0xFF, 0xC0, 0 },
    { 0x2C, 0xFF, 0xC0, 0 },
    { 0x56, 0xFF, 0xC0, 0 },
    { 0x87, 0xFF, 0xC0, 0 },
    { 0xC0, 0xFF, 0xC0, 0 },
    { 0xFF, 0xFF, 0xC0, 0 },
    { 0x00, 0x00, 0xFF, 0 },
    { 0x2C, 0x00, 0xFF, 0 },
    { 0x56, 0x00, 0xFF, 0 },
    { 0x87, 0x00, 0xFF, 0 },
    { 0xC0, 0x00, 0xFF, 0 },
    { 0xFF, 0x00, 0xFF, 0 },
    { 0x00, 0x2C, 0xFF, 0 },
    { 0x2C, 0x2C, 0xFF, 0 },
    { 0x56, 0x2C, 0xFF, 0 },
    { 0x87, 0x2C, 0xFF, 0 },
    { 0xC0, 0x2C, 0xFF, 0 },
    { 0xFF, 0x2C, 0xFF, 0 },
    { 0x00, 0x56, 0xFF, 0 },
    { 0x2C, 0x56, 0xFF, 0 },
    { 0x56, 0x56, 0xFF, 0 },
    { 0x87, 0x56, 0xFF, 0 },
    { 0xC0, 0x56, 0xFF, 0 },
    { 0xFF, 0x56, 0xFF, 0 },
    { 0x00, 0x87, 0xFF, 0 },
    { 0x2C, 0x87, 0xFF, 0 },
    { 0x56, 0x87, 0xFF, 0 },
    { 0x87, 0x87, 0xFF, 0 },
    { 0xC0, 0x87, 0xFF, 0 },
    { 0xFF, 0x87, 0xFF, 0 },
    { 0x00, 0xC0, 0xFF, 0 },
    { 0x2C, 0xC0, 0xFF, 0 },
    { 0x56, 0xC0, 0xFF, 0 },
    { 0x87, 0xC0, 0xFF, 0 },
    { 0xC0, 0xC0, 0xFF, 0 },
    { 0xFF, 0xC0, 0xFF, 0 },
    { 0x2C, 0xFF, 0xFF, 0 },
    { 0x56, 0xFF, 0xFF, 0 },
    { 0x87, 0xFF, 0xFF, 0 },
    { 0xC0, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0 },
    { 0x11, 0x11, 0x11, 0 },
    { 0x18, 0x18, 0x18, 0 },
    { 0x1E, 0x1E, 0x1E, 0 },
    { 0x25, 0x25, 0x25, 0 },
    { 0x2C, 0x2C, 0x2C, 0 },
    { 0x34, 0x34, 0x34, 0 },
    { 0x3C, 0x3C, 0x3C, 0 },
    { 0x44, 0x44, 0x44, 0 },
    { 0x4D, 0x4D, 0x4D, 0 },
    { 0x56, 0x56, 0x56, 0 },
    { 0x5F, 0x5F, 0x5F, 0 },
    { 0x69, 0x69, 0x69, 0 },
    { 0x72, 0x72, 0x72, 0 },
    { 0x7D, 0x7D, 0x7D, 0 },
    { 0x92, 0x92, 0x92, 0 },
    { 0x9D, 0x9D, 0x9D, 0 },
    { 0xA8, 0xA8, 0xA8, 0 },
    { 0xB4, 0xB4, 0xB4, 0 },
    { 0xCC, 0xCC, 0xCC, 0 },
    { 0xD8, 0xD8, 0xD8, 0 },
    { 0xE5, 0xE5, 0xE5, 0 },
    { 0xF2, 0xF2, 0xF2, 0 },
    { 0xFF, 0xFF, 0xFF, 0 },

    { 0xFF, 0xFB, 0xF0, 0 },  // System palette - last 10 colors
    { 0xA0, 0xA0, 0xA4, 0 },
    { 0x80, 0x80, 0x80, 0 },
    { 0xFF, 0x00, 0x00, 0 },
    { 0x00, 0xFF, 0x00, 0 },
    { 0xFF, 0xFF, 0x00, 0 },
    { 0x00, 0x00, 0xFF, 0 },
    { 0xFF, 0x00, 0xFF, 0 },
    { 0x00, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0 },
};
/////////////////////////////////////////////////////////////////////////////
// CE DIBSection global functions

#ifdef _WIN32_WCE
UINT CEGetDIBColorTable(HDC hdc, UINT uStartIndex, UINT cEntries, RGBQUAD *pColors);
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIBSectionLite static functions

// 
// --- In  : nBitsPerPixel - bits per pixel
//           nCompression  - type of compression
// --- Out : 
// --- Returns :The number of colors for this color depth
// --- Effect : Returns the number of color table entries given the number
//              of bits per pixel of a bitmap
/*static*/ int CDIBSectionLite::NumColorEntries(int nBitsPerPixel, int nCompression, DWORD biClrUsed /*= 0*/)
{
    int nColors = 0;

    switch (nBitsPerPixel) 
    {
        case 1:  
            nColors = 2;   
            break;
#ifdef _WIN32_WCE
        case 2:  
            nColors = 4;   
            break;   // winCE only       
#endif
        case 4:  
            nColors = 16;  
            break;
        case 8:  
            nColors = 256; 
            break;
        case 24: 
            nColors = 0;
        case 16:
        case 32:
#ifdef _WIN32_WCE
            nColors = 3;     // I've found that PocketPCs need this regardless of BI_RGB or BI_BITFIELDS
#else
            if (nCompression == BI_BITFIELDS)
                nColors = 3; // 16 or 32 bpp have 3 colors(masks) in the color table if bitfield compression
            else
                nColors = 0; // 16 or 32 bpp have no color table if no bitfield compression
#endif
            break;

        default:
           ASSERT(FALSE);
    }

    // If biClrUsed is provided, and it is a legal value, use it
    if (biClrUsed > 0 && biClrUsed <= (DWORD)nColors)
        return biClrUsed;

    return nColors;
}

// 
// --- In  : nWidth - image width in pixels
//           nBitsPerPixel - bits per pixel
// --- Out :
// --- Returns : Returns the number of storage bytes needed for each scanline 
//               in the bitmap
// --- Effect : 
/*static*/ int CDIBSectionLite::BytesPerLine(int nWidth, int nBitsPerPixel)
{
    return ( (nWidth * nBitsPerPixel + 31) & (~31) ) / 8;
}

#ifndef DIBSECTION_NO_PALETTE

// --- In  : palette - reference to a palette object which will be filled
//           nNumColors - number of color entries to fill
// --- Out :
// --- Returns : TRUE on success, false otherwise
// --- Effect : Creates a halftone color palette independant of screen color depth.
//              palette will be filled with the colors, and nNumColors is the No.
//              of colors to file. If nNumColorsis 0 or > 256, then 256 colors are used.
/*static*/ BOOL CDIBSectionLite::CreateHalftonePalette(CPalette& palette, int nNumColors)
{
    palette.DeleteObject();

    // Sanity check on requested number of colours.
    if (nNumColors <= 0 || nNumColors > 256)
        nNumColors = 256;
    else if (nNumColors <= 2)
        nNumColors = 2;
    else if (nNumColors <= 16)
        nNumColors = 16;
    else if  (nNumColors <= 256)
        nNumColors = 256;

    PALETTEINFO pi;                   
    pi.palNumEntries = (WORD) nNumColors;

    if (nNumColors == 2)
    {
        // According to the MS article "The Palette Manager: How and Why"
        // monochrome palettes not really needed (will use B&W)
        pi.palPalEntry[0].peRed   = ms_StdColors[0].rgbRed;
        pi.palPalEntry[0].peGreen = ms_StdColors[0].rgbGreen;
        pi.palPalEntry[0].peBlue  = ms_StdColors[0].rgbBlue;
        pi.palPalEntry[0].peFlags = 0;
        pi.palPalEntry[1].peRed   = ms_StdColors[255].rgbRed;
        pi.palPalEntry[1].peGreen = ms_StdColors[255].rgbGreen;
        pi.palPalEntry[1].peBlue  = ms_StdColors[255].rgbBlue;
        pi.palPalEntry[1].peFlags = 0;
   }
   else if (nNumColors == 16)
   {
        // According to the MS article "The Palette Manager: How and Why"
        // 4-bit palettes not really needed (will use VGA palette)
       for (int i = 0; i < 8; i++)
       {
           pi.palPalEntry[i].peRed   = ms_StdColors[i].rgbRed;
           pi.palPalEntry[i].peGreen = ms_StdColors[i].rgbGreen;
           pi.palPalEntry[i].peBlue  = ms_StdColors[i].rgbBlue;
           pi.palPalEntry[i].peFlags = 0;
       }
       for (i = 8; i < 16; i++)
       {
           pi.palPalEntry[i].peRed   = ms_StdColors[248+i].rgbRed;
           pi.palPalEntry[i].peGreen = ms_StdColors[248+i].rgbGreen;
           pi.palPalEntry[i].peBlue  = ms_StdColors[248+i].rgbBlue;
           pi.palPalEntry[i].peFlags = 0;
       }
   }
   else // if (nNumColors == 256)
   {
       // Fill palette with full halftone palette
       for (int i = 0; i < 256; i++)
       {
           pi.palPalEntry[i].peRed   = ms_StdColors[i].rgbRed;
           pi.palPalEntry[i].peGreen = ms_StdColors[i].rgbGreen;
           pi.palPalEntry[i].peBlue  = ms_StdColors[i].rgbBlue;
           pi.palPalEntry[i].peFlags = 0;
       }
   }

   return palette.CreatePalette((LPLOGPALETTE) pi);
}
#endif // DIBSECTION_NO_PALETTE


/////////////////////////////////////////////////////////////////////////////
// CDIBSectionLite

CDIBSectionLite::CDIBSectionLite()
{
    // Just in case...
    ASSERT(sizeof(ms_StdColors) / sizeof(ms_StdColors[0]) == 256);

    m_hBitmap     = NULL;
    m_hOldBitmap  = NULL;

#ifndef DIBSECTION_NO_MEMDC_REUSE
    m_bReuseMemDC = FALSE;
#endif

#ifndef DIBSECTION_NO_PALETTE
    m_pOldPalette = NULL;
#endif

#ifndef DIBSECTION_NO_DITHER
    m_bDither     = FALSE;
    m_hDrawDib    = NULL;
#endif

    DeleteObject(); // This will initialise to a known state - ie. empty
}

CDIBSectionLite::~CDIBSectionLite()
{
    DeleteObject();
}

// --- In  :
// --- Out :
// --- Returns :
// --- Effect : Resets the object to an empty state, and frees all memory used.
void CDIBSectionLite::DeleteObject()
{
    // Unselect the bitmap out of the memory DC before deleting bitmap
    ReleaseMemoryDC(TRUE);

    if (m_hBitmap)
        ::DeleteObject(m_hBitmap);
    m_hBitmap = NULL;
    m_ppvBits = NULL;

#ifndef DIBSECTION_NO_PALETTE
    m_Palette.DeleteObject();
#endif

    memset(&m_DIBinfo, 0, sizeof(m_DIBinfo));

#ifndef DIBSECTION_NO_DITHER
    if (m_hDrawDib)
        DrawDibClose(m_hDrawDib);
    m_hDrawDib = NULL;
#endif

    m_iColorDataType = DIB_RGB_COLORS;
    m_iColorTableSize = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDIBSectionLite diagnostics

#ifdef _DEBUG
void CDIBSectionLite::AssertValid() const
{
    ASSERT(m_hBitmap);

    DIBSECTION ds;
    DWORD dwSize = GetObject( m_hBitmap, sizeof(DIBSECTION), &ds );
    ASSERT(dwSize == sizeof(DIBSECTION));

    ASSERT(0 <= m_iColorTableSize && m_iColorTableSize <= 256);

    CObject::AssertValid();
}

void CDIBSectionLite::Dump(CDumpContext& dc) const
{
    CObject::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CDIBSectionLite operations

#ifndef DIBSECTION_NO_DITHER
// --- In  : bDither - whether or not dithering should be enabled
// --- Out :
// --- Returns : TRUE on success
// --- Effect : Turns dithering on by using the DrawDib functions instead of 
//              the GDI functions
BOOL CDIBSectionLite::SetDither(BOOL bDither)
{
    if ( (m_bDither == bDither) &&
         ((m_hDrawDib && m_bDither) || (!m_hDrawDib && !m_bDither)) )
        return TRUE;

    if (!bDither && m_hDrawDib != NULL)
    {
        DrawDibClose(m_hDrawDib);
        m_hDrawDib = NULL;
    }

    m_bDither = bDither;

    return TRUE;
}

// --- In  : 
// --- Out :
// --- Returns : TRUE if dithering is used
// --- Effect : Returns whether or not the DrawDib functions (and hence dithering)
//              is being used.
BOOL CDIBSectionLite::GetDither()
{
    return (m_bDither && GetBitCount() > 8);
}

// --- In  : 
// --- Out :
// --- Returns : A DrawDib context if dithering is supported, else NULL
// --- Effect : Returns a drawdib context for use with DrawDib routines
HDRAWDIB CDIBSectionLite::GetDrawDibContext()
{
    if (!GetDither())
        return NULL;

    if (m_hDrawDib)
        return m_hDrawDib;

    m_hDrawDib = DrawDibOpen();

    return m_hDrawDib;
}


#endif

// --- In  : pDC - Pointer to a device context
//           ptDest - point at which the topleft corner of the image is drawn
// --- Out :
// --- Returns : TRUE on success
// --- Effect : Draws the image 1:1 on the device context
BOOL CDIBSectionLite::Draw(CDC* pDC, CPoint ptDest, CSize size, BOOL bForceBackground /*=FALSE*/) 
{ 
    if (!m_hBitmap)
        return FALSE;

    if (size == CSize(0,0))
	{
		size = GetSize();
	}

    CPoint ptOrigin = CPoint(0,0);

    BOOL bResult = FALSE;

#ifndef DIBSECTION_NO_DITHER
   // We will only be able to get a HDRAWDIB if dithering is enabled
    HDRAWDIB hDrawDib = GetDrawDibContext();
    if (hDrawDib)
    {
#ifndef DIBSECTION_NO_PALETTE
        DrawDibSetPalette( hDrawDib, (HPALETTE)m_Palette);
        DrawDibRealize( hDrawDib,  pDC->GetSafeHdc(),  FALSE);
#endif // DIBSECTION_NO_PALETTE
        bResult = DrawDibDraw(hDrawDib, pDC->GetSafeHdc(), 
                              ptDest.x, ptDest.y, size.cx, size.cy, 
                              GetBitmapInfoHeader(), GetDIBits(), 
                              ptOrigin.x, ptOrigin.y, size.cx, size.cy, 
                              0/*DDF_HALFTONE*/);
    }
    else
    {
#endif
        // Create a memory DC compatible with the destination DC
        CDC* pMemDC = GetMemoryDC(pDC, FALSE);
        if (!pMemDC)
            return FALSE;
        
#ifndef DIBSECTION_NO_PALETTE
        // Select and realize the palette
        CPalette* pOldPalette = NULL;
        if (m_Palette.m_hObject && UsesPalette(pDC))
        {
            pOldPalette = pDC->SelectPalette(&m_Palette, bForceBackground);
            pDC->RealizePalette();
        }
#endif // DIBSECTION_NO_PALETTE
        
        bResult = pDC->BitBlt(ptDest.x, ptDest.y, size.cx, size.cy,
			                  pMemDC, ptOrigin.x, ptOrigin.y, SRCCOPY);
        
#ifndef DIBSECTION_NO_PALETTE
        if (pOldPalette)
            pDC->SelectPalette(pOldPalette, FALSE);
#endif // DIBSECTION_NO_PALETTE
        
    ReleaseMemoryDC();
#ifndef DIBSECTION_NO_DITHER
    }
#endif // DIBSECTION_NO_DITHER

    return bResult;
}

// --- In  : pDC - Pointer to a device context
//           ptDest - point at which the topleft corner of the image is drawn
//           size - size to stretch the image
// --- Out :
// --- Returns : TRUE on success
// --- Effect : Stretch draws the image to the desired size on the device context
BOOL CDIBSectionLite::Stretch(CDC* pDC, CPoint ptDest, CSize size,
                              BOOL bForceBackground /*=FALSE*/) 
{ 
    if (!m_hBitmap)
        return FALSE;

    CPoint ptOrigin = CPoint(0,0);
    CSize imagesize = GetSize();

    BOOL bResult = FALSE;

#ifndef _WIN32_WCE
    pDC->SetStretchBltMode(COLORONCOLOR);
#endif

#ifndef DIBSECTION_NO_DITHER
    // We will only be able to get a HDRAWDIB if dithering is enabled
    HDRAWDIB hDrawDib = GetDrawDibContext();
    if (hDrawDib)
    {
#ifndef DIBSECTION_NO_PALETTE
        DrawDibSetPalette( hDrawDib, (HPALETTE)m_Palette);
        DrawDibRealize( hDrawDib,  pDC->GetSafeHdc(),  FALSE);
#endif // DIBSECTION_NO_PALETTE

        bResult = DrawDibDraw(hDrawDib, pDC->GetSafeHdc(), 
                              ptDest.x, ptDest.y, size.cx, size.cy, 
                              GetBitmapInfoHeader(), GetDIBits(), 
                              0,0, GetWidth(), GetHeight(), 
                              0/*DDF_HALFTONE*/);
    }
    else
    {
#endif // DIBSECTION_NO_DITHER
        // Create a memory DC compatible with the destination DC
        CDC* pMemDC = GetMemoryDC(pDC, FALSE);
        if (!pMemDC)
            return FALSE;
        
#ifndef DIBSECTION_NO_PALETTE
        // Select and realize the palette
        CPalette* pOldPalette = NULL;
        if (m_Palette.m_hObject && UsesPalette(pDC))
        {
            pOldPalette = pDC->SelectPalette(&m_Palette, bForceBackground);
            pDC->RealizePalette();
        }
#endif // DIBSECTION_NO_PALETTE
        
#ifndef _WIN32_WCE
        pDC->SetStretchBltMode(COLORONCOLOR);
#endif // _WIN32_WCE
        
        bResult = pDC->StretchBlt(ptDest.x, ptDest.y, size.cx, size.cy, 
                                  pMemDC, 
                                  ptOrigin.x, ptOrigin.y, imagesize.cx, imagesize.cy, 
                                  SRCCOPY);
        
#ifndef DIBSECTION_NO_PALETTE
        if (pOldPalette)
            pDC->SelectPalette(pOldPalette, FALSE);
#endif // DIBSECTION_NO_PALETTE
        
        ReleaseMemoryDC();
#ifndef DIBSECTION_NO_DITHER
    }
#endif // DIBSECTION_NO_DITHER

    return bResult;
}

//////////////////////////////////////////////////////////////////////////////
// Setting the bitmap...

// --- In  : nIDResource - resource ID
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Initialises the bitmap from a resource. If failure, then object is
//              initialised back to an empty bitmap.
BOOL CDIBSectionLite::SetBitmap(UINT nIDResource)
{
    return SetBitmap(MAKEINTRESOURCE(nIDResource));
}

// --- In  : lpszResourceName - resource name
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Initialises the bitmap from a resource. If failure, then object is
//              initialised back to an empty bitmap.
BOOL CDIBSectionLite::SetBitmap(LPCTSTR lpszResourceName)
{
    HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetResourceHandle(), 
                                        lpszResourceName,
                                        IMAGE_BITMAP, 
                                        0,0, 
#ifdef _WIN32_WCE
                                        0
#else
                                        LR_CREATEDIBSECTION
#endif
                                        );

    if (!hBmp) 
    {
        TRACE0("Unable to LoadImage");
        return FALSE;
    }

    BOOL bResult = SetBitmap(hBmp);
    ::DeleteObject(hBmp);
    return bResult;
}

// --- In  : lpBitmapInfo - pointer to a BITMAPINFO structure
//           lpBits - pointer to image bits. Can be NULL to simply create empty bitmap
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Initialises the bitmap using the information in lpBitmapInfo to determine
//              the dimensions and colors, and the then sets the bits from the bits in
//              lpBits. If failure, then object is initialised back to an empty bitmap.
BOOL CDIBSectionLite::SetBitmap(LPBITMAPINFO lpBitmapInfo, LPVOID lpBits)
{
    DeleteObject();

    if (!lpBitmapInfo) // || !lpBits)
        return FALSE;

    HDC hDC = NULL;
    TRY {
        BITMAPINFOHEADER& bmih = lpBitmapInfo->bmiHeader;

        // Compute the number of colors in the color table
        m_iColorTableSize = NumColorEntries(bmih.biBitCount, bmih.biCompression, bmih.biClrUsed);

        DWORD dwBitmapInfoSize = sizeof(BITMAPINFO) + m_iColorTableSize*sizeof(RGBQUAD);

        // Copy over BITMAPINFO contents
        memcpy(&m_DIBinfo, lpBitmapInfo, dwBitmapInfoSize);

        // Should now have all the info we need to create the sucker.
        //TRACE(_T("Width %d, Height %d, Bits/pixel %d, Image Size %d\n"),
        //      bmih.biWidth, bmih.biHeight, bmih.biBitCount, bmih.biSizeImage);

        // Create a DC which will be used to get DIB, then create DIBsection
        hDC = ::GetDC(NULL);
        if (!hDC) 
        {
            TRACE0("Unable to get DC\n");
            AfxThrowResourceException();
        }

        m_hBitmap = CreateDIBSection(hDC, (const BITMAPINFO*) m_DIBinfo,
                                     m_iColorDataType, &m_ppvBits, NULL, 0);
        ::ReleaseDC(NULL, hDC);
        if (!m_hBitmap)
        {
            TRACE0("CreateDIBSection failed\n");
            AfxThrowResourceException();
        }

        if (m_DIBinfo.bmiHeader.biSizeImage == 0)
        {
            int nBytesPerLine = BytesPerLine(lpBitmapInfo->bmiHeader.biWidth, 
                                             lpBitmapInfo->bmiHeader.biBitCount);
            m_DIBinfo.bmiHeader.biSizeImage = nBytesPerLine * lpBitmapInfo->bmiHeader.biHeight;
        }

#ifndef _WIN32_WCE
        // Flush the GDI batch queue 
        GdiFlush();
#endif
        // Only copy bits if they were provided
        if (lpBits != NULL)
            memcpy(m_ppvBits, lpBits, m_DIBinfo.bmiHeader.biSizeImage);

#ifndef DIBSECTION_NO_PALETTE
        if (!CreatePalette())
        {
            TRACE0("Unable to create palette\n");
            AfxThrowResourceException();
        }
#endif // DIBSECTION_NO_PALETTE
    }
    CATCH (CException, e)
    {
        e->Delete();
        _ShowLastError();
        if (hDC) 
            ::ReleaseDC(NULL, hDC);
        DeleteObject();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}

// --- In  : hBitmap - handle to image
//           pPalette - optional palette to use when setting image
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Initialises the bitmap from the HBITMAP supplied. If failure, then
//              object is initialised back to an empty bitmap.
BOOL CDIBSectionLite::SetBitmap(HBITMAP hBitmap
#ifndef DIBSECTION_NO_PALETTE
                                , CPalette* pPalette /*= NULL*/
#endif
                                )
{
    DeleteObject();

    if (!hBitmap)
        return FALSE;

    // Get dimensions of bitmap
    BITMAP bm;
    if (!::GetObject(hBitmap, sizeof(bm),(LPVOID)&bm))
        return FALSE;
    bm.bmHeight = abs(bm.bmHeight);

    CWindowDC dc(NULL);
#ifndef DIBSECTION_NO_PALETTE
    CPalette* pOldPalette = NULL;
#endif // DIBSECTION_NO_PALETTE

    TRY {
        m_iColorTableSize = NumColorEntries(bm.bmBitsPixel, BI_RGB);

        // Initialize the BITMAPINFOHEADER in m_DIBinfo
        BITMAPINFOHEADER& bih = m_DIBinfo.bmiHeader;
        bih.biSize          = sizeof(BITMAPINFOHEADER);
        bih.biWidth         = bm.bmWidth;
        bih.biHeight        = bm.bmHeight;
        bih.biPlanes        = 1;                // Must always be 1 according to docs
        bih.biBitCount      = bm.bmBitsPixel;

#ifdef _WIN32_WCE
	  //DCR 4/02/01  I've found PocketPCs need BI_BITFIELDS for 16 bit dibs.
        if (bm.bmBitsPixel == 16  || bm.bmBitsPixel == 32)
            bih.biCompression = BI_BITFIELDS; 
        else
            bih.biCompression = BI_RGB;
#else
        bih.biCompression = BI_RGB;
#endif

        bih.biSizeImage     = BytesPerLine(bm.bmWidth, bm.bmBitsPixel) * bm.bmHeight;
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed       = 0;
        bih.biClrImportant  = 0;

        GetColorTableEntries(dc.GetSafeHdc(), hBitmap);

#ifndef DIBSECTION_NO_PALETTE
        // If we have a palette supplied, then set the palette (and hance DIB color
        // table) using this palette
        if (pPalette)
            SetPalette(pPalette);

        if (m_Palette.GetSafeHandle())
        {
            pOldPalette = dc.SelectPalette(&m_Palette, FALSE);
            dc.RealizePalette();
        }
#endif // DIBSECTION_NO_PALETTE

        // Create it!
        m_hBitmap = CreateDIBSection(dc.m_hDC, 
                                     (const BITMAPINFO*) m_DIBinfo,
                                     m_iColorDataType,
                                     &m_ppvBits, 
                                     NULL, 0);
#ifndef DIBSECTION_NO_PALETTE
        if (pOldPalette)
            dc.SelectPalette(pOldPalette, FALSE);
        pOldPalette = NULL; 
#endif // DIBSECTION_NO_PALETTE

        if (!m_hBitmap)
        {
            TRACE0("Unable to CreateDIBSection\n");
            AfxThrowResourceException();
        }

#ifndef DIBSECTION_NO_PALETTE
        // If palette was supplied then create a palette using the entries in the DIB
        // color table.
        if (!pPalette)
            CreatePalette();
#endif // DIBSECTION_NO_PALETTE
   
        // Need to copy the supplied bitmap onto the newly created DIBsection
        CDC memDC, CopyDC;
        if (!CopyDC.CreateCompatibleDC(&dc) || !memDC.CreateCompatibleDC(&dc)) 
        {
            TRACE0("Unable to create compatible DC's\n");
            AfxThrowResourceException();
        }

#ifndef DIBSECTION_NO_PALETTE
        if (m_Palette.GetSafeHandle())
        {
            memDC.SelectPalette(&m_Palette, FALSE);  memDC.RealizePalette();
            CopyDC.SelectPalette(&m_Palette, FALSE); CopyDC.RealizePalette();
        }
#endif // DIBSECTION_NO_PALETTE

#ifndef _WIN32_WCE
        // Flush the GDI batch queue 
        GdiFlush();
#endif

        HBITMAP hOldMemBitmap  = (HBITMAP) SelectObject(memDC.m_hDC,  hBitmap);
        HBITMAP hOldCopyBitmap = (HBITMAP) SelectObject(CopyDC.m_hDC, m_hBitmap);

        CopyDC.BitBlt(0,0, bm.bmWidth, bm.bmHeight, &memDC, 0,0, SRCCOPY);

        SelectObject(memDC.m_hDC, hOldMemBitmap);
        SelectObject(CopyDC.m_hDC, hOldCopyBitmap);

#ifndef DIBSECTION_NO_PALETTE
        if (m_Palette.GetSafeHandle())
        {
            memDC.SelectStockObject(DEFAULT_PALETTE);
            CopyDC.SelectStockObject(DEFAULT_PALETTE);
        }
#endif // DIBSECTION_NO_PALETTE
    }
    CATCH (CException, e)
    {
        e->Delete();
        _ShowLastError();
#ifndef DIBSECTION_NO_PALETTE
        if (pOldPalette)
            dc.SelectPalette(pOldPalette, FALSE);
 #endif // DIBSECTION_NO_PALETTE
        DeleteObject();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// Persistance...

// --- In  : lpszFileName - image filename
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Loads the bitmap from a bitmap file with the name lpszFileName. 
//              If failure, then object is initialised back to an empty bitmap.
BOOL CDIBSectionLite::Load(LPCTSTR lpszFileName)
{
    CFile file;
    if (!file.Open(lpszFileName, CFile::modeRead))
        return FALSE;

    // Get the current file position.  
    DWORD dwFileStart = file.GetPosition();

    // The first part of the file contains the file header.
    // This will tell us if it is a bitmap, how big the header is, and how big 
    // the file is. The header size in the file header includes the color table.
    BITMAPFILEHEADER BmpFileHdr;
    int nBytes;
    nBytes = file.Read(&BmpFileHdr, sizeof(BmpFileHdr));
    if (nBytes != sizeof(BmpFileHdr)) 
    {
        TRACE0("Failed to read file header\n");
        return FALSE;
    }

    // Check that we have the magic 'BM' at the start.
    if (BmpFileHdr.bfType != DS_BITMAP_FILEMARKER)
    {
        TRACE0("Not a bitmap file\n");
        return FALSE;
    }

    // Read the header (assuming it's a DIB). 
    DIBINFO    BmpInfo;
    nBytes = file.Read(&BmpInfo, sizeof(BITMAPINFOHEADER)); 
    if (nBytes != sizeof(BITMAPINFOHEADER)) 
    {
        TRACE0("Failed to read BITMAPINFOHEADER\n");
        return FALSE;
    }

    // Check that we have a real Windows DIB file.
    if (BmpInfo.bmiHeader.biSize != sizeof(BITMAPINFOHEADER))
    {
        TRACE0(" File is not a Windows DIB\n");
        return FALSE;
    }

    // See how big the color table is in the file (if there is one).  
    int nColors = NumColorEntries(BmpInfo.bmiHeader.biBitCount, BmpInfo.bmiHeader.biCompression, BmpInfo.bmiHeader.biClrUsed);
    if (nColors > 0) 
    {
        // Read the color table from the file.
        int nColorTableSize = nColors * sizeof(RGBQUAD);
        nBytes = file.Read(BmpInfo.ColorTable(), nColorTableSize);
        if (nBytes != nColorTableSize) 
        {
            TRACE0("Failed to read color table\n");
            return FALSE;
        }
    }

    // So how big the bitmap surface is.
    int nBitsSize = BmpFileHdr.bfSize - BmpFileHdr.bfOffBits;

    // Double check that the bitmap surface if ok (sometimes the file size is a
    // little larger than the bitmap itself, which may cause overflows)
    int nSurfaceSize = BytesPerLine(BmpInfo.bmiHeader.biWidth, 
                                    BmpInfo.bmiHeader.biBitCount) *    BmpInfo.bmiHeader.biHeight;
    if (nSurfaceSize < nBitsSize)
    {
        // In this case we don't need the entire file
        nBitsSize = nSurfaceSize;
    }

    // Allocate the memory for the bits and read the bits from the file.
    BYTE* pBits = (BYTE*) malloc(nBitsSize);
    if (!pBits) 
    {
        TRACE0("Out of memory for DIB bits\n");
        return FALSE;
    }

    // Seek to the bits in the file.
    file.Seek(dwFileStart + BmpFileHdr.bfOffBits, CFile::begin);

    // read the bits
    nBytes = file.Read(pBits, nBitsSize);
    if (nBytes != nBitsSize) 
    {
        TRACE0("Failed to read bits\n");
        free(pBits);
        return FALSE;
    }

    // Everything went OK.
    BmpInfo.bmiHeader.biSizeImage = nBitsSize;

    if (!SetBitmap((LPBITMAPINFO) BmpInfo, pBits))
    {
        TRACE0("Failed to set bitmap info\n");
        free(pBits);
        return FALSE;
    }

    free(pBits);

    return TRUE;
}

// --- In  : lpszFileName - image filename
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Saves the image to file.
BOOL CDIBSectionLite::Save(LPCTSTR lpszFileName)
{
    BITMAPFILEHEADER   hdr;
    LPBITMAPINFOHEADER lpbi = GetBitmapInfoHeader();

    if (!lpbi || !lpszFileName)
        return FALSE;

    CFile file;
    if (!file.Open(lpszFileName, CFile::modeWrite|CFile::modeCreate))
        return FALSE;

    DWORD dwBitmapInfoSize = sizeof(BITMAPINFO) + (m_iColorTableSize - 1)*sizeof(RGBQUAD);
    DWORD dwFileHeaderSize = dwBitmapInfoSize + sizeof(hdr);

    // Fill in the fields of the file header 
    hdr.bfType       = DS_BITMAP_FILEMARKER;
    hdr.bfSize       = dwFileHeaderSize + lpbi->biSizeImage;
    hdr.bfReserved1  = 0;
    hdr.bfReserved2  = 0;
    hdr.bfOffBits    = dwFileHeaderSize;

    // Write the file header 
    file.Write(&hdr, sizeof(hdr));

    // Write the DIB header
    file.Write(lpbi, dwBitmapInfoSize);
    
    // Write DIB bits
    file.Write(GetDIBits(), lpbi->biSizeImage);

    return TRUE;
}

// --- In  : Bitmap - DIBSection object to be copied
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Creates a copy of another CDIBSection object
BOOL CDIBSectionLite::Copy(CDIBSectionLite& Bitmap) 
{ 
    BOOL bResult = SetBitmap(Bitmap.GetBitmapInfo(), Bitmap.GetDIBits()); 

    if (bResult)
    {
        SetColorTable(Bitmap.GetColorTableSize(), Bitmap.GetColorTable());
#ifndef DIBSECTION_NO_PALETTE
        CreatePalette();
#endif
#ifndef DIBSECTION_NO_DITHER
        SetDither(Bitmap.GetDither());
#endif
    }

    return bResult;
} 

/////////////////////////////////////////////////////////////////////////////
// CDIBSectionLite palette stuff

// --- In  : nNumColors - number of colors to set
//           pColors - array of RGBQUAD's containing colors to set
// --- Out :
// --- Returns : Returns TRUE on success, FALSE otherwise
// --- Effect : Sets the colors used by the image. Only works if # colors <= 256
BOOL CDIBSectionLite::SetColorTable(UINT nNumColors, RGBQUAD *pColors)
{
    if (!m_hBitmap ||!pColors || !nNumColors || m_iColorTableSize == 0 
        || nNumColors > 256)
        return FALSE;

#ifdef _WIN32_WCE
    LPRGBQUAD pColorTable = GetColorTable();
    ASSERT(pColorTable);

    int nCount = min(m_iColorTableSize, nNumColors);
    ::memset(pColorTable, 0, m_iColorTableSize*sizeof(RGBQUAD));
    ::memcpy(pColorTable, pColors, nCount*sizeof(RGBQUAD));
#else
    // Create a memory DC compatible with the screen
    CDC MemDC;
    if (!MemDC.CreateCompatibleDC(NULL))
        return FALSE;

    HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(MemDC.GetSafeHdc(), m_hBitmap);

    ::SetDIBColorTable(MemDC.GetSafeHdc(), 0, nNumColors, pColors);

    // Clean up
    if (hOldBitmap)
        ::SelectObject(MemDC.GetSafeHdc(), hOldBitmap);
#endif

    return TRUE;
}

#ifndef DIBSECTION_NO_PALETTE

// --- In  :
// --- Out :
// --- Returns : TRUE on success
// --- Effect : Creates the palette from the DIBSection's color table. Assumes 
//              m_iColorTableSize has been set and the DIBsection m_hBitmap created
BOOL CDIBSectionLite::CreatePalette()
{
    m_Palette.DeleteObject();

    if (!m_hBitmap)
        return FALSE;

    // Create a 256 color halftone palette if there is no color table in the DIBSection
    if (m_iColorTableSize == 0)
        return CreateHalftonePalette(m_Palette, 256);

    // Get space for the color entries
    RGBQUAD *pRGB = new RGBQUAD[m_iColorTableSize];
    if (!pRGB)
        return CreateHalftonePalette(m_Palette, m_iColorTableSize);

    HDC hDC = ::GetDC(NULL);
    if (!hDC)
    {
        delete [] pRGB;
        return FALSE;
    }

    // Create a memory DC compatible with the current DC
    CDC MemDC;
    MemDC.CreateCompatibleDC(CDC::FromHandle(hDC));
    if (!MemDC.GetSafeHdc())
    {
        delete [] pRGB;
        ::ReleaseDC(NULL, hDC);
        return CreateHalftonePalette(m_Palette, m_iColorTableSize);
    }
    ::ReleaseDC(NULL, hDC);
    
    HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(MemDC.GetSafeHdc(), m_hBitmap);
    if (!hOldBitmap)
    {
        delete [] pRGB;
        return CreateHalftonePalette(m_Palette, m_iColorTableSize);
    }

    // Get the colors used. WinCE does not support GetDIBColorTable so if you
    // are using this on a CE device with palettes, then you need to replace
    // the call with code that manually gets the color table from the m_DIBinfo structure.
#ifdef _WIN32_WCE
    int nColors = ::CEGetDIBColorTable(MemDC.GetSafeHdc(), 0, m_iColorTableSize, pRGB);
#else
    int nColors = ::GetDIBColorTable(MemDC.GetSafeHdc(), 0, m_iColorTableSize, pRGB);
#endif

    // Clean up
    ::SelectObject(MemDC.GetSafeHdc(), hOldBitmap);

    if (!nColors)   // No colors retrieved => the bitmap in the DC is not a DIB section
    {
        delete [] pRGB;
        return CreateHalftonePalette(m_Palette, m_iColorTableSize);
    }   
    
    // Create and fill a LOGPALETTE structure with the colors used.
    PALETTEINFO PaletteInfo;
    PaletteInfo.palNumEntries = m_iColorTableSize;
                        
    for (int i = 0; i < nColors; i++)
    {
        PaletteInfo.palPalEntry[i].peRed   = pRGB[i].rgbRed;
        PaletteInfo.palPalEntry[i].peGreen = pRGB[i].rgbGreen;
        PaletteInfo.palPalEntry[i].peBlue  = pRGB[i].rgbBlue;
        PaletteInfo.palPalEntry[i].peFlags = 0;
    }

    delete [] pRGB;

    // Create Palette!
    return m_Palette.CreatePalette(&PaletteInfo);
}

// --- In  : pPalette - new palette to use
// --- Out :
// --- Returns : TRUE on success
// --- Effect : Sets the current palette used by the image from the supplied CPalette,
//              and sets the color table in the DIBSection
BOOL CDIBSectionLite::SetPalette(CPalette* pPalette)
{
    m_Palette.DeleteObject();

    if (!pPalette)
        return FALSE;

    UINT nColors = pPalette->GetEntryCount();
    if (nColors <= 0 || nColors > 256)
        return FALSE;

    // Get palette entries
    PALETTEINFO pi;
    pi.palNumEntries = (WORD) pPalette->GetPaletteEntries(0, nColors, (LPPALETTEENTRY) pi);
                          
    return SetLogPalette(&pi);
}

// --- In  : pLogPalette - new palette to use
// --- Out :
// --- Returns : TRUE on success
// --- Effect : Sets the current palette used by the image from the supplied LOGPALETTE
BOOL CDIBSectionLite::SetLogPalette(LOGPALETTE* pLogPalette)
{
    if (!pLogPalette)
    {
        CreatePalette();
        return FALSE;
    }

    ASSERT(pLogPalette->palVersion == (WORD) 0x300);

    UINT nColors = pLogPalette->palNumEntries;
    if (nColors <= 0 || nColors > 256)
    {
        CreatePalette();
        return FALSE;
    }

    // Create new palette
    m_Palette.DeleteObject();
    if (!m_Palette.CreatePalette(pLogPalette))
    {
        CreatePalette();
        return FALSE;
    }

    if (m_iColorTableSize == 0)
        return TRUE;

    // Set the DIB colors
    RGBQUAD RGBquads[256]; 
    for (UINT i = 0; i < nColors; i++)
    {
        RGBquads[i].rgbRed   = pLogPalette->palPalEntry[i].peRed;
        RGBquads[i].rgbGreen = pLogPalette->palPalEntry[i].peGreen;
        RGBquads[i].rgbBlue  = pLogPalette->palPalEntry[i].peBlue;
        RGBquads[i].rgbReserved = 0;
    }
    
    return FillDIBColorTable(nColors, RGBquads);
}

// --- In  : nNumColors - number of colors to set
//           pRGB - colors to fill
// --- Out :
// --- Returns : Returns TRUE on success
// --- Effect : Sets the colors used by the image. Only works if # colors <= 256
BOOL CDIBSectionLite::FillDIBColorTable(UINT nNumColors, RGBQUAD *pRGB)
{
    if (!pRGB || !nNumColors || !m_iColorTableSize || nNumColors > 256)
        return FALSE;

    // get the number of colors to return per BITMAPINFOHEADER docs
    UINT nColors;
    LPBITMAPINFOHEADER pBmih = GetBitmapInfoHeader();
    if (pBmih->biClrUsed)
        nColors = pBmih->biClrUsed;
    else
        nColors = 1 << (pBmih->biBitCount*pBmih->biPlanes);

    // Initialize the loop variables
    nColors = min(nNumColors, nColors);

    LPRGBQUAD pColorTable = GetColorTable();
    for (UINT iColor = 0; iColor < nColors; iColor++)
    {
        pColorTable[iColor].rgbReserved = 0;
        pColorTable[iColor].rgbBlue     = pRGB[iColor].rgbBlue;
        pColorTable[iColor].rgbRed      = pRGB[iColor].rgbRed;
        pColorTable[iColor].rgbGreen    = pRGB[iColor].rgbGreen;
    }

    return TRUE;
}

#endif // DIBSECTION_NO_PALETTE


// --- In  : hdc     - the Device Context in which the DIBSection is selected
//           hBitmap - the bitmap whose solor entries are to be queried
//           lpbi    - a pointer to a BITMAPINFO structure that will have it's
//                     color table filled.
// --- Out :
// --- Returns : the number of colors placed in the color table
// --- Effect : This function is a replacement for GetDIBits, in that it retrieves 
//              (or synthesizes) the color table from the given bitmap, and stores 
//              the values in the BITMAPINFO structure supplied.
UINT CDIBSectionLite::GetColorTableEntries(HDC hdc, HBITMAP hBitmap)
{
#ifndef _WIN32_WCE

    // Call GetDIBits with NULL bits pointer to fill in the BITMAPINFOHEADER data
    // Basically we are just getting the color table entries with a minimum of
    // effort
    if (!::GetDIBits(hdc, hBitmap, 0, GetHeight(), NULL, m_DIBinfo, m_iColorDataType))
    {
        TRACE0("Unable to GetDIBits\n");
        AfxThrowResourceException();
    }
    
    // If the driver did not fill in the biSizeImage field, then compute it
    // Each scan line of the image is aligned on a DWORD (32bit) boundary
    if (m_DIBinfo.bmiHeader.biSizeImage == 0)
        m_DIBinfo.bmiHeader.biSizeImage = BytesPerLine(m_DIBinfo.bmiHeader.biWidth, 
                                                       m_DIBinfo.bmiHeader.biBitCount) 
                                            * m_DIBinfo.bmiHeader.biHeight;

    return m_iColorTableSize;

#else

    if (!m_iColorTableSize)
        return 0;

    // Fill the color table with the colors from the bitmap's color table
    LPRGBQUAD pColorTable = GetColorTable();
    
#ifndef _WIN32_WCE
    //DCR 4/02/01  If you are dealing with a 16bpp bitmap in PocketPC, you must
    // set the 3 DWORD color mask. This mask will be applied to a 16bit value to find the 
    // RGB components.  Blue is lowest 5 bits, then Green, then Red.
    if (m_DIBinfo.bmiHeader.biBitCount == 16)
    {
        DWORD dw[3];
        dw[0] = 31744;  //RED bitmask  Bits: 0 11111 00000 00000
        dw[1] = 992;    //GREEN bitmask Bits:0 00000 11111 00000
        dw[2] = 31;     //BLUE bitmask  Bits:0 00000 00000 11111
        memcpy(pColorTable, dw, 3*sizeof(DWORD));

        return 3;
    }
#endif

    // Get the color table from the HBITMAP and copy them over.
    UINT nCount;
    RGBQUAD* pRGB = new RGBQUAD[m_iColorTableSize];
    if (pRGB)
    {
        HBITMAP hOldBitmap = (HBITMAP) SelectObject(hdc, hBitmap);
        nCount = CEGetDIBColorTable(hdc, 0, m_iColorTableSize, pRGB);
        SelectObject(hdc, hOldBitmap);
        if (nCount)
        {
            // m_iColorTableSize = nCount;
            memcpy(pColorTable, pRGB, nCount*sizeof(RGBQUAD));
        }
    }
    delete [] pRGB;

    // Didn't work - so synthesize one.
    if (!nCount)
    {       
        nCount = min( m_iColorTableSize, sizeof(ms_StdColors) / sizeof(ms_StdColors[0]) );
        memcpy(pColorTable, ms_StdColors, nCount*sizeof(RGBQUAD));
    }

    return nCount;
#endif
}

#ifdef _WIN32_WCE
/**********************************************************************
This function is from the MS KB article "HOWTO: Get the Color Table of 
a DIBSection in Windows CE".

PARAMETERS:
HDC - the Device Context in which the DIBSection is selected
UINT - the index of the first color table entry to retrieve
UINT - the number of color table entries to retrieve
RGBQUAD - a buffer large enough to hold the number of RGBQUAD
entries requested

RETURNS:
UINT - the number of colors placed in the buffer

***********************************************************************/
UINT CEGetDIBColorTable(HDC hdc, UINT uStartIndex, UINT cEntries,
                        RGBQUAD *pColors)
{   
    if (pColors == NULL)
        return 0;                       // No place to put them, fail
    
    // Get a description of the DIB Section
    HBITMAP hDIBSection = (HBITMAP) GetCurrentObject( hdc, OBJ_BITMAP );

    DIBSECTION ds;
    DWORD dwSize = GetObject( hDIBSection, sizeof(DIBSECTION), &ds );
    
    if (dwSize != sizeof(DIBSECTION))
        return 0;                      // Must not be a DIBSection, fail
    
    if (ds.dsBmih.biBitCount > 8)
        return 0;                      // Not Palettized, fail
    
    // get the number of colors to return per BITMAPINFOHEADER docs
    UINT cColors;
    if (ds.dsBmih.biClrUsed)
        cColors = ds.dsBmih.biClrUsed;
    else
        cColors = 1 << (ds.dsBmih.biBitCount*ds.dsBmih.biPlanes);
    
    // Create a mask for the palette index bits for 1, 2, 4, and 8 bpp
    WORD wIndexMask = (0xFF << (8 - ds.dsBmih.biBitCount)) & 0x00FF;
    
    // Get the pointer to the image bits
    LPBYTE pBits = (LPBYTE) ds.dsBm.bmBits;
    
    // Initialize the loop variables
    cColors = min( cColors, cEntries );
    BYTE OldPalIndex = *pBits;
 
    UINT TestPixelY;
    if (ds.dsBmih.biHeight > 0 )
        // If button up DIB, pBits points to last row
        TestPixelY = ds.dsBm.bmHeight-1;
    else
        // If top down DIB, pBits points to first row
        TestPixelY = 0;
    
    for (UINT iColor = uStartIndex; iColor < cColors; iColor++)
    {
        COLORREF    rgbColor;
        
        // Set the palette index for the test pixel,
        // modifying only the bits for one pixel
        *pBits = (iColor << (8 - ds.dsBmih.biBitCount)) |
            (*pBits & ~wIndexMask);
        
        // now get the resulting color
        rgbColor = GetPixel( hdc, 0, TestPixelY );
        
        pColors[iColor - uStartIndex].rgbReserved = 0;
        pColors[iColor - uStartIndex].rgbBlue = GetBValue(rgbColor);
        pColors[iColor - uStartIndex].rgbRed = GetRValue(rgbColor);
        pColors[iColor - uStartIndex].rgbGreen = GetGValue(rgbColor);
    }
    
    // Restore the test pixel
    *pBits = OldPalIndex;
    
    return cColors;
}
#endif

// --- In  : pDC - device context to use when calling CreateCompatibleDC
//           bSelectPalette - if TRUE, the current palette will be preselected
// --- Out :
// --- Returns : A pointer to a memory DC
// --- Effect : Creates a memory DC and selects in the current bitmap so it can be
//              modified using the GDI functions. Only one memDC can be created for
//              a given CDIBSectionLite object. If you have a memDC but wish to recreate it
//              as compatible with a different DC, then call ReleaseMemoryDC first.
//              If the memory DC has already been created then it will be recycled.
//              Note that if using this in an environment where the color depth of
//              the screen may change, then you will need to set "m_bReuseMemDC" to FALSE
CDC* CDIBSectionLite::GetMemoryDC(CDC* pDC /*=NULL*/, BOOL bSelectPalette /*=TRUE*/)
{
#ifdef DIBSECTION_NO_MEMDC_REUSE
    ReleaseMemoryDC(TRUE);
#else
    if (!m_bReuseMemDC)
        ReleaseMemoryDC(TRUE);
    else if (m_MemDC.GetSafeHdc())   // Already created?
    {
#ifndef _WIN32_WCE
        // Flush the GDI batch queue 
        GdiFlush();
#endif
        return &m_MemDC;
    }
#endif // DIBSECTION_NO_MEMDC_REUSE

    // Create a memory DC compatible with the given DC
    if (!m_MemDC.CreateCompatibleDC(pDC))
        return NULL;

    // Select in the bitmap
    m_hOldBitmap = (HBITMAP) ::SelectObject(m_MemDC.GetSafeHdc(), m_hBitmap);

#ifndef DIBSECTION_NO_PALETTE
    // Select in the palette
    if (bSelectPalette && UsesPalette(&m_MemDC))
    {
        // Palette should already have been created - but just in case...
        if (!m_Palette.GetSafeHandle())
            CreatePalette();

        m_pOldPalette = m_MemDC.SelectPalette(&m_Palette, FALSE);
        m_MemDC.RealizePalette();
    }
    else
        m_pOldPalette = NULL;
#endif // DIBSECTION_NO_PALETTE

#ifndef _WIN32_WCE
    // Flush the GDI batch queue 
    GdiFlush();
#endif // _WIN32_WCE

    return &m_MemDC;
}

// --- In  : bForceRelease - if TRUE, then the memory DC is forcibly released
// --- Out :
// --- Returns : TRUE on success
// --- Effect : Selects out the current bitmap and deletes the mem dc. If bForceRelease 
//              is FALSE, then the DC release will not actually occur. This is provided 
//              so you can have
//
//                 GetMemoryDC(...)
//                 ... do something
//                 ReleaseMemoryDC()
//
//               bracketed calls. If m_bReuseMemDC is subsequently set to FALSE, then 
//               the same code fragment will still work.
BOOL CDIBSectionLite::ReleaseMemoryDC(BOOL bForceRelease /*=FALSE*/)
{
    if ( !m_MemDC.GetSafeHdc() 
#ifndef DIBSECTION_NO_MEMDC_REUSE
        || (m_bReuseMemDC && !bForceRelease) 
#endif // DIBSECTION_NO_MEMDC_REUSE
        )
        return TRUE; // Nothing to do

#ifndef _WIN32_WCE
    // Flush the GDI batch queue 
    GdiFlush();
#endif // _WIN32_WCE

    // Select out the current bitmap
    if (m_hOldBitmap)
        ::SelectObject(m_MemDC.GetSafeHdc(), m_hOldBitmap);
    m_hOldBitmap = NULL;

#ifndef DIBSECTION_NO_PALETTE
    // Select out the current palette
    if (m_pOldPalette)
        m_MemDC.SelectPalette(m_pOldPalette, FALSE);
    m_pOldPalette = NULL;
#endif // DIBSECTION_NO_PALETTE

    // Delete the memory DC
    return m_MemDC.DeleteDC();
}


#ifdef _DEBUG
// Makes trace windows a little bit more informative...
void CDIBSectionLite::_ShowLastError()
{
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,    
                  NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                  (LPTSTR) &lpMsgBuf, 0, NULL);
    TRACE1("Last error: %s\n", lpMsgBuf);
    LocalFree(lpMsgBuf);
}
#else
void CDIBSectionLite::_ShowLastError() {}
#endif
