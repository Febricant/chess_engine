# NOW — where to pick up

**Last updated:** 2026-09-17
**You are here:** Milestone 1 → **Step 1 — Encode pieces and squares**
**Full map:** [ROADMAP.md](ROADMAP.md)

**Decided in Step 0:** 10x12 mailbox, flat 1D array of 120 elements.

---

## This session — first code of the project

About 3–5 hours. This is where you stop reading and start writing, and it's your first
multi-file C++ build.

### The consequence of your Step 0 decision

You picked 10x12, so **"empty" and "off-board" are two different things** and both need
a value. An 8x8 board would have needed only "empty"; the border cells in your array are
squares that exist in memory but not in chess, and code that confuses the two will walk
a rook straight off the edge of the world.

That distinction is the first thing your piece encoding has to express.

### What to work out

- [ ] **Piece encoding.** You need: empty, off-board, and six piece types in two
      colours. Decide whether colour and type are one value or two. Both designs are
      defensible; think about which one makes "is this piece mine?" cheap, because
      you'll ask that question millions of times per second in Milestone 2.
- [ ] **Square encoding.** Your squares are indices 0–119, but only 64 of them are real.
      Work out which. You will want to convert between a square index and a
      file/rank pair, and between a square index and algebraic notation like `e4`.
- [ ] **The two mapping tables.** A 64→120 table and a 120→64 table is the standard
      mailbox idiom. Work out why you'd want *both* directions before you build them —
      each one answers a different question, and FEN parsing in Step 4 needs one of
      them specifically.
- [ ] **Your first header.** Create a `.hpp` in `include/` and a `.cpp` in `src/`. The
      CMakeLists already globs `src/*.cpp` and puts `include/` on the search path, so
      new files are picked up with **no build-file edit**.
- [ ] **Delete the scratch `main.cpp`.** It has served its purpose. Write a real one.

### C++ concepts this step

`enum class` — and specifically why it's better here than `#define` or a bare C enum ·
`constexpr` for compile-time constants · `std::array` vs a C array · `#pragma once` and
what problem it solves · the header/source split: what goes in which file, and why.

---

## Step 1 is done when

- [ ] `cmake --build build` succeeds with **zero warnings** under `-Wall -Wextra -Wpedantic`
- [ ] A header in `include/` and a source file in `src/`, both picked up automatically
- [ ] You can name any square both ways: index ↔ algebraic
- [ ] The scratch `main.cpp` is gone
- [ ] You can explain why `enum class` is better here than `#define`

---

## Then: Step 2 — the position structure

All six components: piece placement, side to move, four castling-rights flags, en
passant target square, halfmove clock, fullmove number. Not just the pieces.

---

## Not yet

- Move generation, direction offsets, attack detection — Milestone 2. The 10x12 payoff
  arrives there; don't spend it early.
- FEN parsing — Step 4, after the position structure exists
- Anything neural — Milestone 8, with a PyTorch warm-up first
- Hardware parts — Milestone 10, gated on UCI
- Templates, smart pointers, inheritance, operator overloading — not needed

---

## Settled

- **UCI moves to Milestone 5**, ahead of the bitboard rewrite. `CLAUDE.md` updated.
- **The no-code rule covers every language** — C++, Python, firmware. No boilerplate
  exemption. `CLAUDE.md` rule 1 updated.

---

## How this file works

This is the "you are here" pin. Say *"update NOW.md, I finished step 1"* and it'll be
re-pointed. `ROADMAP.md` is the stable full map and changes rarely.
