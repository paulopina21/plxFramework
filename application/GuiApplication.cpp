#include "GuiApplication.h"

CGuiApplication::CGuiApplication(int& argc, char** argv): QGuiApplication(argc, argv)
{
}

void CGuiApplication::setFolder(int role, QString strFolderPath)
{
  m_folders[role] = strFolderPath;
}

QString CGuiApplication::folder(int role)
{
  return m_folders[role];
}
