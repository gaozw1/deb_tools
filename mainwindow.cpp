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
    //退出线程
    connect(this,&MainWindow::sig_quitThread,[=]{
        if(thread->isRunning() == false){
            return;
        }
        thread->quit();
        thread->wait();
    });
    connect(this,&MainWindow::destroyed,[=]{
        if(thread->isRunning() == false){
            return;
        }
        thread->quit();
        thread->wait();
        delete wgetThread;
    });
    connect(wgetThread,&WgetThread::sig_resChanged,this,[=](QString res){
        ui->statusBar->clearMessage();
        //输出包名
        if(res.isEmpty()){
            ui->statusBar->showMessage("url不可用，请重新输入！",5000);
            qInfo() << "结果为空";
            emit sig_quitThread();
            return;
        }
        ui->text_fullPackageName->setText(res);
        qInfo() << "res:" << res;
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
        emit sig_quitThread();
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearAll()
{
    ui->textEdit->clear();
    ui->text_name->clear();
    ui->text_fullPackageName->clear();
}

QString MainWindow::compareVersion()
{
    QProcess process;
    QString version1 = ui->lineEdit_v1->text();
    QString version2 = ui->lineEdit_v2->text();
    qInfo() << version1 << "-----" << version2;
    process.start("bash", QStringList() << "-c"
                  << QString("if dpkg --compare-versions %1 gt %2;then echo '%1 > %2';"
                             "elif dpkg --compare-versions %1 lt %2;then echo '%1 < %2';"
                             "elif dpkg --compare-versions %1 eq %2;then echo '%1 = %2';"
                             "else echo 无法比较;"
                             "fi").arg(version1).arg(version2));

    process.waitForFinished();
    return process.readAllStandardOutput();
}

void MainWindow::on_btn_check_clicked()
{
    clearAll();
    qInfo() << "check";
    if(ui->lineEdit->text().isEmpty()){
        ui->statusBar->showMessage("url为空，请输入url！",5000);
        qInfo() << "url is Empty";
        return;
    }
    if(thread->isRunning() == true){
        return;
    }
    thread->start();
    ui->statusBar->showMessage("检查中，请稍后...");
    emit sig_startThread(ui->lineEdit->text());
}

void MainWindow::on_btn_copy_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty()){
        ui->statusBar->showMessage("无内容可复制",2000);
        return;
    }
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->textEdit->toPlainText());
    ui->statusBar->showMessage("复制成功",1000);
}

void MainWindow::on_btn_compare_clicked()
{
    ui->label_msg->setText(compareVersion());
}

