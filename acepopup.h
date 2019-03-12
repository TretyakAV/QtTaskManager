#ifndef ACEPOPUP_H
#define ACEPOPUP_H

#include <QWidget>
#include <Windows.h>

namespace Ui {
class AcePopUp;
}

struct AceInfo
{
   QString Username;
   ACCESS_MASK AccessMask;
   BYTE AceType;
   DWORD dwStartingAceIndex;
};

class AcePopUp : public QWidget
{
    Q_OBJECT

public:
    explicit AcePopUp(QWidget *parent = nullptr);
    ~AcePopUp();

public slots:
    void TakeAceInformation();

signals:
    void SendAceInformation(AceInfo*);

private:
    Ui::AcePopUp *ui;
};

#endif // ACEPOPUP_H
