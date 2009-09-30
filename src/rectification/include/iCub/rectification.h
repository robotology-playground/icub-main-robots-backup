/** 
 * @ingroup icub_module
 *
 * \defgroup icub_rectification rectification
 *
 * Rectify two images generated by two verging cameras to remove the epipolar distortion.
 *
 * The approach is a direct implementation of the paper by Dankers et al. 2004, which in turn is based on 
 * an adaptation of a technique described in Fusiello et al. 2000.
 *
 * In addition to rectifying the images as detailed in Dankers et al 2004, we also shift the right image
 * with respect to the left image to compensate for differences in the positions of the principal points
 * in the left and right images.  
 * 
 * References.\n
 * A. Dankers, N. Barnes, and A. Zelinsky, 2004.  Active Vision - Rectification and Depth Mapping, 
 * Proc. 2004 Australian Conference on Robotics and Automation. \n
 * A. Fusiello, E. Trucco, and A. Verri, 2000.  A Compact Algorithms for rectification of stereo pairs, 
 * Machine Vision and Applications, Vol. 12, pp. 16-22.
 *
 * 
 * \section lib_sec Libraries
 *
 * YARP.
 *
 * \section parameters_sec Parameters
 * 
 * Command-line \verbatim Parameters \endverbatim
 * 
 * The following key-value pairs can be specified as command-line parameters by prefixing \c -- to the key 
 * (e.g. \c --from \c file.ini ). The value part can be changed to suit your needs; the default values are shown below. 
 *
 * - \c from \c rectification.ini       \n
 *   specifies the configuration file
 *
 * - \c context \c rectification/conf  \n 
 *   specifies the sub-path from \c $ICUB_ROOT/icub/app to the configuration file
 *
 * - \c name \c rectification \n         
 *   specifies the name of the module (used to form the stem of module port names)  
 *
 * - \c robot \c icub  \n           
 *   specifies the name of the robot (used to form the root of robot port names)
 *
 * - \c cameraConfig \c iCubEyes.ini \n
 *
 *   This is a file with at least the intrinsic camera parameters for the left and right cameras,
 *   listed under to group headings \c [CAMERA_CALIBRATION_LEFT] and \c [CAMERA_CALIBRATION_RIGHT]
 *   The parameters that are required for rectification are \c fx, \c fy, \c cx, \c cy
 *
 *   \c [CAMERA_CALIBRATION_LEFT]
 *   
 *   \c w \c 320 \n
 *   \c h \c 240 \n
 *   \c fx \c 225.131 \n
 *   \c fy \c 226.75 \n
 *   \c cx \c 174.222 \n
 *   \c cy \c 141.757 \n
 *   \c k1 \c -0.37542 \n
 *   \c k2 \c 0.165345 \n
 *   \c p1 \c -0.000305031 \n
 *   \c p2\c  0.00148445 \n
 *   
 *   \c [CAMERA_CALIBRATION_RIGHT]
 * 
 *   \c w \c 320 \n
 *   \c h \c 240 \n
 *   \c fx \c 225.904 \n
 *   \c fy \c 227.041 \n
 *   \c cx \c 157.858 \n
 *   \c cy \c 113.51 \n
 *   \c k1 \c -0.377318 \n
 *   \c k2 \c 0.155149 \n
 *   \c p1 \c -0.000726514 \n
 *   \c p2 \c 0.000317338 \n
 * 
 * 
 * Module Configuration File Parameters
 *
 * The following key-value pairs can be specified as parameters in the configuration file e.g. \c rectification.ini 
 * (they can also be specified as command-line parameters if you so wish). 
 * The value part can be changed to suit your needs; the default values are shown below. 
 *   
 * - \c leftImageInPort \c /leftImage:i \n  
 *   specifies the input port name 
 *
 * - \c rightImageInPort \c /rightImage:i   \n  
 *   specifies the input port name  
 *
 * - \c headPort \c /head:i   \n  
 *   specifies the input port name for the head encoder values  
 * 
 * - \c leftImageOutPort \c /leftImage:o   \n 
 *   specifies the output port name for the left rectified image  
 *
 * - \c rightImageOutPort \c /rightImage:o   \n  
 *   specifies the output port name for the right rectified image  
 *
 * All these port names will be prefixed by \c /rectification or whatever else is specifed by the name parameter.
 *
 * \section portsa_sec Ports Accessed
 * 
 * - \c /icub/head/state:o \n
 *   This port is used to get the version and vergence angles (in degrees) from which 
 *   the azimuth (horizontal) angle of the left and right cameras can be derived.
 *   It must be connect to the head input port (\c /rectification/head ) by the application.
 *                      
 * \section portsc_sec Ports Created
 *
 *  Input ports
 *
 * - \c /rectification \n
 *   This port is used to change the parameters of the module at run time or stop the module
 *   The following commands are available
 * 
 *   help \n
 *   quit
 *
 *   Note that the name of this port mirrors whatever is provided by the \c --name parameter value
 *   The port is attached to the terminal so that you can type in commands and receive replies.
 *   The port can be used by other modules but also interactively by a user through the yarp rpc directive, viz.: \c yarp \c rpc \c /rectification
 *   This opens a connection from a terminal to the port and allows the user to then type in commands and receive replies.
 *       
 * - \c /rectification/leftImage:i 
 *
 * - \c /rectification/rightImage:i
 *
 * - \c /rectification/head:i \n
 *   This port needs to be connected to \c /icub/head/state:o to get the version and vergence values.
 *
 * Output ports
 *
 * - \c /rectification \n
 *   see above
 *
 * - \c /rectification/leftImage:o 
 *
 * - \c /rectification/rightImage:o
 *
 *
 * Port types 
 *
 * The functional specification only names the ports to be used to communicate with the module 
 * but doesn't say anything about the data transmitted on the ports. This is defined by the following code. 
 *
 * \c BufferedPort<ImageOf<PixelRgb> >   \c leftImageInPort;     \n
 * \c BufferedPort<ImageOf<PixelRgb> >   \c rightImageInPort;    \n
 * \c BufferedPort<ImageOf<PixelRgb> >   \c leftImageOutPort;    \n
 * \c BufferedPort<ImageOf<PixelRgb> >   \c rightImageOutPort;   \n
 * \c BufferedPort<Vector>               \c headPort;            \n
 *
 * \section in_files_sec Input Data Files
 *
 * None
 *
 * \section out_data_sec Output Data Files
 *
 * None
 *
 * \section conf_file_sec Configuration Files
 *
 * \c rectification.ini  in \c $ICUB_ROOT/app/rectification/conf \n
 * \c icubEyes.ini       in \c $ICUB_ROOT/app/rectification/conf
 * 
 * \section tested_os_sec Tested OS
 *
 * Windows
 *
 * \section example_sec Example Instantiation of the Module
 * 
 * <tt>rectification --name rectification --context rectification/conf --from rectification.ini --robot icub  --configCamera icubEyes.ini</tt>
 *
 * \author 
 *
 * David Vernon
 * 
 * Copyright (C) 2009 RobotCub Consortium
 * 
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 * 
 * This file can be edited at \c $ICUB_ROOT/src/rectification/include/iCub/rectification.h
 * 
 */


