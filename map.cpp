#include "map.h"
#include "all_header.h"

Map::Map(int id, int row, int col)
{
    this->mapId = id;
    this->numRows = row;
    this->numCols = col;
    this->terrain.resize(row, vector<int>(col, 0));
}

void Map::getTerrain(bool uniform)
{
    // making base layout of the terrain
    // eithe uniform or non-uniform
    // this is without the obstacles
    if(uniform)
    {
        for(int i = 0; i < this->numRows; i++)
        {
            for(int j = 0; j < this->numCols; j++)
            {
                this->terrain[i][j] = 5;
            }
        }
    }
    else
    {
        for(int i = 0; i < this->numRows; i++)
        {
            for(int j = 0; j < this->numCols; j++)
            {
                this->terrain[i][j] = rand()%10;
            }
        }
    }

    // now adding the obstacles as per the map ID

    if(this->mapId == 1) // map with half-column of obstacles in the middle
    {
        /*
        Dots are free cells, | are obstacles
        . . . . . 
        . . . . . 
        . . . . . 
        . . | . . 
        . . | . .
        
        */
        if(this->numCols%2 == 1)
        {
            for(int i = this->numRows/2; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
            }
        }
        else
        {
            for(int i = this->numRows/2; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
                this->terrain[i][this->numCols/2 - 1] = INF;
            }
        }
    }
    else if(this->mapId == 2) // map with slit like obstacles in the middle
    {
        /*
        Dots are free cells, | are obstacles
        . . . | . . .
        . . . | . . .
        . . . . . . .
        . . . . . . .
        . . . . . . .
        . . . | . . .
        . . . | . . .
        
        */
        if(this->numCols%2 == 1)
        {
            for(int i = 0; i < this->numRows/3; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
            }
            for(int i = 2*this->numRows/3; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
            }
        }
        else
        {
            for(int i = 0; i < this->numRows/3; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
                this->terrain[i][this->numCols/2 - 1] = INF;
            }
            for(int i = 2*this->numRows/3; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
                this->terrain[i][this->numCols/2 - 1] = INF;
            }
        }
    }
    else if(this->mapId == 3) 
    {
        /*
        map with a slit like obstacle in the middle with 
        random obstacles in the rest of the map
        */
       /*
        Dots are free cells, | are obstacles
        . . . | . . .
        . . . | . . .
        | . . . . | .
        . | . | | . .
        . . . . . . .
        . . . | . . .
        . . . | . . .
        
        */
        if(this->numCols%2 == 1)
        {
            for(int i = 0; i < this->numRows/3; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
            }
            for(int i = 2*this->numRows/3; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
            }
        }
        else
        {
            for(int i = 0; i < this->numRows/3; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
                this->terrain[i][this->numCols/2 - 1] = INF;
            }
            for(int i = 2*this->numRows/3; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
                this->terrain[i][this->numCols/2 - 1] = INF;
            }
        }

        // now adding 20 random obstacles such that the slit is not blocked
        int count = 0;
        while(count < 20)
        {
            int i = rand()%this->numRows;
            int j = rand()%this->numCols;
            if(i <= this->numRows/10 || i >= 9*this->numRows/10)continue; // This creates spawing zone
            if (j <= this->numCols/2 + 2 && j >= this->numCols/2 - 2)continue;
            if(this->terrain[i][j] != INF)
            {
                this->terrain[i][j] = INF;
                count++;
            }
        }
    }
    else if(this->mapId == 4) // map with a slit like obstacle in the middle with random obstacles in the rest of the map
    {
        /*
        Dots are free cells, | are obstacles
        . . . . . . | . . . . . .
        . . . . . . | . . . . . .
        . . . . . . | . . . . . .
        . | | . | . . . . . | | .
        . . | . . . . . | . | . .
        . . | . . | . . . . | . .
        . . | . | . . . | . | . .
        . | | . . . . . . . | | .
        . . . . . . | . . . . . .
        . . . . . . | . . . . . .
        . . . . . . | . . . . . .
        
        */

        if(this->numCols%2 == 1)
        {
            for(int i = 0; i < this->numRows/3; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
            }
            for(int i = 2*this->numRows/3; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
            }
        }
        else
        {
            for(int i = 0; i < this->numRows/3; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
                this->terrain[i][this->numCols/2 - 1] = INF;
            }
            for(int i = 2*this->numRows/3; i < this->numRows; i++)
            {
                this->terrain[i][this->numCols/2] = INF;
                this->terrain[i][this->numCols/2 - 1] = INF;
            }
        }

        // now making the pattern on the left and right

        for(int i = this->numRows/3; i < 2*this->numRows/3; i++)
        {
            if(i == this->numRows/3 || i == 2*this->numRows/3 - 1)
            {
                for(int j = 10; j<15; j++)
                {
                    this->terrain[i][this->numCols/2 - j] = INF;
                    this->terrain[i][this->numCols/2 + j] = INF;
                }
            }
            else
            {
                this->terrain[i][this->numCols/2 - 10] = INF;
                this->terrain[i][this->numCols/2 + 10] = INF;
                this->terrain[i][this->numCols/2 - 11] = INF;
                this->terrain[i][this->numCols/2 + 11] = INF;
            }            
        }

        // now adding 20 random obstacles such that the slit is not blocked

        int count = 0;
        while(count < 20)
        {
            int i = rand()%this->numRows;
            int j = rand()%this->numCols;
            if(i <= this->numRows/10 || i >= 9*this->numRows/10)continue; // This creates spawing zone
            if (j <= this->numCols/2 + 2 && j >= this->numCols/2 - 2)continue;
            if(this->terrain[i][j] != INF)
            {
                this->terrain[i][j] = INF;
                count++;
            }
        }
    }

    return;
} 