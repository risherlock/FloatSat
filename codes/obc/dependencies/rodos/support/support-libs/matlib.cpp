

#include "rodos.h"
#include "math.h" // for sin, sqrt, etc
#include "rodos-result.h"
#include "matlib.h"

namespace RODOS {

//==========Implementierung Vector3D==========================================

Vector3D::Vector3D() {
    x = 0;
    y = 0;
    z = 0;
}

Vector3D::Vector3D (const double &x, const double &y, const double &z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3D::Vector3D(const Vector3D& other) {
    x = other.x;
    y = other.y;
    z = other.z;
}

Vector3D::Vector3D (double* arr) {
    x = arr[0];
    y = arr[1];
    z = arr[2];
}

bool Vector3D::resetIfNAN() {
    bool error = !isfinite(x) || !isfinite(y) || !isfinite(z);
    if(error) x = y = z = 0.0;
    return error;
}

//_________________________________________________________________

Vector3D Vector3D::vecAdd(const Vector3D& other) const {
    Vector3D sum;
    sum.x= x + other.x;
    sum.y= y + other.y;
    sum.z= z + other.z;
    return sum;
}
//_________________________________________________________________

Vector3D Vector3D::vecSub(const Vector3D& other) const {
    Vector3D diff;
    diff.x= x - other.x;
    diff.y= y - other.y;
    diff.z= z - other.z;
    return diff;
}
//_________________________________________________________________

Vector3D  Vector3D::scale(const double &factor) const {
    Vector3D scale;
    scale.x = x * factor;
    scale.y = y * factor;
    scale.z = z * factor;
    return scale;
}
//_________________________________________________________________

Vector3D Vector3D::cross(const Vector3D& other) const {
    Vector3D cross;
    cross.x = this->y*other.z - this->z*other.y;
    cross.y = this->z*other.x - this->x*other.z;
    cross.z = this->x*other.y - this->y*other.x;
    return cross;
}
//_________________________________________________________________

double Vector3D::dot(const Vector3D& other) const {
    double product;
    product = x*other.x + y*other.y + z*other.z;
    return product;
}
//_________________________________________________________________

double Vector3D::getLen() const {
    double len;
    len = sqrt(x*x + y*y + z*z);
    return len;
}
//_________________________________________________________________

double Vector3D::distance(const Vector3D& other) const {
    Vector3D diff = *this - other;
    return diff.getLen();
}

//_________________________________________________________________

Vector3D Vector3D::normalize() const {
    Vector3D norm;
    double len = this->getLen();
    if(isAlmost0(len)) return Vector3D(0,0,0); // avoid division by 0
    norm.x = x/len;
    norm.y = y/len;
    norm.z = z/len;
    return norm;
}
//_________________________________________________________________

bool Vector3D::isNormalized() const {
    double len = this->getLen();
    return isAlmost0(len-1);
}
//_________________________________________________________________

bool Vector3D::equals(const Vector3D& other) const {
    return isAlmost0(x - other.x) && isAlmost0(y - other.y) &&  isAlmost0(z - other.z);
}
//_________________________________________________________________


// if len is 0 you will get NaN
double Vector3D::getAngle( const Vector3D& other) const {
    double angle,product,len;
    len = this->getLen() * other.getLen() ;
    product = this->dot(other);
    angle = acos(product/len);
    return angle;   // radians
}
//_________________________________________________________________

bool Vector3D::isOrthogonal(const Vector3D& other) const {
    double cosAngle = this->dot(other);
    return isAlmost0(cosAngle);
}
//_________________________________________________________________

// if len is 0 you wil get NaN
Polar Vector3D::carToPolar() const {  // polar(r,phi,theta)
    double r     = this->getLen();
    double phi   = atan2(y, x);
    double theta = acos(z/r);
    Polar polar(r,phi,theta);
    return polar;
}
//_________________________________________________________________

#if 0
Vector3D Vector3D::polarToCar() const {
    double x = this->x * sin(this->z)*cos(this->y);
    double y = this->x * sin(this->z)*sin(this->y);
    double z = this->x * cos(this->z);
    Vector3D car(x, y, z);
    return car;
}
#endif

//_________________________________________________________________

Vector3D Vector3D::matVecMult(const Matrix3D& r) const { // M*v
    Vector3D temp = r.getRow1();
    double x = temp.dot(*this);
    temp = r.getRow2();
    double y = temp.dot(*this);
    temp = r.getRow3();
    double z = temp.dot(*this);
    Vector3D result(x, y, z);
    return result;
}
//_________________________________________________________________

Vector3D Vector3D::qRotate(const Quaternion& rotQuat) const { // w = qvq^ Rotates the vector
    Quaternion v(0,(*this)); // no normalize()
    Quaternion w  = rotQuat * v * -rotQuat;

    return w.q;
}
//_________________________________________________________________

Vector3D Vector3D::mRotate(const Matrix3D& r) const { // w = M*v
    Vector3D w(this->matVecMult(r));
    return w;
}

Vector3D Vector3D::yprRotate(const YPR& ypr) const { //  DEPRECATED
    Matrix3D rotor = ypr.toMatrix3D();
    Vector3D w(this->matVecMult(rotor));
    return w;
}

Vector3D Vector3D::rpyRotate(const RPY& rpy) const { // w = M*v
    Matrix3D rotor = rpy.toMatrix3D();
    Vector3D w(this->matVecMult(rotor));
    return w;
}

//_________________________________________________________________

Vector3D Vector3D::aRotate(const AngleAxis& u_phi) const { // Rodriguez-Formula
    Vector3D temp = this->scale(cos(u_phi.phi)).vecAdd(u_phi.u.cross(this->scale(sin(u_phi.phi))));
    Vector3D w = temp.vecAdd(u_phi.u.scale(u_phi.u.dot(*this) * (1-cos(u_phi.phi))));
    return w;
}
//_________________________________________________________________

Vector4D Vector3D::to4D() const {
    Vector4D v;
    v.x=this->x;
    v.y=this->y;
    v.z=this->z;
    return v;
}
//_________________________________________________________________

void Vector3D::print() const {
    PRINTF("(%3.3f \t %3.3f \t %3.3f)\n",this->x,this->y,this->z);
}

Matrix3D skewSymmetricMatrix( const Vector3D &v ) {
    double r[9];

    r[0] =  0.0;
    r[1] = -v.z;
    r[2] =  v.y;
    r[3] =  v.z;
    r[4] =  0.0;
    r[5] = -v.x;
    r[6] = -v.y;
    r[7] =  v.x;
    r[8] =  0.0;

    return Matrix3D(r);
}

Vector3D elementWiseProduct(const Vector3D &left, const Vector3D &right) {

    Vector3D res;

    res.x = left.x * right.x;
    res.y = left.y * right.y;
    res.z = left.z * right.z;

    return res;
}


// if you have 0's you will get NaN
Vector3D elementWiseDivision(const Vector3D &left, const Vector3D &right) {

    Vector3D res;

    res.x = left.x / right.x;
    res.y = left.y / right.y;
    res.z = left.z / right.z;

    return res;
}


void Rotor::print(){
    PRINTF("[%f (%f, %f, %f)]\n", cosAngle, axis.x, axis.y, axis.z);
}


Rotor::Rotor(const Vector3D& fromVector, const Vector3D& toVector) {
    Vector3D from = fromVector.normalize();
    Vector3D to   = toVector.normalize();
    cosAngle = dotProduct  (from, to);
    axis     = crossProduct(from, to);
    if(isAlmost0(axis.getLen())) { cosAngle = axis.x = axis.y = axis.z = 1.0; }
    axis = axis.normalize();
}

bool Rotor::resetIfNAN() {
    bool error = !isfinite(cosAngle) || !isfinite(axis.x) || !isfinite(axis.y) || !isfinite(axis.z);
    if(error) cosAngle = axis.x = axis.y = axis.z = 1.0;
    return error;
}
//================  R(oll) P(itch) Y(aw) as subclass of Vector3D  ============

RPY::RPY(const Quaternion& q) {
    double m21 = 2*q.q.x*q.q.y + 2*q.q0*q.q.z;
    double m11 = 2*q.q0*q.q0-1 + 2*q.q.x*q.q.x;
    double m31 = 2*q.q.x*q.q.z - 2*q.q0*q.q.y;
    double m32 = 2*q.q.y*q.q.z + 2*q.q0*q.q.x;
    double m33 = 2*q.q0*q.q0-1 + 2*q.q.z*q.q.z;

    this->x = atan2(m32, m33);                      // roll
    this->y = atan2(-m31, sqrt(m11*m11 + m21*m21)); // pitch
    this->z = atan2(m21, m11);                      // yaw
}

//_________________________________________________________________

RPY::RPY(const Matrix3D& M) {
    double m21 = M.r[1][0];
    double m11 = M.r[0][0];
    double m31 = M.r[2][0];
    double m32 = M.r[2][1];
    double m33 = M.r[2][2];

    this->x = atan2(m32, m33);                      // roll
    this->y = atan2(-m31, sqrt(m11*m11 + m21*m21)); // pitch
    this->z = atan2(m21, m11);                      // yaw
}

//_________________________________________________________________

RPY::RPY(const AngleAxis& other) {

    Vector3D u = other.u;
    double phi = other.phi;
    double cp  = cos(phi);
    double sp  = sin(phi);


    double m21 = u.x * u.y *(1-cp) + u.z*sp;
    double m11 = u.x * u.x *(1-cp) + cp;
    double m31 = u.x * u.z *(1-cp) - u.y*sp;
    double m32 = u.y * u.z *(1-cp) + u.x*sp;
    double m33 = u.z * u.z *(1-cp) + cp;

    this->x = atan2(m32, m33);                      // roll
    this->y = atan2(-m31, sqrt(m11*m11 + m21*m21)); // pitch
    this->z = atan2(m21, m11);                      // yaw

}

//_________________________________________________________________

RPY::RPY(const Rotor& rot) {

    Vector3D u = rot.axis;
    double cp  = rot.cosAngle;
    double sp  = sqrt(1 - cp*cp);

    double m21 = u.x * u.y *(1-cp) + u.z*sp;
    double m11 = u.x * u.x *(1-cp) + cp;
    double m31 = u.x * u.z *(1-cp) - u.y*sp;
    double m32 = u.y * u.z *(1-cp) + u.x*sp;
    double m33 = u.z * u.z *(1-cp) + cp;

    this->x = atan2(m32, m33);                      // roll
    this->y = atan2(-m31, sqrt(m11*m11 + m21*m21)); // pitch
    this->z = atan2(m21, m11);                      // yaw

}

//_________________________________________________________________

Matrix3D RPY::toMatrix3D() const {
    Matrix3D M;
    double cr = cos(x);
    double cp = cos(y);
    double cy = cos(z);

    double sr = sin(x);
    double sp = sin(y);
    double sy = sin(z);

    M.r[0][0]= cy*cp;
    M.r[0][1]= cy*sp*sr - sy*cr;
    M.r[0][2]= cy*sp*cr + sy*sr;

    M.r[1][0]= sy*cp;
    M.r[1][1]= sy*sp*sr + cy*cr;
    M.r[1][2]= sy*sp*cr - cy*sr;

    M.r[2][0]= -sp;
    M.r[2][1]= cp*sr;
    M.r[2][2]= cp*cr;

    return M;
}
//_________________________________________________________________

Quaternion RPY::toQuaternion() const {
    Quaternion q;
    double cr2 = cos(x/2);
    double cp2 = cos(y/2);
    double cy2 = cos(z/2);

    double sr2 = sin(x/2);
    double sp2 = sin(y/2);
    double sy2 = sin(z/2);

    q.q0  = cr2*cp2*cy2 + sr2*sp2*sy2;
    q.q.x = sr2*cp2*cy2 - cr2*sp2*sy2;
    q.q.y = cr2*sp2*cy2 + sr2*cp2*sy2;
    q.q.z = cr2*cp2*sy2 - sr2*sp2*cy2;

    return q;
}

RPY RPY::accumulateRotation(RPY& increment) {
    Quaternion orig(*this);
    Quaternion inc(increment);
    orig = orig * inc;
    return orig.toRPY();
}


//_________________________________________________________________

AngleAxis RPY::toAngleAxis() const {
    double r = this->x;
    double p = this->y;
    double y = this->z;

    double phi = acos(0.5*(-1 + cos(p)*cos(y) + cos(r)*cos(y) + sin(r)*sin(p)*sin(y) + cos(r)*cos(p) ));

    double u_x = 1/(2*sin(phi))* (sin(r)*cos(p) -cos(r)*sin(p)*sin(y) +sin(r)*cos(y));
    double u_y = 1/(2*sin(phi))* (sin(r)*sin(y) +cos(r)*sin(p)*cos(y) +sin(p));
    double u_z = 1/(2*sin(phi))* (cos(p)*sin(y) -sin(r)*sin(p)*cos(y) + cos(r)*sin(y));

    Vector3D u(u_x, u_y, u_z);
    AngleAxis u_phi(phi, u);

    return u_phi;
}


//======================Implementation Vector4D==============================

Vector4D::Vector4D() {
    this->x=0;
    this->y=0;
    this->z=0;
}

Vector4D::Vector4D (const double &x, const double &y, const double &z) {
    this->x=x;
    this->y=y;
    this->z=z;
}


Vector4D::Vector4D(const Vector4D& other) : Vector3D(other.x, other.y, other.z) { }

// base class 'class RODOS::Vector3D' should be explicitly initialized in the copy constructor [-Wextra]
//Vector4D::Vector4D(const Vector4D& other) {
//    this->x=other.x;
//    this->y=other.y;
//    this->z=other.z;
//    this->scale=other.scale;
//}

Vector4D::Vector4D (double* arr) {
    this->x = arr[0];
    this->y = arr[1];
    this->z = arr[2];
}

//_________________________________________________________________

Vector4D Vector4D::matVecMult(const Matrix4D& r) const {
    double x = r.r[0][0]*this->x + r.r[0][1]*this->y + r.r[0][2]*this->z+r.r[0][3];
    double y = r.r[1][0]*this->x + r.r[1][1]*this->y + r.r[1][2]*this->z+r.r[1][3];
    double z = r.r[2][0]*this->x + r.r[2][1]*this->y + r.r[2][2]*this->z+r.r[2][3];
    Vector4D result(x,y,z);
    return result;
}
//_________________________________________________________________

Vector3D Vector4D::to3D() const {
    Vector3D v(this->x,this->y,this->z);
    return v;
}
//_________________________________________________________________

Vector4D Vector4D::mRotate(const Matrix4D& r) const {
    Vector4D w(this->matVecMult(r));
    return w;
}
//_________________________________________________________________

void Vector4D::print() const {
    PRINTF("(%3.3f \t %3.3f \t %3.3f)\n",this->x,this->y,this->z);
}
//_________________________________________________________________


//==================Implementation Quaternion===============================


//====Standardkonstruktor====

Quaternion::Quaternion() {
    Vector3D q;
    this->q0 = 1.0;
    this->q  = q;
}

Quaternion::Quaternion(const Quaternion& other) {
    this->q0 = other.q0;
    this->q  = other.q;
}

Quaternion::Quaternion(const double &q0, const double &q1, const double &q2, const double &q3) {
    Vector3D q(q1, q2, q3);
    this->q0 = q0;
    this->q  = q;
}

Quaternion::Quaternion(double* arr) {
    Vector3D vec(arr[1], arr[2], arr[3]);
    this->q0 = arr[0];
    this->q  = vec;
}

Quaternion::Quaternion(const double &q0,const Vector3D& q) {
    this->q = q;
    this->q0 = q0;

}

Quaternion::Quaternion(const AngleAxis& other) {
    this->q0 = cos(other.phi/2);
    this->q  = other.u.normalize().scale(sin(other.phi/2));
}


Quaternion::Quaternion(const Rotor& rot) {
    double angle = acos(rot.cosAngle);
    this->q0 = cos(angle/2);
    this->q  = rot.axis.normalize().scale(sin(angle/2));
}


Quaternion::Quaternion(const Matrix3D& other) {  //Algorithm 1

    double q0,q1,q2,q3;
    q0 = q1 = q2 = q3 = 0;
    double c1 = 1+ other.r[0][0] + other.r[1][1] + other.r[2][2];
    double c2 = 1+ other.r[0][0] - other.r[1][1] - other.r[2][2];
    double c3 = 1- other.r[0][0] + other.r[1][1] - other.r[2][2];
    double c4 = 1- other.r[0][0] - other.r[1][1] + other.r[2][2];
    double c = c1;
    // Find the maximum
    if(c2>c) c = c2;
    if(c3>c) c = c3;
    if(c4>c) c = c4;
    //Distinction of cases
    if(c==c1) {
        c = 0.5 * sqrt(c);
        q0 = c;
        q1 = (other.r[2][1]-other.r[1][2])/(4*c);
        q2 = (other.r[0][2]-other.r[2][0])/(4*c);
        q3 = (other.r[1][0]-other.r[0][1])/(4*c);
    }

    if(c==c2) {
        c = 0.5 * sqrt(c);
        q0 = (other.r[2][1]-other.r[1][2])/(4*c);
        q1 = c;
        q2 = (other.r[1][0]+other.r[0][1])/(4*c);
        q3 = (other.r[0][2]+other.r[2][0])/(4*c);
    }

    if(c==c3) {
        c = 0.5 * sqrt(c);
        q0 = (other.r[0][2]-other.r[2][0])/(4*c);
        q1 = (other.r[1][0]+other.r[0][1])/(4*c);
        q2 = c;
        q3 = (other.r[2][1]+other.r[1][2])/(4*c);
    }

    if(c==c4) {
        c = 0.5 * sqrt(c);
        q0 = (other.r[1][0]-other.r[0][1])/(4*c);
        q1 = (other.r[0][2]+other.r[2][0])/(4*c);
        q2 = (other.r[2][1]+other.r[1][2])/(4*c);
        q3 = c;
    }
    Vector3D q(q1,q2,q3);

    this->q0 = q0;
    this->q = q;
}


Quaternion::Quaternion(const YPR& other) { // DEPRECATED
    double a = other.roll/2;
    double b = other.pitch/2;
    double c = other.yaw/2;

    double cdx = cos(a);
    double sdx = sin(a);
    double cdy = cos(b);
    double sdy = sin(b);
    double cdz = cos(c);
    double sdz = sin(c);

    //% Transforms RPY EulerAngles into Quaternion
    //% dx angle of rotation about x-axis (using RAD) | cdx = cos(dx) etc.
    //% dy angle of rotation about y-axis
    //% dz angle of rotation about z-axis

    double q0 = cdz*cdy*cdx + sdz*sdy*sdx;
    double q1 = cdz*cdy*sdx - sdz*sdy*cdx;
    double q2 = cdz*sdy*cdx + sdz*cdy*sdx;
    double q3 = sdz*cdy*cdx - cdz*sdy*sdx;

    double len  = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

    q0 = q0 / len;
    q1 = q1 / len;
    q2 = q2 / len;
    q3 = q3 / len;

    Vector3D q(q1, q2, q3);

    this->q0 = q0;
    this->q  = q;

}

Quaternion::Quaternion(const RPY& other) {

    double a = other.x/2;
    double b = other.y/2;
    double c = other.z/2;

    double cdx = cos(a);
    double sdx = sin(a);
    double cdy = cos(b);
    double sdy = sin(b);
    double cdz = cos(c);
    double sdz = sin(c);

    //% Transforms RPY EulerAngles into Quaternion
    //% dx angle of rotation about x-axis (using RAD) | cdx = cos(dx) etc.
    //% dy angle of rotation about y-axis
    //% dz angle of rotation about z-axis

    double q0 = cdz*cdy*cdx + sdz*sdy*sdx;
    double q1 = cdz*cdy*sdx - sdz*sdy*cdx;
    double q2 = cdz*sdy*cdx + sdz*cdy*sdx;
    double q3 = sdz*cdy*cdx - cdz*sdy*sdx;

    double len  = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

    q0 = q0 / len;
    q1 = q1 / len;
    q2 = q2 / len;
    q3 = q3 / len;

    Vector3D q(q1, q2, q3);

    this->q0 = q0;
    this->q  = q;
}


bool Quaternion::resetIfNAN() {
    bool error = !isfinite(q0) || !isfinite(q.x) || !isfinite(q.y) || !isfinite(q.z);
    if(error) { q0 = 1;   q.x = q.y = q.z = 0; }
    return error;
}

//_________________________________________________________________

//====getter====

Vector3D Quaternion::getVec() const  {
    Vector3D u= this->q.normalize();
    return u;
}

double Quaternion::getAngle() const {
    return 2*acos(q0);
}
//_________________________________________________________________

//==== Add,Sub,qMult,scale====

Quaternion Quaternion::qAdd(const Quaternion& other) const {
    Quaternion sum;
    sum.q0 = this->q0 + other.q0;
    sum.q  = this->q.vecAdd(other.q);
    return sum;
}
//_________________________________________________________________

Quaternion Quaternion::qSub(const Quaternion& other) const {
    Quaternion diff;
    diff.q0 = this->q0 - other.q0;
    diff.q  = this->q.vecSub(other.q);
    return diff;
}
//_________________________________________________________________

Quaternion Quaternion::scale(const double &factor) const {
    Quaternion scale;
    scale.q0 = this->q0 *factor;
    scale.q  = this->q.scale(factor);
    return scale;
}
//_________________________________________________________________

Quaternion Quaternion::qMult(const Quaternion& other) const {
    Quaternion mult, left, right;
    left = (*this);
    right = other;

    if(left.q.equals(right.q)) {
        mult.q0 = left.q0 + right.q0;
        mult.q = left.q;
    }

    // mult.q0 = left.q0*right.q0 - left.q.dot(right.q);
    // mult.q  = left.q.scale(right.q0).vecAdd(right.q.scale(left.q0))
    //         .vecAdd(left.q.cross(right.q));

    mult.q0 = left.q0 * right.q0 - dotProduct(left.q, right.q);

    mult.q  = left.q  * right.q0 + // WARNING: vectro ops
              right.q * left.q0  + // WARNING: operator +, * etc
              crossProduct(left.q, right.q);
    return mult;
}
//_________________________________________________________________

//==== invert,conjugate,qdivide====
Quaternion Quaternion::invert() const {
    double revNorm = 1/(this->getLen() * this->getLen());
    return this->conjugate().scale(revNorm);
}
//_________________________________________________________________

Quaternion Quaternion::conjugate() const {
    Quaternion conjugate;
    conjugate.q0 = this->q0;
    conjugate.q = this->q.scale(-1);
    return conjugate;
}
//_________________________________________________________________

Quaternion Quaternion::qDivide(const Quaternion& other) const {
    Quaternion division = other;
    Quaternion invert = division.conjugate();
    division = this->qMult(invert);
    return division;
}
//_________________________________________________________________

//====Norm,normalize,isNormalized,equals====
double Quaternion::getLen() const {
    double quads = q0*q0 + q.x*q.x + q.y*q.y + q.z*q.z;
    return sqrt(quads);
}
//_________________________________________________________________

Quaternion Quaternion::normalize() const {
    Quaternion unit;
    unit = this->scale(1.0/this->getLen());
    return unit;
}
//_________________________________________________________________

bool Quaternion::isNormalized() const {
    double len=this->getLen();
    return isAlmost0(len-1);
}
//_________________________________________________________________

bool Quaternion::equals(const Quaternion& other) const {
    return(isAlmost0(q0 - other.q0) && q.equals(other.q));
}
//_________________________________________________________________

//====toEuler,toRotationMatrix,getQuat====
AngleAxis Quaternion::toAngleAxis() const {
    AngleAxis u_phi(this->getAngle(), this->getVec());
    return u_phi;
}
//_________________________________________________________________

Matrix3D Quaternion::toMatrix3D() const { // return matrix representation of quaternion
    Matrix3D R;
    //column1
    R.r[0][0] = 2*this->q0 * this->q0-1 + 2*this->q.x * this->q.x;
    R.r[1][0] = 2*this->q.y*this->q.x   + 2*this->q0*this->q.z;
    R.r[2][0] = 2*this->q.z*this->q.x   - 2*this->q0*this->q.y;

    //column 2
    R.r[0][1] = 2*this->q.y*this->q.x   - 2*this->q0*this->q.z;
    R.r[1][1] = 2*this->q0 * this->q0-1 + 2*this->q.y*this->q.y;
    R.r[2][1] = 2*this->q.y*this->q.z   + 2*this->q0*this->q.x;

    //column 3
    R.r[0][2] = 2*this->q.z*this->q.x   + 2*this->q0*this->q.y;
    R.r[1][2] = 2*this->q.y*this->q.z   - 2*this->q0*this->q.x;
    R.r[2][2] = 2*this->q0 * this->q0-1 + 2*this->q.z * this->q.z;
    return R;
}

YPR Quaternion::toYPR() const { // DEPRECATED
    YPR ypr;
    double m21 = 2*this->q.x * this->q.y  + 2*this->q0 *this->q.z;
    double m11 = 2*this->q0  * this->q0-1 + 2*this->q.x*this->q.x;
    double m31 = 2*this->q.x * this->q.z  - 2*this->q0 *this->q.y;
    double m32 = 2*this->q.y * this->q.z  + 2*this->q0 *this->q.x;
    double m33 = 2*this->q0  * this->q0-1 + 2*this->q.z*this->q.z;

    ypr.roll  = atan2(m32, m33);
    ypr.pitch = atan2(-m31, sqrt(m11*m11 + m21*m21));
    ypr.yaw   = atan2(m21, m11);

    return ypr;
}


RPY Quaternion::toRPY() const {
    RPY rpy;
    double m21 = 2*this->q.x * this->q.y  + 2*this->q0 *this->q.z;
    double m11 = 2*this->q0  * this->q0-1 + 2*this->q.x*this->q.x;
    double m31 = 2*this->q.x * this->q.z  - 2*this->q0 *this->q.y;
    double m32 = 2*this->q.y * this->q.z  + 2*this->q0 *this->q.x;
    double m33 = 2*this->q0  * this->q0-1 + 2*this->q.z*this->q.z;

    rpy.x  = atan2(m32, m33);
    rpy.y  = atan2(-m31, sqrt(m11*m11 + m21*m21));
    rpy.z  = atan2(m21, m11);

    return rpy;
}



//_________________________________________________________________

void Quaternion::print() const {
    PRINTF("[ %3.3f \t (%3.3f \t %3.3f \t %3.3f)]\n",q0, q.x, q.y, q.z);
}

Quaternion operator*(const Matrix4D &left, const Quaternion &right) {

    double q[] = { right.q0, right.q.x, right.q.y, right.q.z };
    double res[4] = { 0.0 };

    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            res[i] += left.r[i][j] * q[j];
        }
    }
    return Quaternion( res );
}


