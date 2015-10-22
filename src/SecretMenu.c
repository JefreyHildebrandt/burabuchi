#include <pebble.h>
#include "SecretMenu.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 14
#define NUM_SECOND_MENU_ITEMS 0
  
static Window *s_main_window;
static MenuLayer *s_menu_layer;
static PetSprite *main_pet = NULL;
static PetSprite *pet = NULL;

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
      menu_cell_basic_header_draw(ctx, cell_layer, "Secret Menu");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) 
{
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, "Burabu", NULL, NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, "Burabu Arm", NULL, NULL);
          break;
        case 2:
          menu_cell_basic_draw(ctx, cell_layer, "Chicken Champion", NULL, NULL);
          break;
        case 3:
          menu_cell_basic_draw(ctx, cell_layer, "Danger Jackson", NULL, NULL);
          break;
        case 4:
          menu_cell_basic_draw(ctx, cell_layer, "Burabu Stache", NULL, NULL);
          break;
        case 5:
          menu_cell_basic_draw(ctx, cell_layer, "It's a Living", NULL, NULL);
          break;
        case 6:
          menu_cell_basic_draw(ctx, cell_layer, "Dr. Feelgood", NULL, NULL);
          break;
        case 7:
          menu_cell_basic_draw(ctx, cell_layer, "Solid Chicken", NULL, NULL);
          break;
        case 8:
          menu_cell_basic_draw(ctx, cell_layer, "Stop Chicken Time", NULL, NULL);
          break;
        case 9:
          menu_cell_basic_draw(ctx, cell_layer, "Birdshido Master", NULL, NULL);
          break;
        case 10:
          menu_cell_basic_draw(ctx, cell_layer, "Neck Blob", NULL, NULL);
          break;
        case 11:
          menu_cell_basic_draw(ctx, cell_layer, "Montgomery", NULL, NULL);
          break;
        case 12:
          menu_cell_basic_draw(ctx, cell_layer, "Super Burabujin", NULL, NULL);
          break;
        case 13:
          menu_cell_basic_draw(ctx, cell_layer, "Touch Ghost", NULL, NULL);
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
    pet_sprite_set_and_display_sprites(STARTER, pet, NULL, true);
    break;
    case 1:
    pet_sprite_set_and_display_sprites(MID_BAD, pet, NULL, true);
    break;
    case 2:
    pet_sprite_set_and_display_sprites(MID_MID, pet, NULL, true);
    break;
    case 3:
    pet_sprite_set_and_display_sprites(MID_GOOD, pet, NULL, true);
    break;
    case 4:
    pet_sprite_set_and_display_sprites(END_BAD_BAD, pet, NULL, true);
    break;
    case 5:
    pet_sprite_set_and_display_sprites(END_BAD_MID, pet, NULL, true);
    break;
    case 6:
    pet_sprite_set_and_display_sprites(END_BAD_GOOD, pet, NULL, true);
    break;
    case 7:
    pet_sprite_set_and_display_sprites(END_MID_BAD, pet, NULL, true);
    break;
    case 8:
    pet_sprite_set_and_display_sprites(END_MID_MID, pet, NULL, true);
    break;
    case 9:
    pet_sprite_set_and_display_sprites(END_MID_GOOD, pet, NULL, true);
    break;
    case 10:
    pet_sprite_set_and_display_sprites(END_GOOD_BAD, pet, NULL, true);
    break;
    case 11:
    pet_sprite_set_and_display_sprites(END_GOOD_MID, pet, NULL, true);
    break;
    case 12:
    pet_sprite_set_and_display_sprites(END_GOOD_GOOD, pet, NULL, true);
    break;
    case 13:
    pet_sprite_set_and_display_sprites(DEAD_BAD, pet, NULL, true);
    break;
  }

}

static void main_window_load(Window *window) 
{
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
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
  pet_sprite_set_and_display_sprites(stats_get_current_pet_sprite(), pet, NULL, false);
  menu_layer_destroy(s_menu_layer);
}

void display_secret_menu(PetSprite *pet_) 
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

