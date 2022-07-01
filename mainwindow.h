#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wgetthread.h"
#include <QThread>
#include <QLabel>
#include <QProcess>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void clearAll();

    QString compareVersion();

private slots:
    void on_btn_check_clicked();
    void on_btn_copy_clicked();
    void on_btn_compare_clicked();

protected:


private:
    Ui::MainWindow *ui;
    WgetThread *wgetThread;
    QThread *thread;
    QString statusBarMessage;
    QLabel *statusLabel;
    QString deb_res;

signals:
    void sig_startThread(QString);
    void sig_quitThread();

};

#endif // MAINWINDOW_H
