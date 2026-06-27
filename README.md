# Terminal Text Editor

A Windows-native terminal text editor written in C, built
on top of the Windows Console API. Operates in raw console
mode with no external dependencies.

## Features

- Gap buffer data structure for efficient in-place text
  editing at the cursor position
- Diff-based rendering — only characters from the cursor
  forward are redrawn on each keystroke, minimizing
  unnecessary screen writes
- Raw console input via Windows Console API, bypassing
  line-buffered input entirely
- VT100 escape sequence output for cursor control and
  screen clearing
- Keybind system using function pointers for both virtual
  keys (backspace, enter, tab, arrow keys) and control
  key combinations (Ctrl+Q to quit)
- Automatic line wrapping when cursor reaches window edge
  with correct reverse wrap on backspace

## How It Works

**Gap buffer** — text is stored in a single flat array with
a moveable gap at the cursor position. Inserting a character
writes into the gap and shrinks it; deleting expands it.
Moving the cursor shifts the gap through the array. This
makes insertions and deletions at the cursor O(1) without
allocating new memory on every keystroke.

**Input handling** — the Windows console is set to raw mode
on startup by disabling `ENABLE_LINE_INPUT`,
`ENABLE_ECHO_INPUT`, and `ENABLE_PROCESSED_INPUT`. Every
keypress is read immediately via `ReadConsoleInput` and
dispatched through function pointer tables for virtual keys
and control keys.

**Rendering** — on each keystroke the cursor is moved back
one position, everything from the cursor to end of screen
is erased with `\033[J`, and only the text from the current
gap position forward is rewritten. This avoids redrawing
the entire buffer on every edit.

## Building

Requires GCC and Windows (uses Windows Console API).

```bash
gcc -o TextEditor main.c DynamicArray.c InputHandler.c \
    Terminal.c TextRenderer.c ProgramControl.c
```

## Controls

| Key        | Action                  |
|------------|-------------------------|
| Type       | Insert character        |
| Backspace  | Delete character        |
| Enter      | New line                |
| Tab        | Insert tab              |
| Arrow keys | Move cursor             |
| Ctrl+Q     | Quit                    |

## Project Status

Active development. Core editing and rendering are working.
Autowrap behaviour and multi-line editing are in progress
on a separate branch.

## File Structure

```
main.c            Entry point, setup and input loop
DynamicArray.c/h  Gap buffer implementation
TextRenderer.c/h  Rendering, cursor management
InputHandler.c/h  Raw input, keybind dispatch
Terminal.c/h      Console mode setup, window info
ProgramControl.c/h  Application lifecycle
```