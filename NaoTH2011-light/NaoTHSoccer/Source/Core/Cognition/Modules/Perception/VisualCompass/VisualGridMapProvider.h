/**
* @file VisualGridMapProvider.h
*
* @author <a href="mailto:giorgosmethe@gmail.com">Georgios Methenitis</a>
* Definition of class VisualGridMapProvider
*/

#ifndef VISUALGRIDMAPPROVIDER_H
#define VISUALGRIDMAPPROVIDER_H

#include <vector>
#include "Cognition/Modules/Perception/VisualCompass/VisualCompassParameters.h"
#include "Cognition/Modules/Perception/VisualCompass/VisualCompassFeature.h"
#include "Representations/Infrastructure/FieldInfo.h"
#include "Representations/Modeling/RobotPose.h"
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
    void storeFeature(VisualCompassFeature vcf, RobotPose robotPose, FieldInfo fInfo)
    {
        if(robotPose.isValid)
        {
            Vector3<int> transRot = fieldPosToGridPos(robotPose, fInfo);
            this->gridmap[transRot.x][transRot.y][transRot.z].valid = true;
            memcpy(this->gridmap[transRot.x][transRot.y][transRot.z].featureTable2D, vcf.featureTable2D, sizeof (vcf.featureTable2D));
            this->gridmap[transRot.x][transRot.y][transRot.z].orientation = robotPose.rotation;
        }
        return;
    }

    /*
     *translates the field position to the grid position
     *where we are going to store the feature.
     *Assumes: field pos in the center 0,0
     */
    static Vector3<int> fieldPosToGridPos(RobotPose robotPose, FieldInfo fInfo)
    {
        int x = 0;
        int y = 0;
        double dx = fInfo.xLength / GRID_X_LENGTH;
        double dy = fInfo.yLength / GRID_Y_LENGTH;
        //find the proper bin and grid cell
        double poseX = robotPose.translation.x + fInfo.xLength / 2;
        double poseY = robotPose.translation.y + fInfo.yLength / 2;
        x = max(0, (int) floor(poseX / dx));
        x = min(GRID_X_LENGTH-1, x);
        y = max(0, (int) floor(poseY / dy));
        y = min(GRID_Y_LENGTH-1, y);
        // rotation from 0 to 360 degrees -- normalize
        double theta_full = Math::toDegrees(robotPose.rotation) + 180.0;
        // TODO:: fix it work with rads, HARDCODED VALUE!!!!!!!!!
        int theta = (int) theta_full / 30;
        return Vector3<int>(x, y, theta);
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
