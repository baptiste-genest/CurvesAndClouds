#ifndef QUATERNION_H
#define QUATERNION_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include "algo/linear_algebra/lin_alg.h"
#include "../cnc_error.h"

namespace cnc {

class Quaternion {
public:
    Quaternion() {
        values = vec(4);
    }
    Quaternion(scalar);
    Quaternion(scalar,scalar,scalar,scalar);
    Quaternion(scalar,const vec&);
    Quaternion(const vec&);

    inline Quaternion operator+(const Quaternion& o) const{
        return values + o.values;
    }
    inline Quaternion operator*(scalar x) const{
        return values*x;
    }

    Quaternion operator*(const Quaternion& o) const;

    inline Quaternion invert() const {
        scalar n = norm2();
        if (n < 1e-20)
            throw Cnc_error("Can't invert nul Quaternion");
        return conj()*(1./norm2());
    }

    Quaternion conj() const;

    inline scalar norm() const{
        return values.norm();
    }
    inline scalar norm2() const{
        return values.norm2();
    }

    inline scalar re() const {
        return values(0);
    }
    inline scalar& re() {
        return values(0);
    }
    inline scalar i() const {
        return values(1);
    }
    inline scalar& i() {
        return values(1);
    }
    inline scalar j() const {
        return values(2);
    }
    inline scalar& j() {
        return values(2);
    }
    inline scalar k() const {
        return values(3);
    }
    inline scalar& k() {
        return values(3);
    }

    friend std::ostream& operator<<(std::ostream& o,const Quaternion& q ){
        constexpr static char vars[3] = {'i','j','k'};
        for (uint k = 0;k<4;k++){
            if (std::abs(q.values(k)) > 1e-20){
                if (q.values(k) > 0 && k)
                    o << '+';
                o << q.values(k);
                if (k)
                    o << vars[k-1];
                o << " ";
            }
        }
        return o;
    }

    inline friend Quaternion operator/(const Quaternion& a,const Quaternion& b ){
        return a.invert()*b;
    }

    inline Quaternion operator-() const{
        return -values;
    }

private:
    vec values;
};

}


#endif // QUATERNION_H
