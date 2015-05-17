/*
    wmsensors.c - Part of wmsensors, a Linux utility for monitoring sensors.
    Copyright (c) 1998,1999  Adrian Baugh <adrian.baugh@keble.ox.ac.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <fcntl.h>
#include <X11/Xatom.h>
#include "sensors/sensors.h"
#include "sensors/chips.h"
#include "sensors/error.h"

#include "back.xpm"
#include "mask2.xbm"
#include "mask.xpm"

#define major_VER 1
#define minor_VER 0
#define patch_VER 4
#define MW_EVENTS   (ExposureMask | ButtonPressMask | StructureNotifyMask)
#define FALSE 0
#define Shape(num) (ONLYSHAPE ? num-5 : num)
#define DEFAULT_CONFIG_FILE_NAME "sensors.conf"

/* Global Data storage/structures ********************************************/
int ONLYSHAPE=0; /* default value is noshape */
int updatespeed = 4;
static char *help_message[] = {
"where options include:",
"    -a [command]            turn on alarm-activated code",
"    -l                      turn on multiple LM75 temperature graphs",
"    -u <secs>               updatespeed",
"    -e <program>            program to start on middle-click",
"    -p [+|-]x[+|-]y         position of wmsensors",
"    -r [filename]           record data in a log file",
"    -s                      without groundplate",
"    -i                      start up as icon",
"    -w                      start up withdrawn",
"    -v                      output version",
NULL
};

/* X11 Variables *************************************************************/
Display *dpy;	  /* which DISPLAY */
Window Root;      /* Background-Drawable */
int screen;
int x_fd;
int d_depth;
XSizeHints mysizehints;
XWMHints mywmhints;
Pixel back_pix, fore_pix;
GC NormalGC;
Window iconwin, win;       /* My home is my window */
char *ProgName;
char *Geometry;
char *LedColor = "LightSeaGreen";

/* Thanks to Lars Kellogg-Stedman for removing a kluge from the next line */
char Execute1[] = "xterm -T wmsensors -e sh -c 'sensors | less' &";

char Execute2[] = "/usr/local/bin/sensors -s";
char Execute3[] = "xterm -T wmsensors -e man wmsensors &";
char *ExecuteAlarm;
char *ERR_colorcells = "not enough free color cells\n";
char *ampers = " &";
static char *config_file_name;
FILE *config_file;
FILE *log_file;
char *log_filename;
int log_status;
int count_printings = 0;
static const char *config_file_path[] =
{ "/etc", "/usr/lib/sensors", "/usr/local/lib/sensors", "/usr/lib",
  "/usr/local/lib", ".", 0 };

/* XPM Structures & Variables ************************************************/
typedef struct _XpmIcon {
    Pixmap pixmap;
    Pixmap mask;
    XpmAttributes attributes;
}        XpmIcon;

XpmIcon wmsensors;
XpmIcon visible;
time_t actualtime;
long actualmin;

/* Function definitions ******************************************************/
void GetXPM(void);
Pixel GetColor(char *name);
void RedrawWindow( XpmIcon *v);
void InitLm();
void InsertLm();

/*****************************************************************************/
/* Source Code <--> Function Implementations                                 */
/*****************************************************************************/
void usage()
{
  char **cpp;

  fprintf(stderr,"\nusage:  %s [-options ...] \n", ProgName);
  for (cpp = help_message; *cpp; cpp++) {
    fprintf(stderr, "%s\n", *cpp);
  }
  fprintf(stderr,"\n");
  exit(1);
}

/* This examines global var config_file, and leaves the name there too.
   It also opens config_file. */
int open_this_config_file(char *filename)
{
  config_file = fopen(filename,"r");
  if (! config_file)
    return -errno;
  return 0;
}

