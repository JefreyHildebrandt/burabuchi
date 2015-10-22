#include <pebble.h>
#include "Menu.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 2
#define NUM_SECOND_MENU_ITEMS 0

static Window *s_main_window;
static MenuLayer *s_menu_layer;
static PetSprite *pet;
bool yes_no_menu = false;
bool setcret_menu_access = false;
int secret_menu_count = 0;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) 
{
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) 
{
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) 
{
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) 
{
  switch (section_index) 
  {
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "Menu");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) 
{
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0:
          if(yes_no_menu)
          {
            menu_cell_basic_draw(ctx, cell_layer, "Yes, Reset", "Permanately delete", NULL);
          }
          else
          {
            menu_cell_basic_draw(ctx, cell_layer, "Back", "Return to main screen", NULL);
          }
          break;
        case 1:
          if(yes_no_menu)
          {
            menu_cell_basic_draw(ctx, cell_layer, "No", "Return to main screen", NULL);
          }
          else
          {
            menu_cell_basic_draw(ctx, cell_layer, "Reset Burabu", "Delete current burabu", NULL);
          }
          secret_menu_count++;
          APP_LOG(APP_LOG_LEVEL_DEBUG, "SECRET COUNT:%d", secret_menu_count);
          break;
      }
      break;
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) 
{
  switch (cell_index->row) 
  {
    case 0:
      if(yes_no_menu)
      {
          stats_reset_database(pet);
      }
      secret_menu_count = 0;
      window_stack_remove(s_main_window, false);
    break;
    case 1:
      if(secret_menu_count > 70 && secret_menu_count < 100)
      {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "SECRET COUNT SELECT:%d", secret_menu_count);
        window_stack_remove(s_main_window, false);
        display_secret_menu(pet);
      }
      else
      {
        if(yes_no_menu)
        {
          window_stack_remove(s_main_window, false);
        }
        yes_no_menu = true;
        layer_mark_dirty(menu_layer_get_layer(menu_layer));
      }
      secret_menu_count = 0;
    break;
  }

}

static void main_window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  yes_no_menu = false;

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });
  
  menu_layer_set_normal_colors(s_menu_layer, GColorCeleste, GColorTiffanyBlue);
  menu_layer_set_highlight_colors(s_menu_layer, GColorDukeBlue, GColorPastelYellow);

  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) 
{
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);
  yes_no_menu = false;
  setcret_menu_access = false;
  secret_menu_count = 0;
}

void display_menu(PetSprite *pet_) 
{
  pet = pet_;
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() 
{
  window_destroy(s_main_window);
}

