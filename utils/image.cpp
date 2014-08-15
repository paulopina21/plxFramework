#include "image.h"

bool Image::save(const QString &strFilePath, QByteArray bytes, QSize size, Qt::AspectRatioMode mode)
{
  qDebug() << "Image -> Saving " << bytes.size() << " bytes to:" <<strFilePath;
  QImage image = QImage::fromData(bytes);

  if (size.isValid())
    image = image.scaled(size, mode, Qt::SmoothTransformation);

  if (!image.save(strFilePath, "jpg")) {
    qDebug() << __FUNCTION__ << ": Couldn`t save: " << strFilePath;
    return false;
  }

  return true;
}
