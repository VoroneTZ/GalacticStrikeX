#include "effekts.c"
#include "KPP.c"


action ASky()
{
  set(my, TRANSLUCENT);
  set(my, PASSABLE);

  while (1)
  {
    my.roll -= 0.01 * my.skill1 * time_step;
    wait(1);
  }
}
action Aclouds()
{
  set(my, TRANSLUCENT);
  set(my, PASSABLE);
  my.alpha = 14;
  my.blue = 30;
  while (1)
  {
    my.roll -= 0.1 * my.skill1 * time_step;
    wait(1);
  }
}
