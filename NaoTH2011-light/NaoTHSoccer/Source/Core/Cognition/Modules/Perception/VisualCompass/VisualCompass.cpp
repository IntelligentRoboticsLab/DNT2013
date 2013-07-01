#include "VisualCompass.h"
using namespace std;
using namespace cv;

struct Vertex
{
    float x, y, z;
};

typedef vector<Vertex> VertexList;

VisualCompass::VisualCompass()
{
    DEBUG_REQUEST_REGISTER("VisualCompass:mark_area", "mark the possibles' features area", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:scan_lines", "", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:draw_orientation_loc","from localization", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:draw_orientation_vc","from visual compass", false);
    DEBUG_REQUEST_REGISTER("VisualCompass:draw_visual_grid_map"," dsgew", false);

    // motions
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:standard_stand", "stand as standard or not", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:stand", "The default motion, otherwise do nothing", true);
    DEBUG_REQUEST_REGISTER("VisualCompass:motion:turn_right", "Set the motion request to 'turn_right'.", false);
}

VisualCompass::~VisualCompass()
{
}

bool VisualCompass::saveModel()
{
    // Create a list for testing
    VertexList list;
    Vertex v1 = {1.0f, 2.0f,   3.0f}; list.push_back(v1);
    Vertex v2 = {2.0f, 100.0f, 3.0f}; list.push_back(v2);
    Vertex v3 = {3.0f, 200.0f, 3.0f}; list.push_back(v3);
    Vertex v4 = {4.0f, 300.0f, 3.0f}; list.push_back(v4);

    // Write out a list to a disk file
    ofstream os (COMPASS_DATA_FILE, ios::binary);

    int size1 = list.size();
    os.write((const char*)&size1, 4);
    os.write((const char*)&list[0], size1 * sizeof(Vertex));
    os.close();
    return true;
}

void VisualCompass::clearCompass()
{
    // check if there is a model file and delete it
    if(hasModel()){
        remove(COMPASS_DATA_FILE);
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
    saveModel();

    scanner();
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
            double dx = getFieldInfo().xLength / GRID_X_LENGTH;
    for(int i = 1; i < GRID_X_LENGTH; i++)
    {
        LINE(- getFieldInfo().xLength / 2 + dx * i, getFieldInfo().yLength / 2, - getFieldInfo().xLength / 2 + dx * i, - getFieldInfo().yLength / 2);
    }
    double dy = getFieldInfo().yLength / GRID_Y_LENGTH;
    for(int i = 1; i < GRID_Y_LENGTH; i++)
    {
        LINE(getFieldInfo().xLength / 2, - getFieldInfo().yLength / 2 + dy * i, - getFieldInfo().xLength / 2, - getFieldInfo().yLength / 2 + dy * i);
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
    vector<Math::LineSegment> img_boundaries;
    img_boundaries.push_back(Math::LineSegment(Vector2<double>(0, 0), Vector2<double>(getImage().width()-1, 0)));
    img_boundaries.push_back(Math::LineSegment(Vector2<double>(0, 0), Vector2<double>(0, getImage().height()-1)));
    img_boundaries.push_back(Math::LineSegment(Vector2<double>(getImage().width()-1, 0), Vector2<double>(getImage().width()-1, getImage().height()-1)));
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
