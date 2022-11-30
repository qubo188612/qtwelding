#include "showtasknumdlg.h"
#include "ui_showtasknumdlg.h"

showtasknumdlg::showtasknumdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showtasknumdlg)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止用户修改
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设为整行选中
}

showtasknumdlg::~showtasknumdlg()
{
    delete ui;
}

void showtasknumdlg::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);   //可在QPaintDevice上绘制各种图形。QPaintDevice有之类QWidget、QImage、QOpenGLPaintDevice等
    //绘图前准备画笔、画刷
    QPen pen1; //画笔。绘制图形边线，由颜色、宽度、线风格等参数组成
    pen1.setColor(QColor(255,0,0,255));
    pen1.setWidth(5);
    painter.setPen(pen1);
    painter.drawLine(QPoint(130,650),QPoint(260,650));   //QPainter绘制直线

    QPen pen2;
    pen2.setColor(QColor(0,255,0,255));
    pen2.setWidth(5);
    QVector<qreal>dashes;
    qreal space = 4;
    dashes << 3 << space;
    pen2.setDashPattern(dashes);
    painter.setPen(pen2);
    painter.drawLine(QPoint(130,680),QPoint(260,680));   //QPainter绘制直线

    QPen pen3; //画笔。绘制图形边线，由颜色、宽度、线风格等参数组成
    pen3.setColor(QColor(0,0,255,255));
    pen3.setWidth(3);
    painter.setPen(pen3);
    painter.drawEllipse(182,700,25,25);
}

void showtasknumdlg::on_tableWidget_cellClicked(int row, int column)
{
    QString str = ui->tableWidget->item(row,0)->text();
    int task_num = str.toInt();
    image_draw(task_num);
}

