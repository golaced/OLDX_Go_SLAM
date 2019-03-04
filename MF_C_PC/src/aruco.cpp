
#include <iostream>
#include <sstream>
#include<opencv2/opencv.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include "AttitudePosition.h"
#include "MarkerWorldCoornidate.h"
#include "aruco/aruco.h"
#include "aruco/cvdrawingutils.h"
#include "my_serial.h"
#include "AM.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#define USE_PI 0
#define USE_ARUCO3 0
#if USE_PI
#include <serial/serial.h>
#include <wiringSerial.h>
#endif
using namespace std;
using namespace cv;
using namespace aruco;
#define TEST_QR 1
#define TEST_FLOW 0
#define USE_MAP_OUT 1
float flow_out[2]={0};
int fd;
int number=0, i_t,exsit_mark,module_state=0;Mat frame_t1;
Mat frame_golbal[10];
extern std::vector<MarkerWorld> CoordinateTable;
#if USE_PI
serial::Serial serial_port("/dev/ttyAMA0",115200 , serial::Timeout::simpleTimeout(1000));
#endif
cv::Point3f coordinate_camera;
Attitude attitude_camera;
std::vector< aruco::Marker > Markers;
float marker_map[500],marker_map_reg[500],camera_map[4],mark_pos_ero_check,center_fix_flt;
short cloud_size, lock_cnt,first_mark;
Mat traj(720, 720, CV_8UC3, Scalar::all(255));
Mat traj_show(720, 720, CV_8UC3, Scalar::all(255));
Mat map_show_big(720, 720, CV_8UC3, Scalar::all(255));
int video_num = 0;
int write_flag;
int newfile_flag;
std::ifstream video_num_read;
std::ofstream video_num_write;
#if USE_PI
std::string video_num_path("/home/pi/QT/MF_C/video/video_num.txt");
std::string writer_path("/home/pi/QT/MF_C/video/");
#else
std::string video_num_path("/home/exbot/SLAM/MF_C/video/video_num.txt");
std::string writer_path("/home/exbot/SLAM/MF_C/video/");
#endif
string intToString(int v)
{
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%u", v);

    string str = buf;
    return str;
}

void startWriteVideo(Mat in,std::ifstream &video_num_read,
		cv::VideoWriter &video_writer)
{
  static int init;
  static std::stringstream ss;
  static string video_name;
	video_num_read.open(video_num_path.c_str());
	video_num_read >> video_num;
	video_num_read.close();
	if (video_num_write.is_open())
		video_num_write.close();
        video_num=video_num+1;
	video_num_write.open(video_num_path.c_str());
	video_num_write << (video_num);
	video_num_write.close();

	if (video_writer.isOpened())
	{
		video_writer.release();
	}

	ss << video_num;
	ss >> video_name;
	video_name += ".avi";
       cout << "Write Video: "<<video_name << endl;
	video_writer.open(
			writer_path + video_name,
            CV_FOURCC('D', 'I', 'V', 'X'), 15, Size(in.rows, in.cols));
}


void startWriteFile(std::ofstream &writer_txt)
{   static int init_for_map_est=0;
    int i;
	std::stringstream ss;
	string file_name;
	
   if(en_map_fix==1&&marker_map_read_num==0){
    video_num_read.open(video_num_path.c_str());
    video_num_read >> video_num;
    video_num_read.close();
    cout << video_num << endl;
    video_num_write.open(video_num_path.c_str());
    video_num=video_num + 1;
    video_num_write << (video_num );
    video_num_write.close();
   }else{
    video_num_read.open(video_num_path.c_str());
    video_num_read >> video_num;
    video_num_read.close();
   }

    if(init_for_map_est==0){
    init_for_map_est=1;
    video_num=video_num+1;
    video_num_write.open(video_num_path.c_str());
    video_num_write << (video_num );
    video_num_write.close();
   }
	ss << video_num;
	ss >> file_name;
	file_name += ".txt";
    writer_txt.open(writer_path + file_name);
    int x,y,z,yaw;
    cout<<"map write path:"<<file_name<<endl;
    for(i=0;i<100;i++)
    {
        if(marker_map[i + 399]>=1){
        x= marker_map[i - 1]*10;
        y= marker_map[i + 99]*10;
        z= marker_map[i + 199]*10 ;
        yaw= marker_map[i + 299]*10 ;
        writer_txt<<i<<"\t"<<x<<"\t"<<y<<"\t"<<z<<"\t"<<yaw<<endl;
        }
    }
}



