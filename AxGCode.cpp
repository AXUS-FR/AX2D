#include "AxGCode.h"

//getters
QVector<AxCode> AxGCode::get_gcode() const
{
    return gcode;
};

//setters
void AxGCode::set_gcode(QVector<AxCode> _gcode)
{
    gcode = _gcode;
};
int AxGCode::size()
{
    return gcode.size();
};
void AxGCode::append(AxCode _code)
{
    gcode.append(_code);
};
//contructor
void AxGCode::generate(AxShape shape, int ep1, int ep2)
{

};
AxGCode::AxGCode(AxShape shape, int ep1, int ep2)
{
    generate(shape, ep1, ep2);
};
AxGCode::AxGCode()
{};
// operator overloading
AxCode AxGCode::operator[](int i) const
{
    return gcode[i];
}
QDebug operator<<(QDebug dbg, AxGCode _axgcode)
{
    for(int i=0;i<_axgcode.size(); ++i)
    {
        dbg.nospace() <<"Coodonates of point n°"<<i<<endl;
        dbg.nospace() <<_axgcode[i]<<endl;
    }
    return dbg.maybeSpace();
}