void open_config_file(void)
{
#define MAX_FILENAME_LEN 1024
  char *filename;
  char buffer[MAX_FILENAME_LEN];
  int res,i;

  if (config_file_name && !strcmp(config_file_name,"-")) {
    config_file = stdin;
    return;
  } else if (config_file_name && index(config_file_name,'/')) {
    if ((res = open_this_config_file(config_file_name))) {
      fprintf(stderr,"Could not locate or open config file\n");
      fprintf(stderr,"%s: %s\n",config_file_name,strerror(res));
      exit(1);
    }
  }
  else {
    if (config_file_name)
      filename = config_file_name;
    else
      filename = strdup(DEFAULT_CONFIG_FILE_NAME);
    for (i = 0; config_file_path[i]; i++) {
      if ((snprintf(buffer,MAX_FILENAME_LEN,
		    "%s/%s",config_file_path[i],filename)) < 1) {
        fprintf(stderr,
                "open_config_file: ridiculous long config file name!\n");
        exit(1);
      }
      if (!open_this_config_file(buffer)) {
        free(config_file_name);
        config_file_name = strdup(buffer);
        return;
      }
    }
    fprintf(stderr,"Could not locate or open config file!\n");
    exit(1);
  }
}

int AlarmStatus;  /* Audible beeps should be on by default as they */
                  /* may indicate an urgent problem.               */
int AlarmFlag;    /* This is the "Current alarms" flag             */
int AlarmBeeping; /* This shows whether the alarm is on            */

