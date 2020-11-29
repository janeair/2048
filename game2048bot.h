#ifndef GAME2048BOT_H
#define GAME2048BOT_H

#include <QObject>
#include <QDebug>
#include <cmath>

class Game2048Bot : public QObject
{
    Q_OBJECT

public:
    enum PlayAngle { LeftTop, RightTop, RightBottom, LeftBottom };
    explicit Game2048Bot(uint _size);
    quint64 evaluationMove(QVector<QVector<uint>> board_after_move);

private:
    const uint BoardSize;

    uint KItem = 4;
    uint KPlace = 1;
    uint KRelation = 3;

    quint64 evaluationBoard(QVector<QVector<uint>> board, PlayAngle angle);
    quint16 evaluationPlace(uint i, uint j, PlayAngle _angle);
    quint32 evaluationItem(uint item);
    uint evaluationRelation();

signals:

};

#endif // GAME2048BOT_H
