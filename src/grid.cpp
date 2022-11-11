/**
 * @file grid.cpp
 * @brief Implementation of \a grid.h
 * @author lhm
 */

// Project's headers
#include "grid.h"
#include "cell.h"

// Standard headers
#include <set>

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
        connect(c, &Cell::changed, this, [this](bool) {
            if (_traces)
                _updateTraces();
            emit changed();
        });
    }

    return true;
}

/*****************************************************************************/
void
Grid::activateTraces(bool activate) noexcept
{
    _traces = activate;

    if (_traces)
        _updateTraces();
    else {
        setUpdatesEnabled(false);
        for (auto& cell : _cells) {
            cell->setTrace(false);
        }
        setUpdatesEnabled(true);
    }
}

/*****************************************************************************/
void
Grid::_updateTraces() noexcept
{
    std::set<int> indexes;

    int N{ static_cast<int>(_size) };
    for (auto i{ 0 }, k{ 0 }; i < N; ++i) {
        for (auto j{ 0 }; j < N; ++j, ++k) {
            if (!_cells[k]->get())
                continue;
            for (int l{ 0 }; l < N; ++l) {
                if ((i + l) < N && (j + l) < N)
                    indexes.insert((i + l) * N + (j + l));
                if ((i + l) < N && (j - l) >= 0)
                    indexes.insert((i + l) * N + (j - l));
                if ((i - l) >= 0 && (j + l) < N)
                    indexes.insert((i - l) * N + (j + l));
                if ((i - l) >= 0 && (j - l) >= 0)
                    indexes.insert((i - l) * N + (j - l));

                indexes.insert(i * N + l);
                indexes.insert(l * N + j);
            }
        }
    }

    setUpdatesEnabled(false);
    for (size_t i{ 0 }, k{ 0 }; i < _size; ++i)
        for (size_t j{ 0 }; j < _size; ++j, ++k)
            _cells[k]->setTrace(indexes.count(k));
    setUpdatesEnabled(true);
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
    for (auto c : _cells) {
        c->setTrace(false);
        c->set(false);
    }

    emit changed();
}
