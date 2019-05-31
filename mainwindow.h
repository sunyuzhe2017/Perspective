#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
using namespace cv;

namespace Ui {
class MainWindow;
}
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void DisplayMat(cv::Mat image);
    void mousePressEvent(QMouseEvent *event);
    //void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    Mat perspectivetransfer(Mat srcImage);

signals:
    void button_click_update();
public slots:
    void drawRedCircle();
    //void drawRedPoint(int x,int y);
    void on_actionAbout_triggered();
    void on_actionSpecifications_triggered();

private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_choose_clicked(bool checked);

    void on_pushButton_perspective_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat image;
    QLabel *label;
    int count;//initial for corners order
    int point_x = 10;
    int point_y = 10;
    Point point_lt,point_rt,point_rb,point_lb;
    Mat src_resize;
    Mat save_image;
};

#endif // MAINWINDOW_H
