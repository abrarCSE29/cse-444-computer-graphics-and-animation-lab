# Lab 1: OpenGL Environment Setup

**Course:** CSE444 – Computer Graphics & Animation Lab
**Topic:** OpenGL platform setup (installation, library files and a first program)

## Objective

By the end of this lab you will have a working C++ and OpenGL (freeglut) setup on Windows using MSYS2 and VS Code. You will also compile and run a first program that draws a single red point.

## Requirements

- Windows 10/11 (64-bit)
- Internet connection
- About 1 GB of free disk space

---

## Step 0: Check what is already installed

Check whether MSYS2 and VS Code are already on your machine:

- **MSYS2:** look for the folder `C:\msys64`, or search for "MSYS2 UCRT64" in the Start menu.
- **VS Code:** search for "Visual Studio Code" in the Start menu, or run `code --version` in a terminal.

Skip any installation step for software you already have.

---

## Step 1: Install MSYS2 and the compiler

### 1.0 Install MSYS2

If MSYS2 is not installed, download the installer (`msys2-x86_64-*.exe`) from <https://www.msys2.org/> and install it with the default location (`C:\msys64`).

### 1.1 Update MSYS2

1. Open **MSYS2 UCRT64** from the Start menu.
2. Run:
   ```bash
   pacman -Syu
   ```
3. If it asks to install dependencies, press `Y` and then `Enter`.
4. When it asks to close the window, press `Y` and then `Enter`.
5. Reopen **MSYS2 UCRT64** and run `pacman -Syu` again to finish the update.

### 1.2 Install GCC and freeglut (the OpenGL libraries)

In the **MSYS2 UCRT64** terminal, run:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut
```

Press `Enter` and `Y` to install the requested dependencies. This installs the g++ compiler and the main OpenGL/GLUT libraries.

---

## Step 2: Add the compiler to PATH

1. Click **Search** and open **Edit the system environment variables**.
2. Click **Environment Variables...**.
3. Under your variables, select **Path** and click **Edit**.
4. Click **New** (or **Browse...**) and add:
   ```
   C:\msys64\ucrt64\bin
   ```
5. Click **OK** on every window to save.

### 2.2 Verify

Open a **new** Command Prompt and run:

```bash
g++ --version
```

If it prints a version number, everything is fine so far. If not, **ask for help** before continuing.

---

## Step 3: Install VS Code

1. If VS Code is not installed, download it from <https://code.visualstudio.com/download>. Choose **User Installer – 64 bit**.
2. Open VS Code, go to **Extensions** (`Ctrl+Shift+X`) and install the **C/C++ Extension Pack** (by Microsoft).

> If VS Code was already open while you edited PATH, close and reopen it so it picks up the new PATH.

---

## Step 4: Create the lab folder

Create a folder named `cg-lab-W[id]` (replace `[id]` with your ID) and open it in VS Code with **File → Open Folder**. All lab work will continue in this folder.

---

## Step 5: Write the first program

Create a file named `main.cpp` in the folder and paste this code:

```cpp
#include <GL/glut.h>

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);         // (0,0) bottom-left, (500,500) top-right
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);           // red
    glPointSize(10.0);                  // point size in pixels

    glBegin(GL_POINTS);
        glVertex2i(250, 250);           // point at the centre
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Point in OpenGL");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
```

### What the code does

| Part | Purpose |
|------|---------|
| `init()` | Sets a white background and a 2D coordinate system from (0,0) at the bottom-left to (500,500) at the top-right. |
| `display()` | Clears the screen and draws one red point, 10 px in size, at the centre (250, 250). |
| `main()` | Initialises GLUT, creates a 500×500 window titled "Point in OpenGL", registers `display` and starts the event loop. |

---

## Step 6: Open the terminal

Open the VS Code terminal with **Terminal → New Terminal** (`` Ctrl+` ``).

The first time PowerShell opens, you may see an execution policy error. Fix it with:

```powershell
Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy RemoteSigned
```

---

## Step 7: Compile

```powershell
g++ main.cpp -o main.exe -lfreeglut -lopengl32 -lglu32
```

This creates the executable `main.exe`.

| Flag | Links |
|------|-------|
| `-lfreeglut` | freeglut (windowing and input) |
| `-lopengl32` | core OpenGL |
| `-lglu32` | GLU utilities (e.g. `gluOrtho2D`) |

---

## Step 8: Run

```powershell
.\main.exe
```

A window should open with a red point in the centre. **Setup is complete!**

---

## Step 9 (optional): Build and run with one shortcut

To avoid typing the commands every time:

1. Create a folder named `.vscode` in the root of your lab folder.
2. Inside it, create `tasks.json` and paste:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Build & Run OpenGL",
      "type": "shell",
      "command": "g++ \"${file}\" -o \"${fileDirname}\\${fileBasenameNoExtension}.exe\" -lfreeglut -lopengl32 -lglu32; & \"${fileDirname}\\${fileBasenameNoExtension}.exe\"",
      "group": { "kind": "build", "isDefault": true }
    }
  ]
}
```

Now open any `.cpp` file and press **`Ctrl+Shift+B`** to compile and run it.

Your folder should look like this:

```
cg-lab-W[id]/
├── .vscode/
│   └── tasks.json
├── main.cpp
└── main.exe
```

---

## Troubleshooting

| Problem | Fix |
|---------|-----|
| `g++` is not recognized | Check that `C:\msys64\ucrt64\bin` is in PATH, then open a new terminal (or restart VS Code). |
| `GL/glut.h: No such file or directory` | freeglut is not installed. Repeat Step 1.2 in **MSYS2 UCRT64** (not the plain MSYS2 shell). |
| `undefined reference to glutInit` (or other `gl...` functions) | The `-l` link flags are missing or misspelled. Use the exact command in Step 7. |
| `running scripts is disabled on this system` | Run the `Set-ExecutionPolicy` command from Step 6. |
| `Ctrl+Shift+B` does nothing | Make sure `tasks.json` is inside `.vscode/` at the root of the opened folder, and that a `.cpp` file is the active tab. |

---

## Lab Task

Modify `main.cpp` to try the following:

1. Change the point colour to blue.
2. Increase the point size to 20.
3. Draw three more points at different positions in the window.