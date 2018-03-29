        MIDAS Digital Audio System 1.1.2, release notes
        -----------------------------------------------

A Sahara Surfers production.

Copyright 1996, 1997 Housemarque Inc.

Written by Petteri Kangaslampi and Jarno Paananen
(a.k.a. Alfred & Guru / S2)

This file is part of MIDAS Digital Audio System, and may only be
used, modified and distributed under the terms of the MIDAS
Digital Audio System license, "license.txt". By continuing to use,
modify or distribute this file you indicate that you have
read the license and understand and accept it fully.



1. Introduction and news
------------------------

This is the 1.1.2 release of MIDAS Digital Audio System. This release
contains just some bug fixes and optimizations over 1.1.0 and 1.1.1, no new
functionality has been added. This release should be 100% compatible previous
1.1.x releases, and can simply be dropped in their place. Updating the
libraries is enough, and even re-linking is not necessary for programs that
use the dynamic link libraries.

Most of the changes in this release are minor bug fixes to several parts of
the system. Several small FastTracker 2 compatibility problems have been
fixed, as well as some other module player bugs. MIDASsetAmplification() now
works correctly when called more than once, long bidirectional loops work
correctly, and PAS support is fixed in the DJGPP version.

After this 1.1.2 release, MIDAS Digital Audio System development will continue
mainly towards the future 1.2 and 2.0 versions. We hope that no further
updates to 1.1 will be necessary, but they will be made released if needed.



2. Brief introduction to MIDAS Digital Audio System
---------------------------------------------------

So what is MIDAS Digital Audio System anyway?

In brief, MIDAS is a multichannel digital sound and music system, capable of
playing an unlimited number of channels of digital sound on all supported
platforms. It can play music modules, individual samples, and digital audio
streams, in any combination.

MIDAS supports the following module formats:
        - 4-channel Protracker modules plus 1-32-channel variants (.MOD)
        - 1-32 -channel Scream Tracker 3 modules (.S3M)
        - 2-32 -channel FastTracker 2 modules (.XM)
	- 1-64 -channel Impulse Tracker modules (.IT)

In the Win32 and Linux platforms, MIDAS accesses the sound hardware through
the system sound drivers, and thus works with any sound card that has proper
drivers available. In Win95/NT, both the standard multimedia APIs and
DirectSound are supported. Under MS-DOS, MIDAS supports the following sound
cards:
        - Creative Labs Sound Blaster series (1.0 through SB 16 and AWE32)
        - Gravis UltraSound (regular, MAX, and Plug and Play, with native
          Interwave support)
	- Media Vision Pro Audio Spectrum series (PAS, PAS+ and PAS16) 
        - Microsoft Windows Sound System and compatible cards using the
          Crystal/Analog CODEC. Most inexpensive 16-bit sound cards sold
          nowadays are WSS compatible.

This release of MIDAS can be used for free for free programs, but commercial
usage requires a separate license. Contact "midas@housemarque.fi" for details.



3. What's New?
--------------

Changes since MIDAS 1.0 include:
        - Completely new mixing routines
        - Echo effect support
        - Support for Impulse Tracker modules
        - Automatic filtering
        - Built-in setup functions for Win32
        - Updated MS-DOS setup
        - Sample playback status checking functions

In addition, changes since MIDAS 0.6 releases include:
	- Stream support for MS-DOS
	- DirectSound support
	- WAVE file support for both samples and streams
	- Enhanced documentation, now also in WinHelp format
	- Sound output filtering
	- The ability to play multiple modules simultaneously (but with some
          restrictions - see the documentation)
	- Music channel fading
	- 4:1 ADPCM compression for streams
	- Miscellaneous smaller changes and bug fixes


4. Supported platforms
----------------------

MIDAS supports the following platforms and compilers:
        - 32-bit MS-DOS under DOS/4GW, with Watcom C/C++
	- 32-bit MS-DOS under any DPMI host (such as CWSDPMI) with DJGPP
        - Windows NT/95 with Watcom C/C++
        - Windows NT/95 with Visual C/C++
        - Windows NT/95 with Borland Delphi
        - Linux with GNU C/C++

In addition, under Win32 MIDAS can be used with any programming environment
that supports DLLs. Direct examples are provided only for Watcom and Visual C
and Borland Delphi though.

Under Linux, background and stream playing needs a pthreads compatible
package, preferably the LinuxThreads-package available at:

ftp://ftp.inria.fr/INRIA/Projects/cristal/Xavier.Leroy/linuxthreads.tar.gz

The DJGPP version requires Shawn Hargreaves' Allegro library for its IRQ
wrappers. You can get Allegro from http://www.talula.demon.co.uk/allegro/. At
least version 2.11 is known to work, but later versions should do as
well. Note that the Allegro timer functions cannot be used with MIDAS.



5. Using MIDAS
--------------

Before trying to use MIDAS Digital Audio System, make sure you read all
documentation available in the "doc" directory. The documentation is included
in several formats: WinHelp, Plain ASCII text (*.txt), PostScript (*.ps),
Adobe PDF (*.pdf) and HTML (separate directories). PostScript or PDF is
recommended for printing, and WinHelp for reading online. If you prefer, you
can also use Adobe Acrobat Reader for reading the PDF documents. At the moment
the HTML documentation requires long filenames, so it is not very usable from
plain DOS.

