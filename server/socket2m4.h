



/* This file was automatically generated from m4/socket2.m4, do not edit! */
























/*
 *  m4/m4_sockproto.m4 --  macroized prototypes for libTw functions.
 *                   used as template for a lot of autogenerated files.
 *
 *                   the prototypes are used both on client and server side
 *                   to implement function calls <-> socket data stream
 *                   conversion.
 *                   You can also see this as a custom version of
 *                   remote procedure calling.
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
 
 PROTO(<rettype>,<retflag>, <action>,<object>,<self>, <arg1type>,<arg1flag>, ...)

 the number of arguments the function wants must be deduced
 from the number of formal args in each PROTO(...) definition.

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

{ 0, 0, "FindFunction",
    "0""_"TWS_uldat_STR"_"TWS_byte_STR"V"TWS_byte_STR"_"TWS_byte_STR"V"TWS_byte_STR },

{ 0, 0, "SyncSocket",
    "0""_"TWS_byte_STR },

{ 0, 0, "ServerSizeof",
    "0""_"TWS_byte_STR"_"TWS_byte_STR },

{ 0, 0, "CanCompress",
    "0""_"TWS_byte_STR },
{ 0, 0, "DoCompress",
    "0""_"TWS_byte_STR"_"TWS_byte_STR },

{ 0, 0, "NeedResizeDisplay",
    "0""v"TWS_void_STR },

{ 0, 0, "AttachHW",
    "0""v"TWS_void_STR"_"TWS_uldat_STR"V"TWS_byte_STR"_"TWS_byte_STR },
{ 0, 0, "DetachHW",
    "0""_"TWS_byte_STR"_"TWS_uldat_STR"V"TWS_byte_STR },

{ 0, 0, "SetFontTranslation",
    "0""v"TWS_void_STR"V"TWS_byte_STR },

{ 0, 0, "DeleteObj",
    "0""v"TWS_void_STR"x"obj_magic_STR },
{ 0, 0, "ChangeFieldObj",
    "2""v"TWS_void_STR"x"obj_magic_STR"_"TWS_udat_STR"_"TWS_uldat_STR"_"TWS_uldat_STR },

{ 0, 0, "CreateWidget",
    "0""x"widget_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_uldat_STR"_"TWS_uldat_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_hwattr_STR },
{ 0, 0, "RecursiveDeleteWidget",
    "0""v"TWS_void_STR"x"widget_magic_STR }, /* it is wrapped in socket.c */
{ 0, 0, "MapWidget",
    "2""v"TWS_void_STR"x"widget_magic_STR"x"widget_magic_STR },
{ 0, 0, "UnMapWidget",
    "2""v"TWS_void_STR"x"widget_magic_STR },
{ 0, 0, "SetXYWidget",
    "0""v"TWS_void_STR"x"widget_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR },
{ 0, 0, "GetOwnerWidget",
    "0""x"msgport_magic_STR"x"widget_magic_STR },
{ 0, 0, "ExposeWidget",
    "0""v"TWS_void_STR"x"widget_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_dat_STR"W"TWS_byte_STR"W"TWS_hwfont_STR"W"TWS_hwattr_STR },
				    

{ 0, 0, "CreateGadget",
    "0""x"gadget_magic_STR"x"widget_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR"W"TWS_byte_STR"_"TWS_uldat_STR"_"TWS_uldat_STR"_"TWS_udat_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_dat_STR"_"TWS_dat_STR },

{ 0, 0, "CreateButtonGadget",
    "1""x"gadget_magic_STR"x"widget_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR"V"TWS_byte_STR"_"TWS_uldat_STR"_"TWS_udat_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_dat_STR"_"TWS_dat_STR },

