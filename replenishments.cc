#include "replenishments.h"

#include <QTableWidget>
#include <ctime>
#include <map>

#include "ui_replenishments.h"

Replenishments::Replenishments(QWidget *parent)
    : QWidget(parent), ui(new Ui::Replenishments) {
    money_validator_.setRange(0, INT_MAX);
    ui->setupUi(this);
    ui->r_table->setColumnCount(2);
    ui->r_table->setHorizontalHeaderLabels(QStringList() << "date"
                                                         << "amount");
}

Replenishments::~Replenishments() { delete ui; }
// validates the data from user input for correctness
// creates row in the table, then creates two QTableWidgetItem
// and place date and sum into them. Then places these items into
// the new created row. Then insert the data into "repls_"
void Replenishments::on_r_add_clicked() {
    int not_used = 0;
    QString buff = ui->r_sum->text();
    size_t r_sum = 0;
    if (money_validator_.validate(buff, not_used) == QValidator::Acceptable) {
        rows_ += 1;
        ui->r_table->setRowCount(rows_);
        QDate next_date = ui->r_date->date();
        r_sum = buff.toInt();
        QTableWidgetItem *cell = new QTableWidgetItem;
        cell->setText(QString::number(r_sum));
        ui->r_table->setItem(rows_ - 1, 1, cell);
        cell = new QTableWidgetItem;
        cell->setText((next_date.toString("d.MMMM.yyyy")));
        ui->r_table->setItem(rows_ - 1, 0, cell);
        std::tm next{};
        next.tm_mday = next_date.day();
        next.tm_mon = next_date.month();
        next.tm_year = next_date.year();
        repls_.insert({std::mktime(&next), r_sum});
        ui->out_msg_3->setText(QString::number(repls_.size()));
    }
}

void Replenishments::on_r_reset_clicked() {
    repls_.clear();
    ui->r_table->clear();
    ui->r_table->setRowCount(0);
    rows_ = 0;
}
