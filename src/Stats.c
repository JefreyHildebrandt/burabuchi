#include <pebble.h>
#include "Stats.h"

void stats_decrease_stat_value(uint32_t key);
void stats_increase_stat_value(uint32_t key);
void stats_print_status(); 
int stats_get_increase_value(int cur_val, int increase_value);
int stats_get_decrease_value(int cur_val, int decrease_val, bool sick_status);
bool stats_evolve(int total_tics, pet_sprite current_pet_num, int cur_average, PetSprite *pet, StatusSprites *stat_sprites);
int stats_which_pet_evolve_to(int total_tics, int cur_average);

void stats_new_tic(bool loop, PetSprite *pet, StatusSprites *stat_sprites)
{
  if(persist_read_bool(PET_DEAD_KEY))
  {
    int hygiene = stats_get_decrease_value(persist_read_int(HYGIENE_LEVEL_KEY), 1, false);
    persist_write_int(HYGIENE_LEVEL_KEY, hygiene);
    return;
  }

  //cur_time in seconds
  time_t cur_time = time(NULL);
  time_t time_between = cur_time - persist_read_int(TIME_LAST_TIC_KEY);
  //convert TIME_BETWEEN_TICS to seconds
  int tic_loop = 0;
  int total_tics = persist_read_int(TOTAL_TICS_KEY);
  
  if(!loop)
  {
    tic_loop = 1;
  }
  else
  {
    tic_loop = time_between/((TIME_BETWEEN_PET_BITMAP_CHANGE/1000)*NUM_BITMAP_CHANGE_FOR_TIC);
    persist_write_int(TIME_LAST_TIC_KEY, cur_time);
    
    if(tic_loop > (TICS_UNTIL_DEATH))
    {
      tic_loop = TICS_UNTIL_DEATH;
    }
  }
  
  int sleep_multiplier = 1;
  bool is_sleep = persist_read_bool(SLEEP_STATUS_KEY);
  if(is_sleep)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "SLEEP MULTIPLIER ACTIVATE");
    int temp = persist_read_int(SLEEP_TICS_EVOLVE_EXTEND);
    sleep_multiplier = SLEEP_MULTIPLIER;
  }
  
  int happy = persist_read_int(HUNGER_LEVEL_KEY);
  int hunger = persist_read_int(HUNGER_LEVEL_KEY);
  int active = persist_read_int(ACTIVE_LEVEL_KEY);
  int energy = persist_read_int(ENERGY_LEVEL_KEY);
  int hygiene = persist_read_int(HYGIENE_LEVEL_KEY);
  bool sick = persist_read_bool(SICK_STATUS_KEY);
  int average = persist_read_int(TOTAL_STATS_KEY);
  int current_sprite_num = persist_read_int(CURRENT_SPRITE_KEY);
  bool evolved = false;
  int sleep_extra = persist_read_int(SLEEP_TICS_EVOLVE_EXTEND);
  
  //persist_write_int(SLEEP_TICS_EVOLVE_EXTEND, persist_read_int(SLEEP_TICS_EVOLVE_EXTEND) + 1);
/////////////////////////////////////////////////////TESTCODE//////////////////////
//   int current_pet_num = current_sprite_num;
//   if(current_pet_num == STARTER)
//   {
// //     happy = 1;
// //     hunger = 1;
// //     active = 1;
// //     energy = 1;
// //     hygiene = 1;
    
// //         happy = 3;
// //   hunger = 3;
// //   active = 3;
// //   energy = 3;
// //   hygiene = 3;
    
//       happy = 5;
//   hunger = 5;
//   active = 5;
//   energy = 5;
//   hygiene = 5;
//   }
//   //checks evolution if the pet is currently mid tier
//   else if(current_pet_num == MID_BAD || current_pet_num == MID_MID ||current_pet_num == MID_GOOD)
//   {
// //     happy = 1;
// //     hunger = 1;
// //     active = 1;
// //     energy = 1;
// //     hygiene = 1;
    
// //   happy = 4;
// //   hunger = 4;
// //   active = 4;
// //   energy = 4;
// //   hygiene = 4;
    
// //       happy = 3;
// //   hunger = 3;
// //   active = 3;
// //   energy = 3;
// //   hygiene = 3;
  
//   happy = 5;
//   hunger = 5;
//   active = 5;
//   energy = 5;
//   hygiene = 5;
//   }
//   else if(
//     current_pet_num == END_BAD_BAD  ||
//     current_pet_num == END_BAD_MID  ||
//     current_pet_num == END_BAD_GOOD ||
//     current_pet_num == END_MID_BAD  ||
//     current_pet_num == END_MID_MID  ||
//     current_pet_num == END_MID_GOOD ||
//     current_pet_num == END_GOOD_BAD ||
//     current_pet_num == END_GOOD_MID ||
//     current_pet_num == END_GOOD_GOOD)
//   {
//     happy = 5;
//   hunger = 5;
//   active = 5;
//   energy = 5;
//   hygiene = 5;
//   }
// //   happy = 2;
// //   hunger = 2;
// //   active = 2;
// //   energy = 2;
// //   hygiene = 2;
  
