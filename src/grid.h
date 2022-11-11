#ifndef GRID_H
#define GRID_H

#include <QWidget>

#include <stack>

class Cell;

/*!
 * \brief The Grid class exposes a N x N ChessBoard for N Queens problem
 */
class Grid : public QWidget
{
    Q_OBJECT

public:
    explicit Grid(QWidget* parent = nullptr) noexcept;
    virtual ~Grid() noexcept = default;

    /*!
     * \brief data Get the string representation of the grid
     */
    std::vector<std::string> data() const noexcept;

    /*!
     * \brief fromData fills the grid using custom data
     * \param data the data to use
     */
    [[maybe_unused]] bool fromData(const std::vector<std::string>& data) noexcept;

signals:
    /*!
     * \brief changed emitted when the grid changes
     */
    void changed();

public slots:

    [[maybe_unused]] bool resize(const size_t& size) noexcept;
    void                  clear(void) noexcept;

private:
    std::vector<Cell*> _cells; /*!< Cells of the Grid */
    size_t             _size{ 8 };
};

#endif // GRID_H
