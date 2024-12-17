///////////////////////////////
#include <acknex.h>
#include <default.c>
#include <mtlFX.c>;
#include <mtlView.c>;

///////////////////////////////
var temp;
ENTITY* P1;
ENTITY* P2;
ENTITY* PShield;
var FBulletNum = 0;
var FBulletLvl = 1;
var FBulletType = 2;

var FPlLife = 2;
var FPlLifeType = 1;
var FHitTimer=0;
var FStartGame=0;

var FScore = 0;
var FLastScore = 0;
var FTopScore=0;

SOUND* SNDdestroyd = "destroyd.wav";
SOUND* SNDhits = "hits.wav";
SOUND* SNDhit1 = "hit1.wav";
SOUND* SNDhit2 = "bullet2.wav";
SOUND* SNDhitPl = "explo1.wav";
SOUND* SNDbuzzer = "buzzer.wav";
SOUND* SNDradar = "radar1.wav";
SOUND* SNDheal = "sataus.wav";

ENTITY* SNDPlFire;
BMAP*  bmp_flm_part = "fpart.png";
BMAP*  bmp_b_part = "bpart.png";
BMAP* logo_bmap="logo.png";
BMAP*  bmp_r_part = "rpart.png";
BMAP* bmp_smoke_part = "burns.pcx";


PANEL* panel_red =
{
	pos_x = 0; pos_y = 0;
	size_x = 1920; size_y = 1080;
	red = 255;
	green = 0;
	blue = 0;
	layer = 2;
	flags = LIGHT | TRANSLUCENT | SHOW;
	
}

PANEL* score_panel =
{
  pos_x = 100;
  pos_y = 50;
  digits(20, 20, "Life: %2.f", "Arial#20bi", 1, FPlLife);
  digits(20, 40, "Score: %2.f", "Arial#20bi", 1, FScore);

  flags = OUTLINE;
}


PANEL* score_pan =
{
  pos_x = 0;
  pos_y = 0;
  digits(50, 40, "FINAL SCORE: %2.f", "Arial#40bi", 1, FLastScore);
  digits(50, 80, "TOP SCORE: %2.f", "Arial#40bi", 1, FTopScore);
  digits(0, 180, "PRESS ENTER TO PLAY", "Arial#40bi", 1, NULL);

  flags = OUTLINE;
}
function flm_part_fader(PARTICLE *p)
{
	p.alpha -= (20 * time_step);
	if (p.alpha <= 0) {
		p.lifespan = 0; // remove
	}
}

function set_start_screen()
{
		while(!player){wait(1);}
	wait(1);
	 camera.y = 194;
  camera.x = 121;
  camera.tilt = -32;
  camera.z = 138;
  camera.pan = 238;
  player.x=0;
  player.y=0;
  player.z=0;
  wait(1);
}
function smoke_part_func(PARTICLE *p)
{
	p.bmap = bmp_smoke_part;
	p.size = 32;

	p.x += 0;
	p.y += 0;
	p.z += 0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = 0;

	p.alpha = 100;
	set(p, BRIGHT);
//	set(p,OVERLAY);
	set(p, MOVE);
	set(p, TRANSLUCENT);

	p.event = flm_part_fader;
}

function flm_part_func(PARTICLE *p)
{
	p.bmap = bmp_flm_part;
	p.size = 32;

	p.x += 0;
	p.y += 0;
	p.z += 0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = 0;

	p.alpha = 100;
	set(p, BRIGHT);
//	set(p,OVERLAY);
	set(p, MOVE);
	set(p, TRANSLUCENT);

	p.event = flm_part_fader;
}

function bl_part_func(PARTICLE *p)
{
	p.bmap = bmp_b_part;
	p.size = 32;

	p.x += 0;
	p.y += 0;
	p.z += 0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = 0;

	p.alpha = 100;
	set(p, BRIGHT);
//	set(p,OVERLAY);
	set(p, MOVE);
	set(p, TRANSLUCENT);

	p.event = flm_part_fader;
}

