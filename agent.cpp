#include "agent.h"
#include "all_header.h"
#include <bits/stdc++.h>

pair<double,double> rotate_90_anti(pair<double,double> dir)
{
    pair<double,double> temp;
    temp.first = -(dir.second);
    temp.second = dir.first;
    return temp;
}
void export_dm(Direction_Map& DM)
{   
    string filename;
    cout << "Enter file name for DM : ";
    cin >> filename;
    filename+=".csv";
    filename = "./helper/" + filename;
    ofstream output(filename);
    output << "i_vector , j_vector" << endl;
    output << DM.dirMap.size() << "," << DM.dirMap[0].size() << endl;

    //due to python code generating 90 degree rotated figure, need to tilt the DMs too.
    for(auto x : DM.dirMap)
    {
        for(auto y : x)
        {
            pair<double,double> dir = rotate_90_anti(y);
            output << dir.first << "," << dir.second << endl;
        }
    }
    output.close();
}

void export_terrain(vvi &terrain)
{
    string filename;
    cout << "Enter filename for terrain : ";
    cin >> filename;
    filename+=".csv";
    filename = "./helper/" + filename;
    ofstream output(filename);

    for(auto x : terrain)
    {
        for(auto y : x)output << y << ",";
        output << endl;
    }
    output.close();
}

pi convert_to_dv(pi Vm)
{
    if(Vm.second == 0)
    {
        Vm.second = -Vm.first;
        Vm.first = 0;
    }
    else
    {
        Vm.first = Vm.second;
        Vm.second = 0;
    }
    return Vm;
}


void Agent::updateDirectionMap(pair<int,int> startPos, pair<int,int> endPos, vvi& terrain, Direction_Map &DM)
{
    Direction_Vector Va = DM.dirMap[startPos.first][startPos.second];
    Direction_Vector Vb = DM.dirMap[endPos.first][endPos.second];
    Direction_Vector Vm;
    Vm.first = endPos.first - startPos.first;
    Vm.second = endPos.second - startPos.second;
    Vm = convert_to_dv(Vm);
    Direction_Vector Va_new, Vb_new;
    // double x = calculate_dot(Va_new, Vb_new);
    if(Va.first == 0 && Va.second == 0)
    {
        Va_new = Vm; 
    }
    else
    {
        Va_new.first = alpha*Va.first + (1 - alpha)*Vm.first;
        Va_new.second = alpha*Va.second + (1 - alpha)*Vm.second;
    }

    if(Vb.first == 0 && Vb.second == 0)
    {
        Vb_new = Vm;
    }
    else
    {
        Vb_new.first = alpha*Vb.first + (1 - alpha)*Vm.first;
        Vb_new.second = alpha*Vb.second + (1 - alpha)*Vm.second;
    }

    //Normalising the vectors;
    double ModVa_new = sqrt(Va_new.first*Va_new.first + Va_new.second*Va_new.second);
    double ModVb_new = sqrt(Vb_new.first*Vb_new.first + Vb_new.second*Vb_new.second);

    // cout << "Direction Vectors : " << endl;
    // cout << "Starting pos : " << startPos.first << " " << startPos.second << endl;
    // cout << "Ending pos : " << endPos.first << " " << endPos.second << endl;
    // cout << "Vm : " << Vm.first << " " << Vm.second << endl;
    // cout << "Va : " << Va.first << " " << Va.second << endl;
    // cout << "Vb : " << Vb.first << " " << Vb.second << endl;
    // cout << "Va new : " << Va_new.first << " " << Va_new.second << endl;
    // cout << "Vb new : " << Vb_new.first << " " << Vb_new.second << endl;
    // cout << endl;

    if(ModVa_new != 0)
    {
        Va_new.first/=ModVa_new;
        Va_new.second/=ModVa_new;
    }

    if(ModVb_new != 0)
    {
        Vb_new.first/=ModVb_new;
        Vb_new.second/=ModVb_new;
    }

    DM.dirMap[startPos.first][startPos.second] = Va_new;
    DM.dirMap[endPos.first][endPos.second] = Vb_new;
}


