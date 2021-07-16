#include "rodos.h"

#include "commbuffer.h"

uint32_t printfMask = 0;
class CommBufferTester : public StaticThread<> {

  public:
    void run() {
        printfMask = 1;
       CommBuffer<int32_t> buff;
       int32_t tmp;
       
       buff.put(5);
       buff.get(tmp);
       if (tmp != 5) PRINTF("Something is very wrong with the Commbuffer\n");
       PRINTF("It should print '5': %d \n", static_cast<int>(tmp));        //MUST print 5
       
       if(buff.getOnlyIfNewData(tmp)) {
           PRINTF("MAY NOT happen\n");
        } else {
            PRINTF("No new data\n");     //Must end here
        }
        PRINTF("\nThis run (test) terminates now!\n");
        hwResetAndReboot();

    }
} commbufferTester;

