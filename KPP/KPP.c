
/*
//////////////////////////////

This is an early, not complete version!

 ________________________________________________________
[          Kartoffel's PostProcessing Shaders            ]
|--------------------------------------------------------|
||                                                      ||
||  A ready to use shaderpack containing some basic HQ  ||
||  postprocessing shaders.                             ||
||                                                      ||
||  --------------------------------------------------  ||
||                                                      ||
||  This shaderpack is free.                            ||
||                                                      ||
||  You may use this in any project (commercial and     ||
||  free) but you have to mention somwhere (for example ||
||  on the startup-screen or credits) that these        ||
||  shaders were created by me.                         ||
||                                                      ||
||  --------------------------------------------------  ||
||                                                      ||
||  Copyright (C) 2013 by Alex Pölloth aka 'Kartoffel'  ||
||             mail: alex.poelloth@yahoo.de             ||
||                                                      ||
||          ( do not remove this information )          ||
\\______________________________________________________//
 `------------------------------------------------------´
 
 	Features:
 	
 - True HDR Rendering
 - HQ Bloom (2 layers)
 - Zoom Blur
 - Vignette Effect
 - Video Grain Effect
 - Color Postprocessing (saturation / gamma correction / tonemapping)
 - Extremely inefficient Supersampling-Antialiasing
 
 	Functions:
 
 KPP_Enable();      // Enables the shaders (set resolution before calling this)
 
 KPP_Disable();     // currently buggy :[   -   Disables the shaders
 
 KPP_Renew();       // buggy aswell...   -   restarts the shaders (needed after a resolution change!)
 
 KPP_Toggle();      // Toggles the shaders (but again, doesn't work at the moment)
 
 
 #define SSAA 4.0     // (EXTREMELY SLOW!) putting this in your code BEFORE #include "KPP.c" will enable SuperSampling-Antialiasing
                         this is the best (quality), easiest and slowest antialiasing method
                         number == factor (4 is nice, you shouldn't go higher than 16)
 
 
/*/ // for some settings read this:


#define RENDER_TARGET_FORMAT 12222 // 12222 HDRR (recommended); 32 -> no HDRR (possible but won't look good)


byte KPP_Running = 0; // with this you can check if (read only, don't change this manually!)


/////// Settings (it's probably better to change these at runtime, not here):
// settings marked with a '*' are fine adjusted and look good how they are

var KPP_GammaAdjust = 0.0; // apply gamma correction (gamma corrction messes up the final image!) - (x<0)darker  (x>0)brighter  (x==0)no gamma corection
var KPP_Saturation = 1.0; // overall-saturation, 1 == 100%

var KPP_VideoGrainStrength = 0.275; // * intensity of the video grain effect (also acts as dithering! so keep it above 0.175)
var KPP_VignetteStrength = 0.600; // * intensity of the vignette effect (darkens the edges)

var KPP_ZoomBlurStrength = 0; // intensity of the zoom-blur effect, 0 disables it
var KPP_ZoomBlurPos_X = 0.500; // X position of the zoom blur (0.5 is the middle)
var KPP_ZoomBlurPos_Y = 0.500; // Y... [same as above]

var KPP_BloomThreshold = 0.785; // * how bright something has to be to get "bloomed" (1.0 = 255)
var KPP_BloomStrength = 4.0; // * bloom intensity - the shaders are fine adjusted to 4.0!
var KPP_BloomSaturation = 0.75; // * saturation of the bloom ()

#define RENDER_TARGET_FORMAT 12222 // 12222 HDRR (recommended); 32 no HDRR (possible but won't look good)



// ===> stuff you should know ends here, ugly code ahead
// (and don't change anything below until you REALLY know what you're doing)



#define SSAA_SHARPNESS_1 25
#define SSAA_SHARPNESS_2 35
#define SSAA_SHARPNESS_3 45
#define SSAA_SHARPNESS_4 55

#define SSAA_SHARPNESS_OPTIMAL 35

#ifndef SSAA_SHARPNESS
	#define SSAA_SHARPNESS SSAA_SHARPNESS_OPTIMAL
#endif

#ifdef SSAA
	BMAP * tAA;
#endif

