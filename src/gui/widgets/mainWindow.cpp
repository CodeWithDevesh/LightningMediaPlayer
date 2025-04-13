#include "mainWindow.hpp"

MainWindow::MainWindow(lighter::Core *core_inst) : QWidget(nullptr), core_inst{core_inst}
{
  player = core_inst->getPlayer();

  videoWidget = new VideoWidget(player, this);

  controlBar = new QWidget(this);
  controlBar->setFixedHeight(50);
  controlBar->setStyleSheet("background: rgba(0, 0, 0, 0.01);");

  // Control buttons
  QPushButton *playButton = new QPushButton("Play", controlBar);
  QPushButton *pauseButton = new QPushButton("Pause", controlBar);
  QSlider *slider = new QSlider(Qt::Horizontal, controlBar);

  // Layout for control bar
  QHBoxLayout *controlLayout = new QHBoxLayout(controlBar);
  controlLayout->addWidget(playButton);
  controlLayout->addWidget(pauseButton);
  controlLayout->addWidget(slider);
  controlLayout->setContentsMargins(5, 5, 5, 5);
  controlBar->setLayout(controlLayout);

  videoWidget->lower();

  resize(800, 450);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);

  // Resize video to fill the window
  videoWidget->setGeometry(0, 0, width(), height());

  // Place the control bar at the bottom
  controlBar->setGeometry(0, height() - controlBar->height(), width(), controlBar->height());
}

void MainWindow::show()
{
  QWidget::show();
  player->setXWindow(videoWidget->winId());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  // core_inst->stop();
  core_inst->cleanup();
}