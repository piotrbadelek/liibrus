#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;
bool hasRefreshedForFree = false;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");


	printf("Liibrus na wii moment!");
	
	printf("\n  Eluwina, jesieniara i alternatywka, uzywacie tych slow? \nJesli nie funkcjonuja one w Waszych slownikach, to na pewno znaja je Wasze dzieci lub mlodsi znajomi.\n Dowodem ich popularnosci sa wyniki czwartej edycji plebiscytu na Mlodziezowe Slowo Roku.\n Wydarzenie organizowane kazdego roku przez Wydawnictwo Naukowe PWN pozwala na wglad w najswiezsze slowotworcze trendy panujace wsrod mlodych ludzi. Przyjrzyjmy sie zeszlorocznym wynikom. ");
	
	
	while(1) {

		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) exit(0);

		if( pressed & WPAD_BUTTON_A ) {
			if (hasRefreshedForFree == true) {
				printf("\n Odswiezanie jest funkcja platna. Kup Librusa na Wii w Wii Shop Channel za jedynie 1000 Punktow.\n");		
			} else {
				hasRefreshedForFree = true;
				printf("\n Odswiezono!.\n");		
			}
		}

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}