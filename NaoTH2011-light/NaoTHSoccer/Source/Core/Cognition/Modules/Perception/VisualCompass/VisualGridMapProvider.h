/**
* @file VisualGridMapProvider.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
* Definition of class VisualGridMapProvider
*/

#ifndef VISUALGRIDMAPPROVIDER_H
#define VISUALGRIDMAPPROVIDER_H

#include <vector>
#include "Representations/Infrastructure/FieldInfo.h"
#include "VisualCompassParameters.h"
#include "VisualCompassFeature.h"
#include <time.h>


class VisualGridMapProvider
{

public:
    VisualCompassFeature ***gridmap;
    bool isInitialized;

    void initializeStorageArray()
    {
        // Allocate memory
        gridmap = new VisualCompassFeature**[GRID_X_LENGTH];
        for (int i = 0; i < GRID_X_LENGTH; i++)
        {
            gridmap[i] = new VisualCompassFeature*[GRID_Y_LENGTH];
            for (int j = 0; j < GRID_Y_LENGTH; j++)
            {
                gridmap[i][j] = new VisualCompassFeature[NUM_ANGLE_BINS];
                for(int k = 0; k < NUM_ANGLE_BINS; k++)
                {
                    gridmap[i][j][k].valid = false;
                }
            }
        }
        isInitialized = true;
        return;
    }

    void destroyStorageArray()
    {
        for (int i = 0; i < GRID_X_LENGTH; ++i)
        {
            for (int j = 0; j < GRID_Y_LENGTH; ++j)
                delete [] gridmap[i][j];

            delete [] gridmap[i];
        }
        delete [] gridmap;
        isInitialized = false;
    }

    // takes a vector with features and store them in the grid
    void storeFeature(VisualCompassFeature vcf)
    {
        return;
    }

    /*
     *translates the field position to the grid position
     *where we are going to store the feature.
     *Assumes: field pos in the center 0,0
     */
    static void fieldPosToGridPos(Vector2<double> fieldPos, Vector2<int> &gridPos)
    {
        gridPos.x = floor(( fieldPos.x + FieldInfo().xLength * 0.5 ) / GRID_X_LENGTH);
        gridPos.y = floor(( fieldPos.y + FieldInfo().yLength * 0.5 ) / GRID_Y_LENGTH);
    }

    void saveGridMapModel()
    {
        std::ofstream outBinFile;
        outBinFile.open(COMPASS_DATA_FILE, ios::out | ios::binary);
        for (int i = 0; i < GRID_X_LENGTH; i++){
            for (int j = 0; j < GRID_Y_LENGTH; j++){
                for (int k = 0; k < NUM_ANGLE_BINS; k++){
                    outBinFile.write(reinterpret_cast<char*> (&this->gridmap[i][j][k]), sizeof(VisualCompassFeature));
                }
            }
        }
        outBinFile.close();
        return;
    }

    void readGridMapModel()
    {
        if(!this->isInitialized)
        {
            this->initializeStorageArray();
            std::ifstream inBinFile;
            inBinFile.open(COMPASS_DATA_FILE, ios::in | ios::binary);
            for (int i = 0; i < GRID_X_LENGTH; i++){
                for (int j = 0; j < GRID_Y_LENGTH; j++){
                    for (int k = 0; k < NUM_ANGLE_BINS; k++){
                        inBinFile.read(reinterpret_cast<char*> (&this->gridmap[i][j][k]), sizeof(VisualCompassFeature));
                    }
                }
            }
            inBinFile.close();
        }
        return;
    }
};

#endif // VISUALGRIDPMAPROVIDER_H
