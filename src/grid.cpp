/**
 * @file grid.cpp
 * @brief Implementation of \a grid.h
 * @author lhm
 */

// Project's headers
#include "grid.h"
#include "cell.h"

// External headers
#include <ecv.hpp>

/*****************************************************************************/
Grid::Grid(QWidget* parent) noexcept
  : QWidget(parent)
{
    setFixedSize(500, 500);

    resize(_size);
}

/*****************************************************************************/
bool
Grid::resize(const size_t& size) noexcept
{
    clear();

    _size = size;
    for (auto c : _cells)
        delete (c);

    _cells.clear();
    _cells.reserve(_size * _size);

    for (size_t i{ 0 }; i < _size; ++i)
        for (size_t j{ 0 }; j < _size; ++j)
            _cells.emplace_back(new Cell(i, j, size, this));

    for (auto& c : _cells) {
        c->show();
        connect(c, &Cell::hovered, this, []() { /* TODO */ });
        connect(c, &Cell::changed, this, [this]() { /* TODO */
                                                    emit changed();
        });
    }

    return true;
}

/*****************************************************************************/
std::vector<std::string>
Grid::data() const noexcept
{
    auto ret{ std::vector<std::string>(_size, std::string(_size, ' ')) };

    for (size_t i{ 0 }, k{ 0 }; i < _size; ++i)
        for (size_t j{ 0 }; j < _size; ++j, ++k)
            ret[i][j] = _cells[k]->get() ? '1' : '0';

    return ret;
}

/*****************************************************************************/
bool
Grid::fromData(const std::vector<std::string>& data) noexcept
{
    if (std::empty(data) || _size != std::size(data))
        return false;

    for (const auto& line : data)
        if (_size != std::size(line))
            return false;

    for (size_t i{ 0 }, k{ 0 }; i < _size; ++i)
        for (size_t j{ 0 }; j < _size; ++j, ++k)
            _cells[k]->set(data[i][j] != '0');

    return true;
}

/*****************************************************************************/
void
Grid::clear(void) noexcept
{
    for (auto c : _cells)
        c->set(false);

    emit changed();
}
