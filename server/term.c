/*
 *  term.c  --  create and manage multiple terminal emulator windows on twin
 *
 *  Copyright (C) 1999-2000 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "twin.h"
#include "main.h"
#include "data.h"
#include "methods.h"

#include "remote.h"
#include "pty.h"
#include "tty.h"
#include "resize.h"
#include "util.h"
#include "builtin.h" /* for Builtin_Term_Menu */
#include "common.h"

#define COD_QUIT      (udat)1
#define COD_SPAWN     (udat)3

static char *args[3];

static msgport *Term_MsgPort;
menu *Term_Menu;

static void TwinTermH(msgport *MsgPort);
static void TwinTermIO(int Fd, window *Window);

static void termShutDown(window *Window) {
    if (Window->RemoteData.Fd != NOFD)
	close(Window->RemoteData.Fd);
    UnRegisterWindowFdIO(Window);
}

static window *newTermWindow(void) {
    window *Window = Do(Create,Window)
	(FnWindow, (udat)11, " Twin Term ", (hwcol *)0, Term_Menu, COL(WHITE,BLACK),
	 LINECURSOR, WINDOW_WANT_CHANGE|WINDOW_WANT_KEYS|WINDOW_DRAG|WINDOW_RESIZE|WINDOW_Y_BAR|WINDOW_CLOSE,
	 WINFL_CURSOR_ON|WINFL_USECONTENTS,
	 (udat)82, (udat)27, (uldat)200);
    
    if (Window)
	Act(SetColors,Window)
	(Window, 0x1FF, COL(HIGH|YELLOW,CYAN), COL(HIGH|GREEN,HIGH|BLUE), COL(WHITE,HIGH|BLUE),
	 COL(HIGH|WHITE,HIGH|BLUE), COL(HIGH|WHITE,HIGH|BLUE),
	 COL(WHITE,BLACK), COL(WHITE,HIGH|BLACK), COL(HIGH|BLACK,BLACK), COL(BLACK,HIGH|BLACK));
    
    return Window;
}

static byte OpenTerm(void) {
    window *Window;
    
    if ((Window = newTermWindow())) {
        if (SpawnInWindow(Window, args)) {
	    if (RegisterWindowFdIO(Window, TwinTermIO)) {
		Window->ShutDownHook = termShutDown;
		Act(Map,Window)(Window, All->FirstScreen);
		return TRUE;
	    }
	    close(Window->RemoteData.Fd);
	}
	Delete(Window);
    }
    return FALSE;
}

#ifdef MODULE
static
#endif
byte InitTerm(void) {
    window *Window;
    byte *shellpath, *shell;
    
    if ((shellpath = getenv("SHELL")) &&
	(args[0] = CloneStr(shellpath)) &&
	(args[1] = (shell = strrchr(shellpath, '/'))
	 ? CloneStr(shell) : CloneStr(shellpath)) &&
    
	(Term_MsgPort=Do(Create,MsgPort)
	 (FnMsgPort, 9, "Twin Term", (uldat)0, (udat)0, (byte)0, TwinTermH)) &&
	(Term_Menu=Do(Create,Menu)
	 (FnMenu, Term_MsgPort,
	  COL(BLACK,WHITE), COL(BLACK,GREEN), COL(HIGH|BLACK,WHITE), COL(HIGH|BLACK,BLACK),
	  COL(RED,WHITE), COL(RED,GREEN), (byte)0)) &&
	Info4Menu(Term_Menu, ROW_ACTIVE, (uldat)20, " Built-in Twin Term ", "ptpppppppptpppptpppp") &&

	(Window=Win4Menu(Term_Menu)) &&
	Row4Menu(Window, COD_SPAWN, ROW_ACTIVE, 10, " New Term ") &&
	Row4Menu(Window, COD_QUIT,  FALSE,       6, " Exit ") &&
	Item4Menu(Term_Menu, Window, TRUE, 6, " File ") &&
	
	Item4MenuCommon(Term_Menu)) {
	

	if (args[1][0] == '/')
	    args[1][0] = '-';
	return TRUE;
    }
    return FALSE;
}


