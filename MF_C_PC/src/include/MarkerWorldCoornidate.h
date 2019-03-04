#ifndef __MarkerWorldCoornidate_h__
#define __MarkerWorldCoornidate_h__

#include <opencv2/opencv.hpp>
#include <vector>
#include "inifile.h"
#define MARKER_CONFIG_FILENAME std::string("/home/exbot/SLAM/MF_C/markerconfig.ini")
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
extern int marker_row_num;
extern int marker_col_num;
extern double marker_size;
extern double markers_row_distance;
extern double markers_col_distance;
extern int show_traj,show_camera,save_video,read_video_num,marker_map_num,marker_map_read_num,
map_fix_method,en_map_fix,fast_mode,show_big_map,camera_size_sel;
extern int camera_sel;
extern int use_flow;
extern string cal_file;
#define MARKER_ROW_NUM marker_row_num
#define MARKER_COL_NUM marker_col_num
#define MARKERS_ROW_DISTANCE markers_row_distance
#define MARKERS_COL_DISTANCE markers_col_distance
#define MARKER_SIZE marker_size
void init_sys_para(void);
extern float mark_map[6][5];
class MarkerWorld
{
  public:
    MarkerWorld();
    MarkerWorld(int _id, cv::Point3f _coordinate);
    MarkerWorld(int _id, float _x, float _y, float _z);
    ~MarkerWorld();

    cv::Point3f coordinate;

    int id;

  private:
};

class MarkerWorldCoordinate
{
  public:
    MarkerWorldCoordinate();
    MarkerWorldCoordinate(size_t _size);
    ~MarkerWorldCoordinate();

    size_t size();
    bool setCoordinate(MarkerWorld mw);
    cv::Point3f getCoordinate(int _id);

  private:
    size_t m_size;
    std::vector<MarkerWorld> coorTable;
};

void initCoordinateTable(std::vector<MarkerWorld> &CoordinateTable);

#endif // __MarkerWorldCoornidate_h__
