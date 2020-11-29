#include "game2048.h"

#define DEFAULT_NULL -1

Game2048::Game2048(uint _timeout) : TimerTimeout(_timeout), isPlay(false)
{
    SettingsGui = new Game2048Settings();
    moveTimer = new QTimer(this);
    moveTimer->setSingleShot(true);
    moveTimer->setInterval(TimerTimeout);
    connect(moveTimer, &QTimer::timeout, this, &Game2048::move);
    connect(SettingsGui->Start, &QPushButton::clicked, this, &Game2048::onClickStart);
}

Game2048::~Game2048()
{

}

void Game2048::start()
{
    SettingsGui->show();
}

void Game2048::play(bool play_mode) // true - player | false - bot
{
    isPlay = true;
    SettingsGui->hide();
    BoardGui->show();
    if (!play_mode) // bot is playing
    {
        moveTimer->start();
    }
}

void Game2048::move()
{
    QVector<qint32> board_diff = {DEFAULT_NULL, DEFAULT_NULL, DEFAULT_NULL, DEFAULT_NULL};
    QVector<QVector<uint>> curr_board = BoardGui->getBoard();
    for (uint _move = 0; _move < 4; _move++)
    {
        QVector<QVector<uint>> board_after_move = BoardGui->getBoardAfterMove((Game2048Gui::Move2048)_move);
        if (!BoardGui->compareBoards(curr_board, board_after_move))
        {
            board_diff[_move] = Bot->evaluationMove(board_after_move);
        }
        qDebug() << _move << board_diff[_move];
    }
    qint32 max_diff = board_diff[0];
    uint max_diff_move = 0;
    for (uint i = 1; i < 4; i++)
    {
        if (((board_diff[i] != DEFAULT_NULL) && (board_diff[i] > max_diff)) || max_diff == DEFAULT_NULL)
        {
            max_diff = board_diff[i];
            max_diff_move = i;
        }
    }
    qDebug() << "move: " << max_diff_move;
    if (board_diff[max_diff_move] != DEFAULT_NULL)
    {
        bool move_result = BoardGui->move((Game2048Gui::Move2048)max_diff_move);
        if (move_result && isPlay)
           moveTimer->start();
    }
    else
    {
        isPlay = false;
        BoardGui->finishGame();
    }
}

void Game2048::onFinishGame(uint _score)
{
    isPlay = false;
}

void Game2048::onClickStart()
{
    // getting new game settings
    BoardSize = (uint)SettingsGui->BoardSize->currentIndex()+3;
    BoardAmplifier = (uint)SettingsGui->BoardAmplifier->currentIndex()+2;
    bool player = SettingsGui->Player->isChecked();
    bool bot = SettingsGui->Bot->isChecked();

    // starting new game
    if (player && !bot) // player mode
    {
        BoardGui = new Game2048Gui(true, BoardSize, BoardAmplifier);
        connect(BoardGui, &Game2048Gui::gameFinished, this, &Game2048::onFinishGame);
        connect(BoardGui, &Game2048Gui::closed, this, &Game2048::onBoardGuiClosed);
        connect(BoardGui, &Game2048Gui::restart, this, &Game2048::onPlayerRestart);
        play(true);
    }
    else if (bot && !player) // bot mode
    {
        BoardGui = new Game2048Gui(false, BoardSize, BoardAmplifier);
        connect(BoardGui, &Game2048Gui::gameFinished, this, &Game2048::onFinishGame);
        connect(BoardGui, &Game2048Gui::closed, this, &Game2048::onBoardGuiClosed);
        connect(BoardGui, &Game2048Gui::pause, this, &Game2048::onPlayerPause);
        connect(BoardGui, &Game2048Gui::botSpeedChanged, this, &Game2048::onBotSpeedChanged);
        Bot = new Game2048Bot(BoardSize);
        play(false);
    }
    else // nothing chosen
    {

    }
}

void Game2048::onBoardGuiClosed()
{
    isPlay = false;
    SettingsGui->show();
}

void Game2048::onPlayerRestart()
{
    isPlay = true;
}

void Game2048::onPlayerPause()
{
    if (isPlay)
    {
        moveTimer->stop();
        isPlay = false;
    }
    else
    {
        moveTimer->start();
        isPlay = true;
    }
}

void Game2048::onBotSpeedChanged(bool direction)
{
    if (direction) // speed up
    {
        if (TimerTimeout > 125) TimerTimeout /= 2;
    }
    else // slow down
    {
        if (TimerTimeout < 2000) TimerTimeout *= 2;
    }
    moveTimer->setInterval(TimerTimeout);
}

