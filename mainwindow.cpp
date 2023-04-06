#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置窗口的最小大小
    setMinimumSize(800, 600);

    // 设置窗口的初始大小
    resize(1400, 1000);


    // 创建TextEdit控件
    codeEditor = new QTextEdit(this);

    // 获取屏幕高度
    int screenHeight = QDesktopWidget().availableGeometry().height();

    // 设置输入代码区域的高度为屏幕高度
    int editorHeight = screenHeight;
    codeEditor->setMinimumHeight(editorHeight);
    codeEditor->setMaximumHeight(editorHeight);
    codeEditor->setMinimumWidth(1000);
    codeEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 设置控件大小和位置
    //codeEditor->setGeometry(0, 0, width(), height());

    // 创建QPushButton控件
    saveButton = new QPushButton("保存", this);
    // 设置控件大小和位置
    saveButton->setGeometry(1100, 800, 200, 60);

    // 连接信号和槽
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveToFile);

    saveButton = new QPushButton("parse", this);
    // 设置控件大小和位置
    saveButton->setGeometry(1100, 600, 200, 60);

    // 连接信号和槽
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::printF);
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), "", tr("Text files (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << codeEditor->toPlainText();
            file.close();
            QMessageBox::information(this, tr("保存成功"), tr("文件已保存！"));
        } else {
            //QMessageBox::warning(this, tr("保存失败"), tr("无法保存文件！"));
        }
    }
}

int MainWindow::calnum(int pos){
    int i = 1;
    while(pos > 0 && i <= num.size()){
        pos -= num[i];
        i++;
    }
    return i;
}

void MainWindow::printF(){
    QFile file("test.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << codeEditor->toPlainText();
        file.close();
    }
    //QMessageBox::information(this, tr("yes"), tr("no"));
    // 获取QTextEdit中的文本
    QString text = codeEditor->toPlainText();
    QVector<QString> words = text.split(QRegExp("[\\s]+")).toVector();


    QVector<QString> hang = text.split("\n").toVector();
    qDebug() << hang.size();
    num.push_back(0);
    for(int i = 0; i < hang.size(); i++){
        QVector<QString> word = hang[i].split(" ").toVector();
        //qDebug() << word.size();
        int nums = 0;
        for(int i = 0; i < word.size(); i++){
            if(word[i] != ""){
                nums++;
            }
        }
        num.push_back(nums);
    }


    Parser pa = Parser();
    pa.parse(0);
    pa.tree->printstring();

    for (int i = 0; i < errlist.size(); i++) {
        int index = errlist[i].position - 1;
        switch(errlist[i].kind){
        case check::NAMEERR:
            if (index >= 0 && index < words.size()) {
                // 获取目标单词的QTextCursor
                QTextCursor cursor = codeEditor->textCursor();
                //int a = text.indexOf(words[index]);
                //int b = index;
                cursor.setPosition(0 + text.indexOf(words[index]));
                cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
                // 将单词设置为红色
                QTextCharFormat format;
                format.setForeground(Qt::red);
                cursor.setCharFormat(format);
            }
            break;
        case check::VARERR:
            if (index >= 0 && index < words.size()) {
                // 获取目标单词的QTextCursor
                QTextCursor cursor = codeEditor->textCursor();
                cursor.setPosition(0 + text.indexOf(words[index]));
                cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
                // 将单词设置为蓝色
                QTextCharFormat format;
                format.setForeground(Qt::blue);
                cursor.setCharFormat(format);
            }
            break;
        case check::SENTENCEERR:
        {//int aim = calnum(index);
            //qDebug() << calnum(index);
            int lineNum = calnum(index) - 1;  // QTextEdit中的行号从0开始
            QTextCursor cursor = codeEditor->textCursor();  // 获取当前光标
            cursor.movePosition(QTextCursor::Start);  // 移动光标到文本开头
            for (int i = 0; i < lineNum; i++) {
                cursor.movePosition(QTextCursor::NextBlock);  // 逐行移动光标
            }
            QTextBlockFormat blockFormat = cursor.blockFormat();  // 获取该行的格式
            blockFormat.setBackground(Qt::yellow);
                    //setUnderlineStyle(QTextCharFormat::SingleUnderline);  // 设置下划线
            cursor.setBlockFormat(blockFormat);  // 将格式应用到该行
            break;
        }
        case check::CIRERR:
            index -= 2;
            if (index >= 0 && index < words.size()) {
                // 获取目标单词的QTextCursor
                QTextCursor cursor = codeEditor->textCursor();
                cursor.setPosition(0 + text.indexOf(words[index]));
                cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
                // 将单词设置为蓝色
                QTextCharFormat format;
                format.setForeground(Qt::green);
                cursor.setCharFormat(format);
            }
            break;


        case check::FUNERR:
            int lineNum = calnum(index) - 1;  // QTextEdit中的行号从0开始
            QTextCursor cursor = codeEditor->textCursor();  // 获取当前光标
            cursor.movePosition(QTextCursor::Start);  // 移动光标到文本开头
            for (int i = 0; i < lineNum; i++) {
                cursor.movePosition(QTextCursor::NextBlock);  // 逐行移动光标
            }
            QTextBlockFormat blockFormat = cursor.blockFormat();  // 获取该行的格式
            blockFormat.setBackground(Qt::darkGray);
                    //setUnderlineStyle(QTextCharFormat::SingleUnderline);  // 设置下划线
            cursor.setBlockFormat(blockFormat);  // 将格式应用到该行
            break;
        }
    }

}
