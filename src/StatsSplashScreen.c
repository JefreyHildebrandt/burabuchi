#include <pebble.h>
#include "StatsSplashScreen.h"

static Window *window = NULL;
static Layer *sprites_layer = NULL;
static GRect sprites_layer_rect;
static GBitmap *sprite_one = NULL;
static GBitmap *sprite_two = NULL;
static GBitmap *sprite_three = NULL;
static GBitmap *sprite_four = NULL;
static GPoint sprite_position;
static GPoint pet_position;
static AppTimer *s_render_timer = NULL;
static button_type type;
static PetSprite* pet = NULL;
static StatusSprites *stat_sprites;
void stats_splash_screen_create(Window *window);
void stats_splash_screen_destroy(Window *window);
static int poop_num = 0;
static int evolve_num = 0;
static bool is_asleep = false;

static int count = 0;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) 
{
   window_stack_remove(window, false);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) 
{  
  window_stack_remove(window, false);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  window_stack_remove(window, false);
}

static void click_config_provider(void *context) 
{
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void frame_timer_handler(void *context) 
{  
  layer_mark_dirty(sprites_layer);
    
  s_render_timer = app_timer_register(TIME_BETWEEN_PET_BITMAP_CHANGE, frame_timer_handler, NULL);
}

static void draw_sprites_update_proc(Layer *layer, GContext *ctx) 
{
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  if(count == 1 || count == 0)
  {
    graphics_draw_bitmap_in_rect(ctx, pet_sprite_get_sprite_one(pet), GRect(pet_position.x, pet_position.y, gbitmap_get_bounds(pet_sprite_get_sprite_one(pet)).size.w, gbitmap_get_bounds(pet_sprite_get_sprite_one(pet)).size.h));
    if(type == HUNGER)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_four, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_four).size.w/2, 0, gbitmap_get_bounds(sprite_four).size.w, gbitmap_get_bounds(sprite_four).size.h));
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
    } 
    else if(type == MEDICINE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
    }
    else if(type == FLUSH)
    {
      sprite_position = GPoint(0,0);
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));

      for(int i=0; i<poop_num; i++)
      {
        Sprites *poop = status_sprites_get_poop(stat_sprites, i);
        graphics_draw_bitmap_in_rect(ctx, status_sprites_get_bitmap(poop), GRect(sprite_position.x + status_sprites_get_poop_position(stat_sprites, i).x, sprite_position.y + status_sprites_get_poop_position(stat_sprites, i).y, gbitmap_get_bounds(status_sprites_get_bitmap(poop)).size.w, gbitmap_get_bounds(status_sprites_get_bitmap(poop)).size.h));
      }
 
    }
    else if(type == EXERCISE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_four, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_four).size.w/2, 0, gbitmap_get_bounds(sprite_four).size.w, gbitmap_get_bounds(sprite_four).size.h));
      sprite_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h);
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
    }
    else if(type == SLEEP)
    {
      if(!is_asleep)
      {
        if(count == 0)
        {
          graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
          window_set_background_color(window, GColorPastelYellow);
        }
        else
        {
          graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
        }
      }
      else
      {
        if(count == 0)
        {
          window_set_background_color(window, GColorBlack);
        }
        else
        {
          graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
        }
      }
    }
    else if(type == EVOLVE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_three, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_three).size.w/2, 0, gbitmap_get_bounds(sprite_three).size.w, gbitmap_get_bounds(sprite_three).size.h));
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
    }
  }
  else if(count == 2)
  {
    graphics_draw_bitmap_in_rect(ctx, pet_sprite_get_sprite_two(pet), GRect(pet_position.x, pet_position.y, gbitmap_get_bounds(pet_sprite_get_sprite_two(pet)).size.w, gbitmap_get_bounds(pet_sprite_get_sprite_two(pet)).size.h));
    if(type == HUNGER)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_four, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_four).size.w/2, 0, gbitmap_get_bounds(sprite_four).size.w, gbitmap_get_bounds(sprite_four).size.h));
      graphics_draw_bitmap_in_rect(ctx, sprite_two, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_two).size.w, gbitmap_get_bounds(sprite_two).size.h));
    } 
    else if(type == MEDICINE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_two, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_two).size.w, gbitmap_get_bounds(sprite_two).size.h));
    }
    else if(type == FLUSH)
    {
      sprite_position = GPoint(0, PEBBLE_HEIGHT/2);
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
      
      for(int i=0; i<poop_num; i++)
      {
        Sprites *poop = status_sprites_get_poop(stat_sprites, i);
        graphics_draw_bitmap_in_rect(ctx, status_sprites_get_bitmap(poop), GRect(sprite_position.x + status_sprites_get_poop_position(stat_sprites, i).x, sprite_position.y + status_sprites_get_poop_position(stat_sprites, i).y, gbitmap_get_bounds(status_sprites_get_bitmap(poop)).size.w, gbitmap_get_bounds(status_sprites_get_bitmap(poop)).size.h));
      }
    }
    else if(type == EXERCISE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_four, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_four).size.w/2, 0, gbitmap_get_bounds(sprite_four).size.w, gbitmap_get_bounds(sprite_four).size.h));
      sprite_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
    }
    else if(type == SLEEP)
    {
      if(!is_asleep)
      {
        graphics_draw_bitmap_in_rect(ctx, sprite_two, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_two).size.w, gbitmap_get_bounds(sprite_two).size.h));
        window_set_background_color(window, GColorBlack);
      }
      else
      {
        graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
        window_set_background_color(window, GColorPastelYellow);
      }
    }
    else if(type == EVOLVE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_three, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_three).size.w/2, 0, gbitmap_get_bounds(sprite_three).size.w, gbitmap_get_bounds(sprite_three).size.h));
      graphics_draw_bitmap_in_rect(ctx, sprite_two, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_two).size.w, gbitmap_get_bounds(sprite_two).size.h));
    }
  }
  else
  {
    graphics_draw_bitmap_in_rect(ctx, pet_sprite_get_sprite_one(pet), GRect(pet_position.x, pet_position.y, gbitmap_get_bounds(pet_sprite_get_sprite_one(pet)).size.w, gbitmap_get_bounds(pet_sprite_get_sprite_one(pet)).size.h));
    if(type == HUNGER)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_four, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_four).size.w/2, 0, gbitmap_get_bounds(sprite_four).size.w, gbitmap_get_bounds(sprite_four).size.h));
      graphics_draw_bitmap_in_rect(ctx, sprite_three, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_three).size.w, gbitmap_get_bounds(sprite_three).size.h));
    } 
    else if(type == MEDICINE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_three, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_three).size.w, gbitmap_get_bounds(sprite_three).size.h));
    }
    else if(type == EXERCISE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_four, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_four).size.w/2, 0, gbitmap_get_bounds(sprite_four).size.w, gbitmap_get_bounds(sprite_four).size.h));
      sprite_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h);
      graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
    }
    else if(type == SLEEP)
    {
      if(!is_asleep)
      {
        graphics_draw_bitmap_in_rect(ctx, sprite_two, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_two).size.w, gbitmap_get_bounds(sprite_two).size.h));
      }
      else
      {
        graphics_draw_bitmap_in_rect(ctx, sprite_one, GRect(sprite_position.x, sprite_position.y, gbitmap_get_bounds(sprite_one).size.w, gbitmap_get_bounds(sprite_one).size.h));
      }
    }
    else if(type == EVOLVE)
    {
      graphics_draw_bitmap_in_rect(ctx, sprite_three, GRect(PEBBLE_WIDTH/2-gbitmap_get_bounds(sprite_three).size.w/2, 0, gbitmap_get_bounds(sprite_three).size.w, gbitmap_get_bounds(sprite_three).size.h));
    }
    count = 0;
  }
  count++;
}

