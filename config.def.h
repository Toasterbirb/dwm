/* See LICENSE file for copyright and license details. */

/* include the colorscheme */
#ifdef SYSTEM_CONFIG
#include "/etc/dwm/theme.h"
#else
#include "theme.h"
#endif

#include "keycodes.h"

/* appearance */
static const unsigned int borderpx    = 2;        /* border pixel of windows */
static const unsigned int snap        = 32;       /* snap pixel */
static const int showbar              = 1;        /* 0 means no bar */
static const int topbar               = 1;        /* 0 means bottom bar */
static const int vertpad              = 5;       /* vertical padding of bar */
static const int sidepad              = 10;       /* horizontal padding of bar */
static const int user_bh              = 28;       /* 0 means that dwm will calculate bar height,
									  			   >= 1 means dwm will user_bh as bar height */
static const char *fonts[]            = { "mononoki:size=13" };
static const unsigned int gappih      = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv      = 10;       /* vert inner gap between windows */
static const unsigned int gappoh      = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov      = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps            = 0;        /* 1 means no outer gap when there is only one window */
static const int tag_indicator_height = 2;

static const int enable3dbar          = 1;
static const int shadow_height        = 4;
static const int bar_text_offset      = -1;      /* height offset for the bar text,
													 useful if the text isn't centered when using 3dbar */

/** dmenu **/
static const int  dmenu_centered  = 1;   /* If this is set to false, dmenu will position itself
                                            on top of the status bar */
static const int  dmenu_font_size = 15;
static const char dmenufont[]     = "mononoki:size=18";

/* centered dmenu settings */
static const int  dmenu_x_offset  = 10;
static const int  dmenu_y_offset  = -64;
static const int  dmenu_lines     = 4;
static const int  dmenu_width     = 512;


/** tagging **/
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */
	{ "Tor Browser",  	NULL,       NULL,       0,       	  1,           -1 },
	{ "steam",  		NULL,       NULL,       1 << 4,    	  0,            0 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
#define DMENU_STR_SIZE 12
static char dmenu_x_offset_str[DMENU_STR_SIZE] = "0";
static char dmenu_y_offset_str[DMENU_STR_SIZE] = "0";
static char dmenu_lines_str[DMENU_STR_SIZE] = "0";
static char dmenu_width_str[DMENU_STR_SIZE] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-hp", "firefox", "-m", dmenumon, "-fn", dmenufont, "-x", dmenu_x_offset_str, "-y", dmenu_y_offset_str, "-z", dmenu_width_str, "-l", dmenu_lines_str, "-nb", dmenu_background, "-nf", dmenu_text, "-sb", dmenu_active_background, "-sf", dmenu_active_text, NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier             key           function        argument */
	{ MODKEY,               KEY_D,        spawn,          {.v = dmenucmd } },
	{ MODKEY,               KEY_RETURN,   spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,     KEY_B,        togglebar,      {0} },
	{ MODKEY|ShiftMask,     KEY_J,        movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,     KEY_K,        movestack,      {.i = -1 } },
	{ MODKEY|ControlMask|ShiftMask,     KEY_J,        rotatestack,    {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask,     KEY_K,        rotatestack,    {.i = -1 } },
	{ MODKEY,               KEY_J,        focusstack,     {.i = +1 } },
	{ MODKEY,               KEY_K,        focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,   KEY_I,        incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,   KEY_V,        incnmaster,     {.i = -1 } },
	{ MODKEY,               KEY_H,        setmfact,       {.f = -0.05} },
	{ MODKEY,               KEY_L,        setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,     KEY_RETURN,   zoom,           {0} },
	{ MODKEY,               KEY_TAB,      view,           {0} },
	{ MODKEY|ShiftMask,     KEY_Q,        killclient,     {0} },
	{ MODKEY,               KEY_T,        setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,               KEY_F,     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,               KEY_M,        setlayout,      {.v = &layouts[2]} },
	{ MODKEY,               KEY_SPACE,    setlayout,      {0} },
	{ MODKEY,               KEY_U,        setlayout,      {.v = &layouts[3]} },
	{ MODKEY,               KEY_O,        setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,     KEY_SPACE,    togglefloating, {0} },
	{ MODKEY,               KEY_F,        togglefullscr,  {0} },
	{ MODKEY,               KEY_0,        view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,     KEY_0,        tag,            {.ui = ~0 } },
	{ MODKEY,               KEY_COMMA,    focusmon,       {.i = +1 } },
	{ MODKEY,               KEY_PERIOD,   focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,     KEY_COMMA,    tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,     KEY_PERIOD,   tagmon,         {.i = -1 } },
	//{ MODKEY|Mod4Mask,      KEY_0,        togglegaps,     {0} },
	TAGKEYS(                KEY_1,                     0)
	TAGKEYS(                KEY_2,                     1)
	TAGKEYS(                KEY_3,                     2)
	TAGKEYS(                KEY_4,                     3)
	TAGKEYS(                KEY_5,                     4)
	TAGKEYS(                KEY_6,                     5)
	TAGKEYS(                KEY_7,                     6)
	TAGKEYS(                KEY_8,                     7)
	TAGKEYS(                KEY_9,                     8)
	{ MODKEY|ShiftMask,     KEY_E,        quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* Color mappings
 * You can modify the colors in theme.h */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { "#000000", "#000000", border_inactive },
	[SchemeSel]  = { "#000000", "#000000",  border_active  },
	[SchemeStatus]  = { status_text, status_background,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeStatusShadow]  = { status_3d_shadow, "#000000",  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { tag_active_text, tag_active_background,  "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  = { tag_inactive_text, tag_inactive_background,  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeTagsShadowSel]  = { tag_active_3d_shadow, "#000000",  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeTagsShadowNorm]  = { tag_inactive_3d_shadow, "#000000",  "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  = { info_active_text, info_active_background,  "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  = { info_inactive_text, info_inactive_background,  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
	[SchemeInfoShadowSel]  = { info_active_3d_shadow, "#000000",  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
	[SchemeInfoShadowNorm]  = { info_inactive_3d_shadow, "#000000",  "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
	[SchemeTagsIndicatorNorm]  = { tag_inactive_indicator, "#000000",  "#000000"  },
	[SchemeTagsIndicatorSel]  = { tag_active_indicator, "#000000",  "#000000"  },
};
