
#include "make_maze.h"

int maze_size;
bool*** maze;
int*** maze_route_i;
int*** maze_wilson;

void get_size_of_maze(int max_size, int min_size)
{
	// �Ҵ� ����
	if (maze != NULL)
	{
		for (int z = 0; z < maze_size; z++)
		{
			for (int y = 0; y < maze_size; y++)
				delete[] maze[z][y];
			delete[] maze[z];
		}
		delete[] maze;
		maze = NULL;
	}
	if (maze_route_i != NULL)
	{
		for (int z = 0; z < maze_size; z++)
		{
			for (int y = 0; y < maze_size; y++)
				delete[] maze_route_i[z][y];
			delete[] maze_route_i[z];
		}
		delete[] maze_route_i;
		maze_route_i = NULL;
	}
	if (maze_wilson != NULL)
	{
		for (int z = 0; z < maze_size / 2; z++)
		{
			for (int y = 0; y < maze_size / 2; y++)
				delete[] maze_wilson[z][y];
			delete[] maze_wilson[z];
		}
		delete[] maze_wilson;
		maze_wilson = NULL;
	}
	// �� �Է�
	std::cout << "�̷��� ���������մϴ�" << std::endl;
	std::cout << "�̷��� ����� " << min_size << " ~ " << max_size << " ������ �Է��� �ּ���" << std::endl;
	std::cin >> maze_size;
	if (maze_size < min_size || maze_size > max_size)
	{
		printf("�ٽ� �Է����ּ���. ���� �Է� Ű%d\n", maze_size);
		get_size_of_maze(max_size, min_size);
		return;
	}

	printf("Maze size = %d\n", maze_size);

	// �Ҵ� ����
	if (maze_wilson == NULL)
	{
		maze_wilson = new int** [maze_size];
		for (int z = 0; z < maze_size; z++)
		{
			maze_wilson[z] = new int* [maze_size];
			for (int y = 0; y < maze_size; y++)
				maze_wilson[z][y] = new int[maze_size];
		}
	}

	maze_size = maze_size * 2 + 1;

	if (maze == NULL)
	{
		maze = new bool** [maze_size];
		for (int z = 0; z < maze_size; z++)
		{
			maze[z] = new bool* [maze_size];
			for (int y = 0; y < maze_size; y++)
				maze[z][y] = new bool[maze_size];
		}
	}
	if (maze_route_i == NULL)
	{
		maze_route_i = new int** [maze_size];
		for (int z = 0; z < maze_size; z++)
		{
			maze_route_i[z] = new int* [maze_size];
			for (int y = 0; y < maze_size; y++)
				maze_route_i[z][y] = new int[maze_size];
		}
	}
}
void get_size_of_maze(int mazeSize)
{
	// �Ҵ� ����
	if (maze != NULL)
	{
		for (int z = 0; z < maze_size; z++)
		{
			for (int y = 0; y < maze_size; y++)
				delete[] maze[z][y];
			delete[] maze[z];
		}
		delete[] maze;
		maze = NULL;
	}
	if (maze_route_i != NULL)
	{
		for (int z = 0; z < maze_size; z++)
		{
			for (int y = 0; y < maze_size; y++)
				delete[] maze_route_i[z][y];
			delete[] maze_route_i[z];
		}
		delete[] maze_route_i;
		maze_route_i = NULL;
	}
	if (maze_wilson != NULL)
	{
		for (int z = 0; z < maze_size / 2; z++)
		{
			for (int y = 0; y < maze_size / 2; y++)
				delete[] maze_wilson[z][y];
			delete[] maze_wilson[z];
		}
		delete[] maze_wilson;
		maze_wilson = NULL;
	}
	maze_size = mazeSize;

	// �Ҵ� ����
	if (maze_wilson == NULL)
	{
		maze_wilson = new int** [maze_size];
		for (int z = 0; z < maze_size; z++)
		{
			maze_wilson[z] = new int* [maze_size];
			for (int y = 0; y < maze_size; y++)
				maze_wilson[z][y] = new int[maze_size];
		}
	}

	maze_size = maze_size * 2 + 1;

	if (maze_route_i == NULL)
	{
		maze_route_i = new int** [maze_size];
		for (int z = 0; z < maze_size; z++)
		{
			maze_route_i[z] = new int* [maze_size];
			for (int y = 0; y < maze_size; y++)
				maze_route_i[z][y] = new int[maze_size];
		}
	}

	if (maze == NULL)
	{
		maze = new bool** [maze_size];
		for (int z = 0; z < maze_size; z++)
		{
			maze[z] = new bool* [maze_size];
			for (int y = 0; y < maze_size; y++)
				maze[z][y] = new bool[maze_size];
		}
	}
}

void print_maze()
{
	std::cout << "print_maze"<< std::endl;

	for (int z = 0; z < maze_size; z++)
	{
		std::cout << "start------------ z : " << z << std::endl;
		for (int y = 0; y < maze_size; y++)
		{
			for (int x = 0; x < maze_size; x++)
			{
				//if (maze[z][y][x] == false)
				//	std::cout << "  ";
				//else
				//	std::cout << "��";
				if(maze_route_i[z][y][x] == -1)
					std::cout << "    ";
				else
					printf("%4d", maze_route_i[z][y][x]);
			}
			std::cout << std::endl;
		}
		std::cout << "end------------ z : "<< z << std::endl;
	}
}