void stats_splash_screen_display(button_type type_, PetSprite* pet_, StatusSprites *stat_sprites_, int evolve_num_)
{  
  if(type_ == SETTINGS)
  {
    return;
  }

  type = type_;
  pet = pet_;
  stat_sprites = stat_sprites_;
  poop_num = -1*(stats_hygeine_level()-5);
  is_asleep = stats_is_asleep();
  evolve_num = evolve_num_;
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	  .load = stats_splash_screen_create,
    .unload = stats_splash_screen_destroy,
  });
  //if flush
  GColor color = GColorVividCerulean;
  if(type == HUNGER)
  {
    if(stats_hunger_level() == STAT_FIELD_MAX)
    {
      color = GColorRed;
    }
    else
    {
      color = GColorGreen;
    }
  }
  else if(type == EXERCISE)
  {
    if(stats_active_level() == STAT_FIELD_MAX)
    {
      color = GColorRed;
    }
    else
    {
      color = GColorGreen;
    }
  }
  else if(type == MEDICINE)
  {
    if(!stats_is_sick())
    {
      color = GColorRed;
    }
    else
    {
      color = GColorGreen;
    }
  }
  else if(type == EVOLVE)
  {
    color = GColorWhite;
  }
  window_set_background_color(window, color);
  const bool animated = false;
  window_stack_push(window, animated);
}

