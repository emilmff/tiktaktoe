#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
int change_turn(int turn);

class board {
	std::vector<int> data;
	int size;
	int turn;
	static constexpr int to_lose = 3;
public:
	board();
	board(int s);
	board(std::vector<int>);
	board* set(int r, int c, int val);
	board* change_turn();
	int get(int r, int c);
	int get_turn();
	board* print();
	bool is_valid_move(int r, int c);
	std::vector<std::vector<int>> get_valid_locations();
	std::vector<int> search_direction(int, int, int, int, int);
	static std::vector<std::vector<int>> get_major_dirs();
	std::vector<std::vector<int>> get_involved_dirs(int, int);
	std::vector<int> get_end(int, int, int, int,int );
	int is_end(int,int);
	board* play_turn(int r, int c);
};

class player;

class engine {
	board* b;
	player* p1;
	player* p2;
public:
	engine(player*,player*);
	engine(player*, player*,int);
	engine(board*, player*, player*);
	engine(int starting_turn, int s);

	void print_board();
	player* get_player();
	int game_loop(bool print);
	~engine();
};

class player{
public:
	virtual std::vector<int> get_turn(board*) = 0;
	virtual ~player();
};

class human_player : public player {
public:
	virtual std::vector<int> get_turn(board*);
	~human_player();
};

class minimax_player : public player {
	int depth;
	std::vector<int> minimax(board*,int,int,int,bool,int,int);
public:
	minimax_player(int);
	virtual std::vector<int> get_turn(board*);
	~minimax_player();
};

void play(char);
void play(int);