int main(int argc,char *argv[])
{
  int res, i;
  int multiple_lm75 = 0;
  unsigned int borderwidth ;
  char *display_name = NULL; 
  char *wname = "wmsensors";
  XGCValues gcv;
  unsigned long gcm;
  XEvent Event;
  XTextProperty name;
  XClassHint classHint;
  Pixmap pixmask;

  Geometry = "";
  mywmhints.initial_state = NormalState;
  AlarmStatus = 0;
  AlarmFlag = 1;
  AlarmBeeping = 0;
  log_status = 0;
  /* Parse command line options */
  ProgName = argv[0];

  for(i=1;i<argc;i++) {
    char *arg= argv[i];

    if (arg[0] == '-') {
      switch(arg[1]) {
      case 'a':
        AlarmStatus = 1;
	if (++i >= argc) usage();
	else
       	    ExecuteAlarm = argv[i];
        continue;
      case 'u':
        if(++i >=argc) usage();
        sscanf(argv[i], "%d", &updatespeed);
        continue;
      case 'r':
	if (++i >=argc) log_filename = "wmsensors.log";
	else {
	  if (!strcmp(argv[i],"-")) {
	    log_filename = "stdout";
	    log_file = stdout;
	  }
	  else
	    log_filename = argv[i];
	}
	log_status = 1;
	if (strcmp(log_filename, "stdout"))
	  if (!(log_file = fopen(log_filename, "w"))) {
	    log_status = 0;
	    fprintf(stderr,"Unable to write log file. Continuing anyway...\n");
	  }
	continue;
      case 'e':
        if(++i >=argc) usage();
        strcpy(&Execute2[0], argv[i]);
        strcat(&Execute2[0], " &");
        continue;
      case 's':
        ONLYSHAPE=1;
        continue;
      case 'p':
        if(++i >=argc) usage();
        Geometry = argv[i];
        continue;
      case 'i':
        mywmhints.initial_state = IconicState;
        continue;
      case 'l':
        multiple_lm75 = 1;
        continue;
      case 'w':
        mywmhints.initial_state = WithdrawnState;
        continue;
      case 'v':
        fprintf(stdout, "\nwmsensors version: %i.%i.%i\n", major_VER, minor_VER, patch_VER);
        if(argc == 2) exit(0);
        continue;
      case 'c':
        if(++i >=argc) usage();
        strcpy(&config_file_name[0], argv[i]);
        break;
      default:
        usage();
      }
    }
    else
      {
        fprintf(stderr, "\nInvalid argument: %s\n", arg);
        usage();
      }
  }
  /* Open the display */
  if (!(dpy = XOpenDisplay(display_name)))  
    { 
      fprintf(stderr,"wmsensors: can't open display %s\n", 
	      XDisplayName(display_name)); 
      exit (1); 
    } 

    if (log_status)
      fprintf(log_file, "temp1 temp2 temp3 in0  in1  in2  in3  in4   in5    in6   fan1    fan2    fan3\n");

  screen= DefaultScreen(dpy);
  Root = RootWindow(dpy, screen);
  d_depth = DefaultDepth(dpy, screen);
  x_fd = XConnectionNumber(dpy);
  
  /* Convert XPM Data to XImage */
  GetXPM();
  
  /* Create a window to hold the banner */
  mysizehints.flags= USSize|USPosition;
  mysizehints.x = 0;
  mysizehints.y = 0;

  back_pix = GetColor("white");
  fore_pix = GetColor("black");

  XWMGeometry(dpy, screen, Geometry, NULL, (borderwidth =1), &mysizehints,
	      &mysizehints.x,&mysizehints.y,&mysizehints.width,&mysizehints.height, &i); 

  mysizehints.width = wmsensors.attributes.width;
  mysizehints.height= wmsensors.attributes.height;

  win = XCreateSimpleWindow(dpy,Root,mysizehints.x,mysizehints.y,
			    mysizehints.width,mysizehints.height,
			    borderwidth,fore_pix,back_pix);
  iconwin = XCreateSimpleWindow(dpy,win,mysizehints.x,mysizehints.y,
				mysizehints.width,mysizehints.height,
				borderwidth,fore_pix,back_pix);

  /* activate hints */
  XSetWMNormalHints(dpy, win, &mysizehints);
  classHint.res_name =  "wmsensors";
  classHint.res_class = "WMSensors";
  XSetClassHint(dpy, win, &classHint);

  XSelectInput(dpy,win,MW_EVENTS);
  XSelectInput(dpy,iconwin,MW_EVENTS);
  XSetCommand(dpy,win,argv,argc);
  
  if (XStringListToTextProperty(&wname, 1, &name) ==0) {
    fprintf(stderr, "wmsensors: can't allocate window name\n");
    exit(-1);
  }
  XSetWMName(dpy, win, &name);
  
  /* Create a GC for drawing */
  gcm = GCForeground|GCBackground|GCGraphicsExposures;
  gcv.foreground = fore_pix;
  gcv.background = back_pix;
  gcv.graphics_exposures = FALSE;
  NormalGC = XCreateGC(dpy, Root, gcm, &gcv);  

  if (ONLYSHAPE) { /* try to make shaped window here */
    pixmask = XCreateBitmapFromData(dpy, win, mask2_bits, mask2_width, 
				    mask2_height);
    XShapeCombineMask(dpy, win, ShapeBounding, 0, 0, pixmask, ShapeSet);
    XShapeCombineMask(dpy, iconwin, ShapeBounding, 0, 0, pixmask, ShapeSet);
  }
  
  mywmhints.icon_window = iconwin;
  mywmhints.icon_x = mysizehints.x;
  mywmhints.icon_y = mysizehints.y;
  mywmhints.window_group = win;
  mywmhints.flags = StateHint | IconWindowHint | IconPositionHint
      | WindowGroupHint;
  XSetWMHints(dpy, win, &mywmhints); 

  XMapWindow(dpy,win);
  InitLm();
  InsertLm();
  RedrawWindow(&visible);

  open_config_file(); /* Now we must initialise the sensors library */
  if ((res = sensors_init(config_file)))
  {
    if (res == SENSORS_ERR_PROC)
      fprintf(stderr,
              "/proc/sys/dev/sensors/chips or /proc/bus/i2c unreadable:\n"
              "Make sure you have inserted modules sensors.o and i2c-proc.o!");
    else
      fprintf(stderr,"%s\n",sensors_strerror(res));
    exit(1);
  }

  while(1)
    {
      if (actualtime != time(0))
	{
	  actualtime = time(0);
	  
	  if(actualtime % updatespeed == 0)
	    InsertLm(multiple_lm75, AlarmStatus);

	  RedrawWindow(&visible);
	}
      
      /* read a packet */
      while (XPending(dpy))
	{
	  XNextEvent(dpy,&Event);
	  switch(Event.type)
	    {
	    case Expose:
	      if(Event.xexpose.count == 0 )
		RedrawWindow(&visible);
	      break;
	    case ButtonPress:
              if (AlarmFlag && AlarmBeeping)
	      {
		AlarmFlag = 0;
		break;
	      }
	      switch(Event.xbutton.button) {
	        case Button1:
		  system(Execute1);
		  break;
	        case Button2:
		  system(Execute2);
	        case Button3:
		  system(Execute3);
		  break;
	        default:
		  break;
	      }
	      break;
	    case DestroyNotify:
              XFreeGC(dpy, NormalGC);
              XDestroyWindow(dpy, win);
	      XDestroyWindow(dpy, iconwin);
              XCloseDisplay(dpy);
	      exit(0); 
	    default:
	      break;      
	    }
	}
      XFlush(dpy);
#ifdef SYSV
      poll((struct poll *) 0, (size_t) 0, 50);
#else
      usleep(50000L);			/* 5/100 sec */
#endif
    }
  return 0;
}

