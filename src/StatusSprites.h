#pragma once
#include "Stats.h"
  
typedef struct StatusSprites_ StatusSprites;

typedef struct Sprites_ Sprites;

StatusSprites* status_sprites_create();

void status_sprites_draw(StatusSprites* stat_sprites, GContext *ctx, bool update_sprite, PetSprite *pet);

void status_sprites_destroy(StatusSprites *stat_sprites);

Sprites* status_sprites_get_poop(StatusSprites* stat_sprites, int poop_num);

GBitmap* status_sprites_get_bitmap(Sprites* sprites);

GBitmap* status_sprites_get_hunger_bitmap(StatusSprites* stat_sprites, bool one_or_two);

GPoint status_sprites_get_position(Sprites* sprites);

GPoint status_sprites_get_poop_position(StatusSprites* stat_sprites, int num);