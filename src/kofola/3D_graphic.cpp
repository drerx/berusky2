//------------------------------------------------------------------------------------------------
// version 0.0.1
//------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <errno.h>

#include "3d_all.h"
#include "Berusky3d_kofola_interface.h"
#include "Berusky_universal.h"
#include "3D_graphic.h"
#include "font.h"

_3D_DATA _3dd;
_3D_CURSOR _3dCur;
extern char p3DMDir[MAX_FILENAME];
extern EDIT_TEXT sIndikace[3];
extern char cFontDir[5][64];
extern HINT_TEXTURE pMessageTexture[8];

//------------------------------------------------------------------------------------------------
// init 3d
//------------------------------------------------------------------------------------------------
int _3d_Init(void)
{
  int i;

  _3dd.count = 200;
  _3dd.last = 0;

  _3dd.p_texture = (EDIT_TEXT *) mmalloc((_3dd.count) * sizeof(EDIT_TEXT));
  _3dd.p_sysramtexture = (_3D_TEXTURE *) mmalloc((_3dd.count) * sizeof(_3D_TEXTURE));

  for (i = 0; i < _3dd.count; i++) {
    strcpy(_3dd.p_texture[i].jmeno, "\0");
    _3dd.p_texture[i].load = 0;
    _3dd.p_texture[i].p_bmp = NULL;
    _3dd.p_texture[i].text = 0;

    _3dd.p_sysramtexture[i].bSLoaded = 0;
    _3dd.p_sysramtexture[i].bVLoaded = 0;
  }
  
  _3dd.bMenuVRAMLoad = GetPrivateProfileInt("game", "menu_vram_load", 1, ini_file);
  strcpy(_3dd.bm_dir, BITMAP_DIR);

  return 1;
}

//------------------------------------------------------------------------------------------------
// release 3d
//------------------------------------------------------------------------------------------------
void _3d_Release(void)
{
  int i;

  kprintf(1, "Kofola: - Release Textur...");

  for (i = 0; i < _3dd.count; i++)
    if (_3dd.p_texture[i].load) {
      if (_3dd.p_sysramtexture[i].bVLoaded) {
        txt_zrus_texturu(&_3dd.p_texture[i]);
        _3dd.p_sysramtexture[i].bVLoaded = 1;
      }

      if (_3dd.p_sysramtexture[i].bSLoaded) {
        if (_3dd.p_texture[i].p_bmp)
          txt_zrus_texturu_ram(&_3dd.p_texture[i]);

        _3dd.p_sysramtexture[i].bSLoaded = 0;
      }
    }

  free((void *) _3dd.p_texture);
  free((void *) _3dd.p_sysramtexture);

  _3dd.last = 0;
  _3dd.count = 0;

}

//------------------------------------------------------------------------------------------------
// release texture
//------------------------------------------------------------------------------------------------
void _3d_Release_Texture(int Index)
{
  if (_3dd.p_sysramtexture[Index].bVLoaded) {
    txt_zrus_texturu(&_3dd.p_texture[Index]);
    _3dd.p_sysramtexture[Index].bVLoaded = 0;
  }

  if (_3dd.p_sysramtexture[Index].bSLoaded) {
    txt_zrus_texturu_ram(&_3dd.p_texture[Index]);
    _3dd.p_sysramtexture[Index].bSLoaded = 0;
  }

  strcpy(_3dd.p_texture[Index].jmeno, "\0");
  _3dd.p_texture[Index].load = 0;
  _3dd.p_texture[Index].p_bmp = NULL;
  _3dd.p_texture[Index].text = 0;
}

//------------------------------------------------------------------------------------------------
// load texture
//------------------------------------------------------------------------------------------------
int _3d_Load_Texture(char *p_File_Name, int Index, char bVideoRAM, char bSeek)
{
  if (_3dd.p_texture[Index].load)
    return 0;

  _3dd.p_texture[Index].flag = 0;

  if (bVideoRAM) {
    txt_nahraj_texturu_z_func(p3DMDir, p_File_Name,
                              &_3dd.p_texture[Index], 0, bVideoRAM, 
                              NULL, bmp_nahraj);
  }
  else {
    txt_nahraj_texturu_z_func(p3DMDir, p_File_Name,
                              &_3dd.p_texture[Index], 1, bVideoRAM, 
                              &_3dd.p_sysramtexture[Index].konf, bmp_nahraj);
  }

  if (!_3dd.p_texture[Index].load) {
    kprintf(1, "Can't load %s", p_File_Name);
    _3dd.p_sysramtexture[Index].bVLoaded = 0;
    _3dd.p_sysramtexture[Index].bSLoaded = 0;
    return 0;
  }
  else {
    if (bVideoRAM) {
      _3dd.p_sysramtexture[Index].bVLoaded = 1;
      _3dd.p_sysramtexture[Index].bSLoaded = 0;
    }
    else {
      _3dd.p_sysramtexture[Index].bVLoaded = 0;
      _3dd.p_sysramtexture[Index].bSLoaded = 1;
    }

    return 1;
  }
}

