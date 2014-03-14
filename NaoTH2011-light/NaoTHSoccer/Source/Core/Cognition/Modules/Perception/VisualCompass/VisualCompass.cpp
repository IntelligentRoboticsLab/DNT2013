#include "VisualCompass.h"
using namespace std;
using namespace cv;

VisualCompass::VisualCompass()
{
    total = 0, has_answer = 0;
    sum_angle_error = 0;
    GridMapProvider.isInitialized = false;
    clustered = false;

    // debug stuff
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:mark_area", "mark the possibles' features area", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:scan_lines", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_orientation_loc","from localization", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_orientation_vc","from visual compass", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_visual_grid_map","", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_cell_confidence","", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:debug:draw_info","", false);
    // visual compass
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:record_offline","record the feature map for the location of the robot", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:online_mode", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:reset_counters","", false);

    DEBUG_REQUEST_REGISTER("VisualCompass:functions:grid:read_model_from_config","", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:grid:save_model_to_config","", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:grid:clear_feature_grid_map"," delete the previous generated model", false);

    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:save_color_clusters", "Write current color clusters to file", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:read_color_clusters", "Read previous color clusters from file", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:static_clustering", "Use a preset amount of colors from single image", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:cluster_colors", "Generate color clusters from input images", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:clear_images", "Remove all stored images", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:record_images","stores images to use them for color extraction", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:functions:color:show_clustered_colors","Display color clusters' centroids", false);
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
    GridMapProvider.resetConfidenceOverGrid();
    GridMapProvider.updateConfidenceOverGrid(getRobotPose().theSampleSet, getFieldInfo());

    DEBUG_REQUEST("VisualCompass:functions:record_offline", recordFeatures(););
    DEBUG_REQUEST("VisualCompass:functions:online_mode", victoria(););
    DEBUG_REQUEST("VisualCompass:functions:reset_counters", resetCounters(););

    DEBUG_REQUEST("VisualCompass:functions:grid:save_model_to_config", saveGridMapModel(););
    DEBUG_REQUEST("VisualCompass:functions:grid:read_model_from_config", readGridMapModel(););
    DEBUG_REQUEST("VisualCompass:functions:grid:clear_feature_grid_map", clearCompass(););

    DEBUG_REQUEST("VisualCompass:functions:color:save_color_clusters", saveColorClusters(););
    DEBUG_REQUEST("VisualCompass:functions:color:read_color_clusters", readColorClusters(););
    DEBUG_REQUEST("VisualCompass:functions:color:static_clustering", staticColorClusters(););
    DEBUG_REQUEST("VisualCompass:functions:color:cluster_colors", colorClustering(););
    DEBUG_REQUEST("VisualCompass:functions:color:clear_images", pixelVector.clear(););
    DEBUG_REQUEST("VisualCompass:functions:color:record_images", extractPixelsFromImages(););
    DEBUG_REQUEST("VisualCompass:functions:color:show_clustered_colors", drawClusteredColors(););

    DEBUG_REQUEST("VisualCompass:debug:draw_orientation_loc", drawPoseOrientation(););
    DEBUG_REQUEST("VisualCompass:debug:draw_visual_grid_map", drawVisualGridModel(););
    DEBUG_REQUEST("VisualCompass:debug:draw_cell_confidence", drawCellConfidence(););
    DEBUG_REQUEST("VisualCompass:debug:mark_area", drawROI(););
    DEBUG_REQUEST("VisualCompass:debug:draw_info", drawInfo(););

    DEBUG_REQUEST("VisualCompass:motion:standard_stand", scannerPosition(););
    return;
}

void VisualCompass::staticColorClusters()
{
    pixelVector.clear();
#define DEFAULT_COLOR_CENTROIDS {\
	{  0,   0,   0},\
	{255,   0,   0},\
	{  0, 255,   0},\
	{255, 255,   0},\
	{  0,   0, 255},\
	{255,   0, 255},\
	{  0, 255, 255},\
	{255, 255, 255},\
}
    unsigned char centroids[NUM_OF_COLORS][3] = DEFAULT_COLOR_CENTROIDS;
    for(int i = 0; i < NUM_OF_COLORS; i++)
		{
			  Pixel p;
			  p.channels = centroids[i]; // TODO: copy this array
			  pixelVector.push_back(p);
		}
    extractPixelsFromImages();
}

void VisualCompass::recordedFeatures()
{
    FIELD_DRAWING_CONTEXT;
    //stringstream ss;
    //ss << GridMapProvider.size();
    string str = "4";
    TEXT_DRAWING(600, - getFieldInfo().yLength / 2 - 450, str);
    TEXT_DRAWING(-600, - getFieldInfo().yLength / 2 - 450, "Compass features: ");
    return;
}

void VisualCompass::resetCounters()
{
    total = 0;
    has_answer = 0;
    sum_angle_error = 0;
    square_sum_angle_error = 0;
}

