/**
 * @file mainwindow.cpp
 * @brief Implementation of \a mainwindow.h
 * @author lhm
 */

// Project's headers
#include "mainwindow.h"
#include "./ui_mainwindow.h"

// External headers
#include <ecv.hpp>

// Standard headers
#include <chrono>
#include <random>

// Qt headers
#include <QGraphicsDropShadowEffect>
#include <QIntValidator>

static auto _sols{ ecv::NQueens::generate(ecv::NQueens::make_empty_state(2))->solve(1) };
static std::unique_ptr<ecv::NQueens> _model{ nullptr };

/*****************************************************************************/
void
setupWidget(QWidget* w, const QString& name = "setupWidget")
{
    QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect;
    shadow_effect->setOffset(1, 1);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(2);

    w->setObjectName(name);
    w->setGraphicsEffect(shadow_effect);
}

/*****************************************************************************/
/*!
 * @brief Very basic stopwatch to measure ellapsed time since it has been created
 */
template<class Clock = std::chrono::high_resolution_clock, class Unit = std::chrono::milliseconds>
class stopwatch
{
public:
    explicit stopwatch() noexcept
      : _start{ Clock::now() }
    {}

    Unit elapsed() const noexcept
    {
        return std::chrono::duration_cast<Unit>(Clock::now() - _start);
    }

private:
    typedef std::chrono::time_point<Clock> TimePt;
    const TimePt                           _start;
};

/*****************************************************************************/
class CLineEdit : public QWidget
{
public:
    explicit CLineEdit(QLineEdit* le) noexcept
      : QWidget(le)
      , _le{ le }
      , _txt{ _le->text() }
    {
        _le->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject* obj, QEvent* e) override
    {
        if (obj == _le) {
            if (QEvent::FocusIn == e->type()) {
                _txt = _le->text();
                _le->clear();
            } else if (QEvent::FocusOut == e->type() && _le->text().isEmpty()) {
                _le->setText(_txt);
            }
        }
        return false;
    }

private:
    QLineEdit* _le;
    QString    _txt;
};

/*****************************************************************************/
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->res_gb->setEnabled(false);

    ui->le_sols->setValidator(new QIntValidator(1, 1000000, this));
    new CLineEdit(ui->le_sols);

    for (auto& button : { ui->new_pb, ui->solve_pb, ui->res_pb })
        setupWidget(button);

    setupWidget(ui->le_sols);
    setupWidget(ui->cb_dims);

    setStyleSheet("QLineEdit#setupWidget{background-color:#ffffff;color:#5F5F5F;}"
                  "QLineEdit#setupWidget:hover{background-color:#daffb3;}"
                  "QPushButton#setupWidget{background-color:#ffffff;color:#5F5F5F;}"
                  "QPushButton#setupWidget:hover{background-color:#daffb3;}"
                  "QPushButton#setupWidget:pressed{background-color:#5cb300;}"
                  "QPushButton#setupWidget:!enabled{background-color:rgb(200, 200, 200);}"
                  "QComboBox#setupWidget{background-color:#ffffff;color:#5F5F5F;}"
                  "QComboBox#setupWidget:hover{background-color:#daffb3;}"
                  "QComboBox#setupWidget:pressed{background-color:#5cb300;}"
                  "QComboBox#setupWidget:!enabled{background-color:rgb(200, 200, 200);}");

    for (auto& button : { ui->cb_dims })
        button->setStyleSheet(
          QString("border-top-right-radius:%1px;border-bottom-right-radius:%1px;").arg(3));

    // TODO - Connect to grid changes

    // TODO - Connect buttons with Grid
}

/*****************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}
