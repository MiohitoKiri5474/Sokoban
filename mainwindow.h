#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <utility>
#include <QPushButton>
#include <QDir>
#include <QSignalMapper>

#include <bits/stdc++.h>

// using namespace std;

#define maxN 15
#define F first
#define S second
typedef std::pair < int, int > pii;
#define swap std::swap
#define make_pair std::make_pair

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent ( QKeyEvent* );

public slots:
	void newGame ( void );
	void loadGame ( void );
	void quit ( void );
	void save ( void );
	void menu ( void );
	void restart ( void );
    void nextLevel ( void );
    void chooseLevel ( void );
    void getLevel ( int );
    void back ( void );

private:
    Ui::MainWindow *ui;
    QLabel *vmp[maxN][maxN], *bmp[maxN][maxN], *background, *cleared;
    int mp[maxN][maxN], backmp[maxN][maxN], n, m, step, cnt, all, levelNum;
    pii play;
    QFile file;
    QPixmap pixes[5], none;
    void startLevel ( void );
    void startSave ( void );
    void paintmp ( void );
    QPushButton *newGameButton, *exitButton, *loadGameButton, *chooseLevelButton, *levelButton[maxN][maxN], *backButton;
    QDir dir;
    void hideMenu ( void );
    void showMenu ( void );
    void levelClear ( void );
    void clear ( void );
    QSignalMapper *mpr = new QSignalMapper();
};

#endif // MAINWINDOW_H