#include "odd_spin_box.h"

namespace medianFilter {

OddSpinBox::OddSpinBox(QWidget* parent) : QSpinBox(parent)
{
    setMinimum(1);
    setSingleStep(2);
}

int OddSpinBox::valueFromText(const QString &text) const {
    int value = text.toInt();
    if (value % 2 == 0) {
        --value;
    }
    value = qMax(value, minimum());
    value = qMin(value, maximum());

    return value;
}

} // namespace medianFilter