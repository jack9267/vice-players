#include "serverbrowser.h"
#include "ui_serverbrowser.h"
#include "windows.h"
#include <QTimer>
#include <QtCore>
#include <QtGui>
#include <tlhelp32.h>
#include <stdlib.h>
#include "addfav.h"
#include "ui_addfav.h"

#define MASTERLIST_HOST "master.vc-p.com"
#define MASTERLIST_SERVERS "/list.php"
#define MASTERLIST_SPONSORS "/list.php?class=Sponsors"
#define MASTERLIST_FEATURED "/list.php?class=Hosted"
#define FAVOURITES_FILE "vcp.fav"

ServerBrowser::ServerBrowser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerBrowser)
{
    ui->setupUi(this);
    QStringList labels;
    labels << "Server Name" << "Players" << "Mode" << "Map" << "IP/Port";
    ui->twServers->setHeaderLabels(labels);
    ui->twServers->setColumnWidth(0, 200);
    ui->twServers->setColumnWidth(1, 70);
    ui->twServers->setColumnWidth(2, 170);
    ui->twServers->setColumnWidth(3, 170);
    ui->twServers->setColumnWidth(4, 170);

    ui->twServers->setParent(ui->wServers);
    ui->btnConnect2->setParent(ui->wServers);
    ui->btnRefresh->setParent(ui->wServers);
    ui->btnAddFav->setParent(ui->wServers);

    m_Socket = NULL;
    MasterListStartup();
    http = new QHttp(MASTERLIST_HOST, 80);

    selected = 0;

    QSettings settings("Vice-Players", "Browser");
    ui->leIP->setText(settings.value("IP", "127.0.0.1").toString());
    ui->leNick->setText(settings.value("Nick", "Player").toString());
    ui->lePort->setText(settings.value("Port", "5192").toString());

    connect(http, SIGNAL(done(bool)), this, SLOT(httpDone()));
    connect(ui->twServers, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onDoubleClick(QTreeWidgetItem*,int)) );
    connect(ui->btnRefresh, SIGNAL(released()), this, SLOT(Refresh()));
    connect(ui->btnConnect, SIGNAL(released()), this, SLOT(onConnect()));

    connect(ui->btnAddFav, SIGNAL(clicked()), this, SLOT(AddFavourite()));
    Refresh();
}

const char * GetAppPath ()
{
    QString path = QApplication::applicationDirPath();
    path.replace("/", "\\");
    return "./";
//return path.toStdString().c_str();
}

#define CLIENT_DLL "Client.dll"


void ServerBrowser::onConnect()
{
    QSettings settings("Vice-Players", "Browser");
    settings.setValue("IP", ui->leIP->text().toAscii());
    settings.setValue("Nick", ui->leNick->text().toAscii());
    settings.setValue("Port", ui->lePort->text().toAscii());

    bool windowed = ui->cbWindowed->isChecked();
    const char *szNick = ui->leNick->text().toStdString().c_str();
    const char *szServerIP = ui->leIP->text().toStdString().c_str();
    const char *szPort = ui->lePort->text().toStdString().c_str();
    const char *szPassword = ui->lePassword->text().toStdString().c_str();


    char szParams[1024];

    QString args;

    // Format the command line
    //args.sprintf("\"gta-vc.exe\" -c -h %s -p %s -n %s",ui->leNick->text(),ui->leNick->text(),szNick);
    args = "gta-vc.exe -c -h %1 -p %2 -n %3";
    args.replace("%1", ui->leIP->text());
    args.replace("%2", ui->lePort->text());
    args.replace("%3", ui->leNick->text());

    strcpy(szParams, args.toStdString().c_str());

    // If we have a password add it to the command line
    if(strlen(szPassword) > 0) {
            strcat(szParams," -z ");
            strcat(szParams,szPassword);
    }

    // If the windowed checkbox is checked add a '-w' to the command line
    if(windowed) {
            strcat(szParams, " -w ");
    }

    // Get the library path
    char szLibraryPath[1024];
    sprintf(szLibraryPath, "%s" CLIENT_DLL, GetAppPath());

    // Check if vcp.dll exists
    WIN32_FIND_DATAA fdFileInfo;
    if(FindFirstFileA(szLibraryPath, &fdFileInfo) == INVALID_HANDLE_VALUE) {
        QMessageBox::warning(this, "", "Can't find vcp.dll.");
        return;
    }
    FILE *f = fopen("test1.txt", "w");
    fprintf(f, "%s %s", ui->lePort->text().toAscii(), ui->leIP->text().toAscii());
    fclose(f);
    // Get the gta exe path
    char szGtaExe[1024];
    sprintf(szGtaExe, "%sgta-vc.exe", GetAppPath());

    // Start gta-vc.exe
    STARTUPINFOA siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;
    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));
    siStartupInfo.cb = sizeof(siStartupInfo);
    if(!CreateProcessA(szGtaExe, szParams, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, GetAppPath(), &siStartupInfo,
            &piProcessInfo)) {
            QMessageBox::warning(this, "", "Couldn't launch gta-vc.exe.\nDid you install Vice City: Players to your Vice City directory?");
            //MessageBoxA("Couldn't launch gta-vc.exe.\nDid you install Vice City: Players to your Vice City directory?");
            return;
    }

    // Inject our code into LaunchGTAIV.exe
    if(!InjectLibraryIntoProcess(piProcessInfo.dwProcessId, szLibraryPath)) {
            TerminateProcess(piProcessInfo.hProcess, 0);
            QMessageBox::warning(this, "", "Couldn't inject vcp.dll.");
            //MessageBox("Couldn't inject " CLIENT_DLL ".");
            return;
    }

    // Resume the gta-vc.exe thread
    ResumeThread(piProcessInfo.hThread);
}