void startWriteFile_M(std::ofstream &writer_txt,float dt)
{   static int init_for_map_est=0;
    int i;
std::stringstream ss;
	string file_name;
    if(init_for_map_est==0){
    init_for_map_est=1;
    if(save_video==0)
    video_num=video_num+1;
    video_num_write.open(video_num_path.c_str());
    video_num_write << (video_num );
    video_num_write.close();
    ss << video_num;
    ss >> file_name;
    file_name += "m.txt";
    writer_txt.open(writer_path + file_name);
   }
	
    writer_txt<<int(dt*1000)<<"ms"<<"\t"<<
mark_map[0][0]<<"\t"<<mark_map[0][1]<<"\t"<<mark_map[0][2]<<"\t"<<mark_map[0][3]<<"\t"<<mark_map[0][4]<<"\t"<<
mark_map[1][0]<<"\t"<<mark_map[1][1]<<"\t"<<mark_map[1][2]<<"\t"<<mark_map[1][3]<<"\t"<<mark_map[1][4]<<"\t"<<
mark_map[2][0]<<"\t"<<mark_map[2][1]<<"\t"<<mark_map[2][2]<<"\t"<<mark_map[2][3]<<"\t"<<mark_map[2][4]<<"\t"<<
mark_map[3][0]<<"\t"<<mark_map[3][1]<<"\t"<<mark_map[3][2]<<"\t"<<mark_map[3][3]<<"\t"<<mark_map[3][4]<<"\t"<<
mark_map[4][0]<<"\t"<<mark_map[4][1]<<"\t"<<mark_map[4][2]<<"\t"<<mark_map[4][3]<<"\t"<<mark_map[4][4]<<"\t"<<
mark_map[5][0]<<"\t"<<mark_map[5][1]<<"\t"<<mark_map[5][2]<<"\t"<<mark_map[5][3]<<"\t"<<mark_map[5][4]<<"\t"<<
0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<endl;

}

int cnt_map_read_exist=0;
Point3f Map_aruco[100];
void read_map(void)
{
    std::ifstream map_reader;
    int id=0,x=0,y=0,z=0,yaw=0;
#if USE_PI
    std::string map_path("/home/pi/QT/MF_C/video_r/"+intToString(marker_map_read_num)+".txt");
    #else
    std::string map_path("/home/exbot/SLAM/MF_C/video_r/"+intToString(marker_map_read_num)+".txt");
#endif
    map_reader.open(map_path.c_str());
    cout<<"read Map:"<<map_path<<endl;
    while(!map_reader.eof())
    {
    char temp[255]={0};
    map_reader.getline(temp,255);
    sscanf(temp,"%d\t%d\t%d\t%d\t%d",&id,&x,&y,&z,&yaw);

    marker_map[id + 399]=2;
    Map_aruco[id].x=marker_map_reg[id - 1]=marker_map[id - 1]=(float)x/10.;
    Map_aruco[id].y=marker_map_reg[id + 99]=marker_map[id + 99]=(float)y/10.;
    marker_map_reg[id + 199]=marker_map[id + 199]=(float)z/10.;
    Map_aruco[id].z=marker_map_reg[id + 299]=marker_map[id + 299]=(float)yaw/10.;

    cout<<"Mark id:"<<id<<" x:"<<marker_map[id -1]<<" y:"<< marker_map[id + 99]<<" yaw:"<<marker_map[id + 299]<<endl;
    cnt_map_read_exist++;
    ostringstream ostr_map;
    ostr_map.clear();
    ostr_map.str("");
    ostr_map <<" "<<id;
    //putText(map_show, ostr_map.str(), Point(int(marker_map[id - 1]*1.23+600), int(marker_map[id + 99]*1.23+600)), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 0, 0), 2);

    }
    cout<<"read finish!!!!"<<endl;
    map_reader.close();
}

static bool SortByDown(const float &p1, const float &p2)
{
    return p1 > p2;
}


