/* See LICENSE file for copyright and license details. */

/* behaviour */
static int focusfollowmouse  = 1; /* 1 = focus window under cursor on hover */
static int mousefollowsfocus = 1; /* 1 = warp cursor to focused window on keyboard focus change */
static int warpontagswitch   = 0; /* 1 = warp cursor to focused window when switching tags (off by default) */
/* cursor-on-close: set wmisclosing=1 in unmanage() to disable — off by default */

/* appearance */
static const char *screenshotcmd[] = { "/home/dearv/keybindfile/shfiles/screnshot.sh", NULL };
static const char *dminu[] = { "/home/dearv/keybindfile/shfiles/dmenu.sh", NULL };
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 13;        /* gaps between windows */
static const unsigned int snap      = 0;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 10;        /* 2 is the default spacing around the bar's font */
static const int vertpad            = 10;       /* vertical padding of bar */
static const int sidepad            = 10;       /* horizontal padding of bar */
static const char *fonts[] = { 
    "JetBrainsMono NF:style=ExtraBold:size=10:antialias=true:autohint=true",
    "JoyPixels:size=8:antialias=true:autohint=true" 
};
static char normbgcolor[]           = "#1c1815";
static char normbordercolor[]       = "#1d2021";
static char normfgcolor[]           = "#54453a";
static char selfgcolor[]            = "#e6dfd5";
static char selbordercolor[]        = "#ebdbb2";
static char selbgcolor[]            = "#77654c";
static char titlebgcolor[]          = "#77654c";
static char titlefgcolor[]          = "#29231f";

/* COZY SETTINGS: 0xe0 is thicker and 'frostier' than 0xd0 */
static const unsigned int baralpha    = 0xe0; 
static const unsigned int borderalpha = OPAQUE;

static char *colors[][3] = {
       /* fg           bg           border   */
       [SchemeNorm]  = { normfgcolor,  normbgcolor, normbordercolor },
       [SchemeSel]   = { selfgcolor,   selbgcolor,  selbordercolor  },
       [SchemeTitle] = { titlefgcolor, titlebgcolor, normbordercolor },
};

static const unsigned int alphas[][3] = {
       /* fg      bg        border      */
       [SchemeNorm]  = { OPAQUE, OPAQUE, OPAQUE },
       [SchemeSel]   = { OPAQUE, OPAQUE, OPAQUE },
       [SchemeTitle] = { OPAQUE, OPAQUE, OPAQUE },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                 instance  title            tags mask  isfloating  isterminal  noswallow  monitor */
	{ "St",                  NULL,     NULL,            0,          0,           1,            0,         -1 },
	{ NULL,                  NULL,     "Event Tester", 0,          0,           0,            1,         -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󰽙",      tile },    /* first entry is default */
	{ "󰉨",      NULL },    /* no layout function means floating behavior */
	{ "󰍉",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                        KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,            KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,              KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *roficmd[]   = { "rofi", "-show", "drun", NULL };
static const char *termcmd[]   = { "st", NULL };
static const char *flamecmd[]  = { "flameshot", "gui", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
        { 0,                            XK_Print,  spawn,          {.v = screenshotcmd } },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_e,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, 
	{ MODKEY,                       XK_t,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_r,      togglefloating,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  smarttogglefloating, {0} },
	/* -- focus / resize -- */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_f,      zoom,           {0} },
	/* -- nmaster -- */
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_s,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } }, 
	/* -- gaps -- */
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i =  0 } },
	/* -- bar -- */
	{ MODKEY,                       XK_g,      togglebar,      {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} }, 
	/* -- spawn -- */
	{ ControlMask,                  XK_space,  spawn,          {.v = dminu } }, 
	{ MODKEY,                       XK_space,  spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = flamecmd } },
	/* -- misc -- */
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,      xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	/* -- behaviour toggles: uncomment and assign keys you want -- */
	/* { MODKEY,                    XK_F1,     togglefocusfollowmouse,  {0} }, */
	/* { MODKEY,                    XK_F2,     togglemousefollowsfocus, {0} }, */
};

/* button definitions */
static Button buttons[] = {
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	/* canvas: Mod4+LMB on root pans the viewport */
	{ ClkRootWin,            MODKEY,         Button1,        panmouse,       {0} },
	/* Mod4+RMB on root resizes the keyboard-focused window — use this when
	 * a floating window overlaps the one you want to resize: focus it with
	 * Mod+j/k first, then Mod+rightclick anywhere on the background. */
	{ ClkRootWin,            MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,          MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,          MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,          MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            0,              Button1,        tag,            {0} },
	{ ClkTagBar,            0,              Button3,        toggletag,      {0} },
};