#include "rodos.h"
#include "checksumes.h"

uint32_t printfMask = 0;

uint64_t toBeTested;

class ChecksumTester : public StaticThread<> {

  public:
    void run() {
        printfMask = 1;
        toBeTested = 1;
        char buff[80]; //Buffer for printf, a lot longer than needed

        CRC crc;

        for(unsigned int i = 1; i < 100; i++) {
            toBeTested *= i;
            toBeTested += i;
            SPRINTF(buff, " demo data %lld", static_cast<long long>(toBeTested));
            PRINTF("Checksum of %u: %lld\n", i, static_cast<long long>(toBeTested));

            uint32_t computedCRC1 = RODOS::computeCrc(&toBeTested, sizeof(toBeTested), 0);
            uint32_t computedCRC2 = crc.computeCRC(&toBeTested, sizeof(toBeTested), 0);
            if(computedCRC1 == computedCRC2)
                PRINTF("  OK %u\n", static_cast<unsigned int>(computedCRC1));
            else
                PRINTF(" Err %u != %u\n", static_cast<unsigned int>(computedCRC1), static_cast<unsigned int>(computedCRC2));
            PRINTF("  checksum   %u\n", static_cast<unsigned int>(RODOS::checkSum(&toBeTested, sizeof(toBeTested))));
            PRINTF("  chcksumxor %u\n", static_cast<unsigned int>(RODOS::checkSumXor32((uint32_t*)&toBeTested, 2)));
            PRINTF("  hash; %d\n", hash(buff));
        }

        PRINTF("\nThis run (test) terminates now!\n");
        hwResetAndReboot();
    }
} checksumsTester;
