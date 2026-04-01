# dwm Free-Pan Spatial Canvas Patch

One infinite canvas. Pan freely with **Mod4 + left-click drag** on the desktop background.
No tag grids. No auto-pan. No zones. Just a camera you move wherever you want.

## How it works

Every window has a permanent **canvas position** (`gx`, `gy`) — its address on the
infinite canvas. The monitor has a **camera offset** (`vx`, `vy`).

```
actual_screen_x = gx - vx
actual_screen_y = gy - vy
```

Panning just changes `vx`/`vy`. All windows reposition instantly.
Off-screen windows stay **mapped in X** (no unmap/remap overhead) — they just slide
off the physical screen naturally.

## Controls

| Action | Binding |
|---|---|
| **Pan canvas** | `Mod4 + left-click drag` on desktop background |
| Move a floating window | `Mod4 + left-click drag` on the window |
| All other bindings | unchanged |

## Applying the patch

From inside your dwm source directory:

```bash
patch -p1 < dwm-freepan-canvas.patch
make && sudo make install
```

Or drop in the pre-patched `dwm.c` and `config.h` directly.

## What changed

| File | What |
|---|---|
| `dwm.c` | `Client` gets `int gx, gy`; `Monitor` gets `int vx, vy` |
| `dwm.c` | `resizeclient()` stores `gx/gy`, computes screen pos as `gx-vx / gy-vy` |
| `dwm.c` | `showhide()` keeps all windows mapped, slides off-screen ones to canvas pos |
| `dwm.c` | `configure()` reports `gx-vx / gy-vy` as logical position to clients |
| `dwm.c` | `manage()` inits `gx/gy` from spawn position + current viewport |
| `dwm.c` | `movemouse()` syncs `gx/gy` after dragging a window |
| `dwm.c` | `panmouse()` — **new**: Mod4+LMB drag pans `vx/vy`, calls `arrange()` at 60fps |
| `dwm.c` | `window_unmap()` removed — no longer needed |
| `config.h` | `ClkRootWin + MODKEY + Button1 → panmouse` added to buttons[] |
