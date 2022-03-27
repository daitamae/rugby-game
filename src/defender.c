// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Internal headers
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "defender.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter


/*----------------------------------------------------------------------------*/
/*                              PRIVATE FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

bool equal_direction_defender(direction_t dir1, direction_t dir2)
{
  return dir1.i == dir2.i && dir1.j == dir2.j;
}

direction_t next_direction_defender(direction_t last_direction)
{
  if(equal_direction_defender(last_direction,(direction_t) DIR_UP) ||
    equal_direction_defender(last_direction,(direction_t) DIR_UP_RIGHT) ||
    equal_direction_defender(last_direction,(direction_t) DIR_UP_LEFT) ||
    equal_direction_defender(last_direction,(direction_t) DIR_DOWN) ||
    equal_direction_defender(last_direction,(direction_t) DIR_DOWN_RIGHT) ||
    equal_direction_defender(last_direction,(direction_t) DIR_DOWN_LEFT))
  {
    return (direction_t) DIR_LEFT;
  }
  if(equal_direction_defender(last_direction,(direction_t) DIR_LEFT))
  {
    return (direction_t) DIR_RIGHT;
  }
  return (direction_t) DIR_STAY;
}

direction_t decide_direction_defender(double r1, double r2, double down_lim, double up_lim, double right_lim, double left_lim)
{
  direction_t dir = (direction_t) DIR_STAY;

  if(r1<down_lim)dir.i+=1;
  else if(r1>up_lim)dir.i-=1;
  if(r2<left_lim)dir.j-=1;
  else if(r2>right_lim)dir.j+=1;
  return dir;
}


/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(
    position_t defender_position, Spy attacker_spy) {
  // TODO: unused parameters, remove these lines later

  static int count = 0;
  if(count==0)srand(time(NULL));

  static position_t last_position;
  static direction_t last_direction;
  
  static double down_lim = 0.3333;
  static double up_lim = 0.6666;
  static double left_lim = 0.2;
  static double right_lim = 0.95;
  static int count_spy_used = 0;

  double r1 = (double) rand()/RAND_MAX;
  double r2 = (double) rand()/RAND_MAX;
  double r3 = (double) rand()/RAND_MAX;

  if(count && equal_positions(last_position, defender_position) && !equal_direction_defender(last_direction,(direction_t) DIR_STAY))
  {
    last_direction = next_direction_defender(last_direction);
    return last_direction;
  }

  count++;
  last_position = defender_position;

  if(count > 3 && r3 > 0.3 && !count_spy_used)
  {
    count_spy_used = count;
    position_t attacker_position = get_spy_position(attacker_spy);
    if(attacker_position.i+1>=defender_position.i && attacker_position.i-1<=defender_position.i)
    {
      down_lim = 0.15;
      up_lim = 0.85;
      left_lim = 0.2;
      right_lim = 0.8;
    }
    else if(attacker_position.i>defender_position.i)
    {
      down_lim = 0.7;
      up_lim = 0.9;
    }else if(attacker_position.i<defender_position.i)
    {
      down_lim = 0.1;
      up_lim = 0.3;
    }
  }

  if(count - count_spy_used == 5)
  {
    down_lim = 0.1;
      up_lim = 0.9;
    left_lim = 0.2;
    right_lim = 0.8;
  }

  last_direction = decide_direction_defender(r1,r2,down_lim,up_lim,right_lim,left_lim);
  return last_direction;
}


