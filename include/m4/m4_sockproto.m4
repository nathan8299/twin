

/*
 *  m4/m4_sockproto.m4 --  macroized prototypes for libTw functions.
 *                   used as template for a lot of autogenerated files.
 *
 *                   the prototypes are used both on client and server side
 *                   to implement function calls <-> socket data stream
 *                   conversion.
 *                   You can also see this as a custom version of
 *                   `remote procedure calling'.
 *
 *  Copyright (C) 1999-2001 by Massimiliano Ghilardi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

/*

 format of this file: each non-empty line is of the form
 
 `PROTO'(<rettype>,<retflag>, <action>,<object>,<self>, <arg1type>,<arg1flag>, ...)

 the number of arguments the function wants must be deduced
 from the number of formal args in each `PROTO'(...) definition.

 <flag> : v = void
	  _ = scalar (any integer)
	  x = pointer
	  V(len) = vector of <len> elements
		<len> can be an expression, and may access the other arguments
		of the function as A(n) where <n> is the progressive number
		of the argument: A(1) is the first arg, A(2) the second, ...
	  W(len) = vector of <len> elements, with double-checking/autodetect on len
		so that you can safely pass NULL instead of the vector.

 Function calls wait until server has processed the command and returned the result
 if their return value is not void.
 
 <action> : the function name (Create, Map, Delete, ...)
 <object> : the object it acts upon (Gadget, Window, ...)
 
 <self> : 0 if the server implementation does not need the Fn##object method pointer
	: 1 if the function in the server needs the Fn##object method pointer
	: 2 if the Fn##object method pointer is extracted from the first argument
	

*/

#define OK_MAGIC	((uldat)0x3E4B4F3Cul)
#define FAIL_MAGIC	((uldat)0xFFFFFFFFul)

#define FIND_MAGIC		((uldat)0x646E6946ul) /* i.e. "Find" */
#define MSG_MAGIC		((uldat)0x2167734dul) /* i.e. "Msg!" */

PROTOFindFunction(uldat,_, Find,Function,0, byte,_, byte,V(A(1)), byte,_, byte,V(A(3)))

PROTOSyncSocket(byte,_,  Sync,Socket,0)

PROTO(byte,_, Server,Sizeof,0, byte,_)

PROTO(byte,_, Can,Compress,0)
PROTO(byte,_, Do,Compress,0, byte,_)

PROTO(void,v, NeedResize,Display,0)

PROTO(void,v, Attach,HW,0, uldat,_, byte,V(A(1)), byte,_)
PROTO(byte,_, Detach,HW,0, uldat,_, byte,V(A(1)))

PROTO(void,v,  Set,   FontTranslation,0,  byte,V(0x80))
PROTO(void,v,  Set,UniFontTranslation,0,hwfont,V(0x80))

PROTO(void,v,     Delete,Obj,0, obj,x)
PROTO(void,v,ChangeField,Obj,2, obj,x, udat,_, uldat,_, uldat,_)

PROTO(widget,x,   Create,Widget,0, dat,_, dat,_, uldat,_, uldat,_, dat,_, dat,_, hwattr,_)
PROTO(void,v,RecursiveDelete,Widget,0, widget,x) /* it is wrapped in socket.c */
PROTO(void,v,        Map,Widget,2, widget,x, widget,x)
PROTO(void,v,      UnMap,Widget,2, widget,x)
PROTO(void,v,      SetXY,Widget,0, widget,x, dat,_, dat,_)
PROTO(msgport,x,GetOwner,Widget,0, widget,x)
PROTO(void,v,     Expose,Widget,0, widget,x, dat,_, dat,_, dat,_, dat,_, byte,W(A(2)*A(3)), hwfont,W(A(2)*A(3)), hwattr,W(A(2)*A(3)))
				    

PROTO(gadget,x, Create,Gadget,0,
	widget,x, dat,_, dat,_, byte,W(A(2)*A(3)),
	uldat,_, uldat,_, udat,_, hwcol,_,hwcol,_,hwcol,_,hwcol,_,
	dat,_, dat,_)

PROTO(gadget,x, CreateButton,Gadget,1, widget,x, dat,_, dat,_, byte,V(A(2)*A(3)),
	uldat,_, udat,_, hwcol,_, hwcol,_, hwcol,_, dat,_, dat,_)

PROTO(void,v, WriteTexts,Gadget,2, gadget,x, byte,_, dat,_, dat,_, byte,W(A(2)*A(3)), dat,_, dat,_)
PROTO(void,v, WriteHWFonts,Gadget,2, gadget,x, byte,_, dat,_, dat,_, hwfont,W(A(2)*A(3)), dat,_, dat,_)

									
PROTO(void,v,   Create4Menu,Row,0, window,x, udat,_, byte,_, uldat,_, byte,V(A(4)))

PROTO(window,x,     Create,Window,1, dat,_, byte,V(A(1)), hwcol,W(A(1)), menu,x,
	hwcol,_, uldat,_, uldat,_, uldat,_, dat,_, dat,_, dat,_)
