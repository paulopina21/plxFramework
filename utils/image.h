#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QtQml>
#include <QImage>

class CImage: public QObject
{
  Q_OBJECT
public:
  CImage(QObject *parent = 0) : QObject(parent) { qmlRegisterType<CImage>("utils", 1, 0, "Image");}

  Q_PROPERTY(QString full READ image WRITE setImage NOTIFY imageChanged)
  QString image() const { return m_strImage; }

  Q_PROPERTY(QString thumb READ thumb WRITE setThumb NOTIFY thumbChanged)
  QString thumb() const { return m_strThumb; }

  void setImage(const QString& strImage) { m_strImage = strImage; emit imageChanged(); }
  void setThumb(const QString& strThumb) { m_strThumb = strThumb; emit thumbChanged(); }

signals:
  void imageChanged();
  void thumbChanged();

private:
  QString m_strImage;
  QString m_strThumb;
};

namespace Image
{
  bool save(const QString& strFilePath, QByteArray bytes, QSize size = QSize(), Qt::AspectRatioMode mode = Qt::KeepAspectRatio);
}

//QML_DECLARE_TYPE(CImage)

#endif // IMAGE_H
