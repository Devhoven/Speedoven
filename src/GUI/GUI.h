#include "../Display/EPaperDisplay.h"

class GUI
{
private:
    EPaperDisplay Display;

public: 
    GUI(EPaperDisplay display);

    void Update();

    void Draw();
};