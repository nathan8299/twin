



dnl This file is `m4/proto.m4' : m4 macros to autogenerate proto_m4.h
dnl Tell the user about this.
/* This file was automatically generated with `m4' from `m4/proto.m4', do not edit! */

/*
 *  proto_m4.h  --  check libTw functions prototypes from Tw_sockproto.m4h
 *		    against twin.h or against Tw/Tw.h include files.
 *
 *  Copyright (C) 2001,2002 by Massimiliano Ghilardi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 */

divert(-1)

define(`TRIM', `translit(`$1', ` ')')

define(`CHAIN', `TRIM($1)`'TRIM($2)')

define(`SELF', `$1`'')

define(`NAME', `SELF(Tw_`'CHAIN($1,$2))')

define(`NSHIFT', `ifelse($1, 0, `shift($@)', `NSHIFT(decr($1), shift(shift($@)))')')

define(`PREFIX_ANY', `tdisplay TwD`'ifelse($1, 5, `', `, ')')

define(`tv', v)
define(`t_', _)
define(`tx', x)
define(`tV', V)
define(`tW', W)
define(`tX', X)
define(`tY', Y)

define(`Tv', `void ')
define(`T_', `$1 ')
define(`Tx', `t$1 ')
define(`TV', `TW_CONST $1 *')
define(`TW', `TW_CONST $1 *')
define(`TX', `TW_CONST t$1 *')
define(`TY', `TW_CONST t$1 *')

define(`TYPE', `T$2($1)')

define(`A', `a$1')

define(`ARG', `ifelse($3, v, `', `TYPE($2, $3)`'$4')')

define(`_ARGS', `ifelse(eval($# < 3), 1, `', `, ARG($1,$2,t$3,$4)`'_ARGS(incr($1), NSHIFT(4, $@))')')
define(`ARGS', `ifelse(eval($# < 3), 1, `', `ARG($1,$2,t$3,$4)`'ifelse($#, 4, `', `_ARGS(incr($1), NSHIFT(4, $@))')')')


define(`PROTO', `TYPE($1,$2) NAME($3, $4)(PREFIX_ANY($#)`'ARGS(1, NSHIFT(5, $@)));')
define(`PROTOFindFunction', defn(`PROTO'))
define(`PROTOSyncSocket', defn(`PROTO'))

define(`Tw_ChangeFieldObj', `Tw_ChangeField')

define(`c_doxygen', `
$@')`'dnl

divert

include(`m4/Tw_sockproto.m4h')

