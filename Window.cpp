//
// Created by Frederic on 05/08/2021.
//

#include "Window.h"
#include <QMessageBox>
#include <QtWidgets>
#include <QTimer>

void delay(){
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


//====================================================================



Window::Window(QWidget *parent, Simulatie &sim) : QMainWindow(parent){
    ui = new UI();
    s = sim;

    QGraphicsView *view = new QGraphicsView(ui);
    setCentralWidget(view);

    connect(ui,SIGNAL(rightClicked(int,int)),this,SLOT(rightClicked(int,int)));
    connect(ui,SIGNAL(clicked(int,int)),this,SLOT(clicked(int,int)));
    connect(ui,SIGNAL(missClicked()),this,SLOT(missClicked()));
    connect(ui,SIGNAL(doubleClicked()),this,SLOT(doubleClicked()));

    createActions();
    createMenus();

    ui->draw(sim);
}

void Window::createActions() {
    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Abandon game"));
    connect(exitAct, &QAction::triggered, this, &Window::on_actionExit_triggered);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save game to disk"));
    connect(saveAct, &QAction::triggered, this, &Window::save);
}

void Window::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);
    fileMenu->addAction(saveAct);
}

void Window::on_actionExit_triggered() {
    if (QMessageBox::Yes == QMessageBox::question(this,
                                                  tr("Spel verlaten"),
                                                  tr("Bent u zeker dat u de simulatie wil verlaten?\nNiet opgeslagen wijzigingen gaan verloren.")))
    {
        QApplication::quit();
    }
}

void Window::clicked(int x, int y) {
    s.infect(x, y);
    ui->draw(s);
}

void Window::rightClicked(int x, int y) {
    QMessageBox::information(this,tr("Mens gegevens"),tr(s.world.grid[x][y]->print().c_str()));
}

void Window::missClicked() {
    s.simulate(1);
    ui->draw(s);
}


void Window::doubleClicked(){
    while(!s.autoSimulate()){
        delay();
        ui->draw(s);
    }
    ui->draw(s);
}

void Window::save() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save log"), "",
                                                    tr("Text File (*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QDataStream out(&file);
        out << QString("Test");
    }
}