/**
 * @file cell.cpp
 * @brief Implementation of \a cell.h
 * @author lhm
 */

#include "cell.h"

// Project's headers

// Qt headers
#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QtSvg>

// External headers

QSvgRenderer renderer(QString("imgs/queen.svg"));

/*****************************************************************************/
Cell::Cell(const size_t x, const size_t y, const size_t dim, QWidget* parent) noexcept
  : QWidget(parent)
  , _x{ x }
  , _y{ y }
  , _bg{ new QLabel(this) }
  , _lb{ new QLabel(this) }
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setUpdatesEnabled(false);

    auto pSize{ parent->size() };
    auto cSize{ QSize(pSize.width() / dim, pSize.height() / dim) };

    // Position
    move(_y * cSize.width(), _x * cSize.height());
    setFixedSize(cSize);

    // Setup label to contain value
    int lbMargin{ cSize.width() / 6 - 6 };
    int lbSize{ cSize.width() - 2 * lbMargin };
    _lb->setAlignment(Qt::AlignCenter);
    _lb->setObjectName("cellLabel");
    _lb->setFixedSize(lbSize, lbSize);
    _lb->move(lbMargin, lbMargin);
    setStyleSheet(
      QString("#cellLabel{border-radius:%1px;background-color:transparent;}").arg(lbSize / 2));

    // Setup background
    QString style(QString("background-color:%1;border:1px solid #000000;")
                    .arg(((x + y) % 2) ? "#C8D0DB" : "#E4E7EE"));
    _bg->setFixedSize(cSize);
    _bg->setStyleSheet(style);

    setUpdatesEnabled(true);
}

/*****************************************************************************/
void
Cell::enterEvent(QEvent*)
{
    setFocus(Qt::FocusReason::OtherFocusReason);

    auto lb_effect{ new QGraphicsDropShadowEffect() };

    // Setup hovering effect on labeld
    lb_effect->setOffset(.0);
    lb_effect->setBlurRadius(20.0);
    lb_effect->setColor(Qt::red);

    _lb->setGraphicsEffect(lb_effect);

    emit hovered(true);
}

/*****************************************************************************/
void
Cell::leaveEvent(QEvent*)
{
    _lb->setGraphicsEffect(nullptr);

    emit hovered(false);
}

/*****************************************************************************/
void
Cell::mouseReleaseEvent(QMouseEvent* e)
{
    if (Qt::LeftButton == e->button()) {
        _queen = !_queen;
    }

    emit changed(_queen);
    draw();
}

/*****************************************************************************/
void
Cell::draw() noexcept
{
    if (!_queen) {
        _lb->setPixmap(QPixmap());
        return;
    }

    QImage img = QImage(_lb->size(), QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter = QPainter(&img);

    renderer.render(&painter);
    _lb->setPixmap(QPixmap::fromImage(img));

    update();
}

/*****************************************************************************/
void
Cell::set(bool queen) noexcept
{
    if (_queen == queen)
        return;

    _queen = queen;
    draw();
}
