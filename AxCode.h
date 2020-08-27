#ifndef AXCODE_H
#define AXCODE_H
#include "AxPath.h"
#include "AxLine.h"
#include <QStyle>

#include <QObject>
#include <AxArc.h>
#include <iostream>
using namespace std;

class AxCode
{
public:
    //getters
    AxPath get_path();
    int get_label();

    //setters
    void set_path(AxPath _path);
    void set_label(int _label);

    //constructor
    AxCode(AxPath _path, int _label=0);


    // operator overloading
    friend QDebug operator<<(QDebug dbg, AxCode _axcode);


private:    //axe x=-Z axe y=x
    AxPath path;    //coordinates of the point on x,y,z,A,B,C,U,W,I,K
    int label;      //type of movement to get to this point (fast, slow, clokwise, no-clokwise,....)




};

#endif // AXCODE_H