BMAP * tCamOut; // Camera's output
BMAP * tBloomCut; // Highpass
BMAP * tBlurZ1; // Zoom Blur pass 1
BMAP * tBlurZ2; // Zoom Blur pass 2
BMAP * tDS; // Downsample Pass
BMAP * tBlurH1; // Bloom Blur Horizontal
BMAP * tBlur1; // Bloom Blur Output
BMAP * tDS2; // 2nd Downsample Pass
BMAP * tBlurH2; // Bloom Blur 2 Horizontal
BMAP * tBlur2; // Bloom Blur 2 Output

BMAP * tNoise = "KPP_noise.bmp"; // Video Grain Texture

//#define DEBUG_CHAIN

#define BLUR_FACTOR 2
#define DOWNSAMPLE_STEP 4

#ifdef DEBUG_CHAIN
	#define DEBUG_CHAIN_FNC while(!key_n) wait(1); while(key_n) wait(1)
#else
	void do_nothing() {}
	#define DEBUG_CHAIN_FNC do_nothing()
#endif

//

#ifdef SSAA
	MATERIAL * ppAA = { effect = "KPP_AA.fx"; }
#endif

MATERIAL * ppBloomCut = { effect = "KPP_BCut.fx"; }
MATERIAL * ppBlurZ1 = { effect = "KPP_BlurZ.fx"; }
MATERIAL * ppBlurZ2 = { effect = "KPP_BlurZ.fx"; }
MATERIAL * ppDownsample = { effect = "KPP_DS4x.fx"; flags = AUTORELOAD; }
MATERIAL * ppBlurH1 = { effect = "KPP_Blur.fx"; }
MATERIAL * ppBlurV1 = { effect = "KPP_Blur.fx"; }
MATERIAL * ppDownsample2 = { effect = "KPP_DS4x.fx"; flags = AUTORELOAD; }
MATERIAL * ppBlurH2 = { effect = "KPP_Blur.fx"; }
MATERIAL * ppBlurV2 = { effect = "KPP_Blur.fx"; }
MATERIAL * ppFinal = { effect = "KPP_Final.fx"; }

//

#ifdef SSAA
	VIEW * vAA;
#endif

VIEW * vBloomCut;
VIEW * vBlurZ1;
VIEW * vBlurZ2;
VIEW * vDownsample;
VIEW * vBlurH1;
VIEW * vBlurV1;
VIEW * vDownsample2;
VIEW * vBlurH2;
VIEW * vBlurV2;
VIEW * vFinal;

//

VIEW * pp_view_create_lastview; // used to link automatically
VIEW * pp_view_create_r(var layer, var size_x, var size_y, MATERIAL * mat, BMAP * output)
{
	VIEW * v = view_create(2);
	
	set(v, PROCESS_TARGET);
	set(v, UNTOUCHABLE);
	set(v, NOSHADOW);
	set(v, CHILD);
	set(v, SHOW);
	set(v, ENABLE_VIEW);
	
	reset(v, AUDIBLE);
	
	if(size_x >= 0)
		v.size_x = size_x;
	else
		v.size_x = -screen_size.x / size_x;
	
	if(size_y >= 0)
		v.size_y = size_y;
	else
		v.size_y = -screen_size.y / size_y;
	
	v.material = mat;
	
	pp_view_create_lastview.stage = v;
	
	v.bmap = output;
	
	pp_view_create_lastview = v;
}

void pp_view_create(VIEW * v, var size_x, var size_y, MATERIAL * mat, BMAP * output)
{
	set(v, PROCESS_TARGET);
	set(v, UNTOUCHABLE);
	set(v, NOSHADOW);
	set(v, CHILD);
	set(v, SHOW);
	set(v, ENABLE_VIEW);
	
	reset(v, AUDIBLE);
	
	if(size_x >= 0)
		v.size_x = size_x;
	else
		v.size_x = -screen_size.x * size_x;
	
	if(size_y >= 0)
		v.size_y = size_y;
	else
		v.size_y = -screen_size.y * size_y;
	
	v.material = mat;
	
	pp_view_create_lastview.stage = v;
	
	DEBUG_CHAIN_FNC;
	
	v.bmap = output;
	
	pp_view_create_lastview = v;
}

