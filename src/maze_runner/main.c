/* Simple skeletontest program for in-robot API */

#include "allcode_api.h"

   
int main()
{
	FA_RobotInit();
    
    FA_LCDBacklight(50);

    while(1)
    {   
        /*

        // Uncomment this code to activate motor functions

        // Go forward
        FA_SetMotors(20, 20);
      
        if(FA_ReadIR(1) > 600)  // check front left ir
        {
            FA_Backwards(50);
            FA_Right(30);      
        }

        if(FA_ReadIR(2) > 600)  // check front ir
        {
            FA_Backwards(50);
        }

        if(FA_ReadIR(3) > 600)  // check front right ir
        {
            FA_Backwards(50);
            FA_Left(30);      
        }

        */

	FA_LCDClear();
        FA_LCDPrint("Henlo", 5, 20,25,FONT_NORMAL, LCD_OPAQUE);

        
        if(FA_ReadSwitch(0) == 1)
            FA_PlayNote(1200,200);
        if(FA_ReadSwitch(1) == 1)
            FA_PlayNote(2200,200);
        
		FA_DelayMillis(100);
        
        int i;
        for (i = 0; i < 8; i++) {
            if (FA_ReadIR(i) > 600) {
                FA_LEDOn(i);
            }
            else {
                FA_LEDOff(i);
            }
        }
    }            
 	return 0;
}



