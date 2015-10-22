#include <pebble.h>
#include "StatusSprites.h"

struct Sprites_
{
  GBitmap *sprite;
  GBitmap *sprite_two;
  GPoint pos;
  bool state;
};

typedef struct Sprite_
{
  GBitmap *sprite;
  GPoint pos;
} Sprite;

struct StatusSprites_
{
  Sprites *poop[2];
  GPoint poop_locations[5];
  Sprite *sick;
  Sprites *sleep;
  Sprites *hunger;
  Sprite *halo;
};

void destroy_sprites(Sprites* sprites);
void destroy_sprite(Sprite* sprite);

StatusSprites* status_sprites_create()
{
  StatusSprites *stat_sprites = malloc(sizeof(StatusSprites));
  int is_dead = stats_is_dead();
  if(!is_dead)
  {
    for(int i=0; i<2; i++)
    {
      stat_sprites->poop[i] = malloc(sizeof(Sprites));
      stat_sprites->poop[i]->sprite = gbitmap_create_with_resource(RESOURCE_ID_POOP_ONE);
      stat_sprites->poop[i]->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_POOP_TWO);
    }
  }
  else
  {
    for(int i=0; i<2; i++)
    {
      stat_sprites->poop[i] = malloc(sizeof(Sprites));
      stat_sprites->poop[i]->sprite = gbitmap_create_with_resource(RESOURCE_ID_GHOST_POOP_ONE);
      stat_sprites->poop[i]->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_GHOST_POOP_TWO);
    }
  }
  
  for(int i=0; i<5; i++)
  {
    int x1 = rand()%114;
    int y1 = rand()%72;

    if(y1 < 30)
    {
      y1 += 10;
    }
    stat_sprites->poop_locations[i] = GPoint(x1, y1);
  }
  
  stat_sprites->sick = malloc(sizeof(Sprite));
  stat_sprites->sick->sprite = gbitmap_create_with_resource(RESOURCE_ID_SICKNESS);
  stat_sprites->sick->pos = GPoint(0,0);
  
  stat_sprites->halo = malloc(sizeof(Sprite));
  stat_sprites->halo->sprite = gbitmap_create_with_resource(RESOURCE_ID_HALO);
  stat_sprites->halo->pos = GPoint(0,0);
  
  stat_sprites->sleep = malloc(sizeof(Sprites));
  stat_sprites->sleep->sprite = gbitmap_create_with_resource(RESOURCE_ID_SLEEP_ONE);
  stat_sprites->sleep->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_SLEEP_TWO);
  stat_sprites->sleep->pos = GPoint(0,0);
  stat_sprites->sleep->state = true;
  
  stat_sprites->hunger = malloc(sizeof(Sprites));
  stat_sprites->hunger->sprite = gbitmap_create_with_resource(RESOURCE_ID_HUNGER_ONE);
  stat_sprites->hunger->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_HUNGER_TWO);
  stat_sprites->hunger->pos = GPoint(0,0);
  stat_sprites->hunger->state = true;
  
  return stat_sprites;
}

