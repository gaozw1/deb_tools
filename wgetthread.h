#ifndef WGETTHREAD_H
#define WGETTHREAD_H

#include <QThread>
#include <QObject>

class WgetThread : public QObject
{
    Q_OBJECT
public:
    explicit WgetThread(QObject *parent = nullptr);

signals:
    void sig_resChanged(QString);
public slots:
    void requestUrl(QString);
};

#endif // WGETTHREAD_H