void print_maze_wilson()
{
	std::cout << "print_maze_wilson" << std::endl;

	for (int z = 0; z < maze_size / 2; z++)
	{
		std::cout << "start------------ z : " << z << std::endl;
		for (int y = 0; y < maze_size / 2; y++)
		{
			for (int x = 0; x < maze_size / 2; x++)
				std::cout << maze_wilson[z][y][x] << " ";
			std::cout << std::endl;
		}
		std::cout << "end------------ z : " << z << std::endl;
	}
}


void init_maze()
{
	for (int z = 0; z < maze_size; z++)
	{
		for (int y = 0; y < maze_size; y++)
		{
			for (int x = 0; x < maze_size; x++)
			{
				if (z % 2 == 0 || y % 2 == 0 || x % 2 == 0)
					maze[z][y][x] = true;
				else
					maze[z][y][x] = false;
				maze_route_i[z][y][x] = -1;
			}
		}
	}
}

void init_maze_wilson()
{
	// 0 = Ȯ��
	// 1 = �ʱⰪ
	// 4 = ��
	// 8 = ��
	// 6 = ��
	// 2 = ��
	// 9 = ��
	// 3 = ��
	for (int z = 0; z < maze_size / 2; z++)
		for (int y = 0; y < maze_size / 2; y++)
			for (int x = 0; x < maze_size / 2; x++)
				maze_wilson[z][y][x] = 1;
}