void showtasknumdlg::showimage(cv::Mat pic)
{
    if (pic.empty()==0)
    {
        QImage::Format format = QImage::Format_RGB888;
        switch (pic.type())
        {
        case CV_8UC1:
          format = QImage::Format_Indexed8;
          break;
        case CV_8UC3:
          format = QImage::Format_RGB888;
          break;
        case CV_8UC4:
          format = QImage::Format_ARGB32;
          break;
        }
        QImage img = QImage((const uchar*)pic.data, pic.cols, pic.rows,
        pic.cols * pic.channels(), format);
        img = img.scaled(ui->labelshow->width(),ui->labelshow->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//图片自适应lab大小
        ui->labelshow->setPixmap(QPixmap::fromImage(img));
    }
}

int showtasknumdlg::GetLinefocal(L_line lineIn1,L_line lineIn2,cv::Point *focal)
{
    double line1_x1=lineIn1.st.x;
    double line1_y1=lineIn1.st.y;
    double line1_x2=lineIn1.ed.x;
    double line1_y2=lineIn1.ed.y;
    double line2_x1=lineIn2.st.x;
    double line2_y1=lineIn2.st.y;
    double line2_x2=lineIn2.ed.x;
    double line2_y2=lineIn2.ed.y;

    double A1,B1,C1,A2,B2,C2;
    double M;

    A1=line1_y2-line1_y1;
    B1=line1_x1-line1_x2;
    C1=line1_y1*line1_x2-line1_x1*line1_y2;

    A2=line2_y2-line2_y1;
    B2=line2_x1-line2_x2;
    C2=line2_y1*line2_x2-line2_x1*line2_y2;

    if((lineIn1.st.x==lineIn1.ed.x)&&(lineIn1.st.y==lineIn1.ed.y))
    {
        return 1;
    }
    if((lineIn2.st.x==lineIn2.ed.x)&&(lineIn2.st.y==lineIn2.ed.y))
    {
        return 2;
    }

    M=A1*B2-A2*B1;
    if(M==0)
    {
        return 3;
    }
    else
    {
        focal->x=(int)((double)(C2*B1-C1*B2)/M+0.5);
        focal->y=(int)((double)(C1*A2-C2*A1)/M+0.5);
    }
    return 0;
}

// 画由点组成的虚线
void showtasknumdlg::draw_dotted_line1(cv::Mat img, cv::Point2f p1, cv::Point2f p2, cv::Scalar color, int thickness)
{
    float n = 15; //虚点间隔
    float w = p2.x - p1.x, h = p2.y - p1.y;
    float l = sqrtf(w * w + h * h);
    int m = l / n;
    n = l / m; // 矫正虚点间隔，使虚点数为整数

    circle(img, p1, 1, color, thickness); // 画起点
    circle(img, p2, 1, color, thickness); // 画终点
    // 画中间点
    if (p1.y == p2.y) // 水平线：y = m
    {
        float x1 = cv::min(p1.x, p2.x);
        float x2 = cv::max(p1.x, p2.x);
        for (float x = x1 + n; x < x2; x = x + n)
            circle(img, cv::Point2f(x, p1.y), 1, color, thickness);
    }
    else if (p1.x == p2.x) // 垂直线, x = m
    {
        float y1 = cv::min(p1.y, p2.y);
        float y2 = cv::max(p1.y, p2.y);
        for (float y = y1 + n; y < y2; y = y + n)
            circle(img, cv::Point2f(p1.x, y), 1, color, thickness);
    }
    else // 倾斜线，与x轴、y轴都不垂直或平行
    {
        // 直线方程的两点式：(y-y1)/(y2-y1)=(x-x1)/(x2-x1) -> y = (y2-y1)*(x-x1)/(x2-x1)+y1
        float m = n * abs(w) / l;
        float k = h / w;
        float x1 = cv::min(p1.x, p2.x);
        float x2 = cv::max(p1.x, p2.x);
        for (float x = x1 + m; x < x2; x = x + m)
            circle(img, cv::Point2f(x, k * (x - p1.x) + p1.y), 1, color, thickness);
    }
}

// 画由线组成的虚线
void showtasknumdlg::draw_dotted_line2(cv::Mat img, cv::Point2f p1, cv::Point2f p2, cv::Scalar color, int thickness)
{
    float n = 15; //线长度
    float w = p2.x - p1.x, h = p2.y - p1.y;
    float l = sqrtf(w * w + h * h);
    // 矫正线长度，使线个数为奇数
    int m = l / n;
    m = m % 2 ? m : m + 1;
    n = l / m;

    circle(img, p1, 1, color, thickness); // 画起点
    circle(img, p2, 1, color, thickness); // 画终点
    // 画中间点
    if (p1.y == p2.y) //水平线：y = m
    {
        float x1 = cv::min(p1.x, p2.x);
        float x2 = cv::max(p1.x, p2.x);
        for (float x = x1, n1 = 2 * n; x < x2; x = x + n1)
            line(img, cv::Point2f(x, p1.y), cv::Point2f(x + n, p1.y), color, thickness);
    }
    else if (p1.x == p2.x) //垂直线, x = m
    {
        float y1 = cv::min(p1.y, p2.y);
        float y2 = cv::max(p1.y, p2.y);
        for (float y = y1, n1 = 2 * n; y < y2; y = y + n1)
            line(img, cv::Point2f(p1.x, y), cv::Point2f(p1.x, y + n), color, thickness);
    }
    else // 倾斜线，与x轴、y轴都不垂直或平行
    {
        // 直线方程的两点式：(y-y1)/(y2-y1)=(x-x1)/(x2-x1) -> y = (y2-y1)*(x-x1)/(x2-x1)+y1
        float n1 = n * abs(w) / l;
        float k = h / w;
        float x1 = cv::min(p1.x, p2.x);
        float x2 = cv::max(p1.x, p2.x);
        for (float x = x1, n2 = 2 * n1; x < x2; x = x + n2)
        {
            cv::Point p3 = cv::Point2f(x, k * (x - p1.x) + p1.y);
            cv::Point p4 = cv::Point2f(x + n1, k * (x + n1 - p1.x) + p1.y);
            line(img, p3, p4, color, thickness);
        }
    }
}

void showtasknumdlg::image_draw(int task_num)
{
    int nnHeight=DRAW_IMAGE_HEIGHT;
    int nnWidth=DRAW_IMAGE_WIDTH;
    cv::Mat image=cv::Mat::zeros(nnHeight,nnWidth,CV_8UC3);
    switch(task_num)
    {
    case 100:
    {
        //内角缝
        L_line line1,line2;
        line1.st.y=nnHeight/16.0;
        line1.ed.y=nnHeight/2.0-nnHeight/16.0;
        line1.st.x=nnWidth/16.0*15;
        line1.ed.x=nnWidth/16.0*5;
        cv::line(image,line1.st,line1.ed,cv::Scalar(255,0,0),9);
        line2.st.y=nnHeight/16.0*15;
        line2.ed.y=nnHeight/2.0+nnHeight/16.0;
        line2.st.x=nnWidth/16.0*15;
        line2.ed.x=nnWidth/16.0*5;
        cv::line(image,line2.st,line2.ed,cv::Scalar(255,0,0),9);
        cv::Point focal;
        GetLinefocal(line1,line2,&focal);
        cv::Point2f p1=focal;
        cv::Point2f p2=line2.ed;
        draw_dotted_line2(image,p1,p2,cv::Scalar(0,255,0),3);
        cv::Point2f p3=focal;
        cv::Point2f p4=line1.ed;
        draw_dotted_line2(image,p3,p4,cv::Scalar(0,255,0),3);
        cv::circle(image,focal,20,cv::Scalar(0,0,255),3);
        cv::Point2f p1_1=p1;
        p1_1.y=p1_1.y-25;
        cv::putText(image,"1",p1_1,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
        cv::circle(image,p2,20,cv::Scalar(0,0,255),3);
        cv::Point2f p2_2=p2;
        p2_2.y=p2_2.y-25;
        cv::putText(image,"2",p2_2,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
        cv::circle(image,p4,20,cv::Scalar(0,0,255),3);
        cv::Point2f p3_3=p4;
        p3_3.y=p3_3.y-25;
        cv::putText(image,"3",p3_3,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
    }
    break;
    case 101:
    {
        //外角缝
        L_line line1,line2;
        line1.st.y=nnHeight/16.0;
        line1.ed.y=nnHeight/2.0-nnHeight/16.0;
        line1.st.x=nnWidth/16.0;
        line1.ed.x=nnWidth/16.0*11;
        cv::line(image,line1.st,line1.ed,cv::Scalar(255,0,0),9);
        line2.st.y=nnHeight/16.0*15;
        line2.ed.y=nnHeight/2.0+nnHeight/16.0;
        line2.st.x=nnWidth/16.0;
        line2.ed.x=nnWidth/16.0*11;
        cv::line(image,line2.st,line2.ed,cv::Scalar(255,0,0),9);
        cv::Point focal;
        GetLinefocal(line1,line2,&focal);
        cv::Point2f p1=focal;
        cv::Point2f p2=line2.ed;
        draw_dotted_line2(image,p1,p2,cv::Scalar(0,255,0),3);
        cv::Point2f p3=focal;
        cv::Point2f p4=line1.ed;
        draw_dotted_line2(image,p3,p4,cv::Scalar(0,255,0),3);
        cv::circle(image,focal,20,cv::Scalar(0,0,255),3);
        cv::Point2f p1_1=p1;
        p1_1.y=p1_1.y-25;
        cv::putText(image,"1",p1_1,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
        cv::circle(image,p2,20,cv::Scalar(0,0,255),3);
        cv::Point2f p2_2=p2;
        p2_2.y=p2_2.y-25;
        cv::putText(image,"2",p2_2,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
        cv::circle(image,p4,20,cv::Scalar(0,0,255),3);
        cv::Point2f p3_3=p4;
        p3_3.y=p3_3.y-25;
        cv::putText(image,"3",p3_3,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
    }
    break;
    case 102:
    {
        //Z角波
        L_line line1,line2,line3;
        line1.st.y=nnHeight/16.0;
        line1.ed.y=nnHeight/4.0;
        line1.st.x=nnWidth/2.0;
        line1.ed.x=nnWidth/8.0*5;
        cv::line(image,line1.st,line1.ed,cv::Scalar(255,0,0),9);
        line2.st.y=nnHeight/4.0;
        line2.ed.y=nnHeight/2.0-nnHeight/16.0;
        line2.st.x=nnWidth/8.0*5;
        line2.ed.x=nnWidth/2.0;
        cv::line(image,line2.st,line2.ed,cv::Scalar(255,0,0),9);
        line3.st.y=nnHeight/2.0;
        line3.ed.y=nnHeight/16.0*15;
        line3.st.x=nnWidth/2.0;
        line3.ed.x=nnWidth/16.0*15;
        cv::line(image,line3.st,line3.ed,cv::Scalar(255,0,0),9);
        cv::Point focal;
        GetLinefocal(line2,line3,&focal);
        cv::Point2f p1=focal;
        cv::Point2f p2=line2.ed;
        draw_dotted_line2(image,p1,p2,cv::Scalar(0,255,0),3);
        cv::Point2f p3=focal;
        cv::Point2f p4=line3.st;
        draw_dotted_line2(image,p3,p4,cv::Scalar(0,255,0),3);
        cv::circle(image,focal,20,cv::Scalar(0,0,255),3);
        cv::Point2f p1_1=p1;
        p1_1.y=p1_1.y-25;
        p1_1.x=p1_1.x-25;
        cv::putText(image,"1",p1_1,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
    }
    break;
    case 103:
    {
        //轮廓
        L_line line1,line2,line3,line4,line5;
        line1.st.y=nnHeight/16.0;
        line1.ed.y=nnHeight/8.0;
        line1.st.x=nnWidth/2.0;
        line1.ed.x=nnWidth/2.0;
        cv::line(image,line1.st,line1.ed,cv::Scalar(255,0,0),9);
        draw_dotted_line1(image,line1.st,line1.ed,cv::Scalar(0,0,255),3);
        line2.st.y=line1.ed.y;
        line2.ed.y=nnHeight/4.0;
        line2.st.x=line1.ed.x;
        line2.ed.x=nnWidth/4.0*3;
        cv::line(image,line2.st,line2.ed,cv::Scalar(255,0,0),9);
        draw_dotted_line1(image,line2.st,line2.ed,cv::Scalar(0,0,255),3);
        line3.st.y=line2.ed.y;
        line3.ed.y=nnHeight/2.0;
        line3.st.x=line2.ed.x;
        line3.ed.x=nnWidth/4.0*3;
        cv::line(image,line3.st,line3.ed,cv::Scalar(255,0,0),9);
        draw_dotted_line1(image,line3.st,line3.ed,cv::Scalar(0,0,255),3);
        line4.st.y=nnHeight/8.0*5;
        line4.ed.y=nnHeight/4.0*3;
        line4.st.x=nnWidth/3.0;
        line4.ed.x=nnWidth/2.0;
        cv::line(image,line4.st,line4.ed,cv::Scalar(255,0,0),9);
        draw_dotted_line1(image,line4.st,line4.ed,cv::Scalar(0,0,255),3);
        line5.st.y=line4.ed.y;
        line5.ed.y=nnHeight/16.0*15;
        line5.st.x=line4.ed.x;
        line5.ed.x=nnWidth/2.0;
        cv::line(image,line5.st,line5.ed,cv::Scalar(255,0,0),9);
        draw_dotted_line1(image,line5.st,line5.ed,cv::Scalar(0,0,255),3);
        cv::Point focal;
        focal=line1.st;
        cv::circle(image,focal,20,cv::Scalar(0,0,255),3);
        cv::Point2f p1_1=focal;
        p1_1.y=p1_1.y;
        p1_1.x=p1_1.x-75;
        cv::putText(image,"1",p1_1,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
        focal=line5.ed;
        cv::circle(image,focal,20,cv::Scalar(0,0,255),3);
        cv::Point2f p1_2=focal;
        p1_2.y=p1_2.y;
        p1_2.x=p1_2.x-75;
        cv::putText(image,"2",p1_2,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
    }
    break;
    case 104:
    {
        //Z角波
        L_line line1,line2,line3;
        line1.st.y=nnHeight/16.0;
        line1.ed.y=nnHeight/4.0;
        line1.st.x=nnWidth/2.0;
        line1.ed.x=nnWidth/8.0*5;
        cv::line(image,line1.st,line1.ed,cv::Scalar(255,0,0),9);
        line2.st.y=nnHeight/4.0;
        line2.ed.y=nnHeight/2.0-nnHeight/16.0;
        line2.st.x=nnWidth/8.0*5;
        line2.ed.x=nnWidth/2.0;
        cv::line(image,line2.st,line2.ed,cv::Scalar(255,0,0),9);
        line3.st.y=nnHeight/2.0;
        line3.ed.y=nnHeight/16.0*15;
        line3.st.x=nnWidth/2.0;
        line3.ed.x=nnWidth/16.0*15;
        cv::line(image,line3.st,line3.ed,cv::Scalar(255,0,0),9);
        cv::Point focal;
        GetLinefocal(line2,line3,&focal);
        cv::Point2f p1=focal;
        cv::Point2f p2=line2.ed;
        draw_dotted_line2(image,p1,p2,cv::Scalar(0,255,0),3);
        cv::Point2f p3=focal;
        cv::Point2f p4=line3.st;
        draw_dotted_line2(image,p3,p4,cv::Scalar(0,255,0),3);
        cv::circle(image,focal,20,cv::Scalar(0,0,255),3);
        cv::Point2f p1_1=p1;
        p1_1.y=p1_1.y-25;
        p1_1.x=p1_1.x-25;
        cv::putText(image,"1",p1_1,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
    }
    break;
    default:
    break;
    }
    showimage(image);
}

