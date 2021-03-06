#include"spreadsheet.h"
#include"cell.h"
#include<QtGui>
#include<QMessageBox>
#include<QApplication>
#include<QStringList>
Spreadsheet::Spreadsheet(QWidget *parent)
    :QTableWidget(parent)
{
    autoRecalc=true;
    setItemPrototype(new Cell); //设置表的项原型为自定义的QTableWidgetItem子类
    setSelectionMode(ContiguousSelection);

    connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(somethingChanged()));

    clear();
}

QString Spreadsheet::currentLocation() const
{
    return QChar('A'+currentColumn())+QString::number(currentRow()+1);
}

QString Spreadsheet::curruntFormula() const
{
    return formula(currentRow(),currentColumn());
}

QTableWidgetSelectionRange Spreadsheet::selectedRange() const
{
    QList<QTableWidgetSelectionRange> ranges=selectedRanges();
    if(ranges.isEmpty()){
        return QTableWidgetSelectionRange();
    }
    return ranges.first();
}

void Spreadsheet::clear()
{
    setRowCount(0);
    setColumnCount(0);
    setRowCount(RowCount);
    setColumnCount(ColumnCount);

    for(int i=0;i<ColumnCount;i++){
        QTableWidgetItem *item=new QTableWidgetItem;
        item->setText(QString(QChar('A'+i)));
        setHorizontalHeaderItem(i,item);
    }
    setCurrentCell(0,0);
}

bool Spreadsheet::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Spreadshrrt"),tr("Cannot read file %1:\n%2.").arg((file.fileName()).arg(file.errorString())));
        return false;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_7);

    quint32 magic;
    in>>magic;
    if(magic!=MagicNumber){
        QMessageBox::warning(this,tr("Spreadsheet"),tr("this file is not a spreadsheet file."));
        return false;
    }
    clear();

    quint16 row;
    quint16 col;
    QString str;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd()){
        in>>row>>col>>str;
        setFormula(row,col,str);
    }
    QApplication::restoreOverrideCursor();
    return true;
}

bool Spreadsheet::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Spreadshrrt"),tr("Cannot write file %1:\n%2.").arg((file.fileName()).arg(file.errorString())));
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_7);

    out<<quint32(MagicNumber);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int row=0;row<RowCount;row++){
        for(int col=0;col<ColumnCount;col++){
            QString str=formula(row,col);
            if(!str.isEmpty()){
                out<<quint16(row)<<quint16(col)<<str;
            }
        }
    }
    QApplication::restoreOverrideCursor();
    return true;
}

void Spreadsheet::sort(const SpreadsheetCompare &compare)
{

}

void Spreadsheet::cut()
{
    copy();
    del();
}

void Spreadsheet::copy()
{
    QTableWidgetSelectionRange range=selectedRange();
    QString str;
    for(int i=0;i<range.rowCount();i++){
        if(i>0){
            str+="\n";
        }
        for(int j=0;j<range.columnCount();j++){
            if(j>0){
                str+="\t";
            }
            str+=formula(range.topRow()+i,range.leftColumn()+j);
        }
    }
    QApplication::clipboard()->setText(str);
}

void Spreadsheet::paste()
{
    QTableWidgetSelectionRange range=selectedRange();
    QString str=QApplication::clipboard()->text();
    QStringList rows=str.split('\n');
    int numRows=rows.count();
    int numColumns=rows.first().count('\t')+1;

    if(range.rowCount()*range.columnCount()!=1&&(range.rowCount()!=numRows||range.columnCount()!=numColumns)){
        QMessageBox::information(this,tr("Spreadsheet"),tr("The information cannot be pasted because the copy adn paste areas aren't the same size."));
        return;
    }
    for(int i=0;i<numRows;i++){
        QStringList cols=rows.at(i).split('\t');
        for(int j=0;j<numColumns;j++){
            int row=range.topRow()+i;
            int column=range.leftColumn()+j;
            if((row<(int)RowCount)&&(column<(int)ColumnCount)){
                setFormula(row,column,cols[j]);
            }
        }
    }
    somethingChanged();
    qDebug()<<"paste.";
}

void Spreadsheet::del()
{
    QList<QTableWidgetItem *> items = selectedItems();
    if(!items.isEmpty()){
        for(auto *item:items){
            delete item;
        }
        somethingChanged();
    }
}

void Spreadsheet::selectCurrentRow()
{
    selectRow(currentRow());
}

void Spreadsheet::selectCurrentColumn()
{
    selectColumn(currentColumn());
}

void Spreadsheet::recalculate()
{

}

void Spreadsheet::setAutoRecalculate(bool recal)
{

}

void Spreadsheet::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    int row=currentRow();
    int col=currentColumn()+1;

    while(row<RowCount){
        while(col<ColumnCount){
            if(text(row,col).contains(str,cs)){
                clearSelection();
                setCurrentCell(row,col);
                activateWindow();
                return;
            }
            ++col;
        }
        col=0;
        ++row;
    }
    QApplication::beep();
}

void Spreadsheet::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    int row=currentRow();
    int col=currentColumn()-1;

    while(row>=0){
        while(col>=0){
            if(text(row,col).contains(str,cs)){
                clearSelection();
                setCurrentCell(row,col);
                activateWindow();
                return;
            }
            --col;
        }
        col=ColumnCount-1;
        --row;
    }
    QApplication::beep();
}

void Spreadsheet::somethingChanged()
{
    if(autoRecalc){
        recalculate();
    }
    emit modified();
}

Cell* Spreadsheet::cell(int row, int colmumn) const
{
    return static_cast<Cell*>(item(row,colmumn));
}
QString Spreadsheet::text(int row, int colmun) const
{
    Cell *c=cell(row,colmun);
    if(c){
        return c->text();
    }
    else{
        return "";
    }
}

QString Spreadsheet::formula(int row, int column) const
{
    Cell *c=cell(row,column);
    if(c){
        return c->formula();
    }
    else{
        return "";
    }
}

void Spreadsheet::setFormula(int row, int column, const QString &formula)
{
    Cell *c=cell(row,column);
    if(!c){
        c=new Cell();
        setItem(row,column,c);
    }
    c->setFormula(formula);

}
