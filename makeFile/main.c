#include <genesis.h>
#include "res/res.h"
#include "res/sprites.h"
#include "res/maps.h"
#include "res/palettes.h"


u16 cur_line = 0; //current line
f16 v_offset = 0; //shif of the plane

f16 v_scroll_step = FIX16(3); //step to increase the displacement of the v_offset

u8 hint_y = 160; // end of Hint
u8 hcount_y = 128; // begin of Hint

// Map
#define MAP_WIDTH				512
#define MAP_HEIGHT				512

// Camera
#define CAM_TOP					64
#define CAM_BOTTOM				(224 - 96)
#define CAM_LEFT				96
#define CAM_RIGHT				(320 - 128)

// Math
#define TILE_TO_PIXEL(x)		((x) << 5)
#define PIXEL_TO_TILE(x)		((x) >> 5)

// Physics
#define SPEED					2

// Functions
static void updateInput();
static void updatePhysics();
static void updateCamera();
static void updateSprites();
static void setCameraPosition(s16 x, s16 y);

// Player
Sprite* player;
s16 player_x;
s16 player_y;
s16 player_spd_x;
s16 player_spd_y;
u16 player_ptile_x;
u16 player_ptile_y;


// Camera
s16 cam_x;
s16 cam_y;
int map_y;
char map_char[6] = "a";

// Tile IDs
u16 tile_id;
u16 bg_tile_id;

// Maps
Map* bg_a;
Map* bg_b;

// Map Collision
const u16 MAP_COLLISION[256] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};


 HINTERRUPT_CALLBACK HIntHandler()
{
	
//	VDP_setVerticalScroll(BG_A, cur_line + fix16ToInt(v_offset));
//	v_scroll_step += FIX16(0.02); //Change the step, thereby, the speed of the plane - increases
//	v_offset -= v_scroll_step;
// palette colors change

//	PAL_setColor(20, 0x0640);
//	PAL_setColor(22, 0x0A60);
	// PAL_setColor(18, 0x0860);


	PAL_setColor(34, 0x0ECA);
	PAL_setColor(35, 0x0EA8);
	PAL_setColor(36, 0x0842);
	PAL_setColor(37, 0x0A64);
	PAL_setColor(38, 0x0EEC);
	

};

