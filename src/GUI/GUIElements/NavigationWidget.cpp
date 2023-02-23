#include "NavigationWidget.h"
#include "../../Display/Fonts/NotoSans16x24.h"
#include "../../KomootNavigation/KomootBLE.h"

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
    KomootBLE::DataMutex.lock();
    Display->DrawString(0, 200, String(KomootBLE::Distance).c_str(), &Font16);
    KomootBLE::DataMutex.unlock();
}