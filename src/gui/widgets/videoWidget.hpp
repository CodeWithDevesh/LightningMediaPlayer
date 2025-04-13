#pragma once

#include <QtWidgets>
#include "core/lightning_core.hpp"

class VideoWidget : public QWidget
{

  Q_OBJECT;

public:
  VideoWidget(lighter::Player *player, QWidget *parent = nullptr);

  void mousePressEvent(QMouseEvent *event);

private:
  lighter::Player *player = nullptr;
};