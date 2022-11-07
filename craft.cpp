#include "craft.h"

Craft *Craft::Get()
{
    static Craft fun;
    return &fun;
}

Craft::Craft()
{

}

Craft::~Craft()
{

}

QString Craft::craft_Id_toQString(Craft_ID craft_id)
{
    QString msg;
    switch(craft_id)
    {
        case CRAFT_ID_FIXED_POSTURE:
            msg=QString::fromLocal8Bit("固定焊接姿态");
        break;
    }
    return msg;
}

int Craft::LoadCraft(char* filename)    //读取工艺
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to load JSON file";
        return 1;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    if(0!=decoed_json(allData))
        return 2;

    return 0;
}

int Craft::decoed_json(QByteArray allData)
{
    //这里添加解码

    return 0;
}
