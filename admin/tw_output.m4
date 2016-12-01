m4_define([TW_OUTPUT], [[
  echo '#ifndef _TWAUTOCONF_H'      >  include/twautoconf.h
  echo '#define _TWAUTOCONF_H'      >> include/twautoconf.h
  echo                              >> include/twautoconf.h
  $SED -e 's/define *\([A-Z]\)/define TW_\1/g' \
       -e 's/undef *\([A-Z]\)/undef TW_\1/g'   \
       -e 's/ifdef *\([A-Z]\)/ifdef TW_\1/g'   \
       -e 's/ifndef *\([A-Z]\)/ifndef TW_\1/g' \
                 < include/config.h >> include/twautoconf.h
  echo                              >> include/twautoconf.h
  
  tw_cfg CONF__ALLOC "$enable__alloc"
  tw_cfg CONF__ASM   "$enable__asm"
  tw_cfg CONF_SOCKET "$enable_socket"
  tw_cfg CONF_SOCKET_GZ         "$enable_socket_gz"
  tw_cfg CONF_SOCKET_PTHREADS   "$enable_socket_pthreads"
  tw_cfg CONF_SOCKET_ALIEN      "$enable_socket_alien"
  tw_cfg CONF_SOCKET_EXTENSIONS "$enable_socket_extensions"
  tw_cfg CONF_WM           "$enable_wm"
  tw_cfg CONF_WM_RC        "$enable_wm_rc"
  tw_cfg CONF_WM_RC_SHMMAP "$enable_wm_rc_shmmap"
  tw_cfg CONF_WM_RC_SHRINK "$enable_wm_rc_shrink"
  tw_cfg CONF_TERM         "$enable_term"
  tw_cfg CONF_TERM_DEVPTS  "$enable_term_devpts"
  tw_cfg CONF_PRINTK       "$enable_printk"
  tw_cfg CONF_HW_TTY         "$enable_hw_tty"
  tw_cfg CONF_HW_TTY_LINUX   "$enable_hw_tty_linux"
  tw_cfg CONF_HW_TTY_LRAWKBD "$enable_hw_tty_lrawkbd"
  tw_cfg CONF_HW_TTY_TWTERM  "$enable_hw_tty_twterm"
  tw_cfg CONF_HW_TTY_TERMCAP "$enable_hw_tty_termcap"
  tw_cfg CONF_HW_X11         "$enable_hw_x11"
  tw_cfg CONF_HW_GFX         "$enable_hw_gfx"
  tw_cfg CONF_HW_TWIN        "$enable_hw_twin"
  tw_cfg CONF_HW_DISPLAY     "$enable_hw_display"
  tw_cfg CONF_HW_GGI         "$enable_hw_ggi"
  tw_cfg CONF_EXT               "$enable_ext"
  tw_cfg CONF_OPT_SHADOWS       "$enable_opt_shadows"
  tw_cfg CONF_OPT_BLINK         "$enable_opt_blink"
  tw_cfg CONF_OPT_CURSOR_ALWAYS "$enable_opt_cursor_always"
  tw_cfg CONF_OPT_MENU_HIDE     "$enable_opt_menu_hide"
  tw_cfg CONF_OPT_MENU_INFO     "$enable_opt_menu_info"
  tw_cfg CONF_OPT_MENU_RELAX    "$enable_opt_menu_relax"
  tw_cfg CONF_OPT_SCREEN_SCROLL "$enable_opt_screen_scroll"
  
  echo                              >> include/twautoconf.h
  echo '#include "twfixconfig.h" /* to clean CONF_* macros */'  >> include/twautoconf.h
  echo                              >> include/twautoconf.h
  echo '#endif /* _TWAUTOCONF_H */' >> include/twautoconf.h
  
  $MKDIR_P include/Tw 2>/dev/null
  
  $GREP -v ' CONF_' < include/twautoconf.h | $GREP -v '#include' | $SED -e 's/_TWAUTOCONF_H/_TW_AUTOCONF_H/g' > include/Tw/autoconf.h
]])