/*****************************************************************************/
void nocolor(char *a, char *b)
{
 fprintf(stderr,"wmsensors: can't %s %s\n", a,b);
}

/*****************************************************************************/
/* convert the XPMIcons to XImage */
void GetXPM(void)
{
  static char **alt_xpm;
  XColor col;
  XWindowAttributes attributes;
  int ret;
  char tempc1[12],tempc2[12],tempc3[12],tempc4[12],tempc5[12],tempc6[12],tempc7[12];
  float colr,colg,colb;

  alt_xpm =ONLYSHAPE ? mask_xpm : back_xpm;

  /* for the colormap */
  XGetWindowAttributes(dpy,Root,&attributes);

  /* get user-defined color or validate the default */
  if (!XParseColor (dpy, attributes.colormap, LedColor, &col)) 
    {
      nocolor("parse",LedColor);
    }
  else
    {
      /* Make 7 distinct colours (this may as well be done in the xpm
         now because they will not be changeable (7 colours all
         changeable would get horrible) */

      colr = 255;
      colg = 255;
      colb = 0;
      sprintf(tempc1, "W c #%.2x%.2x%.2x", (int)colr, (int)colg, (int)colb);
      back_xpm[51] = tempc1;

      colr = 255;
      colg = 127;
      colb = 0;
      sprintf(tempc2, "V c #%.2x%.2x%.2x", (int)colr, (int)colg, (int)colb);
      back_xpm[50] = tempc2;

      colr = 0;
      colg = 255;
      colb = 255;
      sprintf(tempc3, "U c #%.2x%.2x%.2x", (int)colr, (int)colg, (int)colb);
      back_xpm[49] = tempc3;

      colr = 208;
      colg = 0;
      colb = 255;
      sprintf(tempc4, "T c #%.2x%.2x%.2x", (int)colr, (int)colg, (int)colb);
      back_xpm[48] = tempc4;

      colr = 196;
      colg = 196;
      colb = 196;
      sprintf(tempc5, "S c #%.2x%.2x%.2x", (int)colr, (int)colg, (int)colb);
      back_xpm[47] = tempc5;

      colr = 0;
      colg = 255;
      colb = 0;
      sprintf(tempc6, "R c #%.2x%.2x%.2x", (int)colr, (int)colg, (int)colb);
      back_xpm[46] = tempc6;

      colr = 255;
      colg = 0;
      colb = 0;
      sprintf(tempc7, "Q c #%.2x%.2x%.2x", (int)colr, (int)colg, (int)colb);
      back_xpm[45] = tempc7;
    }

  wmsensors.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions);
  ret = XpmCreatePixmapFromData(dpy, Root, alt_xpm, &wmsensors.pixmap, 
				&wmsensors.mask, &wmsensors.attributes);
  if(ret != XpmSuccess)
    {fprintf(stderr, ERR_colorcells);exit(1);}

  visible.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions);
  ret = XpmCreatePixmapFromData(dpy, Root, back_xpm, &visible.pixmap, 
				&visible.mask, &visible.attributes);
  if(ret != XpmSuccess)
    {fprintf(stderr, ERR_colorcells);exit(1);}

}