void stats_splash_screen_create(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  sprites_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, sprites_layer);
  layer_set_update_proc(sprites_layer, draw_sprites_update_proc);
  count = 0;
  if(type == HUNGER)
  {
    sprite_one = gbitmap_create_with_resource(RESOURCE_ID_HUNGER_SPLASH_ONE);
    sprite_two = gbitmap_create_with_resource(RESOURCE_ID_HUNGER_SPLASH_TWO);
    sprite_three = gbitmap_create_with_resource(RESOURCE_ID_HUNGER_SPLASH_THREE);
    int hunger_level = stats_hunger_level() + 1;
    if(hunger_level == 0)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_ZERO);
    }
    else if(hunger_level == 1)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_ONE);
    }
    else if(hunger_level == 2)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_TWO);
    }
    else if(hunger_level == 3)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_THREE);
    }
    else if(hunger_level == 4)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_FOUR);
    }
    else if(hunger_level == 5)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_FIVE);
    }
    else
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_ZERO);
    }
    sprite_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
    pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
  }
  else if(type == FLUSH)
  {
    sprite_one = gbitmap_create_with_resource(RESOURCE_ID_FLUSH_SPLASH);
    sprite_position = GPoint(0,0);
    pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
  }
  else if(type == EXERCISE)
  {
    sprite_one = gbitmap_create_with_resource(RESOURCE_ID_WEIGHTS_SPLASH);
    sprite_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
    pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
    
    int active_level = stats_active_level() + 1;
    if(active_level == 0)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_ZERO);
    }
    else if(active_level == 1)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_ONE);
    }
    else if(active_level == 2)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_TWO);
    }
    else if(active_level == 3)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_THREE);
    }
    else if(active_level == 4)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_FOUR);
    }
    else if(active_level == 5)
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_FIVE);
    }
    else
    {
      sprite_four = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_ZERO);
    }
  }
  else if(type == MEDICINE)
  {
    sprite_one = gbitmap_create_with_resource(RESOURCE_ID_SHOT_ONE);
    sprite_two = gbitmap_create_with_resource(RESOURCE_ID_SHOT_TWO);
    sprite_three = gbitmap_create_with_resource(RESOURCE_ID_SHOT_THREE);
    sprite_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2-40, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h);
    pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
  }
  else if(type == SLEEP)
  {
    if(!is_asleep)
    {
      sprite_one = gbitmap_create_with_resource(RESOURCE_ID_LIGHTBULB_ONE);
      sprite_two = gbitmap_create_with_resource(RESOURCE_ID_LIGHTBULB_TWO);
      sprite_position = GPoint(0, 0);
      pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
    }
    else
    {
      sprite_one = gbitmap_create_with_resource(RESOURCE_ID_FIXED_LIGHTBULB);
      sprite_position = GPoint(0, 0);
      pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
    }
  }
  else if(type == EVOLVE)
  {
    sprite_one = gbitmap_create_with_resource(RESOURCE_ID_CURTAIN_ONE);
    sprite_two = gbitmap_create_with_resource(RESOURCE_ID_CURTAIN_TWO);
    if(evolve_num == 0)
    {
      sprite_three = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_ZERO);
    }
    else if(evolve_num == 1)
    {
      sprite_three = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_THREE);
    }
    else if(evolve_num == 2)
    {
      sprite_three = gbitmap_create_with_resource(RESOURCE_ID_SUCCESS_FIVE);
    }
    sprite_position = GPoint(0, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2-10);
    pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
  }
  else if(type == SECRET)
  {
    pet_position = GPoint(PEBBLE_WIDTH/2-pet_sprite_get_bounds(pet).size.w/2, PEBBLE_HEIGHT/2-pet_sprite_get_bounds(pet).size.h/2);
  }
  
  s_render_timer = app_timer_register(1, frame_timer_handler, NULL);
}

void stats_splash_screen_destroy(Window *window)
{ 
  if(sprites_layer)
  {
    layer_destroy(sprites_layer);
    sprites_layer = NULL;
  }
  if(sprite_one)
  {
    gbitmap_destroy(sprite_one);
    sprite_one = NULL;
  }
  if(sprite_two)
  {
    gbitmap_destroy(sprite_two);
    sprite_two = NULL;
  }
  if(sprite_three)
  {
    gbitmap_destroy(sprite_three);
    sprite_three = NULL;
  }
  if(sprite_four)
  {
    gbitmap_destroy(sprite_four);
    sprite_four = NULL;
  }
  if(pet)
  {
    pet = NULL;
  }
  if(s_render_timer)
  {
    app_timer_cancel(s_render_timer);
    s_render_timer = NULL;
  }
  
  count = 0;
  
  poop_num = 0;
  
  if(window)
  {
    window_destroy(window);
    window = NULL;
  }
}

