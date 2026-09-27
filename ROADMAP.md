# Chess Engine — Full Project Plan

> **Where do I start right now?** → [NOW.md](NOW.md). This file is the whole map;
> `NOW.md` is the "you are here" pin, and is kept current as work progresses.

## Context

Michio is building a chess engine as a C++ learning vehicle. Background: basic C, no
prior C++, comfortable in Python, no ML experience. Per `CLAUDE.md`, **he writes every
line of implementation code** — this document is a study and build guide for him, not
an implementation plan for Claude. No code appears here: design decisions, concepts,
research targets, and sequencing only.

The project has three tracks:

1. **Classical engine** (the original 7 milestones) — C++, board through UCI
2. **Neural evaluation** — NNUE, supervised first, then a self-play refinement loop
3. **Physical board** — 3D-printed board and pieces, reed-switch sensing, LED output

### Decisions already made

- NNUE over AlphaZero
- Reed switches over hall-effect or computer vision
- LED move indication now; motorised pieces deferred
- FEN parsing inside milestone 1
- Board layout (8x8 vs 10x12) — 10x12 with 1D array. Makes sensing moves easier than annoying if else statments

### Current state

CMake build works end-to-end under Cygwin (g++ 14.4, cmake 4.4.3, make 4.4.1,
gdb 17.2). `src/main.cpp` holds a 17-line scratch loop with a known out-of-bounds write
(char used as array subscript, line 9), to be discarded as milestone 1 begins.

---

## The headline number

Roughly **340–640 hours**. At 5–8 h/week that is a **1.5 to 3 year** project.

That is worth staring at before committing. Two things make it survivable, and both
are structural rather than motivational:

- **Something plays chess at every stage.** After M4 you have a real opponent. After
  M5 it plugs into a GUI. The neural work and the hardware each improve a thing that
  already works, rather than being prerequisites for a first result.
- **The hardware track runs in parallel.** It depends only on UCI, not on the neural
  work, and 3D printing is wall-clock-heavy but attention-light. You can print a piece
  set over a weekend while studying PyTorch.

---

## Revised milestone order — one proposed change

`CLAUDE.md` currently lists UCI last, at milestone 7. **Recommend moving it to
milestone 5**, immediately after alpha-beta:

| # | Milestone | Was |
|---|---|---|
| 1 | Board representation + FEN + print | 1 |
| 2 | Legal move generation + perft | 2 |
| 3 | Minimax + hand-crafted evaluation | 3 |
| 4 | Alpha-beta pruning | 4 |
| **5** | **UCI + GUI** | **was 7** |
| 6 | Bitboard rewrite | 5 |
| 7 | Transposition tables + move ordering | 6 |
| 8 | NNUE — supervised | new |
| 9 | Self-play refinement loop | new |
| 10 | Physical board | new |

Three reasons UCI has to move:

1. **It is the hub both new tracks plug into.** The physical board talks UCI; the NNUE
   evaluation slots in behind UCI without the board knowing anything changed. Leaving
   it at #7 blocks both.
2. **It is how you test.** Once you speak UCI you can play your engine against
   Stockfish at reduced strength, run automated match series in Cute Chess, and get a
   real Elo number. Without it you are judging your engine by eyeballing its moves.
3. **It unblocks hardware early.** You can develop and debug the entire physical board
   against Stockfish while your own engine is still weak.

**Status: accepted 2026-09-17.** `CLAUDE.md` has been updated to match, so the rule and
the plan agree. The table above is now the authoritative milestone order.

---

## Track 1 — Classical engine

### Milestone 1: board representation

**Goal:** a board that can hold any legal position, be loaded from FEN, and be printed.

**What "the board" contains.** Not just piece placement. A position needs all six of:
piece placement; side to move; four castling-rights flags; the en passant target
square; the halfmove clock; the fullmove number. Castling rights and en passant are
*history-dependent* — you cannot reconstruct them by looking at the pieces. Beginners
omit them and retrofit them painfully around M3. Put them in from day one.

**Step 0 — Read, then choose a square-mapping scheme.** Chess Programming Wiki:
*Board Representation*, *Mailbox*, *10x12 Board*, *Square Mapping Considerations*
(~1–2 h). Then commit to a layout in writing and move on. Time-box it; this is a
decision checkpoint, not an open research project. If you come out with no preference,
take 10x12 — it makes M2 tractable, and M2 is where this project is most likely to
stall. Note that M6 replaces all of it with bitboards anyway, so the stakes are lower
than they feel.

