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
