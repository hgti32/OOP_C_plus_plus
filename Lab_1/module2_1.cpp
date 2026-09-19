#include "module2_1.h"
#include "resource.h"

static INT_PTR CALLBACK Work2_1DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_COMMAND) {
        if (LOWORD(wParam) == IDC_BTN_NEXT) {
            EndDialog(hDlg, 2);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, 0);
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

int RunWork2_1(HWND hParent) {
    return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_WORK2_1), hParent, Work2_1DlgProc);
}