/*****************************************************************************/
/* Removes expose events for a specific window from the queue */
int flush_expose (Window w)
{
  XEvent dummy;
  int i=0;
  
  while (XCheckTypedWindowEvent (dpy, w, Expose, &dummy))i++;
  return i;
}

/*****************************************************************************/
/* Draws the icon window */
void RedrawWindow( XpmIcon *v)
{
  flush_expose (iconwin);
  XCopyArea(dpy,v->pixmap,iconwin,NormalGC,
	    0,0,v->attributes.width, v->attributes.height,0,0);
  flush_expose (win);
  XCopyArea(dpy,v->pixmap,win,NormalGC,
	    0,0,v->attributes.width, v->attributes.height,0,0);

}

/*****************************************************************************/
Pixel GetColor(char *name)
{
  XColor color;
  XWindowAttributes attributes;

  XGetWindowAttributes(dpy,Root,&attributes);
  color.pixel = 0;
   if (!XParseColor (dpy, attributes.colormap, name, &color)) 
     {
       nocolor("parse",name);
     }
   else if(!XAllocColor (dpy, attributes.colormap, &color)) 
     {
       nocolor("alloc",name);
     }
  return color.pixel;
}

/*****************************************************************************/

void InitLm()
{
  /* Save the 14 base colors in wmsensors pixmap */
  XCopyArea(dpy, visible.pixmap, wmsensors.pixmap, NormalGC,
            6,6,15,52, Shape(6), Shape(6));

  /* Copy the base panel to visible */
  XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
	    0,0,mysizehints.width, mysizehints.height, 0 ,0);

  /* Remove the 4 base colors from visible */
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
	    Shape(22),Shape(6),15,52, Shape(6), Shape(6));  

  /* Somewhat tediously put in all the initial guide lines */
  XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
            Shape(16), Shape(8), 1, 1, Shape(31), Shape(11));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(31), Shape(11), 1, 1, Shape(30), Shape(11));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(30), Shape(11), 2, 1, Shape(28), Shape(11));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(28), Shape(11), 4, 1, Shape(24), Shape(11));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(24), Shape(11), 8, 1, Shape(16), Shape(11));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(16), Shape(11), 10, 1, Shape(6), Shape(11));

  /* First one done! Now copy to the others... */

  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 26, 1, Shape(6), Shape(21));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 26, 1, Shape(6), Shape(31));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 26, 1, Shape(6), Shape(42));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 26, 1, Shape(6), Shape(52));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 25, 1, Shape(33), Shape(10));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 25, 1, Shape(33), Shape(18));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 25, 1, Shape(33), Shape(26));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 25, 1, Shape(33), Shape(35));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 25, 1, Shape(33), Shape(44));
  XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
            Shape(6), Shape(11), 25, 1, Shape(33), Shape(53));
}

/************************/
/* GetLimits() function */
/************************/

