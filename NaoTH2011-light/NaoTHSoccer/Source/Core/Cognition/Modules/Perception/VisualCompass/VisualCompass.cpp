#include "VisualCompass.h"
using namespace std;
using namespace cv;

VisualCompass::VisualCompass()
{
    GridMapProvider.isInitialized = false;
    // debug stuff
    DEBUG_REQUEST_REGISTER("VisualCompass:mark_area", "mark the possibles' features area", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:scan_lines", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:draw_orientation_loc","from localization", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:draw_orientation_vc","from visual compass", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:draw_visual_grid_map"," dsgew", false);

    // visual compass
    DEBUG_REQUEST_REGISTER("VisualCompass:clear_feature_grid_map"," delete the previous generated model", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:record","record the feature map for the location of the robot", false);

    // motions
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:standard_stand", "stand as standard or not", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:stand", "The default motion, otherwise do nothing", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:turn_right", "Set the motion request to 'turn_right'.", false);
}

VisualCompass::~VisualCompass()
{
}

void VisualCompass::saveModel()
{
    std::ofstream outBinFile;
    outBinFile.open(COMPASS_DATA_FILE, ios::out | ios::binary);
    for (int i = 0; i < GRID_X_LENGTH; i++){
        for (int j = 0; j < GRID_Y_LENGTH; j++){
            for (int k = 0; k < NUM_ANGLE_BINS; k++){
                outBinFile.write(reinterpret_cast<char*> (&GridMapProvider.gridmap[i][j][k]), sizeof(VisualCompassFeature));
            }
        }
    }
    outBinFile.close();
    return;
}

void VisualCompass::readModel()
{
    if(!GridMapProvider.isInitialized)
    {
        GridMapProvider.initializeStorageArray();
        std::ifstream inBinFile;
        inBinFile.open(COMPASS_DATA_FILE, ios::in | ios::binary);
        for (int i = 0; i < GRID_X_LENGTH; i++){
            for (int j = 0; j < GRID_Y_LENGTH; j++){
                for (int k = 0; k < NUM_ANGLE_BINS; k++){
                    inBinFile.read(reinterpret_cast<char*> (&GridMapProvider.gridmap[i][j][k]), sizeof(VisualCompassFeature));
                }
            }
        }
        inBinFile.close();
    }
    return;
}

void VisualCompass::recordFeatures()
{
    int x = 0;
    int y = 0;
    int theta = 0;
    double dx = getFieldInfo().xLength / GRID_X_LENGTH;
    double dy = getFieldInfo().yLength / GRID_Y_LENGTH;

    if(getRobotPose().isValid)
    {
        double poseX = getRobotPose().translation.x + getFieldInfo().xFieldLength / 2;
        double poseY = getRobotPose().translation.y + getFieldInfo().yFieldLength / 2;
        x = max(0, (int) floor(poseX / dx));
        x = min(GRID_X_LENGTH-1, x);
        y = max(0, (int) floor(poseY / dy));
        y = min(GRID_Y_LENGTH-1, y);

        // rotation from 0 to 360 degrees -- normalize
        double theta_full = Math::toDegrees(getRobotPose().rotation) + 180.0;
        // TODO:: fix it work with rads
        theta = (int) theta_full / 30;
        GridMapProvider.gridmap[x][y][theta].valid = true;
        GridMapProvider.gridmap[x][y][theta].orientation = getRobotPose().rotation;
    }
}

void VisualCompass::clearCompass()
{
    // check if there is a model file and delete it
    if(GridMapProvider.isInitialized)
    {
        GridMapProvider.destroyStorageArray();
    }
}

bool VisualCompass::hasModel()
{
    std::ifstream ifs (COMPASS_DATA_FILE);
    if (ifs.is_open())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool VisualCompass::isValid()
{
    return false;
}

void VisualCompass::execute()
{
    if(!GridMapProvider.isInitialized)
        GridMapProvider.initializeStorageArray();

    DEBUG_REQUEST("VisualCompass:clear_feature_grid_map",
                  clearCompass();
            );

    DEBUG_REQUEST("VisualCompass:record",
                  std::cout << "recording..." << std::endl;
                recordFeatures();
            );

    DEBUG_REQUEST("VisualCompass:draw_orientation_loc",
                  FIELD_DRAWING_CONTEXT;
            CIRCLE(getRobotPose().translation.x,
                   getRobotPose().translation.y, 50);

    ARROW(getRobotPose().translation.x, getRobotPose().translation.y,
          getRobotPose().translation.x + 200*cos(getRobotPose().rotation),
          getRobotPose().translation.y + 200*sin(getRobotPose().rotation));
    );

    DEBUG_REQUEST("VisualCompass:draw_orientation_vc",
                  FIELD_DRAWING_CONTEXT;
            CIRCLE(getRobotPose().translation.x,
                   getRobotPose().translation.y, 50);

    ARROW(getRobotPose().translation.x, getRobotPose().translation.y,
          getRobotPose().translation.x + 400*cos(getRobotPose().rotation),
          getRobotPose().translation.y + 400*sin(getRobotPose().rotation));
    );

    DEBUG_REQUEST("VisualCompass:draw_visual_grid_map",
                  FIELD_DRAWING_CONTEXT;
            // vertical lines
            double dx = getFieldInfo().xLength / GRID_X_LENGTH;
    for(int i = 1; i < GRID_X_LENGTH; i++)
    {
        LINE(- getFieldInfo().xLength / 2 + dx * i, getFieldInfo().yLength / 2, - getFieldInfo().xLength / 2 + dx * i, - getFieldInfo().yLength / 2);
    }
    // horizontal lines
    double dy = getFieldInfo().yLength / GRID_Y_LENGTH;
    for(int i = 1; i < GRID_Y_LENGTH; i++)
    {
        LINE(getFieldInfo().xLength / 2, - getFieldInfo().yLength / 2 + dy * i, - getFieldInfo().xLength / 2, - getFieldInfo().yLength / 2 + dy * i);
    }
    // circles in the middle of each square
    double lx = - getFieldInfo().xLength / 2 + dx / 2;
    double ly = - getFieldInfo().yLength / 2 + dy / 2;
    for(int i =0; i < GRID_X_LENGTH; i++)
    {
        double x = i * dx;
        for(int j =0; j < GRID_Y_LENGTH; j++)
        {
            double y = j * dy;
            CIRCLE(lx + x, ly + y, 50);
            // arrow in each circle represent a saved feature
            for(int ii = 0; ii < NUM_ANGLE_BINS; ii++ )
            {
                if(GridMapProvider.gridmap[i][j][ii].valid)
                {
                    ARROW(lx + x, ly + y,
                          lx + x + 100*cos(GridMapProvider.gridmap[i][j][ii].orientation),
                          ly + y + 100*sin(GridMapProvider.gridmap[i][j][ii].orientation));
                }
            }
        }
    }
    );

    VisualCompass::pixelVector.clear();
    Vector2<double> a(getArtificialHorizon().begin());
    Vector2<double> b(getArtificialHorizon().end());


    ColorClasses::Color color = ColorClasses::red;
    if(isValid(a, b))
        color = ColorClasses::green;

    DEBUG_REQUEST("VisualCompass:mark_area",
                  LINE_PX( color, (int)a.x, (int)(a.y), (int)b.x, (int)(b.y) );
            LINE_PX( color, (int)0, (int)0, (int)a.x, (int)a.y );
    LINE_PX( color, (int)getImage().width()-1, (int)0, (int)b.x, (int)(b.y) );
    LINE_PX( color, (int)0, (int)0, (int)getImage().width()-1, (int)0 );
    );
    if(isValid(a, b))
    {
        colorExtraction();
        verticalScanner();
    }

}

void VisualCompass::scanner()
{
    // reset some stuff by default
    getMotionRequest().forced = false;
    getMotionRequest().standHeight = -1; // sit in a stable position
    head();
    motion();
}

void VisualCompass::head()
{
    getHeadMotionRequest().id = HeadMotionRequest::look_straight_ahead;
}

void VisualCompass::motion()
{
    getMotionRequest().walkRequest.target = Pose2D();

    DEBUG_REQUEST("VisualCompass:motion:stand",
                  getMotionRequest().id = motion::stand;
            );

    getMotionRequest().standardStand = false;
    DEBUG_REQUEST("VisualCompass:motion:standard_stand",
                  getMotionRequest().standardStand = true;
            getMotionRequest().standHeight = -1; // minus means the same value as walk
    );

    DEBUG_REQUEST("VisualCompass:motion:turn_right",
                  getMotionRequest().id = motion::walk;
            getMotionRequest().walkRequest.target.rotation = Math::fromDegrees(-30);
    );

    getMotionRequest().walkRequest.character = 0.5;
    getMotionRequest().walkRequest.coordinate = WalkRequest::Hip;

    double offsetR = 0;
    MODIFY("walk.offset.r", offsetR);
    getMotionRequest().walkRequest.offset.rotation = Math::fromDegrees(offsetR);
    MODIFY("walk.offset.x", getMotionRequest().walkRequest.offset.translation.x);
    MODIFY("walk.offset.y", getMotionRequest().walkRequest.offset.translation.y);
    MODIFY("walk.character", getMotionRequest().walkRequest.character);

}//end testMotion


bool VisualCompass::isValid(Vector2<double> a, Vector2<double> b)
{
    int min_ = (int) min(a.y, b.y);
    int max_ = (int) max(a.y, b.y);
    double max_area = getImage().width() * getImage().height();
    double area = getImage().width() * min_;
    if(min_ != max_)
        area += 0.5 * getImage().width() * abs(max_ - min_);
    double area_covered = area / max_area;
    if(area_covered >= COMPASS_WIDTH_MIN && area_covered <= COMPASS_WIDTH_MAX)
        return true;
    return false;
}

void VisualCompass::colorExtraction()
{
    for(unsigned int i = 0; i < getImage().width(); i++)
    {
        for (unsigned int j = 0; j < getImage().height(); j++)
        {
            VisualCompass::pixelVector.push_back(getImage().get(i, j));
        }
    }
}

vector< vector<Pixel> > VisualCompass::verticalScanner()
{
    vector< vector<Pixel> > scanner;
    for(double p = 5.00; p < (double) getImage().width(); p += COMPASS_FEATURE_STEP)
    {
        vector<Pixel> line;
        Vector2<double> start = getArtificialHorizon().point(p);
        Vector2<int> point((int) start.x, (int) start.y);
        double angle = getArtificialHorizon().getDirection().rotateRight().angle();
        double distance = 0.0;
        while(getImage().isInside(point.x, point.y))
        {
            DEBUG_REQUEST("VisualCompass:scan_lines", POINT_PX(ColorClasses::blue, point.x, point.y););
            point.x = (int) (start.x + distance * cos(angle));
            point.y = (int) (start.y + distance * sin(angle));
            distance++;
        }
        scanner.push_back(line);
    }
    return scanner;
}