// //   happy = 4;
// //   hunger = 4;
// //   active = 4;
// //   energy = 4;
// //   hygiene = 4;
/////////////////////////////////////////////////////END TESTCODE//////////////////

  for(int i=0; i<tic_loop; i++)
  {
    
    total_tics++;
    //adds time to evolve if the pet is asleep
    if(is_sleep)
    {
      sleep_extra++;
    }
    //   TOTAL_STATS_KEY
    average += (happy+hunger+active+hygiene)/4;
    if(!evolved)
    {
      int evolve_time = sleep_extra/2;
      evolved = stats_evolve(total_tics - evolve_time, current_sprite_num, average, pet, stat_sprites);
      
      if(evolved)
      {
        average = 0;
        total_tics = 0;
      }
    }
    
    if(total_tics%(HAPPY_TIC*sleep_multiplier) == 0)
    {
      float happy_average = (hunger+active+hygiene)/3;

      if(happy_average >= (STAT_FIELD_MAX/2))
      {
        happy = stats_get_increase_value(happy, 1);
      }
      else
      {
        happy = stats_get_decrease_value(happy, 1, sick);
      }
    }
    if(total_tics%(HUNGER_TIC*sleep_multiplier) == 0)
    {
      hunger = stats_get_decrease_value(hunger, 1, sick);
    }
    if(total_tics%(ENERGY_TIC*sleep_multiplier) == 0)
    {
      energy = stats_get_increase_value(energy, sleep_multiplier);
    }
    if(total_tics%(ACTIVE_TIC*sleep_multiplier) == 0)
    {
      active = stats_get_decrease_value(active, 1, sick);
    }
    if(total_tics%(HYGIENE_TIC*sleep_multiplier) == 0)
    {
      hygiene = stats_get_decrease_value(hygiene, 1, sick);
    }
    if(total_tics%(SICK_TIC*sleep_multiplier) == 0)
    {
      sick = true;
    }
      
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "AVERAGE:%d", average);
  }
  
  persist_write_int(TOTAL_TICS_KEY, total_tics);
  persist_write_int(SLEEP_TICS_EVOLVE_EXTEND, sleep_extra);
  
  persist_write_int(HAPPINESS_LEVEL_KEY, happy);
  persist_write_int(HUNGER_LEVEL_KEY, hunger);
  persist_write_int(ACTIVE_LEVEL_KEY, active);
  persist_write_int(HYGIENE_LEVEL_KEY, hygiene);
  persist_write_bool(SICK_STATUS_KEY, sick);
  
  persist_write_int(TOTAL_STATS_KEY, average);
  
  stats_print_status();
}

