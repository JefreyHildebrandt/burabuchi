#include <pebble.h>
#include "Buttons.h"

typedef struct 
{
  button_type button_t;
  GBitmap *bitmap;
  GPoint button_pos;
} Button;

struct Buttons_
{
  GBitmap *background;
  Button* hunger_button;
  Button* flush_button;
  Button* exercise_button;
  Button* medicine_button;
  Button* sleep_button;
  Button* settings_button;
};

Buttons* buttons_create_all()
{
  Buttons *buttons = malloc(sizeof(Buttons));
  buttons->background = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_IMG);
    
  buttons->hunger_button = malloc(sizeof(Button));
  buttons->hunger_button->button_t = HUNGER;
  buttons->hunger_button->bitmap = gbitmap_create_with_resource(RESOURCE_ID_HUNGER_IMG);
  buttons->hunger_button->button_pos.x = 0;
  buttons->hunger_button->button_pos.y = 0;
    
  buttons->flush_button = malloc(sizeof(Button));
  buttons->flush_button->button_t = FLUSH;
  buttons->flush_button->bitmap = gbitmap_create_with_resource(RESOURCE_ID_FLUSH_IMG);
  buttons->flush_button->button_pos.x = buttons->hunger_button->button_pos.x + gbitmap_get_bounds(buttons->hunger_button->bitmap).size.w;
  buttons->flush_button->button_pos.y = buttons->hunger_button->button_pos.y;
    
  buttons->exercise_button = malloc(sizeof(Button));
  buttons->exercise_button->button_t = EXERCISE;
  buttons->exercise_button->bitmap = gbitmap_create_with_resource(RESOURCE_ID_EXERCISE_IMG);
  buttons->exercise_button->button_pos.x = buttons->flush_button->button_pos.x + gbitmap_get_bounds(buttons->flush_button->bitmap).size.w;
  buttons->exercise_button->button_pos.y = buttons->flush_button->button_pos.y;
    
  buttons->medicine_button = malloc(sizeof(Button));
  buttons->medicine_button->button_t = MEDICINE;
  buttons->medicine_button->bitmap = gbitmap_create_with_resource(RESOURCE_ID_MEDICINE_IMG);
  buttons->medicine_button->button_pos.x = buttons->hunger_button->button_pos.x;
  buttons->medicine_button->button_pos.y = PEBBLE_HEIGHT - gbitmap_get_bounds(buttons->medicine_button->bitmap).size.h;
    
  buttons->sleep_button = malloc(sizeof(Button));
  buttons->sleep_button->button_t = SLEEP;
  buttons->sleep_button->bitmap = gbitmap_create_with_resource(RESOURCE_ID_SLEEP_IMG);
  buttons->sleep_button->button_pos.x = buttons->medicine_button->button_pos.x + gbitmap_get_bounds(buttons->medicine_button->bitmap).size.w;
  buttons->sleep_button->button_pos.y = PEBBLE_HEIGHT - gbitmap_get_bounds(buttons->sleep_button->bitmap).size.h;
    
  buttons->settings_button = malloc(sizeof(Button));
  buttons->settings_button->button_t = SETTINGS;
  buttons->settings_button->bitmap = gbitmap_create_with_resource(RESOURCE_ID_SETTINGS_IMG);
  buttons->settings_button->button_pos.x = buttons->sleep_button->button_pos.x + gbitmap_get_bounds(buttons->sleep_button->bitmap).size.w;
  buttons->settings_button->button_pos.y = PEBBLE_HEIGHT - gbitmap_get_bounds(buttons->settings_button->bitmap).size.h;
  
  return buttons;
}

void buttons_draw_buttons(Buttons* buttons, GContext *ctx)
{
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  if(!stats_is_asleep())
  {
    graphics_draw_bitmap_in_rect(ctx, buttons->background, GRect(0, 0, PEBBLE_WIDTH, PEBBLE_HEIGHT));
  }
  graphics_draw_bitmap_in_rect(ctx, buttons->hunger_button->bitmap, GRect(buttons->hunger_button->button_pos.x, buttons->hunger_button->button_pos.y, gbitmap_get_bounds(buttons->hunger_button->bitmap).size.w, gbitmap_get_bounds(buttons->hunger_button->bitmap).size.h));
  graphics_draw_bitmap_in_rect(ctx, buttons->flush_button->bitmap, GRect(buttons->flush_button->button_pos.x, buttons->flush_button->button_pos.y, gbitmap_get_bounds(buttons->flush_button->bitmap).size.w, gbitmap_get_bounds(buttons->flush_button->bitmap).size.h));
  graphics_draw_bitmap_in_rect(ctx, buttons->exercise_button->bitmap, GRect(buttons->exercise_button->button_pos.x, buttons->exercise_button->button_pos.y, gbitmap_get_bounds(buttons->exercise_button->bitmap).size.w, gbitmap_get_bounds(buttons->exercise_button->bitmap).size.h));
  graphics_draw_bitmap_in_rect(ctx, buttons->medicine_button->bitmap, GRect(buttons->medicine_button->button_pos.x, buttons->medicine_button->button_pos.y, gbitmap_get_bounds(buttons->medicine_button->bitmap).size.w, gbitmap_get_bounds(buttons->medicine_button->bitmap).size.h));
  graphics_draw_bitmap_in_rect(ctx, buttons->sleep_button->bitmap, GRect(buttons->sleep_button->button_pos.x, buttons->sleep_button->button_pos.y, gbitmap_get_bounds(buttons->sleep_button->bitmap).size.w, gbitmap_get_bounds(buttons->sleep_button->bitmap).size.h));
  graphics_draw_bitmap_in_rect(ctx, buttons->settings_button->bitmap, GRect(buttons->settings_button->button_pos.x, buttons->settings_button->button_pos.y, gbitmap_get_bounds(buttons->settings_button->bitmap).size.w, gbitmap_get_bounds(buttons->settings_button->bitmap).size.h));
}

