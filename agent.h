#ifndef __AGENT__
#define __AGENT__

#include <bits/stdc++.h>
#include "direction_map.h"
#include "all_header.h"

using namespace std;

void export_dm(Direction_Map& DM);
void export_terrain(vvi &terrain);
double getCost(pi currPos, pi direction, Direction_Map& DM, vvi & terrain, double weight = WEIGHT);
bool isEqual(pi one, pi two);
double calculate_dot(Direction_Vector one, Direction_Vector two);
class Agent
{
public:
    int agentId;
    pair<int, int> startingPos;
    pair<int, int> goalPos;
    pair<int, int> currPos;

    //constructor
    Agent(int id, pi start, pi goal);

    // This function updates the Position of the current Agent to the new position
    // along with the updation of direction map.
    // Firstly, it will check if the flag is true or false. (False implies, we need to calculate the best move)
    // True implies that we have the most optimal path and we just need to update the position and direction map
    void updatePos(vector<vector<int>> &terrain, Direction_Map &DM, bool flag = false, pair<int,int> direction = {0,0});

    // This function will return the vector of directions
    // representing the optimal path considering this agent as the only agent in the map.
    vpi getOptimalPath(vvi & terrain, Direction_Map& DM, pi start_pos, pi end_pos, double weight = WEIGHT);
    //return type was vpi earlier.
private:
    // This function returns a pair of direction(int) and cost(double) where the int assumes 0,1,2,3 values to
    // denote the directions left, up, right, down, respectively.
    // This function calculates all the costs for the 4 possible moves and return the move with the least cost.
    // Testing phase
    pair<pair<int,int>, double> getMinMoveCost(vector<vector<int>>& terrain, Direction_Map DM);    
    
    // updates the direction for both the cells: startPos and endPos.
    void updateDirectionMap(pair<int,int> startPos, pair<int,int> endPos, vvi &terrain, Direction_Map &DM);
    
};

#endif