{ 0, 0, "WriteTextsGadget",
    "2""v"TWS_void_STR"x"gadget_magic_STR"_"TWS_byte_STR"_"TWS_dat_STR"_"TWS_dat_STR"W"TWS_byte_STR"_"TWS_dat_STR"_"TWS_dat_STR },
{ 0, 0, "WriteHWFontsGadget",
    "2""v"TWS_void_STR"x"gadget_magic_STR"_"TWS_byte_STR"_"TWS_dat_STR"_"TWS_dat_STR"W"TWS_hwfont_STR"_"TWS_dat_STR"_"TWS_dat_STR },

									
{ 0, 0, "Create4MenuRow",
    "0""v"TWS_void_STR"x"window_magic_STR"_"TWS_udat_STR"_"TWS_byte_STR"_"TWS_uldat_STR"V"TWS_byte_STR },

{ 0, 0, "CreateWindow",
    "1""x"window_magic_STR"_"TWS_dat_STR"V"TWS_byte_STR"W"TWS_hwcol_STR"x"menu_magic_STR"_"TWS_hwcol_STR"_"TWS_uldat_STR"_"TWS_uldat_STR"_"TWS_uldat_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_dat_STR },
{ 0, 0, "Create4MenuWindow",
    "1""x"window_magic_STR"x"menu_magic_STR },
{ 0, 0, "WriteAsciiWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_ldat_STR"V"TWS_byte_STR },
{ 0, 0, "WriteStringWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_ldat_STR"V"TWS_byte_STR },
{ 0, 0, "WriteHWFontWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_ldat_STR"V"TWS_hwfont_STR },
{ 0, 0, "WriteHWAttrWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_ldat_STR"V"TWS_hwattr_STR },
{ 0, 0, "WriteRowWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_ldat_STR"V"TWS_byte_STR },

{ 0, 0, "GotoXYWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_ldat_STR"_"TWS_ldat_STR },
{ 0, 0, "SetColTextWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_hwcol_STR },
{ 0, 0, "SetColorsWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_udat_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR },
{ 0, 0, "ConfigureWindow",
    "2""v"TWS_void_STR"x"window_magic_STR"_"TWS_byte_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_dat_STR"_"TWS_dat_STR },
{ 0, 0, "ResizeWindow",
    "0""v"TWS_void_STR"x"window_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR },

{ 0, 0, "FindWidgetAtWidget",
    "2""x"widget_magic_STR"x"widget_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR },
{ 0, 0, "FocusSubWidget",
    "0""v"TWS_void_STR"x"widget_magic_STR },

{ 0, 0, "CreateGroup",
    "0""x"group_magic_STR },
{ 0, 0, "InsertGadgetGroup",
    "2""v"TWS_void_STR"x"group_magic_STR"x"gadget_magic_STR },
{ 0, 0, "RemoveGadgetGroup",
    "2""v"TWS_void_STR"x"group_magic_STR"x"gadget_magic_STR },

{ 0, 0, "GetSelectedGadgetGroup",
    "2""x"gadget_magic_STR"x"group_magic_STR },
{ 0, 0, "SetSelectedGadgetGroup",
    "2""v"TWS_void_STR"x"group_magic_STR"x"gadget_magic_STR },

{ 0, 0, "Create4MenuMenuItem",
    "0""x"menuitem_magic_STR"x"obj_magic_STR"x"window_magic_STR"_"TWS_byte_STR"_"TWS_dat_STR"V"TWS_byte_STR },
{ 0, 0, "Create4MenuCommonMenuItem",
    "1""_"TWS_uldat_STR"x"menu_magic_STR },

{ 0, 0, "CreateMenu",
    "0""x"menu_magic_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_hwcol_STR"_"TWS_byte_STR },
{ 0, 0, "SetInfoMenu",
    "2""v"TWS_void_STR"x"menu_magic_STR"_"TWS_byte_STR"_"TWS_ldat_STR"V"TWS_byte_STR"W"TWS_hwcol_STR },

{ 0, 0, "CreateMsgPort",
    "0""x"msgport_magic_STR"_"TWS_byte_STR"V"TWS_byte_STR"_"TWS_time_t_STR"_"TWS_frac_t_STR"_"TWS_byte_STR },
{ 0, 0, "FindMsgPort",
    "0""x"msgport_magic_STR"x"msgport_magic_STR"_"TWS_byte_STR"V"TWS_byte_STR },

{ 0, 0, "BgImageScreen",
    "2""v"TWS_void_STR"x"screen_magic_STR"_"TWS_dat_STR"_"TWS_dat_STR"V"TWS_hwattr_STR },

{ 0, 0, "PrevObj",
    "0""x"obj_magic_STR"x"obj_magic_STR },
{ 0, 0, "NextObj",
    "0""x"obj_magic_STR"x"obj_magic_STR },
{ 0, 0, "ParentObj",
    "0""x"obj_magic_STR"x"obj_magic_STR },

{ 0, 0, "G_PrevGadget",
    "0""x"gadget_magic_STR"x"gadget_magic_STR },
{ 0, 0, "G_NextGadget",
    "0""x"gadget_magic_STR"x"gadget_magic_STR },
{ 0, 0, "GroupGadget",
    "0""x"group_magic_STR"x"gadget_magic_STR },

{ 0, 0, "O_PrevWidget",
    "0""x"widget_magic_STR"x"widget_magic_STR },
{ 0, 0, "O_NextWidget",
    "0""x"widget_magic_STR"x"widget_magic_STR },
{ 0, 0, "OwnerWidget",
    "0""x"msgport_magic_STR"x"widget_magic_STR },

{ 0, 0, "FirstScreen",
    "0""x"screen_magic_STR },
{ 0, 0, "FirstWidget",
    "0""x"widget_magic_STR"x"widget_magic_STR },
{ 0, 0, "FirstMsgPort",
    "0""x"msgport_magic_STR },
{ 0, 0, "FirstMenu",
    "0""x"menu_magic_STR"x"msgport_magic_STR },
{ 0, 0, "FirstW",
    "0""x"widget_magic_STR"x"msgport_magic_STR },
{ 0, 0, "FirstGroup",
    "0""x"group_magic_STR"x"msgport_magic_STR },
{ 0, 0, "FirstMutex",
    "0""x"mutex_magic_STR"x"msgport_magic_STR },
{ 0, 0, "FirstMenuItem",
    "0""x"menuitem_magic_STR"x"menu_magic_STR },
{ 0, 0, "FirstGadget",
    "0""x"gadget_magic_STR"x"group_magic_STR },


{ 0, 0, "GetDisplayWidth",
    "0""_"TWS_dat_STR },
{ 0, 0, "GetDisplayHeight",
    "0""_"TWS_dat_STR },

{ 0, 0, "SendToMsgPort",
    "0""_"TWS_byte_STR"x"msgport_magic_STR"_"TWS_udat_STR"V"TWS_byte_STR },
{ 0, 0, "BlindSendToMsgPort",
    "0""v"TWS_void_STR"x"msgport_magic_STR"_"TWS_udat_STR"V"TWS_byte_STR },

{ 0, 0, "GetOwnerSelection",
    "0""x"obj_magic_STR },
{ 0, 0, "SetOwnerSelection",
    "0""v"TWS_void_STR"_"TWS_time_t_STR"_"TWS_frac_t_STR },
{ 0, 0, "RequestSelection",
    "0""v"TWS_void_STR"x"obj_magic_STR"_"TWS_uldat_STR },
{ 0, 0, "NotifySelection",
    "0""v"TWS_void_STR"x"obj_magic_STR"_"TWS_uldat_STR"_"TWS_uldat_STR"V"TWS_byte_STR"_"TWS_uldat_STR"V"TWS_byte_STR },

{ 0, 0, "SetServerUid",
    "0""_"TWS_byte_STR"_"TWS_uldat_STR"_"TWS_byte_STR },


