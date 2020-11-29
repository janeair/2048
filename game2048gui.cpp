#include "game2048gui.h"

Game2048Gui::Game2048Gui(bool _mode, uint _size, uint _pow, Game2048Gui::Theme2048 _theme)
    : PlayMode(_mode), BoardSize(_size), BoardSizePixels(_size*60), BoardAmplifier(_pow), BoardTheme(_theme)
{
    setFixedSize(BoardSizePixels+2*BorderWidth, BoardSizePixels+2*BorderWidth);
    setWindowTitle("2048");
    HelpMenu = new QMenu(this);
    if (PlayMode)
    {
        HelpMenu->addAction("Change Theme", this, &Game2048Gui::onCntrlR, Qt::CTRL + Qt::Key_R);
        HelpMenu->addAction("Restart Game", this, &Game2048Gui::onCntrlS, Qt::CTRL + Qt::Key_S);
    }
    else
    {
        HelpMenu->addAction("Change Theme", this, &Game2048Gui::onCntrlR, Qt::CTRL + Qt::Key_R);
        HelpMenu->addAction("Pause/Resume Game", this, &Game2048Gui::onCntrlS, Qt::CTRL + Qt::Key_S);
        HelpMenu->addAction("Speed Up", this, &Game2048Gui::onSpeedUp, Qt::Key_Right);
        HelpMenu->addAction("Slow Down", this, &Game2048Gui::onSlowDown, Qt::Key_Left);
    }
    QIcon _icon = QIcon("icon.png");
    AnimationTimer = new QTimer(this);
    AnimationTimer->setSingleShot(true);
    connect(AnimationTimer, &QTimer::timeout, this, &Game2048Gui::onAnimationEnd);
    setWindowIcon(_icon);
    fillBoard();
}

void Game2048Gui::paintEvent(QPaintEvent *_event)
{
   drawBoard();
   if (AnimationToPaint) drawAnimation();
}

void Game2048Gui::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
            case Qt::Key_Up:
                if (PlayMode) move(Game2048Gui::Up);
                break;
            case Qt::Key_Down:
                if (PlayMode) move(Game2048Gui::Down);
                break;
            case Qt::Key_Right:
                if (PlayMode) move(Game2048Gui::Right);
                else onSpeedUp();
                break;
            case Qt::Key_Left:
                if (PlayMode) move(Game2048Gui::Left);
                else onSlowDown();
                break;
            case Qt::Key_R:
                if (event->modifiers() == Qt::ControlModifier)
                    onCntrlR();
                break;
            case Qt::Key_S:
                if (event->modifiers() == Qt::ControlModifier)
                    onCntrlS();
                break;
            default:
                break;
            }
    event->accept();
}

void Game2048Gui::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        QPoint event_point = event->globalPos();
        HelpMenu->popup(event_point);
    }
}

void Game2048Gui::closeEvent(QCloseEvent* event)
{
    emit closed();
    event->accept();
}

void Game2048Gui::setPlayMode(bool _mode)
{
    PlayMode = _mode;
}

void Game2048Gui::drawBoard()
{
    QPainter _painter(this);
    _painter.setFont(QFont("Times", 12));
    _painter.setPen(GridColor);
    //draw background
    _painter.fillRect(QRect(0, 0, BoardSizePixels+2*BorderWidth, BoardSizePixels+2*BorderWidth), BackgroundColor);
    //draw gridlines and board
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
        {

            QRect item(BorderWidth + i*BoardSizePixels/BoardSize, BorderWidth + j*BoardSizePixels/BoardSize, BoardSizePixels/BoardSize, BoardSizePixels/BoardSize);
            _painter.setBrush(QBrush(makeColor(Board[i][j])));
            _painter.drawRect(item);
            if (Board[i][j] != 0)
            {
                _painter.drawText(item, Qt::AlignCenter, QString::number(static_cast<quint64>(pow(BoardAmplifier, Board[i][j]))));
            }
        }
}

void Game2048Gui::drawAnimation()
{
    QPainter _painter(this);
    switch (AnimationType) {
        case SpeedUp:
        {
            _painter.setPen(QColor(0,127,0,56));
            _painter.setFont(QFont("Times", 22));
            QRect _rect(0,0,BoardSizePixels+2*BorderWidth,BoardSizePixels+2*BorderWidth);
            _painter.drawText(_rect, Qt::AlignCenter, "speed up");
            AnimationTimer->start(300);
        }
        break;
        case SlowDown:
        {
            _painter.setPen(QColor(127,0,0,56));
            _painter.setFont(QFont("Times", 22));
            QRect _rect(0,0,BoardSizePixels+2*BorderWidth,BoardSizePixels+2*BorderWidth);
            _painter.drawText(_rect, Qt::AlignCenter, "slow down");
            AnimationTimer->start(300);
        }
        break;
        default:
        break;
    }
    AnimationToPaint = false;
}

