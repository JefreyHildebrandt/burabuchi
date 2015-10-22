#include <pebble.h>
#include "PetSprite.h"

struct PetSprite_
{
  pet_sprite type;
  GBitmap *sprite_one;
  GBitmap *sprite_two;
  GPoint position;
  bool which_sprite;
};

void pet_sprite_set_cur_pet(pet_sprite cur_pet, PetSprite *pet);

PetSprite* pet_sprite_create()
{
  pet_sprite cur_pet = stats_get_current_pet_sprite();
  
  PetSprite* pet = malloc(sizeof (PetSprite));
  
  pet_sprite_set_cur_pet(cur_pet, pet);
  
  int x1 = rand()%(PEBBLE_WIDTH - gbitmap_get_bounds(pet->sprite_one).size.w);
  int y1 = rand()%(PEBBLE_HEIGHT - gbitmap_get_bounds(pet->sprite_one).size.h-27-27);
  
  pet->position = GPoint(x1,y1);
      
  pet->which_sprite = true;
  
  return pet;
}

void pet_sprite_set_cur_pet(pet_sprite cur_pet, PetSprite *pet)
{
  pet->type = cur_pet;
  
  switch (cur_pet) 
  {
    case STARTER:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_STARTER_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_STARTER_TWO);
      break;
    case MID_BAD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_MID_BAD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_MID_BAD_TWO);
      break;
    case MID_MID:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_MID_MID_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_MID_MID_TWO);
      break;
    case MID_GOOD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_MID_GOOD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_MID_GOOD_TWO);
      break;
    case END_BAD_BAD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_BAD_BAD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_BAD_BAD_TWO);
      break;
    case END_BAD_MID:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_BAD_MID_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_BAD_MID_TWO);
      break;
    case END_BAD_GOOD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_BAD_GOOD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_BAD_GOOD_TWO);
      break;
    case END_MID_BAD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_MID_BAD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_MID_BAD_TWO);
      break;
    case END_MID_MID:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_MID_MID_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_MID_MID_TWO);
      break;
    case END_MID_GOOD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_MID_GOOD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_MID_GOOD_TWO);
      break;
    case END_GOOD_BAD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_GOOD_BAD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_GOOD_BAD_TWO);
      break;
    case END_GOOD_MID:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_GOOD_MID_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_GOOD_MID_TWO);
      break;
    case END_GOOD_GOOD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_END_GOOD_GOOD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_END_GOOD_GOOD_TWO);
      break;
    case DEAD_BAD:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_DEAD_BAD_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_DEAD_BAD_TWO);
      break;
    case DEAD_GOOD:
      break;
    default:
      pet->sprite_one = gbitmap_create_with_resource(RESOURCE_ID_STARTER_ONE);
      pet->sprite_two = gbitmap_create_with_resource(RESOURCE_ID_STARTER_TWO);
      APP_LOG(APP_LOG_LEVEL_ERROR, "pet_type:%d, not defined.", (int)cur_pet);
      break;
  }
}

pet_sprite pet_sprite_get_pet_type(PetSprite* pet)
{
  return pet->type;
}

void pet_sprite_set_and_display_sprites(pet_sprite pet_type, PetSprite *pet, StatusSprites *stat_sprites, bool display)
{
  if(pet)
  {
    gbitmap_destroy(pet->sprite_one);
    gbitmap_destroy(pet->sprite_two);
    pet_sprite_set_cur_pet(pet_type, pet);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "PET SIZE:%d", gbitmap_get_bounds(pet->sprite_one).size.w);
    if(display)
    {
      stats_splash_screen_display(SECRET, pet, stat_sprites, -1);
    }
  }
}

void pet_sprite_evolve_pet(pet_sprite new_evolution, PetSprite *pet, StatusSprites *stat_sprites, int evolve_num)
{
  if(pet)
  {
    gbitmap_destroy(pet->sprite_one);
    gbitmap_destroy(pet->sprite_two);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "PET CURRENT:%d", pet->type);
    pet_sprite_set_cur_pet(new_evolution, pet);
    if(stat_sprites)
    {
      stats_splash_screen_display(EVOLVE, pet, stat_sprites, evolve_num);
    }
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "PET EVOLVE TO:%d", new_evolution);
  persist_write_int(CURRENT_SPRITE_KEY, new_evolution);
}

void pet_sprite_destroy(PetSprite* pet)
{
  gbitmap_destroy(pet->sprite_one);
  gbitmap_destroy(pet->sprite_two);
  free(pet);
}

GPoint pet_sprite_get_position(PetSprite* pet)
{
  return pet->position;
}
GRect pet_sprite_get_bounds(PetSprite* pet)
{
  return gbitmap_get_bounds(pet->sprite_one);
}
GBitmap* pet_sprite_get_sprite_one(PetSprite* pet)
{
  return pet->sprite_one;
}
GBitmap* pet_sprite_get_sprite_two(PetSprite* pet)
{
  return pet->sprite_two;
}

void pet_sprite_move_randomly(PetSprite* pet)
{
  int r = rand() %4;

//   GRect max_bounds = GRect(0, buttons_height(), PEBBLE_WIDTH, PEBBLE_HEIGHT - (buttons_height()*2));
  int max_left = 0;
  int max_right = PEBBLE_WIDTH - gbitmap_get_bounds(pet->sprite_one).size.w;
  int max_up = 0;
  if(stats_is_dead())
  {
    //Halo Size
    max_up = 20;
  }
  int max_down = 117 - gbitmap_get_bounds(pet->sprite_one).size.h;
  
  int move = 4;
  
  if(stats_active_level() >= 5)
  {
    move = 20;
  }
  
  //move left
  if(r == 0)
  {
    int move_left = pet->position.x - move;
    if(move_left < max_left)
    {
      move_left = max_left;
    }
    pet->position.x = move_left;
  }
  //move right
  else if(r==1)
  {
    int move_right = pet->position.x + move;
    if(move_right > max_right)
    {
      move_right = max_right;
    }
    pet->position.x = move_right;
  }
  //move up
  else if(r==2)
  {
    int move_up = pet->position.y - move;
    if(move_up < max_up)
    {
      move_up = max_up;
    }
    pet->position.y = move_up;
  }
  //move down
  else if(r==3)
  {
    int move_down = pet->position.y + move;
    if(move_down > max_down)
    {
      move_down = max_down;
    }
    pet->position.y = move_down;
  }
  
}

void pet_sprite_draw(PetSprite* pet, GContext *ctx, bool update_sprite)
{
  if(stats_is_asleep())
  {
    return;
  }
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  if(update_sprite)
  {
    pet_sprite_move_randomly(pet);
  }
  if(pet->which_sprite)
  {
    graphics_draw_bitmap_in_rect(ctx, pet->sprite_one, GRect(pet->position.x, pet->position.y, gbitmap_get_bounds(pet->sprite_one).size.w, gbitmap_get_bounds(pet->sprite_one).size.h));
  }
  else
  {
    graphics_draw_bitmap_in_rect(ctx, pet->sprite_two, GRect(pet->position.x, pet->position.y, gbitmap_get_bounds(pet->sprite_two).size.w, gbitmap_get_bounds(pet->sprite_two).size.h));
  }
  if(update_sprite)
  {
    pet->which_sprite = !pet->which_sprite;
  }
}