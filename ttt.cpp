#include"ttt.h"

int change(int turn) {
	return -turn;
}

board::board(int s)
{
	size = s;
	data = std::vector<int>(s*s, 0);
	turn = -1;
}

board::board() {
	size = 3;
	data = std::vector<int>(9, 0);
	turn = -1;
}

board::board(std::vector<int> board) {
	data = board;
	size = (int)std::sqrt((double)data.size());
	turn = -1;
}
board* board::set(int r, int c, int val)
{
	data.at(r * size + c) = val;
	return this;
}

board* board::change_turn() {
	turn = change(turn);
	return this;
}

int board::get(int r, int c)
{
	return data.at(r * size + c);
}

int board::get_turn()
{
	return turn;
}

board* board::print()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (data.at(i * size + j) == -1)
				std::cout << "X";
			else if (data.at(i * size + j) == 1)
				std::cout << "O";
			else if (data.at(i * size + j) == 0)
				std::cout << ".";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return this;
}

bool board::is_valid_move(int r, int c)
{
	if (r < size && c < size) return (data.at(r * size + c) == 0) ? true : false;
	else return false;
}

std::vector<std::vector<int>> board::get_valid_locations()
{
	std::vector<std::vector<int>> out;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (is_valid_move(i, j))
			{
				out.push_back({ i,j });
			}
		}
	}
	return out;
}

std::vector<int> board::search_direction(int r_s, int c_s, int r_d, int c_d,int distance)
{
	std::vector<int> out;

	for (int i = 0; ((r_s + r_d * i) < size) && ((r_s + r_d * i) > -1) && ((c_s + c_d * i) < size ) && ((c_s + c_d * i) > -1) && (i < distance+1); i++)
		out.push_back(data.at((r_s + r_d * i) * size + c_s + c_d * i));
	return out;
}

std::vector<int> board::get_end(int r_s, int c_s, int r_d, int c_d, int distance)
{
	std::vector<int> out = { r_s,c_s };
	for (int i = 1; ((r_s + r_d * i) < size) && ((r_s + r_d * i) > -1) && ((c_s + c_d * i) < size) && ((c_s + c_d * i) > -1) && (i<distance +1); i++)
		out = { r_s + r_d * i,c_s + c_d * i };
	return out;
}

std::vector<std::vector<int>> board::get_major_dirs()
{
	std::vector<std::vector<int>> dirs;
	dirs.push_back({ -1,1 });
	dirs.push_back({ 1,1 });
	dirs.push_back({ 0,1 });
	dirs.push_back({ 1,0 });
	return dirs;
}

std::vector<std::vector<int>> board::get_involved_dirs(int r, int c)
{
	std::vector<std::vector<int>> major_dirs = get_major_dirs();
	std::vector<std::vector<int>> dirs;
	for (auto& i : major_dirs)
	{
		std::vector<int> start = get_end(r, c, -i.at(0), -i.at(1),2);
		std::vector<int> dir = search_direction(start.at(0), start.at(1), i.at(0), i.at(1),4);
		dirs.push_back(dir);
	}
	return dirs;
}

int board::is_end(int r, int c)
{
	std::vector<std::vector<int>> dirs =get_involved_dirs(r,c);
	for (auto& i : dirs)
	{
		int prev = 0;
		int len = 0;
		for (auto& j : i) {
			if (j != 0)
			{
				if (j == prev)
				{
					len += 1;
					if (len == to_lose)
					{
						return prev;
					}
				}
				else
				{
					prev = j;
					len = 1;
				}
			}
			else {
				prev = 0;
				len = 0;
			}
		}
	}
	return 0;
}

board* board::play_turn(int r, int c)
{
	if (is_valid_move(r, c))
	{
		set(r, c, turn);
	}
	return this;
}

engine::engine(player* player1,player* player2) {
	b = new board;
	p1 = player1;
	p2 = player2;
}

engine::engine(player* player1, player* player2,int size) {
	b = new board(size);
	p1 = player1;
	p2 = player2;
}

engine::engine(board* bo, player* player1, player* player2) {
	b = bo;
	p1 = player1;
	p2 = player2;
}

void engine::print_board()
{
	b->print();
}

player* engine::get_player()
{
	if (b->get_turn() == -1)
	{
		return p1;
	}
	else {
		return p2;
	}
}

int engine::game_loop(bool print)
{
	while (1)
	{
		if(print)b->print();
		std::vector<int> move = get_player()->get_turn(b);
		b->play_turn(move[0], move[1]);
		if (int i = b->is_end(move[0],move[1])) return i;
		else if (b->get_valid_locations().size() == 0) return 2;
		b->change_turn();
	}
}

