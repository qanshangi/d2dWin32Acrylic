#include "Tray.h"

NOTIFYICONDATA nid;		//��������
HMENU hMenu;			//���̲˵�

//ʵ��������
void InitTray(HINSTANCE hInstance, HWND hWnd)
{
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = IDI_TRAY;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
    nid.uCallbackMessage = WM_TRAY;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRAY));
    lstrcpy(nid.szTip, APP_NAME);

    hMenu = CreatePopupMenu();//�������̲˵�
    //Ϊ���̲˵��������ѡ��
    AppendMenu(hMenu, MF_STRING, ID_SHOW, TEXT("��ʾ"));
    AppendMenu(hMenu, MF_STRING, ID_EXIT, TEXT("�˳�"));

    Shell_NotifyIcon(NIM_ADD, &nid);
}

//��ʾ������������
void ShowTrayMsg()
{
    lstrcpy(nid.szInfoTitle, APP_NAME);
    lstrcpy(nid.szInfo, TEXT("�����ص�����"));
    nid.uTimeout = 1000;
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}


