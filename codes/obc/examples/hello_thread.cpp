// The thread LowPriorityThread writes the character "." every second 
// and is interrupted every five second by the thread HighPriorityThread, 
// which writes the character "*".

#include <rodos.h>

class HighPriorityThread : public StaticThread<>
{
public:
    HighPriorityThread() : StaticThread("HiPriority", 55) {}
    void run()
    {
        while (1)
        {
            PRINTF("*\n");
            suspendCallerUntil(NOW() + 5 * SECONDS);
        }
    }
} highprio;

class LowPriorityThread : public StaticThread<>
{
public:
    LowPriorityThread() : StaticThread("LowPriority", 25) {}
    void run()
    {
        while (1)
        {
            suspendCallerUntil(NOW() + 1 * SECONDS);
            PRINTF(".\n");
        }
    }
} lowprio;