bool make_maze_wilson()
{
	if (maze_wilson == NULL || maze == NULL)
	{
		std::cout << "�̷��� ũ�Ⱑ �������� �ʾҴ�. G�� ������ JOY�� ǥ�Ͻÿ�" << std::endl;
		return false;;
	}

	init_maze();
	init_maze_wilson();
	int route_count = 0;

	int decide_count = 0; // Ȯ�� ī��Ʈ

	int loop_count; // ����Ƚ��

	// ������ ĭ �ϳ� Ȯ�� - ���⿡ �߾��� ���� ������ ���̵��� ��������

	int first_x = rand() % (maze_size / 2);
	int first_y = rand() % (maze_size / 2);
	int first_z = rand() % (maze_size / 2);

	maze_wilson[first_z][first_y][first_x] = 0;
	decide_count++;
	

	int current_x;
	int current_y;
	int current_z;
	//print_maze_wilson();

	int new_x;
	int new_y;
	int new_z;
	// �ݺ� ����
	while (decide_count < (maze_size / 2) * (maze_size / 2) * (maze_size / 2))
	{
		loop_count = 0;

		// ���ο� ��Ȯ�� ������ ĭ �ϳ� ����
		new_x = rand() % (maze_size / 2);
		new_y = rand() % (maze_size / 2);
		new_z = rand() % (maze_size / 2);

		if (maze_wilson[new_z][new_y][new_x] != 0)
		{
			current_x = new_x;
			current_y = new_y;
			current_z = new_z;
		}
		else
			continue;

		// �� ĭ ���� ������ �������� �����̸� ������ ������ ���
		while (true)
		{
			loop_count++;
			// ���� ���� ���� ����
			if (loop_count < MAX_LOOF_COUNT)
			{
				switch (rand() % 6)
				{
				case 0:
					maze_wilson[current_z][current_y][current_x] = 4;
					break;
				case 1:
					maze_wilson[current_z][current_y][current_x] = 8;
					break;
				case 2:
					maze_wilson[current_z][current_y][current_x] = 6;
					break;
				case 3:
					maze_wilson[current_z][current_y][current_x] = 2;
					break;
				case 4:
					maze_wilson[current_z][current_y][current_x] = 9;
					break;
				case 5:
					maze_wilson[current_z][current_y][current_x] = 3;
					break;
				}
			}
			else
			{
				// ���� Ƚ���� �ʰ��ϸ� �׳� �� �޸���.
				if (current_x > first_x)
					maze_wilson[current_z][current_y][current_x] = 4;
				else if (current_x < first_x)
					maze_wilson[current_z][current_y][current_x] = 6;
				else if (current_y > first_y)
					maze_wilson[current_z][current_y][current_x] = 8;
				else if (current_y < first_y)
					maze_wilson[current_z][current_y][current_x] = 2;
				else if (current_z > first_z)
					maze_wilson[current_z][current_y][current_x] = 3;
				else if (current_z < first_z)
					maze_wilson[current_z][current_y][current_x] = 9;
			}
			// �� ������ �������� �ϸ� �ݺ�
			if (current_x == 0 && maze_wilson[current_z][current_y][current_x] == 4)
				continue;
			if (current_x == maze_size / 2 - 1 && maze_wilson[current_z][current_y][current_x] == 6)
				continue;
			if (current_y == 0 && maze_wilson[current_z][current_y][current_x] == 8)
				continue;
			if (current_y == maze_size / 2 - 1 && maze_wilson[current_z][current_y][current_x] == 2)
				continue;
			if (current_z == 0 && maze_wilson[current_z][current_y][current_x] == 3)
				continue;
			if (current_z == maze_size / 2 - 1 && maze_wilson[current_z][current_y][current_x] == 9)
				continue;
			// Ȯ��ĭ�� ������ ���� ����
			if (maze_wilson[current_z][current_y][current_x] == 4 && maze_wilson[current_z][current_y][current_x - 1] == 0)
				break;
			if (maze_wilson[current_z][current_y][current_x] == 8 && maze_wilson[current_z][current_y - 1][current_x] == 0)
				break;
			if (maze_wilson[current_z][current_y][current_x] == 6 && maze_wilson[current_z][current_y][current_x + 1] == 0)
				break;
			if (maze_wilson[current_z][current_y][current_x] == 2 && maze_wilson[current_z][current_y + 1][current_x] == 0)
				break;
			if (maze_wilson[current_z][current_y][current_x] == 9 && maze_wilson[current_z + 1][current_y][current_x] == 0)
				break;
			if (maze_wilson[current_z][current_y][current_x] == 3 && maze_wilson[current_z - 1][current_y][current_x] == 0)
				break;
			// �ƴϸ� ���� ĭ���� �̵�
			switch (maze_wilson[current_z][current_y][current_x])
			{
			case 4:
				current_x--;
				break;
			case 8:
				current_y--;
				break;
			case 6:
				current_x++;
				break;
			case 2:
				current_y++;
				break;
			case 9:
				current_z++;
				break;
			case 3:
				current_z--;
				break;
			}
			//print_maze_wilson();
		}
		// Ȯ��ĭ�� ������ ������������ Ȯ������ �����ϸ鼭 Ȯ��

		loop_count = 0;
		while (true)
		{
			loop_count++;// �̰� ���⿡ �� �־�
			//std::cout << loop_count << "> ���� ��ǥ : " << tmp_w << ", " << tmp_h << ", ���� : " << maze_wilson[tmp_h][tmp_w] << std::endl;
			//std::cout << loop_count << std::endl;
			//print_maze_wilson();
			// ���� ��ġ�� Ȯ���̸� ���� ����
			if (maze_wilson[new_z][new_y][new_x] == 0)
				break;
			// ���� ��ġ���� ���� ������ ���� �νŴ�.
			maze_route_i[new_z * 2 + 1][new_y * 2 + 1][new_x * 2 + 1] = route_count++;

			switch (maze_wilson[new_z][new_y][new_x])
			{
			case 4:
				maze[new_z * 2 + 1][new_y * 2 + 1][new_x * 2] = false;
				maze_route_i[new_z * 2 + 1][new_y * 2 + 1][new_x * 2] = route_count++;
				break;
			case 8:
				maze[new_z * 2 + 1][new_y * 2][new_x * 2 + 1] = false;
				maze_route_i[new_z * 2 + 1][new_y * 2][new_x * 2 + 1] = route_count++;
				break;
			case 6:
				maze[new_z * 2 + 1][new_y * 2 + 1][new_x * 2 + 2] = false;
				maze_route_i[new_z * 2 + 1][new_y * 2 + 1][new_x * 2 + 2] = route_count++;
				break;
			case 2:
				maze[new_z * 2 + 1][new_y * 2 + 2][new_x * 2 + 1] = false;
				maze_route_i[new_z * 2 + 1][new_y * 2 + 2][new_x * 2 + 1] = route_count++;
				break;
			case 9:
				maze[new_z * 2 + 2][new_y * 2 + 1][new_x * 2 + 1] = false;
				maze_route_i[new_z * 2 + 2][new_y * 2 + 1][new_x * 2 + 1] = route_count++;
				break;
			case 3:
				maze[new_z * 2][new_y * 2 + 1][new_x * 2 + 1] = false;
				maze_route_i[new_z * 2][new_y * 2 + 1][new_x * 2 + 1] = route_count++;
				break;
			}
			//print_maze();
			// ���� ��ġ�� Ȯ���Ѵ�.
			// ���� ��ġ�� �ٲ۴�.
			switch (maze_wilson[new_z][new_y][new_x])
			{
			case 4:
				new_x--;
				maze_wilson[new_z][new_y][new_x + 1] = 0;
				break;
			case 8:
				new_y--;
				maze_wilson[new_z][new_y + 1][new_x] = 0;
				break;
			case 6:
				new_x++;
				maze_wilson[new_z][new_y][new_x - 1] = 0;
				break;
			case 2:
				new_y++;
				maze_wilson[new_z][new_y - 1][new_x] = 0;
				break;
			case 9:
				new_z++;
				maze_wilson[new_z - 1][new_y][new_x] = 0;
				break;
			case 3:
				new_z--;
				maze_wilson[new_z + 1][new_y][new_x] = 0;
				break;
			}
			decide_count++;
		}
	}
	// ��Ȯ��ĭ�� ������ ��
	std::cout << "root_count : " << route_count << std::endl;
	return true;
}
