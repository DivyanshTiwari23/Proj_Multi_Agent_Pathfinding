#include <bits/stdc++.h>
using namespace std;

#include "all_header.h"
#include "map.h"
#include "agent.h"

//to demonstrate code running keep map_size <= 20x20
// number of agents 5.
void test(int x, Map& myMap, Direction_Map& DM);

int main()
{
    int mapChoice;
    cout << "Enter map choice: ";
    cin >> mapChoice;

    int agent_num = 1;
    cout << "Enter number of agents : ";
    cin >> agent_num;

    int map_row, map_col;
    cout << "Enter map size :\nRows : ";
    cin >> map_row;
    cout << "Cols : ";
    cin >> map_col;

    Map myMap = Map(mapChoice, map_row, map_col);
    // generates a uniform terrain of 64x64 with obstacles of our choice
    myMap.getTerrain(true);

    set<int> startingPosSet;
    vector<Agent> agents;
    srand(time(NULL));

    int maxAgentsPossible;
    if(myMap.numCols % 2 == 0)
        maxAgentsPossible = (myMap.numCols - 2)/ 2;
    else
        maxAgentsPossible = (myMap.numCols - 1)/ 2;

    while (agents.size() < agent_num && agents.size() < maxAgentsPossible)
    {
        while (agents.size() < agent_num && agents.size() < maxAgentsPossible)
        {
            int x = rand() % maxAgentsPossible;
            
            if (startingPosSet.find(x) == startingPosSet.end())
            {
                startingPosSet.insert(x);
                agents.push_back(Agent(agents.size(), {myMap.numRows - 1, x}, {myMap.numRows - 1, myMap.numRows - x - 1}));
            }
        }
    }
    agent_num = min(agent_num, maxAgentsPossible);
    cout << "Agents initialised" << endl;
    Direction_Map DM;
    DM.numRows = myMap.numRows;
    DM.numCols = myMap.numCols;
    DM.dirMap = vector<vector<Direction_Vector>>(myMap.numRows, vector<Direction_Vector>(myMap.numCols, {0, 0}));

    // for(int i = 0; i < agents.size() ; i++)
    // {
    //     cout << "Agent number : " << i+1 << endl;
    //     cout << "Enter starting pos : " << endl;
    //     cin >> agents[i].startingPos.first >> agents[i].startingPos.second;
    //     cout << "Enter Goal pos : " << endl;
    //     cin >> agents[i].goalPos.first >> agents[i].goalPos.second;
    //     agents[i].currPos = agents[i].startingPos;
    // }
    /*
    training phase
    each agent will move from its starting position to its goal position one by one and keep updating the direction map.
    */
    int numIters = 10; // number of iterations means number of times each agent will move from its starting position to its goal position and back to its starting position.

    for (int i = 0; i < 2*numIters; i++)
    {
        for (int j = 0; j < agent_num; j++)
        {   
            vpi currOptimalPath = agents[j].getOptimalPath(myMap.terrain, DM, agents[j].startingPos, agents[j].goalPos);
            int count = 0;
            for (auto x : currOptimalPath)
            {  
                agents[j].updatePos(myMap.terrain, DM, true, x);
            }
            // cout << "Agent : " <<  j+1 << " done" << endl;
        }

        for(int j = 0; j < agent_num; j++)
        {
            int id = agents[j].agentId;
            agents[j] = Agent(id, agents[j].goalPos, agents[j].startingPos);
        }
        // cout << "Iteration : " << i+1 << " done" << endl;
    }

    cout << "Training done" << endl;

//uncomment the below code for debuggin purposes.
    // cout << "Entering the loop" << endl;
    // for(int i = 0; i < agent_num; i++)
    // {
    //     vpi path = agents[i].getOptimalPath(myMap.terrain, DM, agents[i].startingPos, agents[i].goalPos);
    //     cout << "Starting Pos : " << endl;
    //     cout << agents[i].startingPos.first << " " << agents[i].startingPos.second << endl;
    //     cout << "Goal Pos : " << endl;
    //     cout << agents[i].goalPos.first << " " << agents[i].goalPos.second << endl;
    //     cout << "Path followed is : " << endl;
    //     for(auto x : path)
    //     {   
    //         cout << x.first << " " << x.second << endl;
    //         agents[i].updatePos(myMap.terrain, DM, true, x);
    //     }
    // }
    // cout << "Reached here" << endl;
    
    /*
        Testing phase starts
    */
    cout << "Exporting DM and terrain of training phase : " << endl;
    export_dm(DM);
    export_terrain(myMap.terrain);    

    cout << endl << endl;

    cout << "Testing Phase starting " << endl;
    while(1)
    {
        int input;
        cout << "Enter your input :\n 1. Weighted\n 2. Unweighted\n 3. Quit\n" << endl;
        cin >> input;
        if(input != 1 && input != 2)break;
        test(input, myMap, DM);
    }
    
    return 0;
}

