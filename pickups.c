action AAmmo1()
{
  set(my, PASSABLE);
  wait(2);
  my.scale_x = 3;
  my.scale_y = 3;
  my.scale_z = 3;
  my.skill1 = 1;
  var pan = 0;
  var pandir = 1;
  while ((my.skill1 > 0) && (my.y > -100) )
  {
    if (pan > 20) {
      pandir = 1;
    }
    if (pan < 0) {
      pandir = 0;
    }
    if (pandir == 0)
    {
      pan += 1 * time_step;
    } else {
      pan -= 1 * time_step;
    }
    c_move(me, nullvector, vector(pan - 10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
    my.pan += 10 * time_step;
    if (vec_dist(my.x, player.x) < 50) {
      my.skill1 = 0;
      snd_play(SNDradar, 70, 0);
      if (FBulletLvl < 4) {
        if (FBulletLvl < 2 || FPlLifeType == 2) {
          FBulletLvl += 1;
        }
      }

      FBulletType = 1;

    }
    wait(1);
  }
  ent_remove(me);
}

action AAmmo2()
{
  set(my, PASSABLE);
  wait(2);
  my.scale_x = 3;
  my.scale_y = 3;
  my.scale_z = 3;
  my.skill1 = 1;
  var pan = 0;
  var pandir = 1;
  while ((my.skill1 > 0) &&  (my.y > -100)  )
  {
    if (pan > 20) {
      pandir = 1;
    }
    if (pan < 0) {
      pandir = 0;
    }
    if (pandir == 0)
    {
      pan += 1 * time_step;
    } else {
      pan -= 1 * time_step;
    }
    c_move(me, nullvector, vector(pan - 10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
    my.pan += 10 * time_step;
    if (vec_dist(my.x, player.x) < 50) {
      my.skill1 = 0;
      snd_play(SNDradar, 70, 0);
      if (FBulletLvl < 4) {
        if (FBulletLvl < 2 || FPlLifeType == 2) {
          FBulletLvl += 1;
        }
      }

      FBulletType = 2;

    }
    wait(1);
  }
  ent_remove(me);
}



action AHeal()
{
  set(my, PASSABLE);
  wait(2);
  my.scale_x = 3;
  my.scale_y = 3;
  my.scale_z = 3;
  my.skill1 = 1;
  var pan = 0;
  var pandir = 1;
  while ((my.skill1 > 0) && (my.y > -100)  )
  {
    if (pan > 20) {
      pandir = 1;
    }
    if (pan < 0) {
      pandir = 0;
    }
    if (pandir == 0)
    {
      pan += 1 * time_step;
    } else {
      pan -= 1 * time_step;
    }
    c_move(me, nullvector, vector(pan - 10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
    my.pan += 10 * time_step;
    if (vec_dist(my.x, player.x) < 50) {
      my.skill1 = 0;
      snd_play(SNDheal, 20, 0);
      if (FPlLifeType == 2) {
        FPlLife += 1;
      } else {
        FPlLifeType = 2;
      }
    }
    wait(1);
  }
  ent_remove(me);
}
