#include "module2_2.h"
#include "resource.h"

static INT_PTR CALLBACK Work2_2DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_COMMAND) {
        if (LOWORD(wParam) == IDC_BTN_BACK) {
            EndDialog(hDlg, 1);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, 0);
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

int RunWork2_2(HWND hParent) {
    return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_WORK2_2), hParent, Work2_2DlgProc);
}