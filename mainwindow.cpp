#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui -> setupUi ( this );

	// load images
	background = new QLabel ( this );
	background -> setGeometry ( 0, -100, 1000, 740 );
	background -> setPixmap ( QPixmap ( ":/images/Title.jpg" ) );
	background -> setScaledContents ( true );
	none.load ( ":/images/none.png" );
	pixes[0].load(":/images/boxes.png");
	pixes[1].load(":/images/bricks.png");
	pixes[2].load(":/images/indexex.png");
	pixes[3].load(":/images/ground.png");
	pixes[4].load(":/images/player.png");
	for ( int i = 0 ; i < maxN ; i++ ){
		for ( int j = 0 ; j < maxN ; j++ ){
			bmp[i][j] = new QLabel ( this );
			vmp[i][j] = new QLabel ( this );
		}
	}

	// build buttons
	newGameButton = new QPushButton ( this );
	chooseLevelButton = new QPushButton ( this );
	exitButton = new QPushButton ( this );
	loadGameButton = new QPushButton ( this );
	backButton = new QPushButton ( this );

	// set buttons position
	newGameButton -> setGeometry ( 390, 200, 200, 50 );
	chooseLevelButton -> setGeometry ( 390, 270, 200, 50 );
	loadGameButton -> setGeometry ( 390, 340, 200, 50 );
	exitButton -> setGeometry ( 390, 470, 200, 50 );
	backButton -> setGeometry ( 390, 470, 200, 50 );

	// set buttons text
	newGameButton -> setText ( "New Game" );
	chooseLevelButton -> setText ( "Choose Level" );
	loadGameButton -> setText ( "Load Game" );
	exitButton -> setText ( "Exit" );
	backButton -> setText ( "Back" );

	// set level button
	for ( int i = 0 ; i  < 4 ; i++ )
		for ( int j = 0 ; j < 4 ; j++ ){
			levelButton[i][j] = new QPushButton ( this );
			levelButton[i][j] -> setGeometry ( j * 70 + 370, i * 70 + 150, 50, 50 );
			levelButton[i][j] -> setText ( QString::number ( i * 4 + j + 1 ) );
			if ( !QFile ( ":/datas/level_" + QString::number ( i * 4 + j + 1 ) + ".dat" ).exists() )
				levelButton[i][j] -> setEnabled ( false );
			levelButton[i][j] -> hide();
			mpr -> setMapping ( levelButton[i][j], i * 4 + j + 1 );
			connect ( levelButton[i][j], SIGNAL	( clicked() ), mpr, SLOT ( map() ) );
		}

	// other operation
	cleared = new QLabel ( this );
	backButton -> hide();
	showMenu();

	// read save file if it's exist
	QFile file;
	file.setFileName ( QDir ( "datas/save.dat" ).absolutePath() );
	if ( !file.open ( QIODevice::ReadOnly ) )
		loadGameButton -> setEnabled ( false );
	else{
		QTextStream in ( &file );
		if ( in.atEnd() )
			loadGameButton -> setEnabled ( false );
		else
			loadGameButton -> setEnabled ( true );
	}
	file.close();

	// link buttons and functions
	connect ( mpr, SIGNAL ( mapped ( int ) ), this, SLOT ( getLevel ( int ) ) );
	connect ( newGameButton, SIGNAL ( clicked() ), this, SLOT ( newGame() ) );
	connect ( chooseLevelButton, SIGNAL ( clicked() ), this, SLOT ( chooseLevel() ) );
	connect ( loadGameButton, SIGNAL ( clicked() ), this, SLOT ( loadGame() ) );
	connect ( exitButton, SIGNAL ( clicked() ), this, SLOT ( quit() ) );
	connect ( backButton, SIGNAL ( clicked() ), this, SLOT ( back() ) );
	connect ( ui -> nextLevelButton, SIGNAL ( clicked() ), this, SLOT ( nextLevel() ) );
	connect ( ui -> replayButton, SIGNAL ( clicked() ), this, SLOT ( restart() ) );
	connect ( ui -> saveButton, SIGNAL ( clicked() ), this, SLOT ( save() ) );
	connect ( ui -> menuButton, SIGNAL ( clicked() ), this, SLOT ( menu() ) );
}

void MainWindow::showMenu ( void ){
	// show buttons
	background -> show();
	newGameButton -> show();
	chooseLevelButton -> show();
	loadGameButton -> show();
	exitButton -> show();

	// hide buttons
	ui -> stepLabel -> hide();
	ui -> step -> hide();
	ui -> levelLabel -> hide();
	ui -> level -> hide();
	ui -> nextLevelButton -> hide();
	ui -> replayButton -> hide();
	ui -> saveButton -> hide();
	ui -> menuButton -> hide();
	cleared -> hide();

	// read save file if it's exist
	QFile file;
	file.setFileName ( QDir ( "datas/save.dat" ).absolutePath() );
	if ( !file.open ( QIODevice::ReadOnly ) )
		loadGameButton -> setEnabled ( false );
	else{
		QTextStream in ( &file );
		loadGameButton -> setEnabled ( in.atEnd() ^ 1 );
	}
	file.close();
}