PROTO(window,x, Create4Menu,Window,1, menu,x)
PROTO(void,v,  WriteAscii  ,Window,2, window,x, ldat,_, byte,V(A(2)))
PROTO(void,v,  WriteString ,Window,2, window,x, ldat,_, byte,V(A(2)))
PROTO(void,v,  WriteHWFont, Window,2, window,x, ldat,_, hwfont,V(A(2)))
PROTO(void,v,  WriteHWAttr ,Window,2, window,x,  dat,_, dat,_, ldat,_, hwattr,V(A(4)))
PROTO(void,v,  WriteRow    ,Window,2, window,x, ldat,_, byte,V(A(2)))

PROTO(void,v,        GotoXY,Window,2, window,x, ldat,_, ldat,_)
PROTO(void,v,    SetColText,Window,2, window,x, hwcol,_)
PROTO(void,v, SetColors,Window,2, window,x, udat,_, hwcol,_, hwcol,_,
	hwcol,_, hwcol,_, hwcol,_, hwcol,_, hwcol,_, hwcol,_, hwcol,_)
PROTO(void,v,     Configure,Window,2, window,x, byte,_, dat,_, dat,_, dat,_, dat,_, dat,_, dat,_)
PROTO(void,v,     Resize,Window,0, window,x, dat,_, dat,_)

PROTO(widget,x,FindWidgetAt,Widget,2, widget,x, dat,_, dat,_)
PROTO(void,v,FocusSub,Widget,0, widget,x)

PROTO(group,x,      Create,Group,0)
PROTO(void,v, InsertGadget,Group,2, group,x, gadget,x)
PROTO(void,v, RemoveGadget,Group,2, group,x, gadget,x)

PROTO(gadget,x, GetSelectedGadget,Group,2, group,x)
PROTO(void,v,   SetSelectedGadget,Group,2, group,x, gadget,x)

PROTO(menuitem,x,    Create4Menu,MenuItem,0,  obj,x, window,x, byte,_, dat,_, byte,V(A(4)))
PROTO(uldat,_, Create4MenuCommon,MenuItem,1, menu,x)

PROTO(menu,x, Create,Menu,0, hwcol,_, hwcol,_, hwcol,_, hwcol,_, hwcol,_, hwcol,_, byte,_)
PROTO(void,v, SetInfo,Menu,2, menu,x, byte,_, ldat,_, byte,V(A(3)), hwcol,W(A(3)))

PROTO(msgport,x,Create,MsgPort,0, byte,_, byte,V(A(1)), time_t,_, frac_t,_, byte,_)
PROTO(msgport,x,  Find,MsgPort,0, msgport,x, byte,_, byte,V(A(2)))

PROTO(void,v,  BgImage,Screen,2, screen,x, dat,_, dat,_, hwattr,V(A(2)*A(3)))

PROTO(obj,x,      Prev,Obj,0, obj,x)
PROTO(obj,x,      Next,Obj,0, obj,x)
PROTO(obj,x,    Parent,Obj,0, obj,x)

PROTO(gadget  ,x, G_Prev,Gadget, 0, gadget ,x)
PROTO(gadget  ,x, G_Next,Gadget, 0, gadget ,x)
PROTO(group   ,x, Group ,Gadget, 0, gadget ,x)

PROTO(widget  ,x, O_Prev,Widget, 0, widget ,x)
PROTO(widget  ,x, O_Next,Widget, 0, widget ,x)
PROTO(msgport ,x, Owner ,Widget, 0, widget ,x)

PROTO(screen  ,x, First,Screen,  0)
PROTO(widget  ,x, First,Widget,  0, widget ,x)
PROTO(msgport ,x, First,MsgPort, 0)
PROTO(menu    ,x, First,Menu,    0, msgport,x)
PROTO(widget  ,x, First,W,       0, msgport,x)
PROTO(group   ,x, First,Group,   0, msgport,x)
PROTO(mutex   ,x, First,Mutex,   0, msgport,x)
PROTO(menuitem,x, First,MenuItem,0, menu   ,x)
PROTO(gadget  ,x, First,Gadget,  0, group  ,x)


PROTO(dat,_, GetDisplay,Width,0)
PROTO(dat,_, GetDisplay,Height,0)

PROTO(byte,_, SendTo,MsgPort,0, msgport,x, udat,_, byte,V(A(2)))
PROTO(void,v, BlindSendTo,MsgPort,0, msgport,x, udat,_, byte,V(A(2)))

PROTO(obj, x, GetOwner,Selection,0)
PROTO(void,v, SetOwner,Selection,0, time_t,_, frac_t,_)
PROTO(void,v,  Request,Selection,0, obj,x, uldat,_)
PROTO(void,v,   Notify,Selection,0, obj,x, uldat,_, uldat,_, byte,V(TW_MAX_MIMELEN), uldat,_, byte,V(A(5)))

PROTO(byte,_, SetServer,Uid,0, uldat,_, byte,_)

