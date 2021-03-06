//=============================================================================
/** @file pVehicleEKF/src/main.cpp
 *
 * @brief
 *  Execution entry point for the pVehicleEKF application
 *
 * @author Dave Billin
 *
 */
//=============================================================================
#include "pVehicleEKF.h"
#include "config.h"
#include <MOOS/libMOOS/MOOSLib.h>
#include <iostream>

using std::cout;
using std::endl;

static std::string APPLICATION_VERSION(APP_VERSION_TUPLE);


/** @defgroup pVehicleEKF
 * @{
 */


//================================
// Function Prototypes
//================================
void PrintUsageInfo(void);
void PrintExampleConfig(void);


//=============================================================================
/** Creates an instance of the application and runs it */
int main(int argc, char* argv[])
{
    const char* szMissionFile = NULL;
    const char* szMOOSName = "pVehicleEKF";

    // Print usage info and exit if no command line arguments were supplied
    if (argc == 1)
    {
		cout << "\nUSAGE:   pVehicleEKF [OPTIONS] MISSION_FILE [APPNAME]\n"
             << "For more info, type 'pVehicleEKF --help' or 'man "
                "pVehicleEKF'\n"
             << endl;

        return 0;
    }

    // Parse command line arguments for options or mission file
    if (argc >= 2)
    {
        // Parse for option switches
        if ( MOOSStrCmp(argv[1], "-e") || MOOSStrCmp(argv[1], "--example") )
        {
            PrintExampleConfig();
            return 0;
        }
        else if ( MOOSStrCmp(argv[1], "-h") || MOOSStrCmp(argv[1], "--help") )
        {
            PrintUsageInfo();
            return 0;
        }
        else if ( MOOSStrCmp(argv[1], "-v") || MOOSStrCmp(argv[1], "--version") )
        {
            cout << "\npVehicleEKF v" << APPLICATION_VERSION << "\n" << endl;
            return 0;
        }
        else    // No command line options, must be a mission file path
        {
            szMissionFile = argv[1];
        }
    }

    // Grab alternate name to use when registering if specified
    if (argc >= 3)
    {
        szMOOSName = argv[2];
    }

    // Create and launch the application
    pVehicleEKF AppObject;
    AppObject.Run((char*)szMOOSName, (char*)szMissionFile);

    return 0;
}




//=============================================================================
/** Prints the application's command-line usage info */
void PrintUsageInfo( void )
{
    cout <<
    "\npVehicleEKF v" << APPLICATION_VERSION << "\n"
    "Written by Dave Billin (dave.billin@vandals.uidaho.edu)\n"
    "\n"
    "USAGE:   pVehicleEKF [OPTIONS] MISSION_FILE [APPNAME]\n"
    "\n"
    "OPTIONS\n"
    "   -e,--example  Prints an example mission file configuration block\n"
    "   -h,--help     Prints application usage information\n"
    "   -v,--version  Displays application version\n"
    "\n"
    "MISSION_FILE\n"
    "   MOOS mission file containing a configuration block\n"
    "\n"
    "APPNAME\n"
    "   Optional name to use when registering with the MOOS database\n"
    "\n"
    "For additional help, type \"man pVehicleEKF\"\n"
    "\n"
    << endl;
}



//=============================================================================
/** Prints an example MOOS mission file configuration block */
void PrintExampleConfig(void)
{
    cout <<
    "\n"
    "//---------------------------------------\n"
    "// GLOBAL-SCOPE MISSION FILE VARIABLES\n"
    "//---------------------------------------\n"
    "\n"
    "// Local coordinate system origin\n"
    "LatOrigin = 46.7302\n"
    "LongOrigin = -117.00893\n"
    "\n"
    "// Navigation beacon locations in local coordinate system"
    "BeaconA_North = 0\n"
    "BeaconA_East = 0\n"
    "BeaconA_Depth = 0\n"
    "BeaconB_North = 100"
    "BeaconB_East = 0\n"
    "BeaconB_Depth = 0\n"
    "BeaconC_North = 50\n"
    "BeaconC_East = -50\n"
    "BeaconC_Depth = 0\n"
    "BeaconD_North = -50\n"
    "BeaconD_East = -50\n"
    "BeaconD_Depth = 0\n"
    "\n"
    "SSH20 = 1.470  // Sound velocity in water (meters/sec)\n"
    "\n"
    "ProcessConfig = pVehicleEKF\n"
    "{\n"
    "   AppTick= 1\n"
    "   CommsTick = 20\n"
    "\n"
    "    //===========================\n"
    "    // OPTIONAL PARAMETERS\n"
    "    //===========================\n"
    "\n"
    "    // Depth (meters) above which pVehicleEKF will stop publishing \n"
    "    // estimated values from the VehicleEKF object and instead \n"
    "    // publish values based on GPS and sensor readings.\n"
    "    EKFDepthThreshold = 0.05\n"
    "\n"
    "\n"
    "    // GPS coordinates with an HPE value less than or equal to this \n"
    "    // threshold are elligible to be published directly when the \n"
    "    // vehicle is at the water's surface.  NOTE: HPE is specified \n"
    "    // in meters\n"
    "    GPS_HPE_Threshold = 0.10\n"
    "\n"
    "\n"
    "    //---------------------------------------------------\n"
    "    // The following mission file parameters may be \n"
    "    // used to re-map subscribed MOOSDB variables\n"
    "    //---------------------------------------------------\n"
    "    //GPS_LATITUDE_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //GPS_LONGITUDE_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //GPS_HEADING_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //GPS_VELOCITY_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //GPS_HPE_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //HEADING_COMPASS_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //DEPTH_SENSOR_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //LBL_2WAYTRAVELTIME_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //IMU_WZ_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //IMU_WY_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //PITCH_SENSOR_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //ROLL_SENSOR_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //LBL_RANGEPERIOD_SUBSCRIBETO = <MOOSDB variable name>\n"
    "    //RPM_VELOCITY_ESTIMATE_SUBSCRIBETO = <MOOSDB variable name>\n"
    "\n"
    "    //---------------------------------------------------\n"
    "    // The following mission file parameters may be \n"
    "    // used to re-map published MOOSDB variables\n"
    "    //---------------------------------------------------\n"
    "    //NAV_X_PUBLISHTO = <MOOSDB variable name>\n"
    "    //NAV_Y_PUBLISHTO = <MOOSDB variable name>\n"
    "    //NAV_DEPTH_PUBLISHTO = <MOOSDB variable name>\n"
    "    //NAV_HEADING_PUBLISHTO = <MOOSDB variable name>\n"
    "    //NAV_YAW_PUBLISHTO = <MOOSDB variable name>\n"
    "    //NAV_SPEED_PUBLISHTO = <MOOSDB variable name>\n"
    "    //EST_YAW_BIAS_PUBLISHTO = <MOOSDB variable name>\n"
    "\n"
	"}\n"
    "\n"
    << endl;
}

/** @} */   // END @defgroup pVehicleEKF
