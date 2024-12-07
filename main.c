///////////////////////////////
#include <acknex.h>
#include <default.c>
#include <mtlFX.c>;
#include <mtlView.c>;

///////////////////////////////
var temp;
ENTITY* P1;
ENTITY* P2;

var FBulletNum = 0;
var FBulletLvl = 1;
var FBulletType = 1;

var FPlLife = 5;
var FPlLifeType = 1;

var FScore = 0;

SOUND* SNDdestroyd = "destroyd.wav";
SOUND* SNDhits = "hits.wav";
SOUND* SNDhit1 = "hit1.wav";

ENTITY* SNDPlFire;


action AExplo()
{

  my.albedo = 400;
  set(my, PASSABLE);
  set(my, TRANSLUCENT);
  //  set(my,BRIGHT);
  my.red = 255;
  my.green = 255;
  my.blue = 255;
  my.lightrange = 120;
  my.alpha = 120;
  snd_play(SNDdestroyd, 50, 0);

  while (my.scale_x < 15)
  {
    my.scale_x += 1.5 * time_step;
    my.scale_y = my.scale_x;
    my.scale_z = my.scale_z;
    my.alpha -= 10 * time_step;
    my.lightrange = my.alpha;
    wait(1);
  }
  ent_remove(me);
}

function AttackPlrEvent()
{
  if (you == player)
  {
    my.skill2 = 30;
    //  effect(flm_part_func3, 1, my.x, nullvector);
    wait (1);
    my.event = NULL;


    set (my, INVISIBLE);
    set (my, PASSABLE);
    if (FPlLifeType == 1)
    {
      FPlLife -= 1;
    }
    else {
      FPlLifeType = 1;
    }
    FBulletLvl = 1;
    ent_remove (my);
  }
}

action ASky()
{
  set(my, TRANSLUCENT);
  while (1)
  {
    my.roll -= 0.01 * my.skill1 * time_step;
    wait(1);
  }
}


