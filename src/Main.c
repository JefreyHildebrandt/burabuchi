#include <pebble.h>
#include "Buttons.h"
#include "Stats.h"
#include "PetSprite.h"
#include "StatsSplashScreen.h"
#include "StatusSprites.h"

static Window *window = NULL;
static Layer *buttons_layer = NULL;
static Layer *sprites_layer = NULL;
static AppTimer *s_render_timer = NULL;
static Buttons *buttons = NULL;
static GBitmap *highlight = NULL;
static PetSprite *pet = NULL;
static StatusSprites *stat_sprites = NULL;
static int cur_button = 0;
static int count = 0;
static bool update_sprite = false;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  stats_splash_screen_display(cur_button, pet, stat_sprites, -1);
  buttons_press(cur_button, pet);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) 
{  
  if(cur_button == 0)
  {
    cur_button = 5;
  }
  else
  {
    cur_button--;
  }
  layer_mark_dirty(buttons_layer);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  if(cur_button == 5)
  {
    cur_button = 0;
  }
  else
  {
    cur_button++;
  }
  layer_mark_dirty(buttons_layer);
}

static void click_config_provider(void *context) 
{
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void frame_timer_handler(void *context) 
{
    update_sprite = true;
    layer_mark_dirty(sprites_layer);
    s_render_timer = app_timer_register(TIME_BETWEEN_PET_BITMAP_CHANGE, frame_timer_handler, NULL);
}

static void draw_buttons_update_proc(Layer *layer, GContext *ctx)
{
  buttons_draw_buttons(buttons, ctx);
  buttons_highlight(buttons, cur_button, highlight, ctx);
}

static void draw_sprites_update_proc(Layer *layer, GContext *ctx) 
{
  pet_sprite_draw(pet, ctx, update_sprite);
  status_sprites_draw(stat_sprites, ctx, update_sprite, pet);
  if(update_sprite)
  {
    if(count >= NUM_BITMAP_CHANGE_FOR_TIC)
    {
      count = 0;
      stats_new_tic(true, pet, stat_sprites);
    }
    
    count++;
  }
  update_sprite = false;
}

static void window_load(Window *window) 
{
  pet = pet_sprite_create();
  
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  buttons_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, buttons_layer);
  layer_set_update_proc(buttons_layer, draw_buttons_update_proc);
  buttons = buttons_create_all();
  highlight = gbitmap_create_with_resource(RESOURCE_ID_HIGHLIGHT_IMG);
  layer_mark_dirty(buttons_layer);
  
  sprites_layer = layer_create(buttons_get_sprite_layer_bounds(buttons));
  layer_add_child(window_layer, sprites_layer);
  layer_set_update_proc(sprites_layer, draw_sprites_update_proc);
  
  stat_sprites = status_sprites_create();
  
  stats_new_tic(true, pet, stat_sprites);
  
  s_render_timer = app_timer_register(1, frame_timer_handler, NULL);
}

static void window_unload(Window *window) 
{
  if(buttons_layer)
  {
    layer_destroy(buttons_layer);
    buttons_layer = NULL;
  }
  if(sprites_layer)
  {
    layer_destroy(sprites_layer);
    sprites_layer = NULL;
  }
  if(s_render_timer)
  {
    app_timer_cancel(s_render_timer);
    s_render_timer = NULL;
  }
  if(highlight)
  {
    gbitmap_destroy(highlight);
    highlight = NULL;
  }
  if(pet)
  {
    pet_sprite_destroy(pet);
    pet = NULL;
  }
  if(buttons)
  {
    buttons_destroy_all(buttons);
  }
  if(stat_sprites)
  {
    status_sprites_destroy(stat_sprites);
  }
  
  cur_button = 0;
  count = 0;
  update_sprite = false;
  
}

static void init(void) 
{
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	  .load = window_load,
    .unload = window_unload,});
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) 
{
  window_destroy(window);
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}
