
#pragma once

#include "math.h" // sin, sqrt, etc
#include "rodos-result.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


namespace RODOS {


// Tutorials in tutorials/matlib


//_________________________________________________________________________

#define Point3D Vector3D
class Vector3D;
class Matrix3D;
class Matrix6D;
class Matrix4D;
class Quaternion;
class CoordinateFrame3D;
class Vector4D;
class Polar;
class AngleAxis;
class YPR; // DEPRECATED
class RPY;

//_________________________________________________________________________

const double EPSILON = 0.0000001;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline bool   isAlmost0(const double &a)   { return fabs(a) < EPSILON; }
inline double grad2Rad(const double &grad) {return grad * M_PI / 180.0; }
inline double rad2Grad(const double &rad)  {return rad * 180.0 / M_PI; }
inline double square( const double &x )    { return x*x; }
inline double frac( const double &x )      { return x - floor(x); }
inline double mod( const double &dividend, const double &divisor ) { return (isAlmost0(divisor) ? 0.0 : divisor * frac( dividend/divisor )); }
uint64_t faculty(const uint32_t &x);
double FMod2p( const double &x); ///< floating point rest, after division with 2Pi
double fabs(const double &value);

//_________________________________________________________________________


class Vector3D {
public:
    double x;
    double y;
    double z;

    Vector3D();                       ///< generates (0,0,0)
    Vector3D (const double &x, const double &y, const double &z);
    Vector3D(const Vector3D& other); ///< copy from vector
    Vector3D (double* arr);          ///< like Vector3D (arr[0],arr[1],arr[2])
    inline Vector3D& operator=   (const Vector3D &other) = default;

    bool resetIfNAN(); ///< sets to (0,0,0) if any component is infinite or NAN

    Vector3D vecAdd(const Vector3D& other) const;
    Vector3D vecSub(const Vector3D& other) const;
    Vector3D  scale(const double &factor) const;

    Vector3D cross(const Vector3D& right) const;
    double dot(const Vector3D& other) const;

    double getLen() const;
    double distance(const Vector3D& other) const;

    Vector3D normalize() const;
    bool isNormalized() const;
    bool equals(const Vector3D& other) const;

    double getAngle(const Vector3D& other) const;
    bool isOrthogonal(const Vector3D& other) const;

    Polar carToPolar() const; ///< cartesian to polar coordinates
    // Vector3D polarToCar() const;

    Vector3D matVecMult(const Matrix3D& M) const;   ///< multiply with a 3x3 matrix can be a rotation
    Vector3D qRotate(const Quaternion& q) const;    ///< rotates using a quaternion
    Vector3D mRotate(const Matrix3D& M) const;      ///< rotate using a rotation matrix
    Vector3D aRotate(const AngleAxis& u_phi) const; ///< rotation using angelaxis
    Vector3D rpyRotate(const RPY& rpy) const;       ///< rotation using rpy
    Vector3D yprRotate(const YPR& ypr) const;       ///< DEPRECATED rotation using ypr

    Vector4D to4D() const; ///< generates (x,y,z,1)

