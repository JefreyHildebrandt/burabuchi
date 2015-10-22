#pragma once
#include "PetSprite.h"
#include "Stats.h"

#define BUTTONS_NUM  6
#define PEBBLE_HEIGHT 168 
#define PEBBLE_WIDTH 144

#ifndef   BUTTON_H
#define   BUTTON_H
typedef enum button_type{
  HUNGER,
  FLUSH,
  EXERCISE,
  MEDICINE,
  SLEEP,
  SETTINGS,
  EVOLVE,
  SECRET
} button_type;
#endif


typedef struct PetSprite_ PetSprite;

typedef struct Buttons_ Buttons;

Buttons* buttons_create_all();

void buttons_draw_buttons(Buttons *buttons, GContext *ctx);
  
void buttons_destroy_all(Buttons *buttons);

GRect buttons_get_sprite_layer_bounds(Buttons *buttons);

//changes new button to button_highlight and old button back to original
void buttons_highlight(Buttons *buttons, button_type highlight, GBitmap *button, GContext *ctx);

//returns all available space for pet sprite layer
GPoint buttons_get_pet_layer_start_point(Buttons *buttons);

void buttons_press(button_type pressed_type, PetSprite *pet);