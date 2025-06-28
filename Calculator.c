#include <windows.h>
#include <stdio.h>

#define ID_DISPLAY 100
#define ID_BUTTON_BASE 200

char input[256] = "";
HWND hDisplay;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Helper to create buttons
void AddButton(HWND hwnd, const char* text, int id, int x, int y, int w, int h) {
    CreateWindow("BUTTON", text, WS_VISIBLE | WS_CHILD,
                 x, y, w, h, hwnd, (HMENU)(id), NULL, NULL);
}

// Evaluate expression (basic only: num1 operator num2)
double EvaluateExpression(const char* expr) {
    double num1 = 0, num2 = 0;
    char op;
    sscanf(expr, "%lf %c %lf", &num1, &op, &num2);
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : 0;
        default: return 0;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wc = {0};
    wc.lpszClassName = "CalcWindow";
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("CalcWindow", "Simple Calculator",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             100, 100, 300, 400,
                             NULL, NULL, hInstance, NULL);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hDisplay = CreateWindow("EDIT", "",
                                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT,
                                10, 10, 260, 30,
                                hwnd, (HMENU)ID_DISPLAY, NULL, NULL);

        const char* btns[4][4] = {
            {"7", "8", "9", "/"},
            {"4", "5", "6", "*"},
            {"1", "2", "3", "-"},
            {"0", "C", "=", "+"}
        };

        int id = ID_BUTTON_BASE;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                AddButton(hwnd, btns[i][j], id++, 10 + j*65, 50 + i*60, 60, 50);
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id >= ID_BUTTON_BASE && id < ID_BUTTON_BASE + 16) {
            const char* labels[16] = {
                "7", "8", "9", "/",
                "4", "5", "6", "*",
                "1", "2", "3", "-",
                "0", "C", "=", "+"
            };
            const char* label = labels[id - ID_BUTTON_BASE];

            if (strcmp(label, "C") == 0) {
                input[0] = '\0';
            } else if (strcmp(label, "=") == 0) {
                double result = EvaluateExpression(input);
                sprintf(input, "%.8g", result);
            } else {
                strcat(input, label);
                if (strchr("+-*/", label[0]))
                    strcat(input, " ");
            }

            SetWindowText(hDisplay, input);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
