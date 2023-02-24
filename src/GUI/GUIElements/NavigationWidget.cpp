#include "NavigationWidget.h"
#include <Arduino.h>
#include "../../Display/Fonts/NotoSans16x24.h"
#include "../../KomootNavigation/KomootBLE.h"
#include "../../KomootNavigation/KomootImages.h"

int count = 0;

NavigationWidget::NavigationWidget(EPaperDisplay* display) : GUIElement(display)
{
    KomootBLE::Init();
}

const char* NavigationWidget::FormatDistance()
{
    if (KomootBLE::Distance == 0)
        return "Now!";
    else if (KomootBLE::Distance > 1000)
        return (*(new String(KomootBLE::Distance / 1000)) + "km").c_str();

    return (*(new String(KomootBLE::Distance)) + "m").c_str();
}

void NavigationWidget::Update()
{
    KomootBLE::Update();
}

void NavigationWidget::Draw()
{
    KomootBLE::DataMutex.lock();

    Display->DrawString(0, 220, String(KomootBLE::CurrentState).c_str(), &Font16);

    Display->DrawString(0, 200, FormatDistance(), &Font16);

    if (KomootBLE::Direction != 0)
        Display->DrawImage(14, 200, DirToImg[KomootBLE::Direction], ARROW_WIDTH, ARROW_HEIGHT);

    Display->DrawString(0, 260, KomootBLE::StreetName, &Font16);

    KomootBLE::DataMutex.unlock();
}