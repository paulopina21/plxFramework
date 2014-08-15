#include "mountprovider.h"
#include <QRegExp>
#include <QDebug>
#include <QStringList>

#include "../utils/log.h"

MountProvider* MountProvider::INSTANCE = 0;

MountProvider::MountProvider()
{
  INSTANCE = this;
}

MountedDevices MountProvider::getDrives(ListMode listMode)
{
  MountedDevices list;
  FILE* pipe = popen("mount", "r");
  if (!pipe) return list;

  QRegExp reMount;

#if defined(Q_OS_MAC)
  reMount.setPattern("(.+) on (.+) \\(([^,]+)");
  CLog::log(LOG_DEBUG, "Mounts Provider", "Mac pattern detected");
#endif

#ifdef Q_OS_ANDROID
  reMount.setPattern("(.+) on (.+) type ([^ ]+)");
  CLog::log(LOG_DEBUG, "Mounts Provider", "Android pattern detected");
#else
  reMount.setPattern("(.+) on (.+) type ([^ ]+)");
  CLog::log(LOG_DEBUG, "Mounts Provider", "Linux pattern detected");
#endif

  char line[1024];
  while (fgets(line, sizeof(line) - 1, pipe))
  {
    if (reMount.indexIn(line) == -1) continue;
//    qDebug() << "LINE: " << line;
    CLog::log( LOG_DEBUG, "Mounts Provider", "line: " + QString(line) );

    MountedDevice device;
    device.device = reMount.cap(1);
    device.mount = reMount.cap(2);
    device.filesystem = reMount.cap(3);

    if (listMode != LISTALL)
    {
      if (device.mount == "/")
        continue;

      if (device.mount == "/net")
        continue;

      if (device.device == "devfs")
        continue;
    }

    if (listMode == LIST_REMOVABLE_ONLY)
    {

//#if defined(Q_OS_MAC)
//  if (!device.mount.contains("/Volumes/"))
//    continue;
//#endif

//#ifdef Q_OS_ANDROID
//  if (!device.mount.contains("/mnt/"))
//    continue;
//#else
//  if (!device.mount.contains("/media/"))
//    continue;
//#endif
    }
    CLog::log( LOG_INFO, "Mounts Provider", "Device: " + reMount.cap(1) );
    CLog::log( LOG_INFO, "Mounts Provider", "Mount Point: " + reMount.cap(2) );
    CLog::log( LOG_INFO, "Mounts Provider", "Filesystem: " + reMount.cap(3) );

    list << device;
  }

  pclose(pipe);

  return list;
}
