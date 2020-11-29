#ifndef GAME2048SETTINGS_H
#define GAME2048SETTINGS_H

#include <QWidget>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPainter>
#include <QTimer>
#include <cmath>

class Game2048Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Game2048Settings();
    QComboBox* BoardSize;
    QComboBox* BoardAmplifier;
    QRadioButton* Player;
    QRadioButton* Bot;
    QPushButton* Start;

public slots:
    void updateColor();

protected:
    void paintEvent(QPaintEvent* event);

private:
    const uint BackgroundWidth = 5;
    quint8 ItemRed = 0;
    quint8 ItemGreen = 0;
    quint8 ItemBlue = 0;
    bool ColorUpdateDirection = true; // true - color up, false - color down
    QTimer* ItemColorUpdateTimer;

signals:

};

#endif // GAME2048SETTINGS_H
