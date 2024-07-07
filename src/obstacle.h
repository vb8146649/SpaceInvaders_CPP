#pragma once
#include<vector>
#include "block.h"
using namespace std;
class Obstacle{
    public:
        Obstacle(Vector2 possition);
        void Draw();
        Vector2 position;
        vector<Block> blocks;
        static vector<vector<int>> grid;
    private:
};  