#include"ttt.h"

board::board(int starting_turn, int s)
{
	size = s;
	for (int i = 0; i < size; i++)
		data.push_back(0);
	turn = starting_turn;
}

board::board() {
	size = 3;
	data = { 0,0,0,0,0,0,0,0,0 };
	turn = BLACK;
}
board* board::set(int r, int c, int val)
{
	data.at(r * size + c) = val;
	return this;
}

int board::get(int r, int c)
{
	return data.at(r * size + c);
}

board* board::print()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (data.at(i * size + j) == -1)
				std::cout << "x";
			else if (data.at(i * size + j) == 1)
				std::cout << "o";
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
	return (data.at(r * size + c) == 0) ? true : false;
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

std::vector<int> board::search_direction(int r_s, int c_s, int r_d, int c_d)
{
	std::vector<int> out;

	for (int i = 1; ((r_s + r_d * i) < size)&& ((r_s + r_d * i) > -1) && ((c_s + c_d * i) < size )&& ((c_s + c_d * i) > -1); i++)
		out.push_back(data.at((r_s + r_d * i) * size + c_s + c_d * i));
	return out;
}

std::vector<int> board::get_end(int r_s, int c_s, int r_d, int c_d)
{
	std::vector<int> out = { r_s,c_s };
	for (int i = 1; ((r_s + r_d * i) < size) && ((r_s + r_d * i) > -1) && ((c_s + c_d * i) < size) && ((c_s + c_d * i) > -1); i++)
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
		std::vector<int> start = get_end(r, c, -i.at(0), -i.at(1));
		std::vector<int> dir = search_direction(start.at(0), start.at(1), i.at(0), i.at(1));
		dir.insert(dir.begin(),  get(r,c));
		dirs.push_back(dir);
	}
	return dirs;
}

bool board::is_end(int r, int c)
{
	std::vector<std::vector<int>> dirs = get_involved_dirs(r,c);

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
						return true;
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
	return false;
}
int main() {
	board* b = new board();
	std::cout << b->print()->is_end(0,0)<<std::endl;
	b->set(0, 0, 1)->set(1,1,1)->set(2,2,1)->print();
	std::cout << b->is_end(0,0);
	
	return 0;
};