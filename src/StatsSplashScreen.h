#pragma once
#include "Buttons.h"
#include "Stats.h"
#include "PetSprite.h"
#include "StatusSprites.h"

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
  
typedef struct StatusSprites_ StatusSprites;

void stats_splash_screen_display(button_type type, PetSprite* pet_sprite, StatusSprites *stat_sprites, int evolve_num);