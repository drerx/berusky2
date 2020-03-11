/* --------------------------------------------------
   Rozhrani k 2D grafice
   --------------------------------------------------
*/

#ifndef __BERUSKY3D_KOFOLA2D_H__
#define __BERUSKY3D_KOFOLA2D_H__

#include "2D_graphic.h"

#define  DDX2_BACK_BUFFER   (-1)
#define  NO_SURFACE         (-1)

typedef void (* DDX2_DEVICE_SCREEN_CALLBACK)(void);

typedef struct _DDX2_SURFACE
{
  bitmapa *p_bmp;
} DDX2_SURFACE;

typedef struct _DDX2_SURFACE_DEVICE_3D
{

  int text;
  int filtr;
  int format;

  BODUV map[4];                 // Mapovaci koordinaty v texture

  int back_dx,                  // Rozmery back-bufferu
      back_dy;

  int text_x,                   // Pozice a rozmery textury v Back-bufferu
      text_y,
      text_dx,
      text_dy,
      text_real_dx,
      text_real_dy;

  int scr_x,                    // Rozmery a pozice device na obrazovce
      scr_y, 
      scr_dx, 
      scr_dy;

} DDX2_SURFACE_DEVICE_3D;

typedef struct _DDX2_SURFACE_DEVICE
{
  struct _DDX2_SURFACE_DEVICE *p_next;
  
  bitmapa                     *p_back_buffer;
  DDX2_SURFACE_DEVICE_3D       hw;
  
  int                          rnum;
  RECT                        *p_rlist;
  int                          draw;
  
  DDX2_DEVICE_SCREEN_CALLBACK  p_resize_callback;
} DDX2_SURFACE_DEVICE;

typedef struct _DDX2_SURFACE_LIST
{

  int surf_num;
  int surf_max;
  DDX2_SURFACE *p_slist;
  dword pruhledna_barva;

} DDX2_SURFACE_LIST;


typedef size_ptr DeviceHandle;
typedef int      SurfaceHandle;


//------------------------------------------------------------------------------------------------
// Name: ddx2Init()
// Desc: Vyrobi rezervoar na surfacy
//------------------------------------------------------------------------------------------------
int ddx2Init(int max_surfacu, dword pruhledna_barva);


//------------------------------------------------------------------------------------------------
// Name: ddx2Release()
// Desc: Zrusi rezervovar na surfacy
//------------------------------------------------------------------------------------------------
void ddx2Release(void);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceCreate()
// Desc: Vyrobi renderovaci device a vlozi ho to seznamu devicu
//       filtr - TRUE - linear, FALSE - near
//       bpp - 16/32
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceCreate(int linear_filtr, int bpp, bool cursor_device = FALSE);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceSetActive()
// Desc: Nastavi renderovaci device jako aktivni
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceSetActive(DeviceHandle handle);

//-----------------------------------------------------------------------------
// Name: ddx2DeviceSetCursor()
// Desc: Set this device as a cursor device
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceSetCursor(DeviceHandle handle);

//-----------------------------------------------------------------------------
// Name: ddx2DeviceRemove()
// Desc: Zrusi zarizeni, pokud je aktivni nastavi aktivni prvni v seznamu
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceRemove(DeviceHandle handle);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceResBackBuffer(int back_dx, int back_dy)
// Desc: Nastavi rozmer back-bufferu
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceSetBackBufferSize(int back_dx, int back_dy);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceSetBackBufferRect(int text_x, int text_y, int text_dx, int text_dy)
// Desc: Nastavi pozici a rozmery textury v back-bufferu
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceSetBackBufferRect(int text_x, int text_y, int text_dx, int text_dy);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceSetTextRenderRec(int vx, int vy, int v_dx, int v_dy)
// Desc: Nastavi oblast textury, ktera se bude prenaset na obrazovku
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceSetTextRenderRec(int vx, int vy, int v_dx, int v_dy);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceSetScreenRec(int scr_x, int scr_y, int scr_dx, int scr_dy)
// Desc: Nastavi pozici textury na obrazovce (umisteni + rozmer)
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceSetScreenRec(int scr_x, int scr_y, int scr_dx = 0, int scr_dy = 0);

//-----------------------------------------------------------------------------
// Name: ddx2DeviceSetScreenRecCallback(DDX2DEVICESCREENRECCALLBACK p_callback)
// Desc: Sets a callback for screen resize
//-----------------------------------------------------------------------------
void         ddx2ScreenResDefaultCallback(void);
DeviceHandle ddx2DeviceSetScreenRecCallback(DDX2_DEVICE_SCREEN_CALLBACK p_callback);

//-----------------------------------------------------------------------------
// Name: ddx2DeviceDeaktivuj()
// Desc: Odmapuje aktivni zarizeni zobrazovky (smaze texturu a pod.)
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceDeaktivuj(void);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceSetRender(int draw)
// Desc: Povoli/zakaze kresleni device (na zacatku je kresleni zakazano)
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceSetRender(int draw);


