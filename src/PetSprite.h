#pragma once
#include "Stats.h"
#include "Buttons.h"
#include "StatsSplashScreen.h"
  
typedef enum pet_sprite
{
  STARTER,
  MID_BAD,
  MID_MID,
  MID_GOOD,
  END_BAD_BAD,
  END_BAD_MID,
  END_BAD_GOOD,
  END_MID_BAD,
  END_MID_MID,
  END_MID_GOOD,
  END_GOOD_BAD,
  END_GOOD_MID,
  END_GOOD_GOOD,
  DEAD_BAD,
  DEAD_GOOD
} pet_sprite;

typedef struct PetSprite_ PetSprite;
typedef struct StatusSprites_ StatusSprites;

/**
 * Create a sprite object
 */
PetSprite* pet_sprite_create();

pet_sprite pet_sprite_get_pet_type(PetSprite* pet);

void pet_sprite_destroy(PetSprite* pet);

/**
 * Destroy a sprite object
 */
void destroy_pet_sprite();

//call this to evolve the pet
void pet_sprite_evolve_pet(pet_sprite new_evolution, PetSprite *pet,  StatusSprites *stat_sprites, int evolve_num);

void pet_sprite_set_and_display_sprites(pet_sprite pet_type, PetSprite *pet, StatusSprites *stat_sprites, bool display);

GPoint pet_sprite_get_position(PetSprite* pet);

GRect pet_sprite_get_bounds(PetSprite* pet);

GBitmap* pet_sprite_get_sprite_one(PetSprite* pet);

GBitmap* pet_sprite_get_sprite_two(PetSprite* pet);

void evolve_pet(pet_sprite new_evolution);

/**
 * Draw the sprite's bitmap to the graphics context
 */
void pet_sprite_draw(PetSprite* pet, GContext *ctx, bool update_sprite);