action AHeal()
{
  set(my, PASSABLE);
  wait(2);
  my.scale_x = 3;
  my.scale_y = 3;
  my.scale_z = 3;
  my.skill1 = 1;
  var pan=0;
  var pandir=1;
  while ((my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
  		if (pan>20){pandir=1;}
  	if (pan<0){pandir=0;}
  	if (pandir==0) 
  	{pan+=1*time_step;}else{pan-=1*time_step;}
    c_move(me, nullvector, vector(pan-10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
    my.pan += 10 * time_step;
    if (vec_dist(my.x, player.x) < 50) {
      my.skill1 = 0;
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

action AAmmo1()
{
  set(my, PASSABLE);
  wait(2);
  my.scale_x = 3;
  my.scale_y = 3;
  my.scale_z = 3;
  my.skill1 = 1;
  var pan=0;
  var pandir=1;
  while ((my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
  	if (pan>20){pandir=1;}
  	if (pan<0){pandir=0;}
  	if (pandir==0) 
  	{pan+=1*time_step;}else{pan-=1*time_step;}
    c_move(me, nullvector, vector(pan-10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
    my.pan += 10 * time_step;
    if (vec_dist(my.x, player.x) < 50) {
      my.skill1 = 0;
      if (FBulletType == 1) {
        if (FBulletLvl < 4) {
          if (FBulletLvl < 2 || FPlLifeType==2) {
            FBulletLvl += 1;
          }
        }
      } else {
        FBulletType = 1;
      }
    }
    wait(1);
  }
  ent_remove(me);
}

action AAsteroSml()
{
  my.group = 1;
  my.skill1 = 20;
  set(my, POLYGON);
  c_ignore(1, 0);
  my.emask |= ( ENABLE_ENTITY);
  my.event = AttackPlrEvent;
  wait(2);
  while ((my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
    c_move(me, vector(30 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    my.roll += 10 * time_step;
    wait(1);
  }
  if (!(my.eflags & CLIPPED))
  {
    FScore += 100;
    snd_play(SNDhits, 10, 0);
  }
  ent_remove(me);
}

action AAsteroBg()
{
  my.group = 1;
  my.scale_x = 3;
  my.scale_y = 3;
  my.scale_z = 3;
  var LAsteroNum = 0;
  ENTITY* LAstero;
  my.pan = 270;
  my.skill1 = 20;
  c_ignore(1, 0);
  my.emask |= ( ENABLE_ENTITY);
  my.event = AttackPlrEvent;
  while ((my.skill1 > 0) && (my.y > -500) )
  {
    c_move(me, vector(10 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    my.roll += 10 * time_step;
    wait(1);
  }
  //  wait(-1);
  set(my, PASSABLE);
  set(my, INVISIBLE);
  if  (!(my.eflags & CLIPPED))
  {
    ent_create("effekt_a.mdl", my.x, AExplo);
    var i = 0;
    LAsteroNum = 0;
    FScore += 500;
    while (i < 5)
    {
      LAstero = ent_create("asteroid.mdl", my.x, AAsteroSml);
      LAstero.pan = LAsteroNum;
      LAsteroNum += 72;
      i += 1;
    }
  }
  ent_remove(me);


}


function Bullet1Event()
{
  set (my, INVISIBLE);
  set (my, PASSABLE);
  my.skill2 = 30;
  //  effect(flm_part_func3, 1, my.x, nullvector);
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
  SNDPlFire = snd_play(SNDhit1, 10, 0);
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
  set(my, POLYGON);

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

    if (FPlLifeType == 1) {
      reset(P1, INVISIBLE);
    } else {
      set(P1, INVISIBLE);
    }
    if (FPlLifeType == 2) {
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
  digits(20, 20, "%2.f", "Arial#20bi", 1, FPlLife);
  digits(20, 40, "%2.f", "Arial#20bi", 1, FScore);

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
  camera.x = 0;
  camera.z = 1505;
  camera.y = -932;
  camera.pan = 90;
  camera.tilt = -47;


}


action Enemy1()
{
  my.group = 1;
  my.scale_x = 3;
  my.scale_y = 3;
  my.scale_z = 3;
  var LAsteroNum = 0;
  ENTITY* LBullet;
  my.pan = 270;
  my.skill1 = 20;
  c_ignore(1, 0);
  my.emask |= ( ENABLE_ENTITY);
  my.event = AttackPlrEvent;
  var ltimer = 0;
  while ((my.skill1 > 0) && (my.y > -500) )
  {
    c_move(me, vector(10 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    ltimer += time_step;

    if (ltimer > 20)
    {

      LBullet = ent_create("bolt.mdl", my.x, AAsteroSml);
      vec_to_angle(LBullet.pan, vec_diff(NULL, player.x, my.x));
      ltimer = 0;
    }

    wait(1);
  }

  //  wait(-1);
  set(my, PASSABLE);
  set(my, INVISIBLE);
  ent_create("effekt_a.mdl", my.x, AExplo);
  wait(-1);
  ent_remove(me);

}

function AsteroSpawner()
{
  randomize();
  var i;
  while (1)
  {
    i = random(4);
    if (i < 1)
    {
      ent_create("asteroid.mdl", vector(random(2000) - 1000, 2000, 0), AAsteroBg);
    }
    else if (i < 2)
    {
      ent_create("ship1.MDL", vector(random(2000) - 1000, 2000, 0), Enemy1);
    }
    if (i < 3)
    {
      ent_create("ammobox.mdl", vector(random(2000) - 1000, 2000, 0), AAmmo1);
    }
    else
    {
      ent_create("healthbox.MDL", vector(random(2000) - 1000, 2000, 0), AHeal);
    }
    wait(-1);
  }
}

function main()
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
  AsteroSpawner();
  media_loop("GalacticGunner.mp3", NULL, 100);
}
