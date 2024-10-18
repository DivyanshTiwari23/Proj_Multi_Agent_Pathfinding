#ifndef __DIRECTION_MAP__
#define __DIRECTION_MAP__

#include <bits/stdc++.h>
using namespace std;

//first is i_component, second is j_component
#define Direction_Vector pair<double, double> 

class Direction_Map{
    public:
    int numRows;
    int numCols;
    vector<vector<Direction_Vector>> dirMap;
};

#endif