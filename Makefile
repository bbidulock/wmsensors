# Makefile generated by imake - do not edit!
# $TOG: imake.c /main/97 1997/06/20 20:23:51 kaleb $

# ----------------------------------------------------------------------
# Makefile generated from "Imake.tmpl" and <Imakefile>
# $TOG: Imake.tmpl /main/245 1997/05/20 10:05:47 kaleb $
#
#
#
#
# $XFree86: xc/config/cf/Imake.tmpl,v 3.32.2.11 1998/11/08 09:06:27 dawes Exp $
# ----------------------------------------------------------------------

all::

.SUFFIXES: .i

# $TOG: Imake.cf /main/28 1997/06/25 08:31:36 barstow $
# $XFree86: xc/config/cf/Imake.cf,v 3.34.2.11 1998/12/22 11:23:01 hohndel Exp $

# -----------------------------------------------------------------------
# site-specific configuration parameters that need to come before
# the platform-specific parameters - edit site.def to change

# site:  $XConsortium: site.def /main/revisionist/4 1996/12/31 08:02:07 kaleb $
# site:  $XFree86: xc/config/cf/site.def,v 3.17.2.1 1997/06/22 10:32:21 dawes Exp $

# $XFree86: xc/config/cf/xf86site.def,v 3.101.2.25 1998/11/08 09:06:29 dawes Exp $

# ----------------------------------------------------------------------
# platform-specific configuration parameters - edit linux.cf to change

# platform:  $TOG: linux.cf /main/36 1997/06/16 22:21:03 kaleb $
# platform:  $XFree86: xc/config/cf/linux.cf,v 3.57.2.16 1998/11/08 11:21:49 hohndel Exp $

# operating system:  Linux 2.2.6 i686 [ELF] (2.2.6)
# libc:	(6.1.1)
# binutils:	(29)

# $XConsortium: lnxLib.rules /main/13 1996/09/28 16:11:01 rws $
# $XFree86: xc/config/cf/lnxLib.rules,v 3.28.2.4 1998/12/18 11:56:08 dawes Exp $

# $XFree86: xc/config/cf/xfree86.cf,v 3.129.2.36 1998/11/07 04:42:24 dawes Exp $

# $XConsortium: xfree86.cf /main/34 1996/12/06 11:45:18 rws $

LINKKITDIR = $(USRLIBDIR)/Server
XF98LINKKITDIR = $(USRLIBDIR)/Server

       XF86SRC = $(SERVERSRC)/hw/xfree86
  XF86ACCELSRC = $(XF86SRC)/accel
    XF86COMSRC = $(XF86SRC)/common
 XF86CONFIGSRC = $(XF86COMSRC)
     XF86HWSRC = $(XF86SRC)/common_hw
     XF86OSSRC = $(XF86SRC)/os-support
  VGADRIVERSRC = $(XF86SRC)/vga256/drivers
VGA16DRIVERSRC = $(XF86SRC)/vga16/drivers
 VGA2DRIVERSRC = $(XF86SRC)/vga2/drivers
 MONODRIVERSRC = $(XF86SRC)/mono/drivers
   S3DRIVERSRC = $(XF86SRC)/accel/s3/drivers
  S3VDRIVERSRC = $(XF86SRC)/accel/s3_virge/drivers

       XF68SRC = $(SERVERSRC)/hw/xfree68
    XF68COMSRC = $(XF68SRC)/common
 XF68CONFIGSRC = $(XF68COMSRC)
     XF68OSSRC = $(XF68SRC)/os-support

           XF98SRC = $(SERVERSRC)/hw/xfree98
      XF98ACCELSRC = $(XF98SRC)/accel
        XF98COMSRC = $(XF98SRC)/common
     XF98CONFIGSRC = $(XF98COMSRC)
         XF98HWSRC = $(XF98SRC)/common_hw/generic
      XF98HWNECSRC = $(XF98SRC)/common_hw/nec
    XF98HWPWSKBSRC = $(XF98SRC)/common_hw/pwskb
     XF98HWPWLBSRC = $(XF98SRC)/common_hw/pwlb
    XF98HWGA968SRC = $(XF98SRC)/common_hw/ga968
         XF98OSSRC = $(XF98SRC)/os-support
  XF98VGADRIVERSRC = $(XF98SRC)/vga256/drivers