function r_part_func(PARTICLE *p)
{
	p.bmap = bmp_r_part;
	p.size = 32;

	p.x += 0;
	p.y += 0;
	p.z += 0;

	p.vel_x = 0;
	p.vel_y = 0;
	p.vel_z = 0;

	p.alpha = 100;
	set(p, BRIGHT);
//	set(p,OVERLAY);
	set(p, MOVE);
	set(p, TRANSLUCENT);

	p.event = flm_part_fader;
}





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

  while (my.alpha > 1)
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
    if (FHitTimer<1)
    {
    	snd_play(SNDhitPl, 100, 0);
    	snd_play(SNDbuzzer, 20, 0);
    	FHitTimer=50;
    if (FPlLifeType == 1)
    {
      FPlLife -= 1;
    }
    else {
      FPlLifeType = 1;
    }
    FBulletLvl = 1;
    }
    if (FPlLife<1){if (FScore>FTopScore){FTopScore=FScore;} FLastScore = FScore; set(score_pan,VISIBLE); reset(score_panel,VISIBLE);FStartGame=0; game_save("score",0,SV_VARS);}
    
    
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
  while ((my.skill1 > 0) && (my.y>-100)  )
  {
  		if (pan>20){pandir=1;}
  	if (pan<0){pandir=0;}
  	if (pandir==0) 
  	{pan+=1*time_step;}else{pan-=1*time_step;}
    c_move(me, nullvector, vector(pan-10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
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
  while ((my.skill1 > 0) && (my.y>-100) )
  {
  	if (pan>20){pandir=1;}
  	if (pan<0){pandir=0;}
  	if (pandir==0) 
  	{pan+=1*time_step;}else{pan-=1*time_step;}
    c_move(me, nullvector, vector(pan-10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
    my.pan += 10 * time_step;
    if (vec_dist(my.x, player.x) < 50) {
      my.skill1 = 0;
      snd_play(SNDradar, 70, 0);
        if (FBulletLvl < 4) {
          if (FBulletLvl < 2 || FPlLifeType==2) {
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
  var pan=0;
  var pandir=1;
  while ((my.skill1 > 0) &&  (my.y>-100)  )
  {
  	if (pan>20){pandir=1;}
  	if (pan<0){pandir=0;}
  	if (pandir==0) 
  	{pan+=1*time_step;}else{pan-=1*time_step;}
    c_move(me, nullvector, vector(pan-10, -30 * time_step, 0), IGNORE_MODELS | IGNORE_PASSABLE | IGNORE_PUSH);
    my.pan += 10 * time_step;
    if (vec_dist(my.x, player.x) < 50) {
      my.skill1 = 0;
     snd_play(SNDradar, 70, 0);
        if (FBulletLvl < 4) {
          if (FBulletLvl < 2 || FPlLifeType==2) {
            FBulletLvl += 1;
          }
        }
      
        FBulletType = 2;
      
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
  while ((FStartGame==1)&&(my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
  	effect(smoke_part_func, 1, my.x, nullvector);
    c_move(me, vector(30 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    my.roll += 10 * time_step;
    wait(1);
  }
  if (!(my.eflags & CLIPPED))
  {
    FScore += 100;
    snd_play(SNDhits, 20, 0);
  }
  ent_remove(me);
}
action ABlt1()
{
  my.group = 1;
  my.skill1 = 20;
  set(my, POLYGON);
  c_ignore(1, 0);
  my.emask |= ( ENABLE_ENTITY);
  my.event = AttackPlrEvent;
  wait(2);
  while ((FStartGame==1)&&(my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
  	effect(bl_part_func, 1, my.x, nullvector);
  	
    c_move(me, vector(50 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    my.roll += 80 * time_step;
    wait(1);
  }
  if (!(my.eflags & CLIPPED))
  {
    FScore += 100;
    snd_play(SNDhits, 20, 0);
  }
  ent_remove(me);
}

action ARocket()
{

  my.group = 1;
  my.skill1 = 20;
  set(my, POLYGON);
  c_ignore(1, 0);
  my.emask |= ( ENABLE_ENTITY);
  my.event = AttackPlrEvent;
  var i=0;  
  wait(2);
  while ((FStartGame==1)&&(my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
  	effect(r_part_func, 1, my.x, nullvector);
  	vec_set(temp,player.x); 
  	vec_sub(temp,my.x);
  	vec_to_angle(i,temp);
  	if (i<180){
  	if (i>my.pan){my.pan+=4*time_step;}else{my.pan-=4*time_step;}}
  	else
  	{
  	if (i>my.pan){my.pan-=4*time_step;}else{my.pan+=4*time_step;}}
  	
    c_move(me, vector(50 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    my.roll += 10 * time_step;
    wait(1);
  }
  if (!(my.eflags & CLIPPED))
  {
    FScore += 100;
    snd_play(SNDhits, 20, 0);
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
  while ((FStartGame==1)&&(my.skill1 > 0) && (my.y > -500) )
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
   effect(r_part_func, 1, my.x, nullvector);
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
  	effect(bl_part_func, 1, my.x, nullvector);
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
  var i=0;
  my.skill2 = 0;
  my.emask |= ( ENABLE_ENTITY);
  my.event = Bullet1Event;
  while (my.skill2 < 30)
  {
  	effect(flm_part_func, 1, my.x, nullvector);
		
    c_scan(my.x,my.pan,vector(360,180,2000),SCAN_ENTS|IGNORE_ME|IGNORE_FLAG2|IGNORE_PASSABLE);
    
    if (you)
    {
    	if (you != player)
    	{
    	  vec_set(temp,your.x); 
  			vec_sub(temp,my.x);
  			vec_to_angle(i,temp); 
  			
  			if (i<180){
  	if (i>my.pan){my.pan+=8*time_step;}else{my.pan-=8*time_step;}}
  	else
  	{
  	if (i>my.pan){my.pan-=8*time_step;}else{my.pan+=8*time_step;}}
  	
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
  
  if (FBulletType==1)
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
  }}
  else
  {
  	if (FBulletType==2)
  {SNDPlFire = snd_play(SNDhit2, 20, 0);
  	var ldir=0;
  while (i < ANum)
  {
  	
    FBulletNum = 90 + (i * 10);
    if (ldir==0)
    {
    ent_create("rocket.mdl", my.x, ABullet2);
    ldir=1;
    }
    else
    {
      FBulletNum = 90 - (i * 10);
      ent_create("rocket.mdl", my.x, ABullet2);
      ldir=0;
    }
    i += 1; 
  }}
  	
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

action AShield()
{
	set(my, INVISIBLE);
	set(my, TRANSLUCENT);
	my.scale_x=7;
	my.scale_y=7;
	my.scale_z=7;
  while (!player) {
    wait(1);
  }
  wait(1);
  set(my, PASSABLE);
  
  my.pan = player.pan;
  PShield = me;
   while (1) {
   	my.pan+=1;
   	if (my.pan>359){my.pan=0;}
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

  	panel_red.alpha=FHitTimer;
  	 if (FHitTimer>0){FHitTimer-=1*time_step;}
  	 if (FStartGame==1)
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
      }
    if (ltimeout > 0) {
      ltimeout -= 1 * time_step;
    }
    wait(1);
    temp = ltimeout;

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
    
    if (FPlLife<1)
    {
    	set(P1, INVISIBLE);
    	set(P2, INVISIBLE);
    	set(my,PASSABLE);
    	}
    	else
    	{reset(my,PASSABLE);}
    
    
    if (FHitTimer > 0) {
      reset(PShield, INVISIBLE);
    } else {
      set(PShield, INVISIBLE);
    }

    P1.x = my.x;
    P2.x = my.x;
    P1.y = my.y;
    P2.y = my.y;
    P1.z = my.z;
    P2.z = my.z;
    PShield.x = my.x;
    PShield.x = my.x;
    PShield.y = my.y;
 
   if (FPlLife==1 && FPlLifeType==1){
  	if (!snd_playing(Buzzer)) { 	
  	Buzzer = snd_loop(SNDbuzzer, 20, 0);}}
  	else  	
  	snd_stop(Buzzer);
  	

  }
}

PANEL* logo_panel =
{
  pos_x = 0;
  pos_y = 0;
   digits(180, 0, "(c) 2024 VTZ Games", "Arial#20bi", 1, NULL);
bmap = logo_bmap; 
  flags = OUTLINE;
 
}



PANEL* start_panel =
{
  pos_x = 0;
  pos_y = 0;
  digits(0, 0, "PRESS ENTER TO START", "Arial#40bi", 1, NULL);
  
  digits(110, 100, "TOP SCORE:  %2.f", "Arial#20bi", 1, FTopScore);

  flags = OUTLINE;
}


PANEL* pause_panel =
{
  pos_x = 0;
  pos_y = 0;
  digits(0, 0, "PAUSE", "Arial#40bi", 1, NULL);
  

  flags = OUTLINE;
}

function StartCameraAnim()
{
		while(!player){wait(1);}
	set(score_panel,VISIBLE);
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
  while ((FStartGame==1)&&(my.skill1 > 0) && (my.y > -500) )
  {
    c_move(me, vector(15 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    ltimer += time_step;

    if (ltimer > 35 -(FBulletLvl*3)-(FPlLifeType*4))
    {

      LBullet = ent_create("bolt.mdl", my.x, ABlt1);
      vec_to_angle(LBullet.pan, vec_diff(NULL, player.x, my.x));
      ltimer = 0;
    }

    wait(1);
  }
  if (my.skill1<1){FScore += 500;}

  //  wait(-1);
  set(my, PASSABLE);
  set(my, INVISIBLE);
  ent_create("effekt_a.mdl", my.x, AExplo);
  wait(-1);
  ent_remove(me);

}

action Enemy2()
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
  while ((FStartGame==1)&&(my.skill1 > 0) && (my.y > -500) )
  {
    c_move(me, vector(20 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    ltimer += time_step;

    if (ltimer > 40-(FBulletLvl*4))
    {

      LBullet = ent_create("rocket.mdl", my.x, ARocket);
      vec_to_angle(LBullet.pan, vec_diff(NULL, player.x, my.x));
      ltimer = 0;
    }

    wait(1);
  }
 if (my.skill1<1){FScore += 500;}
  //  wait(-1);
  set(my, PASSABLE);
  set(my, INVISIBLE);
  ent_create("effekt_a.mdl", my.x, AExplo);
  wait(-1);
  ent_remove(me);

}

function AsteroSpawner()
{
	while(!player){wait(1);}
  randomize();
  var i;
  while (FStartGame==1)
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
    wait(-(2-(FBulletLvl/4)));
  }
}

function BonusSpawner()
{
	wait(-10);
  randomize();
  var i;
  while (FStartGame==1)
  { 
    i = random(6);
    if (i<3){wait(-5);}else
    if (i < 4)
    {
    	if (FBulletLvl<4 || (FBulletLvl==4 && FBulletType!=1)){
      ent_create("ammobox.mdl", vector(random(2000) - 1000, 2000, 0), AAmmo1);}
    }else
    if (i < 5)
    {
    	if (FBulletLvl<4 || (FBulletLvl==4 && FBulletType!=2)){
      ent_create("ammo.mdl", vector(random(2000) - 1000, 2000, 0), AAmmo2);}
    }
    else
    {
    	if (FPlLife<3 || FPlLifeType==1){
      ent_create("healthbox.MDL", vector(random(2000) - 1000, 2000, 0), AHeal);}
    }
    wait(-5);
  }
}

function FStart()
{if (FStartGame==0)
{
	FStartGame=1;
	FScore=0;
	FPlLife=2;
	FBulletLvl=1;
	FBulletType=1;
	player.x=0;
	player.y=0;
	player.z=0;
	wait(1);
	StartCameraAnim();
  AsteroSpawner();
  BonusSpawner();
   reset(logo_panel,VISIBLE);
  reset(start_panel,VISIBLE);
  reset(score_pan,VISIBLE);
}
}

function onpause()
{
	if (FStartGame>0)
	{
	if (freeze_mode==2){reset(pause_panel,VISIBLE);freeze_mode=0;}else{set(pause_panel,VISIBLE);freeze_mode=2;}
}
else
{sys_exit(0);}
}

function main()
{
  vec_set(screen_color, vector(1, 1, 1)); // dark blue
  vec_set(sky_color, vector(1, 1, 1)); // dark blue
  video_window(NULL, NULL, 0, "PJ187");
  video_mode = 12;
  panel_red.alpha=0;
  video_screen = 1;
  d3d_antialias = 1;
  shadow_stencil = 3;
  fps_max = 60;
  level_load("level.wmb");
  	while(!player){wait(1);}
 wait(1);
 
  set_start_screen();

 

 
  start_panel.pos_x = ((screen_size.x)/2)-200; 
  start_panel.pos_y = ((screen_size.y)/1.5);
   reset(pause_panel,VISIBLE);
  pause_panel.pos_x = ((screen_size.x)/2)-100; 
  pause_panel.pos_y = ((screen_size.y)/2);
  

  logo_panel.pos_x = ((screen_size.x)/2)-256; 
  logo_panel.pos_y = ((screen_size.y)/4)-256;
  
  score_pan.pos_x = ((screen_size.x)/2)-200; 
  score_pan.pos_y = ((screen_size.y)/3);
   set(logo_panel,VISIBLE);  
  
  
  set(start_panel,VISIBLE);
  on_esc=onpause;
  on_enter=FStart;

game_load("score",0);

  	FScore=0;
	FPlLife=2;
	FHitTimer=0;

  //StartCameraAnim();
 // AsteroSpawner();
 // BonusSpawner();
  media_loop("GalacticGunner.mp3", NULL, 100);
  
}
