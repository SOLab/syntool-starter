#ifndef GRANULEACTIONS_H
#define GRANULEACTIONS_H

#include <QDesktopServices>
#include <QUrl>

inline void granuleActions(QString serverName, QString currentGranuleId, QString methodName){
    QDesktopServices::openUrl(QUrl(serverName + "/Download.ashx?granule="
                                   +currentGranuleId+"&method="+methodName));
    return;
}

#endif // GRANULEACTIONS_H
