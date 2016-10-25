#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib,"imm32")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");
HWND hEdit1, hEdit2;
WNDPROC EditWndProc;
#define time 100

struct map {
	TCHAR c;
	LPCTSTR pMorseCode;
};

struct map pattern[] = {
	{ TEXT('ｱ'),TEXT("--.--") },
	{ TEXT('ｧ'),TEXT("--.--") },
	{ TEXT('ｲ'),TEXT(".-") },
	{ TEXT('ｨ'),TEXT(".-") },
	{ TEXT('ｳ'),TEXT("..-") },
	{ TEXT('ｩ'),TEXT("..-") },
	{ TEXT('ｴ'),TEXT("-.---") },
	{ TEXT('ｪ'),TEXT("-.---") },
	{ TEXT('ｵ'),TEXT(".-...") },
	{ TEXT('ｫ'),TEXT(".-...") },
	{ TEXT('ｶ'),TEXT(".-..") },
	{ TEXT('ｷ'),TEXT("-.-..") },
	{ TEXT('ｸ'),TEXT("...-") },
	{ TEXT('ｹ'),TEXT("-.--") },
	{ TEXT('ｺ'),TEXT("----") },
	{ TEXT('ｻ'),TEXT("-.-.-") },
	{ TEXT('ｼ'),TEXT("--.-.") },
	{ TEXT('ｽ'),TEXT("---.-") },
	{ TEXT('ｾ'),TEXT(".---.") },
	{ TEXT('ｿ'),TEXT("---.") },
	{ TEXT('ﾀ'),TEXT("-.") },
	{ TEXT('ﾁ'),TEXT("..-.") },
	{ TEXT('ﾂ'),TEXT(".--.") },
	{ TEXT('ｯ'),TEXT(".--.") },
	{ TEXT('ﾃ'),TEXT(".-.--") },
	{ TEXT('ﾄ'),TEXT("..-..") },
	{ TEXT('ﾅ'),TEXT(".-.") },
	{ TEXT('ﾆ'),TEXT("-.-.") },
	{ TEXT('ﾇ'),TEXT("....") },
	{ TEXT('ﾈ'),TEXT("--.-") },
	{ TEXT('ﾉ'),TEXT("..--") },
	{ TEXT('ﾊ'),TEXT("-...") },
	{ TEXT('ﾋ'),TEXT("--..-") },
	{ TEXT('ﾌ'),TEXT("--..") },
	{ TEXT('ﾍ'),TEXT(".") },
	{ TEXT('ﾎ'),TEXT("-..") },
	{ TEXT('ﾏ'),TEXT("-..-") },
	{ TEXT('ﾐ'),TEXT("..-.-") },
	{ TEXT('ﾑ'),TEXT("-") },
	{ TEXT('ﾒ'),TEXT("-...-") },
	{ TEXT('ﾓ'),TEXT("-..-.") },
	{ TEXT('ﾔ'),TEXT(".--") },
	{ TEXT('ｬ'),TEXT(".--") },
	{ TEXT('ﾕ'),TEXT("-..--") },
	{ TEXT('ｭ'),TEXT("-..--") },
	{ TEXT('ﾖ'),TEXT("--") },
	{ TEXT('ｮ'),TEXT("--") },
	{ TEXT('ﾗ'),TEXT("...") },
	{ TEXT('ﾘ'),TEXT("--.") },
	{ TEXT('ﾙ'),TEXT("-.--.") },
	{ TEXT('ﾚ'),TEXT("---") },
	{ TEXT('ﾛ'),TEXT(".-.-") },
	{ TEXT('ﾜ'),TEXT("-.-") },
	{ TEXT('ｦ'),TEXT(".---") },
	{ TEXT('ﾝ'),TEXT(".-.-.") },
	{ TEXT('ｰ'),TEXT(".--.-") },
	{ TEXT('ﾞ'),TEXT("..") },
	{ TEXT('ﾟ'),TEXT("..--.") },
	{ TEXT('､'),TEXT(".-.-.-") },
	{ TEXT(' '),TEXT(" ") },
};