void ServerBrowser::onDoubleClick(QTreeWidgetItem* item,int index)
{
    QStringList s = item->text(4).split(":");
    QString ip = s.at(0);
    QString port = s.at(1);
    ui->leIP->setText(ip);
    ui->lePort->setText(port);
    ui->tabWidget->setCurrentWidget(ui->tabConnect);

    QSettings settings("Vice-Players", "Browser");
    settings.setValue("IP", ui->leIP->text().toAscii());
    settings.setValue("Nick", ui->leNick->text().toAscii());
    settings.setValue("Port", ui->lePort->text().toAscii());
}

void ServerBrowser::Refresh()
{
    ui->twServers->clear();
    if(selected == 1)
        http->get(MASTERLIST_SERVERS);
    else if(selected == 2)
        http->get(MASTERLIST_FEATURED);
    else if(selected == 3)
        http->get(MASTERLIST_SPONSORS);
    else if(selected == 4)
    {
        LoadFavourites();
    }

}

void ServerBrowser::LoadFavourites()
{
    QFile file(FAVOURITES_FILE);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    m_szFav = file.readAll();

    QStringList servers = m_szFav.split("\n");
    for(int i = 0; i < servers.size(); i++)
    {
        QString server = servers.at(i);
        if(server.size() < 7) continue;
        //QMessageBox::information(this, "", server);
        QStringList s = server.split(":");
        QString ip = s.at(0);
        QString port = s.at(1);
        Query(ip.toAscii(), port.toInt(), 'i');
    }
}

void ServerBrowser::SaveFavourites()
{
    QFile file(FAVOURITES_FILE);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    file.write(m_szFav.toAscii());
}

void ServerBrowser::AddFavourite()
{
    addfav dialog;
    dialog.show();
    dialog.exec();
    if(dialog.address != "")
    {
        m_szFav += dialog.address;
        m_szFav += "\n";
        SaveFavourites();
    }
}

void ServerBrowser::DeleteFavourite(QString address)
{
    address += "\n";
    m_szFav.replace(address, "");
    SaveFavourites();
}

void ServerBrowser::httpDone()
{
    if(selected != 4 || selected != 0)
    {
        QString data = http->readAll();
        //QMessageBox::information(this, "", data);
        if(data.size() < 5) return;

        QStringList servers = data.split("\n");
        for(int i = 0; i < servers.size(); i++)
        {
            QString server = servers.at(i);
            if(server.size() < 7) continue;
            //QMessageBox::information(this, "", server);
            QStringList s = server.split(":");
            QString ip = s.at(0);
            QString port = s.at(1);
            Query(ip.toAscii(), port.toInt(), 'i');
        }
    }
}

ServerBrowser::~ServerBrowser()
{
    delete ui;
}

void ServerBrowser::on_tabWidget_currentChanged(QWidget* wdg )
{
    if(wdg == ui->tabServers || wdg == ui->tabFeatured || wdg == ui->tabSponsors || wdg == ui->tabFavourites)
    {
        ui->btnAddFav->hide();
        setFixedSize(800, 600);
        ui->centralWidget->setFixedSize(800, 600);
        ui->tabWidget->setFixedSize(782, 583);
        if(wdg == ui->tabFeatured)
        {
            ui->wServers->setParent(ui->tabFeatured);
            ui->wServers->show();
            selected = 2;
        }
        else if(wdg == ui->tabSponsors)
        {
            ui->wServers->setParent(ui->tabSponsors);
            ui->wServers->show();
            selected = 3;
        }
        else if(wdg == ui->tabServers)
        {
            ui->wServers->setParent(ui->tabServers);
            ui->wServers->show();
            selected = 1;
        }
        else if(wdg == ui->tabFavourites)
        {
            ui->wServers->setParent(ui->tabFavourites);
            ui->wServers->show();
            ui->btnAddFav->show();
            selected = 4;
        }
        Refresh();
    }
    else if(wdg == ui->tabConnect)
    {
        setFixedSize(373, 190);
        ui->centralWidget->setFixedSize(373, 190);
        ui->tabWidget->setFixedSize(351, 171);
    }
}

