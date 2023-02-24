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

void NavigationWidget::Update()
{
    KomootBLE::Update();
}

void NavigationWidget::Draw()
{
    if (KomootBLE::CurrentState == KomootBLE::CONNECTED)
    {
        KomootBLE::DataMutex.lock();

        String distStr = "";
        if (KomootBLE::Distance > 1000)
        {
            distStr.concat(String(KomootBLE::Distance / 1000.0f, 1));
            distStr.concat("km");
        }
        else 
        {
            distStr.concat(String(KomootBLE::Distance));
            distStr.concat("m");
        }

        Display->DrawString((Display->GetWidth() - distStr.length() * 16) / 2, 276, distStr.c_str(), &Font16);

        if (KomootBLE::Direction != 0)
            Display->DrawImage(20, 185, DirToImg[KomootBLE::Direction], ARROW_WIDTH, ARROW_HEIGHT);

        KomootBLE::DataMutex.unlock();
    }
    else if (KomootBLE::CurrentState == KomootBLE::WAITING_FOR_CONNECTION)
    {                               
        Display->DrawString(0, 213, "Connnec-", &Font16);
        Display->DrawString(0, 237, "ting", &Font16);
    }
    else
    {
        Display->DrawString(0, 213, "Not con-", &Font16);
        Display->DrawString(0, 237, "nected", &Font16);
    }
}