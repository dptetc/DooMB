        MIDAS Module Player for Windows NT, version 1.0.2
        -------------------------------------------------

MIDAS MODULE PLAYER IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
WILL ANY OF THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY DAMAGES
CAUSED BY THE USE, OR INABILITY TO USE, OF MIDAS MODULE PLAYER.

MIDAS Module Player may freely be distributed as long as no money is
charged for it, and no attempt is made to restrict further copying,
distribution and using of MIDAS Module Player. If you have problems
with this, or do not understand this, contact us.



1. Foreword
-----------

Thank you for downloading MIDAS Module Player, and, above all, thank you for
reading the documentation. Although it seems MidpNT will never be "finished"
the way I intended it, I decided to label it as version 1.0 anyway - this is
as ready as it will get. The user interface is primitive and there is very
little documentation, but the most important feature is here: music
playback. This version of MidpNT is also the first public demonstration of
several MIDAS Digital Audio System enhancements, such as the new high-quality
mixing routines and the echo effect engine.

So what is MidpNT anyway? MidpNT is a free module player for Windows NT
and 95. It plays Protracker (MOD), Scream Tracker 3 (S3M) and
FastTracker 2 (XM) modules on all sound cards that Windows supports. There is
also some preliminary Impulse Tracker (IT) support in, and if you get lucky it
might even work. 

MidpNT might seem very unfinished and in general look like a music system test
program more than anything else - and that is exactly what it is. The main
intention of MidpNT is to test MIDAS Digital Audio System and give you an
impression what it sounds like - that's why our emphasis is on playback
quality instead of the user interface.

MidpNT isn't loaded with fancy features and extra chrome, and it should thus
be fairly easy to use, even with this limited documentation. I might write
online help some day, but right now I'm too busy for that. If you can't figure
out how to use MidpNT, check out some of the other MIDAS-based module
players. 


2. Using MidpNT
---------------

2.1 Basic Usage
---------------

Using MidpNT is simple - just start it and you are on your way to play
music. You can use File/Open to select a module, or you can pass a module file
name in the command line. If you attempt to start a new instance of MidpNT,
the module file name will just be passed to the old copy and it will take care
of playing it. This allows you to just associate MidpNT with all module
extensions and use Explorer or File Manager to select and play modules.

MidpNT now also accepts a command line option. Yes, that's right, a single
command line option. Pass MidpNT "-m" on the command line before the module
file name to start in minimal UI mode.


2.2 The Views
-------------

In addition to the main console-like window, MidpNT has also several view
windows. These windows can display the song information, the instrument names
(which typically contain messages from the composer) and the Echo Effect
editor. To open a view window, just select it from the "Views" menu, and
position it to a reasonable spot on the screen. After you have positioned the
windows as you wish, select "Save Window Arrangement" from the "Options" menu
- this way MidpNT remembers where the windows were when it next starts.


2.3 Echo Effects
----------------

The most exciting new feature in MidpNT 1.0 are the echo effects. To use them,
open the Echo Effect Editor view, select the effect you want from the list,
and listen carefully. Not all the effects are really echoes or reverbs in the
true sense, but all use the same echo effect engine.

After you have checked the built-in effects, and learnt a word of Finnish in
the process (you probably can guess now what "kaiku" means :), you can start
playing with the echo effect editor to build your own effects. The easiest
way to get started is to modify one of the existing effects, making small
changes to the values and checking how it affects the sound. If you don't want
to mess the predefined effects, rename the effect to something else before
saving it - this way MidpNT will restore the original effect when it is next
started. Also note that you must click "Apply" or press Enter for your changes
to take effect.

An echo effect consists of two parts: Three common values plus a set of
echoes. "Feedback" controls the amount of feedback for the echo effect, or the
amount of "echoed" data that will be used in future echoes. A setting of 1.0
will recycle all echoed data back to the echo delay line, 0.5 half of it
etc. Too large feedback values can cause nasty distortion and clipping. "Gain"
simply controls a total gain for the output, and it can be used to lower the
volume after the echo effect to prevent distortion. A gain of 1.0 has no
effect on the sound, 0.5 halves the volume etc. Finally, the "Echoes" -setting
simply controls the number of echoes the effect has.

Each echo contains four settings: delay, gain, reverse channels, and
filter. "Delay" specifies the echo delay in milliseconds - the larger the
delay, the longer the echo. "Gain" controls the strength of the echo, with 1.0
playing the echoed sound at full volume, 0.5 at half volume etc. Checking
"Reverse Channels" causes the left and right channels to be reversed in the
echo, adding a nice stereo effect. The last setting, "Filter", controls the
possible filtering applied to the echo. If you have more than one or two
echoes, low-pass-filtering them is a good idea, otherwise extra high
frequencies tend to accumulate in the sound. High-pass-filtering the echoes
will make them much weaker, but can be a nice effect in some cases.