/* 
 * Copyright (C) 2009 RobotCub Consortium, European Commission FP6 Project IST-004370
 * Authors: David Vernon
 * email:   david@vernon.eu
 * website: www.robotcub.org 
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */


/*
 * Audit Trail
 * -----------
 * 01/09/09  Began development      DV
 * 12/09/09  Finished development   DV
 */ 


#ifndef __ICUB_RECTIFICATION_MODULE_H__
#define __ICUB_RECTIFICATION_MODULE_H__


/* System includes */

#include <iostream>
#include <string>


/* YARP includes */

#include <yarp/sig/all.h>
#include <yarp/os/all.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/dev/PolyDriver.h>

using namespace std;
using namespace yarp::os; 
using namespace yarp::sig;
using namespace yarp::dev;


/* fourierVision includes */

#include "iCub/fourierVision.h"


class RectificationThread : public Thread
{
private:

   /* class variables */

   int    x, y;
   int    width, height, depth;    // dimensions of the image
   float  vergence;                // relative camera azimuth angle: vg = L-R
   float  leftCameraAngle;         // left camera direction relative to gaze (i.e. relative to version angle)    
   float  rightCameraAngle;        // right camera direction relative to gaze (i.e. relative to version angle) 
   PixelRgb rgbPixel;
   ImageOf<PixelRgb> *leftImage;
   ImageOf<PixelRgb> *rightImage;
   Vector            *encoderPositions;
   DVimage           *leftInput;
   DVimage           *rightInput;
   DVimage           *leftRectified;
   DVimage           *rightRectified;

