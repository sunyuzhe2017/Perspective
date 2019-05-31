#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Image Perspective tool-2019.03.10"));
    label = new QLabel(ui->scrollArea);
    //构造函数中注册事件过滤器
    //ui->labelimage->installEventFilter(this);
    //QObject::connect(this,SIGNAL(button_click_update()),this,SLOT(drawRedCircle()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>Image Perspective:1.0 </h2><p>Copyright 龙性的腾飞 </p>"
                                                "<p>School of Electrical and Information Engineering,Tianjin University</p>"));
}

void MainWindow::on_actionSpecifications_triggered() {
    QMessageBox::about(this, tr("Specifications"),tr("<h2>Steps as follows： </h2><p>1.Open a picture, do not contain chinese characters in the path. </p>"
                                                     "<p>2. Press 'Choose Corners' button and click four corners of the object you want to perspective transform in a clockwise. </p>"
                                                     "<p>3. Press 'Perspective Transform' button, then save the picture transformed.</p>"));
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->labelimage && event->type() == QEvent::Paint)
        drawRedCircle();
    return QWidget::eventFilter(watched, event);

}

void MainWindow::drawRedCircle()
{
    //在label上绘制图形---->一个红色圆点
    QPainter painter(ui->labelimage);
    painter.drawLine (QPoint(0,0),QPoint(100,100));

    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(QColor(255,0,0));
    painter.drawLine (QPoint(0,0),QPoint(100,100));

    qDebug() << "left click";
    painter.drawEllipse(point_x,point_y,4,4);
    qDebug() << point_x;

    //update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 如果是鼠标左键按下
    //---------------------
    //作者：祥知道
    //来源：CSDN
    //原文：https://blog.csdn.net/humanking7/article/details/80707591
    //版权声明：本文为博主原创文章，转载请附上博文链接！
    if (event->button() == Qt::LeftButton && ui->lineEdit_LB->isEnabled()){
        QPoint global_pos = event->globalPos();
        QPoint label_pos = label->mapFromGlobal(global_pos);
        point_x = label_pos.x();
        point_y = label_pos.y();

        QString x = QString::number(label_pos.x(), 10);
        QString y = QString::number(label_pos.y(), 10);
        QString point = "("+x+","+y+")";
        if (ui->pushButton_choose->isChecked()){
            count += 1;
            update();
            //emit button_click_update();
            //drawRedPoint(point_x,point_y);
            switch (count) {
            case 1:
                ui->lineEdit_LT->setText(point);
                point_lt.x = label_pos.x();
                point_lt.y = label_pos.y();
                break;
            case 2:
                ui->lineEdit_RT->setText(point);
                point_rt.x = label_pos.x();
                point_rt.y = label_pos.y();
                break;
            case 3:
                ui->lineEdit_RB->setText(point);
                point_rb.x = label_pos.x();
                point_rb.y = label_pos.y();
                break;
            case 4:
                ui->lineEdit_LB->setText(point);
                point_lb.x = label_pos.x();
                point_lb.y = label_pos.y();
                break;
            }
        }
        //qDebug() << "left click"<<event->pos()<<"global:"<<event->globalPos()<<"label_pos:"<<label_pos;
    }
}

void MainWindow::on_pushButton_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Image"),"/home/sun/",tr("Image File (*.jpg *.png *.bmp)"));
    qDebug()<<"filenames:"<<filename;

    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()
    image = cv::imread(name);
    QString w = QString::number(image.size().width, 10);
    QString h = QString::number(image.size().height, 10);

    QString w_h = " width: "+w+" height: "+h;
    //https://blog.csdn.net/ei__nino/article/details/7297791
    //QString s = QString::number(a, 10);             // s == "63"
    ui->statusBar->showMessage(filename+w_h);

    if(!image.data)
    {
        ui->pushButton_perspective->setEnabled(false);
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else {
        //DisplayMat(image);
        //https://blog.csdn.net/xiezhongyuan07/article/details/79621467
        //https://blog.csdn.net/qq_37233607/article/details/79082243
        ui->pushButton_perspective->setEnabled(true);
        QImage qimage(filename);
        int q_width = qimage.size().width();
        int q_height = qimage.size().height();
        if (q_width>640 || q_height>480 )
        {
            //label = new QLabel(ui->scrollArea);
            cv::resize(image,image,Size(640,q_height*640/q_width));
            //sr= image.resize(640);
            //resize(image, src_resize, Size(640,q_height*640/q_width));
            label->setPixmap(QPixmap::fromImage(qimage).scaled(QSize(640,q_height*640/q_width),Qt::KeepAspectRatio));
            label->setAlignment(Qt::AlignTop);
            ui->scrollArea->setWidget(label);
            qDebug()<<"labelsize:"<<qimage.size();
        }
        else{
            label = new QLabel(ui->scrollArea);
            label->setPixmap(QPixmap::fromImage(qimage));
            label->setAlignment(Qt::AlignTop);
            ui->scrollArea->setWidget(label);
        }
    }
}

