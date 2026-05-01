# chessNova TODO

## Core Engine Features
- [ ] **Transposition Tables (TT):** Implement a hash table to cache evaluated positions and improve search depth.
- [ ] **Opening Books:** Expand and refine opening book support (e.g., polyglot implementation completeness).
- [ ] **Endgame Tablebases ("Closing Books"):** Integrate Syzygy or Gaviota tablebase support for perfect play in the endgame.

## Evaluation Improvements
- [ ] **Pawn Structure:** Enhance evaluation heuristics for pawn structures (e.g., better handling of doubled, isolated, and passed pawns).
- [ ] **Castling & King Safety:** Improve evaluation regarding castling rights, king pawn shield, and king safety in the middlegame.
- [ ] **Automated Tuning:** Implement a framework (e.g., Texel Tuning) to optimize evaluation weights as mentioned in `EVALUATION.md`.

## Refinements & Maintenance
- [ ] **Time Management:** Refine time allocation logic during the search phase to better handle varying time controls.
- [ ] **Documentation Update:** Update `README.md` to reflect current progress, as some items under "Future Focus" are partially or fully implemented.
