#ifndef __MAP__
#define __MAP__

#include "all_header.h"
#include <bits/stdc++.h>

using namespace std;

class Map
{
public:
    int mapId;
    int numRows;
    int numCols;
    vector<vector<int>> terrain;

    //constructor
    Map(int id, int row = 64, int col = 64);

    void getTerrain(bool uniform = true);
};

#endif