void locate_from_map(vector< aruco::Marker > Markers, vector<Point3f>posm, vector<Attitude> attm, Point3f &posc, float &yawc)
{
  vector <float> pos_bufx,pos_bufy,pos_bufz;
  vector <float> yaw_buf;
  for (unsigned int i = 0; i < Markers.size(); i++){
    if(Map_aruco[Markers[i].id].x+Map_aruco[Markers[i].id].y!=0)
    {
        float tempy;
        Point3f pos_temp;
        getCameraPos(Markers[i].Rvec, Markers[i].Tvec, posm[i]);//camera to local marker o dis
        getAttitude(Markers[i], attm[i]);
        tempy=(float)(attm[i].Yaw+Map_aruco[Markers[i].id].z);
        yaw_buf.push_back(tempy);
        pos_temp.x=cos(Map_aruco[Markers[i].id].z*0.0173)*posm[i].x+sin(Map_aruco[Markers[i].id].z*0.0173)*posm[i].y;
        pos_temp.y=-sin(Map_aruco[Markers[i].id].z*0.0173)*posm[i].x+cos(Map_aruco[Markers[i].id].z*0.0173)*posm[i].y;
        pos_temp.z=posm[i].z;
        pos_bufx.push_back(pos_temp.x+Map_aruco[Markers[i].id].x);
        pos_bufy.push_back(pos_temp.y+Map_aruco[Markers[i].id].y);
        pos_bufz.push_back(pos_temp.z);
    }
  }

  if(pos_bufx.size())
  {
       sort(pos_bufx.begin(), pos_bufx.end(), SortByDown);
       sort(pos_bufy.begin(), pos_bufy.end(), SortByDown);
       sort(pos_bufz.begin(), pos_bufz.end(), SortByDown);
       sort(yaw_buf.begin(), yaw_buf.end(), SortByDown);

      if(pos_bufx.size()>3)
      {   float tempy;
          Point3f pos_temp;
          int cnt=0;
          for(int i=1;i<pos_bufx.size()-1;i++)
          {
              cnt++;
              tempy+=yaw_buf[i];
              pos_temp.x+=pos_bufx[i];
              pos_temp.y+=pos_bufy[i];
              pos_temp.z+=pos_bufz[i];
          }
          posc.x=pos_temp.x/cnt; posc.y=pos_temp.y/cnt; posc.z=pos_temp.z/cnt;
          yawc=tempy/cnt;
      }else
      {
          float tempy;
          Point3f pos_temp;
          int cnt=0;
          for(int i=0;i<pos_bufx.size();i++)
          {
              cnt++;
              tempy+=yaw_buf[i];
              pos_temp.x+=pos_bufx[i];
              pos_temp.y+=pos_bufy[i];
              pos_temp.z+=pos_bufz[i];
          }
          posc.x=pos_temp.x/cnt;posc.y=pos_temp.y/cnt; posc.z=pos_temp.z/cnt;
          yawc=tempy/cnt;
      }
  }
}

int key_cnt=66;
void uartSent()
{  unsigned char data_to_send[50];
    int _cnt = 0, i = 0, sum = 0;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAF;
    data_to_send[_cnt++] = 0x21;
    data_to_send[_cnt++] = 0;

    data_to_send[_cnt++] = Markers.size();
    data_to_send[_cnt++] = int(coordinate_camera.x)>>8;
    data_to_send[_cnt++] = int(coordinate_camera.x)%256;
    data_to_send[_cnt++] = int(coordinate_camera.y)>>8;
    data_to_send[_cnt++] = int(coordinate_camera.y)%256;
    data_to_send[_cnt++] = int(coordinate_camera.z)>>8;
    data_to_send[_cnt++] = int(coordinate_camera.z)%256;
    data_to_send[_cnt++] = int(attitude_camera.Pit)>>8;
    data_to_send[_cnt++] = int(attitude_camera.Pit)%256;
    data_to_send[_cnt++] = int(attitude_camera.Rol)>>8;
    data_to_send[_cnt++] = int(attitude_camera.Rol)%256;
    data_to_send[_cnt++] = int(attitude_camera.Yaw)>>8;
    data_to_send[_cnt++] = int(attitude_camera.Yaw)%256;
    data_to_send[_cnt++] = int(flow_out[0])>>8;
    data_to_send[_cnt++] = int(flow_out[0])%256;
    data_to_send[_cnt++] = int(flow_out[1])>>8;
    data_to_send[_cnt++] = int(flow_out[1])%256;
//map
    data_to_send[_cnt++] = int(mark_map[0][0])>>8;
    data_to_send[_cnt++] = int(mark_map[0][0])%256;
    data_to_send[_cnt++] = int(mark_map[0][1])>>8;
    data_to_send[_cnt++] = int(mark_map[0][1])%256;
    data_to_send[_cnt++] = int(mark_map[0][2])>>8;
    data_to_send[_cnt++] = int(mark_map[0][2])%256;
    data_to_send[_cnt++] = int(mark_map[0][3])>>8;
    data_to_send[_cnt++] = int(mark_map[0][3])%256;
    data_to_send[_cnt++] = int(mark_map[0][4]);//id

    data_to_send[_cnt++] = int(mark_map[1][0])>>8;
    data_to_send[_cnt++] = int(mark_map[1][0])%256;
    data_to_send[_cnt++] = int(mark_map[1][1])>>8;
    data_to_send[_cnt++] = int(mark_map[1][1])%256;
    data_to_send[_cnt++] = int(mark_map[1][2])>>8;
    data_to_send[_cnt++] = int(mark_map[1][2])%256;
    data_to_send[_cnt++] = int(mark_map[1][3])>>8;
    data_to_send[_cnt++] = int(mark_map[1][3])%256;
    data_to_send[_cnt++] = int(mark_map[1][4]);//id

    data_to_send[3] = _cnt - 4;
    for (i = 0; i < _cnt; i++)
        sum += data_to_send[i]+key_cnt;
    data_to_send[_cnt++] = sum;
   int Length = _cnt;
#if USE_PI
    serial_port.write(data_to_send, Length);
#endif
}


