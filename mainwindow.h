#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wgetthread.h"
#include <QThread>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString deb_res;

private slots:
    void on_btn_check_clicked();
    void on_btn_copy_clicked();

protected:


private:
    Ui::MainWindow *ui;
    WgetThread *wgetThread;
    QThread *thread;
    QString statusBarMessage;
    QLabel *statusLabel;

signals:
    void sig_startThread(QString);
    void sig_startCountDown(int);
};

#endif // MAINWINDOW_H
