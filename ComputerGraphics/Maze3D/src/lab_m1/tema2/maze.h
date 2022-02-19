#pragma once

#include <vector>
#include <iostream>
#include <random>

namespace m1
{
    enum space {
        EMPTY = 0,
        WALL  = 1,
        ENEMY = 2,
        START = 3,
        EXIT  = 4
    };

    class Maze {

    public:
        Maze() {
            size = 20;
            map = std::vector<std::vector<space>>(size, std::vector<space>(size));

            std::random_device rd;
            rand = std::mt19937(rd());
            enemy_chance = 2000 / (size * size / 2);

            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    map[i][j] = WALL;

            start = exit = std::pair<int, int>(1, 1);
            build_maze(start);

            map[start.first][start.second] = START;
            map[exit.first][exit.second] = EXIT;

            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    char c = ' ';
                    if (map[i][j] == WALL)
                        c = '#';
                    if (map[i][j] == EXIT)
                        c = 'X';
                    if (map[i][j] == START)
                        c = 'S';
                    if (map[i][j] == ENEMY)
                        c = 'O';

                    std::cout << c;
                }
                std::cout << "\n";
            }
        }

        ~Maze() {
            std::cout << "God dammit Bjarne!\n";
        };

        std::vector<std::vector<space>> map;
        std::pair<int, int> start, exit;
        int size;

    private:
        std::mt19937 rand;
        int enemy_chance;

        void build_maze(std::pair<int, int> x) {
            if (x.first == 0 || x.first == size-1 || x.second == 0 || x.second == size-1)
                return;

            if (countNeighs(x) > 1)
               return; 

            map[x.first][x.second] = EMPTY;
            if (x.first + x.second > exit.first + exit.second) {
                exit = x;
            } else {
                if (rand() % 100 < enemy_chance)
                    map[x.first][x.second] = ENEMY;
            }

            auto directions = std::vector<std::pair<int, int>>{{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
            std::shuffle(directions.begin(), directions.end(), rand);

            for (auto dx : directions) {
                build_maze(add(x, dx));
            }
        }

        int countNeighs(std::pair<int, int> x) {
            int count = 0;

            for (auto dx : std::vector<std::pair<int, int>>{{0, 1}, {0, -1}, {-1, 0}, {1, 0}}) {
                if (map[x.first + dx.first][x.second + dx.second] != WALL)
                    count += 1;
            }

            return count;
        }

        std::pair<int, int> add(std::pair<int, int> x, std::pair<int, int> y) {
            return std::pair<int,int>(x.first + y.first, x.second + y.second);
        }
    };
}
