#ifndef MY_SERIAL_H_
#define MY_SERIAL_H_
//#include "serial/serial.h"
#include <aruco/aruco.h>

//
//#define UART_SENT_TYPE_MOUSE 0
//#define UART_SENT_TYPE_CHARACTER 1
//#define UART_SENT_TYPE_TARGET 2
//
//void uartReadThread();
//void enumerate_ports();
//
//extern serial::Serial serial_port;
extern cv::Point3f coordinate_camera;
extern std::vector< aruco::Marker > Markers;
extern float flow_out[2];
void uartSent();
//void Data_pre_character();
//void Data_pre_mouse(int x, int y);
//void Data_pre_target();
//
#endif //MY_SERIAL_H_
