#include "sortdialog.h"
#include "ui_sortdialog.h"
#include <QDebug>
SortDialog::SortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SortDialog)
{
    ui->setupUi(this);
    ui->secondaryGroupBox->hide();
    ui->tertiaryGroupBox->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize); //
    setColumnRange('A','Z');
}

SortDialog::~SortDialog()
{
    delete ui;
}

void SortDialog::setColumnRange(QChar first, QChar last)
{
    ui->primaryColumnCombo->clear();
    ui->secondryColumnCombo->clear();
    ui->tertiaryColumnCombo->clear();

    ui->secondryColumnCombo->addItem(tr("None"));
    ui->tertiaryColumnCombo->addItem(tr("None"));
    ui->primaryColumnCombo->setMinimumSize(ui->secondryColumnCombo->sizeHint());

    QChar ch=first;
    while(ch<=last){
        ui->primaryColumnCombo->addItem(QString(ch));
        ui->secondryColumnCombo->addItem(QString(ch));
        ui->tertiaryColumnCombo->addItem(QString(ch));
        ch=ch.unicode()+1;
    }
}

