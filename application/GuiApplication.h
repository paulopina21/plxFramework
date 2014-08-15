#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H

#include <QGuiApplication>
#include <QObject>
#include <QMap>

class CGuiApplication: public QGuiApplication
{
  Q_OBJECT
public:
  CGuiApplication(int & argc, char ** argv);

public slots:
  void setFolder( int role, QString strFolderPath );
  QString folder(int role);

  virtual void setUp() = 0;
signals:
  void loading(QVariant what);
  void loaded(QVariant what);
  void progress(QVariant what);

private:
  QMap<int, QString> m_folders;
};


#endif // APPLICATION_H
