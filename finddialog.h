#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include<QDialog>
#include<QLabel>
class QCheckBox;
class QLineEdit;
class QPushButton;
class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent=0);

signals:
    void findNext(const QString &str,Qt::CaseSensitivity cs);
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
private slots:
    void findClicked();
    void enableFindButton(const QString &text);

private:
    QLabel *lable;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};

#endif // FINDDIALOG_H