void uartSent2()
{  unsigned char data_to_send[50];
    int _cnt = 0, i = 0, sum = 0;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAF;
    data_to_send[_cnt++] = 0x22;
    data_to_send[_cnt++] = 0;
//map
    data_to_send[_cnt++] = int(mark_map[2][0])>>8;
    data_to_send[_cnt++] = int(mark_map[2][0])%256;
    data_to_send[_cnt++] = int(mark_map[2][1])>>8;
    data_to_send[_cnt++] = int(mark_map[2][1])%256;
    data_to_send[_cnt++] = int(mark_map[2][2])>>8;
    data_to_send[_cnt++] = int(mark_map[2][2])%256;
    data_to_send[_cnt++] = int(mark_map[2][3])>>8;
    data_to_send[_cnt++] = int(mark_map[2][3])%256;
    data_to_send[_cnt++] = int(mark_map[2][4]);//id

    data_to_send[_cnt++] = int(mark_map[3][0])>>8;
    data_to_send[_cnt++] = int(mark_map[3][0])%256;
    data_to_send[_cnt++] = int(mark_map[3][1])>>8;
    data_to_send[_cnt++] = int(mark_map[3][1])%256;
    data_to_send[_cnt++] = int(mark_map[3][2])>>8;
    data_to_send[_cnt++] = int(mark_map[3][2])%256;
    data_to_send[_cnt++] = int(mark_map[3][3])>>8;
    data_to_send[_cnt++] = int(mark_map[3][3])%256;
    data_to_send[_cnt++] = int(mark_map[3][4]);//id

    data_to_send[_cnt++] = int(mark_map[4][0])>>8;
    data_to_send[_cnt++] = int(mark_map[4][0])%256;
    data_to_send[_cnt++] = int(mark_map[4][1])>>8;
    data_to_send[_cnt++] = int(mark_map[4][1])%256;
    data_to_send[_cnt++] = int(mark_map[4][2])>>8;
    data_to_send[_cnt++] = int(mark_map[4][2])%256;
    data_to_send[_cnt++] = int(mark_map[4][3])>>8;
    data_to_send[_cnt++] = int(mark_map[4][3])%256;
    data_to_send[_cnt++] = int(mark_map[4][4]);//id

    data_to_send[_cnt++] = int(mark_map[5][0])>>8;
    data_to_send[_cnt++] = int(mark_map[5][0])%256;
    data_to_send[_cnt++] = int(mark_map[5][1])>>8;
    data_to_send[_cnt++] = int(mark_map[5][1])%256;
    data_to_send[_cnt++] = int(mark_map[5][2])>>8;
    data_to_send[_cnt++] = int(mark_map[5][2])%256;
    data_to_send[_cnt++] = int(mark_map[5][3])>>8;
    data_to_send[_cnt++] = int(mark_map[5][3])%256;
    data_to_send[_cnt++] = int(mark_map[5][4]);//id

    data_to_send[_cnt++] = int(exsit_mark);
    data_to_send[_cnt++] = int(module_state);

//
    data_to_send[3] = _cnt - 4;

    for (i = 0; i < _cnt; i++)
        sum += data_to_send[i];
    data_to_send[_cnt++] = sum+key_cnt;
   int Length = _cnt;

#if USE_PI
    serial_port.write(data_to_send, Length);
#endif
}

void initTraj(cv::Mat &traj, float rectsize, float offset)
{
    Point origin(offset, offset);
    for (size_t i = 0; i < marker_col_num*marker_row_num; i++)
    {
        Rect rect((CoordinateTable[i].coordinate.x - rectsize / 2) * 2 + origin.x, (CoordinateTable[i].coordinate.y - rectsize / 2) * 2 + origin.y, rectsize * 2, rectsize * 2);

        rectangle(traj, rect, Scalar(0, 0, 0), 2);
    }
    cv::line(traj, origin, Point(origin.x, 690), Scalar(72, 61, 139), 2);
    cv::line(traj, origin, Point(690, origin.y), Scalar(72, 61, 139), 2);

    cv::line(traj, Point(690, origin.y), Point(690 - 20, origin.y - 10), Scalar(72, 61, 139), 2);
    cv::line(traj, Point(690, origin.y), Point(690 - 20, origin.y + 10), Scalar(72, 61, 139), 2);

    cv::line(traj, Point(origin.x, 690), Point(origin.x - 10, 690 - 10), Scalar(72, 61, 139), 2);
    cv::line(traj, Point(origin.x, 690), Point(origin.x + 10, 690 - 10), Scalar(72, 61, 139), 2);
}
bool isEnd = false;
VideoCapture cap;

