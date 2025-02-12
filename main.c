#define PRAGMA_PATH "sounds";
#define PRAGMA_PATH "models";
#define PRAGMA_PATH "images";
#define PRAGMA_PATH "levels";
#define PRAGMA_PATH "KPP"



///////////////////////////////
#include <acknex.h>
#include <default.c>
#include <mtlFX.c>;
#include <mtlView.c>;
#include "vars.c"
#include "sounds.c"
#include "panels.c"
#include "player.c"
#include "pickups.c"
#include "enemys.c"
#include "embient.c"

#define SSAA 4.0 // Enable supersampling? -> extremely slow, but good for screenshots ;)  -  if you want this, #define it before including KPP.c

// Add the sub folder & include the scriptfile

#include "KPP.c"



///////////////////////////////




function set_start_screen()
{
  while (!player) {
    wait(1);
  }
  wait(1);
  camera.y = 194;
  camera.x = 121;
  camera.tilt = -32;
  camera.z = 138;
  camera.pan = 238;
  player.x = 0;
  player.y = 0;
  player.z = 0;
  wait(1);
}



function StartCameraAnim()
{
  while (!player) {
    wait(1);
  }
  set(score_panel, VISIBLE);
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


function AsteroSpawner()
{
  while (!player) {
    wait(1);
  }
  randomize();
  var i;
  while (FStartGame == 1)
  {
    i = random(6);
    if (i < 5)
    {
      ent_create("asteroid.mdl", vector(random(2000) - 1000, 2000, 0), AAsteroBg);
    }
    if (i < 2)
    {
      ent_create("ship1.MDL", vector(random(2000) - 1000, 2000, 0), Enemy1);
    }
    if (i < 1)
    {
      ent_create("s1.MDL", vector(random(2000) - 1000, 2000, 0), Enemy2);
    }
    wait(-(2 - (FBulletLvl / 4)));
    //wait(4);
  }
}

function BonusSpawner()
{
  wait(-10);
  randomize();
  var i;
  while (FStartGame == 1)
  {
    i = random(6);
    if (i < 3) {
      wait(-5);
    } else if (i < 4)
    {
      if (FBulletLvl < 4 || (FBulletLvl == 4 && FBulletType != 1)) {
        ent_create("ammobox.mdl", vector(random(2000) - 1000, 2000, 0), AAmmo1);
      }
    } else if (i < 5)
    {
      if (FBulletLvl < 4 || (FBulletLvl == 4 && FBulletType != 2)) {
        ent_create("ammo.mdl", vector(random(2000) - 1000, 2000, 0), AAmmo2);
      }
    }
    else
    {
      if (FPlLife < 3 || FPlLifeType == 1) {
        ent_create("healthbox.MDL", vector(random(2000) - 1000, 2000, 0), AHeal);
      }
    }
    wait(-5);
  }
}

function FStart()
{ if (FStartGame == 0)
  {
    FStartGame = 1;
    FHitTimer = 0;
    FScore = 0;
    FPlLife = 2;
    FBulletLvl = 1;
    FBulletType = 1;
    FPlLifeType = 1;
    player.x = 0;
    player.y = 0;
    player.z = 0;
    wait(1);
    StartCameraAnim();
    reset(logo_panel, VISIBLE);
    reset(start_panel, VISIBLE);
    reset(score_pan, VISIBLE);
    wait(-4);
    AsteroSpawner();
    BonusSpawner();

  }
}

function onpause()
{
  if (FStartGame > 0)
  {
    if (freeze_mode == 2) {
      reset(pause_panel, VISIBLE);
      freeze_mode = 0;
    } else {
      set(pause_panel, VISIBLE);
      freeze_mode = 2;
    }
  }
  else
  {
    sys_exit(0);
  }
}

function main()
{
  vec_set(screen_color, vector(1, 1, 1)); // dark blue
  vec_set(sky_color, vector(1, 1, 1)); // dark blue
  video_window(NULL, NULL, 0, "PJ187");
  video_mode = 12;
  panel_red.alpha = 0;
  video_screen = 1;
  
  d3d_mipmapping = 4;
  shadow_stencil = -1;
  d3d_triplebuffer = 1;
  d3d_anisotropy = 7;
  d3d_antialias = 9;
  fps_max = 60;
  wait(1);
  panel_vtz.alpha = 0;
  panel_blue.alpha = 0;
  panel_vtz.pos_x = ((screen_size.x) / 2) - 256;
  panel_vtz.pos_y = ((screen_size.y) / 2) - 256;

  wait(-1);
  while (panel_vtz.alpha < 100)
  {
    panel_vtz.alpha += 10 * time_step;
    panel_blue.alpha = panel_vtz.alpha;
    wait(1);
  }
  panel_vtz.alpha = 100;
  panel_blue.alpha = panel_vtz.alpha;
  wait(-1);


  level_load("level.wmb");
  while (!player) {
    wait(1);
  }
  wait(1);
  KPP_Enable(); 
wait(1);
  set_start_screen();
  




  start_panel.pos_x = ((screen_size.x) / 2) - 200;
  start_panel.pos_y = ((screen_size.y) / 1.5);
  reset(pause_panel, VISIBLE);
  pause_panel.pos_x = ((screen_size.x) / 2) - 100;
  pause_panel.pos_y = ((screen_size.y) / 2);


  logo_panel.pos_x = ((screen_size.x) / 2) - 256;
  logo_panel.pos_y = ((screen_size.y) / 4) - 256;

  score_pan.pos_x = ((screen_size.x) / 2) - 200;
  score_pan.pos_y = ((screen_size.y) / 3);


  while (panel_vtz.alpha > 0)
  {
    panel_vtz.alpha -= 10 * time_step;
    panel_blue.alpha = panel_vtz.alpha;
    wait(1);
  }
  panel_vtz.alpha = 0;
  reset(panel_vtz, VISIBLE);
  reset(panel_blue, VISIBLE);
  wait(-1);


  set(logo_panel, VISIBLE);


  set(start_panel, VISIBLE);
  on_esc = onpause;
  on_enter = FStart;

 // game_load("score", 0);

  FScore = 0;
  FPlLife = 2;
  FHitTimer = 0;

  //StartCameraAnim();
  // AsteroSpawner();
  // BonusSpawner();
  media_loop("GalacticGunner.mp3", NULL, 100);

}