When you have created an effect you like, click "Save" to save the effect for
future use. Effects are saved in the system registry, in the
"HKEY_CURRENT_USER\Software\Sahara Surfers\MidpNT\Effects" key. Also, if you
manage to create a really cool effect you'd like others to hear as well,
e-mail it to me and I'll add it to the next release. If you have created
reverb effects for the DMP module player in DOS, you should be able to convert
them to MidpNT pretty easily - I myself adapted the hall reverb effect from
there.

Complicated? Hopefully not. The best way to learn the echo effect editor is
simply to play around with it. Keep your volume to a reasonable level with it
though - it is easy to create very loud distortions and feedback.



3. Configuring MidpNT
---------------------

MidpNT can be configured using the entries in the Options menu. Preferences
opens a dialog with generic user interface -related preferences and Sound
Options lets you change the sound output quality. When MidpNT is started for
the first time, the sound quality settings dialog will also appear. The
settings from the Preferences and Sound Options dialogs are automatically saved.

IMPORTANT! If you manage to screw MidpNT settings up so that it refuses to
start any more, edit the Registry from HKEY_CURRENT_USER\Software\Sahara
Surfers\MidpNT. You can also delete the whole MidpNT key -- MidpNT will then
start with default values.



4. Future
---------

MidpNT was written mainly for my own use, and will likely stay that
way. Thus it won't be changing much in the future either -- it contains all
the features I consider essential for a module player. There are several other
graphical MIDAS-based Win32 module players around, which you might want to
check out. See the MIDAS module player web pages at
http://kalahari.ton.tut.fi/s2/midas/ for download links and screenshots.

MidpNT does, however, always contain the latest MIDAS Digital Audio System
features, and always has the latest MIDAS version linked in. MidpNT also often
contains features not available in other MIDAS-based module players, as it can
be using a pre-release version of MIDAS.


5. Contact information
----------------------

Comments, bug reports and improvement ideas are always welcome, although don't
expect anything fancy on the UI side. E-mail pekangas@sci.fi.

The latest version of MidpNT is always available at
http://kalahari.ton.tut.fi/s2/midas/midp.html, and more information about
MIDAS Digital Audio System can be found at
http://kalahari.ton.tut.fi/s2/midas/.

Note! When reporting bugs in MidpNT, please remember to include the
version information and build date displayed in the main window at
startup. (eg. "Version 0.1.5, built Sep 13 1996.")


6. Revisions
------------

1.0.2	- Linked with MIDAS 1.1 beta 1 release
	- Some minor changes, including faster mixing and IT fixes

1.0.1	- Fixed some clicking problems
	- Some IT fixes
	- Other minor playback changes

1.0.0	- Linked with MIDAS 1.1 alpha
	- Added new high-quality mixing routines
	- Added echo effects
	- Some preliminary IT support
	- Other minor changes and fixes

0.2.5   - Linked with MIDAS 1.0 beta 1
	- Added output filter toggle

0.2.4   - Linked with MIDAS 0.6.1, fixes a couple of bugs

0.2.3   - Added the icon. The icon was originally drawn by
          Han Solo of K/oS, and edited a bit by me. So all the ugly
          bits you can blame on myself :) The icon isn't really tested
          on 256-color or 16-color displays, let me know if it looks
          too bad.
        - Linked with MIDAS 0.6.0

0.2.2   - Rewrote archive support completely. Decompression target
          directory can now be set in the preferences. Now handles
          archives where the archive name isn't exactly the same as
          the module name.

0.2.1   - User Interface enhancements: decompression console is hidden,
          MidpNT now displays information in the window caption
        - Minimal UI mode
        - Linked with MIDAS 0.6.rc1

0.2.0   - Major internal reorganization, now uses the documented MIDAS
          DLL API exclusively
        - Several FT2 compatibility bugs fixed in MIDAS

0.1.7   - Linked with MIDAS 0.5.0 releas
        - Small bugfixes

0.1.6   - A new release with the release version of MIDAS 0.5.rc2

0.1.5   - Fixed various Scream Tracker 3 support problems, including
          bad panning and tempos
        - Fixed a potentially dangerous bug in MOD/S3M playback, which
          didn't surface under Windows NT/95 though

0.1.4   - Fixed several FastTracker 2 compatibility problems
        - Fixed some problems in loading invalid S3Ms (produced by ST3)
        - Other miscellaneous bugfixes in MIDAS Digital Audio System code

0.1.3   - Fixed a bug in handling file names with spaces
        - Miscellaneous playback fixes
        - Added 3D-look attribute to Preferences-dialog
        - Changed some accelerator keys - song position is now changed
          with Ctrl-'.' and Ctrl-',', and Center Channels is Ctrl-E.
        - Accelerators now work even if a child window is active

0.1.2	- Fixed some bugs in module playback that were causing crashes

0.1.1   - Changed polling to use a thread instead of a multimedia timer
           - this uses much less CPU
        - Centering channels when there was no module playing crashed
          MidpNT, fixed

0.1.0   - Initial release in this format


[$Id: readme.txt,v 1.15 1997/07/31 20:03:56 pekangas Exp $]
