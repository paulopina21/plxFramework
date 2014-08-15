#ifndef MOUNTPROVIDER_H
#define MOUNTPROVIDER_H

#include <QString>
#include <QList>

struct MountedDevice
{
  QString device;
  QString mount;
  QString filesystem;
};

enum ListMode
{
  LISTALL = 0,
  LIST_REMOVABLE_ONLY = 1,
  LIST_NORMAL = 2
};

typedef QList<MountedDevice> MountedDevices;

class MountProvider
{
  static MountProvider* INSTANCE;
public:
  MountProvider();
  static MountProvider* instance() { return INSTANCE; }

  static MountedDevices getDrives(ListMode listMode = LIST_NORMAL);

};

#endif // MOUNTPROVIDER_H