//TODO implement this function
bool stats_evolve(int total_tics, pet_sprite current_pet_num, int cur_average, PetSprite *pet, StatusSprites *stat_sprites)
{
  //checks evolution if pet is currently the starter
  if(current_pet_num == STARTER)
  {
    if(total_tics >= TICS_EVOLUTION_ONE)
    {
      int evolve_to_pet = stats_which_pet_evolve_to(total_tics, cur_average);
      
      if(evolve_to_pet == 0)
      {
        pet_sprite_evolve_pet(MID_BAD, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 1)
      {
        pet_sprite_evolve_pet(MID_MID, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 2)
      {
        pet_sprite_evolve_pet(MID_GOOD, pet, stat_sprites, evolve_to_pet);
      }
      return true;
    }
  }
  //checks evolution if the pet is currently mid tier
  else if(current_pet_num == MID_BAD)
  {
    if(total_tics >= TICS_EVOLUTION_TWO)
    {
      int evolve_to_pet = stats_which_pet_evolve_to(total_tics, cur_average);
      if(evolve_to_pet == 0)
      {
        pet_sprite_evolve_pet(END_BAD_BAD, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 1)
      {
        pet_sprite_evolve_pet(END_BAD_MID, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 2)
      {
        pet_sprite_evolve_pet(END_BAD_GOOD, pet, stat_sprites, evolve_to_pet);
      }
      return true;
    }
  }
  else if(current_pet_num == MID_MID)
  {
    if(total_tics >= TICS_EVOLUTION_TWO)
    {
      int evolve_to_pet = stats_which_pet_evolve_to(total_tics, cur_average);
      if(evolve_to_pet == 0)
      {
        pet_sprite_evolve_pet(END_MID_BAD, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 1)
      {
        pet_sprite_evolve_pet(END_MID_MID, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 2)
      {
        pet_sprite_evolve_pet(END_MID_GOOD, pet, stat_sprites, evolve_to_pet);
      }
      return true;
    }
  }
  else if(current_pet_num == MID_GOOD)
  {
    if(total_tics >= TICS_EVOLUTION_TWO)
    {
      int evolve_to_pet = stats_which_pet_evolve_to(total_tics, cur_average);
      if(evolve_to_pet == 0)
      {
        pet_sprite_evolve_pet(END_GOOD_BAD, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 1)
      {
        pet_sprite_evolve_pet(END_GOOD_MID, pet, stat_sprites, evolve_to_pet);
      }
      else if(evolve_to_pet == 2)
      {
        pet_sprite_evolve_pet(END_GOOD_GOOD, pet, stat_sprites, evolve_to_pet);
      }
      return true;
    }
  }
  //checks evolution if the pet is currently end tier
  else if(
    current_pet_num == END_BAD_BAD  ||
    current_pet_num == END_BAD_MID  ||
    current_pet_num == END_BAD_GOOD ||
    current_pet_num == END_MID_BAD  ||
    current_pet_num == END_MID_MID  ||
    current_pet_num == END_MID_GOOD ||
    current_pet_num == END_GOOD_BAD ||
    current_pet_num == END_GOOD_MID ||
    current_pet_num == END_GOOD_GOOD)
  {
    if(total_tics >= TICS_UNTIL_DEATH)
    {
      int evolve_to_pet = stats_which_pet_evolve_to(total_tics, cur_average);
      if(evolve_to_pet == 0 || evolve_to_pet == 1)
      {
        pet_sprite_evolve_pet(DEAD_BAD, pet, stat_sprites, evolve_to_pet);
        persist_write_bool(PET_DEAD_KEY, true);
      }
      else if(evolve_to_pet == 2)
      {
        pet_sprite_evolve_pet(pet_sprite_get_pet_type(pet), pet, stat_sprites, evolve_to_pet);
        persist_write_bool(PET_DEAD_KEY, true);
      }
      return true;
    }
  }
  return false;
}

int stats_which_pet_evolve_to(int total_tics, int cur_average)
{
  int max_average = total_tics * STAT_FIELD_MAX;
  int max_average_split = max_average/3;
  
  //minimum evolution
  if(cur_average < max_average_split)
  {
    return 0;
  }
  //mid evolution
  else if(cur_average < max_average_split*2)
  {
    return 1;
  }
  //max evolution
  else
  {
    return 2;
  }
}

bool stats_is_sick()
{
  return persist_read_bool(SICK_STATUS_KEY);
}

bool stats_is_dead()
{
  return persist_read_bool(PET_DEAD_KEY);
}

bool stats_is_asleep()
{
  return persist_read_bool(SLEEP_STATUS_KEY);
}

int stats_hygeine_level()
{
  return persist_read_int(HYGIENE_LEVEL_KEY);
}

int stats_hunger_level()
{
  return persist_read_int(HUNGER_LEVEL_KEY);
}

int stats_energy_level()
{
  return persist_read_int(ENERGY_LEVEL_KEY);
}

int stats_active_level()
{
  return persist_read_int(ACTIVE_LEVEL_KEY);
}

void stats_start_database(PetSprite *pet)
{
  if(!persist_exists(DATABASE_POPULATED_KEY))
  {
    persist_write_bool(DATABASE_POPULATED_KEY, false);
  }
  
  bool database_populate_exist = persist_read_bool(DATABASE_POPULATED_KEY);
  
  if(!database_populate_exist)
  {
    persist_write_bool(PET_DEAD_KEY, false);
    //time_t is the name of the variable that is returned from time(NULL), NULL returns current time
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Time is now %d", time(NULL));
    persist_write_int(TIME_LAST_TIC_KEY, time(NULL));
    persist_write_int(TOTAL_STATS_KEY, 0);
    persist_write_int(TOTAL_TICS_KEY, 0);
    persist_write_int(SLEEP_TICS_EVOLVE_EXTEND, 0);
    
    persist_write_int(HAPPINESS_LEVEL_KEY, STAT_FIELD_MAX);
    persist_write_int(HUNGER_LEVEL_KEY, STAT_FIELD_MAX/2);
    persist_write_int(ENERGY_LEVEL_KEY, STAT_FIELD_MAX);
    persist_write_int(ACTIVE_LEVEL_KEY, STAT_FIELD_MAX/2);
    persist_write_int(HYGIENE_LEVEL_KEY, STAT_FIELD_MAX);
    persist_write_bool(SLEEP_STATUS_KEY, false);
    persist_write_bool(SICK_STATUS_KEY, false);
    
    persist_write_int(CURRENT_SPRITE_KEY, STARTER);
    if(pet)
    {
      pet_sprite_evolve_pet(STARTER, pet, NULL, -1);
    }
    
    persist_write_bool(DATABASE_POPULATED_KEY, true);
  }
  
}

void stats_reset_database(PetSprite *pet)
{
  persist_write_bool(DATABASE_POPULATED_KEY, false);
  stats_start_database(pet);
}

int stats_get_current_pet_sprite()
{
  return persist_read_int(CURRENT_SPRITE_KEY);
}

void stats_hunger_pressed()
{
  stats_increase_stat_value(HUNGER_LEVEL_KEY);
  stats_print_status();
}

void stats_flush_pressed()
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "hygiene %d", (int) STAT_FIELD_MAX);
  persist_write_int(HYGIENE_LEVEL_KEY, STAT_FIELD_MAX);
  stats_print_status();
}

void stats_exercise_pressed()
{
  if(persist_read_int(ENERGY_LEVEL_KEY) <= STAT_FIELD_MIN)
  {
    stats_decrease_stat_value(HAPPINESS_LEVEL_KEY);
  }

  stats_increase_stat_value(ACTIVE_LEVEL_KEY);
  stats_decrease_stat_value(ENERGY_LEVEL_KEY);
  stats_print_status();
}

void stats_medicine_pressed()
{
  bool is_sick = persist_read_bool(SICK_STATUS_KEY);
  if(is_sick)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "No longer sick");
    persist_write_bool(SICK_STATUS_KEY, false);
  }
  else
  {
    stats_decrease_stat_value(HAPPINESS_LEVEL_KEY);
  }
}

void stats_sleep_pressed()
{
  bool is_asleep = !persist_read_bool(SLEEP_STATUS_KEY);
  persist_write_bool(SLEEP_STATUS_KEY, is_asleep);
  
  if(is_asleep)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "sleep true");
  }
  else
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "sleep false");
  }
}
void stats_settings_pressed(PetSprite *pet)
{
  display_menu(pet); 
}