void Game2048Gui::fillBoard()
{
    Game2048Score = 0;
    QVector<uint> _line(BoardSize, 0);
    for (uint i = 0; i < BoardSize; i++)
    {
        Board.push_front(_line);
    }
    addItem();
    addItem();
    update();
}

void Game2048Gui::testBoard()
{
    QVector<uint> _line(BoardSize, 0);
    for (uint i = 0; i < BoardSize; i++)
    {
        Board.push_front(_line);
    }
    uint test_unit = 0;
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
        {
            Board[i][j] = test_unit;
            test_unit++;
        }
}

QVector<QVector<uint>> Game2048Gui::getBoard()
{
    return Board;
}

QVector<QVector<uint>> Game2048Gui::shiftBoard(Game2048Gui::Move2048 _move)
{
    Move2048Score = 0;
    QVector<QVector<uint>> _board = Board;
    switch (_move) {
            case Game2048Gui::Up:
                {
                    for (uint i = 0; i < BoardSize; i++)
                    {
                        uint end = BoardSize-1;
                        uint j = 0;
                        while (j < end)
                        {
                            if (_board[i][j] == 0)
                            {
                                for (uint k = j; k < end; k++)
                                    std::swap(_board[i][k], _board[i][k+1]);
                                end--;
                            }
                            else
                                j++;
                        }
                        for (j = 0; j < end; j++)
                        {
                            if (_board[i][j] == _board[i][j+1])
                            {
                                _board[i][j]++;
                                Move2048Score += pow(BoardAmplifier, _board[i][j]);
                                _board[i][j+1] = 0;
                                for (uint k = j+1; k < end; k++)
                                    std::swap(_board[i][k], _board[i][k+1]);
                                end--;
                            }
                        }
                    }
                }
                break;
            case Game2048Gui::Down:
                {
                    for (uint i = 0; i < BoardSize; i++)
                    {
                        uint end = 0;
                        uint j = BoardSize-1;
                        while (j > end)
                        {
                            if (_board[i][j] == 0)
                            {
                                for (uint k = j; k > end; k--)
                                    std::swap(_board[i][k], _board[i][k-1]);
                                end++;
                            }
                            else
                                j--;
                        }
                        for (j = BoardSize-1; j > end; j--)
                        {
                            if (_board[i][j] == _board[i][j-1])
                            {
                                _board[i][j]++;
                                 Move2048Score += pow(BoardAmplifier, _board[i][j]);
                                _board[i][j-1] = 0;
                                for (uint k = j-1; k > end; k--)
                                    std::swap(_board[i][k], _board[i][k-1]);
                                end++;
                            }
                        }
                    }
                }
                break;
            case Game2048Gui::Right:
                {
                    for (uint j = 0; j < BoardSize; j++)
                    {
                        uint end = 0;
                        uint i = BoardSize-1;
                        while (i > end)
                        {
                            if (_board[i][j] == 0)
                            {
                                for (uint k = i; k > end; k--)
                                    std::swap(_board[k][j], _board[k-1][j]);
                                end++;
                            }
                            else
                                i--;
                        }
                        for (i = BoardSize-1; i > end; i--)
                        {
                            if (_board[i][j] == _board[i-1][j])
                            {
                                _board[i][j]++;
                                 Move2048Score += pow(BoardAmplifier, _board[i][j]);
                                _board[i-1][j] = 0;
                                for (uint k = i-1; k > end; k--)
                                    std::swap(_board[k][j], _board[k-1][j]);
                                end++;
                            }
                        }
                    }
                }
                break;
            case Game2048Gui::Left:
                {
                    for (uint j = 0; j < BoardSize; j++)
                    {
                        uint end = BoardSize-1;
                        uint i = 0;
                        while (i < end)
                        {
                            if (_board[i][j] == 0)
                            {
                                for (uint k = i; k < end; k++)
                                    std::swap(_board[k][j], _board[k+1][j]);
                                end--;
                            }
                            else
                                i++;
                            }
                        for (i = 0; i < end; i++)
                        {
                            if (_board[i][j] == _board[i+1][j])
                            {
                                _board[i][j]++;
                                 Move2048Score += pow(BoardAmplifier, _board[i][j]);
                                _board[i+1][j] = 0;
                                for (uint k = i+1; k < end; k++)
                                    std::swap(_board[k][j], _board[k+1][j]);
                                end--;
                            }
                        }
                    }
                }
                break;
            default:
                return _board;
                break;
            }
    return _board;
}

