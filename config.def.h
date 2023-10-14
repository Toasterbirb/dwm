/* See LICENSE file for copyright and license details. */

/* include the colorscheme */
#include "theme.h"

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
static const int  dmenu_lines     = 7;
static const int  dmenu_width     = 512;


/** tagging **/
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
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
static char dmenu_x_offset_str[5] = "0";
static char dmenu_y_offset_str[5] = "0";
static char dmenu_lines_str[5] = "0";
static char dmenu_width_str[5] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-hp", "firefox", "-m", dmenumon, "-fn", dmenufont, "-x", dmenu_x_offset_str, "-y", dmenu_y_offset_str, "-z", dmenu_width_str, "-l", dmenu_lines_str, "-nb", dmenu_background, "-nf", dmenu_text, "-sb", dmenu_active_background, "-sf", dmenu_active_text, NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier             key     function        argument */
	{ MODKEY,               40,     spawn,          {.v = dmenucmd } }, // d
	{ MODKEY,               36,     spawn,          {.v = termcmd } }, // Return
	{ MODKEY|ShiftMask,     56,     togglebar,      {0} },          // b
	{ MODKEY,               44,     focusstack,     {.i = +1 } },   // j
	{ MODKEY,               45,     focusstack,     {.i = -1 } },   // k
	{ MODKEY|ControlMask,   31,     incnmaster,     {.i = +1 } },   // i
	{ MODKEY|ControlMask,   55,     incnmaster,     {.i = -1 } },   // v
	{ MODKEY,               43,     setmfact,       {.f = -0.05} }, // h
	{ MODKEY,               46,     setmfact,       {.f = +0.05} }, // l
	{ MODKEY|ShiftMask,     36,     zoom,           {0} },          // Return
	{ MODKEY,               23,     view,           {0} },          // Tab
	{ MODKEY|ShiftMask,     24,     killclient,     {0} },          // q
	{ MODKEY,               28,     setlayout,      {.v = &layouts[0]} }, // t
	//{ MODKEY,               41,     setlayout,      {.v = &layouts[1]} }, // f
	{ MODKEY,               58,     setlayout,      {.v = &layouts[2]} }, // m
	{ MODKEY,               65,     setlayout,      {0} },          // space
	{ MODKEY|ShiftMask,     65,     togglefloating, {0} },          // space
	{ MODKEY,               41,     togglefullscr,  {0} },          // f
	{ MODKEY,               19,     view,           {.ui = ~0 } },  // 0
	{ MODKEY|ShiftMask,     19,     tag,            {.ui = ~0 } },  // 0
	{ MODKEY,               59,     focusmon,       {.i = +1 } },   // comma
	{ MODKEY,               60,     focusmon,       {.i = -1 } },   // period
	{ MODKEY|ShiftMask,     59,     tagmon,         {.i = +1 } },   // comma
	{ MODKEY|ShiftMask,     60,     tagmon,         {.i = -1 } },   // period
	{ MODKEY|Mod4Mask,      19,     togglegaps,     {0} },          // 0
	TAGKEYS(                10,                     0)              // 1
	TAGKEYS(                11,                     1)              // 2
	TAGKEYS(                12,                     2)              // 3
	TAGKEYS(                13,                     3)              // 4
	TAGKEYS(                14,                     4)              // 5
	TAGKEYS(                15,                     5)              // 6
	TAGKEYS(                16,                     6)              // 7
	TAGKEYS(                17,                     7)              // 8
	TAGKEYS(                18,                     8)              // 9
	{ MODKEY|ShiftMask,     26,     quit,           {0} },          // e
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
