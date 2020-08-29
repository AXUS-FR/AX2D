#include "AxGCode.h"
#include "common_functions.h"
#include "AxCode.h"
#include "AxPath.h"
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

//function

AxShape AxGCode:: get_piece(AxShape _shape) //get the piece elements in shape
{
    AxShape ret;
    for (int i=0;i<_shape.size();++i) {
        if(_shape[i]->get_kind()==0)
        {
            ret.append(_shape[i]);
        }
    }
    return ret;
};
AxShape AxGCode:: get_brut(AxShape _shape)  //get the brut elements in shape
{
    AxShape ret;       //pour le brut
    for (int i=0;i<_shape.size();++i) {
        if(_shape[i]->get_kind()==1)
        {
            ret.append(_shape[i]);
        }
    }
    return ret;
};
AxShape AxGCode:: fix(AxShape _shape)       //fix unmakable geometries
{
    int y_max=_shape[0]->p1().y();      //en regardant si les y sont croissant
    for (int i=0;i<_shape.size();++i) { //repère les lignes problèmatiques

        if(_shape[i]->p2().y()<y_max)
        {
            _shape[i]->set_kind(-1);    //defaults are set as construction
        }else
        {
            y_max=_shape[i]->p2().y();     //met à jour le plus grand y
        }
    }
    QPoint p1=_shape[0]->p2();  //point de ref
    QPoint p2;                  //point à comparer avec p1
    QPoint inter;               //point d'intersection
    int der=0;                  //dernière position d'une ligne de pièce
    int size=_shape.size()-1;
    for (int i=1;i<size;++i) { //réparration des éventuelles trous
        if(_shape[i]->get_kind()==-1)   //skip les traits de construction
        {
            continue;
        }
        p2=_shape[i]->p1();
        if(p1!=p2)
        {
            if(p1.x()<=p2.x()) //si le x du premier point est plus bas que celui du deuxième
            {
                _shape.append(new AxLine(p1,p2));    //les segements ne sont plus dans l'ordre
                i++;
            }else
            {
                inter.setY(intersect_lignes(AxLine(_shape[der]->p2().x(),_shape[der]->p2().y(),_shape[der]->p2().x()-10,_shape[der]->p2().y()),_shape[i])[1][1]);
                inter.setX(intersect_lignes(AxLine(_shape[der]->p2().x(),_shape[der]->p2().y(),_shape[der]->p2().x()-10,_shape[der]->p2().y()),_shape[i])[1][0]);
                _shape.append(new AxLine(p1,inter));    //les segements ne sont plu dans l'ordre
                _shape[i]->setP1(inter);
            }
        }
        p1=_shape[i]->p2();
        der=i;
    }
    if(_shape[size]->get_kind()==-1)//si les derniers elements sont en constructions
    {
        _shape.append(new AxLine(p1,QPoint(_shape[size]->p2().x(),p1.y())));
    }
    return _shape;
};
void AxGCode:: tour_ebauche(AxShape _shape, int _passe)    //replie AxGCode avec les coordonnées de passages
{   //ne fonctionne pas -> arrive pas à récuperer les coorconnées
    AxPath path;
    QVector<AxCode> ret;
    AxShape brut;
    brut=get_brut(_shape);
    int brut_max_x=0;
    int brut_max_z=0;
    int z_min=brut[0]->p1().x();
    int x_min=brut[0]->p1().y();
    for (int i=0;i<brut.size();++i) {
        if(brut[i]->p1().y()>brut_max_x)
        {
            brut_max_x=brut[i]->p1().y();
        }
        if(brut[i]->p1().x()>brut_max_z)
        {
            brut_max_z=brut[i]->p1().x();
        }
        if(brut[i]->p1().x()<z_min)
        {
            z_min=brut[i]->p1().x();
        }
        if(brut[i]->p1().y()<x_min)
        {
            x_min=brut[i]->p1().y();
        }
    }
    int pos_x=_passe;
    AxLine* line;
    QVector<QVector<int>> cross;
    int nbr_cross=0;
    while (brut_max_x-pos_x>x_min) {
        line=new AxLine(QPoint(brut_max_z+10,brut_max_x-pos_x),QPoint(z_min-10,brut_max_x-pos_x),0);
        nbr_cross=0;
        for (int i=0;i<_shape.size();++i) {
            if(_shape[i]->get_kind()==-1)   //si l'element testé est contruction
            {
                continue;
            }
            if(nbr_cross==0)    //calcul des pts les plus à gauches
            {
                if(line->p2().y()==_shape[i]->p1().y())
                {
                    line->setP2(_shape[i]->p1());
                    continue;
                }
                cross=intersect(*line,static_cast<AxLine>(_shape[i]));//(AxLine) _shape[i]);
                if(cross[0][0]==1)
                {
                    line->setP2(QPoint(cross[1][0],cross[1][1]));
                    nbr_cross++;
                    continue;
                }
            }
            if(nbr_cross==1)    //calcul les points les plus à droites
            {
                if(line->p1().y()==_shape[i]->p2().y())
                {
                    line->setP1(_shape[i]->p2());
                    break;
                }
                cross=intersect(*line,static_cast<AxLine>(_shape[i]));
                if(cross[0][0]==1)
                {
                    line->setP1(QPoint(cross[1][0],cross[1][1]));
                    break;
                }
            }
        }
        path.set_x(line->p1().y());
        path.set_z(line->p1().x());
        ret.append(AxCode(path, 0));

        path.set_x(line->p2().y());
        path.set_z(line->p2().x());
        ret.append(AxCode(path, 0));
        pos_x=pos_x+_passe;
    }

    if(brut_max_x-pos_x+_passe!=x_min)
    {
        line=new AxLine(QPoint(brut_max_z,x_min),QPoint(_shape[0]->p1().x(),x_min),0);
        path.set_x(line->p1().y());
        path.set_z(line->p1().x());
        ret.append(AxCode(path, 0));
        path.set_x(line->p2().y());
        path.set_z(line->p2().x());
        ret.append(AxCode(path, 3));
    }
    this->set_gcode(ret);
};
AxShape AxGCode::tour_ebauche_shape(AxShape _shape, int _passe)
{
    AxShape ret;
    AxShape brut;
    brut=get_brut(_shape);
    int brut_max_x=0;
    int brut_max_z=0;
    int z_min=brut[0]->p1().x();
    int x_min=brut[0]->p1().y();
    for (int i=0;i<brut.size();++i) {
        if(brut[i]->p1().y()>brut_max_x)
        {
            brut_max_x=brut[i]->p1().y();
        }
        if(brut[i]->p1().x()>brut_max_z)
        {
            brut_max_z=brut[i]->p1().x();
        }
        if(brut[i]->p1().x()<z_min)
        {
            z_min=brut[i]->p1().x();
        }
        if(brut[i]->p1().y()<x_min)
        {
            x_min=brut[i]->p1().y();
        }
    }
    int pos_x=_passe;
    AxLine* line;
    QVector<QVector<int>> cross;
    int nbr_cross=0;
    while (brut_max_x-pos_x>x_min) {
        line=new AxLine(QPoint(brut_max_z+10,brut_max_x-pos_x),QPoint(z_min-10,brut_max_x-pos_x),0);
        nbr_cross=0;
        for (int i=0;i<_shape.size();++i) {
            if(_shape[i]->get_kind()==-1)   //si l'element testé est contruction
            {
                continue;
            }
            if(nbr_cross==0)    //calcul des pts les plus à gauches
            {

                if(line->p2().y()==_shape[i]->p1().y())
                {
                    line->setP2(_shape[i]->p1());
                    continue;
                }
                cross=intersect(*line,static_cast<AxLine>(_shape[i]));
                if(cross[0][0]==1)
                {
                    line->setP2(QPoint(cross[1][0],cross[1][1]));
                    nbr_cross++;
                    continue;
                }

            }
            if(nbr_cross==1)    //calcul les points les plus à droites
            {
                if(line->p1().y()==_shape[i]->p2().y())
                {
                    line->setP1(_shape[i]->p2());
                    break;
                }
                cross=intersect(*line,static_cast<AxLine>(_shape[i]));
                if(cross[0][0]==1)
                {
                    line->setP1(QPoint(cross[1][0],cross[1][1]));
                    break;
                }
            }
        }
        ret.append(line);
        pos_x=pos_x+_passe;
    }
    if(brut_max_x-pos_x+_passe!=x_min)
    {
        line=new AxLine(QPoint(brut_max_z,x_min),QPoint(_shape[0]->p1().x(),x_min),0);
        ret.append(line);
    }
    return ret;
};


//contructor
void AxGCode::generate(AxShape shape,  int passe, int ep1, int ep2)
{
    gcode = QVector<AxCode> ();
    //################################Etape0 : séparation de la pièce et du brut
    AxShape shape2 = this->get_piece(shape);
    AxShape brut = this->get_brut(shape);
    //###############################Etape1 : conditionne la pièce
    shape2=this->fix(shape2);
    //############################Etape 2 : expendre de ep1
    //shape2.expend(ep1);
    //#####################Etape3 : remet le brut sur la pièce
    for (int i=0;i<brut.size();++i) {   //mise du brut autour de la piece
        shape2.append(brut[i]);
    }
    //#################Etape 4 : fill AxGCode
    this->tour_ebauche(shape2, passe);
};
AxGCode::AxGCode(AxShape shape, int passe,int ep1, int ep2)
{
    this->generate(shape, passe, ep1, ep2);
};
AxGCode::AxGCode()
{
    gcode = QVector<AxCode>();
};
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