// Constructor function for agent
Agent::Agent(int id, pi start, pi goal)
{
    this->agentId = id;
    this->startingPos = start;
    this->goalPos = goal;

    this->currPos = start;
}

double calculate_dot(Direction_Vector a, Direction_Vector b)
{
    _sleep(50);
    return 0;
}
/*
- given two vectors a and b, this function returns their dot product.
*/
double dot(pair<int,int> a, pair<int,int> b)
{
    double ans = a.first*b.first + a.second*b.second;
    if(a.first == 0 && a.second == 0)return 1;
    if(b.first == 0 && b.second == 0)return 1;
    return ans;
}

//helper function to calculate the cost given current position and direction of movement.
double getCost(pi currPos, pi direction, Direction_Map& DM, vvi & terrain, double weight)
{   
    int rows = terrain.size(), cols = terrain[0].size();
    pi endPos;
    endPos.first = currPos.first + direction.first;
    endPos.second = currPos.second + direction.second;

    if(endPos.first < 0 || endPos.first >= rows || endPos.second < 0 || endPos.second >= cols)return INF;

    pi Vm = direction;
    Vm = convert_to_dv(Vm);
    pi Va = DM.dirMap[currPos.first][currPos.second];
    pi Vb = DM.dirMap[endPos.first][endPos.second];

    double weightA = (1 - dot(Vm, Va))/2;
    double weightB = (1 - dot(Vm, Vb))/2;

    double costAB = terrain[endPos.first][endPos.second] - terrain[currPos.first][currPos.second] + baseCost;

    double cost = costAB + weight*(weightA + weightB)/2;

    return cost;
}

/*
    This function returns the minimum possible cost among the 4 possible movement options along with the minimum cost.
*/
pair<pair<int,int>, double> Agent::getMinMoveCost(vector<vector<int>>& terrain, Direction_Map DM)
{
    vector<pair<int,int>> temp = {{-1,0}, {1,0}, {0,-1}, {0,1}}; //possible Vm s
    pair<int,int> resDirection;
    double resCost = INT_MAX;

    for(auto x : temp)
    {
       double cost = getCost(currPos, x, DM, terrain);
       if(cost < resCost)
       {
            resCost = cost;
            resDirection = x;
       }
    }
    return {resDirection, resCost};
}

// struct cell
// {
//     double cost;
//     pi pos;
// };

// struct compareit
// {
//     bool operator()(cell &one, cell &two)
//     {
//         return one.cost <= two.cost;
//     }
// };

// vvi precompute(vvi & terrain, Direction_Map& DM, pi start_pos, pi end_pos)
// {
//     //dijkstras
//     priority_queue< cell, vector<cell>, compareit > myq;
//     int r = terrain.size(), c = terrain[0].size();

//     vvi distance(r, vector<int>(c, INF));
//     distance[start_pos.first][start_pos.second] = 0;

//     vector<vector<bool>> done(r, vector<bool>(c, false));

//     cell temp;
//     temp.cost = 0;
//     temp.pos = start_pos;
//     myq.push(temp);

//     while(!myq.empty())
//     {
//         cell temp = myq.top();
//         myq.pop();
//         if(done[temp.pos.first][temp.pos.second])continue;
//         done[temp.pos.first][temp.pos.second] = true;

//         vpi directions = {{-1,0}, {1,0}, {0,-1}, {0,1}};
//         for(auto x : directions)
//         {
//             pi next_pos = temp.pos;
//             next_pos.first += x.first;
//             next_pos.second += x.second;
//             if(next_pos.first < 0 || next_pos.first >= r || next_pos.second < 0 || next_pos.second >= c)continue;
//             if(terrain[next_pos.first][next_pos.second] == INF)continue;
            
//             double cost = getCost(temp.pos, x, DM, terrain);
//             if(distance[temp.pos.first][temp.pos.second] + cost < distance[next_pos.first][next_pos.second])
//             {
//                 distance[next_pos.first][next_pos.second] = distance[temp.pos.first][temp.pos.second] + cost;
//                 cell tobe;
//                 tobe.pos = next_pos;
//                 tobe.cost = distance[next_pos.first][next_pos.second];
//                 myq.push(tobe);
//             }
//         }
//     }
//     return distance;
// }

