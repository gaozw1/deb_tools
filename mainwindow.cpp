#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    statusLabel(new QLabel)
{
    ui->setupUi(this);
//    ui->lineEdit->setText("https://aptly.uniontech.com/pkg/security-update/release-candidate/5q2m5rGJ5Zyw5Yy65qGM6Z2i54mIQ1ZF5a6J5YWo5ryP5rSeMjItNC0zMjAyMi0wNC0yMCAyMDozNDoxNw/pool/");

    wgetThread = new WgetThread;
    thread = new QThread(this);
    wgetThread->moveToThread(thread);
    connect(this,&MainWindow::sig_startThread,wgetThread,&WgetThread::requestUrl);
    connect(wgetThread,&WgetThread::sig_resChanged,this,[=](QString res){
        //输出包名
        ui->text_fullPackageName->setText(res);
        statusBarMessage = QString("共%1个包").arg(ui->text_fullPackageName->document()->lineCount()-1);
        statusLabel->setText(statusBarMessage);
        ui->statusBar->addPermanentWidget(statusLabel);

        //输出软件名
        QStringList strList;
        foreach(QString it,res.split("\n",QString::SkipEmptyParts)){
            if(!strList.contains(it.mid(0,it.indexOf("_")))){
                strList.append(it.mid(0,it.indexOf("_")));
            }
        }
        foreach (QString it, strList) {
            ui->text_name->append(it);
        }

        //输出命令
        ui->textEdit->setText("sudo apt install -y "+ui->text_name->toPlainText().replace("\n"," "));

    });

    //关闭窗口退出线程
    connect(this,&MainWindow::destroyed,[=]{
        if(thread->isRunning() == false){
            return;
        }
        thread->quit();
        thread->wait();
        delete wgetThread;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_check_clicked()
{
    qInfo() << "check";
    if(thread->isRunning() == true){
        return;
    }
    thread->start();
    emit sig_startThread(ui->lineEdit->text());
    emit sig_startCountDown(7);

}

void MainWindow::on_btn_copy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->textEdit->toPlainText());
    ui->statusBar->showMessage("复制成功",1000);
//    ui->statusBar->showMessage(statusBarMessage);
}
