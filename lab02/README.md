# Lab 2: Line Drawing Algorithms

**Course:** CSE444 – Computer Graphics & Animation Lab
**Topic:** Drawing a straight line on a pixel grid: DDA, Bresenham and Midpoint

## Why do we need a line drawing algorithm?

A screen is a grid of pixels, and pixels only have whole-number positions. A mathematical line from (x1, y1) to (x2, y2) passes *between* pixels most of the time. A line drawing algorithm decides **which pixels to light up** so the result looks like a straight, unbroken line.

A good algorithm should:

1. Produce a line with **no gaps**.
2. Pick the pixels **closest to the true line**.
3. Be **fast**, because a picture can contain thousands of lines.

All three algorithms here use the same trick to avoid gaps. They step **one pixel at a time along the longer direction** (the *driving axis*). For a gentle line (|slope| < 1) x changes every step. For a steep line (|slope| > 1) y changes every step.

## Files

| File | Algorithm |
|------|-----------|
| `dda.cpp` | DDA (Digital Differential Analyzer) |
| `bresenham.cpp` | Bresenham's line algorithm |
| `midpoint.cpp` | Midpoint line algorithm |

Each program draws the same six test lines: gentle, steep, negative slope, horizontal, vertical, and right-to-left. That way you can compare the algorithms side by side.

### Compile and run

```powershell
g++ dda.cpp -o dda.exe -lfreeglut -lopengl32 -lglu32
.\dda.exe

g++ bresenham.cpp -o bresenham.exe -lfreeglut -lopengl32 -lglu32
.\bresenham.exe

g++ midpoint.cpp -o midpoint.exe -lfreeglut -lopengl32 -lglu32
.\midpoint.exe
```