void _3d_Get_Cursor_Name(char *cName)
{
  FILE *file;
  char filename[MAX_FILENAME], text[32];

  GetPrivateProfileString("game", "cursor", "cursor", filename, 32, ini_file);

  strcpy(cName, filename);
  strcat(cName, ".png");

  strcat(filename, ".inf");

  file = fopen(filename, "r");
  if (!file) {
    kprintf(1, "Soubor %s, nebyl nalezen!", filename);
    return;
  }

  if (fgets(text, 32, file) == NULL) {
    kprintf(1, "Cannot read from %s!", filename);
    return;
  }
  _3dCur.idx = atoi(text);

  if (fgets(text, 32, file) == NULL) {
    kprintf(1, "Cannot read from %s!", filename);
    return;
  }
  _3dCur.idy = atoi(text);

  if (fgets(text, 32, file) == NULL) {
    kprintf(1, "Cannot read from %s!", filename);
    return;
  }
  _3dCur.iaddx = atoi(text);

  if (fgets(text, 32, file) == NULL) {
    kprintf(1, "Cannot read from %s!", filename);
    return;
  }
  _3dCur.iaddy = atoi(text);

  fclose(file);
}

//------------------------------------------------------------------------------------------------
// load list of textures
//------------------------------------------------------------------------------------------------
int _3d_Load_List(char *p_File_Name)
{
  char text[MAX_FILENAME];
  FILE *file = 0;
  int c = 0;

  construct_path(text, MAX_FILENAME, 2, p3DMDir, p_File_Name);

  file = fopen(text, "rb");
  if (!file) {
    kprintf(1, "%s: %s", text, strerror(errno));
    return 0;
  }

  if (chdir((_3dd.bm_dir))) {
    kprintf(1, "Cannot change directory to %s", _3dd.bm_dir);
    return 0;
  }

  kprintf(1, "Kofola: - Load textur pro herni menu");

  txt_trida(TEXT_MENU);
  kom_set_default_text_config(0, 0, 1, 0, 0, 1);

  while (!feof(file)) {
    if (fgets(text, MAX_FILENAME, file) && !feof(file)) {
      newline_cut(text);

      if (!c)
        _3d_Get_Cursor_Name(text);

      if (c < 107 || c > 132) {
        _3d_Load_Texture(text, c, TRUE, !c);
      }
      else {
        _3d_Load_Texture(text, c, _3dd.bMenuVRAMLoad, 0);
      }

      c++;
    }
  }
  fclose(file);

  _3dd.last = c;

  kom_ret_default_text_config();

  return 1;
}

void _3d_Load_From_Sys_To_Video(int iStart)
{
  int i;

  if (_3dd.bMenuVRAMLoad)
    return;

  for (i = iStart; i < 133; i++)
    if (_3dd.p_texture[i].load && !_3dd.p_sysramtexture[i].bVLoaded) {
      txt_nahraj_texturu_do_vram(&_3dd.p_texture[i],
        &_3dd.p_sysramtexture[i].konf);
      _3dd.p_sysramtexture[i].bVLoaded = 1;
    }
}

void _3d_Release_From_Video(int iStart)
{
  int i;

  if (_3dd.bMenuVRAMLoad)
    return;

  for (i = iStart; i < 133; i++)
    if (_3dd.p_sysramtexture[i].bVLoaded && _3dd.p_texture[i].load) {
      txt_zrus_texturu_z_vram(&_3dd.p_texture[i]);      
      _3dd.p_sysramtexture[i].bVLoaded = 0;
    }
}

void _3d_Set_Mask(void)
{
  /*glDisable(GL_BLEND);  
     glAlphaFunc(GL_EQUAL,1.0f); */
}

void _3d_Set_Smooth(void)
{
  /*glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     glAlphaFunc(GL_GREATER,0.0f); */
}

void _3d_Begin_Draw(void)
{
  set_matrix_2d(SCREEN_XRES, SCREEN_YRES);
  glColor4f(1, 1, 1, 1);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glAlphaFunc(GL_GREATER, 0.0f);
}

void _3d_End_Draw(void)
{
  glDisable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5f);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  ret_matrix_2d();
}


//------------------------------------------------------------------------------------------------
// Draws box in 3d device
// int i, float {left, top, right, bottom}
//------------------------------------------------------------------------------------------------
void _3d_Draw_Box(int i, float *vfPoint)
{

  glBindTexture(GL_TEXTURE_2D, _3dd.p_texture[i].text);

  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2d(0, 1);
  glVertex2d(vfPoint[0], vfPoint[1]);
  glTexCoord2d(0, 0);
  glVertex2d(vfPoint[0], vfPoint[3]);
  glTexCoord2d(1, 1);
  glVertex2d(vfPoint[2], vfPoint[1]);
  glTexCoord2d(1, 0);
  glVertex2d(vfPoint[2], vfPoint[3]);
  glEnd();
}

