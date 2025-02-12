#include "vars.c"
#include "effekts.c"
#include "KPP.c"


function Bullet1Event()
{
  set (my, INVISIBLE);
  set (my, PASSABLE);
  my.skill2 = 30;
  effect(r_part_func, 5, my.x, nullvector);
  snd_play(SNDhits, 20, 0);
  wait (1);
  my.event = NULL;
  ent_remove (my);
  if (you) {
    you.skill1 -= 10;
  }
  //  wait(-1);

}

action ABullet1()
{
  my.pan = FBulletNum;
  my.lightrange = 100;
  set(my, FLAG2);
  VECTOR* lv;
  my.skill2 = 0;
  my.emask |= ( ENABLE_ENTITY);
  my.event = Bullet1Event;
  while (my.skill2 < 30)
  {
    effect(gr_part_func, 1, my.x, nullvector);
    lv = vector(70 * time_step, 0, 0);
    c_move(me, lv, nullvector, GLIDE | IGNORE_FLAG2 | IGNORE_PASSABLE);
    wait(1);
    my.roll += 50 * time_step;
    my.skill2 += 1 * time_step;
  }
  ent_remove(me);
}


action ABullet2()
{
  my.pan = FBulletNum;
  my.lightrange = 100;
  set(my, FLAG2);
  VECTOR* lv;
  var i = 0;
  my.skill2 = 0;
  my.emask |= ( ENABLE_ENTITY);
  my.event = Bullet1Event;
  while (my.skill2 < 30)
  {
    effect(flm_part_func, 1, my.x, nullvector);

    c_scan(my.x, my.pan, vector(360, 180, 2000), SCAN_ENTS | IGNORE_ME | IGNORE_FLAG2 | IGNORE_PASSABLE);

    if (you)
    {
      if (you != player)
      {
        vec_set(temp, your.x);
        vec_sub(temp, my.x);
        vec_to_angle(i, temp);

        if (i < 180) {
          if (i > my.pan) {
            my.pan += 8 * time_step;
          } else {
            my.pan -= 8 * time_step;
          }
        }
        else
        {
          if (i > my.pan) {
            my.pan -= 8 * time_step;
          } else {
            my.pan += 8 * time_step;
          }
        }

      }
    }

    lv = vector(70 * time_step, 0, 0);
    c_move(me, lv, nullvector, GLIDE | IGNORE_FLAG2 | IGNORE_PASSABLE);
    wait(1);
    my.roll += 50 * time_step;
    my.skill2 += 1 * time_step;
  }
  ent_remove(me);
}

function FShot1(int ANum)
{
  var i = 0;

  if (FBulletType == 1)
  {
    SNDPlFire = snd_play(SNDhit1, 20, 0);
    while (i < ANum)
    {
      FBulletNum = 90 + (i * 5);
      ent_create("bolt.mdl", my.x, ABullet1);
      if (i > 0)
      {
        FBulletNum = 90 - (i * 5);
        ent_create("bolt.mdl", my.x, ABullet1);
      }
      i += 1;
    }
  }
  else
  {
    if (FBulletType == 2)
    { SNDPlFire = snd_play(SNDhit2, 20, 0);
      var ldir = 0;
      while (i < ANum)
      {

        FBulletNum = 90 + (i * 10);
        if (ldir == 0)
        {
          ent_create("rocket.mdl", my.x, ABullet2);
          ldir = 1;
        }
        else
        {
          FBulletNum = 90 - (i * 10);
          ent_create("rocket.mdl", my.x, ABullet2);
          ldir = 0;
        }
        i += 1;
      }
    }

  }
}

action Ap1()
{
  while (!player) {
    wait(1);
  }
  wait(1);
  	my.material = matGlow;
	my.red = 400;
	my.green = 400;
	my.blue = 100;
	my.ambient = 25;
  set(my, PASSABLE);
  set(my, INVISIBLE);
  my.pan = player.pan;
  P1 = me;

}

action Ap2()
{
  while (!player) {
    wait(1);
  }
  wait(1);
   my.material = matGlow;
	my.red = 800;
	my.green = 800;
	my.blue = 200;
	my.ambient = 25;
  set(my, PASSABLE);
  set(my, INVISIBLE);
  my.pan = player.pan;
  P2 = me;
}

action AShield()
{
  set(my, INVISIBLE);
  set(my, TRANSLUCENT);
  my.scale_x = 7;
  my.scale_y = 7;
  my.scale_z = 7;
  while (!player) {
    wait(1);
  }
  wait(1);
  set(my, PASSABLE);

  my.pan = player.pan;
  PShield = me;
  while (1) {
    my.pan += 1;
    if (my.pan > 359) {
      my.pan = 0;
    }
    wait(1);
  }
}


action Aplayer()
{
  set(my, INVISIBLE);
  player = me;

  wait(1);
  set(my, POLYGON);

  set(my, FLAG2);
  wait(1);
  var ltimeout = 0;

  ent_create("player.mdl", my.x, Ap1);
  ent_create("player2.mdl", my.x, Ap2);
  ent_create("effekt_b.mdl", my.x, AShield);
  ENTITY* Buzzer;
  Buzzer = snd_play(SNDbuzzer, 0, 0);

  while (1)
  {
  	  KPP_ZoomBlurStrength = FHitTimer/2;
    my.roll=0;
    panel_red.alpha = FHitTimer;
    if (FHitTimer > 0) {
      FHitTimer -= 1 * time_step;
    }
    if (FStartGame == 1)
    {
    
      if (key_w && my.y < 1000)
      {
        my.y += 50 * time_step;
      }
      if (key_s && my.y > -100)
      {
        my.y -= 50 * time_step;
      }
      if (key_a && my.x > -900)
      {
        my.x -= 50 * time_step;
        my.roll=-5;
      }
      if (key_d && my.x < 900)
      {
        my.x += 50 * time_step;
        my.roll=5;
      }
      if (key_space)
        if (ltimeout < 1)
        {
          ltimeout = 5;
          FShot1(FBulletLvl);

        }
    }
    if (ltimeout > 0) {
      ltimeout -= 1 * time_step;
    }
    wait(1);
    temp = ltimeout;

    if (FPlLifeType == 1) {
      reset(P1, INVISIBLE);
      if (FPlLife > 0)
      {
        effect(engine2_func, 10, my.x, nullvector);
      }

    } else {
      set(P1, INVISIBLE);
    }
    if (FPlLifeType == 2) {
      reset(P2, INVISIBLE);
      effect(engine_func, 10, my.x, nullvector);
    } else {
      set(P2, INVISIBLE);
    }

    if (FHitTimer > 0) {
      reset(PShield, INVISIBLE);
    } else {
      set(PShield, INVISIBLE);
    }


    if (FPlLife < 1)
    {
      set(P1, INVISIBLE);
      set(P2, INVISIBLE);
      set(PShield, INVISIBLE);
      set(my, PASSABLE);
    }
    else
    {
      reset(my, PASSABLE);
    }




    P1.x = my.x;
    P2.x = my.x;
    P1.y = my.y;
    P2.y = my.y;
    P1.z = my.z;
    P2.z = my.z;
    P1.roll = my.roll;
    P2.roll = my.roll;
    PShield.x = my.x;
    PShield.x = my.x;
    PShield.y = my.y;

    if (FPlLife == 1 && FPlLifeType == 1) {
      if (!snd_playing(Buzzer)) {
        Buzzer = snd_loop(SNDbuzzer, 20, 0);
      }
    }
    else
      snd_stop(Buzzer);


  }
}

