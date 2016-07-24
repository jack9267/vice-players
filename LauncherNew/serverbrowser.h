#ifndef SERVERBROWSER_H
#define SERVERBROWSER_H

#include <QMainWindow>

#include <windows.h>
#include <QThread>
#include <QtCore>
#include <QtNetwork>
#include <QtGui>

namespace Ui {
    class ServerBrowser;
}

class ServerBrowser : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerBrowser(QWidget *parent = 0);
    ~ServerBrowser();

    void MasterListStartup();
    void Query(const char * szIp, unsigned short sPort, char cCommand);

    bool InjectLibraryIntoProcess(DWORD dwProcessId, char * szLibraryPath);
    bool GetProcessIdFromProcessName(wchar_t * szProcessName, DWORD * dwProcessId);

private:
    Ui::ServerBrowser *ui;
    bool bWinsockInitialized;
    SOCKET m_Socket;
    QHttp * http;
    int selected;
    QString m_szFav;

private slots:
    void LoadFavourites();
    void SaveFavourites();
    void AddFavourite();
    void DeleteFavourite(QString address);
    void ListenThreadProc();
    void httpDone();
    void Refresh();
    void onConnect();
    void onDoubleClick(QTreeWidgetItem*,int);
    void on_tabWidget_currentChanged(QWidget* );
};

#endif // SERVERBROWSER_H
