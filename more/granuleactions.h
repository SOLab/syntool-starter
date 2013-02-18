#ifndef GRANULEACTIONS_H
#define GRANULEACTIONS_H

#include <QDesktopServices>
#include <QUrl>

inline void granuleActions(QString serverName, QString currentGranuleId, QString methodName){
    QDesktopServices::openUrl(QUrl(serverName + "/Download.ashx?granule="
                                   +currentGranuleId+"&method="+methodName));
    return;
    // Request URL = http://staging.satin.rshu.ru/Download.ashx?granule=<granule_id>&method=[ftp|opendap|image|kml]
}

#endif // GRANULEACTIONS_H
