#include "wgetthread.h"
#include <QProcess>
#include <QDebug>

WgetThread::WgetThread(QObject *parent) : QObject(parent)
{

}

void WgetThread::requestUrl(QString url)
{
    qInfo()<< "url:" << url;
    QProcess process;
    process.start("bash", QStringList() << "-c"
                  << QString("wget --spider -r -l0 -np -nd -R xz,dsc,udeb %1 2>&1 |grep deb.tmp|awk '{print $2}'|sed 's/.....$//'").arg(url));

    process.waitForFinished();
    emit sig_resChanged(process.readAllStandardOutput());
}