**Step 1 — Encode pieces and squares.** C would reach for `#define` and `int`; C++ has
a better tool. Understand *why* `enum class` exists before picking.

**Step 2 — Build the position structure.** All six components above. Decide struct vs
class, and what should be private.

**Step 3 — Print it.** ASCII, rank 8 at top. Print side-to-move, castling rights and
en passant square too — they are invisible otherwise and will be the source of bugs.
This is your debugging lifeline for the entire project.

**Step 4 — Parse FEN.** The standard one-line position format. Every published
move-generation test case is a FEN string, so this is what makes M2 testable at all.

**Step 5 — Round-trip verification.** Parse, print, compare. Then parse and re-emit
the FEN and confirm you get the identical string back, counters included.

### Milestone 2: the wall

Legal move generation, 30–60 h, and where most hobby engines die. Pseudo-legal moves
are easy; *legality* — pins, check evasion, castling through attacked squares, en
passant's genuinely bizarre edge cases — is where the hours go.

**Perft is the thing that saves you.** It counts leaf nodes to a fixed depth from a
given position, and published reference counts exist for standard test positions. A
mismatch at depth 3 tells you that you have a bug and roughly where. Read the Chess
Programming Wiki *Perft* page before starting, and build perft alongside the generator
rather than after it.

### C++ concepts, in the order you'll meet them

`enum class` (scoping, no implicit int conversion — directly relevant to the bug in
your current `main.cpp`) · `struct` vs `class` · `std::array` vs C arrays · `constexpr`
· header/source split and `#pragma once` · **references and `const` correctness**, the
single biggest idiom shift from C · `std::string` and `std::string_view` for FEN ·
`std::vector` for move lists in M2.

Explicit non-goals for M1: templates, move semantics, smart pointers, inheritance,
operator overloading. Reaching for them now produces worse code, not better.

---

## Track 2 — NNUE

### Why NNUE and not AlphaZero

NNUE replaces the hand-crafted evaluation from M3 while leaving your alpha-beta search
essentially intact. Everything you build in M1–M7 stays useful. It trains supervised on
CPU-evaluable data, runs inference in C++ on CPU with no GPU at play time, and it is
what actually made Stockfish strong. AlphaZero-style MCTS would have thrown away M4 and
most of M7 and needed a GPU farm to reach interesting strength.

### Prerequisite: PyTorch (25–40 h)

You know Python but not ML. Do **one small non-chess project first** — MNIST or
similar — end to end: dataset, training loop, validation split, overfitting, a model
that won't converge and figuring out why. Doing your first training loop and your first
NNUE simultaneously means every bug has two possible causes and you won't know which.

### Milestone 8: supervised NNUE

**Architecture.** Start far simpler than Stockfish. A 768-feature input (64 squares ×
6 piece types × 2 colours) into 256 → 32 → 1 is a real NNUE and is enough to learn on.
Stockfish's HalfKP uses king-bucketed inputs an order of magnitude larger; that is a
later optimisation, not a starting point.

**The "U" is the hard part.** Efficiently Updatable means that when a move changes one
or two pieces you do *not* recompute the first layer — you add and subtract the
affected feature columns incrementally as you make and unmake moves. This is what makes
it fast enough to sit inside a search running millions of nodes per second, and it is
why NNUE requires a finished engine: it hooks directly into make/unmake.

**Training data.** Three options, roughly in order of effort: the Lichess open database
(database.lichess.org) has evaluations on a subset of positions; generate your own by
running Stockfish at fixed shallow depth over positions; or start from an existing
Stockfish-community training set. Target either raw centipawn evaluation or a blend of
evaluation and win/draw/loss outcome — the blend trains more stably.

**Reference.** The official `nnue-pytorch` repository from the Stockfish team is the
best available worked example of the data format, training loop, and quantisation.

**Quantisation.** Stockfish uses int8/int16 arithmetic for speed. Your first version
should use floats and be correct-but-slow. Quantise only once it works.

### Milestone 9: the self-play loop — what "RL" actually means here

Be precise about this, because the expectation gap is the risk. In the NNUE world,
"reinforcement learning" is not policy gradients. It is an **iterated self-play data
loop**: your engine plays itself, the resulting positions are labelled by a deeper
search of the *current* engine, you retrain on those labels, and repeat. It resembles
iterated supervised learning more than textbook RL, and it is genuinely how Stockfish's
networks improved generation over generation.