void GetLimits(double *temp_ll, double *temp2_ul, double *temp3_ul, double *temp1_ul, double *in0_ll, double *in0_ul, double *in1_ll, double *in1_ul, double *in2_ll, double *in2_ul, double *in3_ll, double *in3_ul, double *in4_ll, double *in4_ul, double *in5_ll, double *in5_ul, double *in6_ll, double *in6_ul)
{

  int chip_nr;
  const sensors_chip_name *name;

  /* We set the default limits; these will be used if reading fails. */
  *temp_ll = 20;
  *temp2_ul = 60;
  *temp3_ul = 60;
  *temp1_ul = 60;
  *in0_ll = 1.8;
  *in0_ul = 2.2;
  *in1_ll = 1.8;
  *in1_ul = 2.2;
  *in2_ll = 3.0;
  *in2_ul = 3.6;
  *in3_ll = 4.5;
  *in3_ul = 5.5;
  *in4_ll = 10.80;
  *in4_ul = 13.20;
  *in5_ll = -13.20;
  *in5_ul = -10.80;
  *in6_ll = -5.5;
  *in6_ul = -4.5;

  for (chip_nr = 0; (name=sensors_get_detected_chips(&chip_nr));)
  {
    sensors_get_feature(*name,SENSORS_LM78_IN0_MIN, in0_ll);
    sensors_get_feature(*name,SENSORS_LM78_IN1_MIN, in1_ll);
    sensors_get_feature(*name,SENSORS_LM78_IN2_MIN, in2_ll);
    sensors_get_feature(*name,SENSORS_LM78_IN3_MIN, in3_ll);
    sensors_get_feature(*name,SENSORS_LM78_IN4_MIN, in4_ll);
    sensors_get_feature(*name,SENSORS_LM78_IN5_MIN, in5_ul);
    sensors_get_feature(*name,SENSORS_LM78_IN6_MIN, in6_ul);

    sensors_get_feature(*name,SENSORS_LM78_IN0_MAX, in0_ul);
    sensors_get_feature(*name,SENSORS_LM78_IN1_MAX, in1_ul);
    sensors_get_feature(*name,SENSORS_LM78_IN2_MAX, in2_ul);
    sensors_get_feature(*name,SENSORS_LM78_IN3_MAX, in3_ul);
    sensors_get_feature(*name,SENSORS_LM78_IN4_MAX, in4_ul);
    sensors_get_feature(*name,SENSORS_LM78_IN5_MAX, in5_ll);
    sensors_get_feature(*name,SENSORS_LM78_IN6_MAX, in6_ll);

    sensors_get_feature(*name,SENSORS_LM78_TEMP_OVER, temp1_ul);
    sensors_get_feature(*name,SENSORS_W83781D_TEMP2_OVER, temp2_ul);
    sensors_get_feature(*name,SENSORS_W83781D_TEMP3_OVER, temp3_ul);
  }
}

/********************/
/* GetLm() function */
/********************/

void GetLm(double *temp2, double *temp3, double *temp1, double *in0, double *in1, double *in2, double *in3, double *in6, double *in4, double *in5, double *fan1, double *fan2, double *fan3, int multiple_lm75, double *alrms)
{ 
  int chip_nr;
  const sensors_chip_name *name;
  
  *temp1 = -279;
  *temp2 = -279;
  *temp3 = -279;

  *in0 = -279; 
  *in1 = -279;
  *in2 = -279;
  *in3 = -279;
  *in4 = -279;
  *in5 = -279;
  *in6 = -279;
  *fan1 = 0;
  *fan2 = 0;
  *fan3 = 0;  

  /* Here comes the real code... */

  for (chip_nr = 0; (name=sensors_get_detected_chips(&chip_nr));)
  {
       sensors_get_feature(*name,SENSORS_LM78_IN0, in0);
       sensors_get_feature(*name,SENSORS_LM78_IN1, in1);
       sensors_get_feature(*name,SENSORS_LM78_IN2, in2);
       sensors_get_feature(*name,SENSORS_LM78_IN3, in3);
       sensors_get_feature(*name,SENSORS_LM78_IN4, in4);
       sensors_get_feature(*name,SENSORS_LM78_IN5, in5);
       sensors_get_feature(*name,SENSORS_LM78_IN6, in6);
       sensors_get_feature(*name,SENSORS_LM78_TEMP, temp1);
       sensors_get_feature(*name,SENSORS_LM78_FAN1, fan1);
       sensors_get_feature(*name,SENSORS_LM78_FAN2, fan2);
       sensors_get_feature(*name,SENSORS_LM78_FAN3, fan3);
       sensors_get_feature(*name,SENSORS_LM78_ALARMS, alrms);
       sensors_get_feature(*name,SENSORS_W83781D_TEMP2, temp2);
       sensors_get_feature(*name,SENSORS_W83781D_TEMP3, temp3);
  }
}

