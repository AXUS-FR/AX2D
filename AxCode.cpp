#include "AxCode.h"

//getters
AxPath AxCode::get_path()
{
    return path;
};
int AxCode::get_label()
{
    return label;
};

//setters
void AxCode::set_path(AxPath _path)
{
    path = _path;
};
void AxCode::set_label(int _label)
{
    label = _label;
};


//constructor
AxCode::AxCode(AxPath _path, int _label)
{
    path = _path;
    label = _label;

};

// operator overloading
QDebug operator<<(QDebug dbg, AxCode _axcode)
{
    dbg.nospace() << _axcode.get_path();
    dbg.nospace() << "Label : " << _axcode.get_label();
    return dbg.maybeSpace();
};