//========================Implementation Matrix3D=============================

//====Constructors====
Matrix3D::Matrix3D() {
    //column1
    r[0][0] = 1.0;
    r[1][0] = 0.0;
    r[2][0] = 0.0;

    //column 2
    r[0][1] = 0.0;
    r[1][1] = 1.0;
    r[2][1] = 0.0;

    //column 3
    r[0][2] = 0.0;
    r[1][2] = 0.0;
    r[2][2] = 1.0;
}
//_________________________________________________________________

Matrix3D::Matrix3D(const Vector3D& column1,const Vector3D& column2,const Vector3D& column3) {
    //column 1
    r[0][0] = column1.x;
    r[1][0] = column1.y;
    r[2][0] = column1.z;

    //column 2
    r[0][1] = column2.x;
    r[1][1] = column2.y;
    r[2][1] = column2.z;

    //column 3
    r[0][2] = column3.x;
    r[1][2] = column3.y;
    r[2][2] = column3.z;

}
//_________________________________________________________________

Matrix3D::Matrix3D(double* arr) {
    int k = 0;
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            r[i][j]=arr[k];
            k++;
        }
    }
}
//_________________________________________________________________

Matrix3D::Matrix3D(const Matrix3D& other) {
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            r[i][j]=other.r[i][j];
        }
    }
}
//_________________________________________________________________

