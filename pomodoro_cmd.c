#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>

/*
Goal of the project:
    Have a 25 minute timer with 5 minute breaks thrown into it
    Disconnect from wifi during the 25 minute work
*/

// The basics of the pomodoro
void pomodoro(int total_time, int session_num, int wifi_index) {
    float work_time = 5.0 / 6 * total_time / session_num * 60 * 1000;
    float break_time = 1.0 / 6 * total_time / session_num * 60 * 1000;
    char command[200];

    printf("Total work time: %f\nTotal break time: %f\n", work_time/1000, break_time/1000);

    for (int i = 0; i < session_num; i++) {
        // disabling the wifi adapter
        sprintf(command, "wmic path win32_networkadapter where index=%d call disable", wifi_index);
        system(command);
        // work/study for a determined amount of time
        Sleep(work_time);
        // enable the wifi adapter
        sprintf(command, "wmic path win32_networkadapter where index=%d call enable", wifi_index);
        system(command);
        // break for a determined amount of time
        Sleep(break_time);
    }
}

// Getting the index for the wifi index
int get_index() {
    int index_num = -1;
    for (int i = 0; i < 99; i++) {
        char command[200];
        sprintf(command, "wmic path win32_networkadapter where index=%d call disable", i);
        system(command);
        if (system("ping www.google.com -n 1")) {
            index_num = i;
            i = 100;
        }
        sprintf(command, "wmic path win32_networkadapter where index=%d call enable", index_num == -1 ? i : index_num);
        system(command);
    }
    system("cls");
    return index_num;
}

int main() {
    int index_num, total_time, session_num;

    // brute force the index num
    index_num = get_index();

    // set up the total time and session count
    puts("Enter the total time (in minutes) and number of study session: ");
    scanf("%d %d", &total_time, &session_num);

    // run the pomodoro
    pomodoro(total_time, session_num, index_num);

    return 0;
}