void MainWindow::hideMenu ( void ){
	// hide buttons
	background -> hide();
	newGameButton -> hide();
	chooseLevelButton -> hide();
	loadGameButton -> hide();
	exitButton -> hide();

	// show buttons
	ui -> stepLabel -> show();
	ui -> step -> show();
	ui -> levelLabel -> show();
	ui -> level -> show();
	ui -> replayButton -> show();
	ui -> saveButton -> show();
	ui -> menuButton -> show();
}

void MainWindow::newGame ( void ){
	hideMenu();
	levelNum = 1;
	startLevel();
}

void MainWindow::loadGame ( void ){
	hideMenu();
	startSave();
}

void MainWindow::quit ( void ){
	this -> close();
}

void MainWindow::back(){
	// show buttons
	newGameButton -> show();
	chooseLevelButton -> show();
	loadGameButton -> show();
	exitButton -> show();

	// hide buttons
	backButton -> hide();
	for ( int i = 0 ; i < 4 ; i++ )
		for ( int j = 0 ; j < 4 ; j++ )
			levelButton[i][j] -> hide();
}

void MainWindow::getLevel ( int chooseLevelNum ){
	hideMenu();
	backButton -> hide();
	for ( int i = 0 ; i < 4 ; i++ )
		for ( int j = 0 ; j < 4 ; j++ )
			levelButton[i][j] -> hide();

	levelNum = chooseLevelNum, startLevel();
}

void MainWindow::nextLevel ( void ){
	levelNum++;
	startLevel();
}

void MainWindow::restart ( void ){
	startLevel();
}

void MainWindow::save ( void ){
	QFile file;
	if ( !QDir ( "datas" ).exists() )
		QDir ( "." ).mkdir ( "datas" );
	file.setFileName ( QDir ( "datas/save.dat" ).absolutePath() );
	qDebug() << file.open ( QIODevice::WriteOnly | QIODevice::Text );
	QTextStream out ( &file );
	out << levelNum << ' ' << step << '\n';
	for ( int i = 0 ; i < n ; i++ ){
		for ( int j = 0 ; j < m ; j++ )
			out << mp[i][j] << ' ';
		out << '\n';
	}
	out << "---\n";
	for ( int i = 0 ; i < n ; i++ ){
		for ( int j = 0 ; j < m ; j++ )
			out	<< backmp[i][j] << ' ';
		if ( i < n - 1 )
			out << '\n';
	}
	file.close();
}

void MainWindow::menu(){
	showMenu();

	// clear maps
	for ( int i = 0 ; i < n ; i++ )
		for ( int j = 0 ; j < m ; j++ )
			bmp[i][j] -> clear(), vmp[i][j] -> clear();
}

void MainWindow::chooseLevel ( void ){
	// hide buttons
	newGameButton -> hide();
	chooseLevelButton -> hide();
	loadGameButton -> hide();
	exitButton -> hide();

	// show buttons
	backButton -> show();
	for ( int i = 0 ; i < 4 ; i++ )
		for ( int j = 0 ; j < 4 ; j++ )
			levelButton[i][j] -> show();
}

void MainWindow::startLevel ( void ){
	QString swp;
	QStringList list;
	QFile file;
	file.setFileName ( ":/datas/level_" + QString::number ( levelNum ) + ".dat" );
	if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
		return;
	QTextStream in ( &file );
	all = step = 0;
	for ( int i = 0 ; !in.atEnd() ; n = ++i ){
		swp = in.readLine();
		list = swp.split ( " " );
		for ( int j = 0 ; j < list.size() && list[j] != "" ; m = ++j ){
			backmp[i][j] = mp[i][j] = list[j].toInt();
			if ( mp[i][j] == 2 )
				mp[i][j] = -1, all++;
			else if ( mp[i][j] == 3 )
				mp[i][j] = -1;
			else if ( mp[i][j] == 4 )
				backmp[i][j] = 3, play = pii ( i, j );
			else if ( mp[i][j] > 4 )
				backmp[i][j] = 3;
		}
	}
	file.close();
	ui -> level -> display ( QString::number ( levelNum ) );
	ui -> nextLevelButton -> hide();
	cleared -> hide();
	paintmp();
}

void MainWindow::startSave ( void ){
	QString swp;
	QStringList list;
	QFile file;
	file.setFileName ( QDir ( "datas/save.dat" ).absolutePath() );
	if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
		return;
	QTextStream in ( &file );
	swp = in.readLine();
	list = swp.split ( " " );
	levelNum = list[0].toInt();
	step = list[1].toInt();
	for ( int i = 0 ; !in.atEnd() ; n = ++i ){
		swp = in.readLine();
		if ( swp == "---" )
			break;
		list = swp.split ( " " );
		for ( int j = 0 ; j < list.size() && list[j] != "" ; m = ++j ){
			mp[i][j] = list[j].toInt();
			if ( mp[i][j] == 4 )
				play = pii ( i, j );
		}
	}

	for ( int i = 0 ; !in.atEnd() ; i++ ){
		swp = in.readLine();
		list = swp.split ( " " );
		for ( int j = 0 ; j < list.size() && list[j] != "" ; j++ )
			backmp[i][j] = list[j].toInt();
	}
	file.close();
	ui -> level -> display ( QString::number ( levelNum ) );
	ui -> nextLevelButton -> hide();
	paintmp();
}