Matrix3D::Matrix3D(const Vector3D& init) { //diagonalmatrix from vector
    //column 1
    r[0][0] = init.x;
    r[1][0] = 0.0;
    r[2][0] = 0.0;

    //column 2
    r[0][1] = 0.0;
    r[1][1] = init.y;
    r[2][1] = 0.0;

    //column 3
    r[0][2] = 0.0;
    r[1][2] = 0.0;
    r[2][2] = init.z;
}
//_________________________________________________________________

Matrix3D::Matrix3D(const YPR& ypr) { // DEPRECATED
    double cy = cos(ypr.yaw);
    double cp = cos(ypr.pitch);
    double cr = cos(ypr.roll);
    double sy = sin(ypr.yaw);
    double sp = sin(ypr.pitch);
    double sr = sin(ypr.roll);

    this->r[0][0]= cy*cp;
    this->r[0][1]= cy*sp*sr - sy*cr;
    this->r[0][2]= cy*sp*cr + sy*sr;

    this->r[1][0]= sy*cp;
    this->r[1][1]= sy*sp*sr + cy*cr;
    this->r[1][2]= sy*sp*cr - cy*sr;

    this->r[2][0]= -sp;
    this->r[2][1]= cp*sr;
    this->r[2][2]= cp*cr;
}

Matrix3D::Matrix3D(const RPY& rpy) {
    double cr = cos(rpy.x);
    double cp = cos(rpy.y);
    double cy = cos(rpy.z);

    double sr = sin(rpy.x);
    double sp = sin(rpy.y);
    double sy = sin(rpy.z);

    this->r[0][0]= cy*cp;
    this->r[0][1]= cy*sp*sr - sy*cr;
    this->r[0][2]= cy*sp*cr + sy*sr;

    this->r[1][0]= sy*cp;
    this->r[1][1]= sy*sp*sr + cy*cr;
    this->r[1][2]= sy*sp*cr - cy*sr;

    this->r[2][0]= -sp;
    this->r[2][1]= cp*sr;
    this->r[2][2]= cp*cr;
}
//_________________________________________________________________

Matrix3D::Matrix3D(const AngleAxis& other) { // general rotation matrix
    Vector3D u = other.u;
    double phi = other.phi;
    double cp  = cos(phi);
    double sp  = sin(phi);

    // 1 Spalte
    this->r[0][0]= u.x *u.x *(1-cp) + cp;
    this->r[1][0]= u.x *u.y *(1-cp) + u.z*sp;
    this->r[2][0]= u.x *u.z *(1-cp) - u.y*sp;

    // 2 Spalte
    this->r[0][1]= u.x *u.y *(1-cp) - u.z*sp;
    this->r[1][1]= u.y *u.y *(1-cp) + cp;
    this->r[2][1]= u.z *u.y *(1-cp) + u.x*sp;

    // 3 Spalte
    this->r[0][2]= u.x *u.z *(1-cp) + u.y*sp;
    this->r[1][2]= u.y *u.z *(1-cp) - u.x*sp;
    this->r[2][2]= u.z *u.z *(1-cp) + cp;
}
//_________________________________________________________________

Matrix3D::Matrix3D(const Quaternion& other) {

    //column1
    this->r[0][0] = 2*other.q0*other.q0-1 + 2*other.q.x * other.q.x;
    this->r[1][0] = 2*other.q.y*other.q.x  + 2*other.q0*other.q.z;
    this->r[2][0] = 2*other.q.z*other.q.x  - 2*other.q0*other.q.y;

    //column 2
    this->r[0][1] = 2*other.q.y*other.q.x - 2*other.q0*other.q.z;
    this->r[1][1] = 2*other.q0*other.q0-1 + 2*other.q.y*other.q.y;
    this->r[2][1] = 2*other.q.y*other.q.z + 2*other.q0*other.q.x;

    //column 3
    this->r[0][2] = 2*other.q.z*other.q.x + 2*other.q0*other.q.y;
    this->r[1][2] = 2*other.q.y*other.q.z - 2*other.q0*other.q.x;
    this->r[2][2] = 2*other.q0*other.q0-1 + 2*other.q.z * other.q.z;
}
//_________________________________________________________________

//====mAdd,mSub,scale,mMult,getAngle,getVec====
double Matrix3D::getAngle() const {
    double angle;
    angle = acos(0.5*(this->r[0][0]+this->r[1][1]+this->r[2][2]-1));
    return angle;
}
//_________________________________________________________________

Vector3D Matrix3D::getVec() const {
    double x,y,z,angle;
    angle = this->getAngle();
    x = 1.0/(2*sin(angle)) * (this->r[2][1]-this->r[1][2]);
    y = 1.0/(2*sin(angle)) * (this->r[0][2]-this->r[2][0]);
    z = 1.0/(2*sin(angle)) * (this->r[1][0]-this->r[0][1]);
    Vector3D u(x,y,z);

    return u;
}
//_________________________________________________________________

//====Getter for columns and rows ====
Vector3D Matrix3D::getRow1() const {
    Vector3D row1(this->r[0][0],this->r[0][1],this->r[0][2]);
    return row1;
}
//_________________________________________________________________

Vector3D Matrix3D::getRow2() const {
    Vector3D row1(this->r[1][0],this->r[1][1],this->r[1][2]);
    return row1;
}
//_________________________________________________________________

Vector3D Matrix3D::getRow3() const {
    Vector3D row1(this->r[2][0],this->r[2][1],this->r[2][2]);
    return row1;
}
//_________________________________________________________________

Vector3D Matrix3D::getColumn1() const {
    Vector3D row1(this->r[0][0],this->r[1][0],this->r[2][0]);
    return row1;
}
//_________________________________________________________________

Vector3D Matrix3D::getColumn2() const {
    Vector3D row1(this->r[0][1],this->r[1][1],this->r[2][1]);
    return row1;
}
//_________________________________________________________________

Vector3D Matrix3D::getColumn3() const {
    Vector3D row1(this->r[0][2],this->r[1][2],this->r[2][2]);
    return row1;
}
//_________________________________________________________________

//====Setter for columns and rows =====
void Matrix3D::setRow1(const Vector3D& row) {
    this->r[0][0]= row.x;
    this->r[0][1]= row.y;
    this->r[0][2]= row.z;
}
//_________________________________________________________________
void Matrix3D::setRow2(const Vector3D& row) {
    this->r[1][0]= row.x;
    this->r[1][1]= row.y;
    this->r[1][2]= row.z;
}
//_________________________________________________________________

void Matrix3D::setRow3(const Vector3D& row) {
    this->r[2][0]= row.x;
    this->r[2][1]= row.y;
    this->r[2][2]= row.z;
}
//_________________________________________________________________

void Matrix3D::setColumn1(const Vector3D& column) {
    this->r[0][0]= column.x;
    this->r[1][0]= column.y;
    this->r[2][0]= column.z;
}
//_________________________________________________________________

