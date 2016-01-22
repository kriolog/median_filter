#ifndef ODD_SPIN_BOX_H
#define ODD_SPIN_BOX_H

#include <QSpinBox>

namespace medianFilter {

class OddSpinBox : public QSpinBox
{
     Q_OBJECT

public:
     OddSpinBox(QWidget* parent = 0);

protected:
    int valueFromText(const QString &text) const;
};

} // namespace medianFilter

#endif // ODD_SPIN_BOX_H