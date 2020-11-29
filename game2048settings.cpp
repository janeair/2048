#include "game2048settings.h"

Game2048Settings::Game2048Settings()
{
    setFixedSize(QSize(192+2*BackgroundWidth,192+2*BackgroundWidth));
    setWindowTitle("2048");

    ItemColorUpdateTimer = new QTimer(this);
    ItemColorUpdateTimer->setInterval(100);
    connect(ItemColorUpdateTimer, &QTimer::timeout, this, &Game2048Settings::updateColor);

    const QString font("font: 11pt Times;");
    BoardSize = new QComboBox(this);
    BoardSize->setStyleSheet(font);
    BoardSize->setEditable(false);
    BoardSize->setInsertPolicy(QComboBox::InsertAtBottom);
    BoardSize->setMinimumWidth(40);
    BoardSize->addItem("3x3");
    BoardSize->addItem("4x4");
    BoardSize->addItem("5x5");
    BoardSize->setCurrentIndex(1);

    BoardAmplifier = new QComboBox(this);
    BoardAmplifier->setStyleSheet(font);
    BoardAmplifier->setEditable(false);
    BoardAmplifier->setInsertPolicy(QComboBox::InsertAtBottom);
    BoardAmplifier->setMinimumWidth(40);
    BoardAmplifier->addItem("x2");
    BoardAmplifier->addItem("x3");
    BoardAmplifier->addItem("x4");
    BoardAmplifier->setCurrentIndex(0);

    Player = new QRadioButton("Myself", this);
    Player->setStyleSheet(font);
    Bot = new QRadioButton("That Guy", this);
    Bot->setStyleSheet(font);
    QVBoxLayout* v_layout = new QVBoxLayout;
    v_layout->addWidget(Player, Qt::AlignCenter);
    v_layout->addWidget(Bot, Qt::AlignCenter);
    QGroupBox* g_box = new QGroupBox();
    g_box->setLayout(v_layout);

    Start = new QPushButton("Start", this);
    Start->setStyleSheet(font);

    QGridLayout* grid_layout = new QGridLayout;
    grid_layout->addWidget(BoardSize, 0, 0, Qt::AlignRight);
    grid_layout->addWidget(BoardAmplifier, 0, 1, Qt::AlignLeft);
    grid_layout->addWidget(g_box, 1, 0, 2, 2, Qt::AlignCenter);
    grid_layout->addWidget(Start, 3, 0, 1, 2, Qt::AlignCenter);
    grid_layout->setHorizontalSpacing(15);
    grid_layout->setVerticalSpacing(10);
    setLayout(grid_layout);
    setFocus();

    ItemColorUpdateTimer->start();
}

void Game2048Settings::paintEvent(QPaintEvent* event)
{
    uint _itemcount = BoardSize->currentIndex()+3;
    uint _amp = BoardAmplifier->currentIndex()+2;
    uint _itemsize = 180/_itemcount;
    uint _itemgap = 12/(_itemcount-1);
    QPainter _painter(this);
    QPen _pen(QColor(ItemRed, ItemGreen, ItemBlue, 127));
    _pen.setCapStyle(Qt::RoundCap);
    _painter.setPen(_pen);
    _painter.setFont(QFont("Times", 10));
    //draw background
    _painter.fillRect(QRect(0, 0, _itemsize*_itemcount+_itemgap*(_itemcount-1)+2*BackgroundWidth, _itemsize*_itemcount+_itemgap*(_itemcount-1)+2*BackgroundWidth), Qt::white);
    //draw gridlines
    for (uint i = 0; i < _itemcount; i++)
        for (uint j = 0; j < _itemcount; j++)
        {
            QRect item(BackgroundWidth + i*(_itemsize + _itemgap), BackgroundWidth + j*(_itemsize + _itemgap), _itemsize, _itemsize);
            _painter.drawRect(item);
            _painter.drawText(item, Qt::AlignCenter, QString::number(pow((_amp), i+j+1)));
        }
}

void Game2048Settings::updateColor()
{
    if (ColorUpdateDirection)
    {
        if (ItemRed < 248)
            ItemRed+=8;
        else if (ItemGreen < 248)
            ItemGreen+=8;
        else if (ItemBlue < 248)
            ItemBlue+=8;
        else
            ColorUpdateDirection = false;
    }
    else
    {
        if (ItemRed > 7)
            ItemRed-=8;
        else if (ItemGreen > 7)
            ItemGreen-=8;
        else if (ItemBlue > 7)
            ItemBlue-=8;
        else
            ColorUpdateDirection = true;
    }
    if (this->isVisible()) update();
}
