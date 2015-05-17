/*
    internals.h - Part of wmsensors, a Linux utility for monitoring sensors.
    Copyright (c) 1998  Adrian Baugh <adrian.baugh@keble.ox.ac.uk>

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


/*************************************************************************/
/* Internal definitions header for wmsensors                             */
/* For modification by hackers only! (or those with undetected CPUs)     */
/* Most stuff is automatic now.                                          */
/*************************************************************************/

/*************************************************************************/
/* This is the default CPU voltage settings, in case the auto-detection  */
/* fails. I've tested auto-detection on an Intel Pentium 75 - let me     */
/* know if it fails for anything else. These settings are PII Deschutes  */
/* settings because that's what I have and I'm the developer :)          */
/*                                                                       */
/* For a PII Klamath you want NORMAL_SYSTEM_CORE_VOLTAGE 2.8 (AMDs too?) */
/* For a SMP system you do not need to care about the IO_VOLTAGE as it   */
/* is taken care of automatically (it is the same as the CORE_VOLTAGE).  */
/*************************************************************************/

#define NORMAL_SYSTEM_CORE_VOLTAGE 2.0
#define NORMAL_SYSTEM_IO_VOLTAGE 3.3

/* CPU auto-detection parameters */
#define EARLY_PENTIUM 1
#define LATE_PENTIUM 2
#define PENTIUM_MMX 3
#define EARLY_PENTIUM_PRO 4
#define LATE_PENTIUM_PRO 5
#define KLAMATH 6
#define DESCHUTES 7
#define XEON 8
#define CYRIX 9
#define CYRIX686MX 10
#define AMDK5 11
#define AMDK6 12
#define AMDK62 13



