# NOW — where to pick up

**Last updated:** 2026-10-15
**You are here:** Milestone 1 → **Step 2 — The position structure**
**Full map:** [ROADMAP.md](ROADMAP.md)

**Decided so far:** 10x12 mailbox, flat 1D array of 120 (Step 0). Pieces as one byte:
0 off-board, 1 empty, 2–7 white, black = white + 8, so bit 3 is the colour (Step 1).

---

## Step 1 — done (2026-10-15)

Took **10+ hours** against a 3–5 hour plan. Most of the extra went into reworking code
that already worked; see "Lessons" below.

- `pieces` is an `enum class` with a `std::uint8_t` underlying type; `black_conversion`
  is the named bit-3 constant
- Helpers `is_piece` / `is_white` / `is_black` / `piece_type`, verified on every value
- Square names both ways: index → name and name → index, with input validation
  (returns −1 for bad text). Round trip verified on all 64 squares
- 12×10 debug printer and 8×8 printer (rank 8 on top), both taking the board by `const &`
- Build is clean under `-Wall -Wextra -Wpedantic`; merged in PRs #3–#5
- **Pending:** the `enum class` quiz (explain why it beats `#define`)

### Carried over (small, fix when convenient)

- `helper.hpp` and `encoding.hpp` include each other. Move the `pieces` enum into
  `helper.hpp` so includes run one way only — Step 2 adds headers, so do it then.
- The yes/no helpers return `int`; `bool` says what they mean. `piece_type` could return
  `pieces`.
- The two square-name functions still use the 64-entry name table plus border math. It
  works; leave it unless Step 4 (FEN) makes it awkward.

### Lessons from Step 1

- **`int` for arithmetic and loop counters; `std::uint8_t` only for storage.** Three bugs
  came from `std::uint8_t`: a countdown loop wrapping 0 → 255, rank-1 math going below
  zero, and a number printing as the letter `D` (`std::uint8_t` *is* `unsigned char`).
- **Header and `.cpp` signatures must match exactly** — a mismatch makes two different
  functions (overloads), not one.
- **Read the first error, then rebuild.** The rest are usually fallout.
- **Warnings are bugs reported early.** `-Waddress` found a function named but not
  called. Use `cmake --build --preset default --clean-first` to see every warning.
- **Test every value, not one example.** A full table caught `is_piece` missing knights
  and queens.
- **Once it works and passes its test, move on.** Note improvements; don't redesign.

---

## This step — the position structure

Estimate at your actual pace: **5–8 hours.**

A chess position is more than where the pieces stand. Two positions with identical
pieces can be different positions, because of whose turn it is, who may still castle,
and whether en passant is possible. Step 2 puts all of it in **one thing** you can pass
around.

### The six components

1. **Piece placement** — your 120 board
2. **Side to move** — white or black
3. **Castling rights** — four of them: white king-side, white queen-side, black
   king-side, black queen-side
4. **En passant target square** — a square index, or "none"
5. **Halfmove clock** — moves since the last capture or pawn move (the 50-move rule)
6. **Fullmove number** — starts at 1, goes up after black moves

### What to work out

- [ ] **`struct` or `class`?** In C++ they differ in exactly one way. Find out what it is,
      then decide which one says what you mean. The question underneath: should other
      code be allowed to change the fields directly, or only through functions you
      provide?
- [ ] **A type for each field.** Side to move has exactly two values — what does that
      suggest, given what you learned about `enum class`? Castling: four `bool`s, or four
      bits in one byte (the same trick as your colour bit)? Both work; pick the one you
      can explain. En passant needs a value meaning "no square" that can never be a real
      index — you already chose one for name → index.
- [ ] **What "empty" means.** When a position is created, what should every field hold?
      Look up default member initializers; a position that starts half-filled with
      garbage is a Step 4 bug waiting to happen.
- [ ] **Where the functions go.** Your printers and helpers take the board as a
      parameter today. Once the board lives inside a position, decide whether they take
      a position, or become member functions. Look up `const` member functions and what
      the `const` after the parameter list promises.
- [ ] **The starting position.** Until FEN exists, something has to fill a position with
      the opening setup. Reuse `piece_board_initial` for the pieces; the other five
      fields have known starting values — find them.
- [ ] **Fix the circular include** (carried over) while you're adding the new header.

### C++ concepts this step

`struct` vs `class` and the one real difference · access specifiers (`public` /
`private`) · member functions and `const` member functions · default member initializers
· a second `enum class` (for colour) · passing the whole position by `const &`.
Reference: *A Tour of C++*, chapter *Classes*; cppreference *Classes* and *Default
member initializer*.

---

## Step 2 is done when

- [ ] One type holds **all six** components — not just the pieces
- [ ] A freshly created position has a defined value in every field
- [ ] You can set up the starting position and print it
- [ ] Clean build under `-Wall -Wextra -Wpedantic` with **zero warnings** (`--clean-first`)
- [ ] You can stop in gdb, expand the position, and say what every field means
- [ ] You can explain why you chose `struct` or `class`

---

## Then: Step 3 — print it properly

Your 8×8 printer already gets the board right. Step 3 adds the invisible state under it:
side to move, castling rights, en passant square. Those are the fields that cause bugs
precisely because you can't see them.

---

## Not yet

- Move generation, direction offsets, attack detection — Milestone 2. An empty
  `move_validity.cpp` came and went; leave it until Milestone 2.
- FEN parsing — Step 4, after the position struct exists
- Anything neural — Milestone 8, with a PyTorch warm-up first
- Hardware parts — Milestone 10, gated on UCI
- Templates, smart pointers, inheritance, operator overloading — not needed

---

## Settled

- **UCI moves to Milestone 5**, ahead of the bitboard rewrite. `CLAUDE.md` updated.
- **The no-code rule covers every language** — C++, Python, firmware. No boilerplate
  exemption. `CLAUDE.md` rule 1 updated.
- **C++20**, for `std::format` (2026-10-11).
- **Headers are `.hpp`**; small hot helpers may live in headers as `constexpr`/`inline`.

---

## How this file works

This is the "you are here" pin. Say *"update NOW.md, I finished step 2"* and it'll be
re-pointed. `ROADMAP.md` is the stable full map and changes rarely.
