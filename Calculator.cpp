#include <windows.h>
#include <string>
#include <sstream>

#define ID_DISPLAY 101
#define ID_BTN_BASE 200

HWND hDisplay;
std::string input = "";

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Function to evaluate simple expressions: num1 op num2
double Evaluate(const std::string& expr) {
    double num1 = 0, num2 = 0;
    char op = 0;
    std::stringstream ss(expr);
    ss >> num1 >> op >> num2;

    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return (num2 != 0) ? (num1 / num2) : 0;
        default: return num1;
    }
}

// Create a button
void AddButton(HWND hwnd, const char* text, int id, int x, int y, int w, int h) {
    CreateWindow("BUTTON", text, WS_VISIBLE | WS_CHILD,
                 x, y, w, h, hwnd, (HMENU)id, NULL, NULL);
}

// Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "CalcApp";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, "Calculator - C++ WinAPI",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
                             NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hDisplay = CreateWindow("EDIT", "",
                                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_AUTOHSCROLL,
                                10, 10, 260, 30,
                                hwnd, (HMENU)ID_DISPLAY, NULL, NULL);

        const char* labels[4][4] = {
            {"7", "8", "9", "/"},
            {"4", "5", "6", "*"},
            {"1", "2", "3", "-"},
            {"0", "C", "=", "+"}
        };

        int id = ID_BTN_BASE;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                AddButton(hwnd, labels[i][j], id++, 10 + j * 65, 60 + i * 60, 60, 50);
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id >= ID_BTN_BASE && id < ID_BTN_BASE + 16) {
            const char* labels[16] = {
                "7", "8", "9", "/",
                "4", "5", "6", "*",
                "1", "2", "3", "-",
                "0", "C", "=", "+"
            };
            std::string value = labels[id - ID_BTN_BASE];

            if (value == "C") {
                input.clear();
            } else if (value == "=") {
                try {
                    double result = Evaluate(input);
                    input = std::to_string(result);
                    // Remove trailing .0
                    if (input.find('.') != std::string::npos) {
                        input.erase(input.find_last_not_of('0') + 1, std::string::npos);
                        if (input.back() == '.') input.pop_back();
                    }
                } catch (...) {
                    input = "Error";
                }
            } else {
                if (value == "+" || value == "-" || value == "*" || value == "/")
                    input += " " + value + " ";
                else
                    input += value;
            }

            SetWindowText(hDisplay, input.c_str());
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