   bool debug;
  	    
   /* thread parameters: they are pointers so that they refer to the original variables in rectification */

   BufferedPort<ImageOf<PixelRgb> > *leftImagePortIn;
   BufferedPort<ImageOf<PixelRgb> > *rightImagePortIn;
   BufferedPort<ImageOf<PixelRgb> > *leftImagePortOut;
   BufferedPort<ImageOf<PixelRgb> > *rightImagePortOut;   
   BufferedPort<Vector>            *robotPort;

   float *fxLeft,  *fyLeft;          // focal length
   float *fxRight, *fyRight;         // focal length
   float *cxLeft,  *cyLeft;          // coordinates of the principal point
   float *cxRight, *cyRight;         // coordinates of the principal point

public:

   /* class methods */

   RectificationThread(BufferedPort<ImageOf<PixelRgb> > *leftImageIn, 
                       BufferedPort<ImageOf<PixelRgb> > *rightImageIn,
                       BufferedPort<ImageOf<PixelRgb> > *leftImageOut, 
                       BufferedPort<ImageOf<PixelRgb> > *rightImageOut,
                       BufferedPort<Vector>            *robotPort,
                       float *fxLeftValue,  float *fyLeftValue, 
                       float *cxLeftValue,  float *cyLeftValue, 
                       float *fxRightValue, float *fyRightValue, 
                       float *cxRightValue, float *cyRightValue);
   bool threadInit();     
   void threadRelease();
   void run(); 
};


class Rectification:public RFModule
{
   /* module parameters */

   string moduleName;
   string robotName; 
   string robotPortName;  
   string leftInputPortName;
   string rightInputPortName;
   string leftOutputPortName;  
   string rightOutputPortName;  
   string handlerPortName;
   string cameraConfigFilename;
   float  fxLeft,  fyLeft;          // focal length
   float  fxRight, fyRight;         // focal length
   float  cxLeft,  cyLeft;          // coordinates of the principal point
   float  cxRight, cyRight;         // coordinates of the principal point


   /* class variables */

   bool debug;

   BufferedPort<ImageOf<PixelRgb> > leftImageIn;      
   BufferedPort<ImageOf<PixelRgb> > rightImageIn;      
   BufferedPort<ImageOf<PixelRgb> > leftImageOut;     
   BufferedPort<ImageOf<PixelRgb> > rightImageOut;   
   BufferedPort<Vector>             robotPort;
   Port                             handlerPort;      //a port to handle messages 

   /* pointer to a new thread to be created and started in configure() and stopped in close() */

   RectificationThread *rectificationThread;


public:
   Rectification();
   bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
   bool interruptModule();                       // interrupt, e.g., the ports 
   bool close();                                 // close and shut down the module
   bool respond(const Bottle& command, Bottle& reply);
   double getPeriod(); 
   bool updateModule();
};


#endif // __ICUB_RECTIFICATION_MODULE_H__
//empty line to make gcc happy

