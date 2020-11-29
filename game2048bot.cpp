#include "game2048bot.h"

Game2048Bot::Game2048Bot(uint _size) : BoardSize(_size)
{

}

quint64 Game2048Bot::evaluationBoard(QVector<QVector<uint>> board, Game2048Bot::PlayAngle angle)
{
    uint evaluation = 0;
    // item + place evaluation
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
        {
            evaluation += evaluationItem(board[i][j]) * evaluationPlace(i,j,angle);
        }
    // item + relation evaluation
    for (uint i = 0; i < BoardSize; i++)
        for (uint j = 0; j < BoardSize; j++)
        {
            if (i < BoardSize-1 && board[i][j] == board[i+1][j])
                evaluation += evaluationItem(board[i][j]) * evaluationRelation();
            if (j < BoardSize-1 && board[i][j] == board[i][j+1])
                evaluation += evaluationItem(board[i][j]) * evaluationRelation();
        }
    return evaluation;
}

quint64 Game2048Bot::evaluationMove(QVector<QVector<uint>> board_after_move)
{
    quint64 max_evaluation = 0;
    for (uint i = 0; i < 4; i++)
    {
        quint64 evaluation = evaluationBoard(board_after_move, (Game2048Bot::PlayAngle)i);
        if (i == 0 || evaluation > max_evaluation)
            max_evaluation = evaluation;
    }
    return max_evaluation;
}

quint16  Game2048Bot::evaluationPlace(uint i, uint j, Game2048Bot::PlayAngle _angle)
{
    if (i < BoardSize && j < BoardSize)
    {
        uint Kh = 0;
        uint Kv = 0;
        switch(_angle) {
            case LeftTop:
            {
                Kh = BoardSize - i;
                Kv = BoardSize - j;
            }
            break;
            case RightTop:
            {
                Kh = i+1;
                Kv = BoardSize - j;
            }
            break;
            case RightBottom:
            {
                Kh = i+1;
                Kv = j+1;
            }
            break;
            case LeftBottom:
            {
                Kh = BoardSize - i;
                Kv = j+1;
            }
            break;
            default:
            break;
        }
        return (Kh+KPlace)*(Kv+KPlace);
    }
    else
        return 0;
}

quint32 Game2048Bot::evaluationItem(uint item)
{
    if (item == 0)
        return 0;
    else
        return pow(2,item+KItem);
}

uint Game2048Bot::evaluationRelation()
{
    return KRelation*BoardSize;
}

