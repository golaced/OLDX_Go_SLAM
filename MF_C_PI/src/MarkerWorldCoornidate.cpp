#include "MarkerWorldCoornidate.h"
#include "inifile.h"
std::vector<MarkerWorld> CoordinateTable;
float mark_map[6][5];
using namespace inifile;
int marker_row_num;
int marker_col_num;
double marker_size;
double markers_row_distance;
double markers_col_distance;
int show_traj,show_camera,save_video,read_video_num,marker_map_num,
marker_map_read_num,map_fix_method,en_map_fix,fast_mode,show_big_map,camera_size_sel;
int camera_sel;
int use_flow;
string cal_file;
void loadMarkerConfig(const std::string &configfilename)
{
    inifile::IniFile m_inifile;
    m_inifile.load(configfilename);
    std::string section("MarkerConfig");
    int ret = 0;
    marker_row_num = m_inifile.getIntValue(section, "marker_H_num", ret);
    marker_col_num = m_inifile.getIntValue(section, "marker_L_num", ret);
    marker_size = m_inifile.getDoubleValue(section, "marker_size", ret)/100.;
    markers_row_distance = m_inifile.getDoubleValue(section, "markers_H_distance", ret);
    markers_col_distance = m_inifile.getDoubleValue(section, "markers_L_distance", ret);
    camera_sel=m_inifile.getIntValue(section, "camera_sel", ret);
    show_traj=m_inifile.getIntValue(section, "show_traj", ret);
    show_camera=m_inifile.getIntValue(section, "show_camera", ret);
    save_video=m_inifile.getIntValue(section, "save_video", ret);
    read_video_num=m_inifile.getIntValue(section, "read_video_num", ret);
    marker_map_num=m_inifile.getIntValue(section, "marker_map_num", ret);
    map_fix_method=m_inifile.getIntValue(section, "map_fix_method", ret);
    en_map_fix=m_inifile.getIntValue(section, "en_map_fix", ret);
    marker_map_read_num=m_inifile.getIntValue(section, "marker_map_read_num", ret);
    fast_mode=m_inifile.getIntValue(section, "fast_mode", ret);
    show_big_map=m_inifile.getIntValue(section, "show_big_map", ret);
    cal_file=m_inifile.getStringValue(section, "cal_file", ret);
    use_flow=m_inifile.getIntValue(section, "use_flow", ret);
    camera_size_sel=m_inifile.getIntValue(section, "camera_size_sel", ret);
    cout<<"row_num="<<marker_row_num<<"  col_num="<<marker_col_num<<"  size="<<marker_size<<"  ros_dis="<<markers_row_distance<<"  col_dis="<<markers_col_distance<<endl;
}

void loadMap(const std::string &configfilename)
{
    inifile::IniFile m_inifile;
    m_inifile.load(configfilename);
    std::string section("MarkerConfig");
    int ret = 0;
    marker_row_num = m_inifile.getIntValue(section, "marker_H_num", ret);
    marker_col_num = m_inifile.getIntValue(section, "marker_L_num", ret);
    marker_size = m_inifile.getDoubleValue(section, "marker_size", ret)/100.;
    markers_row_distance = m_inifile.getDoubleValue(section, "markers_H_distance", ret);
    markers_col_distance = m_inifile.getDoubleValue(section, "markers_L_distance", ret);
    camera_sel=m_inifile.getIntValue(section, "camera_sel", ret);
    show_traj=m_inifile.getIntValue(section, "show_traj", ret);
    show_camera=m_inifile.getIntValue(section, "show_camera", ret);
    cal_file=m_inifile.getStringValue(section, "cal_file", ret);
    use_flow=m_inifile.getIntValue(section, "use_flow", ret);
    cout<<"row_num="<<marker_row_num<<"  col_num="<<marker_col_num<<"  size="<<marker_size<<"  ros_dis="<<markers_row_distance<<"  col_dis="<<markers_col_distance<<endl;
}


void init_sys_para(void)
{

  loadMarkerConfig(MARKER_CONFIG_FILENAME);

}
void initCoordinateTable(std::vector<MarkerWorld> &CoordinateTable)
{


    for (size_t id = 0; id < 100; id++)
    {
	int x = (id % MARKER_COL_NUM - 1 + (id % MARKER_COL_NUM == 0 ? MARKER_COL_NUM : 0)) * MARKERS_COL_DISTANCE;
	int y = (id / MARKER_COL_NUM - (id % MARKER_COL_NUM == 0 ? 1 : 0)) * MARKERS_ROW_DISTANCE;
	MarkerWorld mw(id, x, y, 0);
	CoordinateTable.push_back(mw);
    }
    //CoordinateTable.push
}

MarkerWorld::MarkerWorld()
{
    coordinate = cv::Point3f(0.0, 0.0, 0.0);
    id = -1;
}

MarkerWorld::MarkerWorld(int _id, float _x, float _y, float _z)
{
    coordinate = cv::Point3f(_x, _y, _z);
    id = _id;
}

MarkerWorld::MarkerWorld(int _id, cv::Point3f _coordinate)
{
    coordinate = cv::Point3f(_coordinate.x, _coordinate.y, _coordinate.z);
    id = _id;
}

MarkerWorld::~MarkerWorld()
{
}

MarkerWorldCoordinate::MarkerWorldCoordinate(size_t _size)
{
    m_size = _size;
    coorTable.resize(m_size, MarkerWorld(0, cv::Point3f(0, 0, 0)));

    for (size_t i = 0; i < m_size; i++)
    {
	coorTable[i].id = i + 1;
    }
}

MarkerWorldCoordinate::MarkerWorldCoordinate()
{
    m_size = 0;
    coorTable.resize(m_size);
}

size_t MarkerWorldCoordinate::size()
{
    return m_size;
}

bool MarkerWorldCoordinate::setCoordinate(MarkerWorld mw)
{
    coorTable[mw.id].coordinate = cv::Point3f(mw.coordinate.x, mw.coordinate.y, mw.coordinate.z);

    return true;
}

cv::Point3f MarkerWorldCoordinate::getCoordinate(int _id)
{
    return coorTable[_id].coordinate;
}

MarkerWorldCoordinate::~MarkerWorldCoordinate()
{
}
