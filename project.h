#ifndef PROJECT_H
#define PROJECT_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>
#include "global.h"
#include "craft.h"
#if _MSC_VER||WINDOWS_TCP
#include <QDir>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#define PROJECT_PATH_MOTO       "./SAVE/path.bsd"

#define PROJECT_ID_TOTAL_NUM       1      //项目类型总数
typedef enum PROJECT_ID
{
    PROGECT_ID_TEACH_SCAN=0,       //示教扫描类型
}Project_ID;

class Project
{
public:
    static Project *Get();

    QString project_path;       //最后一次项目路径

    Project_ID project_Id;      //项目总类型id

    QString project_Id_toQString(Project_ID project_Id); //项目总类型id转QString输出

    QString project_name;       //项目名

    std::vector<QString> project_cmdlist;  //项目指令集合

    int SaveProject(QString filename);    //保存项目

    int LoadProject(QString filename);    //读取项目

    int SaveProjectPath(QString filename);    //保存最后一次项目路径，以便下次打开

    int LoadProjectPath(QString filename);    //读取最后一次项目路径，以便下次打开

    std::vector<Point_robpos_result> project_robpos_trace;  //点轨迹

    std::vector<Coord_Matrix4d_result> project_coord_matrix4d;  //零点定位矩阵

    std::vector<Scan_trace_result> project_scan_trace;  //扫描轨迹

    std::vector<Weld_trace_result> project_weld_trace;  //跟踪轨迹

    std::vector<Weld_tracing_result> project_interweld_trace; //焊接跟踪工艺轨迹

    std::vector<Weld_crafts_result> project_crafts_trace; //创建跟踪工艺

protected:
    Project();
    ~Project();

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码

    QVariantHash encoedpath_json(); //把项目路径进行json编码

    int decoedpath_json(QByteArray allData);//把项目路径进行json解码

    QString JsonToQstring(QJsonObject jsonObject);

    QJsonObject QstringToJson(QString jsonString);
};

#endif // PROJECT_H