void VisualCompass::victoria()
{
    // for statistics
    total++;
    double angle_error = 0.0;
    bool updated = false;
    // end

    if(validHorizon() && clustered)
    {
        vector< vector<Pixel> > stripes;
        verticalScanner(stripes);
        VisualCompassFeature tmp;
        tmp.createFeatureFromScanLine(stripes, ClusteringProvider);
        vector<WeightedExperts::output> temp = queryModel.best_match(getRobotPose(), getFieldInfo(), GridMapProvider, tmp);

        double sim = DBL_MAX;
        double ori = 0;
        for(unsigned int i = 0; i < temp.size(); i++)
        {
            if(sim > temp.at(i).confidence)
            {
                sim = temp.at(i).confidence;
                ori = temp.at(i).orientation;
            }
        }
        DEBUG_REQUEST("VisualCompass:debug:draw_orientation_vc", drawCompassOrientation(ori););
        if(temp.size() > 0)
        {
            if(ori != getRobotPose().rotation)
            {
                has_answer++;
                double _angle = abs(ori - getRobotPose().rotation);
                if(_angle > Math::pi)
                {
                    _angle -= Math::pi2;
                }
                angle_error = abs(_angle);
                sum_angle_error += angle_error;
                square_sum_angle_error += (angle_error * angle_error);
                updated = true;
            }
        }

    }

    if (updated)
    {
        std::cout << "Used images | Mean error | MSE " << std::endl;
        std::cout << has_answer << "/" << total << " | ";
        //std::cout << Math::toDegrees(angle_error) << " | ";
        std::cout << sum_angle_error / has_answer << " | ";
        std::cout << square_sum_angle_error / has_answer << " | ";
        std::cout << std::endl;
    }
    return;
}

void VisualCompass::drawInfo()
{
    recordedFeatures();
    return;
}

void inline VisualCompass::initializeGridMapModel()
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
    colorExtraction();
    colorClustering();
    ClusteringProvider.readClusters(str);
    if (ClusteringProvider.getClusters())
        clustered = true;
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
        GridMapProvider.initializeStorageArray();
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

void VisualCompass::drawClusteredColors()
{
    unsigned int clustered = 0;
    for(unsigned int i = 0; i < getImage().width(); i++)
    {
        for (unsigned int j = 0; j < getImage().height(); j++)
        {
            clustered = ClusteringProvider.nearestNeighborIndex(getImage().get(i,j).channels[0], getImage().get(i,j).channels[1],getImage().get(i,j).channels[2]);
            POINT_PX(ColorClasses::Color(clustered), i, j);
        }
    }
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
        // HACK starting from bottom pixel
        start.y = getImage().height() - 1;
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
                    PEN("0000FF", 1);
                    ARROW(lx + x, ly + y,
                          lx + x + (50 + (200 * GridMapProvider.gridmap[i][j][ii].measurement_certainty)) * cos(GridMapProvider.gridmap[i][j][ii].orientation),
                          ly + y + (50 + (200 * GridMapProvider.gridmap[i][j][ii].measurement_certainty)) * sin(GridMapProvider.gridmap[i][j][ii].orientation));
                    PEN("000000", 1);
                }
            }
        }
    }
    return;
}

void VisualCompass::drawCellConfidence()
{
    FIELD_DRAWING_CONTEXT;
    double dx = getFieldInfo().xLength / GRID_X_LENGTH;
    double dy = getFieldInfo().yLength / GRID_Y_LENGTH;
    double lx = - getFieldInfo().xLength / 2 + dx / 2;
    double ly = - getFieldInfo().yLength / 2 + dy / 2;
    for(int i =0; i < GRID_X_LENGTH; i++)
    {
        double x = i * dx;
        for(int j =0; j < GRID_Y_LENGTH; j++)
        {
            double y = j * dy;
            PEN("0000FF", 1);
            FILLBOX(lx + x + dx / 2 - 150, ly + y, lx + x + dx / 2 - 150 + 100 , ly + y + 400 * GridMapProvider.gridCellConfidence[i][j]);
            PEN("000000", 1);
            FILLBOX(lx + x + dx / 2 - 150, ly + y - 2, lx + x + dx / 2 - 150 + 100 , ly + y + 1);
            FILLBOX(lx + x + dx / 2 - 150, ly + y + 401, lx + x + dx / 2 - 150 + 100 , ly + y + 404);
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

void VisualCompass::drawCompassOrientation(double orientation)
{
    FIELD_DRAWING_CONTEXT;
    CIRCLE(getRobotPose().translation.x,
           getRobotPose().translation.y, 50);

    PEN("FF0000", 1);
    ARROW(getRobotPose().translation.x, getRobotPose().translation.y,
          getRobotPose().translation.x + 400*cos(orientation),
          getRobotPose().translation.y + 400*sin(orientation));
    PEN("000000", 1);
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
    TEXT_DRAWING(-300, - getFieldInfo().yLength / 2 - 300, "SavedImgs: ");
    return;
}
