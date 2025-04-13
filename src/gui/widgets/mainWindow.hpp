#pragma once

#include <QtWidgets>
#include "core/lightning_core.hpp"
#include "videoWidget.hpp"

class MainWindow : public QWidget
{
  Q_OBJECT;

public:
  MainWindow(lighter::Core *core_inst);
  void show();

protected:
  void closeEvent(QCloseEvent *event);
  void resizeEvent(QResizeEvent *event);

private:
  lighter::Core *core_inst;
  QHBoxLayout *controlLayout;
  QVBoxLayout *mainLayout;

  VideoWidget *videoWidget;
  QWidget *controlBar;

  lighter::Player *player;
};