# Evaluation
**Basic Material Evaluation**
The most fundamental step is calculating the Material Balance. We iterate through all the pieces and sum their values (using the constants we just defined).
  * Formula: (White Material - Black Material)
  * Perspective: Ensure the final score is relative to the side currently moving.

**Piece-Square Tables**
Pieces have different values depending on where they stand. For example, a Knight is much stronger in the center than in a corner. We need to create 8x8 arrays for each
piece type that assign a bonus or penalty to every square on the board.
  * Knights: Bonus for central squares.
  * Pawns: Bonus for advancing toward promotion.
  * Kings: Penalty for being in the center during the middlegame, but a bonus for being central in the endgame.

**Tapered Evaluation (Game Phase)**
A chess engine evaluates positions differently in the Middlegame versus the Endgame.
  * We need a way to calculate the Game Phase (usually based on how much material is left).
  * We then calculate two scores: a Middlegame score and an Endgame score.
  * The final evaluation is an interpolation between the two based on the current phase.

**Positional Heuristics**
Once material and PST are in place, we add specific chess knowledge:
  * Pawn Structure: Penalize doubled or isolated pawns; reward passed pawns.
  * Mobility: Reward pieces (especially bishops and rooks) for having more legal moves available.
  * King Safety: Evaluate the pawn shield in front of the king and the number of enemy attackers nearby.
  * Bishop Pair: Giving a small bonus if a side has both bishops.

**Efficient Updates (Incremental Evaluation)**
Instead of recalculating everything from scratch every time evaluate() is called, we can update the score incrementally inside of make_move and unmake_move functions. This is a significant performance optimization.

**Tuning**
Finally, we will need to tune the weights. If our engine values a Knight too highly compared to a Bishop, it will make poor trades. This is usually done by testing the
engine against other versions of itself or using automated tuning tools (like Texel tuning).