You can also open a `.cpp` file and press **`Ctrl+Shift+B`** (see [Lab 1, Step 9](../lab01/README.md#step-9-optional-build-and-run-with-one-shortcut)).

---

## 1. DDA (Digital Differential Analyzer)

### Idea

Along a straight line, x and y both change at a constant rate. Work out how much each one changes per step, then keep adding those amounts. Round to the nearest pixel each time.

### Steps

1. `dx = x2 - x1`, `dy = y2 - y1`
2. `steps = max(|dx|, |dy|)`
3. `xInc = dx / steps`, `yInc = dy / steps`. One of these is always ±1.
4. Start at `(x, y) = (x1, y1)`. Repeat `steps + 1` times:
   - plot `(round(x), round(y))`
   - `x += xInc`, `y += yInc`

### Example: (1, 1) to (8, 5)

`dx = 7`, `dy = 4`, `steps = 7`, `xInc = 1`, `yInc = 4/7 ≈ 0.571`

| Step | x | y (exact) | Pixel plotted |
|------|---|-----------|---------------|
| 0 | 1 | 1.000 | (1, 1) |
| 1 | 2 | 1.571 | (2, 2) |
| 2 | 3 | 2.143 | (3, 2) |
| 3 | 4 | 2.714 | (4, 3) |
| 4 | 5 | 3.286 | (5, 3) |
| 5 | 6 | 3.857 | (6, 4) |
| 6 | 7 | 4.429 | (7, 4) |
| 7 | 8 | 5.000 | (8, 5) |

### Pros and cons

| ✔ Good | ✘ Not so good |
|--------|---------------|
| Easiest to understand and code | Uses **floating-point** numbers, which are slower than integers on simple hardware |
| One formula handles every slope and direction | Needs a `round()` for every pixel |
| | Small errors add up because `yInc` is stored inexactly. On very long lines the pixels can drift away from the true line |

---

## 2. Bresenham's Line Algorithm

### Idea

Take a gentle line. At every step, x moves by exactly one, and y has only **two choices**: stay where it is, or move by one. Bresenham keeps a whole-number **decision parameter `p`**. Its sign tells which of the two pixels is closer to the true line. Updating `p` needs only integer additions, with no division, floats or rounding.

### Steps (gentle line, |dx| ≥ |dy|)

1. `dx = |x2 - x1|`, `dy = |y2 - y1|`
2. `p = 2·dy − dx`
3. Start at `(x1, y1)`. Repeat `dx + 1` times:
   - plot `(x, y)`, then move x by one
   - if `p < 0` → y stays the same, and `p = p + 2·dy`
   - if `p ≥ 0` → y moves by one, and `p = p + 2·dy − 2·dx`

For a **steep** line (|dy| > |dx|), swap the roles of x and y.

For lines going left or down, step by `-1` instead of `+1` (the `sx`, `sy` variables in the code).

### Where does `p` come from?

At column x the true line is at some height y_true. The two candidate pixels sit at distances `d1` (to the lower pixel) and `d2` (to the upper pixel). If `d1 − d2 < 0` the lower pixel is closer. Multiplying `d1 − d2` by `dx` removes the fraction, and the result is `p`. Only its **sign** matters, so scaling it doesn't change the decision.

### Example: (1, 1) to (8, 5)

`dx = 7`, `dy = 4`, `2·dy = 8`, `2·dy − 2·dx = −6`, starting `p = 2·4 − 7 = 1`

| Step | Pixel plotted | p | Decision | New p |
|------|---------------|---|----------|-------|
| 0 | (1, 1) | 1 | p ≥ 0 → y up | 1 − 6 = −5 |
| 1 | (2, 2) | −5 | p < 0 → y same | −5 + 8 = 3 |
| 2 | (3, 2) | 3 | p ≥ 0 → y up | 3 − 6 = −3 |
| 3 | (4, 3) | −3 | p < 0 → y same | −3 + 8 = 5 |
| 4 | (5, 3) | 5 | p ≥ 0 → y up | 5 − 6 = −1 |
| 5 | (6, 4) | −1 | p < 0 → y same | −1 + 8 = 7 |
| 6 | (7, 4) | 7 | p ≥ 0 → y up | 7 − 6 = 1 |
| 7 | (8, 5) | | end point | |

These are the same pixels as DDA, found using only integer arithmetic.

### Pros and cons

| ✔ Good | ✘ Not so good |
|--------|---------------|
| **Integer only**: addition, subtraction and comparison | Slightly harder to understand than DDA |
| No rounding, no drift: always picks the closest pixel | Needs separate code for the gentle and steep cases |
| Very fast; easy to build into hardware | |

---

## 3. Midpoint Line Algorithm

### Idea

Write the line as an equation:

```
F(x, y) = dy·x − dx·y + c
```

- `F = 0` → the point is **on** the line
- `F > 0` → the point is **below** the line
- `F < 0` → the point is **above** the line

After plotting pixel `(x, y)` on a gentle line, the next pixel is either
**E** = `(x+1, y)` (east) or **NE** = `(x+1, y+1)` (north-east).
Test the **midpoint** between them, `M = (x+1, y+½)`:

- `F(M) ≤ 0` → M is on or above the line → the line is closer to **E**
- `F(M) > 0` → M is below the line → the line is closer to **NE**

### Steps (gentle line, |dx| ≥ |dy|)

1. `dx = |x2 - x1|`, `dy = |y2 - y1|`
2. `d = 2·dy − dx`. This is `2·F(M)` at the first midpoint, doubled so it stays a whole number.
3. `dE = 2·dy`, `dNE = 2·(dy − dx)`
4. Start at `(x1, y1)`. Repeat `dx + 1` times:
   - plot `(x, y)`, then move x by one
   - if `d ≤ 0` → choose E (y same), and `d = d + dE`
   - if `d > 0` → choose NE (y moves), and `d = d + dNE`

For steep lines swap x and y. For other directions use `sx`, `sy`.

### Example: (1, 1) to (8, 5)

`d` starts at 1, `dE = 8`, `dNE = −6`. The numbers are exactly the same as in the Bresenham table, and the result is the same pixels.

### Midpoint vs. Bresenham: aren't they the same?

**For straight lines, yes, almost exactly.** Both start at `2·dy − dx` and add the same two constants. The only difference is a **tie**, when the true line passes exactly halfway between two pixels:

- Bresenham (`p ≥ 0` → move) picks the **upper** pixel.
- Midpoint (`d ≤ 0` → stay) picks the **lower** pixel.

Example: (0, 0) to (2, 1) gives `(0,0) (1,1) (2,1)` with Bresenham and `(0,0) (1,0) (2,1)` with Midpoint. Both answers are equally correct.

They differ in **how the method is explained**. Bresenham compares two distances. Midpoint plugs the midpoint into the line's equation. The midpoint view **generalises**: you can use the same method for any curve with an equation `F(x, y) = 0`. That is how the **Midpoint Circle** and **Midpoint Ellipse** algorithms work, which you will meet next.

### Pros and cons

| ✔ Good | ✘ Not so good |
|--------|---------------|
| Integer only, as fast as Bresenham | For lines, gives nothing extra over Bresenham |
| Has a clear geometric meaning (above or below the line) | Its derivation takes a little more algebra |
| Works for circles, ellipses and other curves too | |

---

## Comparison

| | DDA | Bresenham | Midpoint |
|---|---|---|---|
| Arithmetic | Floating point | Integer | Integer |
| Operations per pixel | 2 float adds + 2 rounds | 1 compare + 1–2 integer adds | 1 compare + 1–2 integer adds |
| Rounding needed | Yes | No | No |
| Accuracy | Good, but long lines can drift | Exact (closest pixel) | Exact (closest pixel) |
| Speed | Slower | Fast | Fast |
| Ease of understanding | Easiest | Medium | Medium |
| Extends to circles / ellipses | Not easily | Yes (Bresenham circle) | Yes (this is its main strength) |
| Time complexity | O(max(\|dx\|, \|dy\|)) | O(max(\|dx\|, \|dy\|)) | O(max(\|dx\|, \|dy\|)) |
| Extra memory | O(1) | O(1) | O(1) |

All three visit the same number of pixels, so they have the same **Big-O** time. What differs is the **cost of each step**: integer additions are cheaper than floating-point additions plus rounding.

## Which algorithm is best?

**Bresenham (or Midpoint, which is the same for lines) is the best general-purpose choice** for drawing lines on a pixel grid. It uses only integers, never drifts, and always picks the pixel closest to the true line.

**DDA is the best for learning**, because it follows directly from the slope of a line.

## When should you use each one?

| Use… | When… |
|------|-------|
| **DDA** | You are learning, or writing a quick prototype. Floating point is cheap on your hardware (on modern CPUs the speed gap is small). You need to **interpolate other values along the line** at the same time, such as colour, depth or texture coordinates. DDA-style stepping (add a fixed increment each pixel) is how GPUs interpolate these. |
| **Bresenham** | You need fast, exact lines with integers only: embedded systems, microcontrollers, LCD/LED displays, plotters, CNC machines, pixel-art tools, or grid-based games (line of sight, tile tracing). |
| **Midpoint** | You want one method to draw **lines, circles and ellipses**. Learn the midpoint line first, then apply the same idea to the Midpoint Circle algorithm. |

> **In real OpenGL programs** you would just call `glBegin(GL_LINES)`. The GPU draws the line in hardware. These algorithms show what happens inside that call, and they are still used wherever no graphics hardware is available.

---

## Lab Task

1. Run all three programs and check that they draw the same picture.
2. Trace DDA and Bresenham by hand for the line **(2, 3) → (12, 9)** and check that you get the same pixels.
3. Change the test lines in `display()` to draw a **star** or a **house** using only your own line function.
4. Draw the line (0, 0) → (2, 1) with Bresenham and Midpoint and write down the pixels. Why are they different, and which one is "right"?
