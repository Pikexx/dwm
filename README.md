# dwm Spatial Canvas Patch

Replaces dwm's bitmask-based tag visibility with an **Infinite Viewport / Spatial Canvas** system.

## Concept

The 9 standard dwm tags become zones on a **3×3 spatial grid**:

```
Tag1 (0,0)    Tag2 (mw,0)    Tag3 (2mw,0)
Tag4 (0,mh)   Tag5 (mw,mh)   Tag6 (2mw,mh)
Tag7 (0,2mh)  Tag8 (mw,2mh)  Tag9 (2mw,2mh)
```

Switching to a tag **moves the viewport camera** to that zone.
Every window lives at a permanent **canvas coordinate** (`gx`, `gy`),
and its screen position is always: `screen_x = gx - vx`, `screen_y = gy - vy`.

## What Changed

### `struct Client`
- Added `int gx, gy` — absolute position on the infinite canvas.

### `struct Monitor`
- Added `int vx, vy` — the viewport/camera offset on the canvas.

### `resizeclient()`
- Now stores `gx = x + vx`, `gy = y + vy` for every resize.
- Actual X11 position is `gx - vx` / `gy - vy` (equals `x`/`y` within same viewport).

### `view()`
- Calls `tagtocanvas()` to compute the new `(vx, vy)` for the selected tag.
- Sets `selmon->vx` / `selmon->vy` before calling `arrange()`.

### `tagtocanvas()` *(new function)*
- Maps a tag bitmask to a grid cell: tag index `i` → column `i%3`, row `i/3`.
- `vx = col * mw`, `vy = row * mh`.

### `focus()`
- **Auto-panning**: if the newly focused window's `gx`/`gy` is outside the current
  viewport rect, the camera slides to center on that window.

### `showhide()`
- Off-viewport (invisible) windows are **kept mapped** (not unmapped).
- They are slid to their canvas-relative screen position (`gx-vx`, `gy-vy`),
  which places them physically off-screen — no X unmapping overhead.

### `window_map()`
- Uses `gx - vx`, `gy - vy` for the `XMoveResizeWindow` call.

### `configure()`
- Reports `gx - vx`, `gy - vy` to the client as its logical screen position.

### `manage()`
- Initialises `gx = x + vx`, `gy = y + vy` when a new window is managed.

### `movemouse()`
- Syncs `gx`/`gy` after dragging a floating window.

### `setfullscreen()`
- Restores position from canvas coords on un-fullscreen.

## Applying the Patch

```bash
# From your dwm source directory:
patch -p0 < dwm-spatial-canvas.patch
make
sudo make install
```

Or simply copy `dwm.c` (already patched) into your dwm source tree.

## Key Bindings (unchanged)
`Mod+1`…`Mod+9` — navigate to tag zone on canvas  
`Mod+Shift+1`…`Mod+Shift+9` — send focused window to a tag/zone