void MainWindow::paintmp ( void ){
	for ( int i = 0 ; i < n ; i++ ){
		for ( int j = 0 ; j < m ; j++ ){
			bmp[i][j] -> setGeometry ( j * 80 + 200, i * 80, 80, 80 );
			vmp[i][j] -> setGeometry ( j * 80 + 200, i * 80, 80, 80 );
			if ( 1 <= backmp[i][j] && backmp[i][j] <= 3 )
				bmp[i][j] -> setPixmap ( pixes[backmp[i][j]] );
			if ( mp[i][j] == -1 )
				vmp[i][j] -> setPixmap ( none );
			else if ( mp[i][j] <= 4 )
				vmp[i][j] -> setPixmap ( pixes[mp[i][j]] );
			else
				vmp[i][j] -> setPixmap ( pixes[0] );
			bmp[i][j] -> setScaledContents ( true );
			vmp[i][j] -> setScaledContents ( true );
		}
	}

	ui -> step -> display ( QString::number ( step ) );
	cnt = all;
	for ( int i = 0 ; i < n ; i++ )
		for ( int j = 0 ; j < m ; j++ )
			if ( backmp[i][j] == 2 && mp[i][j] >= 5 )
				cnt--;
	if ( cnt == 0 ){
		if ( QFile ( ":/datas/level_" + QString::number ( levelNum + 1 ) + ".dat" ).exists() ){
			ui -> nextLevelButton -> show();
			levelClear();
		}
		else
			clear();
	}
}

void MainWindow::levelClear ( void ){
	cleared -> setGeometry ( 210, 250, 400, 300 );
	cleared -> setPixmap ( QPixmap ( ":/images/clear.png" ) );
	cleared -> setScaledContents ( true );
	cleared -> show();
}

void MainWindow::clear ( void ){
	cleared -> setGeometry ( 190, 240, 400, 300 );
	cleared -> setPixmap ( QPixmap ( ":/images/clearAll.png" ) );
	cleared -> setScaledContents ( true );
	cleared -> show();
}

void MainWindow::keyPressEvent(QKeyEvent *e){
	if(!cnt)
		return;
	if ( e->key() == Qt::Key_W ){
		if ( mp[play.F - 1][play.S] > 4 || mp[play.F - 1][play.S] == -1 ){
			if ( mp[play.F - 1][play.S] > 4 )
				if ( mp[play.F - 2][play.S] == 1 || mp[play.F - 2][play.S] > 4 )
					goto end;
				else
					swap ( mp[play.F - 1][play.S], mp[play.F - 2][play.S] );
			step++;
			swap ( mp[play.F][play.S], mp[play.F - 1][play.S] );
			play = make_pair ( play.F - 1, play.S );
		}
	}
	if ( e->key() == Qt::Key_S ){
		if ( mp[play.F + 1][play.S] > 4 || mp[play.F + 1][play.S] == -1 ){
			if(mp[play.F + 1][play.S] > 4)
				if(mp[play.F + 2][play.S] == 1 || mp[play.F + 2][play.S] > 4)
					goto end;
				else
				  	swap(mp[play.F + 1][play.S], mp[play.F + 2][play.S]);
			step++;
			swap ( mp[play.F][play.S], mp[play.F + 1][play.S] );
			play = make_pair ( play.F + 1, play.S );
		}
	}
	if(e->key() == Qt::Key_A){
		if ( mp[play.F][play.S - 1] > 4 || mp[play.F][play.S - 1] == -1 ){
			if(mp[play.F][play.S - 1] > 4)
				if(mp[play.F][play.S - 2] == 1 || mp[play.F][play.S - 2] > 4)
					goto end;
				else
					swap(mp[play.F][play.S - 1], mp[play.F][play.S - 2]);
			step++;
			swap(mp[play.F][play.S], mp[play.F][play.S - 1]);
			play = make_pair(play.F, play.S - 1);
		}
	}
	if ( e->key() == Qt::Key_D ){
		if ( mp[play.F][play.S + 1] > 4 || mp[play.F][play.S + 1] == -1 ){
			if ( mp[play.F][play.S + 1] > 4 )
				if ( mp[play.F][play.S + 2] == 1 || mp[play.F][play.S + 2] > 4)
					goto end;
				else
					swap ( mp[play.F][play.S + 1], mp[play.F][play.S + 2] );
			step++;
			swap ( mp[play.F][play.S], mp[play.F][play.S + 1] );
			play = make_pair ( play.F, play.S + 1 );
		}
	}
	end:
	paintmp();
}

MainWindow::~MainWindow(){
	delete ui;
}
