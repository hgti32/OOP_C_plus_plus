#include "module1.h"
#include "resource.h"
#include <string>

using namespace std;

static INT_PTR CALLBACK Work1DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            SetScrollRange(GetDlgItem(hDlg, IDC_SCROLLBAR1), SB_CTL, 1, 100, TRUE);
            SetScrollPos(GetDlgItem(hDlg, IDC_SCROLLBAR1), SB_CTL, 50, TRUE);
            SetDlgItemInt(hDlg, IDC_SCROLL_VAL, 50, FALSE);
            return (INT_PTR)TRUE;

        case WM_HSCROLL: {
            HWND hScroll = (HWND)lParam;
            int pos = GetScrollPos(hScroll, SB_CTL);
            int request = LOWORD(wParam);
            
            switch (request) {
                case SB_LINELEFT: pos -= 1; break;
                case SB_LINERIGHT: pos += 1; break;
                case SB_PAGELEFT: pos -= 10; break;
                case SB_PAGERIGHT: pos += 10; break;
                case SB_THUMBTRACK:
                case SB_THUMBPOSITION: pos = HIWORD(wParam); break;
            }   
            
            if (pos < 1) pos = 1;
            if (pos > 100) pos = 100;
            
            SetScrollPos(hScroll, SB_CTL, pos, TRUE);
            SetDlgItemInt(hDlg, IDC_SCROLL_VAL, pos, FALSE);
            return (INT_PTR)TRUE;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                int finalPos = GetScrollPos(GetDlgItem(hDlg, IDC_SCROLLBAR1), SB_CTL);
                wstring text = L"Вибране число: " + to_wstring(finalPos);
                SetWindowTextW(GetParent(hDlg), text.c_str());
                EndDialog(hDlg, IDOK);
                return (INT_PTR)TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, IDCANCEL);
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

void RunWork1(HWND hParent) {
    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_WORK1), hParent, Work1DlgProc);
}