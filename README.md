# CSE444 – Computer Graphics & Animation Lab

Lab work for **CSE444: Computer Graphics & Animation Lab**, written in **C++** with **OpenGL** and **freeglut** on Windows (MSYS2 + VS Code).

Each lab is in its own folder (`lab01`, `lab02`, …). Every lab folder has its own `README.md` explaining the topic, the code, and that lab's tasks.

## Labs

| Lab | Topic | Folder | What's inside |
|-----|-------|--------|---------------|
| 01 | OpenGL environment setup | [`lab01/`](lab01/) | Installing MSYS2, g++ and freeglut; a first program that draws points |
| 02 | Line drawing algorithms | [`lab02/`](lab02/) | DDA, Bresenham and Midpoint line algorithms, with a comparison of when to use each |

## Repository structure

```
.
├── README.md            ← you are here
├── .gitignore           ← keeps compiled .exe files out of git
├── .vscode/
│   └── tasks.json       ← Ctrl+Shift+B: build and run the open .cpp file
├── lab01/               ← Lab 1: OpenGL setup
│   ├── README.md
│   └── main.cpp
└── lab02/               ← Lab 2: Line drawing algorithms
    ├── README.md
    ├── dda.cpp
    ├── bresenham.cpp
    └── midpoint.cpp
```

## Branches

| Branch | Purpose |
|--------|---------|
| `main` | Stable branch. Holds every finished lab. |
| `line-drawing-algorithms` | Lab 2 work (DDA, Bresenham, Midpoint) and this repository restructure. Merged into `main` when done. |

### Workflow for a new lab

1. Start from an up-to-date `main`:
   ```bash
   git checkout main
   git pull
   git checkout -b <lab-topic>        # e.g. circle-drawing-algorithms
   ```
2. Create the next folder (`lab03/`, `lab04/`, …) with a `README.md` and the `.cpp` files.
3. Add a row for the lab to the **Labs** table above, and its branch to the **Branches** table.
4. Commit, push, and open a pull request into `main`.

## Getting started

New machine? Follow [Lab 1](lab01/README.md) to install the compiler and OpenGL libraries.

### Compile and run any program

From inside a lab folder:

```powershell
g++ <file>.cpp -o <file>.exe -lfreeglut -lopengl32 -lglu32
.\<file>.exe
```

Or open the `.cpp` file in VS Code and press **`Ctrl+Shift+B`**.

Compiled `.exe` files are ignored by git. Only source code and READMEs are committed.