XF98VGA16DRIVERSRC = $(XF98SRC)/vga16/drivers
 XF98VGA2DRIVERSRC = $(XF98SRC)/vga2/drivers
 XF98MONODRIVERSRC = $(XF98SRC)/mono/drivers
XF98NECS3DRIVERSRC = $(XF98SRC)/accel/s3nec/drivers
XF98PWSKBDRIVERSRC = $(XF98SRC)/accel/s3pwskb/drivers
 XF98PWLBDRIVERSRC = $(XF98SRC)/accel/s3pwlb/drivers
XF98GA968DRIVERSRC = $(XF98SRC)/accel/s3ga968/drivers

        XFREE86DOCDIR = $(DOCDIR)
      XFREE86PSDOCDIR = $(DOCPSDIR)
    XFREE86HTMLDOCDIR = $(DOCHTMLDIR)
XFREE86JAPANESEDOCDIR = $(DOCDIR)/Japanese

# $XConsortium: xf86.rules /main/9 1996/10/31 14:54:26 kaleb $
# $XFree86: xc/config/cf/xf86.rules,v 3.16.2.1 1997/05/18 12:00:01 dawes Exp $

# ----------------------------------------------------------------------
# site-specific configuration parameters that go after
# the platform-specific parameters - edit site.def to change

# site:  $XConsortium: site.def /main/revisionist/4 1996/12/31 08:02:07 kaleb $
# site:  $XFree86: xc/config/cf/site.def,v 3.17.2.1 1997/06/22 10:32:21 dawes Exp $

# ---------------------------------------------------------------------
# Imake rules for building libraries, programs, scripts, and data files
# rules:  $TOG: Imake.rules /main/222 1997/07/17 20:04:40 kaleb $
# rules:  $XFree86: xc/config/cf/Imake.rules,v 3.33.2.10 1998/11/04 10:53:01 dawes Exp $

 _NULLCMD_ = @ echo -n

TKLIBNAME = tk

TKLIBDIR = /usr/lib

TCLLIBNAME = tcl

TCLIBDIR = /usr/lib

          PATHSEP = /
            SHELL = /bin/sh

              TOP = .
      CURRENT_DIR = .

            IMAKE = imake
           DEPEND = gccmakedep
        MKDIRHIER = mkdir -p
    EXPORTLISTGEN =
        CONFIGSRC = $(TOP)/config
         IMAKESRC = $(CONFIGSRC)/imake
        DEPENDSRC = $(CONFIGSRC)/util

          INCROOT = /usr/X11R6/include
        USRLIBDIR = /usr/X11R6/lib
        VARLIBDIR = /var/lib
         SHLIBDIR = /usr/X11R6/lib
       LINTLIBDIR = $(USRLIBDIR)/lint
          MANPATH = /usr/X11R6/man
    MANSOURCEPATH = $(MANPATH)/man
           MANDIR = $(MANSOURCEPATH)1
        LIBMANDIR = $(MANSOURCEPATH)3
       FILEMANDIR = $(MANSOURCEPATH)5

               AR = ar clq
  BOOTSTRAPCFLAGS =
               CC = gcc
               AS = as

.SUFFIXES: .cc

              CXX = c++
          CXXFILT = c++filt
           CXXLIB =
    CXXDEBUGFLAGS = -O2 -fno-strength-reduce
CXXDEPENDINCLUDES =
 CXXEXTRA_DEFINES =
