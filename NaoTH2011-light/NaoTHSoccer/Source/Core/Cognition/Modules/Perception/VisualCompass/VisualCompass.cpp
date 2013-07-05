#include "VisualCompass.h"
using namespace std;
using namespace cv;

VisualCompass::VisualCompass()
{
    GridMapProvider.isInitialized = false;
    clustered = false;

    // debug stuff
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:mark_area", "mark the possibles' features area", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:scan_lines", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_orientation_loc","from localization", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_orientation_vc","from visual compass", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_visual_grid_map","", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_info","", false);
    // visual compass
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:record_offline","record the feature map for the location of the robot", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:online_mode", "", false);

    DEBUG_REQUEST_REGISTER("VisualCompass:functions:grid:read_model_from_config","", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:grid:save_model_to_config","", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:grid:clear_feature_grid_map"," delete the previous generated model", false);

    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:save_color_clusters", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:read_color_clusters", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:cluster_colors", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:clear_images","", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:record_images","stores images to use them for color extraction", false);
    // motions
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:scanner_standing", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:standard_stand", "stand as standard or not", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:stand", "The default motion, otherwise do nothing", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:turn_right", "Set the motion request to 'turn_right'.", false);
}

VisualCompass::~VisualCompass()
{
}

void VisualCompass::execute()
{
    initializeGridMapModel();

    DEBUG_REQUEST("VisualCompass:functions:record_offline", recordFeatures(););
    DEBUG_REQUEST("VisualCompass:functions:online_mode", victoria(););

    DEBUG_REQUEST("VisualCompass:functions:grid:save_model_to_config", saveGridMapModel(););
    DEBUG_REQUEST("VisualCompass:functions:grid:read_model_from_config", readGridMapModel(););
    DEBUG_REQUEST("VisualCompass:functions:grid:clear_feature_grid_map", clearCompass(););

    DEBUG_REQUEST("VisualCompass:functions:color:cluster_colors", colorClustering(););
    DEBUG_REQUEST("VisualCompass:functions:color:record_images", extractPixelsFromImages(););
    DEBUG_REQUEST("VisualCompass:functions:color:save_color_clusters", saveColorClusters(););
    DEBUG_REQUEST("VisualCompass:functions:color:read_color_clusters", readColorClusters(););
    DEBUG_REQUEST("VisualCompass:functions:color:clear_images", pixelVector.clear(););

    DEBUG_REQUEST("VisualCompass:debug:draw_orientation_loc", drawPoseOrientation(););
    DEBUG_REQUEST("VisualCompass:debug:draw_orientation_vc", drawCompassOrientation(););
    DEBUG_REQUEST("VisualCompass:debug:draw_visual_grid_map", drawVisualGridModel(););
    DEBUG_REQUEST("VisualCompass:debug:mark_area", drawROI(););
    DEBUG_REQUEST("VisualCompass:debug:draw_info", drawInfo(););

    DEBUG_REQUEST("VisualCompass:motion:standard_stand", scannerPosition(););
    return;
}

void VisualCompass::victoria()
{
    if(validHorizon() && clustered)
    {
        vector< vector<Pixel> > stripes;
        verticalScanner(stripes);
        VisualCompassFeature tmp;
        tmp.createFeatureFromScanLine(stripes, ClusteringProvider);
        double ora = queryModel.best_match(getRobotPose(), getFieldInfo(), GridMapProvider, tmp);
        std::cout << "<<" << ora << ">>>" << std::endl;
    }
    return;
}

void VisualCompass::drawInfo()
{

}

void VisualCompass::initializeGridMapModel()
{
    if(!GridMapProvider.isInitialized) GridMapProvider.initializeStorageArray();
    return;
}

void VisualCompass::saveGridMapModel()
{
    GridMapProvider.saveGridMapModel();
    return;
}

void VisualCompass::readGridMapModel()
{
    GridMapProvider.readGridMapModel();
    return;
}

void VisualCompass::saveColorClusters()
{
    const char* str = CLUSTER_DATA_FILE;
    ClusteringProvider.saveClusters(str);
    return;
}

void VisualCompass::readColorClusters()
{
    const char* str = CLUSTER_DATA_FILE;
    ClusteringProvider.readClusters(str);
    return;
}

void VisualCompass::recordFeatures()
{
    if(validHorizon() && clustered)
    {
        vector< vector<Pixel> > stripes;
        verticalScanner(stripes);
        VisualCompassFeature tmp;
        tmp.createFeatureFromScanLine(stripes, ClusteringProvider);
        GridMapProvider.storeFeature(tmp, getRobotPose(), getFieldInfo());
    }
    return;
}