void status_sprites_draw(StatusSprites* stat_sprites, GContext *ctx, bool update_sprite, PetSprite *pet)
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "HERE");
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  if(stats_is_asleep())
  {
    if(stat_sprites->sleep->state)
    {
      graphics_draw_bitmap_in_rect(ctx, stat_sprites->sleep->sprite, GRect(stat_sprites->sleep->pos.x, stat_sprites->sleep->pos.y, gbitmap_get_bounds(stat_sprites->sleep->sprite).size.w, gbitmap_get_bounds(stat_sprites->sleep->sprite).size.h));
    }
    else
    {
      graphics_draw_bitmap_in_rect(ctx, stat_sprites->sleep->sprite_two, GRect(stat_sprites->sleep->pos.x, stat_sprites->sleep->pos.y, gbitmap_get_bounds(stat_sprites->sleep->sprite_two).size.w, gbitmap_get_bounds(stat_sprites->sleep->sprite_two).size.h));
    }
    stat_sprites->sleep->state = !stat_sprites->sleep->state;
    return;
  }
  
  int poop_num = -1*(stats_hygeine_level()-STAT_FIELD_MAX);
  for(int i=0; i<poop_num; i++)
  {
    int state = rand()%2;
    if(state == 0)
    {
      graphics_draw_bitmap_in_rect(ctx, stat_sprites->poop[state]->sprite, GRect(stat_sprites->poop_locations[i].x, stat_sprites->poop_locations[i].y, gbitmap_get_bounds(stat_sprites->poop[state]->sprite).size.w, gbitmap_get_bounds(stat_sprites->poop[state]->sprite).size.h));
    }
    else
    {
      graphics_draw_bitmap_in_rect(ctx, stat_sprites->poop[state]->sprite_two, GRect(stat_sprites->poop_locations[i].x, stat_sprites->poop_locations[i].y, gbitmap_get_bounds(stat_sprites->poop[state]->sprite_two).size.w, gbitmap_get_bounds(stat_sprites->poop[state]->sprite_two).size.h));
    }
    stat_sprites->poop[state]->state = !stat_sprites->poop[state]->state;
  }

  if(stats_is_sick())
  {
    graphics_draw_bitmap_in_rect(ctx, stat_sprites->sick->sprite, 
                                 GRect(((pet_sprite_get_position(pet).x + pet_sprite_get_bounds(pet).size.w/2) - (gbitmap_get_bounds(stat_sprites->sick->sprite).size.w/2)), 
                                       pet_sprite_get_position(pet).y, 
                                       gbitmap_get_bounds(stat_sprites->sick->sprite).size.w, 
                                       gbitmap_get_bounds(stat_sprites->sick->sprite).size.h));
  }
  
  if(stats_is_dead())
  {
    graphics_draw_bitmap_in_rect(ctx, stat_sprites->halo->sprite, 
                                 GRect(((pet_sprite_get_position(pet).x + pet_sprite_get_bounds(pet).size.w/2) - (gbitmap_get_bounds(stat_sprites->halo->sprite).size.w/2)), 
                                       pet_sprite_get_position(pet).y - 20, 
                                       gbitmap_get_bounds(stat_sprites->halo->sprite).size.w, 
                                       gbitmap_get_bounds(stat_sprites->halo->sprite).size.h));
  }
  
  if(stats_hunger_level() < 5)
  {
    if(stat_sprites->hunger->state)
    {
      graphics_draw_bitmap_in_rect(ctx, stat_sprites->hunger->sprite, 
                                 GRect((pet_sprite_get_position(pet).x + pet_sprite_get_bounds(pet).size.w/2), 
                                       pet_sprite_get_position(pet).y + pet_sprite_get_bounds(pet).size.h/2, 
                                       gbitmap_get_bounds(stat_sprites->hunger->sprite).size.w, 
                                       gbitmap_get_bounds(stat_sprites->hunger->sprite).size.h));
    }
    else
    {
      graphics_draw_bitmap_in_rect(ctx, stat_sprites->hunger->sprite_two, 
                                 GRect((pet_sprite_get_position(pet).x + pet_sprite_get_bounds(pet).size.w/2), 
                                       pet_sprite_get_position(pet).y + pet_sprite_get_bounds(pet).size.h/2, 
                                       gbitmap_get_bounds(stat_sprites->hunger->sprite_two).size.w, 
                                       gbitmap_get_bounds(stat_sprites->hunger->sprite_two).size.h));
    }
    stat_sprites->hunger->state = !stat_sprites->hunger->state;
  }
}

Sprites* status_sprites_get_poop(StatusSprites* stat_sprites, int poop_num)
{
  int r = rand()%2;
  return stat_sprites->poop[r];
}

GBitmap* status_sprites_get_bitmap(Sprites* sprites)
{
  int r = rand()%2;
  if(r == 0)
  {
    return sprites->sprite;
  }
  else
  {
    return sprites->sprite_two;
  }
}

GBitmap* status_sprites_get_hunger_bitmap(StatusSprites* stat_sprites, bool one_or_two)
{
  if(one_or_two)
  {
    return stat_sprites->hunger->sprite;
  }
  else
  {
    return stat_sprites->hunger->sprite_two;
  }
}

GPoint status_sprites_get_poop_position(StatusSprites* stat_sprites, int num)
{
  return stat_sprites->poop_locations[num];
}

GPoint status_sprites_get_position(Sprites* sprites)
{
  return sprites->pos;
}

void status_sprites_destroy(StatusSprites *stat_sprites)
{
  for(int i=0; i<2; i++)
  {
    destroy_sprites(stat_sprites->poop[i]);
  }
  destroy_sprite(stat_sprites->sick);
  destroy_sprites(stat_sprites->sleep);
  destroy_sprites(stat_sprites->hunger);
  free(stat_sprites);
}

void destroy_sprites(Sprites* sprites)
{
  gbitmap_destroy(sprites->sprite);
  gbitmap_destroy(sprites->sprite_two);
  free(sprites);
}

void destroy_sprite(Sprite* sprite)
{
  gbitmap_destroy(sprite->sprite);
  free(sprite);
}