CXXEXTRA_INCLUDES =
   CXXSTD_DEFINES = -Dlinux -D__i386__ -D_POSIX_C_SOURCE=199309L -D_POSIX_SOURCE -D_XOPEN_SOURCE=500L -D_BSD_SOURCE -D_SVID_SOURCE $(CXXPROJECT_DEFINES)
       CXXOPTIONS =
      CXXINCLUDES = $(INCLUDES) $(TOP_INCLUDES) $(CXXEXTRA_INCLUDES)
       CXXDEFINES = $(CXXINCLUDES) $(CXXSTD_DEFINES) $(THREADS_CXXDEFINES) $(CXXEXTRA_DEFINES) $(DEFINES)
         CXXFLAGS = $(CXXDEBUGFLAGS) $(CXXOPTIONS) $(THREADS_CXXFLAGS) $(CXXDEFINES)

         COMPRESS = compress
          GZIPCMD = gzip
              CPP = /lib/cpp $(STD_CPP_DEFINES)
    PREPROCESSCMD = gcc -E $(STD_CPP_DEFINES)
          INSTALL = install
     INSTALLFLAGS = -c
               LD = ld
              LEX = flex -l
           LEXLIB = -lfl
             YACC = bison -y
           CCYACC = bison -y
             LINT = lint
      LINTLIBFLAG = -C
         LINTOPTS = -axz
               LN = ln -s
             MAKE = make
               MV = mv -f
               CP = cp

           RANLIB = ranlib
  RANLIBINSTFLAGS =

               RM = rm -f
        MANSUFFIX = 1x
     LIBMANSUFFIX = 3x
    FILEMANSUFFIX = 5x
            TROFF = groff -Tps
            NROFF = nroff
         MSMACROS = -ms
        MANMACROS = -man
              TBL = tbl
              EQN = eqn
             NEQN = neqn
              COL = col
         COLFLAGS = -b

            DVIPS = dvips
            LATEX = latex

     STD_INCLUDES =
  STD_CPP_DEFINES = -traditional -Dlinux -D__i386__ -D_POSIX_C_SOURCE=199309L -D_POSIX_SOURCE -D_XOPEN_SOURCE=500L -D_BSD_SOURCE -D_SVID_SOURCE $(PROJECT_DEFINES)
      STD_DEFINES = -Dlinux -D__i386__ -D_POSIX_C_SOURCE=199309L -D_POSIX_SOURCE -D_XOPEN_SOURCE=500L -D_BSD_SOURCE -D_SVID_SOURCE $(PROJECT_DEFINES)
 EXTRA_LOAD_FLAGS =
  EXTRA_LDOPTIONS =
  EXTRA_LIBRARIES =
             TAGS = ctags

   PARALLELMFLAGS =

    SHAREDCODEDEF =
         SHLIBDEF =

     SHLIBLDFLAGS = -shared

         PICFLAGS = -fPIC

      CXXPICFLAGS = -fPIC

    PROTO_DEFINES = -DFUNCPROTO=15 -DNARROWPROTO

     INSTPGMFLAGS = -s

     INSTBINFLAGS = -m 0755
     INSTUIDFLAGS = -m 4711
     INSTLIBFLAGS = -m 0644
     INSTINCFLAGS = -m 0444
     INSTMANFLAGS = -m 0444
     INSTDATFLAGS = -m 0444
    INSTKMEMFLAGS = -m 4711

      PROJECTROOT = /usr/X11R6

      CDEBUGFLAGS = -O2 -fno-strength-reduce
        CCOPTIONS =

      ALLINCLUDES = $(INCLUDES) $(EXTRA_INCLUDES) $(TOP_INCLUDES) $(STD_INCLUDES)
       ALLDEFINES = $(ALLINCLUDES) $(STD_DEFINES) $(EXTRA_DEFINES) $(PROTO_DEFINES) $(THREADS_DEFINES) $(DEFINES)
           CFLAGS = $(CDEBUGFLAGS) $(CCOPTIONS) $(THREADS_CFLAGS) $(ALLDEFINES)
        LINTFLAGS = $(LINTOPTS) -DLINT $(ALLDEFINES) $(DEPEND_DEFINES)
         LDPRELIB = -L$(USRLIBDIR)
        LDPOSTLIB =
        LDOPTIONS = $(CDEBUGFLAGS) $(CCOPTIONS)  $(EXTRA_LDOPTIONS) $(THREADS_LDFLAGS) $(LOCAL_LDFLAGS) $(LDPRELIBS)
     CXXLDOPTIONS = $(CXXDEBUGFLAGS) $(CXXOPTIONS) $(EXTRA_LDOPTIONS) $(THREADS_CXXLDFLAGS) $(LOCAL_LDFLAGS) $(LDPRELIBS)

           LDLIBS = $(LDPOSTLIBS) $(THREADS_LIBS) $(SYS_LIBRARIES) $(EXTRA_LIBRARIES)

           CCLINK = $(CC)

          CXXLINK = $(CXX)

     LDSTRIPFLAGS = -x
   LDCOMBINEFLAGS = -r
      DEPENDFLAGS =