    void print() const;
};

Vector3D elementWiseProduct (const Vector3D &left, const Vector3D &right);
Vector3D elementWiseDivision(const Vector3D &left, const Vector3D &right);

/*******************        operators       **********************/

inline Vector3D operator+   (const Vector3D &left, const Vector3D &right) { return left.vecAdd(right); }
inline Vector3D operator-   (const Vector3D &left, const Vector3D &right) { return left.vecSub(right); }
inline Vector3D operator*   (const double   &left, const Vector3D &right) { return right.scale(left); }
inline Vector3D operator*   (const Vector3D &left, const double   &right) { return left.scale(right); }
inline Vector3D operator/   (const Vector3D &left, const double   &right) { return left.scale(1.0/right); }
inline double   dotProduct  (const Vector3D &left, const Vector3D &right) { return left.dot(right);   }
inline Vector3D crossProduct(const Vector3D &left, const Vector3D &right) { return left.cross(right); }


//_________________________________________________________________________

/** rotor: basis to rotate from one vector to another **/

class Rotor {
public:
    double cosAngle;
    Vector3D axis;
    Rotor()                                   { cosAngle = axis.x = axis.y = axis.z = 1.0; }
    Rotor(const Rotor& other)                 =default;
    Rotor(const double ca, const Vector3D& u) { cosAngle = ca; axis = u; }
    Rotor(const Vector3D& fromVector, const Vector3D& toVector); ///< rotor to go from one vector ot other
    bool resetIfNAN(); ///< sets to (1,(1,1,1)) if any component is infinite or NAN
    bool isNoRotation() { return isAlmost0(1 - cosAngle)  || isAlmost0(axis.getLen()) || (fabs(cosAngle) > 1); }
    void print();
};

//_________________________________________________________________________


/**
  RPY: Roll, Pitch, Yaw ist not a vector, but we need to reuse all functions from vector, therefore
  we use  RPY as it were a vector
**/

class RPY  : public Vector3D {
public:
    RPY() { }
    RPY(const Vector3D& a)                                       { x = a.x; y = a.y; z = a.z; }
//    RPY(const RPY& a)                                            { x = a.x; y = a.y; z = a.z; }
// base class 'class RODOS::Vector3D' should be explicitly initialized in the copy constructor [-Wextra]
    RPY(const RPY& a) : Vector3D(a.x, a.y, a.z) { }
    RPY(const double roll, const double pitch, const double yaw) { x = roll; y = pitch; z = yaw; };
    RPY(const Quaternion& q);
    RPY(const Matrix3D& M);
    RPY(const AngleAxis& other);
    RPY(const Rotor& rot);
    inline RPY& operator= (const RPY& other) = default;

    RPY accumulateRotation(RPY& increment); ///<  converts both to quaternion, mulitply and back to RPY

    Matrix3D   toMatrix3D() const;
    Quaternion toQuaternion() const;
    AngleAxis  toAngleAxis() const;
    double     getRotationAngle() const;
    Vector3D   getRoationAxis() const;

    inline double getRoll()  const   { return x; }
    inline double getPitch() const   { return y; }
    inline double getYaw()   const   { return z; }
    inline void   setRoll(double r)  { x = r;    }
    inline void   setPitch(double p) { y = p;    }
    inline void   setYaw(double yaw) { z = yaw;  }

};


//_________________________________________________________________________


//This class is supposed to be used for homogenous transformations. 
//This means that despite its name, it only consists out of 3 components x, y and z.
class Vector4D : public Vector3D {
public:
    Vector4D(); ///< creates (0,0,0,1)
    Vector4D (const double &x, const double &y, const double &z);
    Vector4D(const Vector4D& other);
    inline Vector4D& operator= (const Vector4D& other) = default;
    Vector4D (double* arr); ///< like Vector4D(arr[0],arr[1],arr[2],arr[3])
    Vector4D matVecMult(const Matrix4D& M) const; // WARNING: bottom row of the matrix MUST be 0, 0, 0, 1 as it is used for homogenous transformations.
    Vector3D to3D() const; ///< returns (x,y,z)
    Vector4D mRotate(const Matrix4D& M) const;
    void print() const;
};


//_________________________________________________________________________


class Quaternion {

public:
    double q0;
    Vector3D q;
    Quaternion(); ///< creates q = (1,(0,0,0))
    Quaternion(const Quaternion& q);
    Quaternion(const double &q0, const double &q1, const double &q2, const double &q3); ///< (q0, (q1,q2,q3))
    Quaternion(double* arr); /// < like Quaternion(arr[0],(arr[1],arr[2],arr[3]))
    Quaternion(const double &q0,const Vector3D& q);
    Quaternion(const AngleAxis& other); ///<  q = cos(phi/2) + u*sin(phi/2)*/
    Quaternion(const Rotor& rot);
    Quaternion(const Matrix3D& other);
    Quaternion(const RPY& other);
    Quaternion(const YPR& other); ///< DEPRECATED
    inline Quaternion& operator= (const Quaternion& other) = default;

    bool resetIfNAN(); ///< sets to (1,(0,0,0)) if any component is infinit or NAN

