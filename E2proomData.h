#ifndef E2PROOMDATA_H
#define E2PROOMDATA_H

#include "global.h"
#include <string>
#if _MSC_VER
#include <QDir>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "tistdtypes.h"
#include "FileOut.h"

#define E2POOM_CAMDLG_SAVEBUFF              64
#define E2POOM_CAMDLG_SYSPATH_MOTO			"./SAVE/E2P_CAMDLG.bsd"

#define E2POOM_CAMDLG_MODPOSX1_MIN          0
#define E2POOM_CAMDLG_MODPOSX1_MAX          100000
#define E2POOM_CAMDLG_MODPOSX1_USE          0
#define E2POOM_CAMDLG_MODPOSY1_MIN          0
#define E2POOM_CAMDLG_MODPOSY1_MAX          100000
#define E2POOM_CAMDLG_MODPOSY1_USE          8000
#define E2POOM_CAMDLG_MODPOSX2_MIN          0
#define E2POOM_CAMDLG_MODPOSX2_MAX          100000
#define E2POOM_CAMDLG_MODPOSX2_USE          2750
#define E2POOM_CAMDLG_MODPOSY2_MIN          0
#define E2POOM_CAMDLG_MODPOSY2_MAX          100000
#define E2POOM_CAMDLG_MODPOSY2_USE          0
#define E2POOM_CAMDLG_MODPOSX3_MIN          0
#define E2POOM_CAMDLG_MODPOSX3_MAX          100000
#define E2POOM_CAMDLG_MODPOSX3_USE          5750
#define E2POOM_CAMDLG_MODPOSY3_MIN          0
#define E2POOM_CAMDLG_MODPOSY3_MAX          100000
#define E2POOM_CAMDLG_MODPOSY3_USE          0
#define E2POOM_CAMDLG_MODPOSX4_MIN          0
#define E2POOM_CAMDLG_MODPOSX4_MAX          100000
#define E2POOM_CAMDLG_MODPOSX4_USE          8500
#define E2POOM_CAMDLG_MODPOSY4_MIN          0
#define E2POOM_CAMDLG_MODPOSY4_MAX          100000
#define E2POOM_CAMDLG_MODPOSY4_USE          8000

#define E2POOM_CAMDLG_CVIMG_POSX1_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX1_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX1_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY1_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY1_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY1_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX2_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX2_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX2_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY2_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY2_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY2_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX3_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX3_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX3_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY3_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY3_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY3_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX4_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX4_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX4_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY4_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY4_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY4_USE       0

class E2proomData
{
public:
    E2proomData();
    ~E2proomData();

    Int32 camdlg_modposX1;
    Int32 camdlg_modposY1;
    Int32 camdlg_modposX2;
    Int32 camdlg_modposY2;
    Int32 camdlg_modposX3;
    Int32 camdlg_modposY3;
    Int32 camdlg_modposX4;
    Int32 camdlg_modposY4;

    Int32 camdlg_cvimg_posX1;
    Int32 camdlg_cvimg_posY1;
    Int32 camdlg_cvimg_posX2;
    Int32 camdlg_cvimg_posY2;
    Int32 camdlg_cvimg_posX3;
    Int32 camdlg_cvimg_posY3;
    Int32 camdlg_cvimg_posX4;
    Int32 camdlg_cvimg_posY4;

    void write_camdlg_para();
    void init_camdlg_para();

    Int32 camdlg_modposX1_min;
    Int32 camdlg_modposX1_max;
    Int32 camdlg_modposX1_use;
    Int32 camdlg_modposY1_min;
    Int32 camdlg_modposY1_max;
    Int32 camdlg_modposY1_use;
    Int32 camdlg_modposX2_min;
    Int32 camdlg_modposX2_max;
    Int32 camdlg_modposX2_use;
    Int32 camdlg_modposY2_min;
    Int32 camdlg_modposY2_max;
    Int32 camdlg_modposY2_use;
    Int32 camdlg_modposX3_min;
    Int32 camdlg_modposX3_max;
    Int32 camdlg_modposX3_use;
    Int32 camdlg_modposY3_min;
    Int32 camdlg_modposY3_max;
    Int32 camdlg_modposY3_use;
    Int32 camdlg_modposX4_min;
    Int32 camdlg_modposX4_max;
    Int32 camdlg_modposX4_use;
    Int32 camdlg_modposY4_min;
    Int32 camdlg_modposY4_max;
    Int32 camdlg_modposY4_use;

    Int32 camdlg_cvimg_posX1_min;
    Int32 camdlg_cvimg_posX1_max;
    Int32 camdlg_cvimg_posX1_use;
    Int32 camdlg_cvimg_posY1_min;
    Int32 camdlg_cvimg_posY1_max;
    Int32 camdlg_cvimg_posY1_use;
    Int32 camdlg_cvimg_posX2_min;
    Int32 camdlg_cvimg_posX2_max;
    Int32 camdlg_cvimg_posX2_use;
    Int32 camdlg_cvimg_posY2_min;
    Int32 camdlg_cvimg_posY2_max;
    Int32 camdlg_cvimg_posY2_use;
    Int32 camdlg_cvimg_posX3_min;
    Int32 camdlg_cvimg_posX3_max;
    Int32 camdlg_cvimg_posX3_use;
    Int32 camdlg_cvimg_posY3_min;
    Int32 camdlg_cvimg_posY3_max;
    Int32 camdlg_cvimg_posY3_use;
    Int32 camdlg_cvimg_posX4_min;
    Int32 camdlg_cvimg_posX4_max;
    Int32 camdlg_cvimg_posX4_use;
    Int32 camdlg_cvimg_posY4_min;
    Int32 camdlg_cvimg_posY4_max;
    Int32 camdlg_cvimg_posY4_use;
 /***************************/
    void write();
private:
    void read_para();
    void check_para();
};

#endif // E2PROOMDATA_H
