///////////////////////////////
#include <acknex.h>
#include <default.c>

///////////////////////////////
var temp;
ENTITY* P1;
ENTITY* P2;

var FBulletNum = 0;
var FBulletLvl = 1;
action ASky()
{
  set(my, TRANSLUCENT);
  while (1)
  {
    my.roll -= 0.01 * my.skill1 * time_step;
    wait(1);
  }
}

action ABullet1()
{
  my.pan = FBulletNum;
  set(my, FLAG2);
  VECTOR* lv;
  var llife = 0;
  while (llife < 30)
  {
    lv = vector(70 * time_step, 0, 0);
    c_move(me, lv, nullvector, GLIDE | IGNORE_FLAG2 | IGNORE_PASSABLE);
    wait(1);
    my.roll += 50 * time_step;
    llife += 1 * time_step;
  }
  ent_remove(me);
}

function FShot1(int ANum)
{
  var i = 0;
  while (i < ANum)
  {
    FBulletNum = 90 + (i * 10);
    ent_create("bolt.mdl", my.x, ABullet1);
    if (i > 0)
    {
      FBulletNum = 90 - (i * 10);
      ent_create("bolt.mdl", my.x, ABullet1);
    }
    i += 1;
  }
}

action Ap1()
{
  while (!player) {
    wait(1);
  }
  wait(1);
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
  set(my, PASSABLE);
  set(my, INVISIBLE);
  my.pan = player.pan;
  P2 = me;
}

action Aplayer()
{
  set(my, INVISIBLE);
  player = me;
  wait(1);


  set(my, FLAG2);
  wait(1);
  var ltimeout = 0;

  ent_create("player.mdl", my.x, Ap1);
  ent_create("player2.mdl", my.x, Ap2);
  while (1)
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
    }
    if (key_d && my.x < 900)
    {
      my.x += 50 * time_step;
    }
    if (key_space)
      if (ltimeout < 1)
      {
        ltimeout = 5;
        FShot1(FBulletLvl);

      }
    if (ltimeout > 0) {
      ltimeout -= 1 * time_step;
    }
    wait(1);
    temp = ltimeout;

    if (key_1) {
      FBulletLvl = 1;
    }
    if (key_2) {
      FBulletLvl = 2;
    }
    if (key_3) {
      FBulletLvl = 3;
    }
    if (key_4) {
      FBulletLvl = 4;
    }
    if (key_5) {
      FBulletLvl = 5;
    }

    if (FBulletLvl < 3) {
      reset(P1, INVISIBLE);
    } else {
      set(P1, INVISIBLE);
    }
    if (FBulletLvl > 2) {
      reset(P2, INVISIBLE);
    } else {
      set(P2, INVISIBLE);
    }

    P1.x = my.x;
    P2.x = my.x;
    P1.y = my.y;
    P2.y = my.y;
    P1.z = my.z;
    P2.z = my.z;
  }
}

PANEL* my_panel =
{
  pos_x = 100;
  pos_y = 50;
  digits(20, 20, "%2.f", "Arial#20bi", 1, temp);
  flags = SHOW | OUTLINE;
}

function StartCameraAnim()
{
  camera.y = 194;
  camera.x = 121;
  camera.tilt = -32;
  camera.z = 138;
  camera.pan = 238;
  wait(-2);
  while (camera.pan > 90)
  {
    camera.x = -229 * cosv(camera.pan);
    camera.y = -229 * sinv(camera.pan);

    camera.pan -= 1 * time_step * 5;
    wait(1);
  }
  var i = 0;
  while (i < (60) / time_step)
  {
    i += 1;
    camera.z += (23) * time_step;
    camera.y -= (12) * time_step;
    camera.tilt -= (0.25) * time_step;
    wait(1);
  }
  camera.z = 1505;
  camera.y = -932;
  camera.tilt = -47;
}


{
  vec_set(screen_color, vector(1, 1, 1)); // dark blue
  vec_set(sky_color, vector(1, 1, 1)); // dark blue
  video_window(NULL, NULL, 0, "PJ187");
  video_mode = 12;
  d3d_antialias = 1;
  shadow_stencil = 3;
  fps_max = 60;
  level_load("level.wmb");
  vec_set(camera.x, vector(0, 0, 0));
  vec_set(camera.tilt, vector(0, 0, 0));
  StartCameraAnim();
}