    double    getAngle() const; ///< radians
    Vector3D  getVec() const;

    Quaternion qAdd(const Quaternion& other) const;
    Quaternion qSub(const Quaternion& other) const;
    Quaternion scale(const double &factor) const;
    Quaternion qMult(const Quaternion& right) const;

    Quaternion conjugate() const; ///< returns  (q0, -(q) )
    Quaternion invert() const; ///< returns multiplicative Inverse
    Quaternion qDivide(const Quaternion& denominator) const; ///< (*this)*(other.invert())*/

    double getLen() const;
    Quaternion normalize() const;
    bool isNormalized() const;
    bool equals(const Quaternion& other) const;
    AngleAxis toAngleAxis() const;
    Matrix3D toMatrix3D() const;
    RPY toRPY() const;
    YPR toYPR() const; ///< DEPRECATED

    void print() const;
};

/*******************        operators       **********************/

inline Quaternion operator+(const Quaternion &left, const Quaternion &right)  { return left.qAdd(right); }
inline Quaternion operator-(const Quaternion &left, const Quaternion &right)  { return left.qSub(right); }
inline Quaternion operator*(const Quaternion &left, const Quaternion &right)  { return left.qMult(right); }
inline Quaternion operator*(const double     &left, const Quaternion &right)  { return right.scale(left); }
inline Quaternion operator*(const Quaternion &left, const double     &right)  { return left.scale(right); }
inline Vector3D   operator*(const Quaternion &left, const Vector3D   &right)  { return right.qRotate(left); }
inline Quaternion operator/(const Quaternion &left, const double     &right)  { return left.scale(1.0/right); }
inline Quaternion operator-(const Quaternion &right)                   { return right.conjugate(); }

inline Quaternion qX(const double &phi) { return Quaternion(cos(phi/2), sin(phi/2), 0.0, 0.0); }
inline Quaternion qY(const double &phi) { return Quaternion(cos(phi/2), 0.0, sin(phi/2), 0.0); }
inline Quaternion qZ(const double &phi) { return Quaternion(cos(phi/2), 0.0, 0.0, sin(phi/2)); }
inline Quaternion q1()           { return Quaternion(1,          0.0, 0.0, 0.0); }
Quaternion operator*(const Matrix4D &left, const Quaternion &right);


//_________________________________________________________________________


class Matrix3D {
public:
    double r[3][3];
    Matrix3D(); ///< identity matrix
    Matrix3D(const Vector3D& column1, const Vector3D& column2, const Vector3D& column3);
    Matrix3D(double* arr);             ///< from left to rigth from top to button
    Matrix3D(const Matrix3D& other);
    Matrix3D(const Vector3D& init);    ///< diagonalmatrix of vector
    Matrix3D(const RPY& other);        ///< rotation matrix from Euler
    Matrix3D(const YPR& other);        ///< DEPRECATED
    Matrix3D(const AngleAxis& other);  ///< corresponding rotation matrix
    Matrix3D(const Quaternion& other); ///< corresponding rotation matrix
    inline Matrix3D& operator= (const Matrix3D& other) = default;


    Vector3D getVec() const; ///< return the rotations axis which is coded in the matrix
    double getAngle() const; ///< returns the rotation angle which is coded in the matrix


    Vector3D getRow1() const;
    Vector3D getRow2() const;
    Vector3D getRow3() const;

    Vector3D getColumn1() const;
    Vector3D getColumn2() const;
    Vector3D getColumn3() const;

    void setRow1(const Vector3D& row);
    void setRow2(const Vector3D& row);
    void setRow3(const Vector3D& row);

    void setColumn1(const Vector3D& column);
    void setColumn2(const Vector3D& column);
    void setColumn3(const Vector3D& column);


    Matrix3D mAdd(const Matrix3D& other) const; ///< adds element for element
    Matrix3D mSub(const Matrix3D& other) const; ///< subs element for element

    Matrix3D scale(const double &factor) const;
    Matrix3D mMult(const Matrix3D& other) const;
    Matrix3D mDivide(const Matrix3D& other) const; ///< multiply with inverse


