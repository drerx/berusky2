#include <stdio.h>
#include <unistd.h>
#include "3d_all.h"
#include "Berusky3d_kofola_interface.h"
#include "menu_script.h"
#include "2D_graphic.h"
#include "game_init.h"

typedef struct
{
  int iPicture;
  int iTimeToNextPicture;
  int xPos;
  int yPos;
} COMICS_PICTURE;

#define DARKEN_ADD  -200

static char bCimicsEnd;
static int iActualBmp;
static TIMER_ID uiTimerID;
static COMICS_PICTURE cmcs_Picture[64];

/*static MCI_DGV_OPEN_PARMS		mciOpen;
static MCI_DGV_WINDOW_PARMS		mciWindow; 
static DWORD					wDeviceID = 0;
static DWORD					wError;*/

extern APAK_HANDLE *pBmpArchive;

void cmcs_Draw(int iIndex, int xPos, int yPos)
{
/*
	BitBlt(_2dd.hDC, xPos, yPos,
		   _2dd.bitmap[iIndex].bitmap.bmWidth,
		   _2dd.bitmap[iIndex].bitmap.bmHeight,
		   _2dd.bitmap[iIndex].bitmapDC,0,0,SRCCOPY);
*/
}

void cmcs_Read_Line(char *pLine, COMICS_PICTURE * pPicture)
{
  int p = 0;
  char expression[MAX_FILENAME];

  p = Find_Next_Expresion(pLine, p, expression);
  pPicture->xPos = atoi(expression);
  p = Find_Next_Expresion(pLine, p, expression);
  pPicture->yPos = atoi(expression);
  p = Find_Next_Expresion(pLine, p, expression);
  pPicture->iTimeToNextPicture = atoi(expression);
}

void cmcs_Next_Picture(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
  COMICS_PICTURE *pPicture = &cmcs_Picture[iActualBmp];

  KillTimer(NULL, uiTimerID);

  if (pPicture->iPicture == -1) {
    bCimicsEnd = 1;
    return;
  }

  if (!pPicture->iTimeToNextPicture) {
    cmcs_Draw(pPicture->iPicture, pPicture->xPos, pPicture->yPos);

    iActualBmp++;
    pPicture = &cmcs_Picture[iActualBmp];
  }

  cmcs_Draw(pPicture->iPicture, pPicture->xPos, pPicture->yPos);


  uiTimerID =
    SetTimer(NULL, 0, pPicture->iTimeToNextPicture,
    (TIMERPROC) cmcs_Next_Picture);

  iActualBmp++;
}

void cmcs_Start_Comics(char *cFile, HWND hWnd, AUDIO_DATA * p_ad, char bMusic)
{
  int i;
  char text[MAX_FILENAME] = "";
  FILE *file;

  bCimicsEnd = 0;
  iActualBmp = 0;

  file = fopen(cFile, "r");

  if (!file)
    return;

  for (i = 0; i < 64; i++)
    cmcs_Picture[i].iPicture = -1;

  while (strcmp(text, "LOAD_END")) {
    if (fgets(text, MAX_FILENAME, file) == NULL)
      return;
    newline_cut(text);

    if (!strcmp(text, "LOAD_END"))
      break;

    i = ddxLoadBitmap(text, pBmpArchive);
  }

  i = 0;

  while (strcmp(text, "COMICS_END")) {
    if (fgets(text, MAX_FILENAME, file) == NULL)
      return;
    newline_cut(text);
    cmcs_Read_Line(text, &cmcs_Picture[i]);
    cmcs_Picture[i].iPicture = i + 1;
    i++;
  }

  fclose(file);

	if(bMusic)
		if (p_ad->bAudio && p_ad->Music_Gain >= 0.05f)
			ap_Setup_and_Play_Song(1,0, p_ad);

  cmcs_Draw(0, 0, 0);
  cmcs_Draw(cmcs_Picture[0].iPicture, cmcs_Picture[0].xPos,
    cmcs_Picture[0].yPos);

  uiTimerID =
    SetTimer(NULL, 0, (UINT) cmcs_Picture[0].iTimeToNextPicture,
    (TIMERPROC) cmcs_Next_Picture);

  iActualBmp++;

  while (!bCimicsEnd) {
    spracuj_spravy(0);
    if (key[K_ESC]) {
      key[K_ESC] = 0;
      bCimicsEnd = 1;
    }

    Sleep(10);
  }

	if(bMusic)
		if(ogg_playing())
			ap_Stop_Song(p_ad);

  KillTimer(NULL, uiTimerID);
    
  // TODO
  // _2D_Release()
}

void cmcs_Play_Intro(char *cFile, HWND hWnd, AUDIO_DATA * p_ad)
{
  if (chdir(DATA_DIR))
    return;
  cmcs_Start_Comics("gamelogo.txt", hWnd, p_ad, 0);
}