void VIntHandler()
{
//	cur_line += 1; //move the current line, each frame
//	v_scroll_step = FIX16(3.0); //Reset the step with which the plain moves.
//	v_offset = 0;
	
// return to original colors

//	PAL_setColor(20, 0x0220);
//	PAL_setColor(22, 0x0420);
//	PAL_setColor(18, 0x0240);


	PAL_setColor(34, 0x0AAE);
	PAL_setColor(35, 0x088E);
	PAL_setColor(36, 0x0228);
	PAL_setColor(37, 0x044A);
	PAL_setColor(38, 0x0EEE);

}
 void color_changer() {
static	int time;

	time++;
	if (time == 1)
	{
		PAL_setColor(50, 0x0862);
		PAL_setColor(51, 0x0840);
		PAL_setColor(54, 0x0A86);
	}
	if (time == 11)
	{
		PAL_setColor(54, 0x0862);
		PAL_setColor(50, 0x0840);
		PAL_setColor(51, 0x0A86);
		
	}
	if (time == 21)
	{
		PAL_setColor(54, 0x0840);
		PAL_setColor(50, 0x0A86);
		PAL_setColor(51, 0x0862);
		
	}
	if (time >= 31)
	{
		time = 0;
	}

}
int main() {
/// Palette
	u16 palette[64];
	
	// Screen Width
	VDP_setScreenWidth320();
	VDP_setTextPalette(PAL0);
	VDP_setTextPlane(WINDOW);
	// Init Sprite Engine
	SPR_init();
	

	// Default Palette
	PAL_setColors(0, (u16*)palette_black, 64, DMA);

	// Load Tileset
	tile_id = TILE_USER_INDEX;
	bg_tile_id = tile_id;
	VDP_loadTileSet(&ts_stage_01, bg_tile_id, DMA);
	tile_id += ts_stage_01.numTile;
	
	// Player Position
	player_x = 64;
	player_y = 64;

	// Camera Position
	cam_x = -1;
	cam_y = -1;

	// Init Maps
	bg_a = MAP_create(&map_stage_01, BG_A, bg_tile_id);
	bg_b = MAP_create(&map_stage_01_bg, BG_B, bg_tile_id);

	// Increase Buffer Size
	DMA_setBufferSize(9000);

	// Set Camera Position
	setCameraPosition(0, 0);

	// VBlank
	SYS_doVBlankProcess();

	// Reset Buffer Size
	DMA_setBufferSizeToDefault();


	// Player Sprite
	player = SPR_addSprite(&spr_player, player_x, player_y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
//	VDP_setWindowVPos(FALSE,1);
	// Update Sprites
	SPR_update();

	// Copy Palette
	memcpy(&palette[0], pal_stage_01a.data, 16 * 2);
	memcpy(&palette[16], pal_stage_01b.data, 16 * 2);
	memcpy(&palette[32], pal_player.data, 16 * 2);
	memcpy(&palette[48], pal_stage_02a.data, 16 * 2);

	
	
	
	// Fade In
	PAL_fadeIn(0, (4 * 16) - 1, palette, 60, FALSE);

	
	while (TRUE) {
		
		// Input
		
		VDP_clearTextArea(0,0,6,1);
		sprintf(map_char,"%d", map_y);
		VDP_drawText(map_char,0,0);
		color_changer();
	//	if (player_spd_y < 0)
		//{
		//	if (hcount_y > 0 && hcount_y <= 224)
		//	{
	//		hcount_y ++ ;
	//		}
	//	}
		
			
				{	SYS_disableInts();
				    
					VDP_setHIntCounter(hcount_y);
					
					VDP_setHInterrupt(hint_y);

					SYS_setHIntCallback(HIntHandler);

					PAL_setColor(34, 0x0ECA);
					PAL_setColor(35, 0x0EA8);
					PAL_setColor(36, 0x0842);
					PAL_setColor(37, 0x0A64);
					PAL_setColor(38, 0x0EEC);
					
					SYS_setVIntCallback(VIntHandler);

					PAL_setColor(34, 0x0AAE);
					PAL_setColor(35, 0x088E);
					PAL_setColor(36, 0x0228);
					PAL_setColor(37, 0x044A);
					PAL_setColor(38, 0x0EEE);
					
				}	SYS_enableInts();
			
				
		
		
		
		updateInput();

		// Physics
		updatePhysics();

		// Camera
		updateCamera();

		// Sprites
		updateSprites();

		// VBlank
		SYS_doVBlankProcess();
	}

	// Release Maps
	MEM_free(bg_a);
	MEM_free(bg_b);
		
	return 0;
}

static void updateInput()
{
	// Joypad Value
	u16 value = JOY_readJoypad(JOY_1);

	// Move Left/Right
	if (value & BUTTON_LEFT) { player_spd_x = -SPEED; }
	else if (value & BUTTON_RIGHT) { player_spd_x = SPEED; }
	else { player_spd_x = 0; }

	// Move Up/Down
	if (value & BUTTON_UP) { player_spd_y = -SPEED; }
	else if (value & BUTTON_DOWN) { player_spd_y = SPEED; }
	else { player_spd_y = 0; }

}

static void updatePhysics()
{
	// Tiles
	u16 tile_top, tile_bottom, tile_left, tile_right;

	// Edges
	u16 top_left, top_right, bottom_left, bottom_right;

	// Tile X/Y
	u16 tile_x, tile_y;

	
	

	// Up/Down
	if (player_spd_y < 0)
	{
		// Tiles
		tile_top = PIXEL_TO_TILE(player_y + player_spd_y);
		tile_left = PIXEL_TO_TILE(player_x + player_spd_x);
		tile_right = PIXEL_TO_TILE(player_x + player_spd_x + 32);

		// Collision
		top_left = MAP_COLLISION[tile_left + (tile_top << 4)];
		top_right = MAP_COLLISION[tile_right + (tile_top << 4)];
		// hcount_y - map_y;
		// Move
		if (top_left == 0 && top_right == 0) {
			player_y += player_spd_y;
			
			
			
			
			

						
							
					
						
						
						
					
					
				
			
					
				
				
				
			
			
			
			

			
			
			
			
		}
		else
		{
			tile_y = TILE_TO_PIXEL(tile_top) + 32;

			// Snap to Grid
			if (player_y > tile_y) { player_y = tile_y; }
			else
			{
				// Tiles
				tile_left = PIXEL_TO_TILE(player_x);
				tile_right = PIXEL_TO_TILE(player_x + 32 - 1);

				// Recheck Collision
				top_left = MAP_COLLISION[tile_left + (tile_top << 4)];
				top_right = MAP_COLLISION[tile_right + (tile_top << 4)];

				// Slide
				if (top_left == 0 && top_right == 0) { player_y += player_spd_y;}
			}
		}
	}
	else if (player_spd_y > 0)
	{
		
		// Tiles
		tile_bottom = PIXEL_TO_TILE(player_y + player_spd_y + 32);
		tile_left = PIXEL_TO_TILE(player_x + player_spd_x);
		tile_right = PIXEL_TO_TILE(player_x + player_spd_x + 32);

		// Collision
		bottom_left = MAP_COLLISION[tile_left + (tile_bottom << 4)];
		bottom_right = MAP_COLLISION[tile_right + (tile_bottom << 4)];

		// Move
		if (bottom_left == 0 && bottom_right == 0) {
			player_y += player_spd_y;
			
			
			

					
					
				
			
			
			
			
		}
		else
		{
			// Tile Y
			tile_y = TILE_TO_PIXEL(tile_bottom) - 32;

			// Snap to Grid
			if (player_y < tile_y) { player_y = tile_y; }
			else
			{
				// Tiles
				tile_left = PIXEL_TO_TILE(player_x);
				tile_right = PIXEL_TO_TILE(player_x + 32 - 1);

				// Recheck Collision
				bottom_left = MAP_COLLISION[tile_left + (tile_bottom << 4)];
				bottom_right = MAP_COLLISION[tile_right + (tile_bottom << 4)];

				// Slide
				if (bottom_left == 0 && bottom_right == 0) { player_y += player_spd_y; }
			}
		}
	}

	// Left/Right
	if (player_spd_x < 0)
	{
		// Tiles
		tile_top = PIXEL_TO_TILE(player_y + player_spd_y);
		tile_bottom = PIXEL_TO_TILE(player_y + player_spd_y + 32);
		tile_left = PIXEL_TO_TILE(player_x + player_spd_x);

		// Collision
		top_left = MAP_COLLISION[tile_left + (tile_top << 4)];
		bottom_left = MAP_COLLISION[tile_left + (tile_bottom << 4)];

		// Move
		if (top_left == 0 && bottom_left == 0) { player_x += player_spd_x; }
		else
		{
			// Tile X
			tile_x = TILE_TO_PIXEL(tile_left) + 32;

			// Snap to Grid
			if (player_x > tile_x) { player_x = tile_x; }
			else
			{
				// Tiles
				tile_top = PIXEL_TO_TILE(player_y);
				tile_bottom = PIXEL_TO_TILE(player_y + 32 - 1);

				// Recheck Collision
				top_left = MAP_COLLISION[tile_left + (tile_top << 4)];
				bottom_left = MAP_COLLISION[tile_left + (tile_bottom << 4)];

				// Slide
				if (top_left == 0 && bottom_left == 0) { player_x += player_spd_x; }
			}
		}
	}
	else if (player_spd_x > 0)
	{
		// Tiles
		tile_top = PIXEL_TO_TILE(player_y + player_spd_y);
		tile_bottom = PIXEL_TO_TILE(player_y + player_spd_y + 32);
		tile_right = PIXEL_TO_TILE(player_x + player_spd_x + 32);

		// Collision
		top_right = MAP_COLLISION[tile_right + (tile_top << 4)];
		bottom_right = MAP_COLLISION[tile_right + (tile_bottom << 4)];

		// Move
		if (top_right == 0 && bottom_right == 0) { player_x += player_spd_x; }
		else
		{
			// Tile X
			tile_x = TILE_TO_PIXEL(tile_right) - 32;

			// Snap to Grid
			if (player_x < tile_x) { player_x = tile_x; }
			else
			{
				// Tiles
				tile_top = PIXEL_TO_TILE(player_y);
				tile_bottom = PIXEL_TO_TILE(player_y + 32 - 1);

				// Recheck Collision
				top_right = MAP_COLLISION[tile_right + (tile_top << 4)];
				bottom_right = MAP_COLLISION[tile_right + (tile_bottom << 4)];

				// Slide
				if (top_right == 0 && bottom_right == 0) { player_x += player_spd_x; }
			}
		}
	}
}

static void updateCamera()
{

	// Player Relative Position
	s16 player_x_screen = player_x - cam_x;
	s16 player_y_screen = player_y - cam_y;

	// New Camera Position
	s16 ncam_x, ncam_y;

	// Update Camera Position
	if (player_x_screen < CAM_LEFT) { ncam_x = player_x - CAM_LEFT; }
	else if (player_x_screen > CAM_RIGHT) { ncam_x = player_x - CAM_RIGHT; }
	else { ncam_x = cam_x; }
	if (player_y_screen < CAM_TOP) { ncam_y = player_y - CAM_TOP; }
	else if (player_y_screen > CAM_BOTTOM) { ncam_y = player_y - CAM_BOTTOM; }
	else { ncam_y = cam_y; }

	// Camera Limits
	if (ncam_x < 0) { ncam_x = 0; }
	else if (ncam_x > (MAP_WIDTH - 320)) { ncam_x = (MAP_WIDTH - 320); }
	if (ncam_y < 0) { ncam_y = 0; }
	else if (ncam_y > (MAP_HEIGHT - 224)) { ncam_y = (MAP_HEIGHT - 224); }

	// Set Hint Position
	map_y = 128 - ncam_y;
	hint_y = 196 - ncam_y;

	
	if (player_y >= hint_y)
	{

		PAL_setColor(34, 0x0ECA);
		PAL_setColor(35, 0x0EA8);
		PAL_setColor(36, 0x0842);
		PAL_setColor(37, 0x0A64);
		PAL_setColor(38, 0x0EEC);

		VDP_setHInterrupt(0);

		

	}
	else
	{
		VDP_setHInterrupt(hint_y);
	}
//Set Camera Position
	if (map_y < 0)
	{
		map_y = 0;
		
	}
	hcount_y = map_y;
	setCameraPosition(ncam_x, ncam_y);
}

static void updateSprites()
{
	// Sprite Position
	SPR_setPosition(player, player_x - cam_x, player_y - cam_y);

	// Update Sprite Engine
	SPR_update();
}

static void setCameraPosition(s16 x, s16 y)
{
	// Set Camera Position
	if ((x != cam_x) || (y != cam_y))
	{
		// Set Position
		cam_x = x;
		cam_y = y;

		// Scroll Maps
		MAP_scrollTo(bg_a, x, y);
		MAP_scrollTo(bg_b, x, y );
	}
}