    Matrix3D cofac() const; ///< each entry aij holds the subdeterminate Aij (for inverse matrices)
    Matrix3D adjoint() const; ///<transposed cofactor matrix
    Matrix3D invert() const;
    Matrix3D transpose() const;
    double trace() const; ///< return the trace of the matrix

    void rotationX(const double &angle); ///< reinit as rotation matrix
    void rotationY(const double &angle); ///< reinit as rotation matrix
    void rotationZ(const double &angle); ///< reinit as rotation matrix

    double determinant() const;
    bool isOrthogonal() const;

    bool equals(const Matrix3D& other) const;

    Quaternion toQuaternion() const;
    RPY toRPY() const;
    YPR toYPR() const; ///< DEPRECATED
    AngleAxis toAngleAxis() const;

    void print() const;
};


/*******************        operators       **********************/

inline Matrix3D operator+(const Matrix3D &left, const Matrix3D &right) { return left.mAdd(right); }
inline Matrix3D operator-(const Matrix3D &left, const Matrix3D &right) { return left.mSub(right); }
inline Matrix3D operator*(const Matrix3D &left, const Matrix3D &right) { return left.mMult(right); }

inline Matrix3D operator*(const double   &left, const Matrix3D &right) { return right.scale(left); }
inline Matrix3D operator*(const Matrix3D &left, const double   &right) { return left.scale(right); }
inline Matrix3D operator/(const Matrix3D &left, const double   &right) { return left.scale(1.0/right); }

inline Matrix3D operator*   (const Vector3D &left, const Vector3D &right) { return Matrix3D( left*right.x, left*right.y, left*right.z ); }

inline Vector3D operator*(const Matrix3D &left, const Vector3D &right) { return right.matVecMult(left); } // in matVecMult right and left are toggled
inline bool operator==(const Matrix3D &left, const Matrix3D &right) { return left.equals(right); }
inline bool operator!=(const Matrix3D &left, const Matrix3D &right) { return !left.equals(right); }


//_________________________________________________________________________

/**  DEPRECATED!! use RPY */

class YPR {
public:
    double yaw;
    double pitch;
    double roll;
    YPR(); ///< inits (0,0,0)
    YPR(const YPR& other);
    YPR(const double &yaw, const double &pitch, const double &roll);
    YPR(const Vector3D& v) { roll = v.x; pitch = v.y; yaw = v.z; }
    YPR(const Quaternion& q);
    YPR(const Matrix3D& M);
    YPR(const AngleAxis& other);

    YPR& operator=(YPR&&) = default;
    YPR& operator=(const YPR&) = default;

    YPR  scale(const double &factor) const;

    Matrix3D toMatrix3D() const;
    Quaternion toQuaternion() const;
    AngleAxis toAngleAxis() const;

    void print() const;
};

//_________________________________________________________________________


///represents a rotation with an angle and axis
class AngleAxis {
public:
    Vector3D u; ///< rotation axis, must be an unit vector
    double phi; ///< rotation angle in radians

    AngleAxis(); ///< (0, (1,0,0);
    AngleAxis(const double &phi, const double &x, const double &y, const double &z);
    AngleAxis(const double &phi, const Vector3D& u);
    AngleAxis(const AngleAxis& other);
    AngleAxis(const Quaternion& q);
    AngleAxis(const Matrix3D& M);
    AngleAxis(const RPY& rpy);
    AngleAxis(const YPR& ypr); ///< DEPRECATED
    inline AngleAxis& operator= (const AngleAxis& other) = default;

    Quaternion toQuaternion() const;
    Matrix3D toMatrix3D() const;
    RPY toRPY() const;
    YPR toYPR() const; ///< DEPRECATED
    
    bool equals(AngleAxis& other);       //Compares 2 AngleAxis, returns true if the result of the rotation is the same, so if the axis points in the opposite direction and the angle is the other way round, the rotation is the same, adding 2Pi to the angle doesn't change the rotation

    void print() const;
};


//_________________________________________________________________________

/// 4x4-Matrix Rotation and translation
class Matrix4D {
public:
    double r[4][4];