void test(int x, Map& myMap, Direction_Map& DM)
{
    int numTestingAgents;
    cout << "Enter number of testing agents : ";
    cin >> numTestingAgents;

    vector<Agent> testingAgents;
    Direction_Map DM2;
    DM2.numRows = myMap.numRows;
    DM2.numCols = myMap.numCols;
    DM2.dirMap = vector<vector<Direction_Vector>>(myMap.numRows, vector<Direction_Vector>(myMap.numCols, {0, 0}));

    pi starting, goal;
    for(int i = 0; i < numTestingAgents; i++)
    {
        cout << "Agent id : " << i << endl;
        cout << "Enter starting pos : ";
        cin >> starting.first >> starting.second;
        cout << "Enter goal pos : ";
        cin >> goal.first >> goal.second;

        Agent myagent(i, starting, goal);
        testingAgents.push_back(myagent);
    }
    
    vector<vpi> optimalPaths;
    for(int i = 0; i < numTestingAgents; i++)
    {   
        vpi path;
        if(x == 1)
            path = testingAgents[i].getOptimalPath(myMap.terrain, DM, testingAgents[i].startingPos, testingAgents[i].goalPos);
        else
            path = testingAgents[i].getOptimalPath(myMap.terrain, DM, testingAgents[i].startingPos, testingAgents[i].goalPos, 0);

        optimalPaths.push_back(path);    
    }

    map<int,int> stepsTaken; //number of steps taken by each agent;
    int iterations = 1;
    int reachedGoal = iterations*numTestingAgents;

    int collisions = 0;
    while(reachedGoal)
    {
    //marking present and next positions of each agent.
        map<int, pi > nextPos;
        map<int, pi > currentPos;
        for(int i = 0; i < numTestingAgents; i++)
        {
            if(testingAgents[i].currPos == testingAgents[i].goalPos)continue;
            currentPos[i] = testingAgents[i].currPos;
            pi temp = testingAgents[i].currPos;
            pi dir = optimalPaths[i][stepsTaken[i]];
            temp.first+=dir.first;
            temp.second+=dir.second;
            nextPos[i] = temp;
        }
        vector<int> keys;
        for(auto x : currentPos)keys.push_back(x.first);
        int n = keys.size();
        vector<bool> move(n, true);

    //checking collisions
        for(int i = 0; i < n; i++)
        {   
            int collide = 0;   
            if(move[i] == false)continue;  
            for(int j = i+1; j < n; j++)
            {
                if(isEqual(nextPos[keys[i]], nextPos[keys[j]]))
                {
                    collide++;
                    cout << "Collision while tryin to reach the same place : " ;
                    cout << nextPos[keys[j]].first << " " << nextPos[keys[j]].second;
                    move[j] = false;
                }
                else if(isEqual(nextPos[keys[i]], currentPos[keys[j]]) && 
                    isEqual(nextPos[keys[j]], currentPos[keys[i]]))
                {
                    cout << "Head on collision : ";
                    cout << nextPos[keys[i]].first << " " << nextPos[keys[i]].second << endl;
                    collide++;
                }
            }
            collisions+=collide;
        }
    //Movement
        for(int i = 0; i < n; i++)
        {
            if(move[i])
            {
                int id = keys[i];
                pi dir;
                dir.first = nextPos[id].first - currentPos[id].first;
                dir.second = nextPos[id].second - currentPos[id].second;
                testingAgents[id].updatePos(myMap.terrain, DM2, true, dir);
                // testingAgents[id].currPos = nextPos[id];
                stepsTaken[id]++;
                if(testingAgents[id].currPos == testingAgents[id].goalPos)
                    reachedGoal--;
            }
        }
    }

    cout << endl;
    cout << "Number of collisions are : " << collisions << endl << endl;
    cout << "Exporting the direction map :" << endl;

    export_dm(DM2); 
}