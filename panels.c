BMAP* logo_bmap = "logo.png";
BMAP* vtz_bmap = "vtz.png";

PANEL* panel_vtz =
{
  pos_x = 0; pos_y = 0;
  size_x = 512; size_y = 512;
  layer = 5;
  flags = LIGHT | TRANSLUCENT | SHOW;
  bmap = vtz_bmap;
}
PANEL* panel_blue =
{
  pos_x = 0; pos_y = 0;
  size_x = 2000; size_y = 2000;
  red = 27;
  green = 27;
  blue = 35;
  layer = 2;
  flags = LIGHT | TRANSLUCENT | SHOW;

}

PANEL* panel_red =
{
  pos_x = 0; pos_y = 0;
  size_x = 2000; size_y = 2000;
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

PANEL* logo_panel =
{
  pos_x = 0;
  pos_y = 0;
  digits(180, 0, "v1.1 (c) 2024 VTZ Games", "Arial#20bi", 1, NULL);
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


