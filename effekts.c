MATERIAL* matGlow = { effect = "object.fx"; } // a normalmapping + glow shader


action AExplo()
{

  my.albedo = 400;
  set(my, PASSABLE);
  set(my, TRANSLUCENT);
  //  set(my,BRIGHT);
  //my.material = matGlow;
  my.red = 1255;
  my.green = 1255;
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



function flm_part_fader(PARTICLE *p)
{
  p.alpha -= (20 * time_step);
  if (p.alpha <= 0) {
    p.lifespan = 0; // remove
  }
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
  //  set(p,OVERLAY);
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
  //  set(p,OVERLAY);
  set(p, MOVE);
  set(p, TRANSLUCENT);

  p.event = flm_part_fader;
}

function bl_part_func(PARTICLE *p)
{
  p.bmap = bmp_b_part;
  p.size = 32;
    p.blue = 100;
  p.red = 0;
  p.green = 400;

  p.x += 0;
  p.y += 0;
  p.z += 0;

  p.vel_x = 0;
  p.vel_y = 0;
  p.vel_z = 0;

  p.alpha = 100;
  set(p, BRIGHT);
  //  set(p,OVERLAY);
  set(p, MOVE);
  set(p, TRANSLUCENT);

  p.event = flm_part_fader;
}

function gr_part_func(PARTICLE *p)
{
  p.bmap = bmp_b_part;
  p.size = 32;
  p.blue = 0;
  p.red = 10;
  p.green = 400;

  p.x += 0;
  p.y += 0;
  p.z += 0;

  p.vel_x = 0;
  p.vel_y = 0;
  p.vel_z = 0;

  p.alpha = 100;
  set(p, BRIGHT);
  //  set(p,OVERLAY);
  set(p, MOVE);
  set(p, TRANSLUCENT);

  p.event = flm_part_fader;
}


function engine_part_fader(PARTICLE *p)
{
  p.alpha -= (40 * time_step);
  if (p.alpha <= 0) {
    p.lifespan = 0; // remove
  }
}

function engine_func(PARTICLE *p)
{
  p.bmap = bmp_b_part;
  p.size = 16;
  p.blue = 38;

  var i = integer(random(3));

  if (i == 0)
  {
    p.x += 0;
    p.y -= 67;
    p.z += 0;
  }

  if (i == 1)
  {
    p.x += 73;
    p.y -= 50;
    p.z += 0;
  }

  if (i == 2)
  {
    p.x -= 73;
    p.y -= 50;
    p.z += 0;
  }

  p.vel_x = 2 - (random(4));
  p.vel_y = -10;
  p.vel_z = 2 - (random(4));

  p.alpha = 100;
  set(p, BRIGHT);
  //  set(p,OVERLAY);
  set(p, MOVE);
  set(p, TRANSLUCENT);

  p.event = engine_part_fader;
}

function engine2_func(PARTICLE *p)
{
  p.bmap = bmp_b_part;
  p.size = 16;
  p.blue = 38;
  p.lifespan = 200;
  var i = integer(random(3));

  if (i == 0)
  {
    p.x += 0;
    p.y -= 67;
    p.z += 6;
  }

  if (i == 1)
  {
    p.x += 23;
    p.y -= 67;
    p.z += 0;
  }

  if (i == 2)
  {
    p.x -= 23;
    p.y -= 67;
    p.z += 0;
  }

  p.vel_x = 2 - (random(4));
  p.vel_y = -10;
  p.vel_z = 2 - (random(4));

  p.alpha = 100;
  set(p, BRIGHT);
  //  set(p,OVERLAY);
  set(p, MOVE);
  set(p, TRANSLUCENT);

  p.event = engine_part_fader;
}

function r_part_func(PARTICLE *p)
{
  p.bmap = bmp_r_part;
  p.size = 32;
   p.blue = 0;
  p.red = 400;
  p.green = 50;
  

  p.x += 0;
  p.y += 0;
  p.z += 0;

  p.vel_x = random(3) - 6;
  p.vel_y = random(3) - 6;
  p.vel_z = random(3) - 6;

  p.alpha = 100;
  set(p, BRIGHT);
  //  set(p,OVERLAY);
  set(p, MOVE);
  set(p, TRANSLUCENT);

  p.event = flm_part_fader;
}