void Matrix3D::setColumn2(const Vector3D& column) {
    this->r[0][1]= column.x;
    this->r[1][1]= column.y;
    this->r[2][1]= column.z;
}
//_________________________________________________________________

void Matrix3D::setColumn3(const Vector3D& column) {
    this->r[0][2]= column.x;
    this->r[1][2]= column.y;
    this->r[2][2]= column.z;
}
//_________________________________________________________________

Matrix3D Matrix3D::mAdd(const Matrix3D& other) const {
    Matrix3D add;

    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            add.r[i][j] = this->r[i][j] + other.r[i][j];
        }
    }
    return add;
}
//_________________________________________________________________

Matrix3D Matrix3D::mSub(const Matrix3D& other) const {
    Matrix3D sub;
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            sub.r[i][j] = this->r[i][j] - other.r[i][j];
        }
    }
    return sub;
}
//_________________________________________________________________

Matrix3D Matrix3D::scale(const double &factor) const {
    Matrix3D scale;
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            scale.r[i][j] = this->r[i][j]*factor;
        }
    }
    return scale;
}
//_________________________________________________________________
Matrix3D Matrix3D::mMult(const Matrix3D& other) const {
    Matrix3D mult;
    double sum;

    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++) {
            sum = 0.0;
            for (int k=0; k<3; k++)
                sum += this->r[i][k] * other.r[k][j];
            mult.r[i][j] = sum;
        }
    return mult;
}
//_________________________________________________________________
//==== invert,transpose,mDivide====
Matrix3D Matrix3D::cofac() const {
    Matrix3D cofak;
    cofak.r[0][0]=this->r[1][1] * this->r[2][2]-this->r[2][1] * this->r[1][2];
    cofak.r[0][1]=this->r[1][2] * this->r[2][0]-this->r[1][0] * this->r[2][2];
    cofak.r[0][2]=this->r[1][0] * this->r[2][1]-this->r[2][0] * this->r[1][1];
    cofak.r[1][0]=this->r[2][1] * this->r[0][2]-this->r[0][1] * this->r[2][2];
    cofak.r[1][1]=this->r[0][0] * this->r[2][2]-this->r[2][0] * this->r[0][2];
    cofak.r[1][2]=this->r[2][0] * this->r[0][1]-this->r[0][0] * this->r[2][1];
    cofak.r[2][0]=this->r[0][1] * this->r[1][2]-this->r[1][1] * this->r[0][2];
    cofak.r[2][1]=this->r[1][0] * this->r[0][2]-this->r[1][2] * this->r[0][0];
    cofak.r[2][2]=this->r[0][0] * this->r[1][1]-this->r[1][0] * this->r[0][1];
    return cofak;
}
//_________________________________________________________________
Matrix3D Matrix3D::adjoint() const {
    Matrix3D adjoint;
    adjoint = this->cofac().transpose();
    return adjoint;
}
//_________________________________________________________________
double Matrix3D::trace() const {
    return this->r[0][0] + this->r[1][1] + this->r[2][2];
}
//_________________________________________________________________
Matrix3D Matrix3D::invert() const {
    double det;
    Matrix3D inverse;

    if(this->isOrthogonal()) {
        inverse = this->transpose();
        return inverse;
    }

    det = this->determinant();
    inverse = this->adjoint().scale(1/det);

    return inverse;
}
//_________________________________________________________________


Matrix3D Matrix3D::transpose() const {
    Matrix3D transpose;
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            transpose.r[i][j]=this->r[j][i];
        }
    }
    return transpose;
}
//_________________________________________________________________

Matrix3D Matrix3D::mDivide(const Matrix3D& other) const {
    Matrix3D inverse,divide;

    inverse = other.invert();
    divide  = this->mMult(inverse);
    return divide;
}
//_________________________________________________________________
//====Fundamentalrotations====

void Matrix3D::rotationX(const double &angle) {
    Vector3D c1(1.0, 0.0, 0.0);
    setColumn1(c1);
    Vector3D c2(0.0, cos(angle), sin(angle));
    setColumn2(c2);
    Vector3D c3(0.0, -sin(angle), cos(angle));
    setColumn3(c3);
}
//_________________________________________________________________
void Matrix3D::rotationY(const double &angle) {
    Vector3D c1(cos(angle), 0.0, -sin(angle));
    setColumn1(c1);
    Vector3D c2(0.0, 1.0, 0.0);
    setColumn2(c2);
    Vector3D c3(sin(angle), 0.0, cos(angle));
    setColumn3(c3);
}
//_________________________________________________________________
void Matrix3D::rotationZ(const double &angle) {
    Vector3D c1(cos(angle), sin(angle), 0.0);
    setColumn1(c1);
    Vector3D c2(-sin(angle), cos(angle), 0.0);
    setColumn2(c2);
    Vector3D c3(0.0, 0.0, 1.0);
    setColumn3(c3);
}
//_________________________________________________________________
//==== determinant,isOrthogonal ,equals====
double Matrix3D::determinant() const {
    double det,a1,a2,a3,a4,a5,a6;
    a1 = this->r[0][0]*this->r[1][1]*this->r[2][2];
    a2 = this->r[0][1]*this->r[1][2]*this->r[2][0];
    a3 = this->r[0][2]*this->r[1][0]*this->r[2][1];
    a4 = -(this->r[2][0]*this->r[1][1]*this->r[0][2]);
    a5 = -(this->r[2][1]*this->r[1][2]*this->r[0][0]);
    a6 = -(this->r[2][2]*this->r[1][0]*this->r[0][1]);
    det = a1+a2+a3+a4+a5+a6;
    return det;
}
//_________________________________________________________________

bool Matrix3D::isOrthogonal() const {
    Matrix3D Id;
    if(this->mMult(this->transpose()).equals(Id)) {
        return true;
    }
    return false;
}
//_________________________________________________________________
bool Matrix3D::equals(const Matrix3D& other) const {
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            if(!isAlmost0(this->r[i][j] - other.r[i][j])) {
                return false;
            }
        }
    }
    return true;
}
//_________________________________________________________________

//==== toQuaternion====
Quaternion Matrix3D::toQuaternion() const {

    double q0,q1,q2,q3;
    q0 = q1 = q2 = q3 = 0;
    double c1 = 1+ this->r[0][0] + this->r[1][1] + this->r[2][2];
    double c2 = 1+ this->r[0][0] - this->r[1][1] - this->r[2][2];
    double c3 = 1- this->r[0][0] + this->r[1][1] - this->r[2][2];
    double c4 = 1- this->r[0][0] - this->r[1][1] + this->r[2][2];
    double c = c1;
    // Find the maximum
    if(c2>c) c = c2;
    if(c3>c) c = c3;
    if(c4>c) c = c4;
    //Dis of cases
    if(c==c1) {
        c = 0.5 * sqrt(c);
        q0 = c;
        q1 = (this->r[2][1]-this->r[1][2])/(4*c);
        q2 = (this->r[0][2]-this->r[2][0])/(4*c);
        q3 = (this->r[1][0]-this->r[0][1])/(4*c);
    }

    if(c==c2) {
        c = 0.5 * sqrt(c);
        q0 = (this->r[2][1]-this->r[1][2])/(4*c);
        q1 = c;
        q2 = (this->r[1][0]+this->r[0][1])/(4*c);
        q3 = (this->r[0][2]+this->r[2][0])/(4*c);
    }

    if(c==c3) {
        c = 0.5 * sqrt(c);
        q0 = (this->r[0][2]-this->r[2][0])/(4*c);
        q1 = (this->r[1][0]+this->r[0][1])/(4*c);
        q2 = c;
        q3 = (this->r[2][1]+this->r[1][2])/(4*c);
    }

    if(c==c4) {
        c = 0.5 * sqrt(c);
        q0 = (this->r[1][0]-this->r[0][1])/(4*c);
        q1 = (this->r[0][2]+this->r[2][0])/(4*c);
        q2 = (this->r[2][1]+this->r[1][2])/(4*c);
        q3 = c;
    }
    Quaternion q(q0,q1,q2,q3);

    return q;

}
//_________________________________________________________________

YPR Matrix3D::toYPR() const { // DEPRECATED
    YPR y;
    double m21 = this->r[1][0];
    double m11 = this->r[0][0];
    double m31 = this->r[2][0];
    double m32 = this->r[2][1];
    double m33 = this->r[2][2];
    y.pitch = atan2(-m31, sqrt(m11*m11 + m21*m21));
    y.yaw   = atan2(m21, m11);
    y.roll  = atan2(m32, m33);

    return y;
}

RPY Matrix3D::toRPY() const {
    RPY rpy;
    double m21 = this->r[1][0];
    double m11 = this->r[0][0];
    double m31 = this->r[2][0];
    double m32 = this->r[2][1];
    double m33 = this->r[2][2];
    rpy.x     = atan2(m32, m33);
    rpy.y     = atan2(-m31, sqrt(m11*m11 + m21*m21));
    rpy.z     = atan2(m21, m11);

    return rpy;
}

//_________________________________________________________________

AngleAxis Matrix3D::toAngleAxis() const {
    double phi = acos(0.5*(this->r[0][0] + this->r[1][1] + this->r[2][2]-1));
    double sp = sin(phi);

    double x = 1/(2*sp)* (this->r[2][1] - this->r[1][2]);
    double y = 1/(2*sp)* (this->r[0][2] - this->r[2][0]);
    double z = 1/(2*sp)* (this->r[1][0] - this->r[0][1]);
    Vector3D u(x,y,z);
    AngleAxis u_phi(phi, u.normalize());
    return u_phi;
}
//_________________________________________________________________

void Matrix3D::print() const {
    PRINTF(" __\t\t\t\t\t\t__\n");
    for(int k=0; k<3; k++) {
        PRINTF("| ");
        for(int l = 0; l<3; l++) {
            PRINTF("%3.3f \t ",this->r[k][l]);
        }
        PRINTF(" |\n");
    }
    PRINTF(" --\t\t\t\t\t\t--\n");
}
//_________________________________________________________________


//=============Implementation YPR============================================

YPR::YPR() {
    this->yaw   = 0;
    this->pitch = 0;
    this->roll  = 0;
}

YPR::YPR(const YPR& other) {
    this->yaw   = other.yaw;
    this->pitch = other.pitch;
    this->roll  = other.roll;
}
//_________________________________________________________________

YPR::YPR(const double &yaw,const double &pitch,const double &roll) {
    this->yaw   = yaw;
    this->pitch = pitch;
    this->roll  = roll;
}
//_________________________________________________________________

YPR::YPR(const Quaternion& q) {
    double m21 = 2*q.q.x*q.q.y + 2*q.q0*q.q.z;
    double m11 = 2*q.q0*q.q0-1 + 2*q.q.x*q.q.x;
    double m31 = 2*q.q.x*q.q.z - 2*q.q0*q.q.y;
    double m32 = 2*q.q.y*q.q.z + 2*q.q0*q.q.x;
    double m33 = 2*q.q0*q.q0-1 + 2*q.q.z*q.q.z;

    this->pitch = atan2(-m31, sqrt(m11*m11 + m21*m21));
    this->yaw   = atan2(m21, m11);
    this->roll  = atan2(m32, m33);
}
//_________________________________________________________________

YPR::YPR(const Matrix3D& M) {
    double m21 = M.r[1][0];
    double m11 = M.r[0][0];
    double m31 = M.r[2][0];
    double m32 = M.r[2][1];
    double m33 = M.r[2][2];

    this->pitch = atan2(-m31, sqrt(m11*m11 + m21*m21));
    this->yaw   = atan2(m21, m11);
    this->roll  = atan2(m32, m33);
}
//_________________________________________________________________

