

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
    if (FHitTimer < 1)
    {
      snd_play(SNDhitPl, 100, 0);
      snd_play(SNDbuzzer, 20, 0);
      FHitTimer = 50;
      if (FPlLifeType == 1)
      {
        FPlLife -= 1;
      }
      else {
        FPlLifeType = 1;
      }
      FBulletLvl = 1;
    }
    if (FPlLife < 1) {
      if (FScore > FTopScore) {
        FTopScore = FScore;
      } FLastScore = FScore;
      set(score_pan, VISIBLE);
      reset(score_panel, VISIBLE);
      FStartGame = 0;
      game_save("score", 0, SV_VARS);
    }
    ent_remove (my);
  }
  if (you)
  {
    if (is(you, FLAG2))
    {
    }
    else
    {
      my.event = NULL;
      set (my, INVISIBLE);
      set (my, PASSABLE);
      my.skill2 = 30;
      effect(r_part_func, 10, my.x, nullvector);

      wait (1);
      if (you)
        you.skill1 -= 100;


    }
  }


}


action AAsteroSml()
{
  my.group = 2;
  my.skill1 = 20;
  set(my, POLYGON);
  c_ignore(2, 0);
  my.emask |= ( ENABLE_ENTITY);
  my.event = AttackPlrEvent;
  wait(2);
  while ((FStartGame == 1) && (my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
    effect(smoke_part_func, 1, my.x, nullvector);
    my.roll += 10 * time_step;
    c_move(me, vector(30 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
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
  while ((FStartGame == 1) && (my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
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
  var i = 0;
  wait(2);
  while ((FStartGame == 1) && (my.skill1 > 0) && (!(my.eflags & CLIPPED)) )
  {
    effect(r_part_func, 1, my.x, nullvector);
    vec_set(temp, player.x);
    vec_sub(temp, my.x);
    vec_to_angle(i, temp);
    if (i < 180) {
      if (i > my.pan) {
        my.pan += 4 * time_step;
      } else {
        my.pan -= 4 * time_step;
      }
    }
    else
    {
      if (i > my.pan) {
        my.pan -= 4 * time_step;
      } else {
        my.pan += 4 * time_step;
      }
    }

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
  while ((FStartGame == 1) && (my.skill1 > 0) && (my.y > -500) )
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
  while ((FStartGame == 1) && (my.skill1 > 0) && (my.y > -500) )
  {
    c_move(me, vector(15 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    ltimer += time_step;

    if (ltimer > 35 - (FBulletLvl * 3) - (FPlLifeType * 4))
    {

      LBullet = ent_create("bolt.mdl", my.x, ABlt1);
      vec_to_angle(LBullet.pan, vec_diff(NULL, player.x, my.x));
      ltimer = 0;
    }

    wait(1);
  }
  if (my.skill1 < 1) {
    FScore += 500;
  }

  //  wait(-1);
  set(my, PASSABLE);
  set(my, INVISIBLE);
  if  (!(my.eflags & CLIPPED))
    ent_create("effekt_a.mdl", my.x, AExplo);
  // wait(-1);
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
  while ((FStartGame == 1) && (my.skill1 > 0) && (my.y > -500) )
  {
    c_move(me, vector(20 * time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
    ltimer += time_step;

    if (ltimer > 40 - (FBulletLvl * 4))
    {

      LBullet = ent_create("rocket.mdl", my.x, ARocket);
      vec_to_angle(LBullet.pan, vec_diff(NULL, player.x, my.x));
      ltimer = 0;
    }

    wait(1);
  }
  if (my.skill1 < 1) {
    FScore += 500;
  }
  //  wait(-1);
  set(my, PASSABLE);
  set(my, INVISIBLE);
  if  (!(my.eflags & CLIPPED))
    ent_create("effekt_a.mdl", my.x, AExplo);
  // wait(-1);
  ent_remove(me);

}