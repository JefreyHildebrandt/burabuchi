#pragma once
#include "PetSprite.h"
#include "Menu.h"
/***PERSIST DATA***/
#define DATABASE_POPULATED_KEY 054
#define PET_DEAD_KEY 12345
#define TIME_LAST_TIC_KEY 276
#define TOTAL_STATS_KEY 3867
#define TOTAL_TICS_KEY 490
  
#define HAPPINESS_LEVEL_KEY 5786
#define HUNGER_LEVEL_KEY 645
#define ENERGY_LEVEL_KEY 77987
#define ACTIVE_LEVEL_KEY 843
#define SLEEP_STATUS_KEY 954
#define SICK_STATUS_KEY 107879
#define HYGIENE_LEVEL_KEY 12097

#define CURRENT_SPRITE_KEY 11345
  
/***DEFINE DATA***/
#define STAT_FIELD_MAX 5
#define STAT_FIELD_MIN 0
//in ms
#define TIME_BETWEEN_PET_BITMAP_CHANGE 1000
#define NUM_BITMAP_CHANGE_FOR_TIC 9

#define TICS_EVOLUTION_ONE 50
#define TICS_EVOLUTION_TWO TICS_EVOLUTION_ONE *2
#define TICS_UNTIL_DEATH TICS_EVOLUTION_ONE *2

//////////////////TEST CODE//////////////////////////////
// #define NUM_BITMAP_CHANGE_FOR_TIC 5
// #define TICS_EVOLUTION_ONE 5
// #define TICS_EVOLUTION_TWO 5
// #define TICS_UNTIL_DEATH 5

//Number of tics until stat changes
// #define HAPPY_TIC 1
// #define HUNGER_TIC 2
// #define ENERGY_TIC 1
// #define ACTIVE_TIC 3
// #define HYGIENE_TIC 2
// #define SICK_TIC 30
//////////////////END TEST CODE//////////////////////////
  
#define HAPPY_TIC 5
#define HUNGER_TIC 8
#define ENERGY_TIC 10
#define ACTIVE_TIC 12
#define HYGIENE_TIC 5
#define SICK_TIC 30
#define SLEEP_MULTIPLIER 4
/***FUNCTIONS***/
typedef struct PetSprite_ PetSprite;
typedef struct StatusSprites_ StatusSprites;

bool stats_is_sick();

void stats_new_tic(bool loop, PetSprite *pet, StatusSprites *stat_sprites);

bool stats_is_asleep();

int stats_hygeine_level();

int stats_active_level();

int stats_energy_level();

int stats_hunger_level();

bool stats_is_dead();

void stats_start_database();

void stats_reset_database(PetSprite *pet);

int stats_get_current_pet_sprite();

void stats_hunger_pressed();

void stats_flush_pressed();

void stats_exercise_pressed();

void stats_medicine_pressed();

void stats_sleep_pressed();

void stats_settings_pressed(PetSprite *pet);
