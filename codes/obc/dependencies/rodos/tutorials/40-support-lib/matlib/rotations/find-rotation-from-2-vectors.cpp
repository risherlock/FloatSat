
#include "rodos.h"
#include "matlib.h"

#include "random.h" // just to test

static const int NUM_OF_ROTATIONS_TO_TEST = 10000;

class TestRotation : public StaticThread<> {
    void run();
} testRotation;

void TestRotation::run() {
    int numOfWrongs = 0;
    Result<AngleAxis> result;
    Vector3D a ;
    Vector3D a_;
    Vector3D b ;
    Vector3D b_;

    AngleAxis originalRotor; // Generated randomly
    AngleAxis reconstructedRotor;


    for(int i = 0; i < NUM_OF_ROTATIONS_TO_TEST; i++) {

        // Random 2 original vectors
        a.x = drand();
        a.y = drand();
        a.z = drand();

        b.x = drand();
        b.y = drand();
        b.z = drand();

        // generate randomly a rotation for both vectors
        {
            double   d = abs(drand() * M_PI);
            Vector3D h = { drand(), drand(), drand() };

            originalRotor = AngleAxis(d, h);
        }

        a_ = a.aRotate(originalRotor);
        b_ = b.aRotate(originalRotor);

        result = findRotationsAngleAxis(a, a_, b, b_);
        if(!result.isOk()) PRINTF(" wrong computation, errorcode = %d\n", static_cast<int>(result.getErr()));
        reconstructedRotor = result.val;

        bool ok = true;
        //PRINTF("----------------------------------\n");
        //PRINTF("  original Rotor: "); originalRotor.print();
        //PRINTF("  Reconstructed : "); reconstructedRotor.print();
        if(!originalRotor.u.equals(reconstructedRotor.u))          ok = false;
        if(!isAlmost0(originalRotor.phi - reconstructedRotor.phi)) ok = false;
        if(!a_.equals(a.aRotate(reconstructedRotor)))              ok = false;
        if(!b_.equals(b.aRotate(reconstructedRotor)))              ok = false;

        if(!ok) {
            numOfWrongs++;
            PRINTF("    Error for test vector Nr %d\n", i);

            PRINTF("      orig a               : "); a.print();
            PRINTF("      a after orig rotation: "); a_.print();
            PRINTF("      a after rec  rotation; "); a.aRotate(reconstructedRotor).print();
            PRINTF("      --------------------\n");

            PRINTF("      orig b               : "); b.print();
            PRINTF("      b after orig rotation: "); b_.print();
            PRINTF("      b after rec  rotation; "); b.aRotate(reconstructedRotor).print();
        }
    }

    PRINTF("%d from %d wrong\n", numOfWrongs, NUM_OF_ROTATIONS_TO_TEST);
}
