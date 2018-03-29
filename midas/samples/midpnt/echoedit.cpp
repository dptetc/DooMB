/*      echoedit.cpp
 *
 * MidpNT echo effect editor view
 *
 * $Id: echoedit.cpp,v 1.3 1997/07/11 13:36:39 pekangas Exp $
 *
 * Copyright 1996 Petteri Kangaslampi
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "midasdll.h"
#include "MidpView.h"
#include "MidpNT.h"
#include "MidpModeless.h"
#include "midpecho.h"
#include "echoedit.h"
#include "midpres.h"
#include "ViewList.h"


static BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wparam,
    LPARAM lparam);

extern midpEchoEffectList echoEffectList;


#define NUMECHOEDITS 8

typedef struct
{
    int delayEdit, gainEdit, reverseCheck, filterCombo;
} EchoEditControls;

EchoEditControls echoControls[NUMECHOEDITS] =
{
    { ECHOEDITOR_DELAY1, ECHOEDITOR_GAIN1, ECHOEDITOR_REVERSE1,
      ECHOEDITOR_FILTER1 },
    { ECHOEDITOR_DELAY2, ECHOEDITOR_GAIN2, ECHOEDITOR_REVERSE2,
      ECHOEDITOR_FILTER2 },
    { ECHOEDITOR_DELAY3, ECHOEDITOR_GAIN3, ECHOEDITOR_REVERSE3,
      ECHOEDITOR_FILTER3 },
    { ECHOEDITOR_DELAY4, ECHOEDITOR_GAIN4, ECHOEDITOR_REVERSE4,
      ECHOEDITOR_FILTER4 },
    { ECHOEDITOR_DELAY5, ECHOEDITOR_GAIN5, ECHOEDITOR_REVERSE5,
      ECHOEDITOR_FILTER5 },
    { ECHOEDITOR_DELAY6, ECHOEDITOR_GAIN6, ECHOEDITOR_REVERSE6,
      ECHOEDITOR_FILTER6 },
    { ECHOEDITOR_DELAY7, ECHOEDITOR_GAIN7, ECHOEDITOR_REVERSE7,
      ECHOEDITOR_FILTER7 },
    { ECHOEDITOR_DELAY8, ECHOEDITOR_GAIN8, ECHOEDITOR_REVERSE8,
      ECHOEDITOR_FILTER8 }
};



EchoEditView::EchoEditView()
{
    window = NULL;
}


EchoEditView::~EchoEditView()
{
    if ( window != NULL )
    {
        delete window;
        window = NULL;
    }
}


char *EchoEditView::Name(void)
{
    return "EchoEditView";
}


char *EchoEditView::Description(void)
{
    return "Echo Effect Editor";
}


midpViewWindow *EchoEditView::CreateViewWindow(Registry *registry)
{
    if ( window != NULL )
        return (midpViewWindow*) window;
    window = new EchoEditWindow(0, this, registry);
    return (midpViewWindow*) window;
}


void EchoEditView::DestroyViewWindow(midpViewWindow *_window)
{
    assert(_window == window);
    _window = _window;

    delete window;
    window = NULL;
}



EchoEditWindow::EchoEditWindow(int instanceNumber, midpView *view,
                               Registry *registry)
    : midpViewWindow(instanceNumber, view, registry)
{
    HWND        parent = NULL;

    numEffects = 0;
    e = NULL;
    init = 0;

    if ( viewsChildren )
        parent = mainWinHandle;

    instanceNumber = instanceNumber;

    modeless.hwnd = hwnd = CreateDialogParam(instance, "ECHOEDITOR",
                                             parent, (DLGPROC) DialogProc,
                                             (LPARAM) this);

    midpModelessList.AddItem(&modeless);
    viewWindowList.AddWindow(this);
}



EchoEditWindow::~EchoEditWindow(void)
{
    viewWindowList.RemoveWindow(this);
    midpModelessList.RemoveItem(&modeless);
    DestroyWindow(hwnd);
}



static BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wparam,
    LPARAM lparam)
{
    EchoEditWindow *window;

    if ( message == WM_INITDIALOG )
        SetWindowLong(hwnd, GWL_USERDATA, lparam);

    window = (EchoEditWindow*) GetWindowLong(hwnd, GWL_USERDATA);

    return window->ClassDialogProc(hwnd, message, wparam, lparam);
}


BOOL CALLBACK EchoEditWindow::ClassDialogProc(HWND hwnd, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    char str[64];
    int n;
    unsigned i;
    
    hwnd = hwnd;
    wparam = wparam;
    lparam = lparam;

    str[0] = 0;

    switch ( message )
    {
        case WM_INITDIALOG:
            this->hwnd = hwnd;
            if ( startX != CW_USEDEFAULT )
                SetWindowPos(hwnd, NULL, startX, startY, 17, 42,
                    SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER | SWP_NOACTIVATE);
            SendDlgItemMessage(hwnd, ECHOEDITOR_NUMECHOES, EM_SETLIMITTEXT,
                               1, 0);

            for ( i = 0; i < NUMECHOEDITS; i++ )
            {
                n = echoControls[i].filterCombo;
                SendDlgItemMessage(hwnd, n, CB_ADDSTRING, 0, (LPARAM) "None");
                SendDlgItemMessage(hwnd, n, CB_ADDSTRING, 0,
                                   (LPARAM) "Low-pass");
                SendDlgItemMessage(hwnd, n, CB_ADDSTRING, 0,
                                   (LPARAM) "High-pass");
            }
            
            UpdateEffects();
            SelectEffect(0);
            init = 1;
            return TRUE;
            
        case WM_CLOSE:
            ownerView->DestroyViewWindow(this);
            return TRUE;

        case WM_COMMAND:
            switch ( LOWORD(wparam) )
            {
                case IDCANCEL:
                    ReadEffect();
                    ownerView->DestroyViewWindow(this);
                    return TRUE;

                case ECHOEDITOR_APPLY:
                    ReadEffect();
                    e->Activate();
                    return TRUE;

                case ECHOEDITOR_NUMECHOES:
                    if ( !init )
                        return FALSE;
                    switch ( HIWORD(wparam) )
                    {
                        case EN_CHANGE:
                            SendDlgItemMessage(hwnd, ECHOEDITOR_NUMECHOES,
                                               WM_GETTEXT, 63, (LPARAM) str);
                            if ( strlen(str) > 0 )
                            {
                                n = atoi(str);
                                if ( n < 0 )
                                    n = 0;
                                if ( n > MAXECHOES )
                                    n = MAXECHOES;
                                if ( n == (int) e->GetNumEchoes() )
                                    return TRUE;
                                ReadEffect();
                                e->SetNumEchoes(n);
                                SelectEffect(currentNum);
                            }
                            return TRUE;
                    }
                    return FALSE;

                case ECHOEDITOR_ADDECHOBUTTON:
                    ReadEffect();
                    if ( e->GetNumEchoes() < MAXECHOES )
                        e->SetNumEchoes(e->GetNumEchoes() + 1);
                    SelectEffect(currentNum);
                    return TRUE;

                case ECHOEDITOR_DELECHOBUTTON:
                    ReadEffect();
                    if ( e->GetNumEchoes() > 0 )
                        e->SetNumEchoes(e->GetNumEchoes() - 1);
                    SelectEffect(currentNum);
                    return TRUE;

                case ECHOEDITOR_RENAME:
                    RenameEffect();
                    return TRUE;

                case ECHOEDITOR_SAVE:
                    ReadEffect();
                    e->SaveToRegistry();
                    return TRUE;

                case ECHOEDITOR_DELETE:
                    echoEffectList.DeleteEffect(e);
                    UpdateEffects();
                    SelectEffect(currentNum);
                    return TRUE;

                case ECHOEDITOR_NEW:
                    if ( numEffects >= MAXEFFECTS )
                        return FALSE;
                    ReadEffect();
                    e = echoEffectList.NewEffect();
                    UpdateEffects();
                    SelectEffect(numEffects);
                    numEffects++;
                    return TRUE;

                case ECHOEDITOR_EFFECTNAME:
                    switch ( HIWORD(wparam) )
                    {
                        case CBN_SELCHANGE:
                            ReadEffect();
                            SelectEffect(SendDlgItemMessage(hwnd,
                                ECHOEDITOR_EFFECTNAME, CB_GETCURSEL, 0, 0));
                            e->Activate();
                            return TRUE;
                    }
                    return FALSE;
            }
    }

    return FALSE;
}



void EchoEditWindow::UpdateEffects()
{
    midpEchoEffect *effect;
    
    SendDlgItemMessage(hwnd, ECHOEDITOR_EFFECTNAME, CB_RESETCONTENT, 0, 0);

    numEffects = 0;    

    effect = echoEffectList.FirstEffect();
    while ( effect != NULL )
    {
        effects[numEffects++] = effect;
        SendDlgItemMessage(hwnd, ECHOEDITOR_EFFECTNAME, CB_ADDSTRING, 0,
                           (LPARAM) effect->GetName());
        effect = echoEffectList.NextEffect();        
    }
}


void EchoEditWindow::SelectEffect(unsigned number)
{
    midpEcho *echo;
    unsigned n;
    unsigned numEchoes;
    char str[64];
    
    if ( number > (numEffects-1) )
        number = numEffects-1;    

    e = effects[number];
    currentNum = number;    
    SendDlgItemMessage(hwnd, ECHOEDITOR_EFFECTNAME, CB_SETCURSEL, number, 0);

    SendDlgItemMessage(hwnd, ECHOEDITOR_NUMECHOES, WM_SETTEXT, 0,
                       (LPARAM) itoa(e->GetNumEchoes(), str, 10));

    sprintf(str, "%f", e->GetFeedback());
    SendDlgItemMessage(hwnd, ECHOEDITOR_FEEDBACKEDIT, WM_SETTEXT, 0,
                       (LPARAM) str);

    sprintf(str, "%f", e->GetGain());
    SendDlgItemMessage(hwnd, ECHOEDITOR_GAINEDIT, WM_SETTEXT, 0,
                       (LPARAM) str);

    numEchoes = e->GetNumEchoes();
    if ( numEchoes > NUMECHOEDITS )
        numEchoes = NUMECHOEDITS;

    for ( n = 0; n < numEchoes; n++ )
    {
        echo = e->GetEcho(n);
        
        EnableWindow(GetDlgItem(hwnd, echoControls[n].delayEdit), TRUE);
        sprintf(str, "%f", echo->GetDelay());
        SendDlgItemMessage(hwnd, echoControls[n].delayEdit, WM_SETTEXT, 0,
                           (LPARAM) str);

        EnableWindow(GetDlgItem(hwnd, echoControls[n].gainEdit), TRUE);
        sprintf(str, "%f", echo->GetGain());
        SendDlgItemMessage(hwnd, echoControls[n].gainEdit, WM_SETTEXT, 0,
                           (LPARAM) str);

        EnableWindow(GetDlgItem(hwnd, echoControls[n].reverseCheck), TRUE);
        SendDlgItemMessage(hwnd, echoControls[n].reverseCheck, BM_SETCHECK,
                           echo->GetReverse(), 0);

        EnableWindow(GetDlgItem(hwnd, echoControls[n].filterCombo), TRUE);
        SendDlgItemMessage(hwnd, echoControls[n].filterCombo, CB_SETCURSEL,
                           echo->GetFilterType(), 0);
    }

    for ( n = numEchoes; n < NUMECHOEDITS; n++ )
    {
        EnableWindow(GetDlgItem(hwnd, echoControls[n].delayEdit), FALSE);
        SendDlgItemMessage(hwnd, echoControls[n].delayEdit, WM_SETTEXT, 0,
                           (LPARAM) "------");
        
        EnableWindow(GetDlgItem(hwnd, echoControls[n].gainEdit), FALSE);
        SendDlgItemMessage(hwnd, echoControls[n].gainEdit, WM_SETTEXT, 0,
                           (LPARAM) "------");
        
        EnableWindow(GetDlgItem(hwnd, echoControls[n].reverseCheck), FALSE);
        SendDlgItemMessage(hwnd, echoControls[n].reverseCheck, BM_SETCHECK,
                           0, 0);

        EnableWindow(GetDlgItem(hwnd, echoControls[n].filterCombo), FALSE);
        SendDlgItemMessage(hwnd, echoControls[n].filterCombo, CB_SETCURSEL,
                           0, 0);        
    }
}



void EchoEditWindow::ReadEffect()
{
    midpEcho *echo;
    unsigned n;
    unsigned numEchoes;
    char str[64];
    
    SendDlgItemMessage(hwnd, ECHOEDITOR_FEEDBACKEDIT, WM_GETTEXT, 63,
                       (LPARAM) str);
    e->SetFeedback(atof(str));

    SendDlgItemMessage(hwnd, ECHOEDITOR_GAINEDIT, WM_GETTEXT, 63,
                       (LPARAM) str);
    e->SetGain(atof(str));

    numEchoes = e->GetNumEchoes();
    if ( numEchoes > NUMECHOEDITS )
        numEchoes = NUMECHOEDITS;

    for ( n = 0; n < numEchoes; n++ )
    {
        echo = e->GetEcho(n);

        SendDlgItemMessage(hwnd, echoControls[n].delayEdit, WM_GETTEXT, 63,
                           (LPARAM) str);
        echo->SetDelay(atof(str));

        SendDlgItemMessage(hwnd, echoControls[n].gainEdit, WM_GETTEXT, 63,
                           (LPARAM) str);
        echo->SetGain(atof(str));

        echo->SetReverse(SendDlgItemMessage(hwnd, echoControls[n].reverseCheck,
                                            BM_GETCHECK, 0, 0));

        echo->SetFilterType(SendDlgItemMessage(hwnd,
                                               echoControls[n].filterCombo,
                                               CB_GETCURSEL, 0, 0));
    }
}



static BOOL CALLBACK RenameDialogProc(HWND hwnd, UINT msg, WPARAM wparam,
                                      LPARAM lparam)
{
    static char *dest;

    switch ( msg )
    {
        case WM_INITDIALOG:
            dest = (char*) lparam;
            SendDlgItemMessage(hwnd, ECHORENAME_NAME, WM_SETTEXT, 0,
                               (LPARAM) dest);
            return TRUE;

        case WM_COMMAND:
            switch ( LOWORD(wparam) )
            {
                case IDOK:
                    SendDlgItemMessage(hwnd, ECHORENAME_NAME, WM_GETTEXT, 63,
                                       (LPARAM) dest);
                    EndDialog(hwnd, 1);
                    return TRUE;

                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;
            }
    }

    return FALSE;
}


void EchoEditWindow::RenameEffect()
{
    char name[64];

    strcpy(name, e->GetName());
    if ( DialogBoxParam(instance, "ECHORENAME", hwnd,
                        (DLGPROC) RenameDialogProc, (LPARAM) name) == 1 )
        e->Rename(name);
    UpdateEffects();
    SelectEffect(currentNum);
}



/*
 * $Log: echoedit.cpp,v $
 * Revision 1.3  1997/07/11 13:36:39  pekangas
 * Added filters and total gain to echoes.
 * Fixed crashes when output mode was changed when an echo effect was
 * active.
 *
 * Revision 1.2  1997/07/10 19:00:58  pekangas
 * Fixed a Watcom C retail build error
 *
 * Revision 1.1  1997/07/10 18:41:09  pekangas
 * Initial revision
 *
*/
