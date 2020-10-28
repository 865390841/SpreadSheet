#include<QtGui>
#include"cell.h"
Cell::Cell()
{
    setDirty();
}

QTableWidgetItem *Cell::clone() const
{
    return new Cell(*this);
}

void Cell::setData(int role, const QVariant &value)
{
    QTableWidgetItem::setData(role,value);
    if(role==Qt::EditRole){
        setDirty();
    }
}

QVariant Cell::data(int role) const
{
    if(role==Qt::DisplayRole){
        if(value().isValid()){
            return value().toString();
        }
        else{
            return "###";
        }
    }
    else if(role==Qt::TextAlignmentRole){
        if(value().type()==QVariant::String){
            return int(Qt::AlignLeft|Qt::AlignVCenter);
        }
        else{
            return int(Qt::AlignRight|Qt::AlignVCenter);
        }
    }
    else{
        return QTableWidgetItem::data(role);
    }
}

void Cell::setFormula(const QString &formula)
{
    setData(Qt::EditRole,formula);
}

QString Cell::formula() const
{
    return data(Qt::EditRole).toString();
}

void Cell::setDirty()
{
    cacheIsDirty=true;
}
const QVariant Invalid;
QVariant Cell::value() const
{
    if(cacheIsDirty){
        cacheIsDirty=false;

        QString formulaStr=formula();
        if(formulaStr.startsWith('\'')){
            cachedValue=formulaStr.mid(1);
        }
        else if(formulaStr.startsWith('=')){
            cachedValue=Invalid;
            QString expr=formulaStr.mid(1);
            expr.replace(" ","");
            expr.append(QChar::Null);

            int pos=0;
            cachedValue=evalExpression(expr,pos);
            if(expr[pos]!=QChar::Null){
                cachedValue=Invalid;
            }
        }
        else{
            bool ok;
            double d=formulaStr.toDouble(&ok);
            if(ok){
                cachedValue=d;
            }
            else{
                cachedValue=formulaStr;
            }
        }
    }
    return cachedValue;
}

QVariant Cell::evalExpression(const QString &str, int &pos) const
{
    return QVariant();
}

QVariant Cell::evalTerm(const QString &str, int &pos) const
{
    return QVariant();
}

QVariant Cell::evalFactor(const QString &str, int &pos) const
{
    return QVariant();
}
