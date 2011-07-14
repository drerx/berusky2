#ifndef __DXTLIB_H__
#define __DXTLIB_H__
/*********************************************************************NVMH2****
Path:  C:\Dev\devrel\Nv_sdk_4\Dx8_private\PhotoShop\dxtlib
File:  dxtlib.h

Copyright (C) 1999, 2000 NVIDIA Corporation
This file is provided without support, instruction, or implied warranty of any
kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
not liable under any circumstances for any damages or loss whatsoever arising
from the use or inability to use this file or items derived from it.

Comments:


******************************************************************************/











typedef HRESULT(*MIPcallback) (void *data, int miplevel, DWORD size);

// call back
// pointer to data
// mip level
// size of chunk


/*
   Compresses an image with a user supplied callback with the data for each MIP level created
   Only supports input of RGB 24 or ARGB 32 bpp
*/
HRESULT nvDXTcompress(unsigned char *raw_data,  // pointer to data (24 or 32 bit)
  unsigned long w,              // width in texels
  unsigned long h,              // height in texels
  DWORD pitch, DWORD TextureFormat,     // list below
  bool bGenMipMaps,             // auto gen MIP maps
  bool bDither, DWORD depth,    // 3 or 4
  MIPcallback callback = 0);    // callback for generated levels

// if callback is == 0 (or not specified), then WriteDTXnFile is called with all file info
//
// You must write the routines (or provide stubs)
// void WriteDTXnFile(count, buffer);
// void ReadDTXnFile(count, buffer);
// 
//
void WriteDTXnFile(DWORD count, void *buffer);
void ReadDTXnFile(DWORD count, void *buffer);


// TextureFormat
#define TF_DXT1            10
#define TF_DXT1_1BitAlpha  11
#define TF_DXT3            12
#define TF_DXT5            13
#define TF_RGB4444         14
#define TF_RGB1555         15
#define TF_RGB565          16
#define TF_RGB8888         17


#define DXTERR_INPUT_POINTER_ZERO -1
#define DXTERR_DEPTH_IS_NOT_3_OR_4 -2
#define DXTERR_NON_POWER_2 -3


/* example

LPDIRECT3DTEXTURE8 pCurrentTexture = 0; 

HRESULT LoadAllMipSurfaces(void * data, int iLevel)
{
    HRESULT hr;
    LPDIRECT3DSURFACE8 psurf;
    D3DSURFACE_DESC sd;
    D3DLOCKED_RECT lr;
       
    hr = pCurrentTexture->GetSurfaceLevel(iLevel, &psurf);
    
    if (FAILED(hr))
        return hr;
    psurf->GetDesc(&sd);
    
    
    hr = pCurrentTexture->LockRect(iLevel, &lr, NULL, 0);
    if (FAILED(hr))
        return hr;
    
    memcpy(lr.pBits, data, sd.Size);
    
    hr = pCurrentTexture->UnlockRect(iLevel);
    
    ReleasePpo(&psurf);
    
    return 0;
}
       

    hr = D3DXCreateTexture(m_pd3dDevice, Width, Height, nMips,  0,   D3DFMT_DXT3,  D3DPOOL_MANAGED, &pCurrentTexture);
    nvDXTcompress(raw_data, Width, Height, DXT3, true, 4, LoadAllMipSurfaces);

*/


unsigned char *nvDXTdecompress(int &w, int &h, int &depth, int &total_width,
  int &rowBytes);


enum ColorFormat
{
  COLOR_RGB,
  COLOR_ARGB,
  COLOR_BGR,
  COLOR_BGRA,
  COLOR_RGBA,
  COLOR_ABGR,
};

#endif
