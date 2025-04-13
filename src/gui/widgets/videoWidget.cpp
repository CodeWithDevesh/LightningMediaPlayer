#include "videoWidget.hpp"

VideoWidget::VideoWidget(lighter::Player *player, QWidget *parent) : QWidget(parent), player{player}
{
}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
  if (player)
  {
    if (player->isPlaying())
      player->pause();
    else
      player->play();
  }
}