#ifndef SHOWTASKNUMDLG_H
#define SHOWTASKNUMDLG_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <QPen>
#include <QPoint>
#include <QPaintEvent>
#include <QPainter>

#define DRAW_IMAGE_HEIGHT       960
#define DRAW_IMAGE_WIDTH       1080

namespace Ui {
class showtasknumdlg;
}


class L_line{
public:
    cv::Point st;
    cv::Point ed;
};

class showtasknumdlg : public QDialog
{
    Q_OBJECT

public:
    explicit showtasknumdlg(QWidget *parent = nullptr);
    ~showtasknumdlg();

protected:
    void showimage(cv::Mat pic);
    void image_draw(int task_num);

    int GetLinefocal(L_line lineIn1,L_line lineIn2,cv::Point *focal);
    void draw_dotted_line1(cv::Mat img, cv::Point2f p1, cv::Point2f p2, cv::Scalar color, int thickness);
    void draw_dotted_line2(cv::Mat img, cv::Point2f p1, cv::Point2f p2, cv::Scalar color, int thickness);

    void paintEvent(QPaintEvent *event);

private slots:
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::showtasknumdlg *ui;
};

#endif // SHOWTASKNUMDLG_H