YPR::YPR(const AngleAxis& other) {

    Vector3D u = other.u;
    double phi = other.phi;
    double cp  = cos(phi);
    double sp  = sin(phi);


    double m21 = u.x * u.y *(1-cp) + u.z*sp;
    double m11 = u.x * u.x *(1-cp) + cp;
    double m31 = u.x * u.z *(1-cp) - u.y*sp;
    double m32 = u.y * u.z *(1-cp) + u.x*sp;
    double m33 = u.z * u.z *(1-cp) + cp;

    this->pitch = atan2(-m31, sqrt(m11*m11 + m21*m21));
    this->yaw   = atan2(m21, m11);
    this->roll  = atan2(m32, m33);
}
//_________________________________________________________________

YPR  YPR::scale(const double &factor) const {
    YPR scale;
    scale.pitch = pitch * factor;
    scale.yaw   = yaw   * factor;
    scale.roll  = roll  * factor;
    return scale;
}

//_________________________________________________________________

Matrix3D YPR::toMatrix3D() const {
    Matrix3D M;
    double cy = cos(yaw);
    double cp = cos(pitch);
    double cr = cos(roll);
    double sy = sin(yaw);
    double sp = sin(pitch);
    double sr = sin(roll);

    M.r[0][0]= cy*cp;
    M.r[0][1]= cy*sp*sr - sy*cr;
    M.r[0][2]= cy*sp*cr + sy*sr;

    M.r[1][0]= sy*cp;
    M.r[1][1]= sy*sp*sr + cy*cr;
    M.r[1][2]= sy*sp*cr - cy*sr;

    M.r[2][0]= -sp;
    M.r[2][1]= cp*sr;
    M.r[2][2]= cp*cr;

    return M;
}
//_________________________________________________________________

Quaternion YPR::toQuaternion() const {
    Quaternion q;
    double cy2 = cos(yaw/2);
    double cp2 = cos(pitch/2);
    double cr2 = cos(roll/2);
    double sy2 = sin(yaw/2);
    double sp2 = sin(pitch/2);
    double sr2 = sin(roll/2);

    q.q0  = cr2*cp2*cy2 + sr2*sp2*sy2;
    q.q.x = sr2*cp2*cy2 - cr2*sp2*sy2;
    q.q.y = cr2*sp2*cy2 + sr2*cp2*sy2;
    q.q.z = cr2*cp2*sy2 - sr2*sp2*cy2;

    return q;

}
//_________________________________________________________________

AngleAxis YPR::toAngleAxis() const {
    double y = this->yaw;
    double p = this->pitch;
    double r = this->roll;

    double phi = acos(0.5*(-1 + cos(p)*cos(y) + cos(r)*cos(y) + sin(r)*sin(p)*sin(y) + cos(r)*cos(p) ));

    double u_x = 1/(2*sin(phi))* (sin(r)*cos(p) -cos(r)*sin(p)*sin(y) +sin(r)*cos(y));
    double u_y = 1/(2*sin(phi))* (sin(r)*sin(y) +cos(r)*sin(p)*cos(y) +sin(p));
    double u_z = 1/(2*sin(phi))* (cos(p)*sin(y) -sin(r)*sin(p)*cos(y) + cos(r)*sin(y));

    Vector3D u(u_x, u_y, u_z);
    AngleAxis u_phi(phi, u);

    return u_phi;
}
//_________________________________________________________________

void YPR::print() const {
    PRINTF("(%3.3f*PI \t %3.3f*PI \t %3.3f*PI)\n", yaw/M_PI, pitch/M_PI, roll/M_PI);
}
//_________________________________________________________________


//==================Implementation AngleAxis================================


//==== Constructors=====
AngleAxis::AngleAxis() {
    Vector3D u(1,0,0);
    this->u   = u;
    this->phi = 0;
}
//_________________________________________________________________

AngleAxis::AngleAxis(const double &phi, const double &x, const double &y, const double &z) {
    this->phi = phi;
    this->u.x = x;
    this->u.y = y;
    this->u.z = z;
    this->u = this->u.normalize();
}
//_________________________________________________________________

AngleAxis::AngleAxis(const AngleAxis& other) {
    this->u   = other.u;
    this->phi = other.phi;
}
//_________________________________________________________________

AngleAxis::AngleAxis(const double &phi, const Vector3D& u) {
    this->u   = u;
    this->u   = this->u.normalize();
    this->phi = phi;
}
//_________________________________________________________________

AngleAxis::AngleAxis(const Quaternion& q) {
    this->u   = q.getVec().normalize();
    this->phi = q.getAngle();
}
//_________________________________________________________________

AngleAxis::AngleAxis(const Matrix3D& M) {
    this->phi= acos(0.5*(M.r[0][0] + M.r[1][1] + M.r[2][2]-1));

    double x = 1/(2*sin(phi))* (M.r[2][1] - M.r[1][2]);
    double y = 1/(2*sin(phi))* (M.r[0][2] - M.r[2][0]);
    double z = 1/(2*sin(phi))* (M.r[1][0] - M.r[0][1]);
    Vector3D u(x,y,z);
    this->u = u.normalize();
}
//_________________________________________________________________

AngleAxis::AngleAxis(const YPR& ypr) { // DEPRECATED
    double y = ypr.yaw;
    double p = ypr.pitch;
    double r = ypr.roll;

    double phi = acos(0.5*(-1 + cos(p)*cos(y) + cos(r)*cos(y) + sin(r)*sin(p)*sin(y) + cos(r)*cos(p) ));
    double u_x = 1/(2*sin(phi))* (sin(r)*cos(p) -cos(r)*sin(p)*sin(y) +sin(r)*cos(y));
    double u_y = 1/(2*sin(phi))* (sin(r)*sin(y) +cos(r)*sin(p)*cos(y) +sin(p));
    double u_z = 1/(2*sin(phi))* (cos(p)*sin(y) -sin(r)*sin(p)*cos(y) + cos(r)*sin(y));
    Vector3D u(u_x,u_y,u_z);

    this->phi = phi;
    this->u = u.normalize();

}

AngleAxis::AngleAxis(const RPY& rpy) {
    double r = rpy.x;
    double p = rpy.y;
    double y = rpy.z;

    double phi = acos(0.5*(-1 + cos(p)*cos(y) + cos(r)*cos(y) + sin(r)*sin(p)*sin(y) + cos(r)*cos(p) ));
    double u_x = 1/(2*sin(phi))* (sin(r)*cos(p) -cos(r)*sin(p)*sin(y) +sin(r)*cos(y));
    double u_y = 1/(2*sin(phi))* (sin(r)*sin(y) +cos(r)*sin(p)*cos(y) +sin(p));
    double u_z = 1/(2*sin(phi))* (cos(p)*sin(y) -sin(r)*sin(p)*cos(y) + cos(r)*sin(y));
    Vector3D u(u_x,u_y,u_z);

    this->phi = phi;
    this->u = u.normalize();
}
//_________________________________________________________________

Quaternion AngleAxis::toQuaternion() const {
    double q0  = cos(this->phi/2);
    Vector3D q = this->u.scale(sin(this->phi/2));
    Quaternion quat(q0,q);
    return quat;
}
//_________________________________________________________________

Matrix3D AngleAxis::toMatrix3D() const { // General rotation matrix
    Matrix3D R;
    Vector3D u = this->u;
    double phi = this->phi;

    // 1 Spalte
    R.r[0][0]= u.x *u.x *(1-cos(phi)) +cos(phi);
    R.r[1][0]= u.x *u.y *(1-cos(phi)) +u.z*sin(phi);
    R.r[2][0]= u.x *u.z *(1-cos(phi)) -u.y*sin(phi);

    // 2 Spalte
    R.r[0][1]= u.x *u.y *(1-cos(phi)) -u.z*sin(phi);
    R.r[1][1]= u.y *u.y *(1-cos(phi)) +cos(phi);
    R.r[2][1]= u.z *u.y *(1-cos(phi)) +u.x*sin(phi);

    // 3 Spalte
    R.r[0][2]= u.x *u.z *(1-cos(phi)) +u.y*sin(phi);
    R.r[1][2]= u.y *u.z *(1-cos(phi)) -u.x*sin(phi);
    R.r[2][2]= u.z *u.z *(1-cos(phi)) +cos(phi);

    return R;
}
//_________________________________________________________________

YPR AngleAxis::toYPR() const { // DEPRECATED
    YPR ypr;
    Vector3D u = this->u;
    double phi = this->phi;
    double m21 = u.x *u.y *(1-cos(phi)) + u.z*sin(phi);
    double m11 = u.x *u.x *(1-cos(phi)) + cos(phi);
    double m31 = u.x *u.z *(1-cos(phi)) - u.y*sin(phi);
    double m32 = u.y *u.z *(1-cos(phi)) + u.x*sin(phi);
    double m33 = u.z *u.z *(1-cos(phi)) + cos(phi);

    ypr.pitch = atan2(-m31,sqrt(m11*m11+m21*m21));
    ypr.yaw = atan2(m21 , m11);
    ypr.roll = atan2(m32 , m33);

    return ypr;

}

RPY AngleAxis::toRPY() const {
    RPY rpy;
    Vector3D u = this->u;
    double phi = this->phi;
    double m21 = u.x *u.y *(1-cos(phi)) + u.z*sin(phi);
    double m11 = u.x *u.x *(1-cos(phi)) + cos(phi);
    double m31 = u.x *u.z *(1-cos(phi)) - u.y*sin(phi);
    double m32 = u.y *u.z *(1-cos(phi)) + u.x*sin(phi);
    double m33 = u.z *u.z *(1-cos(phi)) + cos(phi);

    rpy.x = atan2(m32 , m33);
    rpy.y = atan2(-m31,sqrt(m11*m11+m21*m21)); 
    rpy.z = atan2(m21 , m11);

    return rpy;
}



//_________________________________________________________________

bool AngleAxis::equals(AngleAxis& other) {
    if (isAlmost0(FMod2p(this->phi - other.phi + M_PI) - M_PI) && this->u.equals(other.u)) return true;
    
    if (isAlmost0(FMod2p(this->phi + other.phi + M_PI) - M_PI) && this->u.equals(-1 * other.u)) return true;
    
    return false;
}

void AngleAxis::print() const {
    PRINTF("[%3.3f*PI \t %3.3f \t %3.3f \t %3.3f]\n",phi/M_PI, u.x, u.y, u.z);
}
//_________________________________________________________________


//========================Implementation Matrix4D============================


//====Constructors====
Matrix4D::Matrix4D() {
    //column1
    r[0][0] = 1.0;
    r[1][0] = 0.0;
    r[2][0] = 0.0;
    r[3][0] = 0.0;

    //column 2
    r[0][1] = 0.0;
    r[1][1] = 1.0;
    r[2][1] = 0.0;
    r[3][1] = 0.0;

    //column 3
    r[0][2] = 0.0;
    r[1][2] = 0.0;
    r[2][2] = 1.0;
    r[3][2] = 0.0;

    //column 4
    r[0][3] = 0.0;
    r[1][3] = 0.0;
    r[2][3] = 0.0;
    r[3][3] = 1.0;
}
//_________________________________________________________________

Matrix4D::Matrix4D(const Matrix3D& rot, const Vector3D& trans) {
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            r[i][j]=rot.r[i][j];
        }
    }

    //column 4
    r[0][3] = trans.x;
    r[1][3] = trans.y;
    r[2][3] = trans.z;
    r[3][3] = 1.0;

    //row 4
    r[3][0]=0.0;
    r[3][1]=0.0;
    r[3][2]=0.0;

}
//_________________________________________________________________

Matrix4D::Matrix4D(double* arr) {
    int k = 0;
    for(int i = 0 ; i<4; i++) {
        for(int j = 0; j<4; j++) {
            r[i][j]=arr[k];
            k++;
        }
    }
}