void MainWindow::DisplayMat(cv::Mat image)
{
    cv::Mat rgb;
    QImage img;
    if(image.channels() == 3)
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_RGB888);
    }
    QSize label_resize = QSize(image.size().width,image.size().height);
    //qDebug()<<"imagesize:"<<image.size();
    ui->labelimage->setPixmap(QPixmap::fromImage(img).scaled(label_resize));//setPixelmap(QPixmap::fromImage(img));
    ui->labelimage->resize(ui->labelimage->pixmap()->size());//resize(ui->label->pixmap()->size());
    //ui->scrollArea->setWidget(ui->labelimage);会出现崩溃，必须QLabel创建才可以
    //qDebug()<<"labelsize:"<<ui->labelimage->size();

}
//---------------------
//作者：call_me_yang
//来源：CSDN
//原文：https://blog.csdn.net/yang6464158/article/details/38109415
//版权声明：本文为博主原创文章，转载请附上博文链接！

void MainWindow::on_pushButton_choose_clicked(bool checked)
{
    if(checked){
        ui->pushButton_choose->setText("选择结束");
        count = 0;
        ui->lineEdit_LT->setEnabled(true);
        ui->lineEdit_LT->clear();
        ui->lineEdit_RT->setEnabled(true);
        ui->lineEdit_RT->clear();
        ui->lineEdit_RB->setEnabled(true);
        ui->lineEdit_RB->clear();
        ui->lineEdit_LB->setEnabled(true);
        ui->lineEdit_LB->clear();
    }
    else
    { ui->pushButton_choose->setText("选择角点"); }
}
/*void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    qDebug()<<"labelsize";
    QPen pen(Qt::red,5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawLine (QPoint(0,0),QPoint(100,100));
    //must use QImage
}*/
Mat MainWindow::perspectivetransfer(Mat srcImage)
{   //选择的待变换角点
    Point2f pts_src[] = { point_lt,point_rt,point_rb,point_lb};
    //透视变换后的角点
    int height = ui->lineEdit_height->text().toInt();
    int width = ui->lineEdit_width->text().toInt();
    Point2f pts_dst[] = {Point(10, 10),Point(10+width, 10),
    Point(10+width, 10+height) ,Point(10, 10+height) };
    Mat &&M = cv::getPerspectiveTransform(pts_src, pts_dst);
    std::cout<<"M:"<<M<<std::endl;
    Mat warp;
    Mat PerspectivedImg(height+20, width+20, CV_8UC1);
    PerspectivedImg.setTo(0);
    cv::warpPerspective(srcImage, warp, M, PerspectivedImg.size(), cv::INTER_LINEAR , cv::BORDER_REPLICATE);
    //imshow("After Perspectived", warp);
    return warp;
}

void MainWindow::on_pushButton_perspective_clicked()
{
    save_image = perspectivetransfer(image);
    DisplayMat(save_image);
}

void MainWindow::on_pushButton_save_clicked()
{
    QString filename =QFileDialog::getSaveFileName(this,
    tr("Save Image..."),"/home/sun/deeplearning",tr("Image File (*.jpg *.png *.bmp)"));
    qDebug()<<"filenames:"<<filename;
    std::string After_perspective = filename.toStdString();
    imwrite(After_perspective,save_image);
}
