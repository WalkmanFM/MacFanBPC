#include "ventana.h"
#include "ui_ventana.h"
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Ventana::Ventana(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ventana)
{
    ui->setupUi(this);
    ui->t0->setText("Temperatura Core 0");
    ui->t1->setText("Temperatura Core 1");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTemp()));
    timer->start(1000);
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(setFanLabel()));
    connect(ui->slider, SIGNAL(sliderReleased()), this, SLOT(setFanRPM()));
    showTemp();
    setFanLabel();
    setFanRPM();

    setWindowTitle(tr("MacFanBP"));

}

void Ventana::showTemp(){

    FILE *c0;
    char temp[2];
    //Leer la temperatura del sensor del core 0
    c0 = fopen("/sys/devices/platform/applesmc.768/temp4_input", "r");
    fgets(temp,3,c0);
    fclose(c0);
    ui->l0->display(temp);
    int t1 = atoi(temp);
    //Leer tempereatura del sensor del core 1
    c0 = fopen("/sys/devices/platform/applesmc.768/temp10_input", "r");
    fgets(temp,3,c0);
    fclose(c0);
    ui->l1->display(temp);
    int t2 = atoi(temp);
    ui->progressBar->setValue(((t1+t2)/2));
}

void Ventana::setFanLabel(){
    QString rpm = QString::number(ui->slider->value());
    ui->fanValue->setText(rpm + "RPM");
}

void Ventana::setFanRPM(){
    FILE *c0;
    //Leer la temperatura del sensor del core 0
    c0 = fopen("/sys/devices/platform/applesmc.768/fan1_min", "w");

    QString rpm = ui->fanValue->text();
    rpm = rpm.left(4); //Erasing RPM string
    QByteArray ba = rpm.toLatin1();
    char *rpms = ba.data();

    fprintf(c0, "%s", rpms);
    fclose(c0);
}



Ventana::~Ventana()
{
    delete ui;
}
