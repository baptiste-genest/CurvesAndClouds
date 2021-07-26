#include "rotations.h"

cnc::mat cnc::algo::geometry::Rx(scalar th){
    scalar c = cos(th),s = sin(th);
    return mat(3,3,{
                   1,0,0,
                   0,c,-s,
                   0,s,c
               });
}
cnc::mat cnc::algo::geometry::Ry(scalar th){
    scalar c = cos(th),s = sin(th);
    return mat(3,3,{
                   c,0,s,
                   0,1,0,
                   -s,0,c
               });
}
cnc::mat cnc::algo::geometry::Rz(scalar th){
    scalar c = cos(th),s = sin(th);
    return mat(3,3,{
                   c,-s,0,
                   s,c,0,
                   0,0,1
               });
}

