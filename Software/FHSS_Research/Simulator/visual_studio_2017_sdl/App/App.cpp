#include "App.h"
#include "Page/Page.h"

void App_Create(void)
{
    StatusBar_Create();
    
    lv_obj_t* scr = Page_ComSlaveTest_Create();
    lv_scr_load(scr);
}
