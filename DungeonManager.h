#pragma once

class DungeonManager {

public:
  DungeonManager();
  ~DungeonManager();
  const int get_player_row() const;
  const int get_player_col() const;
  const int get_cell(const int row, const int col) const;
  const int get_gridsize() const;

  void set_player_row(const int row);
  void set_player_col(const int col);
  void incr_player_row();
  void decr_player_row();
  void incr_player_col();
  void decr_player_col();
  void set_player_position(const int row, const int col);

private:
  int player_row = 1;
  int player_col = 0;

  const int gridsize = 32;
  int grid[32][32] = {0};
};