GPoint buttons_get_pet_layer_start_point(Buttons* buttons)
{
  //graphics_draw_bitmap_in_rect(ctx, pet->sprite_one, GRect(pet->position.x, pet->position.y, gbitmap_get_bounds(pet->sprite_one).size.w,  gbitmap_get_bounds(pet->sprite_one).size.h));
  
  int point_x = buttons->hunger_button->button_pos.x;
  int point_y = buttons->hunger_button->button_pos.y + gbitmap_get_bounds(buttons->hunger_button->bitmap).size.h;
  
//   APP_LOG(APP_LOG_LEVEL_DEBUG, "Hunger button: %d,%d", buttons->hunger_button->button_pos.x, buttons->hunger_button->button_pos.y);
  
  return GPoint(point_x, point_y);
}

GRect buttons_get_sprite_layer_bounds(Buttons *buttons)
{
  int x = buttons->hunger_button->button_pos.x;
  int y = buttons->hunger_button->button_pos.y + gbitmap_get_bounds(buttons->hunger_button->bitmap).size.h;
  int width = PEBBLE_WIDTH;
  int height = PEBBLE_HEIGHT - y + gbitmap_get_bounds(buttons->settings_button->bitmap).size.h;
  return GRect(x, y, width, height);
}

void buttons_highlight(Buttons *buttons, button_type highlight, GBitmap *button, GContext *ctx)
{
  switch(highlight)
  {
    case HUNGER:
    graphics_draw_bitmap_in_rect(ctx, button, GRect(buttons->hunger_button->button_pos.x, buttons->hunger_button->button_pos.y, gbitmap_get_bounds(button).size.w, gbitmap_get_bounds(button).size.h));
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "hunger highlighted");
    break;
    case FLUSH:
    graphics_draw_bitmap_in_rect(ctx, button, GRect(buttons->flush_button->button_pos.x, buttons->flush_button->button_pos.y, gbitmap_get_bounds(button).size.w, gbitmap_get_bounds(button).size.h));
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "flush highlighted");
    break;
    case EXERCISE:
    graphics_draw_bitmap_in_rect(ctx, button, GRect(buttons->exercise_button->button_pos.x, buttons->exercise_button->button_pos.y, gbitmap_get_bounds(button).size.w, gbitmap_get_bounds(button).size.h));
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "exercise highlighted");
    break;
    case MEDICINE:
    graphics_draw_bitmap_in_rect(ctx, button, GRect(buttons->medicine_button->button_pos.x, buttons->medicine_button->button_pos.y, gbitmap_get_bounds(button).size.w, gbitmap_get_bounds(button).size.h));
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "medicine highlighted");
    break;
    case SLEEP:
    graphics_draw_bitmap_in_rect(ctx, button, GRect(buttons->sleep_button->button_pos.x, buttons->sleep_button->button_pos.y, gbitmap_get_bounds(button).size.w, gbitmap_get_bounds(button).size.h));
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "sleep highlighted");
    break;
    case SETTINGS:
    graphics_draw_bitmap_in_rect(ctx, button, GRect(buttons->settings_button->button_pos.x, buttons->settings_button->button_pos.y, gbitmap_get_bounds(button).size.w, gbitmap_get_bounds(button).size.h));
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "settings highlighted");
    break;
    case EVOLVE:
    break;
    case SECRET:
    break;
  }
}

void buttons_press(button_type pressed_type, PetSprite *pet)
{
  switch(pressed_type)
  {
    case HUNGER:
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "hunger pressed");
    stats_hunger_pressed();
    break;
    case FLUSH:
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "flush pressed");
    stats_flush_pressed();
    break;
    case EXERCISE:
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "exercise pressed");
    stats_exercise_pressed();
    break;
    case MEDICINE:
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "medicine pressed");
    stats_medicine_pressed();
    break;
    case SLEEP:
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "sleep pressed");
    stats_sleep_pressed();
    break;
    case SETTINGS:
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "settings pressed");
    stats_settings_pressed(pet);
    break;
    case EVOLVE:
    break;
    case SECRET:
    break;
  }
}

void destroy_button(Button *button)
{
  gbitmap_destroy(button->bitmap);
  free(button);
  button = NULL;
}

void buttons_destroy_all(Buttons *buttons)
{
  gbitmap_destroy(buttons->background);
  buttons->background = NULL;
  destroy_button(buttons->hunger_button);
  destroy_button(buttons->flush_button);
  destroy_button(buttons->exercise_button);
  destroy_button(buttons->medicine_button);
  destroy_button(buttons->sleep_button);
  destroy_button(buttons->settings_button);
  free(buttons);
}