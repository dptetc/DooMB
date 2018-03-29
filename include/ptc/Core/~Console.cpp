//
// Console class for OpenPTC 1.0 C++ Implementation
// Copyright (c) 1999 Glenn Fiedler (ptc@gaffer.org)
// This source code is licensed under the GNU Lesser GPL
// See http://www.gnu.org/copyleft/lesser.html for details
//

// include files
#include <stdio.h>
#include <string.h>
#include <fstream.h>
#include "Core/Log.h"
#include "Core/Key.h"
#include "Core/Area.h"
#include "Core/Color.h"
#include "Core/Error.h"
#include "Core/Console.h"

// include consoles
#include "GDI/Console.h"
#include "DirectX/Console.h"




PTCAPI Console::Console()
{
    // defaults
    console = 0;

    // clear hacky option console flag
    hacky_option_console_flag = false;

    // configure console
    configure("ptc.cfg");
}


PTCAPI Console::~Console()
{
    // close
    close();

    // check console
    if (console)
    {
        // free console
        delete console;
    }
}




void PTCAPI Console::configure(const char file[])
{
    // open configuration file
    ifstream is(file,ios::in | ios::nocreate);
    
    // check stream
    if (!is) return;

    // read option strings
    while (!is.eof())
    {
        // option line
        char line[1024];

        // read line from stream
        is.getline(line,1024);

        // process option
        option(line);
    }
}




bool PTCAPI Console::option(const char option[])
{
#ifdef __PTC_LOGGING__

    // check for "enable logging" option
    if (stricmp(option,"enable logging")==0)
    {
        // enable logging
        Log::enable();

        // recognized
        return true;
    }

    // check for "disable logging" option
    if (stricmp(option,"disable logging")==0)
    {
        // disable logging
        Log::disable();

        // recognized
        return true;
    }

#endif

    // check console
    if (console)
    {
        // console option string
        return console->option(option);                                             
    }
    else
    {
        // create console by name
        console = create(option);

        // check console
        if (console)
        {
            // set that magical hacky option console flag!
            hacky_option_console_flag = true;

            // recognized
            return true;
        }
        else
        {
            // not recognized
            return false;
        }
    }
}




const Mode* PTCAPI Console::modes()
{
    // check console
    if (console)
    {
        // get console modes
        return console->modes();
    }
    else
    {
        // current console
        BaseConsole *console = 0;

        // console index
        int index = -1;

        // current mode
        int mode = 0;

        // iterate
        while (true)
        {
            // next console
            index ++;

            try
            {
                // create console by index
                console = create(index);
            }
            catch (Error&)
            {
                // free console
                delete console;

                // null console
                console = 0;

                // failure
                continue;
            }

            // check console
            if (!console) break;

            // get console mode list array
            const Mode *modes = console->modes();

            // local mode index
            int local = 0;

            // append modes to list
            while (modes[local].valid())
            {
                // copy mode over to combined list
                m_modes[mode] = modes[local];

                // next mode
                local ++;
                mode ++;
            }
            
            // free console
            delete console;

            // null console
            console = 0;
        }

        // todo: strip duplicate modes from list?

        // terminate list
        m_modes[mode] = Mode();

        // return modes
        return m_modes;
    }
}




