/*      echoedit.h
 *
 * MidpNT echo effect editor view
 *
 * $Id: echoedit.h,v 1.1 1997/07/10 18:41:09 pekangas Exp $
 *
 * Copyright 1997 Petteri Kangaslampi
*/

#ifndef __echoedit_h
#define __echoedit_h

#define MAXEFFECTS 256

#include "registry.h"


class EchoEditWindow;

class EchoEditView : public midpView
{
private:
    EchoEditWindow *window;
public:
    EchoEditView();
    virtual ~EchoEditView();
    virtual char *Name(void);
    virtual char *Description(void);
    virtual midpViewWindow *CreateViewWindow(Registry *registry = NULL);
    virtual void DestroyViewWindow(midpViewWindow *window);
};


class EchoEditWindow : public midpViewWindow
{
protected:
    midpModeless    modeless;
    midpEchoEffect  *effects[MAXEFFECTS];
    midpEchoEffect  *e;
    unsigned numEffects;
    unsigned currentNum;
    int init;
public:
    EchoEditWindow(int instanceNumber, midpView *view,
        Registry *registry = NULL);
    virtual ~EchoEditWindow(void);
    void UpdateEffects();
    void SelectEffect(unsigned number);
    void ReadEffect();
    void RenameEffect();
    BOOL CALLBACK EchoEditWindow::ClassDialogProc(HWND hwnd, UINT message,
        WPARAM wparam, LPARAM lparam);
};


#endif

/*
 * $Log: echoedit.h,v $
 * Revision 1.1  1997/07/10 18:41:09  pekangas
 * Initial revision
 *
*/