void KPP_Enable()
{
	if(KPP_Running) return;
	
	KPP_Running = 1;
	
	var ds_step_x = integer(screen_size.x / DOWNSAMPLE_STEP);
	var ds_step_y = integer(screen_size.y / DOWNSAMPLE_STEP);
	
	var ds_step_x2 = integer(screen_size.x / (DOWNSAMPLE_STEP * DOWNSAMPLE_STEP));
	var ds_step_y2 = integer(screen_size.y / (DOWNSAMPLE_STEP * DOWNSAMPLE_STEP));
	
	//
	
	#ifdef SSAA
		var SSAA_SizeX = integer(screen_size.x * sqrt(SSAA));
		var SSAA_SizeY = integer(screen_size.y * sqrt(SSAA));
		tAA = bmap_createblack(SSAA_SizeX, SSAA_SizeY, RENDER_TARGET_FORMAT);
	#endif
	
	tCamOut = bmap_createblack(screen_size.x, screen_size.y, RENDER_TARGET_FORMAT);
	
	tBlurZ1 = bmap_createblack(screen_size.x, screen_size.y, RENDER_TARGET_FORMAT);
	tBlurZ2 = bmap_createblack(screen_size.x, screen_size.y, RENDER_TARGET_FORMAT);
	
	tBloomCut = bmap_createblack(screen_size.x, screen_size.y, RENDER_TARGET_FORMAT);
	
	tDS = bmap_createblack(ds_step_x, ds_step_y, RENDER_TARGET_FORMAT);
	
	tBlurH1 = bmap_createblack(ds_step_x, ds_step_y, RENDER_TARGET_FORMAT);
	tBlur1 = bmap_createblack(ds_step_x, ds_step_y, RENDER_TARGET_FORMAT);
	
	tDS2 = bmap_createblack(ds_step_x2, ds_step_y2, RENDER_TARGET_FORMAT);
	
	tBlurH2 = bmap_createblack(ds_step_x2, ds_step_y2, RENDER_TARGET_FORMAT);
	tBlur2 = bmap_createblack(ds_step_x2, ds_step_y2, RENDER_TARGET_FORMAT);
	
	//
	
	#ifdef SSAA
		ppAA.skin1 = tAA;
		ppAA.skill1 = floatv(SSAA_SizeX / screen_size.x);
		ppAA.skill2 = floatv(SSAA_SizeY / screen_size.y);
	#endif
	
	ppBlurZ1.skin1 = tCamOut;
	ppBlurZ2.skin1 = tBlurZ1;
	
	ppBloomCut.skin1 = tBlurZ2;
	ppBloomCut.skill1 = floatv(KPP_BloomThreshold);
	ppBloomCut.skill2 = floatv(KPP_BloomStrength);
	ppBloomCut.skill3 = floatv(KPP_BloomSaturation);
	
	ppDownsample.skin1 = tBloomCut;
	ppDownsample.skill1 = floatv(screen_size.x / ds_step_x);
	ppDownsample.skill2 = floatv(screen_size.y / ds_step_y);
	
	ppBlurH1.skin1 = tDS;
	ppBlurH1.skill1 = floatv( 2 * BLUR_FACTOR );
	ppBlurH1.skill2 = floatv( 0 );
	ppBlurV1.skin1 = tBlurH1;
	ppBlurV1.skill1 = floatv( 0 );
	ppBlurV1.skill2 = floatv( 2 * BLUR_FACTOR );
	
	ppDownsample2.skin1 = tBlur1;
	ppDownsample2.skill1 = floatv(screen_size.x / ds_step_x);
	ppDownsample2.skill2 = floatv(screen_size.y / ds_step_y);
	
	ppBlurH2.skin1 = tDS2;
	ppBlurH2.skill1 = floatv( 5.6 * BLUR_FACTOR );
	ppBlurH2.skill2 = floatv( 5.6 * BLUR_FACTOR );
	ppBlurV2.skin1 = tBlurH2;
	ppBlurV2.skill1 = floatv( 5.6 * BLUR_FACTOR );
	ppBlurV2.skill2 = floatv(-5.6 * BLUR_FACTOR);
	
	ppFinal.skin1 = tBlurZ2;
	ppFinal.skin2 = tBlur1;
	ppFinal.skin3 = tBlur2;
	
	ppFinal.skill1 = floatv(screen_size.x / ds_step_x);
	ppFinal.skill2 = floatv(screen_size.y / ds_step_y);
	ppFinal.skill3 = floatv(screen_size.x / ds_step_x2);
	ppFinal.skill4 = floatv(screen_size.y / ds_step_y2);
	
	//
	
	#ifdef SSAA
		camera.size_x = SSAA_SizeX;
		camera.size_y = SSAA_SizeY;
		
		camera.bmap = tAA;
		
		ppAA.skill3 = floatv(SSAA_SHARPNESS);
	#else
		camera.bmap = tCamOut;
	#endif
	
	//
	
	pp_view_create_lastview = camera;
	
	#ifdef SSAA
		vAA = pp_view_create_r(2, camera.size_x, camera.size_y, ppAA, tCamOut);
	#endif
	
	vBlurZ1 = pp_view_create_r(2, -1, -1, ppBlurZ1, tBlurZ1);
	vBlurZ2 = pp_view_create_r(2, -1, -1, ppBlurZ2, tBlurZ2);
	
	vBloomCut = pp_view_create_r(2, -1, -1, ppBloomCut, tBloomCut);
	
	vDownsample = pp_view_create_r(2, ds_step_x, ds_step_y, ppDownsample, tDS);
	
	vBlurH1 = pp_view_create_r(2, ds_step_x, ds_step_y, ppBlurH1, tBlurH1);
	vBlurV1 = pp_view_create_r(2, ds_step_x, ds_step_y, ppBlurV1, tBlur1);
	
	vDownsample2 = pp_view_create_r(2, ds_step_x2, ds_step_y2, ppDownsample2, tDS2);
	
	vBlurH2 = pp_view_create_r(2, ds_step_x2, ds_step_y2, ppBlurH2, tBlurH2);
	vBlurV2 = pp_view_create_r(2, ds_step_x2, ds_step_y2, ppBlurV2, tBlur2);
	
	vFinal = pp_view_create_r(3, -1, -1, ppFinal, vFinal);
	
	while(KPP_Running)
	{
		
		ppBlurZ1.skill1 = floatv(KPP_ZoomBlurPos_X);
		ppBlurZ1.skill2 = floatv(KPP_ZoomBlurPos_Y);
		ppBlurZ1.skill3 = floatv(KPP_ZoomBlurStrength * 2);
		
		ppBlurZ2.skill1 = floatv(KPP_ZoomBlurPos_X);
		ppBlurZ2.skill2 = floatv(KPP_ZoomBlurPos_Y);
		ppBlurZ2.skill3 = floatv(KPP_ZoomBlurStrength);
		
		
		ppBloomCut.skill1 = floatv(KPP_BloomThreshold);
		ppBloomCut.skill2 = floatv(KPP_BloomStrength);
		ppBloomCut.skill3 = floatv(KPP_BloomSaturation);
		
		
		ppFinal.skill5 = floatv(random(1));
		ppFinal.skill6 = floatv(random(1));
		
		ppFinal.skill7 = floatv(KPP_VideoGrainStrength);
		ppFinal.skill8 = floatv(KPP_VignetteStrength);
		ppFinal.skill9 = floatv(KPP_Saturation);
		ppFinal.skill10 = floatv(-KPP_GammaAdjust / 10);
		
		
		#ifdef SSAA
			ppAA.skill3 = floatv(SSAA_SHARPNESS);
		#endif
		
		wait(1);
	}
}

