#ifndef QUICKAPPLICATION_H
#define QUICKAPPLICATION_H

//#include <QQuickView>
#include "GuiApplication.h"


class CQuickApplication : public CGuiApplication//, public QQuickView
{
public:
  CQuickApplication(int & argc, char ** argv);
};

#endif // QUICKAPPLICATION_H
