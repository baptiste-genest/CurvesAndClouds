#ifndef SMOOTH_INTERPOLATOR_H
#define SMOOTH_INTERPOLATOR_H


class smooth_interpolator
{
public:
    smooth_interpolator();

    int getYo() const;
    void setYo(int value);

private:

    int yo;
};

#endif // SMOOTH_INTERPOLATOR_H