void ServerBrowser::MasterListStartup()
{
        bWinsockInitialized = false;
        if(!bWinsockInitialized)
        {
                WSADATA wsaData;
                WSAStartup(MAKEWORD(2, 0), &wsaData);
                bWinsockInitialized = true;
        }

        if(!m_Socket)
        {
                m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

                if(!m_Socket)
                {
                        //IVMessageBox("Socket initialization failed.");
                        return;
                }

                u_long ulArg = 1;
                ioctlsocket(m_Socket, FIONBIO, &ulArg);
        }
        QTimer * timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(ListenThreadProc()));
        timer->start(10);
}


void ServerBrowser::Query(const char * szIp, unsigned short sPort, char cCommand)
{
        if(m_Socket)
        {
                SOCKADDR_IN pTarget;
                pTarget.sin_family = AF_INET; // address family Internet
                pTarget.sin_port = htons(sPort); // port to connect on
                pTarget.sin_addr.s_addr = inet_addr(szIp); // target ip

                char szOutput[4];
                szOutput[0] = 'V';
                szOutput[1] = 'C';
                szOutput[2] = 'P';
                szOutput[3] = cCommand;

                sendto(m_Socket, (char *)szOutput, sizeof(szOutput), 0, (const sockaddr *)&pTarget, sizeof(SOCKADDR_IN));
        }
}

void ServerBrowser::ListenThreadProc()
{
        char szBuf[2048];
        szBuf[0] = '\0';
        //QMessageBox::warning(0, "", "listen");
        sockaddr_in sa;
        sa.sin_family = AF_INET;
        int iLen = sizeof(sa);
        int iBytesRead = recvfrom(m_Socket, szBuf, sizeof(szBuf), 0, (sockaddr *)&sa, &iLen);
        //printf("iBytesRead: %d", iBytesRead);
        if(iBytesRead > 3 && szBuf[0] == 'V' && szBuf[1] == 'C' && szBuf[2] == 'P') // IVMP
        {
                printf("Query");
                unsigned int nLen = 4;

                int iQueryType = szBuf[nLen - 1];
                printf("Querytype: %c\n", szBuf[nLen - 1]);
                if(iQueryType == 'i')
                {
                        // Server info query

                        // Read the host name length
                        int iHostNameLen = *(int *)&szBuf[nLen];
                        nLen += sizeof(int);

                        // Allocate the host name buffer
                        char * szHostname = new char[iHostNameLen + 1];

                        // Read the host name
                        memcpy(szHostname, (szBuf + nLen), iHostNameLen);
                        szHostname[iHostNameLen] = '\0';
                        nLen += iHostNameLen;

                        // Read the mode name length
                        int iModeNameLen = *(int *)&szBuf[nLen];
                        nLen += sizeof(int);

                        // Allocate the mode name buffer
                        char * szModeName = new char[iModeNameLen + 1];

                        // Read the mode name
                        memcpy(szModeName, (szBuf + nLen), iModeNameLen);
                        szModeName[iModeNameLen] = '\0';
                        nLen += iModeNameLen;

                        // Read the map name length
                        int iMapNameLen = *(int *)&szBuf[nLen];
                        nLen += sizeof(int);

                        // Allocate the map name buffer
                        char * szMapName = new char[iMapNameLen + 1];

                        // Read the map name
                        memcpy(szMapName, (szBuf + nLen), iMapNameLen);
                        szMapName[iMapNameLen] = '\0';
                        nLen += iMapNameLen;

                        // Read the player count
                        int iPlayerCount = *(int *)&szBuf[nLen];
                        nLen += sizeof(int);

                        // Read the max player limit
                        int iMaxPlayers = *(int *)&szBuf[nLen];
                        nLen += sizeof(int);

                        // Read if the server is passworded or not
                        bool bPassworded = *(bool *)&szBuf[nLen];
                        nLen += sizeof(bool);

                        printf("Hostname: %s\n", szHostname);
                        printf("Players: %d/%d\n", iPlayerCount, iMaxPlayers);
                        char * dest = new char[256];
                        const char *ntoaStr = inet_ntoa( sa.sin_addr );
                        strcpy(dest, ntoaStr);

                        QString host = "%1:%2";
                        QString port;
                        QString ip = dest;
                        port.setNum(ntohs(sa.sin_port));
                        host.replace("%1", ip);
                        host.replace("%2", port);

                        QString players = "%1/%2";
                        QString p;
                        QString m;
                        p.setNum(iPlayerCount);
                        m.setNum(iMaxPlayers);
                        players.replace("%1", p);
                        players.replace("%2", m);

                        QTreeWidgetItem *wdgitem = new QTreeWidgetItem;
                        wdgitem->setText(0, szHostname);
                        wdgitem->setText(1, players);
                        wdgitem->setText(2, szModeName);
                        wdgitem->setText(3, szMapName);
                        wdgitem->setText(4, host);
                        ui->twServers->addTopLevelItem(wdgitem);

                        // Free the host name buffer
                        delete szHostname;
                }
        }
        return;
}

