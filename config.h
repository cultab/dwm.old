#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 8;        /* gaps between windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminus:style=Regular:pixelsize=15:antialias=true:autohint=true", "monospace:size=10" };
static const char dmenufont[]       =   "Terminus:style=Regular:pixelsize=15:antialias=true:autohint=true";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_green[]       = "#c3e88d";
static const char col_bgrey[]       = "#292d3d";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_bgrey, col_gray2 },
	[SchemeSel]  = { col_bgrey, col_green, col_green },
};

/* tagging */
static const char *tags[] = { " Web:1", " Dev:2", " Dev:3", " Dev:4", " Chat:5", " Sys:6" };
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *tags[] = { "➊", "➋", "➌", "➍", "➎", "➏", "➐", "➑", "➒" };
//static const char *tags[] = { "", "", "", "", "", "", "❽", "", "" };

/* xprop(1):
 *	WM_CLASS(STRING) = instance, class
 *	WM_NAME(STRING) = title
 */
static const Rule rules[] = {
	/* class               instance    title       tags mask     isfloating   monitor */
	{ "Gimp",              NULL,       NULL,       0,            1,           -1 },
	{ "Vivaldi-Stable",    NULL,       NULL,       1 << 1,       0,           -1 },
	{ "discord",           NULL,       NULL,       1 << 4,       0,           -1 },
	{ "MEGAsync",          NULL,       NULL,       1 << 5,       1,           -1 },
	{ "Barrier",           NULL,       NULL,       1 << 5,       1,           -1 },
	{ "keepassxc",         NULL,       NULL,       1 << 5,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol arrange function */
	{ "[T]",    tile },    /* first entry is default */
	{ "[F]",    NULL },    /* no layout function means floating behavior */
	{ "[M]",    monocle },
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
static const char *dmenucmd[]     = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_green, "-sf", col_bgrey, NULL };
static const char *tmux_termcmd[] = { "st", "-e", "tmux" };
static const char *termcmd[]      = { "st", NULL };
//static const char *fffcmd[]      =  { "st", "-e", "fff" };
static const char *mutecmd[]      = { "amixer", "-q", "set", "Master", "toggle", NULL };
static const char *volupcmd[]     = { "amixer", "-q", "set", "Master", "10%+", "unmute", NULL };
static const char *voldowncmd[]   = { "amixer", "-q", "set", "Master", "10%-", "unmute", NULL };
static const char *miccmd[]       = { "amixer", "set", "Capture", "toggle", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
	/* modifier                     key                      function        argument */
	{ MODKEY,                       XK_r,                    spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,               spawn,          {.v = tmux_termcmd } },
	{ MODKEY|ControlMask,           XK_Return,               spawn,          {.v = termcmd } },
    { 0,                            XF86XK_AudioMute,        spawn,          {.v = mutecmd } },
    { 0,                            XF86XK_AudioLowerVolume, spawn,          {.v = voldowncmd } },
    { 0,                            XF86XK_AudioRaiseVolume, spawn,          {.v = volupcmd } },
    { 0,                            XF86XK_AudioMicMute,     spawn,          {.v = miccmd } },
	{ MODKEY,                       XK_grave,                togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,                    togglebar,      {0} },
	{ MODKEY,                       XK_j,                    focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                    focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                    incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                    incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                    setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,                    setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,                    setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,                    setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return,               zoom,           {0} },
	{ MODKEY,                       XK_Tab,                  view,           {0} },
	{ MODKEY|ShiftMask,             XK_d,                    killclient,     {0} },
	{ MODKEY,                       XK_t,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                    setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                togglefloating, {0} },
	{ MODKEY,                       XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                    tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,               focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,               tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                                    0)
	TAGKEYS(                        XK_2,                                    1)
	TAGKEYS(                        XK_3,                                    2)
	TAGKEYS(                        XK_4,                                    3)
	TAGKEYS(                        XK_5,                                    4)
	TAGKEYS(                        XK_6,                                    5)
	TAGKEYS(                        XK_7,                                    6)
	TAGKEYS(                        XK_8,                                    7)
	TAGKEYS(                        XK_9,                                    8)
	{ MODKEY|ShiftMask,             XK_q,                    quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,                    quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