void play(LPCTSTR pMorseCode)
{
	int size = lstrlen(pMorseCode);
	for (int i = 0; i<size; i++)
	{
		switch (pMorseCode[i])
		{
		case TEXT('.'):
			Beep(920, time);
			break;
		case TEXT('-'):
			Beep(920, 3 * time);
			break;
		case TEXT(' '):
			Sleep(time);//語と語の間隔（実際は7あく）
			break;
		}
		Sleep(time);//短点や長点の間隔（実際は1あく）
	}
	Sleep(2 * time);//字と字の間隔（実際は3あく）
}

void morse(const TCHAR c)
{
	for (int i = 0; i<sizeof(pattern) / sizeof(map); i++)
	{
		if (pattern[i].c == c)
		{
			play(pattern[i].pMorseCode);
		}
	}
}

LRESULT CALLBACK EditProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HIMC hIMC;
	switch (msg)
	{
	case WM_IME_COMPOSITION:
		if (lParam & GCS_RESULTREADSTR)
		{
			hIMC = ImmGetContext(hWnd);
			int intLength = ImmGetCompositionString(
				hIMC,
				GCS_RESULTREADSTR,
				0, 0);//バイト単位で返ってくるが気にしない
			if (intLength>0)
			{
				LPTSTR lpszText = (LPTSTR)GlobalAlloc(
					GMEM_ZEROINIT, sizeof(TCHAR)*(intLength + 1));
				ImmGetCompositionString(hIMC,
					GCS_RESULTREADSTR, lpszText, intLength);
				SendMessage(hEdit2, EM_REPLACESEL, 0, (LPARAM)lpszText);
				GlobalFree(lpszText);
			}
			ImmReleaseContext(hWnd, hIMC);
		}
		break;
	case WM_CONTEXTMENU:
	case WM_PASTE:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		return 0;
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
		case VK_UP:
		case VK_HOME:
		case VK_END:
		case VK_DELETE:
		case VK_BACK:
			SetWindowText(hEdit1, 0);
			SetWindowText(hEdit2, 0);
			return 0;
		}
		break;
	case WM_CHAR:
		hIMC = ImmGetContext(hWnd);
		if (ImmGetOpenStatus(hIMC) == 0)
		{
			if (wParam >= 32) {
				TCHAR szText[2];
				szText[0] = (TCHAR)wParam;
				szText[1] = 0;
				SetWindowText(hEdit2, szText);
			}
		}
		ImmReleaseContext(hWnd, hIMC);
		break;
	default:
		break;
	}
	return CallWindowProc(EditWndProc, hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		hEdit1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			TEXT("EDIT"),
			0,
			WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL,
			10, 10, 512, 32,
			hWnd,
			0,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		hEdit2 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			TEXT("EDIT"),
			0,
			WS_CHILD | ES_AUTOHSCROLL | ES_READONLY,
			10, 90, 512, 32,
			hWnd,
			0,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		CreateWindow(
			TEXT("BUTTON"),
			TEXT("再生"),
			WS_VISIBLE | WS_CHILD,
			10, 50, 64, 32,
			hWnd,
			(HMENU)100,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		EditWndProc = (WNDPROC)SetWindowLongPtr(hEdit1,
			GWLP_WNDPROC, (LONG_PTR)EditProc1);
		break;
	case WM_SETFOCUS:
		SetFocus(hEdit1);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 100)
		{
			DWORD dwSize = GetWindowTextLength(hEdit2);
			if (dwSize)
			{
				LPTSTR lpszKana = (LPTSTR)GlobalAlloc(
					0, sizeof(TCHAR)*(dwSize + 1));
				GetWindowText(hEdit2, lpszKana, dwSize + 1);
				for (DWORD i = 0; i<dwSize; i++)
				{
					morse(lpszKana[i]);
				}
				GlobalFree(lpszKana);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("和文モールス信号を再生する"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