This will make your engine meaningfully stronger. It will not produce AlphaZero.
Calibrate accordingly and the result will feel like the success it is.

---

## Track 3 — physical board

Depends only on **M5 (UCI)**. Can proceed in parallel with Tracks 1 and 2 from that
point, and should, since print time is wall-clock-heavy and attention-light.

### Sensing: reed switches

Small neodymium magnets in the piece bases; 64 reed switches, one per square, wired as
an 8x8 scanned matrix. This detects **occupied/empty only** — not colour, not piece
type. That is sufficient, because your engine already tracks the position: "d2 emptied,
then d4 filled" names the move unambiguously.

**Two details that bite:**

- **Diodes.** A scanned switch matrix needs one diode per switch (1N4148 is standard)
  or you get *ghosting* — three closed switches making a fourth appear closed. With 32
  pieces on the board this is not a corner case, it is the normal state.
- **Reed switches are directional.** They respond to field along their axis. Magnet
  orientation in the piece base and switch orientation under the square have to agree,
  and this is much easier to fix in the design than after 64 of them are soldered.
  Prototype a single square before committing to the full matrix.

### Move detection is a state machine, and the edge cases are chess-specific

Lift-then-place is the easy path. The ones that will break a naive implementation:

- **Captures** — the captured piece is lifted first, and the two lifts happen before
  the place. Two squares empty simultaneously with no move completed yet.
- **Castling** — two pieces move in one turn, and users do it in either order.
- **En passant** — the captured pawn is removed from a square the capturing pawn never
  touches. A "which square changed" model that assumes the capture square equals the
  destination square will simply be wrong here.
- **Piece hovering** — users lift a piece, think, and put it back on the origin square.
  This must not be interpreted as a move.

Your engine's own legal move list is what resolves the ambiguity: generate the legal
moves, and match the observed square-change pattern against them. Detection should ask
"which legal move is consistent with what I just saw," not "what happened."

### Output: addressable LEDs

64 WS2812B LEDs, one per square, single data pin. Light the from-square and to-square
for the engine's move. Also useful for signalling illegal moves and board-desync
recovery, which you will need more than you expect.

### Controller and connection

ESP32 recommended over Arduino Mega — WiFi means the board need not be tethered to the
PC. 64 inputs via shift registers or MCP23017 I2C expanders. The MCU does sensing, LED
driving, and move-pattern reporting; the PC runs the engine and speaks UCI. Keep chess
logic off the microcontroller.

### 3D printing

Board almost certainly in quadrants or tiles — a full board exceeds most printers' bed.
Pieces need a cavity for the magnet, either a print pause or a glue-in pocket. Budget
significant wall-clock print time for 32 pieces plus board sections, and print a single
pawn and a single square first to validate magnet fit and switch triggering before
committing a kilogram of filament.

### Parts budget

Reed switches ~$12 · diodes ~$2 · magnets ~$8 · ESP32 ~$8 · WS2812B ~$12 · 5V PSU ~$10
· wire/perfboard/connectors ~$15 · filament ~$25–50. **Total roughly $95–120**, plus a
gantry later if you take the motorised path.

---

## Time estimates

Assume learning C++ (and later PyTorch) while building. Individual variance is easily 3x.

| Milestone | Estimate |
|---|---|
| 1. Board representation + FEN | 11–20 h |
| 2. Legal move generation + perft | 30–60 h |
| 3. Minimax + evaluation | 10–20 h |
| 4. Alpha-beta | 8–15 h |
| 5. UCI + GUI | 10–20 h |
| 6. Bitboard rewrite | 40–80 h |
| 7. Transposition tables + move ordering | 20–40 h |
| **Classical engine subtotal** | **130–255 h** |
| PyTorch prerequisite (non-chess project) | 25–40 h |
| 8. NNUE data pipeline | 15–25 h |
| 8. NNUE training and tuning | 25–50 h |
| 8. C++ inference + incremental updates | 25–40 h |
| 9. Self-play refinement loop | 30–60 h |
| **Neural subtotal** | **120–215 h** |
| 10. Electronics: matrix, diodes, LEDs | 30–50 h |
| 10. 3D design and printing | 25–50 h |
| 10. ESP32 firmware | 20–35 h |
| 10. UCI integration and edge cases | 15–25 h |
| **Hardware subtotal** | **90–160 h** |
| **Total** | **340–630 h** |