struct node
{
    int x;
    int y;
    double cost;
};

struct compare
{
    bool operator()(node one, node two)
    {
        return one.cost >= two.cost;
    }
};

bool isEqual(pi one, pi two)
{
    if(one.first != two.first)return false;
    if(one.second != two.second)return false;
    return true;
}

vpi tracePath(vector<vector< pi >>& parent, pi dest)
{   
    // cout << "Entering tracePath" << endl;
    vpi ret;
    int x = dest.first;
    int y = dest.second;

    while(!isEqual(parent[x][y], {x, y}))
    {   
        int x_dir = x - parent[x][y].first;
        int y_dir = y - parent[x][y].second;
        ret.push_back({x_dir, y_dir});

        int temp_x = parent[x][y].first;
        int temp_y = parent[x][y].second;
        x = temp_x;
        y = temp_y;
    }
    reverse(ret.begin(), ret.end());
    // cout << "Exiting tracePath" << endl;
    return ret;
}

/*
- This function gives the most optimal path available from source to destination.
- Return vector<pair<int,int>> each pi denoting i, j vectors of movement.
- Used during the training phase
- Uses the BFS search to find the most optimal path.
- During the testing phase greedy approach or rather the local window approach will be used.
*/
vpi Agent::getOptimalPath(vvi & terrain, Direction_Map& DM, pi start_pos, pi end_pos, double weight)
{   
    // cout << "entered getOptimal Path" << endl;
    int rows = terrain.size();
    int cols = terrain[0].size();

    vector<vector<double>> distance(rows, vector<double>(cols, INF));
    vector<vector<bool>> processed(rows, vector<bool>(cols, false));
    vector<vector< pi >> parent(rows, vector< pi >(cols));

    for(int i = 0; i < rows;  i++)
    {
        for(int j = 0; j < cols; j++)
        {
            parent[i][j].first = i;
            parent[i][j].second = j;
        }
    }
    priority_queue<pair<double,int>> myq;
    distance[start_pos.first][start_pos.second] = 0;
    int position = start_pos.first*cols + start_pos.second;
    myq.push({0, position});

    while(!myq.empty())
    {
        pi topmost = myq.top();
        myq.pop();
        int px = topmost.second/cols;
        int py = topmost.second % cols;

        if(processed[px][py])continue;
        processed[px][py] = true;
        vpi directions = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        for(auto dir : directions)
        {
            int next_x = px + dir.first;
            int next_y = py + dir.second;

            if(next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols)continue;
            if(terrain[next_x][next_y] == INF)continue;

            double cost = getCost({px, py}, dir, DM, terrain, weight);

            if(distance[px][py] + cost < distance[next_x][next_y])
            {   
                distance[next_x][next_y] = distance[px][py] + cost;
                parent[next_x][next_y] = {px, py};
                int posi = next_x*cols + next_y;
                myq.push({-distance[next_x][next_y], posi});
            }
        }
    }

    // cout << "calling tracepath" << endl;
    vpi ret = tracePath(parent, end_pos);
    
    return ret;
}


/*
    This function firstly checks if the flag is false or true.
    False -> we need to calculate the best move and then update the direction map and agent position
    True -> we have the most optimal path and we just need to update the direction map and agent position
    - For each step, function needs to be called seperately.
*/
void Agent::updatePos(vector<vector<int>> &terrain, Direction_Map &DM, bool flag, pair<int,int> direction)
{
    if(flag)//When path is there.
    {
        // we have most optimal path, we just need to update the agent position and the direction map
        pair<int,int> nextPos = currPos;
        nextPos.first += direction.first;
        nextPos.second += direction.second;
        updateDirectionMap(currPos, nextPos, terrain, DM);
        this->currPos = nextPos;
    }
    else // When path needs to be calculated.
    {
        // calculate best move and update the direction map and agent position accordingly.
        pair<pair<int,int>, double> res = getMinMoveCost(terrain, DM);
        pair<int,int> nextPos = currPos;
        nextPos.first += res.first.first;
        nextPos.second += res.first.second;
        updateDirectionMap(currPos, nextPos, terrain, DM);
        this->currPos = nextPos;
    }
}