void VisualCompass::clearCompass()
{
    // check if there is a model file and delete it
    if(GridMapProvider.isInitialized)
    {
        GridMapProvider.destroyStorageArray();
    }
    return;
}

bool VisualCompass::hasGridMapModel()
{
    std::ifstream ifs (COMPASS_DATA_FILE);
    if (ifs.is_open()) return true;
    return false;
}

void VisualCompass::scannerPosition()
{
    // reset some stuff by default
    getMotionRequest().forced = false;
    getMotionRequest().standHeight = -1; // sit in a stable position
    head();
    motion();
    return;
}

void VisualCompass::head()
{
    getHeadMotionRequest().id = HeadMotionRequest::look_straight_ahead;
    return;
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
    return;

}

bool VisualCompass::validHorizon()
{
    Vector2<double> a(getArtificialHorizon().begin());
    Vector2<double> b(getArtificialHorizon().end());
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
    if(getFrameInfo().getFrameNumber() % 5 == 0)
    {
        for(unsigned int i = 0; i < getImage().width(); i++)
        {
            for (unsigned int j = 0; j < getImage().height(); j++)
            {
                pixelVector.push_back(getImage().get(i, j));
            }
        }
        num_images++;
    }
    return;
}

void VisualCompass::verticalScanner(vector< vector<Pixel> > &scanner)
{
    double start = 0.50 * getImage().width() / COMPASS_FEATURE_NUMBER;
    double step = getImage().width() / COMPASS_FEATURE_NUMBER;
    for(double p = start; p < (double) getImage().width(); p += step)
    {
        vector<Pixel> line;
        Vector2<double> start = getArtificialHorizon().point(p);
        Vector2<int> point((int) start.x, (int) start.y);
        double angle = getArtificialHorizon().getDirection().rotateRight().angle();
        double distance = 0.0;
        while(getImage().isInside(point.x, point.y))
        {
            line.push_back(getImage().get(point.x, point.y));
            DEBUG_REQUEST("VisualCompass:debug:scan_lines", POINT_PX(ColorClasses::blue, point.x, point.y););
            point.x = (int) (start.x + distance * cos(angle));
            point.y = (int) (start.y + distance * sin(angle));
            distance++;
        }
        scanner.push_back(line);
    }
    return;
}

void VisualCompass::colorClustering()
{
    if(pixelVector.size() > 0 && !clustered)
    {
        ClusteringProvider.setClusters(NUM_OF_COLORS);
        ClusteringProvider.initializeColorModel(pixelVector, NUM_OF_COLORS);
        clustered = true;
    }
    return;
}

void VisualCompass::drawVisualGridModel()
{
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
    return;
}

void VisualCompass::drawPoseOrientation()
{
    FIELD_DRAWING_CONTEXT;
    CIRCLE(getRobotPose().translation.x,
           getRobotPose().translation.y, 50);

    ARROW(getRobotPose().translation.x, getRobotPose().translation.y,
          getRobotPose().translation.x + 200*cos(getRobotPose().rotation),
          getRobotPose().translation.y + 200*sin(getRobotPose().rotation));
    return;
}

void VisualCompass::drawCompassOrientation()
{
    FIELD_DRAWING_CONTEXT;
    CIRCLE(getRobotPose().translation.x,
           getRobotPose().translation.y, 50);

    ARROW(getRobotPose().translation.x, getRobotPose().translation.y,
          getRobotPose().translation.x + 400*cos(getRobotPose().rotation),
          getRobotPose().translation.y + 400*sin(getRobotPose().rotation));
    return;
}

void VisualCompass::drawROI()
{
    Vector2<double> a(getArtificialHorizon().begin());
    Vector2<double> b(getArtificialHorizon().end());
    ColorClasses::Color color = ColorClasses::red;
    if(validHorizon())
        color = ColorClasses::green;
    LINE_PX( color, (int)a.x, (int)(a.y), (int)b.x, (int)(b.y) );
    LINE_PX( color, (int)0, (int)0, (int)a.x, (int)a.y );
    LINE_PX( color, (int)getImage().width()-1, (int)0, (int)b.x, (int)(b.y) );
    LINE_PX( color, (int)0, (int)0, (int)getImage().width()-1, (int)0 );
    return;
}

void VisualCompass::extractPixelsFromImages()
{
    colorExtraction();
    FIELD_DRAWING_CONTEXT;
    stringstream ss;
    ss << num_images;
    string str = ss.str();
    TEXT_DRAWING(300, - getFieldInfo().yLength / 2 - 300, str);
    TEXT_DRAWING(-300, - getFieldInfo().yLength / 2 - 300, "Images: ");
    return;
}
