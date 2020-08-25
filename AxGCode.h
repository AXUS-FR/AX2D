#ifndef AXGCODE_H
#define AXGCODE_H
#include "AxCode.h"
#include <QVector>
#include "AxShape.h"
class AxGCode
{
public:
    //getters
    QVector<AxCode> get_gcode() const;

    //setters
    void set_gcode(QVector<AxCode> _gcode);
    int size();
    void append(AxCode _code);
    //contructor
    void generate(AxShape shape, int ep1=0, int ep2=0);
    AxGCode(AxShape shape, int ep1=0, int ep2=0);
    AxGCode();


    // operator overloading
    friend QDebug operator<<(QDebug dbg, AxGCode _axgcode);
    AxCode operator[](int i) const;
private:

    QVector<AxCode> gcode;

};

#endif // AXGCODE_H