bool ServerBrowser::GetProcessIdFromProcessName(wchar_t * szProcessName, DWORD * dwProcessId)
{
        bool bReturn = false;

        // Create a tool help 32 process snapshot
        HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if(!hProcessSnapShot)
                return false;

        PROCESSENTRY32 ProcessEntry;
        ProcessEntry.dwSize = sizeof(ProcessEntry);

        if(Process32First(hProcessSnapShot, &ProcessEntry))
        {
                // Enumerate through all processes
                while(Process32Next(hProcessSnapShot, &ProcessEntry))
                {
                        // Check the process name to see if it matches szProcessName
                        if(!wcscmp(ProcessEntry.szExeFile, szProcessName))
                        {
                                // It matches, set the process id and return true
                                *dwProcessId = ProcessEntry.th32ProcessID;
                                bReturn = true;
                                break;
                        }
                }
        }

        // Close the snapshot handle
        CloseHandle(hProcessSnapShot);

        return bReturn;
}

/////////////////////////////////////////////////////////////////////////////

bool ServerBrowser::InjectLibraryIntoProcess(DWORD dwProcessId, char * szLibraryPath)
{
        bool bReturn = true;

        // Open our target process
        HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, dwProcessId);

        if(!hProcess) {
                // Failed to open the process
                MessageBoxA(NULL, "Failed to open the remote process.", NULL, NULL);
                return false;
        }

        // Get the length of the library path
        size_t sLibraryPathLen = (strlen(szLibraryPath) + 1);

        // Allocate the a block of memory in our target process for the library name
        void * pRemoteLibraryPath = VirtualAllocEx(hProcess, NULL, sLibraryPathLen, MEM_COMMIT, PAGE_READWRITE);

        // Write our library name to the allocated block of memory
        SIZE_T sBytesWritten = 0;
        WriteProcessMemory(hProcess, pRemoteLibraryPath, (void *)szLibraryPath, sLibraryPathLen, &sBytesWritten);

        if(sBytesWritten != sLibraryPathLen) {
                MessageBoxA(NULL, "Failed to write library path into remote process.", NULL, NULL);
                bReturn = false;
        } else {
                // Get the handle of Kernel32.dll
                HMODULE hKernel32 = GetModuleHandle(L"Kernel32");

                // Get the address of the LoadLibraryA function from Kernel32.dll
                FARPROC pfnLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");

                // Create a thread inside the target process to load our library
                HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibraryA, pRemoteLibraryPath, 0, NULL);

                if(hThread) {
                        // Wait for the created thread to end
                        WaitForSingleObject(hThread, INFINITE);

                        // Get the remote thread exit code
                        /*DWORD dwExitCode;
                        GetExitCodeThread(hThread, &dwExitCode);

                        if(dwExitCode != 0)
                        {
                                IVMessageBox("Failed to inject library into remote process. Cannot launch IV:MP.");
                                bReturn = false;
                        }*/

                        // Close our thread handle
                        CloseHandle(hThread);
                } else {
                        // Thread creation failed
                        MessageBoxA(NULL, "Failed to create remote thread in remote process.", NULL, NULL);
                        bReturn = false;
                }
        }

        // Free the allocated block of memory inside the target process
        VirtualFreeEx(hProcess, pRemoteLibraryPath, sizeof(pRemoteLibraryPath), MEM_RELEASE);

        // If the injection failed terminate the target process
        if(!bReturn)
                TerminateProcess(hProcess, 0);

        // Close our target process
        CloseHandle(hProcess);

        return bReturn;
}