At 5–8 h/week: **milestone 1 in 2–4 weeks**, a playable engine (M5) in **8–14 months**,
the whole thing in **1.5–3 years**. The hardware track overlaps the neural track, so
calendar time is somewhat less than the sum suggests.

---

## Verification

**Milestone 1 is done when:** the starting position prints correctly with rank 8 at top;
a mid-game FEN loads and matches; FEN round-trip is lossless including castling rights,
en passant square and both counters; malformed FEN is rejected without crashing or
silently corrupting the board; the build is clean under `-Wall -Wextra -Wpedantic` with
zero warnings; and you can set a gdb breakpoint and understand every field you see.

**Milestone 2:** perft matches published node counts to depth 5 on the standard test
position set — including Kiwipete and the known en-passant/promotion trap positions,
not just the starting position.

**Milestone 5:** your engine appears in Cute Chess, completes a 100-game match against
Stockfish at a reduced skill level without a protocol error or illegal move, and
returns a real Elo estimate.

**Milestone 8:** the NNUE engine beats your own hand-crafted-eval engine from M4 over a
statistically meaningful match (several hundred games); and C++ inference output matches
the PyTorch model's output on the same positions to within float tolerance. That second
check catches the incremental-update bugs, which are otherwise near-impossible to find.

**Milestone 9:** each generation beats the previous one in head-to-head match play.

**Milestone 10:** a full game played on the physical board, including at least one
capture, one castling, and one en passant, with no desync.

---

## Schedule for milestone 1, at 5–8 h/week

Each week ends at a **resumable checkpoint** — build green, one sentence describing
what's done. At this pace there will be gaps; the checkpoints are what make a two-week
gap cost ten minutes instead of a weekend.

- **Week 1 — Read and decide (5–7 h).** Chess Programming Wiki board-representation
  pages; skim *Perft*. Alongside: *User-Defined Types* and *Modularity* from *A Tour of
  C++*, and `enum class` on cppreference. *Checkpoint:* layout decision written down
  with a sentence saying why.
- **Week 2 — Encoding and structure (6–8 h).** Piece/square encoding, position struct
  with all six components, first header in `include/` and source in `src/`. Discard the
  scratch `main.cpp`. *Checkpoint:* clean build, zero warnings.
- **Week 3 — Print (5–7 h).** ASCII rendering plus state fields. *Checkpoint:* starting
  position prints correctly.
- **Week 4–5 — FEN and verification (6–10 h).** Parse, emit, round-trip, reject bad
  input. *Checkpoint:* milestone 1 complete per the verification list.

Ordering matters far more than dates. If a week slips, slip it.

---

## Research sources

**Engine:** Chess Programming Wiki (chessprogramming.org) — canonical for the whole
project · cppreference.com for exact C++ behaviour · *A Tour of C++* (Stroustrup),
chapters *User-Defined Types*, *Modularity*, *Classes*, *Strings* · Bluefever Software's
"Programming a Chess Engine in C" on YouTube for architecture — **caveat: it is C, not
C++**, so take the what-and-why and decide the C++ expression yourself.

**NNUE:** the `nnue-pytorch` repository and its docs (the Stockfish team's own training
code) · the Chess Programming Wiki *NNUE* page · Stockfish's own evaluation source for
how quantised inference is structured.

**Hardware:** existing DIY electronic chessboard build logs are plentiful and worth
reading before ordering parts — the reed-matrix-plus-WS2812B design is well-trodden and
someone has already hit the mistake you are about to make.

---

## Working agreement (from `CLAUDE.md`)

Michio writes all of it. Claude explains concepts, discusses tradeoffs, points at bug
*categories and locations* without supplying corrected lines, and reviews code only
after it compiles and runs.

**Confirmed 2026-09-17:** the no-implementation-code rule covers **every language in the
project** — C++, the Python NNUE training code, and the ESP32 firmware — with no
exemption for "standard boilerplate" like a PyTorch training loop or an Arduino
`setup()`/`loop()` skeleton. `CLAUDE.md` rule 1 now says so explicitly.

Good asks along the way: *"explain what `enum class` protects me from"* · *"my FEN
parser mangles castling rights"* · *"is this idiomatic C++ or C with extra syntax?"* ·
*"quiz me on board representation before I start M2"* · *"why won't my NNUE converge?"*
