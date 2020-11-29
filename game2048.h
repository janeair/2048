#ifndef GAME2048_H
#define GAME2048_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "game2048gui.h"
#include "game2048bot.h"
#include "game2048settings.h"

class Game2048 : public QObject
{
    Q_OBJECT

public:
    Game2048(uint _timeout = 500);
    ~Game2048();

    void start();
    void play(bool play_mode);

public slots:
    void onFinishGame(uint _score);
    void move();
    void onClickStart();
    void onBoardGuiClosed();
    void onPlayerRestart();
    void onPlayerPause();
    void onBotSpeedChanged(bool direction);
    //virtual void onBotRestart();

private:
    uint BoardSize;
    uint BoardAmplifier;
    uint TimerTimeout;
    bool isPlay;
    Game2048Gui* BoardGui = nullptr;
    Game2048Settings* SettingsGui;
    Game2048Bot* Bot = nullptr;
    QTimer* moveTimer;
};

#endif // GAME2048_H