/***************************************************************************/

void InsertLm(int multiple_lm75, int AlarmRequired)
{
   double temp2, temp3, temp1, in0, in1, in3, in2, in6, in4, in5;
   double temp2_ul, temp3_ul, temp1_ul, in0_ul, in1_ul, in3_ul, in2_ul;
   double in6_ul, in4_ul, in5_ul, temp_ll;
   double in0_ll, in1_ll, in3_ll, in2_ll, in6_ll, in4_ll, in5_ll;   
   double fan1, fan2, fan3, act;
   double alrms;
   int temp1p, temp2p, temp3p, in0p, in1p, in2p, in3p, in4p, in5p, in6p, fan1p, fan2p, fan3p;
   GetLm(&temp2, &temp3, &temp1, &in0, &in1, &in2, &in3, &in6, &in4, &in5, &fan1, &fan2, &fan3, multiple_lm75, &alrms);
   GetLimits(&temp_ll, &temp2_ul, &temp3_ul, &temp1_ul, &in0_ll, &in0_ul, &in1_ll, &in1_ul, &in2_ll, &in2_ul, &in3_ll, &in3_ul, &in4_ll, &in4_ul, &in5_ll, &in5_ul, &in6_ll, &in6_ul);
   if (temp3==-279 && temp2 !=-279)
     temp3 = temp2;
   /* Before we transform the data, write it to the log file if requested */
   if (log_status) {
     if ((temp1==-279 || temp2==-279 || in3==-279 || in6==-279 || in4==-279
           || in5==-279 || in0==-279 || in1==-279 || in2==-279) && count_printings)
       fprintf(log_file, "# Error ");
     fprintf(log_file, "%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f\n",
             temp1, temp2, temp3, in0, in1, in2, in3, in4, in5, in6, fan1,
             fan2, fan3);
     fflush(log_file);
   }

   /* Sort out whether the alarms need triggering */
   if (((alrms || ((temp1 > temp1_ul) || (temp2 > temp2_ul) || (temp3 > temp3_ul))) && temp1 > -279 && temp2 > -279 && temp3 > -279) && AlarmRequired)
     {
       if (alrms > 0.5)
	 fprintf(stderr,"Alarm! Alarm on IN%.0f\n",alrms-2);
       else
	 fprintf(stderr,"Alarm! Temperature 1: %.2f  Temperature 2: %.2f  Temperature 3: %.2f\n",temp1, temp2, temp3);
       system(ExecuteAlarm);
     }

   /* Convert data into actual pixel values */
   /* Temperatures (for left-hand column) */
/*   fprintf(log_file, "# Pixel conversion.\n"); /**/
   temp2p=((temp2-temp_ll)/3)+42;
   temp3p=((temp3-temp_ll)/3)+42;
   temp1p=((temp1-temp_ll)/3)+42;
   /* Safety checks on temperature levels */
   if (temp1p > 52) temp1p = 51;
   if (temp2p > 52) temp2p = 51;
   if (temp3p > 52) temp3p = 51;

   /* Left hand column */
   in3p=32+(((in3-in3_ll)/(in3_ul-in3_ll))*(43-32));
   in6p=23+(((in6-in6_ll)/(in6_ul-in6_ll))*(32-23));
   in4p=11+(((in4-in4_ll)/(in4_ul-in4_ll))*(22-11));
   in5p= 1+(((in5-in5_ll)/(in5_ul-in5_ll))*(22-12));

   /* Right hand column */
   in0p=44+(((in0-in0_ll)/(in0_ul-in0_ll))*(53-44));
   in1p=37+(((in1-in1_ll)/(in1_ul-in1_ll))*(44-36));
   in2p=29+(((in2-in2_ll)/(in2_ul-in2_ll))*(36-29));

   fan1p=(fan1/625)+19;
   fan2p=(fan2/625)+10;
   fan3p=(fan3/625)+1;

/*   fprintf(log_file, "# Window redraw.\n");   /**/
   /* Move the areas (ie shift the pre-drawn rectangles left) */
   XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
        	Shape(7), Shape(6), 25, 52, Shape(6), Shape(6));
    XCopyArea(dpy, visible.pixmap, visible.pixmap, NormalGC,
                Shape(33), Shape(6), 25, 52, Shape(32), Shape(6));

    /* Blacks out the right-hand columns so we don't get old data copied
left */
    XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(20), Shape(6), 1, 57, Shape(57), Shape(6));
    XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(20), Shape(6), 1, 57, Shape(31), Shape(6));

    /* Draws the dividing line down the middle of the display */
    XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
	      Shape(8), Shape(6), 1, 57, Shape(32), Shape(6));

    /* Draws the grey lines where the normal values of the parameters lie */
    XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(31), Shape(11));
    XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(31), Shape(21));
     XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(31), Shape(31));
     XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(31), Shape(42));
     XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(31), Shape(52));
    XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(57), Shape(10));
    XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(57), Shape(18));
     XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(57), Shape(26));
     XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(57), Shape(35));
     XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(16), Shape(8), 1, 1, Shape(57), Shape(44));
     XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
	      Shape(16), Shape(8), 1, 1, Shape(57), Shape(53));

