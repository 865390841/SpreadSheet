#include "gotocelldialog.h"
#include "ui_gotocelldialog.h"

GoToCellDialog::GoToCellDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoToCellDialog)
{
    ui->setupUi(this);
    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp));
    setWindowIcon(QIcon(":/images/images/bingo"));

}

GoToCellDialog::~GoToCellDialog()
{
    delete ui;
}

void GoToCellDialog::on_lineEdit_textChanged()
{
    if(!ui->lineEdit->text().isEmpty()){
        ui->okButton->setEnabled(true);
    }
    else{
        ui->okButton->setEnabled(false);
    }
}
