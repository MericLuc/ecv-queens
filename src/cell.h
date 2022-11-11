#ifndef CELL_H
#define CELL_H

#include <QSvgRenderer>
#include <QWidget>

class QLabel;
class QImage;
class QPainter;

class Cell : public QWidget
{
    Q_OBJECT
    friend class Grid;

public:
    Cell(const size_t x, const size_t y, const size_t dim = 9, QWidget* parent = nullptr) noexcept;
    virtual ~Cell() noexcept = default;

    void set(bool queen) noexcept;
    bool get(void) const noexcept { return _queen; }

signals:
    void changed(bool);
    void hovered(bool);

protected:
    void enterEvent(QEvent*) override;
    void leaveEvent(QEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

    void updateVal(size_t) noexcept;
    void draw() noexcept;
    void setTrace(bool) noexcept;

private:
    const size_t _x, _y;
    bool         _queen{ false };

    // UI related members
    QLabel* _bg{ nullptr };
    QLabel* _lb{ nullptr };
    QLabel* _highlight{ nullptr };

    static QSvgRenderer renderer;
};

#endif // CELL_H
