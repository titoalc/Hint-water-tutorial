#include <genesis.h>
#include "res/res.h"

u16 cur_line = 0; //current line
f16 v_offset = 0; //shif of the plane

f16 v_scroll_step = FIX16(3); //step to increase the displacement of the v_offset

HINTERRUPT_CALLBACK HIntHandler()
{
	VDP_setVerticalScroll(BG_A, cur_line + fix16ToInt(v_offset));
	v_scroll_step += FIX16(0.02); //Change the step, thereby, the speed of the plane - increases
	v_offset -= v_scroll_step;

};

void VIntHandler()
{
	cur_line += 1; //move the current line, each frame
	v_scroll_step = FIX16(3.0); //Reset the step with which the plain moves.
	v_offset = 0;
}

int main() {
	VDP_drawImage(BG_A,&img,0,0);
	
	PAL_setColors(0,palette_black,64,DMA);

	SYS_disableInts();
	{
		VDP_setHIntCounter(0);
		VDP_setHInterrupt(1);
		SYS_setHIntCallback(HIntHandler);
		
		SYS_setVIntCallback(VIntHandler);
	}
	SYS_enableInts();
	
	PAL_fadeIn(0, 15, img.palette->data, 32, FALSE);
	
	while (TRUE) {
		SYS_doVBlankProcess();
	}
	return;
}