# Not sure this belongs here
         TKLIBDIR = /usr/lib
         TKINCDIR = /usr/include
        TKLIBNAME = tk
        TKLIBRARY = -L$(TKLIBDIR) -l$(TKLIBNAME)
        TCLLIBDIR = /usr/lib
        TCLINCDIR = /usr/include
       TCLLIBNAME = tcl
       TCLLIBRARY = -L$(TCLLIBDIR) -l$(TCLLIBNAME)

        MACROFILE = linux.cf
           RM_CMD = $(RM)

    IMAKE_DEFINES =

         IRULESRC = $(CONFIGDIR)
        IMAKE_CMD = $(IMAKE) -DUseInstalled -I$(IRULESRC) $(IMAKE_DEFINES)

     ICONFIGFILES = $(IRULESRC)/Imake.tmpl $(IRULESRC)/X11.tmpl 			$(IRULESRC)/site.def $(IRULESRC)/$(MACROFILE) 			$(IRULESRC)/xfree86.cf $(IRULESRC)/xf86.rules $(IRULESRC)/xf86site.def $(IRULESRC)/host.def $(EXTRA_ICONFIGFILES)

# $TOG: X11.rules /main/4 1997/04/30 15:23:24 kaleb $
#
#
#
# $XFree86: xc/config/cf/X11.rules,v 1.1.1.1.2.3 1998/10/04 15:22:47 hohndel Exp $

# ----------------------------------------------------------------------
# X Window System Build Parameters and Rules
# $TOG: X11.tmpl /main/292 1997/05/20 10:05:59 kaleb $
#
#
#
#
# $XFree86: xc/config/cf/X11.tmpl,v 1.8.2.9 1998/12/30 10:04:09 dawes Exp $

CONNECTION_FLAGS = -DUNIXCONN -DTCPCONN -DHAS_STICKY_DIR_BIT