Matrix3D Matrix4D::getRotation() const {
    Matrix3D R;
    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            R.r[i][j]=this->r[i][j];
        }
    }

    return R;
}
//_________________________________________________________________

Vector3D Matrix4D::getTranslation() const {
    Vector3D trans(this->r[0][3],this->r[1][3],this->r[2][3]);
    return trans;

}
//_________________________________________________________________

Matrix4D Matrix4D::scale(const double &factor) const {
    Matrix4D scale;
    for(int i = 0 ; i<4; i++) {
        for (int j = 0; j < 4; j++) {
            scale.r[i][j]=this->r[i][j]*factor;
        }
    }
    return scale;
}
//_________________________________________________________________

Matrix4D Matrix4D::mMult(const Matrix4D& other) const {
    Matrix4D mult;
    double sum;

    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++) {
            sum = 0.0;
            for (int k=0; k<4; k++)
                sum += this->r[i][k] * other.r[k][j];
            mult.r[i][j] = sum;
        }
    return mult;
}
//_________________________________________________________________

//==== invert====
Matrix4D Matrix4D::invert() const {
    Matrix3D transpose = this->getRotation().transpose();
    Vector3D invtrans  = this->getTranslation().matVecMult(transpose);
    invtrans = invtrans.scale(-1);
    Matrix4D inverse(transpose,invtrans);

    return inverse;
}
//_________________________________________________________________

bool Matrix4D::equals(Matrix4D& other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!isAlmost0(this->r[i][j] - other.r[i][j])) return false;
        }
    }
    
    return true;
}
//-----------------------------------------------------------------

void Matrix4D::print() const {
    for(int k=0; k<4; k++) {
        for(int l = 0; l<4; l++) {
            if(this->r[k][l]>=0) {
                PRINTF(" ");
            }
            PRINTF("%3.3f \t",this->r[k][l]);
        }
        PRINTF("\n");
    }
}
//_________________________________________________________________

Matrix4D operator+(const Matrix4D &left, const Matrix4D &right) {

    Matrix4D sum;
    for ( int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            sum.r[i][j] = left.r[i][j] + right.r[i][j];
        }
    }

    return sum;
}

Matrix4D operator*(const Matrix4D &left, const double   &right) {

    Matrix4D res;

    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            res.r[i][j] = left.r[i][j] * right;
        }
    }
    return res;
}

Matrix4D operator/(const Matrix4D &left, const double   &right) {

    Matrix4D res;

    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            res.r[i][j] = left.r[i][j] * 1.0/right;
        }
    }
    return res;
}


//==================Implementation CoordinateFrame3D========================


//====Constructors====

//changed to orthogonal normalized coordinate system
CoordinateFrame3D::CoordinateFrame3D(){
    Vector3D x(1,0,0);
    Vector3D y(0,1,0);
    Vector3D z(0,0,1);
    Vector3D o;

    this->x=x;
    this->y=y;
    this->z=z;
    this->origin = o;
}
//_________________________________________________________________

CoordinateFrame3D::CoordinateFrame3D(const Vector3D& x, const Vector3D& y,const Vector3D& z, const Vector3D& origin) {
    this->x = x.normalize();
    this->y = y.normalize();
    this->z = z.normalize();
    this->origin=origin;
}
//_________________________________________________________________

CoordinateFrame3D::CoordinateFrame3D(const Vector3D& x, const Vector3D& y,const Vector3D& origin) {
    this->x=x.normalize();
    this->y= x.cross(y).normalize();
    this->z = x.cross(this->y).normalize();
    this->origin = origin;
}
//_________________________________________________________________

CoordinateFrame3D::CoordinateFrame3D(const CoordinateFrame3D& other) {
    this->x=other.x;
    this->y=other.y;
    this->z =other.z;
    this->origin=other.origin;
}
//_________________________________________________________________

//====methods=======
Matrix4D CoordinateFrame3D::mapTo(const CoordinateFrame3D& rotated) const { // derives rotation matrix A : [v]r = A[v]f [v]f given
    Matrix3D R;
    //column1
    R.r[0][0] = this->x.dot(rotated.x);
    R.r[1][0] = this->y.dot(rotated.x);
    R.r[2][0] = this->z.dot(rotated.x);


    //column 2
    R.r[0][1] = this->x.dot(rotated.y);
    R.r[1][1] = this->y.dot(rotated.y);
    R.r[2][1] = this->z.dot(rotated.y);


    //column 3
    R.r[0][2] = this->x.dot(rotated.z);
    R.r[1][2] = this->y.dot(rotated.z);
    R.r[2][2] = this->z.dot(rotated.z);

    Vector3D trans;
    trans = rotated.origin.vecSub(this->origin);
    Matrix4D Rot(R,trans);

    return Rot;

}
//_________________________________________________________________

//====rotate,translate====
CoordinateFrame3D CoordinateFrame3D::translate(const Vector3D& trans) const {
    CoordinateFrame3D T((*this));
    T.origin = this->origin.vecAdd(trans);

    return T;
}
//_________________________________________________________________
CoordinateFrame3D CoordinateFrame3D::rotate(const Matrix3D& rot) const {
    CoordinateFrame3D R;
    R.x = this->x.mRotate(rot);
    R.y = this->y.mRotate(rot);
    R.z = this->z.mRotate(rot);
    R.origin= this->origin;

    return R;
}
//_________________________________________________________________
CoordinateFrame3D CoordinateFrame3D::rotate(const Quaternion& q) const {
    CoordinateFrame3D R;
    R.x = this->x.qRotate(q);
    R.y = this->y.qRotate(q);
    R.z = this->x.qRotate(q);
    R.origin= this->origin;

    return R;
}
//_________________________________________________________________


//=====================Implementation Complex================================


Complex::Complex() {
    this->Re = 0.0;
    this->Im = 0.0;
}
//_________________________________________________________________

Complex::Complex(const double &Re, const double &Im) {
    this->Re = Re;
    this->Im = Im;
}
//_________________________________________________________________

Complex::Complex(const Complex& other) {
    this->Re = other.Re;
    this->Im = other.Im;
}
//_________________________________________________________________

Complex Complex::cAdd(const Complex& other) const {
    Complex z;
    z.Re = other.Re + this->Re;
    z.Im = other.Im + this->Im;
    return z;
}
//_________________________________________________________________

Complex Complex::cSub(const Complex& other) const {
    Complex z;
    z.Re = other.Re - this->Re;
    z.Im = other.Im - this->Im;
    return z;
}
//_________________________________________________________________

Complex Complex::cScale(const double &scale) const {
    Complex z =(*this);
    z.Re *= scale;
    z.Im *= scale;
    return z;
}
//_________________________________________________________________

Complex Complex::cMult(const Complex& other) const {
    Complex z;
    z.Re = this->Re*other.Re -(other.Im*this->Im);
    z.Im = this->Re*other.Im +this->Im*other.Re;

    return z;
}
//_________________________________________________________________

Complex Complex::cPow(const int &exponent) const {
    Complex z;
    if(exponent ==0) {
        z.Re =1;
        return z;
    }

    z= (*this);
    for(int i = 1; i<exponent; i++) {
        z=z.cMult(*this);
    }

    return z;
}
//_________________________________________________________________

Complex Complex::cExp() const {
    Complex z;
    z.Re = exp(this->Re)*cos(this->Im);
    z.Im = exp(this->Re)*sin(this->Im);

    return z;
}
//_________________________________________________________________


//============Implementation Polar====================================

Polar::Polar() {
    this->r     = 0;
    this->phi   = 0;
    this->theta = 0;
}
//_________________________________________________________________

Polar::Polar(const double &r,const double &phi,const double &theta) {
    this->r     = r;
    this->phi   = phi;
    this->theta = theta;
}
//_________________________________________________________________

Polar::Polar(const Polar& other) {
    this->r     = other.r;
    this->phi   = other.phi;
    this->theta = other.theta;
}
//_________________________________________________________________

Polar::Polar(const Vector3D& other) {
    this->r     = sqrt(other.x*other.x + other.y*other.y + other.z*other.z) ;
    this->phi   = atan2(other.y,other.x);
    this->theta = acos(other.z/this->r);
}
//_________________________________________________________________

Vector3D Polar::toCartesian() const {
    double x = this->r*sin(this->theta)*cos(this->phi);
    double y = this->r*sin(this->theta)*sin(this->phi);
    double z = this->r*cos(this->theta);
    Vector3D cartesian(x,y,z);

    return cartesian;
}
//_________________________________________________________________

void Polar::print() const {
    PRINTF("[r= %3.3f \t phi= %3.3f*PI \t theta= %3.3f*PI]\n",r, phi/M_PI, theta/M_PI);
}
//_________________________________________________________________


//=====================================================Implementation global methods============================================

uint64_t faculty(const uint32_t &x) {
    uint64_t faculty = 1;
    for (uint32_t i = 2; i <= x; i++)
        faculty *= i;
    return faculty;
}
//_________________________________________________________________

double FMod2p( const double &x) {
    double rval = fmod(x, 2*M_PI);
    if(rval < 0.0) rval+= 2*M_PI;
    return rval;
}

double fabs(const double &value) {
    double dval = value;
    if (dval < 0) dval = -dval;
    return dval;
}

Vector3D rotateX(const Vector3D& s,const double &angle) {
    Vector3D end;
    Matrix3D Rx;
    Rx.rotationX(angle);
    end = s.matVecMult(Rx);
    return end;
}
//_________________________________________________________________

Vector3D rotateY(const Vector3D& s,const double &angle) {
    Vector3D end;
    Matrix3D Ry;
    Ry.rotationY(angle);
    end = s.matVecMult(Ry);
    return end;
}
//_________________________________________________________________

Vector3D rotateZ(const Vector3D& s,const double &angle) {
    Vector3D end;
    Matrix3D Rz;
    Rz.rotationZ(angle);
    end = s.matVecMult(Rz);
    return end;
}

//_________________________________________________________________


//===Rn according to WGS84
// angle in rad
double R_n(const double angle) {
    double a,e2,f;
    double Rn;
    a = A_WGS84; //m
    f = F_WGS84;

    e2 =1-(1-f)*(1-f);

    double phi = angle;
    double sinp = sin(phi);

    double tmp = sqrt(1-e2*sinp*sinp);
    Rn = a/tmp;
    return Rn;

}
//_________________________________________________________________

double daysSinceY2k(int year,int month,int day,int hour,int minute,double second) {
    double days2k;
    double temp1,temp2;
    temp1 = -floor(0.25*7*(year+floor(1.0/12*(month+9))));
    temp2 = floor(275*month/9.0)+(1.0/24*(hour+minute/60.0+second/3600.0));
    days2k = 367* year +temp1 + temp2 + day - 730531.5;
    return days2k;
}

//===ECEF2ECI

double utcToJD(int year, int month, int date, int hour, int minute, double second){
	double JD;
	double d2k = daysSinceY2k(year,month,date,hour,minute,second);
	JD = d2k + JD2000;
	return JD;
}

/*
* Sidereal Time differs from solar time because of earth's precession
* GMST means Greenwhich Mean Sidereal Time defined as the hour angle of the vernal equinox
* GAST means Greenwhich Apparent Sidereal Time
* Both angles differ as the true vernal equinox' position is not uniformly but varies with nutation
*/

/*
* The functions jd2GMST and jd2GAST are borrowed
* from matlab versions
* http://de.mathworks.com/matlabcentral/fileexchange/28176-julian-date-to-greenwich-mean-sidereal-time
* http://de.mathworks.com/matlabcentral/fileexchange/28232-convert-julian-date-to-greenwich-apparent-sidereal-time
* Copyright (c) 2010, Darin Koblick
* All rights reserved.
*/