engine::~engine() {
	delete p1;
	delete p2;
	delete b;
}

player::~player() {
}

std::vector<int> human_player::get_turn(board* b)
{
	int r, c;
	while (1) {
		std::cin >> r >> c;
		if(b->is_valid_move(r,c)) return { r, c };
	}
}

human_player::~human_player()
{

}

minimax_player::minimax_player(int d)
{
	depth = d;
}

std::vector<int> minimax_player::minimax(board* b,int dep, int alpha, int beta,bool maximizing, int prev_row, int prev_col) {
	if (prev_row != -1) {
		if (int i = b->is_end(prev_row, prev_col))
		{
			if (i == change(b->get_turn())) return maximizing ? std::vector<int>({ 1, -1,-1 }) : std::vector<int>({ -1, -1,-1 });
			else return { 0,-1,-1 };
		}
		if (dep == 0) return{ 0,-1, -1 };
	}
	int val;
	std::vector<int> move;
	if (maximizing)
	{
		val = -2;
		std::vector<std::vector<int>> possible_moves = b->get_valid_locations();
		if (possible_moves.size() == 0) return { 0,-1,-1 };
		move = possible_moves[0];

		for (auto m : possible_moves)
		{
			int r = m[0], c = m[1];
			b->play_turn(r, c);
			int new_val = minimax(b->change_turn(), dep - 1, alpha, beta, false, r, c)[0];
			b->change_turn();
			b->set(r, c, 0);
			if (new_val > val)
			{
				val = new_val;
				move = {r,c};
			}
			alpha = std::max(alpha, val);
			if (alpha >= beta) break;
		}
	}
	else {
		val = 2;
		std::vector<std::vector<int>> possible_moves = b->get_valid_locations();
		if (possible_moves.size() == 0) return { 0,-1,-1 };
		move = possible_moves[0];

		for (auto m : possible_moves)
		{
			int r = m[0], c = m[1];
			b->play_turn(r, c);
			int new_val = minimax(b->change_turn(), dep - 1, alpha, beta, true, r, c)[0];
			b->change_turn();
			b->set(r, c, 0);
			if (new_val < val)
			{
				val = new_val;
				move = { r,c };
			}
			beta = std::min(beta, val);
			if (alpha >= beta) break;
		}
	}
	return { val,move[0],move[1] };
}

std::vector<int> minimax_player::get_turn(board* b)
{
	std::vector<int> val_move = minimax(b, depth, -2, 2, true, -1, -1);
	return { val_move[1],val_move[2] };
}

minimax_player::~minimax_player()
{
}


void play(char player)
{
	int size = 0;
	std::cout << "please enter what size board you'd like to play on:";
	while (size < 3)
	{
		std::cin >> size;
		if (size < 3)std::cout << "try again";
	}
	int depth = 0;
	std::cout << "please enter what depth of minimax you'd like to play against:";
	while (depth <1)
	{
		std::cin >> depth;
		if (size < 1)std::cout << "try again";
	}

	engine* game = new engine(new minimax_player(depth), new human_player, size);

	if (player == 'X')
	{
		game = new engine(new human_player, new minimax_player(depth), size);
	}
	std::cout << "you play by typing in 2 numbers : the first being the row you want to play in, second one the column" << "\n";
	int result = game->game_loop(true);
	game->print_board();
	switch (result)
	{
	case(-1):
		if (player != 'X') std::cout << "you won!";
		else std::cout << "haha you lost!";
		break;
	case(1):
		if (player == 'X') std::cout << "you won!";
		else std::cout << "haha you lost!";
		break;
	case(2):
		std::cout << "draw";
		break;
	}
}

void play(int num)
{
	while (num > 0)
	{
		std::vector<int> data(9,0);
		int num_x = 0;
		int num_o = 0;
		for (int i = 0; i < 9; i++)
		{
			char next;
			std::cin >> next;
			if (next == 'x')
			{ 
				data[i] = -1;
				num_x++;
			}
			else if (next == 'o')
			{
				data[i] = 1;
				num_o++;
			}
		}
		board* b = new board(data);
		if (num_x > num_o)
		{
			b->change_turn();
		}
		engine* game = new engine(b, new minimax_player(9), new minimax_player(9));
		int result = game->game_loop(false);
		switch (result)
		{
		case -1:
			std::cout << "O";
			break;
		case 1:
			std::cout << "X";
			break;
		case 2:
			std::cout << "N";
			break;
		}
		num--;
		delete game;
	}
}

int main() {
	std::string s;
	std::cin >> s;
	if (isdigit(s[0]))
	{
		play(stoi(s));
	}
	else
	{
		play(s[0]);
	}
	return 0;
};