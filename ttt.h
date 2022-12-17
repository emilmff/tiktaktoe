#pragma once
#include <iostream>
#include <vector>

int change_turn(int turn);

class board {
	std::vector<int> data;
	int size;
	int turn;
	static constexpr int to_lose = 3;
	static constexpr int WHITE = 1;
	static constexpr int BLACK = -1;
public:
	board();
	board(int starting_turn, int s);
	board* set(int r, int c, int val);
	int get(int r, int c);
	board* print();
	bool is_valid_move(int r, int c);
	std::vector<std::vector<int>> get_valid_locations();
	std::vector<int> search_direction(int, int, int, int);
	static std::vector<std::vector<int>> get_major_dirs();
	std::vector<std::vector<int>> get_involved_dirs(int, int);
	std::vector<int> get_end(int, int, int, int);
	bool is_end(int,int);
	board* play_turn(int r, int c);
};

class engine {
	board* b;

	engine();
	engine(int starting_turn, int s);

	int game_loop();
	std::vector<int> get_turn();
};

class player{
protected:	
	int color;
	player(int c);
	virtual std::vector<int> get_turn() = 0;
};

class human_player : public player {
	human_player(int c);
	virtual std::vector<int> get_turn();
};
class minimax {

};