/*DWORD cmcs_Play_Movie(DWORD wDevID, DWORD dwFrom, DWORD dwTo) 
{ 
    MCI_DGV_PLAY_PARMS mciPlay;    // play parameters 
    DWORD dwFlags = 0; 
 
    // Check dwFrom. If it is != 0 then set parameters and flags. 
    if (dwFrom){ 
        mciPlay.dwFrom = dwFrom; // set parameter 
        dwFlags |= MCI_FROM;     // set flag to validate member 
    } 
 
    // Check dwTo. If it is != 0 then set parameters and flags. 
    if (dwTo){ 
        mciPlay.dwTo = dwTo;    // set parameter 
        dwFlags |= MCI_TO;      // set flag to validate member 
    } 
 
    // Send the MCI_PLAY command and return the result. 
    return mciSendCommand(wDevID, MCI_PLAY, dwFlags, 
       (DWORD)(LPVOID)&mciPlay); 
} 
*/
/*void cmcs_Stop_Video(void)
{
	mciSendCommand(wDeviceID, MCI_STOP, 0, 0);  
    mciSendCommand(wDeviceID, MCI_CLOSE, 0, 0);
}*/

void cmcs_Game_Down(void)
{
/*
	ShowCursor(FALSE);
	spracuj_spravy(0);
	ShowWindow(hWnd,SW_SHOW);
	spracuj_spravy(0);
	SetForegroundWindow(hWnd);						
	spracuj_spravy(0);
	SetFocus(hWnd);	
	spracuj_spravy(0);

	mciSendString("open digitalvideo", NULL, 0, NULL);
*/
}

void cmcs_Game_Up(void)
{
/*
	mciSendString("close digitalvideo", NULL, 0, NULL); 

	ShowCursor(TRUE);
	spracuj_spravy(0);
	ShowWindow(hWnd,SW_SHOW);
	spracuj_spravy(0);
	ShowWindow(hWnd,SW_MAXIMIZE);
	spracuj_spravy(0);
*/
}

void cmcs_Play_Video(char *pFile, long dwVideoTime, AUDIO_DATA * p_ad)
{
/*
	int done = 0;
	long counter = 0;
	char lpstrFile[256];

	GetPrivateProfileString("files","bitmap_dir","c:\\",lpstrFile,256,ini_file);
  working_file_translate(lpstrFile,256);
	chdir(lpstrFile);

	sprintf(lpstrFile, "open %s type mpegvideo alias anakreonvideo style overlapped", pFile, hWnd);

	done = mciSendString(lpstrFile,0,0,0);

	if(done)
	{
		mciGetErrorString(done, lpstrFile, 256);
		kprintf(1,"%s", lpstrFile);
	}

	done = mciSendString("break anakreonvideo on 27", 0, 0, 0);

	wsprintf(lpstrFile, "put %s %s %s", "anakreonvideo", "destination at 0 0 1024 768", ""); 
	done = mciSendString(lpstrFile,0,0,0);
	
	wsprintf(lpstrFile, "put %s %s %s", "anakreonvideo", "window client at 0 0 1024 768", ""); 
	done = mciSendString(lpstrFile,0,0,0);

	wsprintf(lpstrFile, "put %s %s %s", "anakreonvideo", "window client at 0 0 1024 768", ""); 
	done = mciSendString(lpstrFile,0,0,0);

	done = mciSendString("window anakreonvideo state show", 0, 0, 0);

	if(done)
	{
		mciGetErrorString(done, lpstrFile, 256);	
		kprintf(1,"%s", lpstrFile);
	}

	//ShowWindow(hWnd,SW_MINIMIZE);
	spracuj_spravy(0);

	wsprintf(lpstrFile, "setaudio %s %s %s", "anakreonvideo", "volume to 250", "");
	done = mciSendString(lpstrFile,0,0,0);

	done = mciSendString("play anakreonvideo from 0 wait fullscreen",0,0,hWnd);

	if(done)
	{
		mciGetErrorString(done, lpstrFile, 256);	
		kprintf(1,"%s", lpstrFile);
	}
	
	mciSendString("stop anakreonvideo",0,0,0);	
	mciSendString("close anakreonvideo",0,0,0);
*/
}

void cmcs_Start_Picture(int Index, long time, AUDIO_DATA * p_ad, char bMusic)
{
  long timecnt = 0;
  char bCimicsEnd = 0;

  cmcs_Draw(Index, 0, 0);

  while (!bCimicsEnd) {
    spracuj_spravy(0);
    if (key[K_ESC]) {
      key[K_ESC] = 0;
      bCimicsEnd = 1;
    }

    Sleep(10);

    timecnt += 10;

    if (timecnt > time)
      bCimicsEnd = 1;
  }
	if(bMusic)
		if(ogg_playing())
			ap_Stop_Song(p_ad);
}