double jd2GMST(double jd){
	double gmst;
	double jdmin = floor(jd) - 0.5;
	double jdmax = floor(jd) + 0.5;
	double jd0 = jd;
	if(jd > jdmax) jd0 = jdmax;
	else jd0 = jdmin;
	double h = (jd - jd0) * 24;
    double d = jd - JD2000;
	double d0 = jd0 - JD2000;
	double t = d/36525;
	gmst = fmod(6.697374558 + 0.06570982441908*d0
					+ 1.00273790935*h + 0.000026*(t*t),24);
	gmst = grad2Rad(gmst * 15);
	return gmst;
}

double jd2GAST(double jd){
	double gast;
	double thetam = jd2GMST(jd);
	double t = (jd-JD2000)/36525;

	double epsilonm = grad2Rad(23.439291-0.0130111*t - 1.64E-07*(t*t) + 5.04E-07*(t*t*t));
	double l = grad2Rad(280.4665 + 36000.7698 * t);
	double dL = grad2Rad(218.3165 + 481267.8813*t);
	double omega = grad2Rad(125.04452 - 1934.136261*t);

	double dPSI = -17.20*sin(omega) - 1.32*sin(2*l) - 0.23*sin(2*dL)
			+ 0.21*sin(2*omega);
	double dEPSILON = 9.20*cos(omega) + 0.57*cos(2*l) + 0.10*cos(2*dL)
			-0.09*cos(2*omega);

	dPSI = grad2Rad(dPSI*(1./3600));
	dEPSILON = grad2Rad(dEPSILON*(1./3600));
	gast = fmod(thetam+dPSI*cos(epsilonm+dEPSILON),2*M_PI);
	return gast;
}

Matrix3D eciToECEFrotmat(double jd){
	double theta = jd2GAST(jd);
	Matrix3D rot;
    rot.rotationZ(theta);
	return rot;
}

//====ECEF (meter, meter, meter) to ECI (meter, meter, meter)
Vector3D ecefToECI(const Vector3D ecef, int64_t utcNanoseconds){
	double jd =  ((double)utcNanoseconds / (double)DAYS) + JD2000;
	Matrix3D rot = eciToECEFrotmat(jd);
	return  ecef.matVecMult(rot);
}

//====ECI (meter, meter, meter) to ECEF (meter, meter, meter)
Vector3D eciToECEF(const Vector3D eci, int64_t utcNanoseconds){
	double jd =  ((double)utcNanoseconds / (double)DAYS) + JD2000;
	Matrix3D rot = eciToECEFrotmat(jd);
	return eci.matVecMult(rot.transpose());
}


double eciToECEF(int64_t utcNanoseconds) {  //DEPRECATED

    double days2k = (double)utcNanoseconds / (double)DAYS;
    double angle  = (280.46061837+360.98564736628*days2k)/180*M_PI;
    angle         = FMod2p(angle);
    return angle;
}

//===Geodetic (height, longitude, latitude) to ECEF
Vector3D geodeticToECEF(const Polar& polar) {
    double x,y,z;
    double e2,f;
    f = F_WGS84;
    e2 = 1-(1-f)*(1-f);

    double h      = polar.r;
    double phi    = polar.phi;
    double lambda = polar.theta;
    double rn = R_n(phi);

    x = (rn+h) * cos(phi) * cos(lambda);
    y = (rn+h) * cos(phi) * sin(lambda);
    z = (rn*(1-e2)+h) * sin(phi);

    Vector3D ecef(x,y,z);
    return ecef;
}

//====ECEF to Geodetic===  latitude longitude height (phi,lambda,h)from x,y,z

Polar ecefToGeodetic(const Vector3D& other) {
    double lambda,phi,h;
    double a,b,e2,f;
    a = A_WGS84; //m
    f = F_WGS84;
    b = a*(1-f);
    e2 = 1-(b*b)/(a*a);

    double x,y,z;
    x = other.x;
    y = other.y;
    z = other.z;

    double p = sqrt(x*x+y*y);
    double phi0 = atan2(z,(p*(1-e2)));
    double n0 = R_n(phi0);
    double h0 = (p / cos(phi0)) - n0;

    double eht = 1e-5;
    double ephi = 1e-12;

    h = 0;
    phi = 0;
    double dh = 1;
    double dphi = 1;
    double n = 0;

    while((dh>eht) | (dphi > ephi)){
		phi = phi0;
		n = n0;
		h = h0;
		phi0 = atan2(z,p*(1-e2*(n/(n+h))));
		n0 = R_n(phi);
		h0 = p/cos(phi) - n;
		dh = fabs(h0-h);
		dphi = fabs(phi0-phi);
    }

    lambda = atan2(y,x);

    Polar llh(h, phi,lambda);

    return llh;
}


//_________________________________________________________________

//=====================================================Vector6D============================================

Vector6D::Vector6D() {

    for (int i=0; i<6; i++) {
        v[i]=0.0;
    }
}

Vector6D::Vector6D(const double* arr) {

    for (int i=0; i<6; i++) {
        v[i]=arr[i];
    }
}

Vector6D::Vector6D(double x_, double y_, double z_,
                   double u_, double v_, double w_) {

  v[0]=x_; v[1]=y_; v[2]=z_;
  v[3]=u_; v[4]=v_; v[5]=w_;
}

Vector6D::Vector6D( const Vector3D &upper, const Vector3D &lower ) {

    v[0] = upper.x;
    v[1] = upper.y;
    v[2] = upper.z;
    v[3] = lower.x;
    v[4] = lower.y;
    v[5] = lower.z;
}

double Vector6D::getLen() const {
    return sqrt(dotProduct(*this,*this));
}

Vector6D Vector6D::vecAdd(const Vector6D& other) const {

    Vector6D Aux;
    for (int i=0; i<6; i++) {
        Aux.v[i]=v[i]+other.v[i];
    }
    return Aux;
}

Vector6D Vector6D::vecSub(const Vector6D& other) const {

    Vector6D Aux;
    for (int i=0; i<6; i++) {
        Aux.v[i]=v[i]-other.v[i];
    }
    return Aux;
}

Vector6D Vector6D::matVecMult(const Matrix6D& M) const {

    Vector6D Aux;
    double Sum;
    for (int i=0; i<6; i++) {
        Sum = 0.0;
        for (int j=0; j<6; j++) {
            Sum += M.r[i][j] * v[j];
        }
        Aux.v[i] = Sum;
    }
    return Aux;
}

Vector6D Vector6D::scale(const double &factor) const {

    Vector6D Aux;
    for (int i=0; i<6; i++) {
        Aux.v[i]=factor*v[i];
    }
    return Aux;
}

double dotProduct(const Vector6D& left, const Vector6D& right) {

    double Sum = 0.0;
    for (int i=0; i<6; i++) {
        Sum+=left.v[i]*right.v[i];
    }
    return Sum;
}

bool Vector6D::equals(const Vector6D& other) const {
    for (int i = 0; i < 6; i++) {
        if (!isAlmost0(this->v[i] - other.v[i])) return false;
    }
    
    return true;
}

//=====================================================Matrix6D============================================

Matrix6D::Matrix6D() {

    // Initialization
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            if ( i == j ) {
                r[i][j]=1.0;
            }
            else {
                r[i][j]=0.0;
            }
        }
    }
}

Matrix6D::Matrix6D(const Matrix6D& other) {

    // Initialization
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            r[i][j]=other.r[i][j];
        }
    }
}

Matrix6D::Matrix6D(const double* arr) {

    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            r[i][j]=arr[i*6+j];
        }
    }
}

Matrix6D::Matrix6D(const Vector6D& diag) {

    // Initialization
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            if ( i == j ) {
                r[i][j] = diag.v[i];
            }
            else {
                r[i][j]=0.0;
            }
        }
    }
}

Matrix6D::Matrix6D(const Matrix3D &upperLeft, const Matrix3D &upperRight, const Matrix3D &lowerLeft, const Matrix3D &lowerRight) {

    for ( int i = 0; i < 3; i++ ) {
        for ( int j = 0; j < 3; j++ ) {
            r[  i][  j] =  upperLeft.r[i][j];
            r[  i][3+j] = upperRight.r[i][j];
            r[3+i][  j] =  lowerLeft.r[i][j];
            r[3+i][3+j] = lowerRight.r[i][j];
        }
    }
}

Matrix6D Matrix6D::transpose() const {

    Matrix6D transpose;
    for(int i = 0 ; i<6; i++) {
        for(int j = 0; j<6; j++) {
            transpose.r[i][j]=this->r[j][i];
        }
    }
    return transpose;
}

Matrix6D Matrix6D::invert() const {

    int j = 0, i = 0;
    double d = 0;

    Matrix6D inverse;
    Vector6D col;
    Vector6D indx;
    Matrix6D a = *this;

    /* LU- decomposition */
    if ( ludcmp(a, indx, d) ) {
        for (j = 0; j < 6; j++) {
            for (i = 0; i < 6; i++) {
                col.v[i] = 0;
            };
            col.v[j] = 1.0;
            /* backsubstitution */
            lubksb(a, indx, col);
            for (i = 0; i < 6; i++) {
                inverse.r[i][j] = col.v[i];
            };
        };
    }

    return inverse;
}

Matrix3D Matrix6D::upperLeft() const {

    Matrix3D P;

    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            P.r[i][j] = this->r[  i][  j];
        }
    }
    return P;
}

Matrix3D Matrix6D::upperRight() const {

    Matrix3D Q;

    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            Q.r[i][j] = this->r[  i][3+j];
        }
    }
    return Q;
}

Matrix3D Matrix6D::lowerLeft() const {
    Matrix3D R;

    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            R.r[i][j] = this->r[3+i][  j];
        }
    }
    return R;
}

Matrix3D Matrix6D::lowerRight() const {

    Matrix3D S;

    for(int i = 0 ; i<3; i++) {
        for(int j = 0; j<3; j++) {
            S.r[i][j] = this->r[3+i][3+j];
        }
    }
    return S;
}

Matrix6D Matrix6D::scale(const double &factor) const {

    Matrix6D Aux;
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            Aux.r[i][j]=factor*r[i][j];
        }
    }
    return Aux;
}

// Component access
Vector6D Matrix6D::getColumn(const int &j) const {

    Vector6D Res;
    for (int i=0; i<6; i++) {
        Res.v[i]=r[i][j];
    }
    return Res;
}

Vector6D Matrix6D::getRow(const int &i) const {

    Vector6D Res;
    for (int j=0; j<6; j++)  {
        Res.v[j]=r[i][j];
    }
    return Res;
}

Vector6D Matrix6D::diag() const {

    Vector6D Vec;
    for (int i=0; i<6; i++) {
        Vec.v[i] = r[i][i];
    }
    return Vec;
}

void Matrix6D::setColumn(const int &j, const Vector6D& column) {

    if ((j<0) || (6<=j)) {
        return;
    }
    for (int i=0; i<6; i++) {
        r[i][j] = column.v[i];
    }
}

void Matrix6D::setRow(const int &i, const Vector6D& row) {

    if ((i<0) || (6<=i)) {
        return;
    }
    for (int j=0; j<6; j++) {
        r[i][j] = row.v[j];
    }
}

// Matrix6D operators
Matrix6D operator/(const Matrix6D& Mat, double value) {

    Matrix6D Aux;
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            Aux.r[i][j]=Mat.r[i][j]/value;
        }
    }
    return Aux;
}

// Matrix6D addition and subtraction

Matrix6D Matrix6D::mAdd(const Matrix6D& other) const {

    Matrix6D Aux;
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            Aux.r[i][j] = r[i][j] + other.r[i][j];
        }
    }
    return Aux;
}


Matrix6D Matrix6D::mSub(const Matrix6D& other) const {

    Matrix6D Aux;
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            Aux.r[i][j] = r[i][j] - other.r[i][j];
        }
    }
    return Aux;
}

