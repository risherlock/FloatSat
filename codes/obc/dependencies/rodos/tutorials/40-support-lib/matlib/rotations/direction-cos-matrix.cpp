#include "rodos.h"
#include "matlib.h"


/* cos_direction_matrix_from_vectors
 *  when we use only one vector to rate a body, we get an ambiguous rotation
 *  which has one degree of freedom open.
 *  To get an unambiguous rotation we have to use two not co-linear vectors of the
 *  body. This functions finds the unit rotation which satisfied the rotation
 *  of these both vectors.
 *  But! The body may not be formatted. The geometric relationship of the
 *  two vectors has to be the same before and after the rotation.
 */

//____________________________________________________ Test


#include "random.h" //only to test

static const int NUM_OF_ROTATIONS_TO_TEST = 1000;

class TestRotation : public StaticThread<> {
    void run();
} testRotation;

void TestRotation::run() {
    int numOfWrongs = 0;

    Vector3D a ;
    Vector3D a_;
    Vector3D b ;
    Vector3D b_;

    AngleAxis originalRotor; // Generated randomly
    Matrix3D  originalMatrix;
    Matrix3D  reconstrcutedRotor;


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

            originalRotor  = AngleAxis(d, h);
            originalMatrix = Matrix3D(originalRotor);
        }

        a_ = a.aRotate(originalRotor);
        b_ = b.aRotate(originalRotor);

        reconstrcutedRotor = direction_cos_matrix_from_vectors(a, a_, b, b_);

        bool ok = true;
        //PRINTF("----------------------------------\n");
        //PRINTF("  original Rotor: "); original Matrix.print();
        //PRINTF("  Reconstructed : "); reconstructed Rotor.print();
        if(!a_.equals(a.mRotate(reconstrcutedRotor))) ok = false;
        if(!b_.equals(b.mRotate(reconstrcutedRotor))) ok = false;

        if(!ok) {
            numOfWrongs++;
            PRINTF("    Error for test vector Nr %d\n", i);

            PRINTF("      orig a               : "); a.print();
            PRINTF("      a after orig rotation: "); a_.print();
            PRINTF("      a after rec. Rotation; "); a.aRotate(reconstrcutedRotor).print();
            PRINTF("      --------------------\n");

            PRINTF("      orig b               : "); b.print();
            PRINTF("      b after orig rotation: "); b_.print();
            PRINTF("      b after rec. Rotation; "); b.aRotate(reconstrcutedRotor).print();
            PRINTF("      --------------------- \n");
            PRINTF("      Original Matrix      :"); originalMatrix.print();
            PRINTF("      Reconstructed Matrix :"); reconstrcutedRotor.print();
            PRINTF("      Difference           :"); (reconstrcutedRotor - originalMatrix).print();
            if(!reconstrcutedRotor.isOrthogonal()) {PRINTF("Not orthogonal \n");}
        }
    }

    PRINTF("%d from %d wrong\n", numOfWrongs, NUM_OF_ROTATIONS_TO_TEST);
}