Mat Filter_aruco(Mat in,int user_threshold)
{
  Mat I_filtered,I;
  if(user_threshold > 0)
  {
  cv::cvtColor(in, I, COLOR_BGR2GRAY);
  //cv::GaussianBlur(I, I_filtered, cv::Size(0,0),2);
  //cv::addWeighted(I, 2.5, I_filtered, -1.5, 0, I_filtered);
  cv::equalizeHist(I,I_filtered);
  cv::threshold(I_filtered,I_filtered,user_threshold,0,3);
 }else
      in.copyTo(I_filtered);
  return I_filtered;
}


void warpFfine(cv::Mat &inputIm, cv::Mat &tempImg, float angle)
{
    CV_Assert(!inputIm.empty());
    Mat inputImg;
    inputIm.copyTo(inputImg);
    float radian = (float) (angle / 180.0 * CV_PI);
    int uniSize = (int) (max(inputImg.cols, inputImg.rows) * 1.414);
    int dx = (int) (uniSize - inputImg.cols) / 2;
    int dy = (int) (uniSize - inputImg.rows) / 2;
    copyMakeBorder(inputImg, tempImg, dy, dy, dx, dx, BORDER_CONSTANT);
    Point2f center((float) (tempImg.cols / 2), (float) (tempImg.rows / 2));
    Mat affine_matrix = getRotationMatrix2D(center, angle, 1.0);
    warpAffine(tempImg, tempImg, affine_matrix, tempImg.size());
    float sinVal = fabs(sin(radian));
    float cosVal = fabs(cos(radian));
    Size targetSize((int) (inputImg.cols * cosVal + inputImg.rows * sinVal),
            (int) (inputImg.cols * sinVal + inputImg.rows * cosVal));
    int x = (tempImg.cols - targetSize.width) / 2;
    int y = (tempImg.rows - targetSize.height) / 2;
    Rect rect(x, y, targetSize.width, targetSize.height);
    tempImg = Mat(tempImg, rect);
}


