#include <windows.h>
#include <shellapi.h>

#define IDI_TRAY	101
#define ID_SHOW		40001
#define ID_EXIT		40002

#define WM_TRAY (WM_USER + 100)
#define WM_TASKBAR_CREATED RegisterWindowMessage(TEXT("TaskbarCreated"))

#define APP_NAME	TEXT("Win32Acrylic")
#define APP_TIP		TEXT("Win32")


//ʵ��������
void InitTray(HINSTANCE hInstance, HWND hWnd);

//��ʾ������������
void ShowTrayMsg();