After at least browsing through the documentation, you should check of the
examples in the "samples" directory. The directory "samples/common" should be
especially useful, as it contains several small programs that demonstrate the
MIDAS API. DOS programmer's should also check "samples/dos". "samples/midpnt"
contains the source code for MIDAS Module Player for Windows NT, a slightly
bigger and more complex program. Finally, see "samples/dsound" for an example
on how to use DirectSound.

Some of the examples might require a bit tweaking, depending your target
system. In particular, you may need to edit a few lines at the beginning of
the Makefiles in the directories. However, the code itself should compile
cleanly on all supported platforms.

When you are ready to use MIDAS Digital Audio System in your own programs, it
is probably useful to use one of the examples as a model. Remember that you
are allowed to use MIDAS for free programs as is, but for commercial usage you
will need to negotiate a license with us. In addition, your program HAS to
include proper credits for MIDAS, either in the program itself or its
documentation. This is even a requirement in the MIDAS license, and if you
fail to include the credits you are breaking it.



6. Contact Information
----------------------

Comments? Bug reports? Want more information about MIDAS Digital Audio System
licensing?  Contact us! Just select the correct e-mail address:

Technical questions and comments: Petteri Kangaslampi, pekangas@sci.fi

Licensing information: Housemarque Inc., midas@housemarque.fi

Linux-specific questions: Jarno Paananen, jpaana@iki.fi

The updated MIDAS Digital Audio System WWW-pages are now on-line at
http://www.s2.org/midas/. The pages are being moved to www.housemarque.com, at
least partially, but for the time being the most up-to-date information will
be available at s2.org.

In addition, there is a mailing list available for MIDAS-related
announcements. Send mail to "majordomo@s2.org", with the words "subscribe
midas" in the BODY of the message. This is a standard Majordomo list, so all
normal Majordomo commands apply.



7. Getting MIDAS
----------------

The best and fastest way to get MIDAS is the Internet. The latest MIDAS
version is always at "http://www.s2.org/midas/". In addition, you can find
there the latest MIDAS news, MIDP releases, and general information.

In addition, MIDAS is available at the following fine BBSes:

        Edge Of Delight
                Node 1: +32-2-3755651   Boca 28.8K (V34/VFAST)
                Node 2: +32-2-3758923   ZyXEL 19.2K (V32terbo)
                Node 3: +32-2-3721089   ISDN 64K (X75)
                Chaos Managers: Cobra, aCceSs & Fredy

        The Underworld BBS
                Node 1: +41-22-9600621  USR Courier V.34+, ISDN Analog
                Node 2: +41-22-9600622  USR Courier V.34+, ISDN Analog
                Node 3: +41-22-9600623  ZyXEL Elite 2864I 64kb ISDN,
                                                ISDN Digital
                Sysop: Synoptic

        WarmBoot BBS
                Node 1: +55-194-261993
                Sysop: Warmbooter

        South of Heaven BBS
                Node 1: +1-916-567-1090 (2x28.8k)
                Sysop: Pantera

        Psycho Beaver BBS
                Node 1: +972-9-8320175 (2x28.8k)
                Sysop: riff raff

	Splatter Punk BBS
		Node 1: +49-2378-2627 (28.8k, 20:00-06:00 CET)
		Sysop: Teasy

	Boiled Sweets Music BBS
		Node 1: +44-171-686-0135 (USR Courier V.34+ 33.6k)
		Node 2: +44-171-686-0136 (USR Sportster V.34+ 33.6k)
		Sysop: Dweezil


	BBS [P]OWER (C)HA][ENGE
		Node 1: +45-97-250117
		Sysop: x-otic

        Apologies for any boards we forgot - email us with your current
        information and you are back on the list. I have to confess I haven't
        kept very good track on our distribution sites, and now deliberately
        left out everybody I haven't heard from in a year or so...


8. Acknowledgements
-------------------

There is no way we could have MIDAS what it is alone, and we feel we really
owe a big "thank you" to the following people:

	Otto Chrons: Thanks for the sound cards, programming information, plus
	optimization and debugging hints.

	Teemu Kalvas: Thanks for some serious beta-testing, optimization
	ideas, and just general helpfulness.

	Jarno Heikkinen: Thanks for your support and the GUS PnP code,
	although it went along with SW mixing support.

	Francois Baligant: One of the most supportive people I know. Nice
	meeting you at Wired'96, too bad I couldn't make it this year.

	Mikko Karvonen and Janne Oksanen: Thanks for composing all that wicked
	music :)

	Hannu Koivisto: Thanks for testing and commenting on MidpNT -- maybe
	some day the instrument list will get fixed...

	Khaled Agrama: Thanks for the cooperation so far, hopefully I'll have
	more time to help you in the future.

	All authors of MIDAS-based module players: Thanks for your support,
	now go and upgrade to 1.1!

	And everybody who has sent us feedback on MIDAS - too numerous to
	mention, and all extremely important. Keep it coming!


Plus some traditional hillos to our friends in the scene and beyond:

	#cokers - you know who you are

	Jesse Hallio
	Alik. Katainen J-P :)
	Kari-Pekka Koljonen
	Kraku
	Nemesis1
	Statix
	Unreal

	The whole Virtual Visions team	
	Everybody in Parallax
	Our friends in Prime

	Byterapers
	CNCD
	Complex
	Doomsday Productions
	Paranoids
	Virtual Dreams


And finally, thanks to all our friends, both real and virtual - thank you for
being who you are, for doing what you do, the way you do.


-- Petteri Kangaslampi and Jarno Paananen, 1 Feb 1998