    Matrix4D(); ///< no rotation, no translation
    Matrix4D(const Matrix3D& rot, const Vector3D& trans);
    Matrix4D(double* arr); ///< arr[16], left to right, top  to bottom
    Matrix4D(const Matrix4D& other) = default;

    Matrix3D getRotation() const;    ///< returns the corresponding rotation matrix
    Vector3D getTranslation() const; ///< returns the translation vector

    Matrix4D scale(const double &factor) const;
    Matrix4D mMult(const Matrix4D& right) const;
    Matrix4D invert() const; ///< inverted rotation and translation, does NOT invert arbitrary matrices
    
    bool equals(Matrix4D& other);       //Returns true if all elements are (nearly) equal

    void print() const;
};


//_________________________________________________________________________



/** Cartesian coordinate system in Euclidian space
      3 orthogonal unit vectors x, y, z and 1 origin, x, y and z must form a right handed system and must be orthogonal */
class CoordinateFrame3D {
public:
    Vector3D x; ///< Unit vector in x-direction
    Vector3D y; ///< Unit vector in y-direction
    Vector3D z; ///< Unit vector in z-direction
    Vector3D origin; ///< Origin of the system

    CoordinateFrame3D(); ///< all elements as unit vectors, origin at (0,0,0)
    CoordinateFrame3D(const Vector3D& x, const Vector3D& y, const Vector3D& z, const Vector3D& origin); //WARNING: Does NOT check, if x, y and z are unit vector and orthogonal
    CoordinateFrame3D(const Vector3D& x, const Vector3D& y, const Vector3D& origin); ///< z will be generated ortogonal to x, y
    CoordinateFrame3D(const CoordinateFrame3D& other);
    inline CoordinateFrame3D& operator= (const CoordinateFrame3D& other) = default;

    Matrix4D mapTo(const CoordinateFrame3D& other) const; ///< rotation and translation from this to other

    CoordinateFrame3D translate(const Vector3D& trans) const;
    CoordinateFrame3D rotate(const Matrix3D& rot) const;
    CoordinateFrame3D rotate(const Quaternion& q) const;
};


//_________________________________________________________________________


class Complex {
public:
    double Re;
    double Im;

    Complex(); ///< (0,0)
    Complex(const Complex& other);
    inline Complex& operator= (const Complex& other) = default;
    Complex(const double &Re, const double &Im);
    Complex cAdd(const Complex& other) const;
    Complex cSub(const Complex& other) const;       //WARNING: calculates other - this
    Complex cScale(const double &scale) const;
    Complex cMult(const Complex& other) const;
    Complex cPow(const int &exponent) const; ///< Complex z = (a+bi)^exponent, WARNING exponent MUST be >= 0
    Complex cExp() const;                    ///< Complex z = e^(*this) */
};

inline Complex operator+ (const Complex &left, const Complex &right) { return left.cAdd(right); }
inline Complex operator- (const Complex &left, const Complex &right) { return right.cSub(left); }
inline Complex operator* (const double  &left, const Complex &right) { return right.cScale(left); }
inline Complex operator* (const Complex &left, const double  &right) { return left.cScale(right); }
inline Complex operator* (const Complex &left, const Complex &right) { return left.cMult(right); }
inline Complex operator/ (const Complex &left, const double  &right) { return left.cScale(1.0/right); }
inline bool operator==   (const Complex &left, const Complex &right) { return (isAlmost0(left.Re - right.Re) && isAlmost0(left.Im - right.Im));}
inline bool operator!=   (const Complex &left, const Complex &right) { return !(left == right);}

//_________________________________________________________________________

class Polar {
public:
    double r;
    double phi; ///< radians
    double theta; ///< radians

    Polar(); ///< (0,0,0)
    Polar(const double &r, const double &phi, const double &theta);
    Polar(const Polar& other);
    Polar(const Vector3D& other); ///< corresponding polar to equivalent (x,y,z)
    inline Polar& operator= (const Polar& other) = default;

    Vector3D toCartesian() const;

    void print() const;
};


//_________________________________________________________________________



//=====================================================Matrix4D============================================

Matrix4D operator+(const Matrix4D &left, const Matrix4D &right);
Matrix4D operator*(const Matrix4D &left, const double   &right);
Matrix4D operator/(const Matrix4D &left, const double   &right);


//_________________________________________________________________________

class Vector6D {

