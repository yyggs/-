#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <qdebug.h>
#include "parser.h"
extern std::vector<check> errlist;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    std::vector<int> num;

private slots:
    void saveToFile();
    void printF();
    int calnum(int pos);   //计算处于第几行

private:
    QTextEdit *codeEditor;
    QPushButton *saveButton;
    QPushButton *parseButton;
};


#endif // MAINWINDOW_H
