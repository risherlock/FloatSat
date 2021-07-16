
#include "rodos.h"
#include "allocableobjects.h"
#include "rodos-result.h"

struct TestObj {
    int a, b, c;
};

AllocableObjects<TestObj, 20> allocableTestObjs;


class TestAllocs : public StaticThread<> {
  public:
    TestObj* ptr[50];
    void     run();
} testAllocs;

void TestAllocs::run() {
    allocableTestObjs.init();
    PRINTF("free object %d\n", allocableTestObjs.getNumOfFreeItems());

    PRINTF("allocate\n");
    for(int i = 0; i < 25; i++) {
        ptr[i] = allocableTestObjs.alloc();
        Result<uint32_t> x = allocableTestObjs.indexOf(ptr[i]);
        PRINTF("    index %d/%d, free = %d ptr = %x\n", x.isOk(), x.val, allocableTestObjs.getNumOfFreeItems(), (int)(intptr_t)ptr[i]);
    }

    PRINTF("copy references\n");
    for(int i = 25; i < 35; i++) {
        ptr[i] = allocableTestObjs.copyReference(ptr[i-10]); // 5 ok, 5 wrong
        Result<uint32_t> x = allocableTestObjs.indexOf(ptr[i]);
        PRINTF("    index %d/%d, free = %d ptr = %x\n", x.isOk(), x.val, allocableTestObjs.getNumOfFreeItems(), (int)(intptr_t)ptr[i]);
    }

    PRINTF("free\n");
    for(int i = 0; i < 40; i++) {
        int ok = allocableTestObjs.free(ptr[i]);
        Result<uint32_t> x = allocableTestObjs.indexOf(ptr[i]);
        PRINTF("    ok = %d index %d/%d, free = %d\n", ok, x.isOk(), x.val, allocableTestObjs.getNumOfFreeItems());
    }
}