void PTCAPI Console::open(const char title[],int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(title,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(title,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


void PTCAPI Console::open(const char title[],const Format &format,int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(title,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(title,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


void PTCAPI Console::open(const char title[],int width,int height,const Format &format,int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(title,width,height,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(title,width,height,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


void PTCAPI Console::open(const char title[],const Mode &mode,int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(title,mode,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(title,mode,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


void PTCAPI Console::close()
{
    // check console
    if (console)
    {
        // close console
        console->close();
    }

    // reset that magical hacky option console flag!
    hacky_option_console_flag = false;
}





void PTCAPI Console::flush()
{
    // check
    check();

    // flush console
    console->flush();
}


void PTCAPI Console::finish()
{
    // check
    check();

    // finish console
    console->finish();
}


void PTCAPI Console::update()
{
    // check
    check();

    // update console
    console->update();
}


void PTCAPI Console::update(const Area &area)
{
    // check
    check();

    // update console area
    console->update(area);
}




bool PTCAPI Console::key()
{
    // check
    check();

    // check for key press
    return console->key();
}

                         
Key PTCAPI Console::read()
{
    // check
    check();
    
    // read key press
    return console->read();
}




void PTCAPI Console::copy(BaseSurface &surface)
{
    // check
    check();

    // copy console to surface
    console->copy(surface);
}


void PTCAPI Console::copy(BaseSurface &surface,const Area &source,const Area &destination)
{
    // check
    check();

    // copy console area to surface area
    console->copy(surface,source,destination);
}




void* PTCAPI Console::lock()
{
    // check
    check();

    // lock console
    return console->lock();
}


void PTCAPI Console::unlock()
{
    // check
    check();

    // unlock console
    console->unlock();
}




void PTCAPI Console::load(const void *pixels,int width,int height,int pitch,const Format &format,const Palette &palette)
{
    // check
    check();

    // load pixels to console
    console->load(pixels,width,height,pitch,format,palette);
}


void PTCAPI Console::load(const void *pixels,int width,int height,int pitch,const Format &format,const Palette &palette,const Area &source,const Area &destination)
{
    // check
    check();

    // load pixels to console area
    console->load(pixels,width,height,pitch,format,palette,source,destination);
}




void PTCAPI Console::save(void *pixels,int width,int height,int pitch,const Format &format,const Palette &palette)
{
    // check
    check();
    
    // save console pixels
    console->save(pixels,width,height,pitch,format,palette);
}


void PTCAPI Console::save(void *pixels,int width,int height,int pitch,const Format &format,const Palette &palette,const Area &source,const Area &destination)
{
    // check
    check();

    // save console area pixels
    console->save(pixels,width,height,pitch,format,palette,source,destination);
}




void PTCAPI Console::clear()
{
    // check
    check();

    // clear console
    console->clear();
}


void PTCAPI Console::clear(const Color &color)
{
    // check
    check();

    // clear console
    console->clear(color);
}


void PTCAPI Console::clear(const Color &color,const Area &area)
{
    // check
    check();

    // clear console area
    console->clear(color,area);
}




void PTCAPI Console::palette(const Palette &palette)
{
    // check
    check();

    // set console palette
    console->palette(palette);
}


const Palette& PTCAPI Console::palette() const
{
    // check
    check();

    // get console palette
    return console->palette();
}




void PTCAPI Console::clip(const Area &area)
{
    // check
    check();

    // set console clip area
    console->clip(area);
}




int PTCAPI Console::width() const
{
    // check
    check();

    // get console width
    return console->width();
}


int PTCAPI Console::height() const
{
    // check
    check();

    // get console height
    return console->height();
}


int PTCAPI Console::pages() const
{
    // check
    check();

    // get console pages
    return console->pages();
}


int PTCAPI Console::pitch() const
{
    // check
    check();

    // get console pitch
    return console->pitch();
}


const Area& PTCAPI Console::area() const
{
    // check
    check();

    // get console area
    return console->area();
}


const Area& PTCAPI Console::clip() const
{
    // check
    check();

    // get console clip area
    return console->clip();
}


const Format& PTCAPI Console::format() const
{
    // check
    check();

    // get console format
    return console->format();
}


const char* PTCAPI Console::name() const
{
    // check console
    if (console)
    {
        // get console name
        return console->name();
    }
    else
    {
        // get name
        return "Win32";
    }
}


const char* PTCAPI Console::title() const
{
    // check
    check();

    // get console title
    return console->title();
}


const char* PTCAPI Console::information() const
{
    // check
    check();

    // get console information
    return console->information();
}




#ifdef __PTC_WIN32_EXTENSIONS__


void PTCAPI Console::open(HWND window,int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(window,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(window,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


void PTCAPI Console::open(HWND window,const Format &format,int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(window,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(window,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


void PTCAPI Console::open(HWND window,int width,int height,const Format &format,int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(window,width,height,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(window,width,height,format,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


void PTCAPI Console::open(HWND window,const Mode &mode,int pages)
{
    // check console
    if (console)
    {
        try
        {
            // open console
            console->open(window,mode,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // free console
            delete console;

            // null console
            console = 0;

            // check the magical hacky option console create flag
            if (hacky_option_console_flag==true)
            {
                // reset that magical hacky option console flag!
                hacky_option_console_flag = false;

                // composite error message
                throw Error("could not open console",error);
            }
        }
    }

    // composite error
    Error composite;

    // console index
    int index = -1;

    // iterate
    while (true)
    {
        // next console
        index ++;

        try
        {
            // create console by index
            console = create(index);

            // check console
            if (!console) break;

            // open console
            console->open(window,mode,pages);

            // success
            return;
        }
        catch (Error &error)
        {
            // add error to composite error message
            composite = Error(error.message(),composite);

            // free console
            delete console;

            // null console
            console = 0;

            // failure
            continue;
        }
    }

    // free console
    delete console;

    // null console
    console = 0;

    // error message
    throw composite;
}


HWND PTCAPI Console::window() const
{
    // check
    check();

    // get console window
    return console->window();
}


LPDIRECTDRAW PTCAPI Console::lpDD() const
{
    // check
    check();

    // get console lpDD
    return console->lpDD();
}


LPDIRECTDRAW2 PTCAPI Console::lpDD2() const
{
    // check
    check();

    // get console lpDD2
    return console->lpDD2();
}



LPDIRECTDRAWSURFACE PTCAPI Console::lpDDS() const
{
    // check
    check();

    // get console lpDDS
    return console->lpDDS();
}



LPDIRECTDRAWSURFACE PTCAPI Console::lpDDS_primary() const
{
    // check
    check();

    // get console lpDDS primary
    return console->lpDDS_primary();
}



LPDIRECTDRAWSURFACE PTCAPI Console::lpDDS_secondary() const
{
    // check
    check();

    // get console lpDDS secondary
    return console->lpDDS_secondary();
}


#endif




BaseConsole* Console::create(int index)
{
    // check index
    switch (index)
    {
        case 0:  return new DirectXConsole();
        case 1:  return new GDIConsole();
    }

    // failure
    return 0;
}


BaseConsole* Console::create(const char name[])
{
    // check for "DirectX"
    if (stricmp(name,"DirectX")==0) return new DirectXConsole();

    // check for "GDI"
    if (stricmp(name,"GDI")==0) return new GDIConsole();

    // failure
    return 0;
}



void Console::check() const
{
    #ifdef __DEBUG__

        // check console pointer
        if (!console) throw Error("console is not open (core)");

    #else

        // no checking in release build

    #endif
}