int stats_get_decrease_value(int cur_val, int decrease_val, bool sick_status)
{
  cur_val -= decrease_val;
  if(sick_status)
  {
    cur_val-= decrease_val;
  }
  if(cur_val <= STAT_FIELD_MIN)
  {
    return STAT_FIELD_MIN;
  }
  else
  {
    return cur_val;
  }
}

void stats_decrease_stat_value(uint32_t key)
{
  int cur_val = persist_read_int(key);
  cur_val--;
  if(persist_read_bool(SICK_STATUS_KEY))
  {
    cur_val--;
  }
  if(cur_val <= STAT_FIELD_MIN)
  {
    persist_write_int(key, STAT_FIELD_MIN);
  }
  else
  {
    persist_write_int(key, cur_val);
  }
}

int stats_get_increase_value(int cur_val, int increase_value)
{
  cur_val += increase_value;
  if(cur_val > STAT_FIELD_MAX)
  {
    return STAT_FIELD_MAX;
  }
  else
  {
    return cur_val;
  }
}

void stats_increase_stat_value(uint32_t key)
{
  int cur_val = persist_read_int(key);
  cur_val++;
  if(cur_val > STAT_FIELD_MAX)
  {
    if(key != ENERGY_LEVEL_KEY && key != HAPPINESS_LEVEL_KEY)
    {
      stats_decrease_stat_value(HAPPINESS_LEVEL_KEY);
    }
    persist_write_int(key, STAT_FIELD_MAX);
  }
  else
  {
    persist_write_int(key, cur_val);
  }
}

void stats_print_status()
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "HAPPINESS:%d", (int)persist_read_int(HAPPINESS_LEVEL_KEY));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "HUNGER:%d", (int)persist_read_int(HUNGER_LEVEL_KEY));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ENERGY:%d", (int)persist_read_int(ENERGY_LEVEL_KEY));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "ACTIVE:%d", (int)persist_read_int(ACTIVE_LEVEL_KEY));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "HYGIENE:%d", (int)persist_read_int(HYGIENE_LEVEL_KEY));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SICK:%s", persist_read_bool(SICK_STATUS_KEY) ? "true" : "false");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SLEEP:%s", persist_read_bool(SLEEP_STATUS_KEY) ? "true" : "false");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "TOTAL STATS:%d", (int)persist_read_int(TOTAL_STATS_KEY));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "TOTAL TICS:%d", (int)persist_read_int(TOTAL_TICS_KEY));
}