# -----------------------------------------------------------------------
# X Window System make variables; these need to be coordinated with rules

             XTOP = $(TOP)
           BINDIR = /usr/X11R6/bin
     BUILDINCROOT = $(TOP)/exports
      BUILDINCDIR = $(BUILDINCROOT)/include
      BUILDINCTOP = ../..
      BUILDLIBDIR = $(TOP)/exports/lib
      BUILDLIBTOP = ../..
      BUILDBINDIR = $(TOP)/exports/bin
      BUILDBINTOP = ../..
    XBUILDINCROOT = $(XTOP)/exports
     XBUILDINCDIR = $(XBUILDINCROOT)/include/X11
     XBUILDINCTOP = ../../..
     XBUILDBINDIR = $(XBUILDINCROOT)/bin
           INCDIR = $(INCROOT)
           ADMDIR = /usr/adm
           LIBDIR = $(USRLIBDIR)/X11
   TOP_X_INCLUDES =

           VARDIR = /var/X11

           DOCDIR = $(LIBDIR)/doc
       DOCHTMLDIR = $(DOCDIR)/html
         DOCPSDIR = $(DOCDIR)/PostScript
          FONTDIR = $(LIBDIR)/fonts
         XINITDIR = $(LIBDIR)/xinit
           XDMDIR = $(LIBDIR)/xdm
        XDMVARDIR = $(VARLIBDIR)/xdm
           TWMDIR = $(LIBDIR)/twm
           XSMDIR = $(LIBDIR)/xsm
           NLSDIR = $(LIBDIR)/nls
       XLOCALEDIR = $(LIBDIR)/locale
        PEXAPIDIR = $(LIBDIR)/PEX
      LBXPROXYDIR = $(LIBDIR)/lbxproxy
  PROXYMANAGERDIR = $(LIBDIR)/proxymngr
        XPRINTDIR = $(LIBDIR)
      XAPPLOADDIR = $(LIBDIR)/app-defaults
       FONTCFLAGS = -t

     INSTAPPFLAGS = $(INSTDATFLAGS)

              RGB = rgb
            FONTC = bdftopcf
        MKFONTDIR = mkfontdir

       DOCUTILSRC = $(XTOP)/doc/util
        CLIENTSRC = $(TOP)/clients
          DEMOSRC = $(TOP)/demos
       XDOCMACROS = $(DOCUTILSRC)/macros.t
       XIDXMACROS = $(DOCUTILSRC)/indexmacros.t
       PROGRAMSRC = $(TOP)/programs
           LIBSRC = $(XTOP)/lib
          FONTSRC = $(XTOP)/fonts
       INCLUDESRC = $(BUILDINCROOT)/include
      XINCLUDESRC = $(INCLUDESRC)/X11
        SERVERSRC = $(XTOP)/programs/Xserver
       CONTRIBSRC = $(XTOP)/../contrib
   UNSUPPORTEDSRC = $(XTOP)/unsupported
           DOCSRC = $(XTOP)/doc
           RGBSRC = $(XTOP)/programs/rgb
      BDFTOPCFSRC = $(PROGRAMSRC)/bdftopcf
     MKFONTDIRSRC = $(PROGRAMSRC)/mkfontdir
    FONTSERVERSRC = $(PROGRAMSRC)/xfs
       FONTINCSRC = $(XTOP)/include/fonts
        EXTINCSRC = $(XTOP)/include/extensions
     TRANSCOMMSRC = $(LIBSRC)/xtrans
   TRANS_INCLUDES = -I$(TRANSCOMMSRC)

       XENVLIBDIR = $(USRLIBDIR)
   CLIENTENVSETUP = LD_LIBRARY_PATH=$(XENVLIBDIR)

# $XConsortium: lnxLib.tmpl,v 1.5 95/01/11 21:44:44 kaleb Exp $
# $XFree86: xc/config/cf/lnxLib.tmpl,v 3.9 1996/02/24 04:32:52 dawes Exp $

          XLIBSRC = $(LIBSRC)/X11

SOXLIBREV = 6.1
DEPXONLYLIB =
XONLYLIB =  -lX11

LINTXONLY = $(LINTLIBDIR)/llib-lX11.ln

         XLIBONLY = $(XONLYLIB)

      XEXTLIBSRC = $(LIBSRC)/Xext

SOXEXTREV = 6.3
DEPEXTENSIONLIB =
EXTENSIONLIB =  -lXext

LINTEXTENSION = $(LINTLIBDIR)/llib-lXext.ln

LINTEXTENSIONLIB = $(LINTEXTENSION)
          DEPXLIB = $(DEPEXTENSIONLIB) $(DEPXONLYLIB)
             XLIB = $(EXTENSIONLIB) $(XONLYLIB)
         LINTXLIB = $(LINTXONLYLIB)

    XSSLIBSRC = $(LIBSRC)/Xss

