# dwm Spatial Canvas — v7 Changelog

## Bugs fixed from v6

### Bug 1 — Viewport contamination between workspaces (ROOT CAUSE)
**Symptom:** Panning on tag 1 would move windows on tag 2. Switching back showed
the contaminated position.

**Root cause:** `vx/vy` was a single pair on the monitor struct. Panning tag 1
wrote the same `vx/vy` that tag 2 would read.

**Fix:** Added `tagvx[32]` / `tagvy[32]` arrays to `Monitor`. Each tag stores its
own viewport independently. `view()` saves the departing tag's viewport and
restores the arriving tag's. `panmouse()` writes to both `vx/vy` (active cache)
and `tagvx/tagvy` (persistent store).

---

### Bug 2 — Cannot move windows with Mod4+drag
**Symptom:** Dragging a window would not move it, or would stop it at the screen edge.

**Root cause:** `movemouse()` had hard clamps:
```c
if (nx < selmon->wx)  nx = selmon->wx;           // ← prevented any movement
if (nx + WIDTH > wx + ww)  nx = wx + ww - WIDTH; // ← snapped to edge
```
These were added trying to prevent cross-workspace drift, but the real fix
is per-tag viewports (Bug 1). The clamps are now fully removed.

---

### Bug 3 — Tiled windows cannot be dragged at all
**Symptom:** Mod4+drag on a tiled window did nothing visible (went into tile-swap
branch).

**Root cause:** `movemouse()` had two branches: float-drag and tile-swap.
Tiled windows entered the swap branch which swapped window contents
instead of moving the window.

**Fix:** `movemouse()` now auto-floats any tiled window before grabbing the
pointer. The tile-swap branch is removed entirely — on a spatial canvas,
dragging always means "reposition on the canvas."

---

### Bug 4 — Tag switch was instant / jarring
**Symptom:** Switching tags had no animation; windows snapped.

**Fix:** Added `animatepan()` — quadratic ease-out over 14 frames × 12ms
(~168ms total). `view()` now:
1. Saves old viewport
2. Loads new viewport + positions windows at final coords (no flicker)
3. Slides the camera from old → new via `animatepan()`
4. Finalises stacking with `restack()` + `showhide()`

Animation only runs when the viewport actually changes (switching to a tag
with a different saved pan position). Same-position switches are instant.

---

## How to apply

From your dwm source directory (containing the v6 `dwm.c`):

```bash
patch -p1 < dwm-canvas-v7.patch
make && sudo make install
```

Or replace `dwm.c` directly with the included pre-patched file.

## Summary of new fields

| Field | Location | Purpose |
|---|---|---|
| `tagvx[32]` | `Monitor` | Per-tag saved canvas X offset |
| `tagvy[32]` | `Monitor` | Per-tag saved canvas Y offset |
| `vx`, `vy` | `Monitor` (existing) | Active viewport cache (always = tagvx/tagvy of current tag) |

## New functions

| Function | Purpose |
|---|---|
| `seltagidx(m)` | Returns 0-based index of active tag — indexes tagvx/tagvy |
| `animatepan(m, from, to)` | Quadratic ease-out slide between two viewport positions |
