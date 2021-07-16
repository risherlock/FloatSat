#include "rodos.h"
#include "random.h"

uint32_t printfMask = 0;

#define MAX_STEPS 100u      //Maximum steps of a worm

static Application module("semaphoretest");

#define MAX_X 30
#define MAX_Y 30
#define RIGHT_LIMIT 20

Semaphore lowerWindow;
Semaphore printProtector;

bool occupied[MAX_X][MAX_Y] = { { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0 } };

void MAIN() {
    PRINTF(SCREEN_CLEAR);
    for(int i = 0; i < MAX_X; i++) {
        PRINTF(SCREEN_MOVE_CURSOR_TO, i, RIGHT_LIMIT + 1);
        PRINTF(":");
    }
}

static char wormLetter = 'A';

static int prio = 1;        //Priority of the thread, priority is already used in RODOS

class worm : public StaticThread<> {
    int32_t  x, y, speed_x, speed_y;
    char letter;

  public:
    worm() : StaticThread<>("Worm", prio) {
        x       = static_cast<int32_t>(uint32Rand() % RIGHT_LIMIT);
        y       = static_cast<int32_t>(uint32Rand() % MAX_Y) + 1;
        speed_x = static_cast<int32_t>(uint32Rand() % 3) - 1;
        speed_y = static_cast<int32_t>(uint32Rand() % 3) - 1;
        letter  = wormLetter++;
        
        prio += 100;
    }

    void run() {
        printfMask = 1;

        int last_x, last_y;
        for (uint32_t i = 0; i < MAX_STEPS; i++) {
            occupied[x][y] = false;
            last_x         = x;
            last_y         = y;
            printProtector.enter();
            PRINTF("%c %d %d \n", letter, static_cast<int>(y), static_cast<int>(x));
            
            printProtector.leave();
            bool inUpperSide = x <= RIGHT_LIMIT;
            bool inLowerSide = x > RIGHT_LIMIT;


            // Change speed randomly
            if(uint32Rand() % 10 < 2) {
                speed_x = static_cast<int32_t>(uint32Rand() % 3) - 1;
                speed_y = static_cast<int32_t>(uint32Rand() % 3) - 1;
                if(speed_x == 0 && speed_y == 0) speed_y = 1;
            }


            if(x >= MAX_X-1) speed_x = -1;
            if(y >= MAX_Y-1) speed_y = -1;
            if(x <= 1) speed_x = 1;
            if(y <= 1) speed_y = 1;
            x += speed_x;
            y += speed_y;

            if(occupied[x][y]) {
                x = last_x;
                y = last_y;
            }
            occupied[x][y] = true;

           

            if(inUpperSide && x > RIGHT_LIMIT) lowerWindow.enter();
            if(inLowerSide && x <= RIGHT_LIMIT) lowerWindow.leave();

            suspendCallerUntil(NOW() + 10 * MILLISECONDS);
        }
        hwResetAndReboot();
    }
};


worm wrms[3];