DEPXSSLIB = $(USRLIBDIR)/libXss.a
XSSLIB =  -lXss

LINTXSS = $(LINTLIBDIR)/llib-lXss.ln

    XXF86MISCLIBSRC = $(LIBSRC)/Xxf86misc

DEPXXF86MISCLIB = $(USRLIBDIR)/libXxf86misc.a
XXF86MISCLIB =  -lXxf86misc

LINTXXF86MISC = $(LINTLIBDIR)/llib-lXxf86misc.ln

    XXF86VMLIBSRC = $(LIBSRC)/Xxf86vm

DEPXXF86VMLIB = $(USRLIBDIR)/libXxf86vm.a
XXF86VMLIB =  -lXxf86vm

LINTXXF86VM = $(LINTLIBDIR)/llib-lXxf86vm.ln

    XXF86DGALIBSRC = $(LIBSRC)/Xxf86dga

DEPXXF86DGALIB = $(USRLIBDIR)/libXxf86dga.a
XXF86DGALIB =  -lXxf86dga

LINTXXF86DGA = $(LINTLIBDIR)/llib-lXxf86dga.ln

    XDPMSLIBSRC = $(LIBSRC)/Xdpms

DEPXDPMSLIB = $(USRLIBDIR)/libXdpms.a
XDPMSLIB =  -lXdpms

LINTXDPMS = $(LINTLIBDIR)/llib-lXdpms.ln

         XAUTHSRC = $(LIBSRC)/Xau

DEPXAUTHLIB = $(USRLIBDIR)/libXau.a
XAUTHLIB =  -lXau

LINTXAUTH = $(LINTLIBDIR)/llib-lXau.ln

      XDMCPLIBSRC = $(LIBSRC)/Xdmcp

DEPXDMCPLIB = $(USRLIBDIR)/libXdmcp.a
XDMCPLIB =  -lXdmcp

LINTXDMCP = $(LINTLIBDIR)/llib-lXdmcp.ln

           XMUSRC = $(LIBSRC)/Xmu

SOXMUREV = 6.0
DEPXMULIB =
XMULIB =  -lXmu

LINTXMU = $(LINTLIBDIR)/llib-lXmu.ln

       OLDXLIBSRC = $(LIBSRC)/oldX

DEPOLDXLIB = $(USRLIBDIR)/liboldX.a
OLDXLIB =  -loldX

LINTOLDX = $(LINTLIBDIR)/llib-loldX.ln

         XPLIBSRC = $(LIBSRC)/Xp

SOXPREV = 6.2
DEPXPLIB =
XPLIB =  -lXp

LINTXP = $(LINTLIBDIR)/llib-lXp.ln

       TOOLKITSRC = $(LIBSRC)/Xt

SOXTREV = 6.0
DEPXTOOLONLYLIB =
XTOOLONLYLIB =  -lXt

LINTXTOOLONLY = $(LINTLIBDIR)/llib-lXt.ln

      DEPXTOOLLIB = $(DEPXTOOLONLYLIB) $(DEPSMLIB) $(DEPICELIB)
         XTOOLLIB = $(XTOOLONLYLIB) $(SMLIB) $(ICELIB)
     LINTXTOOLLIB = $(LINTXTOOLONLYLIB)

       XALIBSRC = $(LIBSRC)/Xa

SOXAREV = 1.0
DEPXALIB =
XALIB =  -lXa

LINTXA = $(LINTLIBDIR)/llib-lXa.ln

       AWIDGETSRC = $(LIBSRC)/Xaw

SOXAWREV = 6.1
DEPXAWLIB =
XAWLIB =  -lXaw

LINTXAW = $(LINTLIBDIR)/llib-lXaw.ln

         XILIBSRC = $(LIBSRC)/Xi