char u_key[20]="C1414526A0CC8E9ED80";//   SET you Lisence Here!!!!!!!!!!!!!!!!!!!!
float cpu_speed_w=1;
void thread_marker(void)
{
    Mat InImage;
    Mat pic_t1;
    cout<<"CV_VERSION"<<CV_VERSION<<endl;
    initCoordinateTable(CoordinateTable);
    cout<<cal_file<<endl;
    int SH=320,SW=240;
    switch(camera_size_sel){
    case 0:SH=320;SW=240;break;
    case 1:SH=240;SW=240;break;
    case 2:SH=128;SW=128;break;
    }
    cv::Size InImage_size(SH,SW);

float MAP_SHOW_SCALE=0.4;
if(show_big_map)MAP_SHOW_SCALE=0.816;
Mat map_show(720*MAP_SHOW_SCALE, 720*MAP_SHOW_SCALE, CV_8UC3, Scalar::all(255));
    aruco::CameraParameters CamParam;
    MarkerDetector MDetector;
    float MarkerSize = MARKER_SIZE;
    int p1 = 7;
    int p2 = 7;
    int t_p_range = 0;
    int f_thr=0;
    int subpix=1;
    int cloud_sizep=22;//30;
    int mark_pos_ero_checkp=6*10;
    int lock_cntp=2;
    int center_fix_fltp=66;//0.36*100;
    int use_map_fix_force=0;
    if(show_camera||show_traj){
    cv::namedWindow("thes", 1);
    //createTrackbar("p1", "thes", &p1, 101);
    //createTrackbar("p2", "thes", &p2, 50);
    createTrackbar("range", "thes", &t_p_range, 31);
    createTrackbar("filter", "thes", &f_thr, 255);
    //createTrackbar("subpix", "thes", &subpix, 20);
    createTrackbar("cloud_size", "thes", &cloud_sizep, 49);
    createTrackbar("ero_fix", "thes", &mark_pos_ero_checkp, 200);
    createTrackbar("fix_cnt", "thes", &lock_cntp, 20);
    createTrackbar("fix_step", "thes", &center_fix_fltp, 100);
    createTrackbar("en_fix", "thes", &use_map_fix_force, 2);
    }
    ostringstream ostr_pos;
    ostringstream ostr_angle;
    ostringstream ostr_map;
    Point3f pos_camera(0, 0, 0);
     int map_save=0;
    float offset = 50;
    initTraj(traj, marker_size*100, offset);
    Mat traj_empty = traj.clone();
    int drawPointSize = 25;
    vector<Point2f> drawPointKF;
    vector<Point2f> drawPointSrc;
    drawPointKF.resize(drawPointSize,Point2f(0,0));
    drawPointSrc.resize(drawPointSize, Point2f(0, 0));
    
    cv::TickMeter tm,tm_uart;
    VideoWriter video_writer;
    std::ofstream file_writer,file_writer_M;
    #if USE_PI
      string video_read_s("/home/pi/QT/MF_C/video_r/"+intToString(read_video_num)+".avi");
   #else
      string video_read_s("/home/exbot/SLAM/MF_C/video_r/"+intToString(read_video_num)+".avi");
   #endif
    VideoCapture cap1(video_read_s);
    cout<<"read_video"<<video_read_s<<endl;
    if(marker_map_read_num!=0)
     read_map();
    CamParam.readFromXMLFile(cal_file);
    CamParam.resize(InImage_size);
    cout << CamParam.CameraMatrix << endl;
    cout << CamParam.Distorsion << endl;
    set_key(u_key);
    for(;;)
    {
        int bad_frame=0;
        tm.reset();
        tm.start();
        module_state=0;
        if(read_video_num==0){
        cap >> pic_t1;
        if(!pic_t1.empty())
        cv::resize(Filter_aruco(pic_t1,f_thr), InImage, InImage_size);
        else
        bad_frame=1;
        }
        if(read_video_num!=0){
        cap1>>pic_t1;
        if(!pic_t1.empty())
        cv::resize(Filter_aruco(pic_t1,f_thr), InImage, InImage_size);
        else
        bad_frame=1;
        }
   if(!bad_frame){
       if(read_video_num==0)
	warpFfine(InImage,InImage,180);
        p1 = p1 / 2 * 2 + 1;
        p2 = p2 / 2 * 2 + 1;
#if !USE_ARUCO3
        MDetector.setThresholdParamRange(t_p_range);
        MDetector.setThresholdParams(p1, p2);
        MDetector.setCornerRefinementMethod(MDetector.SUBPIX ,subpix);//=1;//.minCornerDistance()
        MDetector.setThresholdMethod(MDetector.ADPT_THRES);
        MDetector.setMinMaxSize(0.03,0.5);
#else 
       if(fast_mode)
        MDetector.setDetectionMode(DM_FAST);
        //MDetector.setDictionary();
        //MDetector.setMarkerLabeler();
#endif
        MDetector.detect(InImage, Markers, CamParam, MarkerSize);
      
        static int flag=0;
       if (save_video&&!flag)
       {
           video_writer.release();
           startWriteVideo(InImage,video_num_read, video_writer);
           flag = 1;
       }
       if(flag){
	   video_writer << InImage;
       }

        for (unsigned int i = 0; i < Markers.size(); i++)
        { 
	  if(show_camera)
            Markers[i].draw(InImage, Scalar(0, 0, 255), 2);
        }
        mark_map_publish(Markers);
        float check_in[6][5]={0};int j=0;
	 vector<Point3f>posm;
	 vector<Attitude > attm;
	 float yaw_m;
        if (CamParam.isValid() && MarkerSize != -1 && 1)
        {
            for (unsigned int i = 0; i < Markers.size(); i++)
            {
                getAttitude(Markers[i], attitude_camera);
		getCameraPos(Markers[i].Rvec, Markers[i].Tvec, pos_camera);
		posm.push_back(pos_camera);
		attm.push_back(attitude_camera);
		if(show_camera){
                ostr_angle.clear();
                ostr_angle.str("");
                ostr_angle << "          Y=" << (int)attitude_camera.Yaw ;//<< " " << "Rol=" << (int)attitude_camera.Rol;       
		putText(InImage, ostr_angle.str(), Markers[i].getCenter() + Point2f(-100, -35), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(55, 255, 0), 2);
                ostr_pos.clear();
                ostr_pos.str("");
                ostr_pos << "          x=" << (int)pos_camera.x;// << " " << "y=" << (int)pos_camera.y ;//<< " " << "z=" << (int)pos_camera.z;
                putText(InImage, ostr_pos.str(), Markers[i].getCenter() + Point2f(-100, -20), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(55, 255, 0), 2);
                ostr_pos.clear();
                ostr_pos.str("");
                ostr_pos << "          y=" << -(int)pos_camera.y ;//<< " " << "y=" << (int)pos_camera.y ;//<< " " << "z=" << (int)pos_camera.z;
                putText(InImage, ostr_pos.str(), Markers[i].getCenter() + Point2f(-100, 20), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(55, 255, 0), 2);
		}
	      if(j<8&&en_map_fix){
	      check_in[j][0]=pos_camera.x;
	      check_in[j][1]=-pos_camera.y;
	      check_in[j][2]=pos_camera.z;
	      check_in[j][3]=attitude_camera.Yaw;
	      check_in[j][4]=Markers[i].id;
	       j++;
	      }
	    }
	    locate_from_map(Markers, posm,attm,   coordinate_camera,yaw_m);
        }

        #define PI3_DT 88
        static float dt=PI3_DT;
        cpu_speed_w=1-LIMIT(LIMIT((dt-PI3_DT),0,2*PI3_DT)/PI3_DT,0.0,0.5);
        cloud_size=cloud_sizep*cpu_speed_w;
        mark_pos_ero_check=(float)mark_pos_ero_checkp/10.;
        lock_cnt=lock_cntp;
        center_fix_flt=(float)center_fix_fltp/100.;
        static short init_map=1;
        static short has_map;
        int en_map_fix_temp=en_map_fix;
        if(use_map_fix_force>=1)
          en_map_fix=use_map_fix_force-1;
        if(en_map_fix)
	  AM( marker_map,  camera_map, &first_mark,check_in[0],check_in[1],check_in[2],check_in[3],check_in[4],check_in[5],
                cloud_size,  mark_pos_ero_check,  lock_cnt, center_fix_flt,&init_map,map_fix_method,en_map_fix_temp,&has_map);

        static float map_draw[100][6];
        static int cnt_draw_jra=0;

         j=0;
        float k=1.23*MAP_SHOW_SCALE;
        int offd=300*MAP_SHOW_SCALE,//map show offset 
	map_cnt=0;
	static int cnt_reset_draw_map;
	Mat white_img1(720*MAP_SHOW_SCALE, 720*MAP_SHOW_SCALE, CV_8UC3, Scalar::all(255));
	if(cnt_reset_draw_map++>36&&show_traj){cnt_reset_draw_map=0;
	  white_img1.copyTo(map_show);
	  for(int i=0;i<100;i++)
	     map_draw[i][5]=0;
	}
        for(int i=0;i<100;i++){
         if(marker_map[i + 399]>=1&&show_traj){
             map_cnt++;
             map_draw[i][0]= marker_map[i - 1]*k;
             map_draw[i][1]= marker_map[i + 99]*k;
             map_draw[i][2]= marker_map[i + 199] ;
             map_draw[i][3]= marker_map[i + 299] ;

             map_draw[i][4]=i;
             
             int rectsize=marker_size*100*0.6*MAP_SHOW_SCALE;
             Rect rect((map_draw[i][0]- rectsize / 2)+offd-rectsize/2 ,
                       (map_draw[i][1]- rectsize / 2)+offd-rectsize/2 ,
                       rectsize * 2, rectsize * 2);
	     if(marker_map[i + 399]>=2){//fixed
             rectangle(map_show, rect, CV_RGB(0, 0, 255), 1);
	     circle(map_show, Point(map_draw[i][0]+offd, map_draw[i][1]+offd),1, CV_RGB(0, 0, 255),1.4);}
	     else{
	     rectangle(map_show, rect, CV_RGB(255, 0, 0), 1);   
	     circle(map_show, Point(map_draw[i][0]+offd, map_draw[i][1]+offd),1, CV_RGB(255, 0, 0),1.4);}
             if(map_draw[i][5]==0){
             ostr_map.clear();
             ostr_map.str("");
             ostr_map <<" "<<i;
             putText(map_show, ostr_map.str(), Point(map_draw[i][0]+offd, map_draw[i][1]+offd), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(55, 55, 0), 2*MAP_SHOW_SCALE);
             cout<<"==============Fixed mark: "<<i<<" x:"<<map_draw[i][0]<<" y:"<<map_draw[i][1]<<"==========="<<endl;
             map_draw[i][5]=1;
             }
         }
        }
	
        int cnt_map_change=0;
        if(marker_map_read_num!=0||0)//update saving
        {
            for(int i=0;i<100;i++)
            {
                if(sqrt(pow(marker_map[i - 1]-marker_map_reg[i - 1],2)+pow(marker_map[i + 199]-marker_map_reg[i + 199],2))
                 >marker_size*1.618)
                cnt_map_change++;
                if(cnt_map_change>100*0.3){

                    for(int id=0;id<100;id++)
                    {
                    marker_map_reg[id - 1]=marker_map[id - 1];
                    marker_map_reg[id + 99]=marker_map[id + 99];
                    marker_map_reg[id + 199]=marker_map[id + 199];
                    marker_map_reg[id + 299]=marker_map[id + 299];
                    }
                    cout << "save map!!!!!!" << endl;
                    file_writer.close();
                    startWriteFile(file_writer);
                 break;
                }
            }
        }
        else if(map_cnt>=marker_map_num&&map_save==0&&read_video_num==0)
        {
            map_save=1;
            cout << "save map!!!!!!" << endl;
            file_writer.close();
            startWriteFile(file_writer);
            module_state=3;
        }
        
        if(!en_map_fix){
        float temp;
        camera_map[0]=coordinate_camera.x;camera_map[1]=coordinate_camera.y;attitude_camera.Yaw=yaw_m;//temp;
        }
        
        if(cnt_draw_jra++>0&&show_traj){
            cnt_draw_jra=0;
            circle(map_show, Point(camera_map[0]*k+offd, camera_map[1]*k+offd),1, CV_RGB(255, 0, 0),2);
        }

       if(show_traj){
        cv::imshow("map", map_show);
        ostr_pos.clear();
        ostr_pos.str("");
        ostr_pos << "          x=" << (int)coordinate_camera.x << " " << "y=" << (int)coordinate_camera.y<< " z=" << (int)coordinate_camera.z;

        putText(InImage, ostr_pos.str(), Point(-60, 30), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(55, 55, 0), 2);
        ostr_pos.clear();
        ostr_pos.str("");
        ostr_pos << "          Y=" << (int)attitude_camera.Yaw<<" P="<<(int)attitude_camera.Pit<<" R="<<(int)attitude_camera.Rol;

        putText(InImage, ostr_pos.str(), Point(-60, 50), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(55, 55, 0), 2);
        //putText(map_show, ostr_pos.str(), Point(-60, 30), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(55, 55, 0), 2);
        static float x_show = 0, y_show = 0;
        static float x = 0;
        static float y = 0;
        if ((int(coordinate_camera.x) != 0) && (int(coordinate_camera.y) != 0))
        {
            x = (coordinate_camera.x) * 2;
            y = (coordinate_camera.y) * 2;
        }

        x_show = x;
        y_show = y;

        drawPointSrc.insert(drawPointSrc.begin(), Point(x_show, y_show) + Point(offset, offset));
        drawPointSrc.pop_back();
        int draw_size=drawPointSrc.size()*0.2;//draw length
        for (size_t i = 0; i < draw_size; i++)
        {
            circle(traj, drawPointSrc[i], 3, CV_RGB(255, 0, 0), 1);
        }

        circle(traj, Point(x_show, y_show)+Point(offset,offset),3, CV_RGB(255, 0, 0), 1);
        traj.copyTo(traj_show);
}
        if(show_camera)
        cv::imshow("thes", InImage);
        //cv::imshow("thes", MDetector.getThresholdedImage());
        tm_uart.reset();tm_uart.start();
        uartSent();uartSent2();
        tm_uart.stop();
        if(show_traj&&0){traj = traj_empty.clone();// stander 100 markers 
        cv::imshow("in", traj_show);}
        tm.stop();
        static int cnt_time_show;
        if(cnt_time_show++>3*cpu_speed_w){cnt_time_show=0;
        cout<<"exsit_mark: "<<map_cnt<<"  x=: "<<coordinate_camera.x<<"   y=: "
           <<coordinate_camera.y<<"   Yaw=: "<<attitude_camera.Yaw<<"  dt:"
          <<tm.getTimeMilli()<<"ms"<<"  dt_tx:"<<tm_uart.getTimeMilli()<<"ms"<<"  cpu_w:"<<cpu_speed_w<<endl;
        }
	static float time;
        time+=(float)tm.getTimeMilli()/1000.;
	startWriteFile_M(file_writer_M,time);
        exsit_mark=map_cnt;
        dt=tm.getTimeMilli();
}//bad_frame
else{
  cout<<"bad_frame"<<endl;
    if(map_save==0){
    map_save=1;
    cout << "save map!!!!!!" << endl;
    file_writer.close();
    startWriteFile(file_writer);
    module_state=3;
    }
}
#if USE_PI
        char c = (char)waitKey(20);
#else
        char c = (char)waitKey(100);
#endif
        if( c == 's' &&map_save==0){
	      map_save=1;
	      cout << "save map!!!!!!" << endl;
	      file_writer.close();
	      startWriteFile(file_writer);
	      module_state=3;
	      }
	else if(c == 27)  
            break;
    }
    video_writer.release();
    cout << "video_writer.release();" << endl;
    file_writer.close();
    cout << "file_writer.close();" << endl;
}


int main(void)
{
    try
    {
#if USE_PI
        while((fd = serialOpen ("/dev/ttyAMA0",115200))<0)
        {
            cout<<"serial err"<<endl;
            usleep(2000*1000);
        }
#endif       
        init_sys_para();
#if USE_PI
        cap.open(camera_sel);
#endif
        usleep(2000*1000);
        thread_marker();
        return 0;
    }
    catch (std::exception &ex)
    {
        cout << "Exception :" << ex.what() << endl;
    }
}