  public:
    double v[6];

    Vector6D();
    Vector6D( const Vector6D& other ) = default;
    Vector6D( const double* arr );
    Vector6D( double x_, double y_, double z_,
              double u_, double v_, double w_ );
    Vector6D( const Vector3D &upper, const Vector3D &lower );

    double getLen() const;
    Vector6D scale(const double &factor) const;
    Vector6D vecAdd(const Vector6D& other) const;
    Vector6D vecSub(const Vector6D& other) const;
    Vector6D matVecMult(const Matrix6D& M) const;
    
    bool equals(const Vector6D& other) const;
};

/*******************        operators       **********************/
inline Vector6D operator+(const Vector6D& left, const Vector6D& right) { return left.vecAdd(right); }
inline Vector6D operator-(const Vector6D& left, const Vector6D& right) { return left.vecSub(right); }
inline Vector6D operator*(double value, const Vector6D& right) { return right.scale(value); }
inline Vector6D operator*(const Vector6D& left, const double &value) { return left.scale(value); }
inline Vector6D operator/(const Vector6D &left, const double &right) { return left.scale(1.0/right); }

inline bool operator==(const Vector6D& left, const Vector6D& right) { return left.equals(right);}
inline bool operator!=(const Vector6D& left, const Vector6D& right) { return !left.equals(right);}
/*******************        functions       **********************/
double dotProduct(const Vector6D& left, const Vector6D& right);

//=====================================================Matrix6D============================================

class Matrix6D {

public:
    double r[6][6];

    Matrix6D();
    Matrix6D(const Matrix6D& other);
    Matrix6D(const Vector6D& diag);
    Matrix6D(const double* arr);
    Matrix6D(const Matrix3D &upperLeft, const Matrix3D &upperRight, const Matrix3D &lowerLeft, const Matrix3D &lowerRight);
    inline Matrix6D& operator= (const Matrix6D& other) = default;

    Vector6D getColumn(const int &j) const;
    Vector6D getRow(const int &i) const;
    Vector6D diag() const;
    Matrix6D transpose() const;
    Matrix6D invert() const;
    void setColumn(const int &j, const Vector6D& column);
    void setRow(const int &i, const Vector6D& row);
    Matrix6D scale(const double &factor) const;
    Matrix6D mAdd(const Matrix6D& other) const;
    Matrix6D mSub(const Matrix6D& other) const;
    Matrix6D mMult(const Matrix6D& other) const;

    Matrix3D upperLeft() const;
    Matrix3D upperRight() const;
    Matrix3D lowerLeft() const;
    Matrix3D lowerRight() const;
    
