# Setup — chess_engine

How to get this project building and running on a fresh machine, and how the
tooling fits together. Written for both humans and AI assistants: every step
is a numbered command block, and every config file is explained in the
[Reference](#reference) section.

> **TL;DR** (on a machine that is already set up)
> ```bash
> cmake --preset default                      # configure (once, or after CMake changes)
> cmake --build --preset default              # compile
> ./build/chess_engine                        # run
> ```
> In VS Code: **Ctrl+Shift+B** to build, **F5** to build + run under the debugger.

---

## Contents

1. [What you need](#1-what-you-need)
2. [Install WSL + Ubuntu (Windows only)](#2-install-wsl--ubuntu-windows-only)
3. [Install the toolchain](#3-install-the-toolchain)
4. [Get the code](#4-get-the-code)
5. [Set up VS Code](#5-set-up-vs-code)
6. [Build and run](#6-build-and-run)
7. [Troubleshooting](#7-troubleshooting)
8. [Reference](#reference)

---

## 1. What you need

| Tool | Version known to work | Why |
|---|---|---|
| Windows + WSL 2 | Ubuntu 26.04 LTS | Linux environment on Windows. Skip on native Linux. |
| `g++` | 15.2 | C++ compiler. Project requires **C++17**. |
| `cmake` | 4.2 (needs **≥ 3.25** for presets v6) | Build system. |
| `make` | any | CMake generates Makefiles. |
| `gdb` | any | Debugger used by F5 in VS Code. |
| `git` | any | Version control. |
| VS Code | latest | Editor, connected to WSL. |

**Do not** use Cygwin or MinGW compilers/CMake. A Cygwin CMake previously
corrupted the build cache (it produced `.exe` files and a broken
`build/CMakeCache.txt`). Everything runs inside WSL.

---

## 2. Install WSL + Ubuntu (Windows only)

In **PowerShell as Administrator**:

```powershell
wsl --install -d Ubuntu
```

Reboot if asked, open "Ubuntu" from the Start menu, and create a Linux
username/password. All remaining commands run **inside the Ubuntu terminal**.

---

## 3. Install the toolchain

```bash
sudo apt update
sudo apt install -y build-essential cmake gdb git
```

`build-essential` provides `g++` and `make`.

Verify:

```bash
g++ --version      # expect 15.x or newer
cmake --version    # must be >= 3.25
gdb --version
```

If Ubuntu's `cmake` is older than 3.25, install a newer one from
<https://cmake.org/download/> or via `sudo snap install cmake --classic`.

---

## 4. Get the code

```bash
mkdir -p /mnt/c/users/<you>/projects
cd /mnt/c/users/<you>/projects
git clone https://github.com/Febricant/chess_engine.git
cd chess_engine
```

Set your git identity if this is a new machine:

```bash
git config --global user.name  "Your Name"
git config --global user.email "you@example.com"
```

> **Note:** The repo lives on the Windows drive (`/mnt/c/...`) so it is visible
> from Windows too. Keeping it in the Linux home (`~/projects`) builds faster;
> either works.

---

## 5. Set up VS Code

1. Install VS Code on **Windows**.
2. Install the **WSL** extension (`ms-vscode-remote.remote-wsl`).
3. From the Ubuntu terminal, inside the repo:
   ```bash
   code .
   ```
   This opens VS Code connected to WSL (bottom-left corner shows
   `WSL: Ubuntu`).
4. Install these extensions **in WSL** (Extensions panel → "Install in WSL"):

   | Extension | ID |
   |---|---|
   | C/C++ | `ms-vscode.cpptools` |
   | CMake Tools | `ms-vscode.cmake-tools` |
   | C/C++ Extension Pack (optional) | `ms-vscode.cpptools-extension-pack` |

   Or from the terminal:
   ```bash
   code --install-extension ms-vscode.cpptools
   code --install-extension ms-vscode.cmake-tools
   ```

5. Run the configure step once (see next section) so IntelliSense can find
   headers, then **Ctrl+Shift+P → "Developer: Reload Window"**.

The `.vscode/` folder in the repo already contains the tasks, launch config
and settings — nothing else to configure.

---

## 6. Build and run

### From the terminal

```bash
cmake --preset default              # 1. configure → creates build/
cmake --build --preset default      # 2. compile   → build/chess_engine
./build/chess_engine                # 3. run
```

- Step 1 is only needed the first time, after editing `CMakeLists.txt` /
  `CMakePresets.json`, or after deleting `build/`.
- New `.cpp` files under `src/` are picked up automatically by step 2.

One-liner for the edit–compile–run loop:

```bash
cmake --build --preset default && ./build/chess_engine
```

### From VS Code

| Action | How | What it runs |
|---|---|---|
| Build | **Ctrl+Shift+B** | Task `CMake Compile` (runs `CMake Build` first) |
| Build + run + debug | **F5** (config `chess_engine`) | Builds, then launches `build/chess_engine` in gdb |
| Configure only | Terminal → Run Task → `CMake Build` | `cmake --preset default` |

Compiler errors from Ctrl+Shift+B appear in the **Problems** panel as
clickable file:line entries.

> ⚠️ **Do not use the ▶ "Run C/C++ File" button** in the editor's top-right
> corner. It comes from the C/C++ extension, ignores CMake, and compiles only
> the open file with plain `gcc` — so it can't find `include/`, doesn't
> compile the other `.cpp` files, and doesn't link the C++ standard library.
> Clicking it also re-adds a `C/C++: gcc build active file` task to
> `.vscode/tasks.json`; delete that task if it appears.

### Release build (for measuring engine speed)

The preset builds in **Debug** (no optimisation, full debug info). For
timing search/perft:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/g++
cmake --build build-release
./build-release/chess_engine
```

---

## 7. Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| `fatal error: encoding.h: No such file or directory` when clicking ▶ | ▶ button bypasses CMake | Use Ctrl+Shift+B / F5 / terminal instead (see §6). |
| Red squiggle on `#include "something.h"` but terminal build works | IntelliSense hasn't read `build/compile_commands.json` | Run `cmake --preset default`, then Reload Window. |
| `undefined reference to 'foo(int)'` at link time | Declaration in the `.h` and definition in the `.cpp` have different signatures (C++ treats them as two overloads) | Make the parameter/return types match exactly. |
| `cmake --build` says `could not load cache` / compiler errors mentioning `.exe` | Stale or Windows-made `build/` | `rm -rf build && cmake --preset default` |
| `CMake Error: ... preset version 6 ... not supported` | CMake older than 3.25 | Upgrade CMake (§3). |
| Program waits and prints nothing | It's waiting for `std::cin` input | Type input in the terminal (F5: the debug terminal). |

Nuke-and-rebuild, which fixes most build weirdness:

```bash
rm -rf build
cmake --preset default
cmake --build --preset default
```

---

## Reference

### Repo layout

```
chess_engine/
├── CMakeLists.txt         # what to build: all src/*.cpp → chess_engine, C++17, warnings on
├── CMakePresets.json      # how to configure: g++, Debug, Makefiles, export compile_commands
├── include/               # headers (.h) — on the include path, so #include "x.h" works
├── src/                   # sources (.cpp) — globbed automatically
├── build/                 # generated, git-ignored; binary is build/chess_engine
├── .vscode/
│   ├── tasks.json         # CMake Build (configure) + CMake Compile (build, default)
│   ├── launch.json        # F5 config "chess_engine": build then gdb
│   └── settings.json      # use presets; IntelliSense ← build/compile_commands.json
├── CLAUDE.md              # rules for AI assistants (teach, don't write code)
├── ROADMAP.md             # milestone plan
├── NOW.md                 # current milestone / "you are here"
└── SETUP.md               # this file
```

### Config files explained

**`CMakeLists.txt`**
- `CMAKE_CXX_STANDARD 17`, required, no GNU extensions (`-std=c++17`).
- Defaults to `Debug` if no build type is given.
- `file(GLOB_RECURSE ... CONFIGURE_DEPENDS src/*.cpp)` — every `.cpp` in
  `src/` is compiled; new files need no CMake edit.
- `target_include_directories(... include)` — headers are found by name.
- Warnings: `-Wall -Wextra -Wpedantic`.

**`CMakePresets.json`** — preset `default`:
- Generator `Unix Makefiles`, output dir `build/`.
- `CMAKE_CXX_COMPILER=/usr/bin/g++`, `CMAKE_BUILD_TYPE=Debug`.
- `CMAKE_EXPORT_COMPILE_COMMANDS=ON` → writes `build/compile_commands.json`
  (the exact compiler command for each file; IntelliSense reads it).

**`.vscode/tasks.json`**
- `CMake Build` → `cmake --preset default` (configure).
- `CMake Compile` → `cmake --build --preset default`; depends on `CMake Build`;
  default build task; `$gcc` problem matcher.

**`.vscode/launch.json`**
- `chess_engine`: `cppdbg` / gdb, program `build/chess_engine`, cwd = repo
  root, `preLaunchTask: CMake Compile`.

**`.vscode/settings.json`**
- `cmake.useCMakePresets: always`, `cmake.configureOnOpen: false`.
- `C_Cpp.default.compileCommands: ${workspaceFolder}/build/compile_commands.json`.

**`.gitignore`**
- `build/`, `*.exe`, `.claude/settings.local.json`.

### Notes for AI assistants

- Read `CLAUDE.md` first: this is a learning project, and the owner writes
  all implementation code. Explain, review and debug — do not write engine,
  Python or firmware code.
- Build/verify with the terminal commands in §6; never invent new build
  paths. The binary is always `build/chess_engine`.
- Tooling/config changes (CMake, `.vscode/`, docs) are fine to make when asked.