/*     fprintf(log_file, "# Redrawing graphs.\n"); /**/
    /* CPU temps and motherboard temp */
    act = 58 - temp2p;
    if( temp2 > -100)
      /* Height 1 rectangle */
       XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
	      Shape(6), Shape(6), 1, 1, Shape(31), Shape(act));

    if (multiple_lm75)
    {
      act = 58 - temp3p;
      if ( temp3 > -100)
        XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
               Shape(6), Shape(6), 1, 1, Shape(31), Shape(act));
    }

    act = 58 - temp1p;
    if( temp1 > -100)
       XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
              Shape(6), Shape(6), 1, 1, Shape(31), Shape(act));

    /* in0 */
    act = 58 - in0p;
    if( in0 > -100)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
		Shape(17), Shape(6), 1, 1, Shape(57), Shape(act));

    /* in1 */
    act = 58 - in1p;
    if( in1 > -100)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
		Shape(18), Shape(6), 1, 1, Shape(57), Shape(act));

    /* in2 */
    act = 58 - in2p;
    if( in2 > -100)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
		Shape(15), Shape(6), 1, 1, Shape(57), Shape(act));

    /* in3 */
    act = 58 - in3p;
    if( in3 > -100)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
		Shape(9), Shape(6), 1, 1, Shape(31), Shape(act)); 

    /* in6 */
    act = 58 - in6p;
    if (in6 > -100)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
		Shape(10), Shape(6), 1, 1, Shape(31), Shape(act));

    /* in4 */
    act = 58 - in4p;
    if (in4 > -100)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
                Shape(11), Shape(6), 1, 1, Shape(31), Shape(act));

    /* in5 */
    act = 58 - in5p;
    if (in5 > -100)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
                Shape(12), Shape(6), 1, 1, Shape(31), Shape(act));
 
    /* fan1 */
    act = 58 - fan1p;
    if (fan1p > 20)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
                Shape(13), Shape(6), 1, 1, Shape(57), Shape(act));
    /* fan2 */
    act = 58 - fan2p;
    if (fan2p > 11)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
                Shape(14), Shape(6), 1, 1, Shape(57), Shape(act));
    /* fan3 */
    act = 58 - fan3p;
    if (fan3p > 1)
      XCopyArea(dpy, wmsensors.pixmap, visible.pixmap, NormalGC,
                Shape(19), Shape(6), 1, 1, Shape(57), Shape(act));
    count_printings++;
}