    bool equals(const Matrix6D& other) const;
};


/*******************        functions       **********************/
Matrix6D dyadic(const Vector6D& left, const Vector6D& right);
bool ludcmp(Matrix6D &a, Vector6D &indx, double &d);
void lubksb(Matrix6D &a, Vector6D &indx, Vector6D &b);

/* findRotationsAngleAxis
 *  When we use only one vector to rotate a body, we get an ambiguous rotation
 *  which has one degree of freedom open.
 *  To get an unambiguous rotation we have to use two not co-linear vectors of the
 *  body. This functions finds the unit rotation which satisfied the rotation
 *  of these both vectors.
 *  But! The body may not be formatted. The geometric relationship of the
 *  two vectors has to be the same before and after the rotation.
 *   compare to "cosine direction matrix"
 *
 */
Result<AngleAxis>  findRotationsAngleAxis(Vector3D fromA, Vector3D toA, Vector3D fromB, Vector3D toB);

/* cos_direction_matrix_from_vectors
 *  when we use only one vector to rotate a body, we get an ambiguous rotation
 *  which has one degree of freedom open.
 *  To get an unambiguous rotation we have to use two not co-linear vectors of the
 *  body. This functions finds the unit rotation which satisfied the rotation
 *  of these both vectors.
 *  But! The body may not be formatted. The geometric relationship of the
 *  two vectors has to be the same before and after the rotation.
 */
Matrix3D direction_cos_matrix_from_vectors(Vector3D fromA, Vector3D toA, Vector3D fromB, Vector3D toB);


/*******************        operators       **********************/
inline Matrix6D operator*(const double &left,   const Matrix6D& right) { return right.scale(left); }
inline Matrix6D operator*(const Matrix6D& left, const double &right) { return left.scale(right); }
inline Matrix6D operator/(const Matrix6D &left, const double &right) { return left.scale(1.0/right); }
inline Matrix6D operator+(const Matrix6D& left, const Matrix6D& right) { return left.mAdd(right); }
inline Matrix6D operator-(const Matrix6D& left, const Matrix6D& right) { return left.mSub(right); }
inline Matrix6D operator*(const Matrix6D& left, const Matrix6D& right) { return left.mMult(right); }
inline Matrix6D operator*(const Vector6D &left, const Vector6D &right) { return dyadic(left, right); }
inline Vector6D operator*(const Matrix6D& left, const Vector6D& right) { return right.matVecMult(left); }

inline bool operator==(const Matrix6D& left, const Matrix6D& right) { return left.equals(right); }
inline bool operator!=(const Matrix6D& left, const Matrix6D& right) { return !left.equals(right); }
//Matrix6D operator/(const Matrix6D& Mat, double value);

//_________________________________________________________________________


Vector3D rotateX( const Vector3D& s, const double &angle); ///< rotates vector s angle (radians) arround x
Vector3D rotateY( const Vector3D& s, const double &angle); ///< rotates vector s angle (radians) arround y
Vector3D rotateZ( const Vector3D& s, const double &angle); ///< rotates vector s angle (radians) arround z

//_________________________________________________________________________

/**
jd means Julian Date, which describes the days from 1st January −4712 (4713 bc) 12:00 o' clock
*/
const double JD2000 = 2451545.0; //days  -- Julian Date on 01.01.2000 12'o clock
const double A_WGS84 = 6378137.0; // m
const double F_WGS84 = 1./298.257223563;

double R_n(const double degreeOfLatitude); ///< earthradius in meter at given latitude (rad)

/**
Reference Coordinate Systems

ECEF - Earth Centered Earth Fixed, x-Axis pointing to International Reference Meridian
ECI - Earth Centered Inertial, x-Axis aligned to Earth's Mean Equator and Equinox at 12:00 Terrestrial Time on 1 January 2000
Geodetic System, based on WGS84 Ellipsoid, defined by latitude, longitude, altitude
*/
double daysSinceY2k(int year,int month,int day,int hour,int minute,double second);
double utcToJD(int year, int month, int date, int hour, int minute, double second);
double jd2GAST(double jd);
double jd2GMST(double jd);
Matrix3D eciToECEFrotmat(double jd);
double eciToECEF(int64_t utcNanoseconds);  //DEPRECATED
Vector3D eciToECEF(const Vector3D ecef, int64_t  utcNanoseconds); ///<(meter, meter, meter) -> (meter, meter, meter)
Vector3D ecefToECI(const Vector3D eci, int64_t  utcNanoseconds);///<(meter, meter, meter) -> (meter, meter, meter)

Vector3D geodeticToECEF(const Polar& polar); ///< WARNING! Earth-surface (meter, rad, rad) -> Eart-center (meter, meter, meter)

/**
Converts cartesian coordinates to ellipsoidal.
   Uses iterative alogithm with Heiskanen and Moritz's Method (1967)
   This method has been proved fastest when convergence of both
   latitude and ellipsoidal height is required (see H. S. Fok and H.
   Baki Iz "A Comparative Analysis of the Performance of Iterative
   and Non-iterative Solutions to the Cartesian to Geodetic
   Coordinate Transformation")
   Computation time is only slightly worse than for the
   Bowring direct formula but accuracy is better.
*/
Polar    ecefToGeodetic(const Vector3D& other);     ///< WARNING! Earth-center (meter, meter, meter) -> Earth-surface (meter, rad, rad)


Matrix3D skewSymmetricMatrix(const Vector3D &v);


}