void KPP_Disable()
{
	if(!KPP_Running) return;
	
	camera.bmap = NULL;
	camera.stage = NULL;
	
	camera.size_x = screen_size.x;
	camera.size_y = screen_size.y;
	
	#ifdef SSAA
		ptr_remove(vAA);
	#endif
	ptr_remove(vBlurZ1);
	ptr_remove(vBlurZ2);
	ptr_remove(vBloomCut);
	ptr_remove(vDownsample);
	ptr_remove(vBlurH1);
	ptr_remove(vBlurV1);
	ptr_remove(vDownsample2);
	ptr_remove(vBlurH2);
	ptr_remove(vBlurV2);
	ptr_remove(vFinal);
	
	#ifdef SSAA
		bmap_purge(tAA);
	#endif
	bmap_purge(tCamOut);
	bmap_purge(tBlurZ1);
	bmap_purge(tBlurZ2);
	bmap_purge(tBloomCut);
	bmap_purge(tDS);
	bmap_purge(tBlurH1);
	bmap_purge(tBlur1);
	bmap_purge(tDS2);
	bmap_purge(tBlurH2);
	bmap_purge(tBlur2);
	
	KPP_Running = 0;
}

void KPP_Renew()
{
	KPP_Disable();
	
	wait(1);
	
	KPP_Enable();
}

void KPP_Toggle()
{
	if(KPP_Running)
		KPP_Disable();
	else
		KPP_Enable();
}