SOXINPUTREV = 6.0
DEPXILIB =
XILIB =  -lXi

LINTXI = $(LINTLIBDIR)/llib-lXi.ln

      XTESTLIBSRC = $(LIBSRC)/Xtst

SOXTESTREV = 6.1
DEPXTESTLIB =
XTESTLIB =  -lXtst

LINTXTEST = $(LINTLIBDIR)/llib-lXtst.ln

        PEXLIBSRC = $(LIBSRC)/PEX5

SOPEXREV = 6.0
DEPPEXLIB =
PEXLIB =  -lPEX5

LINTPEX = $(LINTLIBDIR)/llib-lPEX5.ln

        XIELIBSRC = $(LIBSRC)/XIE

SOXIEREV = 6.0
DEPXIELIB =
XIELIB =  -lXIE

LINTXIE = $(LINTLIBDIR)/llib-lXIE.ln

      PHIGSLIBSRC = $(LIBSRC)/PHIGS

DEPPHIGSLIB = $(USRLIBDIR)/libphigs.a
PHIGSLIB =  -lphigs

LINTPHIGS = $(LINTLIBDIR)/llib-lphigs.ln

DEPXBSDLIB = $(USRLIBDIR)/libXbsd.a
XBSDLIB =  -lXbsd

LINTXBSD = $(LINTLIBDIR)/llib-lXbsd.ln

           ICESRC = $(LIBSRC)/ICE

SOICEREV = 6.3
DEPICELIB =
ICELIB =  -lICE

LINTICE = $(LINTLIBDIR)/llib-lICE.ln

            SMSRC = $(LIBSRC)/SM

SOSMREV = 6.0
DEPSMLIB =
SMLIB =  -lSM

LINTSM = $(LINTLIBDIR)/llib-lSM.ln

           XKEYSRC = $(LIBSRC)/Xkey

SOXKEYREV = 6.0
DEPXKEYLIB =
XKEYLIB =  -lXkey

LINTXKEY = $(LINTLIBDIR)/llib-lXkey.ln

         FSLIBSRC = $(LIBSRC)/FS

DEPFSLIB = $(USRLIBDIR)/libFS.a
FSLIB =  -lFS

LINTFS = $(LINTLIBDIR)/llib-lFS.ln

         FONTLIBSRC = $(LIBSRC)/font

DEPFONTLIB = $(USRLIBDIR)/libfont.a
FONTLIB =  -lfont

LINTFONT = $(LINTLIBDIR)/llib-lfont.ln

          XPMLIBSRC = $(LIBSRC)/Xpm

DEPXPMLIB = $(USRLIBDIR)/libXpm.a
XPMLIB =  -lXpm

LINTXPM = $(LINTLIBDIR)/llib-lXpm.ln

    XKBFILELIBSRC = $(LIBSRC)/xkbfile

DEPXKBFILELIB = $(USRLIBDIR)/libxkbfile.a
XKBFILELIB =  -lxkbfile

LINTXKBFILE = $(LINTLIBDIR)/llib-lxkbfile.ln

     XKBCOMPCMD = xkbcomp

    XKBUILIBSRC = $(LIBSRC)/xkbui

DEPXKBUILIB = $(USRLIBDIR)/libxkbui.a
XKBUILIB =  -lxkbui

LINTXKBUI = $(LINTLIBDIR)/llib-lxkbui.ln

          DEPLIBS = $(DEPXAWLIB) $(DEPXMULIB) $(DEPXTOOLLIB) $(DEPXLIB)

         DEPLIBS1 = $(DEPLIBS)
         DEPLIBS2 = $(DEPLIBS)
         DEPLIBS3 = $(DEPLIBS)
         DEPLIBS4 = $(DEPLIBS)
         DEPLIBS5 = $(DEPLIBS)
         DEPLIBS6 = $(DEPLIBS)
         DEPLIBS7 = $(DEPLIBS)
         DEPLIBS8 = $(DEPLIBS)
         DEPLIBS9 = $(DEPLIBS)
         DEPLIBS10 = $(DEPLIBS)

