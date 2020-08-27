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

    //function
    AxShape get_piece(AxShape _shape);
    AxShape get_brut(AxShape _shape);
    AxShape fix(AxShape _shape);
        void tour_ebauche(AxShape _shape, int _passe);  //rempli gcode avec coordonnées
                    //bien que composée du même code que tour_ebauche_shape, impossible de récuperer correctement les coordonées
        AxShape tour_ebauche_shape(AxShape _shape, int _passe); //renvoie une shape composée d'AxLine des passages de l'outil
        //ne fonctionne pas à cause des int dans les fonctions intersect qui empeche de repérer certaines colisions

    //contructor
    void generate(AxShape shape,  int passe=1, int ep1=0, int ep2=0);
    AxGCode(AxShape shape,  int passe=1, int ep1=0, int ep2=0);
    AxGCode();


    // operator overloading
    friend QDebug operator<<(QDebug dbg, AxGCode _axgcode);
    AxCode operator[](int i) const;
private:

    QVector<AxCode> gcode;  //axe x=-Z axe y=x

};

#endif // AXGCODE_H