//-----------------------------------------------------------------------------
// Name: ddx2DeviceGetInfo()
// Desc: Cte informace o velikost obsazene pameti a poctu surfacu (vsech)
//-----------------------------------------------------------------------------
DeviceHandle ddx2DeviceGetInfo(int *p_surfacu, int *p_mem);


//-----------------------------------------------------------------------------
// Name: ddx2SetRect()
// Desc: Nastavi seznam recu na kresleni, NULL = nic nekreslit
//-----------------------------------------------------------------------------
void ddx2SetRect(RECT * p_rlist, int rnum);

//------------------------------------------------------------------------------------------------
// finds first free index
//------------------------------------------------------------------------------------------------
SurfaceHandle ddx2FindFreeSurface(void);

//------------------------------------------------------------------------------------------------
// release bitmap
//------------------------------------------------------------------------------------------------
SurfaceHandle ddx2ReleaseBitmap(SurfaceHandle iSurface);

//------------------------------------------------------------------------------------------------
// load bitmap from a directory na pozici
//------------------------------------------------------------------------------------------------
SurfaceHandle ddx2LoadBitmapPos(SurfaceHandle handle, char *pFileName, char *pDirName);

//------------------------------------------------------------------------------------------------
// load bitmap from a directory
//------------------------------------------------------------------------------------------------
SurfaceHandle ddx2LoadBitmap(char *pFileName, char *pDirName);

//------------------------------------------------------------------------------------------------
// load bitmap from DISK
//------------------------------------------------------------------------------------------------
SurfaceHandle ddx2LoadBitmapDisk(char *pFileName);

//------------------------------------------------------------------------------------------------
// load bitmap from DISK na pozici
//------------------------------------------------------------------------------------------------
SurfaceHandle ddx2LoadBitmapPosDisk(SurfaceHandle handle, char *pFileName);


//------------------------------------------------------------------------------------------------
// load list of bitmaps from a directory
//------------------------------------------------------------------------------------------------
int ddx2LoadList(char *pFileName, char *pBmpDir, char *p_bmp_dir);


//------------------------------------------------------------------------------------------------
// Create Surface
//------------------------------------------------------------------------------------------------
SurfaceHandle ddx2CreateSurface(int x, int y, int idx);

void ddx2DrawSurfaceColorKey(int iSurface, int *com, int layer, COLORREF color);
void ddx2DrawDisplayColorKey(int *com, int layer, COLORREF color);
void ddx2DrawSurface(int iSurface, int *com, int layer);
void ddx2DrawDisplay(int *com, int layer);

BOOL ddx2TransparentBlt(SurfaceHandle dst, int dx, int dy, int dsirka, int dvyska,
                        SurfaceHandle src, int sx, int sy, dword pruhledna);
BOOL ddx2TransparentBltDisplay(int dx, int dy, int dsirka, int dvyska,
                               int dcSrcSurface, 
                               int sx, int sy, int ssirka, int svyska,
                               UINT crTransparent);
BOOL ddx2TransparentBltFull(SurfaceHandle dst, int dx, int dy,
                            SurfaceHandle src, dword barva);

BOOL ddx2BitBlt(SurfaceHandle dst, int dx, int dy, int sirka, int vyska,
                SurfaceHandle src, int sx, int sy);
BOOL ddx2BitBltStretch(SurfaceHandle dst, int dx, int dy, int dst_width, int dst_height,
                       SurfaceHandle src, int sx, int sy, int src_width, int src_height);
BOOL ddx2BitBltDisplay(int dx, int dy, int sirka, int vyska,
                       int dcSrcSurface, int sx, int sy);
BOOL ddx2BitBltFull(SurfaceHandle dst, int dx, int dy, SurfaceHandle src);


int ddx2GetWidth(SurfaceHandle src);
int ddx2GetHeight(SurfaceHandle src);

void ddx2CleareSurfaceColor(SurfaceHandle iSurface, COLORREF color);
void ddx2FillRect(SurfaceHandle iSurface, RECT * rect, COLORREF color);

void ddx2CleareSurface(SurfaceHandle iSurface);

void ddx2AddRectItem(RECT_LINE * p_rl, RECT rect, int iLayer = 0);
void ddx2DrawCursor(SurfaceHandle iSurface, int x, int y, int dx, int dy, dword pruhledna);
void ddx2DrawCursorSetDraw(int draw);

//------------------------------------------------------------------------------------------------
// Povoli/zakaze rendering hry (3D modelu a pod.)
//------------------------------------------------------------------------------------------------
void ddx2GameRender(int render);

// ----------------------------------------------------------------------------
// Name: ddx2RenderDevices()
// Desc: Vykresli menu pouze menu, bez mazani obrazovky a flipu
//       Vola se pokud se menu ma kreslit jako soucast hry (volat po renderingu
//       menu s listim)
// ----------------------------------------------------------------------------
void ddx2RenderDevices(G_KONFIG * p_ber);


// ----------------------------------------------------------------------------
// Name: ddx2RenderujVse()
// Desc: Vykresli menu
//       Kresli menu vcetne vymazani obrazovky
// ----------------------------------------------------------------------------
void ddx2RenderujVse(G_KONFIG * p_ber);


#endif