void _3d_Put_Texture_In_VRAM(int *text, int x, int y, char *data)
{
  glGenTextures(1, (GLuint *) text);
  glBindTexture(GL_TEXTURE_2D, *text);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y,
               0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void _3d_Load_Hint(HINT_TEXTURE * bTexture, char *cHint, int i, int iSection)
{
  fn_Get_Font_Texture(iSection, cHint);

  _3d_Put_Texture_In_VRAM((int *) &bTexture[i].text.text,
    _3dd.p_sysramtexture[_3dd.last - 1].x,
    _3dd.p_sysramtexture[_3dd.last - 1].y,
    _3dd.p_sysramtexture[_3dd.last - 1].data);

  bTexture[i].tx = _3dd.p_sysramtexture[_3dd.last - 1].tx;
  bTexture[i].ty = _3dd.p_sysramtexture[_3dd.last - 1].ty;
  bTexture[i].x = _3dd.p_sysramtexture[_3dd.last - 1].x;
  bTexture[i].y = _3dd.p_sysramtexture[_3dd.last - 1].y;

  free((void *) _3dd.p_sysramtexture[_3dd.last - 1].data);
  memset(&_3dd.p_texture[_3dd.last - 1], 0, sizeof(EDIT_TEXT));
  memset(&_3dd.p_sysramtexture[_3dd.last - 1], 0, sizeof(_3D_TEXTURE));
  _3dd.last--;
}

void _3d_Load_Indikace(void)
{
  ZeroMemory(sIndikace, 3 * sizeof(EDIT_TEXT));

  txt_trida(TEXT_MENU);
  kom_set_default_text_config(0, 0, 1, 0, 0, 1);
  txt_nahraj_texturu_z_func(p3DMDir, "camera1.png", &sIndikace[0], 0, 1,
    NULL, bmp_nahraj);
  txt_nahraj_texturu_z_func(p3DMDir, "vitamin1.png", &sIndikace[1], 0, 1,
    NULL, bmp_nahraj);
  txt_nahraj_texturu_z_func(p3DMDir, "lock1.png", &sIndikace[2], 0, 1,
    NULL, bmp_nahraj);
  kom_ret_default_text_config();
}

void _3d_Gen_Hints(HINT_TEXTURE * bTexture, int tsize)
{
  char text[MAX_FILENAME];
  int i;

  ZeroMemory(bTexture, sizeof(EDIT_TEXT) * tsize);

  if (!fn_Set_Font(cFontDir[4])) {
    kprintf(1, "Unable to set font!");
    return;
  }

  if (!fn_Load_Bitmaps()) {
    kprintf(1, "Unable to load font bitmaps");
    return;
  }

  kom_set_default_text_config(0, 0, 1, 0, 0, 1);
  txt_nahraj_texturu_z_func(p3DMDir, "hint_frame.png", &bTexture[0].text,
    0, 1, NULL, bmp_nahraj);
  kom_ret_default_text_config();

  for (i = 1; i < 26; i++) {
    sprintf(text, "##hint_%d", i);
    _3d_Load_Hint(bTexture, text, i, 0);
    kprintf(1, "Load Hint: %s", text);
  }

  sprintf(text, "##message_yes");
  _3d_Load_Hint(pMessageTexture, text, 0, 0);
  _3d_Load_Hint(pMessageTexture, text, 1, 1);
  kprintf(1, "Load Message: %s", text);

  sprintf(text, "##message_no");
  _3d_Load_Hint(pMessageTexture, text, 2, 0);
  _3d_Load_Hint(pMessageTexture, text, 3, 1);
  kprintf(1, "Load Message: %s", text);

  sprintf(text, "##message_restart");
  _3d_Load_Hint(pMessageTexture, text, 4, 0);
  kprintf(1, "Load Message: %s", text);

  sprintf(text, "##message_save");
  _3d_Load_Hint(pMessageTexture, text, 5, 0);
  kprintf(1, "Load Message: %s", text);

  sprintf(text, "##message_mainmenu");
  _3d_Load_Hint(pMessageTexture, text, 6, 0);
  kprintf(1, "Load Message: %s", text);

  sprintf(text, "##message_demo");
  _3d_Load_Hint(pMessageTexture, text, 7, 0);
  kprintf(1, "Load Message: %s", text);

  fn_Release_Font(1);
}

void _3d_Release_Hints(HINT_TEXTURE * bTexture, int tsize)
{
  int i;

  for (i = 1; i < tsize; i++)
    txt_zrus_texturu(&bTexture[i].text);

  for (i = 0; i < 3; i++)
    txt_zrus_texturu(&sIndikace[i]);

  for (i = 0; i < 8; i++)
    txt_zrus_texturu(&pMessageTexture[i].text);
}