Matrix6D Matrix6D::mMult(const Matrix6D& other) const {

    Matrix6D Aux;
    double Sum;
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            Sum = 0.0;
            for (int k=0; k<6; k++) {
                Sum += r[i][k] * other.r[k][j];
            }
            Aux.r[i][j] = Sum;
        }
    }
    return Aux;
}

// Matrix and vector functions
Matrix6D dyadic(const Vector6D& left, const Vector6D& right) {

    Matrix6D Mat;
    for (int i=0; i<6; i++) {
        for (int j=0; j<6; j++) {
            Mat.r[i][j] = left.v[i]*right.v[j];
        }
    }
    return Mat;
}

bool Matrix6D::equals(const Matrix6D& other) const {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (!isAlmost0(this->r[i][j] - other.r[i][j])) return false;
        }
    }
    
    return true;
}

/*! \brief LU-decomposition of a matrix
 * \param a the matrix which is decomposed
 * \param indx vector that records the row permutation effected by partial pivoting
 * \param d +/-1 depending on whether the number of row interchanges was even or odd
 * \return LU-decomposed matrix
 *
 * Given the matrix a[0..n-1][0..n-1], this routine replaces it by the LU decomposition
 * of a rowwise permutation of itself. a is input. On Output, it is arranged as in Equation
 * (2.3.14) of Ref. [1]; indx [0..n-1] is an output vector that records the row permutation
 * effected by the partial pivoting; d is output as +/-1 depending on whether ther number of
 * row interchanges was even or odd, respectively. This routine is used in combination with
 * lubksb to solve linear equations or invert a matrix.
 *
 * Reference: [1] Press, W., Teukolsky, S., Vetterling, W., Flannery, B.: Numerical Recipes in C++, Second Edition,
 * Cambridge University Press, 1992
 */
bool ludcmp(Matrix6D &a, Vector6D &indx, double &d) {

    const double TINY = 1.0e-20; // a small number
    const int n = 6;
    int i, imax=0, j, k;
    double big, sum, dum, temp;

    Vector6D vv; // vv stores the implicit scaling of each row
    d = 1.0; // no row interchanges yet
    // loop over rows to get the implicit scaling information
    for (i = 0; i < n; i++) {
        big = 0.0;
        for (j = 0; j < n; j++) {
            if ( (temp = fabs(a.r[i][j])) > big ) {
                big = temp;
            }
        }; // end for j
        if ( big == 0.0 ) {
            // error: matrix is singular
            return false;
        };
        vv.v[i] = 1.0/big; // save the scaling

    };	//end for i
    // this is the loop over columns of Crout's method
    for (j = 0; j < n; j++) {
        for(i = 0; i < j; i++) {
            // this is equation (2.3.13) of Ref. [1] except i == j
            sum = a.r[i][j];
            for (k = 0; k < i; k++) {
                sum -= a.r[i][k]*a.r[k][j];
            }; // end for k
            a.r[i][j] = sum;
        }; // end for i
        big = 0.0; // initialize for the search for largest pivot element
        // this is i ==j of equation (2.3.13) an i == j+1..N-1 of equation (2.3.13)
        for (i = j; i < n; i++) {
            sum = a.r[i][j];
            for (k = 0; k < j; k++) {
                sum -= a.r[i][k]*a.r[k][j];
            }
            a.r[i][j] = sum;
            if ( (dum = vv.v[i]*fabs(sum)) >= big ) {
                // is the figure of merit for the pivot better than the best so far?
                big = dum;
                imax = i;
            };
        }; // end for i
        if (j != imax) {
            // do we need to interchange rows?
            for (k = 0; k < n; k++) {
                dum = a.r[imax][k];
                a.r[imax][k] = a.r[j][k];
                a.r[j][k] = dum;
            } // end for k
            d = -d; // change the parity of d
            vv.v[imax] = vv.v[j]; // interchange the scale factor
        }; // end if
        indx.v[j] = static_cast<double> (imax);
        if ( a.r[j][j] == 0.0 ) {
            // if the pivot element is zero the matrix is singular (at least to the
            // precision of the algorithm). For some applications on singular matrices,
            // it is desirable to substitute TINY for zero
            a.r[j][j] = TINY;
        };
        if ( j != n-1 ) {
            // devide the pivot element
            dum = 1.0/a.r[j][j];
            for (i = j+1; i < n; i++) {
                a.r[i][j] *= dum;
            };
        };
    }; // end for j
    return true;
}

/*! \brief solves a set of linear equations A*X = B with a LU decomposition of the matrix a
 * \param a the LU decomposition of the matrix A
 * \param indx permutation vector
 * \param b right-hand side vector B for
 * \return solution X for the set of linear equations
 *
 * Solves the set of n linear equations A*X = B. Here a[0..n-1][0..n-1] is input, not as the matrix a
 * but rather as its LU decomposition, determined ny tje routine ludcmp. b[0..n-1] is the
 * input as the right-hand side vector B, and returns with the solution vector X. a and indx are
 * not modified by this routine and can ne left in place for successive calls with different right-hand sides b.
 * This routine takes into account the possibility that b will begin with many zero elements, so it is efficient for
 * use in matrix inversion
 *
 * Reference: [1] Press, W., Teukolsky, S., Vetterling, W., Flannery, B.: Numerical Recipes in C++, Second Edition,
 * Cambridge University Press, 1992
 */
void lubksb(Matrix6D &a, Vector6D &indx, Vector6D &b) {

    const int n = 6;
    int i, ii=0, ip,j;

    double sum;

    for (i = 0; i < n; i++) {
        // when ii is set to a positive value, it will become the index of the first
        // nonvanishing element of b. We now do the forward substitution, equation (2.3.6)
        // of Ref [1] The only new wrinkle is to unscramble ther permutation as we go
        ip = static_cast<int> ( indx.v[i] );
        sum = b.v[ip];
        b.v[ip] = b.v[i];
        if ( ii != 0) {
            for (j = ii-1; j < i; j++) {
                sum -= a.r[i][j]*b.v[j];
            };
        } // end if
        else {
            if ( sum != 0 ) {
                // a nonzero element was encointered, so from now on we will have to
                // do the sums in the loop above
                ii = i+1;
            };
        }; // end else
        b.v[i] = sum;
    }; // end for i
    for (i = n-1; i >= 0; i--) {
        // now we do the backsubstitution, equation (2.3.7) of Ref [1]
        sum = b.v[i];
        for (j = i+1; j < n; j++) {
            sum -= a.r[i][j]*b.v[j];
        };
        // store a component of the solution vector X
        b.v[i] = sum/a.r[i][i];
    };
}


//____________________________________________________________________

/* findRotationsAngleAxis
 *  When we use only one vector to rate a body, we get an ambiguous rotation
 *  which has one degree of freedom open.
 *  To get an unambiguous rotation we have to use two not co-linear vectors of the
 *  body. This functions finds the unit rotation which satisfied the rotation
 *  of these both vectors.
 *  But! The body may not be formatted. The geometric relationship of the
 *  two vectors has to be the same before and after the rotation.
 *   compare to "cosinus direction matrix"
 */

Result<AngleAxis>  findRotationsAngleAxis(Vector3D fromA, Vector3D toA, Vector3D fromB, Vector3D toB) {

    fromA = fromA.normalize();
    toA   = toA.normalize();
    fromB = fromB.normalize();
    toB   = toB.normalize();

    //_______________________________________________ Find rotation axis
    // one axis for both transformations, A and B

    Vector3D rotA = toA - fromA;
    Vector3D rotB = toB - fromB;
    Vector3D rotAxis = crossProduct(rotA, rotB);
    rotAxis = rotAxis.normalize();;

    //______________________________________________________ find rotation angle
    // find angle, first find projection on rotation surface
    // we find the projection of both vectors to a surface orthogonal to
    // the rotation axis. Then we see there the angle of rotation.

    Vector3D projectionFromA = fromA - (dotProduct(fromA, rotAxis) * rotAxis);
    Vector3D projectionToA   = toA   - (dotProduct(toA  , rotAxis) * rotAxis);
    Vector3D projectionFromB = fromB - (dotProduct(fromB, rotAxis) * rotAxis);
    Vector3D projectionToB   = toB   - (dotProduct(toB  , rotAxis) * rotAxis);

    projectionFromA = projectionFromA.normalize();
    projectionToA   = projectionToA.normalize();
    projectionFromB = projectionFromB.normalize();
    projectionToB   = projectionToB.normalize();


    double cosAngleA = dotProduct(projectionFromA, projectionToA);
    double cosAngleB = dotProduct(projectionFromB, projectionToB);
    if(!isAlmost0(cosAngleA - cosAngleB)) return ErrorCode::VECTOR_LEN;
    
    AngleAxis rotor(acos(cosAngleA), rotAxis);

    /** BUT!!! Some times (50%) the rotation axis is inverted. Check and correct ***/
    Vector3D newA = fromA.aRotate(rotor);
    if(!newA.equals(toA))  rotor = AngleAxis(acos(cosAngleA), rotAxis*(-1.0));
    newA = fromA.aRotate(rotor);
    if(!newA.equals(toA))  return { ErrorCode::COLINEAR_VECTORS, rotor } ;

    return rotor;
}


// Intern function, DO NOT use directly, use only  direction_cos_matrix_from_vectors(...)
Matrix3D _DCM_generator(Vector3D fromA, Vector3D toA, Vector3D fromB, Vector3D toB) {

    fromA = fromA.normalize();
    toA   = toA.normalize();
    fromB = fromB.normalize();
    toB   = toB.normalize();

    //Help vector to define a Cartesian coordinates system
    Vector3D fromX = fromA;
    Vector3D fromZ = fromA.cross(fromB);
    Vector3D fromY = fromZ.cross(fromX);

    Vector3D toX = toA;
    Vector3D toZ = toA.cross(toB);
    Vector3D toY = toZ.cross(toX);

    // normalize
    fromX = fromX.normalize();
    fromY = fromY.normalize();
    fromZ = fromZ.normalize();

    toX = toX.normalize();
    toY = toY.normalize();
    toZ = toZ.normalize();

    Matrix3D result = Matrix3D();

    result.r[0][0] = fromX.dot(toX);
    result.r[0][1] = fromX.dot(toY);
    result.r[0][2] = fromX.dot(toZ);

    result.r[1][0] = fromY.dot(toX);
    result.r[1][1] = fromY.dot(toY);
    result.r[1][2] = fromY.dot(toZ);

    result.r[2][0] = fromZ.dot(toX);
    result.r[2][1] = fromZ.dot(toY);
    result.r[2][2] = fromZ.dot(toZ);

    return result;
}

/* cos_direction_matrix_from_vectors
 *  when we use only one vector to rate a body, we get an ambiguous rotation
 *  which has one degree of freedom open.
 *  To get an unambiguous rotation we have to use two not co-linear vectors of the
 *  body. This functions finds the unit rotation which satisfied the rotation
 *  of these both vectors.
 *  But! The body may not be formatted. The geometric relationship of the
 *  two vectors has to be the same before and after the rotation.
 */


// WARNING!!! First we have to rotate to (1,0,0) and (0,1,0) and then continue else it does not work, WHY?!?!?!
Matrix3D direction_cos_matrix_from_vectors(Vector3D fromA, Vector3D toA, Vector3D fromB, Vector3D toB) {
    Vector3D x(1,0,0);
    Vector3D y(0,1,0);

    // first rotate to unit vectors x and y and then to final vector.
    // Else it does not work and we do not know why :(

    Matrix3D m1 = _DCM_generator(fromA, x, fromB, y);
    Matrix3D m2 = _DCM_generator(x, toA, y, toB);

    return m2*m1;
}


}
