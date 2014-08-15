#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QtQml>
#include <QList>



class CSettings : public QSettings
{
  Q_OBJECT
  static CSettings* INSTANCE;
public:
  enum eFOLDERS
  {
    FOLDER_STORAGE = 0,
    FOLDER_PERSISTENT_STORAGE,
    FOLDER_LOG,
    FOLDER_TEMP,
    FOLDER_ASSETS,
    FOLDER_DATABASE,
    FOLDER_ALBUMS,
    FOLDER_FANARTS
  };


  explicit CSettings(QObject * parent = 0);
  explicit CSettings(const QString & fileName, QSettings::Format format = QSettings::NativeFormat, QObject * parent = 0);
  static CSettings* instance();

signals:
  
public slots:
//  void setValue(const QString& strGroup, const QString& strName, const QVariant& value);
//  QVariant getValue(const QString& strGroup, const QString& strName, const QVariant &defaultValue = "");
//  QVariant getValue(const QString& strField, const QVariant& defaultValue = "");

  static void setFolder( int role, QString strFolderPath );
  static QString folder(int role);

private:
  QMap<int, QString> m_folders;

};

#endif // APPLICATIONSETTINGS_H