bool Game2048Gui::compareBoards(QVector<QVector<uint>> board1, QVector<QVector<uint>> board2)
{
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
        {
            if (board1[i][j] != board2[i][j])
                return false; // different
        }
    return true; // same
}

bool Game2048Gui::isGameFinished()
{
    // checking for free places
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
            if (Board[i][j] == 0)
                return false;
    // checking for possible merges
    for (uint i = 0; i < BoardSize-1; i++)
        for (uint j = 0; j < BoardSize-1; j++)
        {
            if (Board[i][j] == Board[i][j+1] || Board[i][j] == Board[i+1][j])
                return false;
        }
    for (uint i = 0; i < BoardSize-1; i++)
    {
        if (Board[i][BoardSize-1] == Board[i+1][BoardSize-1] || Board[BoardSize-1][i] == Board[BoardSize-1][i+1])
            return false;
    }
    return true;
}

void Game2048Gui::finishGame()
{
    int exit_return = QMessageBox::information(this, "2048", "Wasted\nScore: " + QString::number(Game2048Score),
                    QMessageBox::Ok, QMessageBox::Ok);
    if (exit_return == QMessageBox::Ok)
    {
        emit gameFinished(Game2048Score);
        if (PlayMode) // player mode
        {
            fillBoard();
            emit restart();
        }
        else          // bot mode
        {
            close();
        }
    }
}

QVector<QVector<uint>> Game2048Gui::getBoardAfterMove(Game2048Gui::Move2048 _move)
{
    QVector<QVector<uint>> _board = shiftBoard(_move);
    return _board;
}

bool Game2048Gui::move(Game2048Gui::Move2048 _move)
{
    QVector<QVector<uint>> _board = shiftBoard(_move);
    if (compareBoards(_board, Board)) // move isn't done
    {
        return false;
    }
    else // move is done
    {
        Board = _board;
        Game2048Score += Move2048Score;
        addItem();
        update();
        if (isGameFinished())
            finishGame();
        return true;
    }
}

bool Game2048Gui::addItem()
{
    uint places_count = 0;
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
            if (Board[i][j] == 0) places_count++;
    if (places_count == 0)
        return false;
    quint32 rnd_place = QRandomGenerator::global()->bounded(places_count);
    quint32 rnd_item = QRandomGenerator::global()->bounded(10);
    //qDebug() << rnd_place+1 << rnd_item;
    uint added_item = 1;
    if (rnd_item == 9) added_item = 2;
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
            if (Board[i][j] == 0)
            {
                if (rnd_place == 0)
                {
                    Board[i][j]+=added_item;
                    return true;
                }
                else
                    rnd_place--;
            }
    return true;
}

QColor Game2048Gui::makeColor(uint number)
{
    if (number == 0)
    {
        return QColor(Qt::white);
    }
    else
    {
        QColor NumColor;
        NumColor.setAlpha(191);
        switch (BoardTheme) {
            case Red:
                {
                    NumColor.setGreen(number*256/(BoardSize*BoardSize) - 1);
                    NumColor.setRed(255-number*256/(BoardSize*BoardSize));
                    NumColor.setBlue(number*256/(BoardSize*BoardSize) - 1);
                    return NumColor;
                }
                break;
            case Green:
                {
                    NumColor.setGreen(255-number*256/(BoardSize*BoardSize));
                    NumColor.setRed(number*256/(BoardSize*BoardSize) - 1);
                    NumColor.setBlue(number*256/(BoardSize*BoardSize) - 1);
                    return NumColor;
                }
                break;
            case Blue:
                {
                    NumColor.setGreen(number*256/(BoardSize*BoardSize) - 1);
                    NumColor.setRed(number*256/(BoardSize*BoardSize) - 1);
                    NumColor.setBlue(255-number*256/(BoardSize*BoardSize));
                    return NumColor;
                }
                break;
            default:
                break;
        }
    }
}

void Game2048Gui::onCntrlR()
{
    if (BoardTheme == Red)
        BoardTheme = Green;
    else
        if (BoardTheme == Green)
            BoardTheme = Blue;
        else
            BoardTheme = Red;
    update();
}

void Game2048Gui::onCntrlS()
{
    if (PlayMode)
        finishGame();
    else
        emit pause();
}

void Game2048Gui::onSpeedUp()
{
    if (!PlayMode)
    {
        AnimationType = Game2048Gui::SpeedUp;
        AnimationToPaint = true;
        emit botSpeedChanged(true);
        update();
    }
}

void Game2048Gui::onSlowDown()
{
    if (!PlayMode)
    {
        AnimationType = Game2048Gui::SlowDown;
        AnimationToPaint = true;
        emit botSpeedChanged(false);
        update();
    }
}

void Game2048Gui::onAnimationEnd()
{
    AnimationToPaint = false;
    update();
}
