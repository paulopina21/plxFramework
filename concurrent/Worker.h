#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QRunnable>
#include <QVariant>
//#include "../models/ListItem.h"

class CWorker: public QObject, public QRunnable
{
  Q_OBJECT
public:
  CWorker(QObject* parent = 0): QObject(parent) {}

public slots:
  virtual void run() = 0;

signals:
  void started(QVariant what);
  void progress(QVariant progress);
  void finished(QVariant result);
  void error(QVariant err);
};

#endif // WORKER_H