XMULIBONLY = -lXmu
XMULIB = $(XMULIBONLY) $(XTOOLLIB) $(XLIB)

        CONFIGDIR = $(LIBDIR)/config

    USRLIBDIRPATH = $(USRLIBDIR)
        LDPRELIBS = -L$(USRLIBDIR)
       LDPOSTLIBS =
     TOP_INCLUDES = -I$(INCROOT) $(TOP_X_INCLUDES)
  PROJECT_DEFINES =

CXXPROJECT_DEFINES =

# ----------------------------------------------------------------------
# start of Imakefile

DEBUG = 1

DESTDIR = /usr/X11R6
BINDIR = /bin

XPMLIB = -L/usr/lib/X11 -lXpm -lm -lsensors
DEPLIBS = $(DEPXLIB)

LOCAL_LIBRARIES = $(XPMLIB) $(XLIB)

LINTLIBS = $(LINTXLIB)

EXTRA_DEFINES = -Debug

SRCS = wmsensors.c
OBJS = wmsensors.o

 PROGRAM = wmsensors

all:: wmsensors

wmsensors: $(OBJS) $(DEPLIBS)
	$(RM) $@
	$(CCLINK) -o $@ $(LDOPTIONS) $(OBJS) $(LOCAL_LIBRARIES) $(LDLIBS)  $(EXTRA_LOAD_FLAGS)

install:: wmsensors
	@if [ -d $(DESTDIR)$(BINDIR) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(DESTDIR)$(BINDIR)); fi
	$(INSTALL) $(INSTALLFLAGS) $(INSTPGMFLAGS)  wmsensors $(DESTDIR)$(BINDIR)/wmsensors

install.man:: wmsensors.man
	@if [ -d $(DESTDIR)$(MANDIR) ]; then set +x; \
	else (set -x; $(MKDIRHIER) $(DESTDIR)$(MANDIR)); fi
	$(INSTALL) $(INSTALLFLAGS) $(INSTMANFLAGS) wmsensors.man $(DESTDIR)$(MANDIR)/wmsensors.$(MANSUFFIX)

depend::
	$(DEPEND) $(DEPENDFLAGS) -- $(ALLDEFINES) $(DEPEND_DEFINES) -- $(SRCS)

lint:
	$(LINT) $(LINTFLAGS) $(SRCS) $(LINTLIBS)
lint1:
	$(LINT) $(LINTFLAGS) $(FILE) $(LINTLIBS)

clean::
	$(RM) wmsensors

# ----------------------------------------------------------------------
# common rules for all Makefiles - do not edit

.c.i:
	$(RM) $@
	 $(CC) -E $(CFLAGS) $(_NOOP_) $*.c > $@

emptyrule::

clean::
	$(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a .emacs_* tags TAGS make.log MakeOut  "#"*

Makefile::
	-@if [ -f Makefile ]; then set -x; \
	$(RM) Makefile.bak; $(MV) Makefile Makefile.bak; \
	else exit 0; fi
	$(IMAKE_CMD) -DTOPDIR=$(TOP) -DCURDIR=$(CURRENT_DIR)

tags::
	$(TAGS) -w *.[ch]
	$(TAGS) -xw *.[ch] > TAGS

man_keywords::

# ----------------------------------------------------------------------
# empty rules for directories that do not have SUBDIRS - do not edit

install::
	@echo "install in $(CURRENT_DIR) done"

install.man::
	@echo "install.man in $(CURRENT_DIR) done"

install.linkkit::
	@echo "install.linkkit in $(CURRENT_DIR) done"

Makefiles::

includes::

depend::

# ----------------------------------------------------------------------
# dependencies generated by makedepend

