#ifndef SAVEOPTIONSTRING_H
#define SAVEOPTIONSTRING_H

#include <QWidget>

namespace Ui {
class SaveOptionString;
}

class SaveOptionString : public QWidget
{
    Q_OBJECT

public:
    explicit SaveOptionString(QWidget *parent = nullptr);
    ~SaveOptionString();

private:
    Ui::SaveOptionString *ui;
};

#endif // SAVEOPTIONSTRING_H