static void TwinTermH(msgport *MsgPort) {
    msg *Msg;
    event_any *Event;
    udat Code/*, Repeat*/;
    window *Win;
    
    while ((Msg=Term_MsgPort->FirstMsg)) {
	Remove(Msg);
	
	Event=&Msg->Event;
	if (Msg->Type==MSG_WINDOW_KEY) {
	    Code=Event->EventKeyboard.Code;
	    /* send keypresses */
	    Win = Event->EventKeyboard.Window;
	    (void)RemoteWindowWriteQueue(Win, Event->EventKeyboard.SeqLen,
					 Event->EventKeyboard.AsciiSeq);
	} else if (Msg->Type==MSG_SELECTION) {
	    
	    if ((Win = Event->EventSelection.Window))
		TwinSelectionRequest((obj *)Term_MsgPort, Win->Id, TwinSelectionGetOwner());

	} else if (Msg->Type==MSG_SELECTIONNOTIFY) {
	    
	    if ((Win = (window *)Id2Obj(window_magic >> magic_shift,
					Event->EventSelectionNotify.ReqPrivate)))
		(void)RemoteWindowWriteQueue(Win, Event->EventSelectionNotify.Len,
					     Event->EventSelectionNotify.Data);
	    
	} else if (Msg->Type==MSG_WINDOW_MOUSE) {
	    Code=Event->EventMouse.Code;
	    /* send mouse movements */
	    Win = Event->EventMouse.Window;
	    if (Win && Win->TtyData) {
		byte len = 0, buf[10] = "\033[?M";
		udat x, y;
		
		x = Event->EventMouse.X;
		y = Event->EventMouse.Y;
		    
		if (Win->TtyData->Flags & TTY_REPORTMOUSE2) {
		    /* classic xterm-style reporting */
		    buf[2] = 'M';
		    if (isPRESS(Code)) switch (Code & PRESS_ANY) {
		      case PRESS_LEFT: buf[3] = ' '; break;
		      case PRESS_MIDDLE: buf[3] = '!'; break;
		      case PRESS_RIGHT: buf[3] = '\"'; break;
		    }
		    else if (isRELEASE(Code))
			buf[3] = '#';
		    else {
			Delete(Msg);
			continue;
		    }
		    buf[4] = '!' + x;
		    buf[5] = '!' + y;
		    len = 6;
		} else if (Win->TtyData->Flags & TTY_REPORTMOUSE) {
		    /* new-style reporting */
		    buf[2] = '5';
		    buf[3] = 'M';
		    buf[4] = ' ' + (Code & HOLD_ANY);
		    buf[5] = '!' + (x & 0x7f);
		    buf[6] = '!' + (x >> 7);
		    buf[7] = '!' + (y & 0x7f);
		    buf[8] = '!' + (y >> 7);
		    len = 9;
		}
		if (len)
		    (void)RemoteWindowWriteQueue(Win, len, buf);
	    }
	} else if (Msg->Type==MSG_WINDOW_GADGET) {
	    if (!Event->EventGadget.Code)
		/* 0 == Close Code */
		Delete(Event->EventGadget.Window);
	} else if (Msg->Type==MSG_MENU_ROW) {
	    if (Event->EventMenu.Menu==Term_Menu) {
		Code=Event->EventMenu.Code;
		switch (Code) {
		  case COD_SPAWN:
		    OpenTerm();
		    break;
		  default:
		    break;
		}
	    }
	}
	Delete(Msg);
    }
}

static void TwinTermIO(int Fd, window *Window) {
    static byte buf[BIGBUFF];
    uldat got = 0, chunk = 0;
    
    do {
	/*
	 * BIGBUFF - 1 to avoid silly windows...
	 * linux ttys buffer up to 4095 bytes.
	 */
	chunk = read(Fd, buf + got, BIGBUFF - 1 - got);
    } while (chunk && chunk != (uldat)-1 && (got += chunk) < BIGBUFF - 1);
    
    if (got)
	Act(WriteAscii,Window)(Window, got, buf);
    else if (chunk == (uldat)-1 && errno != EINTR && errno != EAGAIN)
	/* something bad happened to our child :( */
	Delete(Window);
}

#ifdef MODULE
static void OverrideMethods(byte enter) {
    static void (*oldWriteAscii)(window *, uldat, byte *);
    static void (*oldWriteHWAttr)(window *, udat, udat, uldat, hwattr *);
    static window *(*oldFocus)(window *);

    if (enter) {
	if (!oldWriteAscii && FnWindow->WriteAscii != WriteAscii) {
	    oldWriteAscii = FnWindow->WriteAscii;
	    FnWindow->WriteAscii = WriteAscii;
	}
	if (!oldWriteHWAttr && FnWindow->WriteHWAttr != WriteHWAttr) {
	    oldWriteHWAttr = FnWindow->WriteHWAttr;
	    FnWindow->WriteHWAttr = WriteHWAttr;
	}
	if (!oldFocus && FnWindow->Focus != KbdFocus) {
	    oldFocus = FnWindow->Focus;
	    FnWindow->Focus = KbdFocus;
	    ForceKbdFocus();
	}
    } else {
	if (oldWriteAscii && FnWindow->WriteAscii == WriteAscii) {
	    FnWindow->WriteAscii = oldWriteAscii;
	    oldWriteAscii = NULL;
	}
	if (oldWriteHWAttr && FnWindow->WriteHWAttr == WriteHWAttr) {
	    FnWindow->WriteHWAttr = oldWriteHWAttr;
	    oldWriteHWAttr = NULL;
	}
	if (oldFocus && FnWindow->Focus == KbdFocus) {
	    ForceKbdFocus();
	    FnWindow->Focus = oldFocus;
	    oldFocus = NULL;
	}
    }
}

/* from builtin.c */
extern menu *Builtin_Term_Menu;

byte InitModule(module *Module) {
    if (InitTerm()) {
	OverrideMethods(TRUE);
	/* let setup.c know we exist */
	Builtin_Term_Menu = Term_Menu;
	return TRUE;
    }
    return FALSE;
}

void QuitModule(module *Module) {
    /* cast 'oblivion' on setup.c */
    window *Window;
    Builtin_Term_Menu = NULL;
    OverrideMethods(FALSE);
    if (Term_MsgPort) {
	if (Term_Menu)
	    for (Window = Term_Menu->FirstWindow; Window; Window = Window->ONext) {
	    }
	Delete(Term_MsgPort);
    }
}
#endif /* MODULE */
