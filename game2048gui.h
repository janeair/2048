#ifndef GAME2048GUI_H
#define GAME2048GUI_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMenu>
#include <QTimer>
#include <cmath>
#include <QIcon>
#include <QDebug>
#include <QMessageBox>

class Game2048Gui : public QWidget
{
    Q_OBJECT

public:
    enum Move2048 { Left, Up, Right, Down };
    enum Theme2048 { Red, Green, Blue };
    enum Animation2048 { SpeedUp, SlowDown };

    explicit Game2048Gui(bool _mode, uint _size = 4, uint _pow = 2, Game2048Gui::Theme2048 _theme = Game2048Gui::Green);
    bool compareBoards(QVector<QVector<uint>> board1, QVector<QVector<uint>> board2);
    QVector<QVector<uint>> getBoard();
    QVector<QVector<uint>> getBoardAfterMove(Move2048 _move);
    void setPlayMode(bool _mode);
    bool move(Move2048 _move);
    void finishGame();

private:
    bool PlayMode; // true - player | false - bot
    const uint BoardSize;
    const uint BoardSizePixels;
    const uint BoardAmplifier;

    const uint BorderWidth = 5;
    const QColor BackgroundColor = Qt::white;
    const QColor GridColor = Qt::black;
    const QColor TextColor = Qt::black;

    bool AnimationToPaint = false;
    Animation2048 AnimationType;

    uint Game2048Score = 0; // global game score
    uint Move2048Score; // score added during current move`
    Theme2048 BoardTheme;
    QMenu* HelpMenu;
    QVector<QVector<uint>> Board;
    QTimer* AnimationTimer;

    void drawBoard();
    void drawAnimation();
    QVector<QVector<uint>> shiftBoard(Game2048Gui::Move2048 _move);
    QColor makeColor(uint number);
    void fillBoard();
    bool addItem();
    bool isGameFinished();

    //test
    void testBoard();

protected:
    void paintEvent(QPaintEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event);

signals:
    void gameFinished(uint _score);
    void closed();
    void restart();
    void pause();
    void botSpeedChanged(bool direction); // true - speed up, false - slow down

public slots:
    void onCntrlR();
    void onCntrlS();
    void onSpeedUp();
    void onSlowDown();
    void onAnimationEnd();
};

#endif // GAME2048GUI_H
