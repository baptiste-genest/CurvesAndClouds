#include "quaternion.h"

cnc::Quaternion::Quaternion(cnc::scalar a):
    values(4)
{
    values(0) = a;
}

cnc::Quaternion::Quaternion(cnc::scalar a, cnc::scalar b, cnc::scalar c, cnc::scalar d) : values(4)
{
    re() = a;
    i() = b;
    j() = c;
    k() = d;
}

cnc::Quaternion::Quaternion(cnc::scalar a, const cnc::vec & v) : values(4)
{
    if (v.rowNum() != 3)
        throw Cnc_error("Can't build Quaternion as a + v if size(v) != 3");
    re() = a;
    for (uint k = 0;k<3;k++)
        values(k+1) = v(k);
}

cnc::Quaternion::Quaternion(const cnc::vec & v) : values(v)
{
}

cnc::Quaternion cnc::Quaternion::conj() const
{
    vec v = values;
    for (uint k = 1;k<4;k++)
        v(k) *= -1;
    return v;
}

cnc::Quaternion cnc::Quaternion::operator*(const Quaternion& o) const{
    typedef const scalar& rsc;
    rsc a = re(),
        b = i(),
        c = j(),
        d = k();
    rsc e =o.re(),
        f =o.i(),
        g =o.j(),
        h =o.k();
    return Quaternion(a*e - b*f - c*g - d*h,
                      a*f + b*e + c*h - d*g,
                      a*g + c*e + d*f - b*h,
                      a*h + d*e + b*g - c*f);
}
