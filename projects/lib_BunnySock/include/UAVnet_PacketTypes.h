//=============================================================================
/*    Copyright (C) 2012  Dave Billin

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//-----------------------------------------------------------------------------
/** @file UAVnet_PacketTypes.h
 *
 * @brief
 *	Type definitions and constants used in conjunction with UAVnet format
 *	network packets.
 *
 * @note
 *	Ported from UAV_PACKET_TYPES.lib Dynamic C library checked out on 4-18-2011
 */
//=============================================================================

//#ifndef UAV_PACKET_TYPES_H
//#define UAV_PACKET_TYPES_H

#include <stdint.h>		// Standard sized integer type definitions



//--------------------------------------------------------------
/* IMPORTANT: UAVnet was originally designed for 8-bit Rabbit
 * microcontrollers.  Due to their 8-bit data bus, these Rabbit
 * micros used single-Byte-aligned memory addressing.  By
 * contrast, PC platforms (i386, amd64) generally do their
 * addressing aligned to 4-Byte (32-bit) boundaries.  As a
 * result, we must enable structure packing to be able to
 * access fields in UAVnet packets.  This results in larger,
 * slower code for the PC in order to accommodate unaligned
 * structure accesses, but it is necessary to accommodate the
 * legacy Rabbit code
 */
#pragma pack(push, 1)	// This should work for gcc and Visual Studio 2010+


//===========================
// CONSTANTS
//===========================

/** @enum e_PacketSizes
 * @brief Size constants common to all UAVnet packets
 */
enum e_PacketSizes
{
	UAVNET_PACKET_SIZE = 128
};


/** @def UAVNET_HEADER_SIZE
 * @brief
 *	Size of a UAVnet packet header
 */
#define UAVNET_HEADER_SIZE 	sizeof(UAVnet::PacketHeader_t)




/** @enum e_UavNetPacketTypeIds
 * @brief
 *	Integer ID's used in the PacketType field of a UAVnet packet header to
 *	identify the type of information conveyed in the packet's payload.
 */
enum e_UavNetPacketTypeIds
{
	TYPE_GENERIC = 0, 	// 0 Generic packet type
	TYPE_GENERIC_LOG,	// 1
	TYPE_SENSOR,		// 2 Sensor readings sent by SPOCK
	TYPE_SENSOR_LOG,	// 3
	TYPE_CONTROLS,		// 4 SCOTTY servo positions and motor speed
	TYPE_CONTROLS_LOG,	// 5
	TYPE_COMMAND,		// 6 Command from one controller to another
	TYPE_COMMAND_LOG,	// 7
	TYPE_LBL_POS,		// 8 Position information from Woods Hole Modem or GPS
	TYPE_LBL_POS_LOG,	// 9
	TYPE_WHM_MSG,		// 10 Message received from the WHOI acoustic modem
	TYPE_WHM_MSG_LOG,	// 11
	TYPE_GPGGA,			// 12 GPS info
	TYPE_GPGGA_LOG,		// 13
	TYPE_BUOY_POS,    	// 14 LBL buoy positions sent from Uhura to Kirk.
	TYPE_BUOY_POS_LOG,	// 15
	TYPE_KALMAN,		// 16 Kalman filter info
	TYPE_KALMAN_LOG,	// 17
	TYPE_IMU,			// 18 "IMU" message contents from the Archangel IMU
	TYPE_IMU_LOG,		// 19
	TYPE_TELEMETRY,		// 20 Telemetry packet sent from Kirk to DSP
	TYPE_TELEMETRY_LOG,	// 21
	TYPE_AHRS,			// 22 "AHRS" messages from the Archangel IMU
	TYPE_AHRS_LOG,		// 23
	TYPE_SYNCH_RANGE,	// 24 Synchronous navigation range
	TYPE_SYNCH_RANGE_LOG,// 25
	TYPE_KALMSHIP,		// 26 Used for logging of the ship EKF
	TYPE_KALMSHIP_LOG,	// 27
	TYPE_DEPTH, 		// 28 Used for high rate depth sensor logging
	TYPE_DEPTH_LOG,		// 29
	TYPE_EKFTEST_IMU_MSG,	/* Generated by the EKFTest app to split apart
							   TYPE_IMU_LOG packets */
	NUM_PACKET_TYPEIDS
};


// These may be used to identify a command_packet.
// All Command numbers start with "CMD_"
typedef enum
{
	CMD_SET_TRIM,			/* 0(0x00) Request to set SCOTTIE servo trims or reply to a
									   CMD_REQ_SERVO_TRIM command sent to SCOTTIE
								Parameter[0]: Rudder servo center
								Parameter[1]: Right elevator servo center
								Parameter[2]: Left elevator servo center
								Parameter[3]: Servo coupling coefficient (multiplied by 100)
							*/

	CMD_ABORT,				/* 1(0x01) Abort/stop current mission
								Parameter[0]: Abort code (reason for abort)
							*/

	CMD_START_MISSION,		/* 2(0x02) Start running a new mission
								Parameter[0]: Mission number to run
								Parameter[1]: Run number;
								Parameter[2]: Vehicle position ID in formation
								Parameter[3]: Vehicle ID;
								Parameter[4]: Leader flag (1 = vehicle is the leader; 0 = not leader)
								Parameter[5]: LBL nav ping cycle ID
								Parameter[6]: Day
								Parameter[7]: Month
								Parameter[8]: Year
							*/

	CMD_ZERO,				/* 3(0x03) Zero selected sensors
								Parameter[0]: ID of sensor to zero (or 0 to reset all)
							*/

	CMD_REQ_LOG_TABLE,		/* 4(0x04) Request log file from KIRK
								Parameter[0]: log file being requested (0..255)
							*/

	CMD_SEND_LOG_TABLE,		/* 5(0x05) Send one entry of log table
								Parameter[0]: Number of packets in the log
								Parameter[1]: Mission number
								Parameter[2]: Hour
								Parameter[3]: Minute
								Parameter[4]: Second
							*/

	CMD_REQ_LOG_RUN,		/* 6(0x06) Request log entries for single run
								Parameter[0]: Run number being requested
								Parameter[1]: Packet number in the log entries
							*/

	CMD_RESET_LOG,			/* 7(0x07) Resets log to beginning
								(command takes no parameters)
							*/

	CMD_END_OF_FILE,		/* 8(0x08) Indicates end of log table or log run
								(command takes no parameters)
							*/

	CMD_REQ_MSG,			/* 9(0x09) Request/reply containing a WHOI message
								Parameter[0]: Message type (BIT13, BYTE32)
							*/

	CMD_REPORT_RPM,			/* 10(0x0A) Measured prop RPM reported by SCOTTIE
								Parameter[0]: measured prop RPM
							*/

	CMD_REQ_TRIM,			/* 11(0x0B) Request for SCOTTIE servo trims
								(command takes no parameters)
							*/

	CMD_SET_LEADER_BROADCAST,/* 12(0x0C) KIRK tells UHURA whether or not to broadcast
								Parameter[0]: 1 = broadcast; 0 = don't broadcast
							*/

	CMD_BUOYS_RECEIVED,		/* 13(0x0D) Kirk tells Uhura that he receive buoy locations packet.
								Parameter[0]: 1 = received; 0 = not received
							*/

	CMD_START_RECORDER,		/* 14(0x0E) Kirk requests DSP to start Recording.
								Parameter[0]: Vehicle ID
								Parameter[1]: Mission number
								Parameter[2]: Run number
								Parameter[3]: Restart (nudge) number
								Parameter[4]: DSP record engine state (sent in response)
							*/

	CMD_STOP_RECORDER,		/* 15(0x0F) Kirk requests DSP to stop Recording.
								(command takes no parameters)
							*/

	CMD_RESTART_RECORDER,	/* 16(0x10) Kirk requests DSP to re-start recording
								Parameter[0]: Vehicle ID
								Parameter[1]: Mission number
								Parameter[2]: Run number
								Parameter[3]: Restart (nudge) number
								Parameter[4]: DSP record engine state (sent in response)
							*/

	CMD_QUERY_RECORDER,		/* 17(0x11) Request/reply containing DSP record engine state
								Parameter[0]: not used
								Parameter[1]: not used
								Parameter[2]: not used
								Parameter[3]: not used
								Parameter[4]: DSP record engine state (sent in reply;
											 ignored in request)
							*/

	CMD_GPS_DATE,			/* 18(0x12) Report of Date of Position fix
								Parameter[0]: Day
								Parameter[1]: Month
								Parameter[2]: Year
							*/

	CMD_START_32BYTE, 		/* 19(0x13) 32Byte Message Start mission message.
								Parameter[0]: Mission number
							*/

	CMD_ENABLE_SENSOR_UPDATES,	/* 20 (0x14) Enable/disable SPOCK sending sensor packets every
											 1/4 second.
								Parameter[0]:
									1 - enable normal updates (default setting)
									0 - disable updates (and send sensor packets
										only in response to a CMD_REQUEST_SENSORS
										command packet).

								Parameter[1]: 'magic' key value 0x01ff
								Parameter[2]: 'magic' key value 0xfe02
								Parameter[3]: 'magic' key value 0x03fd
								Parameter[4]: 'magic' key value 0xfc04
							*/

	CMD_REQUEST_SENSORS		/* 21 (0x15) Request sensor value(s) from SPOCK
								Parameter[0]: Integer ID specifying sensor value(s) being
											 requested or (-1) to request all sensor values
							*/
} command_type;






/** @enum Sensor ID's that may be specified in parameter_1 of a
 *	CMD_REQUEST_SENSOR command packet */
enum e_SensorIds
{
	ALL_SENSORS = -1,	/**< ALL sensor values */
	SENSORS = 0,		/**< Normal SPOCK sensors */
	HI_RATE_DEPTH,		/**< Depth readings taken at 20 Hz  */
	NUM_SENSOR_IDS		/**< The number of valid sensor ID's (used for
							 bounds-checking - not a valid sensor ID) */
};





/** @enum e_AbortTypeIds
 * @brief
 *	ID's indicating the reason for a mission abort
 */
enum e_AbortCodes
{
	ABORT_NORM,			// 0(0x00) normal abort/mission complete
	ABORT_MANUAL,		// 1(0x01) abort from keyboard input (debug only)
	ABORT_UHURA,		// 2(0x02) abort received from UHURA
	ABORT_VOLTAGE,		// 3(0x03) battery voltage too low
	ABORT_H2O,			// 4(0x04) water leak detected
	ABORT_RUN_TIME,		// 5(0x05) run length too long
	ABORT_DEPTH,		// 6(0x06) depth too great
	ABORT_TILT,			// 7(0x07) pitch too great
	ABORT_DIST,			// 8(0x08) distance from waypoint too great (GPS only)
	ABORT_BAD_POS,		// 9(0x09) too many bad position packets
	ABORT_NO_DIVE,		// 10(0x0A) failed to dive
	ABORT_LEADER,		// 11(0x0B) commanded to abort by leader
	ABORT_KALMAN,		// 12(0x0C) kalman filter jump
	ABORT_BADRANGES,	// 13(0x0D) kalman filter abort if 10 bad ranges
	ABORT_INVERT, 		// 14(0x0E) kalman filter abort if failed invert
	ABORT_BAD_IMU,    	// 15(0x0F) Imu is not responding
	ABORT_OFF_COURSE,	// 16(0x10) Vehicle is too far off of course
	ABORT_NO_SHIPEKF 	// 17(0x11) Ship EKF didn't start
};




/** @enum e_UavnetWhoiMessageTypeIds
 * @brief
 * 	ID's used in the msg_size_type field of TYPE_WHM_MSG and TYPE_WHM_MSG_LOG
 *	packets to indicate the type of message it contains
 */
enum e_WhoiMessageTypeIds
{
	BIT13,
	BYTE32,
	SYNCH_BUOY
};





//===========================
// DATA TYPES
//===========================


//------------------------------------------
/** @typedef PacketHeader_t
 *
 * @brief
 * 	Data structure forming the packet header that all UAVnet packets begin
 *	with.
 */
typedef struct
{
	uint16_t Type;		/**< Packet type from e_UavNetPacketTypeIds */
	uint16_t SourceId;	/**< ID of the device sending the packet */
	uint16_t DestId;	/**< ID of the device the packet is addressed to */
	uint16_t Reserved;	/**< <b> Reserved for future use */
	uint32_t MsTimeStamp;	/**< Millisecond time stamp associated with the
								 packet data */
} PacketHeader_t;





//------------------------------------------
/** @typedef GenericPacket_t
 * @brief
 * 	Data structure used to represent a generic, typeless UAVnet packet.
 *
 * @par Packet Type
 *	TYPE_GENERIC, TYPE_GENERIC_LOG
 */
typedef struct
{
	PacketHeader_t Header;		/**< Packet header */
	char Payload[UAVNET_PACKET_SIZE - sizeof(PacketHeader_t)]; /**< payload
																	data */
} GenericPacket_t;



//------------------------------------------
/** @typedef SensorPacket_t
 * @brief
 *	Sensor data sent by SPOCK
 *
 * @par Packet Type
 *	TYPE_SENSOR, TYPE_SENSOR_LOG
 */
typedef struct
{
	PacketHeader_t Header;		/**< Packet header */
	float CompassHeading;		/**< Digital compass heading (degrees) */
	float Depth_cm;				/**< Depth from pressure sensor (centimeters) */
	float BatteryVoltage;		/**< Battery bus voltage */
	uint16_t WaterLeakDetected;	/**< Water leak detected flag (1 = TRUE;
									 2 = FALSE */

	float Temperature_C;		/**< Internal vehicle temperature (degrees C) */

	float GPS_Longitude;		/**< GPS longitude */
	float GPS_Latitude;			/**< GPS latitude */
	float GPS_Velocity;			/**< GPS velocity (meters per second) */
	float GPS_HPE;				/**< GPS horizontal position error */
	float GPS_Heading;			/**< GPS heading (degrees) */

	//ACCELLEROMETER
	float Accelerometer_X;		/**< Acceleration about X-axis */
	float Accelerometer_Y;		/**< Acceleration about Y-axis */

	float Pitch;				/**< Vehicle pitch (degrees) */
	float Roll;					/**< Vehicle roll (degrees) */
	float Dip;					/**< Vehicle dip (degrees) */
	float BatteryCurrent;		/**< Battery current (currently unused) */
	float BatteryCharge;		/**< Battery charge (currently unused) */
	float YawRate;				/**< Yaw rate (degrees per second) */
	uint16_t GPS_Hours;				/**< GPS Time: hour in the day */
	uint16_t GPS_Minutes;			/**< GPS Time: minute in the hour */
	uint16_t GPS_Seconds;			/**< GPS Time: second in the minute */
	int32_t GPS_PpsMs;				/**< Milliseconds since midnight derived from
										 GPS PPS */

	char Pad[ UAVNET_PACKET_SIZE - (sizeof(PacketHeader_t) +
			  17*sizeof(float) + 4*sizeof(uint16_t) + sizeof(int32_t)) ]; // 92

} SensorPacket_t;




//------------------------------------------
/** @typedef ControlsPacket_t
 * @brief
 *	Sent from KIRK to SCOTTY to communicate actuator controls
 *
 * @par Packet Type
 *	TYPE_CONTROLS, TYPE_CONTROLS_LOG
 */
typedef struct
{
	PacketHeader_t Header;		/**< Packet header */
	int16_t DesiredPropRpm;		/**< Desired propeller RPM */
	int16_t ControlType;		/**< Control packet type (currently unused) */
	int16_t RudderAngle_ddeg;	/**< Rudder angle (decidegrees) */
	int16_t Elevator_ddeg;		/**< Elevator angle (decidegrees) */
	int16_t Aileron_ddeg;		/**< Aileron angle (decidegrees) */
	uint16_t WaypointNumber;	/**< Target waypoint number */
	uint16_t MissionMode;		/**< Current mission mode */
	float PathVector_North;		/**< North component of vehicle's path vector */
	float PathVector_East;		/**< East component of vehicle's path vector */
	float EstPosVector_North;	/**< North component of estimated position
									 vector */
	float EstPosVector_East;	/**< East component of estimated position
									 vector */
	float TargetVector_N;		/**< North component of target vector */
	float TargetVector_E;		/**< East component of target vector */
	float ReferenceHeading;		/**< Reference heading */
	float HeadingError;			/**< Difference between desired heading and
									 current heading */
	float DistanceFromPath;		/**< Distance from path vector (TRAJ control
									 only) */
	float TargetVectorDistance;	/**< Length of target vector */
	float DistanceSoFar;		/**< Distance traveled after reaching current
									 target waypoint */
	float DistanceRemaining;	/**< Distance remaining to current target
									 waypoint */
	float DepthErrorSum;		/**< integrated depth error control value */
	float PerpDistanceSum;		/**< integrated perpendicular distance Control
									 Value */

	char Pad[UAVNET_PACKET_SIZE -
	         (14*sizeof(float) + 5*sizeof(int16_t) + 2*sizeof(int16_t) +
	          sizeof(PacketHeader_t))];
} ControlsPacket_t;




//------------------------------------------
/** @typedef CommandPacket_t
 * @brief
 *	Sent from KIRK to SCOTTY to communicate actuator controls
 *
 * @par Packet Type
 *	TYPE_COMMAND, TYPE_COMMAND_LOG
 */
typedef struct
{
	PacketHeader_t Header;		/**< Packet header */
	int16_t CommandId;			/**< Command ID from e_CommandTypeIds */
	int16_t Parameter[10];		/**< Command parameters */

	char Pad[UAVNET_PACKET_SIZE - (sizeof(PacketHeader_t) +
								   11*sizeof(int16_t) )];
} CommandPacket_t;





//------------------------------------------
/** @typedef LblEstPositionPacket_t
 * @brief
 *	Packet sent from UHURA to KIRK containing the estimated vehicle position
 *	based on ranges from LBL transponder beacons.
 *
 * @par Packet Type
 *	TYPE_LBL_POS, TYPE_LBL_POS_LOG
 */
typedef struct{
	PacketHeader_t Header;		/**< Packet header */
	float Position_N;			/**< North component of the buoy position */
	float Position_E;			/**< East component of the buoy position */
	long TimeStamp;				/**< Time stamp of position information */
	char BuoyBitmap;			/**< Bitmap of buoys used to calculate the
									 position */
	float Range_A;				/**< Range (meters) to Buoy A */
	float Range_B;				/**< Range (meters) to Buoy B */
	float Range_C;				/**< Range (meters) to Buoy C */
	float Range_D;				/**< Range (meters) to Buoy D */

	char Pad[UAVNET_PACKET_SIZE - (6*sizeof(float) +
			 sizeof(long) + sizeof(char) + sizeof(PacketHeader_t))];
} LblEstPositionPacket_t;




//------------------------------------------
/** @typedef WhoiMsgPacket_t
 * @brief
 *	Packet containing a message from the WHOI acoustic modem: sent from UHURA
 * to KIRK
 *
 * @par Packet Type
 *	TYPE_WHM_MSG, TYPE_WHM_MSG_LOG
 */
typedef struct
{
	PacketHeader_t Header;		/**< Packet header */
	uint16_t MessageTypeId;		/**< TypeID from e_WhoiMessageTypeIds
									 indicating the type of message contained
									 in the packet */
	uint16_t DestVehicleID;		/**< Acoustic ID the message is addressed to */
	uint16_t SourceVehicleID;	/**< Acoustic ID the message was sent from */

	char MessageData[UAVNET_PACKET_SIZE - (3*sizeof(int16_t) +
					 sizeof(PacketHeader_t))]; //16
} WhoiMsgPacket_t;




//------------------------------------------
/** @typedef GpsGPGGAPacket_t
 * @brief
 *	Contains a GPGGA NMEA sentence from the Garmin GPS18x
 *
 * @par Packet Type
 *	TYPE_GPGGA, TYPE_GPGGA_LOG
 */
typedef struct
{
	PacketHeader_t Header;		/**< Packet header */
	char gpgga_msg[UAVNET_PACKET_SIZE - sizeof(PacketHeader_t)];
} GpsGPGGAPacket_t;




//------------------------------------------
/** @typedef LblBeaconLocationPacket_t
 * @brief
 *	Packet sent from Uhura to Kirk containing the positions of LBL transponder
 *	beacons A through D.
 *
 * @par Packet Type
 *	TYPE_BUOY_POS, TYPE_BUOY_POS_LOG
 */
typedef struct
{
	PacketHeader_t Header;		/**< Packet header */
	uint16_t SoundSpeedInH2O;	/**< Velocity of sound in water (meters/sec)*/
	int32_t OriginLatitude;	/**< Latitude of coordinate system origin */
	int32_t OriginLongitude;	/**< Longitude of coordinate system origin */

	float BeaconA_N;			/**< North coordinate of LBL beacon A */
	float BeaconA_E;			/**< East coordinate of LBL beacon A */
	float BeaconA_Depth;		/**< Depth of LBL beacon A */

	float BeaconB_N;			/**< North coordinate of LBL beacon B */
	float BeaconB_E;			/**< East coordinate of LBL beacon B */
	float BeaconB_Depth;		/**< Depth of LBL beacon B */

	float BeaconC_N;			/**< North coordinate of LBL beacon C */
	float BeaconC_E;			/**< East coordinate of LBL beacon C */
	float BeaconC_Depth;		/**< Depth of LBL beacon C */

	float BeaconD_N;			/**< North coordinate of LBL beacon D */
	float BeaconD_E;			/**< East coordinate of LBL beacon D */
	float BeaconD_Depth;		/**< Depth of LBL beacon D */

	char Pad[UAVNET_PACKET_SIZE - (sizeof(uint16_t) + 2*sizeof(int32_t) +
								   12*sizeof(float) + sizeof(PacketHeader_t))];	// 70
} LblBeaconLocationPacket_t;




//------------------------------------------
/** @typedef VehicleEKFPacket_t
 * @brief
 *	Packet containing vehicle EKF data (mostly used for logging)
 *
 * @par Packet Type
 *	TYPE_KALMAN, TYPE_KALMAN_LOG
 */
typedef struct
{
	PacketHeader_t Header;	/**< Packet header */
	float State_1;
	float State_2;
	float State_3;
	float State_4;
	float State_5;
	float P_1_1;
	float P_2_2;
	float P_3_3;
	float P_4_4;
	float P_5_5;
	float Delta_Time;
	float Z_gyro;
	float P_1_2;
	float P_1_3;
	float P_1_4;
	float P_1_5;
	float P_2_3;
	float P_2_4;
	float P_2_5;
	float P_3_4;
	float P_3_5;
	float P_4_5;

	char Pad[UAVNET_PACKET_SIZE - (22*sizeof(float) + sizeof(PacketHeader_t))];	// 100
} VehicleEKFPacket_t;




//------------------------------------------
/** @typedef ShipEKFPacket_t
 * @brief
 *	Packet containing data from the (target) ship EKF (mostly used for logging)
 *
 * @par Packet Type
 *	TYPE_KALMSHIP, TYPE_KALMSHIP_LOG
 */
typedef struct
{
	PacketHeader_t Header;	/**< Packet header */
	float State_1;
	float State_2;
	float State_3;
	float State_4;
	float P_1_1;
	float P_2_2;
	float P_3_3;
	float P_4_4;
	float Delta_Time;
	float P_1_2;
	float P_1_3;
	float P_1_4;
	float P_2_3;
	float P_2_4;
	float P_3_4;
	char Pad[UAVNET_PACKET_SIZE - (15*sizeof(float) + sizeof(PacketHeader_t))]; // 72
} ShipEKFPacket_t;




//------------------------------------------
/** @typedef DepthField_t
 * @brief
 *	A data structure used in high-rate depth packets
 */
typedef struct
{
	uint32_t time;// 4
	float pressure;    // 8
} DepthField_t;




//------------------------------------------
/** @typedef DepthField_t
 * @brief
 *	A packet used to record high-rate depth sensor (pressure) measurements
 *
 * @par Packet Type
 *	TYPE_DEPTH, TYPE_DEPTH_LOG
 */
typedef struct
{
	PacketHeader_t Header;	/**< Packet header */
	DepthField_t depth_field[14];
	char Pad[UAVNET_PACKET_SIZE - (14*sizeof(DepthField_t) +
								  sizeof(PacketHeader_t))];
} DepthPacket_t;



/*typedef struct{
	unsigned long  ms_time;
	float accX;
	float accY;
	float accZ;
	float gyrA;
	float gyrB;
	float gyrC;
}t_imudat;	// size 24 bytes

typedef struct{			//Added: 3/13/09  Added to include a packet allowing KIRK to record IMU data.
	PACKET_HEADER      				// 12 generic packet header info
	t_imudat imudata[4]; 			// 108

   char pad[PACKET_SIZE - 108];
}imu_packet;   */






//------------------------------------------
/** @typedef ImuMessage_t
 * @brief
 *	Data structure holding the sensor readings in a single "IMU" message from
 *	the Archangel IM^3 IMU module.  Used in IMU packets.
 */
typedef struct
{
	uint32_t TimeStampMs;	/**< Millisecond time stamp associated with the
								 sensor readings */
	float Acc_X;			/**< X accelerometer reading */
	float Acc_Y;			/**< Y accelerometer reading */
	float Acc_Z;			/**< Z accelerometer reading */
	float GyroA;			/**< Gyro A reading */
	float GyroB;			/**< Gyro B reading */
	float GyroC;			/**< Gyro C reading */
} ImuMessage_t;


//------------------------------------------
/** @typedef ImuPacket_t
 * @brief
 *	A packet used to record "IMU" messages from the Archangel IM^3 IMU
 *
 * @par Packet Type
 *	TYPE_IMU, TYPE_IMU_LOG
 */
typedef struct
{			//Added: 3/13/09  Added to include a packet allowing KIRK to record IMU data.
	PacketHeader_t Header;	/**< Packet header */

	ImuMessage_t Reading[4];	/**< IMU message contents */

	char Pad[UAVNET_PACKET_SIZE - (4*sizeof(ImuMessage_t) +
								   sizeof(PacketHeader_t))];
} ImuPacket_t;



//------------------------------------------
/** @typedef SingleImuPacket_t
 * @brief
 *	A packet generated by the EKFTest application that contains a single "IMU"
 *	message from the Archangel IM^3 IMU
 *
 * @par Packet Type
 *	TYPE_EKFTEST_IMU_MSG
 */
typedef struct
{
	PacketHeader_t Header;	/**< Packet header */

	ImuMessage_t Reading;	/**< IMU message contents */

	char Pad[UAVNET_PACKET_SIZE - (sizeof(ImuMessage_t) +
								   sizeof(PacketHeader_t))];

} SingleImuPacket_t;





/*typedef struct{
//	long  ms_time;
	float roll;
	float pitch;
	float yaw;
	float rollrate;
	float pitchrate;
	float yawrate;
}t_ahrsdat;

typedef struct{			//Added: 3/13/09  Added to include a packet allowing KIRK to record IMU data.
	PACKET_HEADER      				// 12 generic packet header info
	t_ahrsdat ahrsdata[4]; 			// 108

   char pad[PACKET_SIZE - 108];
}imu_packet;   */


/*
typedef struct{			//Added: 3/13/09  Added to include a packet allowing KIRK to record IMU data.
	PACKET_HEADER      	// 12 generic packet header info
   float roll1;         // 16
   float pitch1;    	   // 20
   float yaw1;		      // 24
   float rollrate1;		// 28
	float pitchrate1;		// 32
	float yawrate1;		// 36
   float roll2;         // 40
   float pitch2;    	   // 44
   float yaw2;		      // 48
   float rollrate2;		// 52
	float pitchrate2;		// 56
	float yawrate2;		// 60
   float roll3;         // 64
   float pitch3;    	   // 68
   float yaw3;		      // 72
   float rollrate3;		// 76
	float pitchrate3;		// 80
	float yawrate3;		// 84
   float roll4;         // 88
   float pitch4;    	   // 92
   float yaw4;		      // 96
   float rollrate4;		// 100
	float pitchrate4;		// 104
	float yawrate4;		// 108

	char pad[PACKET_SIZE - 108];
}ahrs_packet;
*/




//------------------------------------------
/** @typedef DspTelemetryPacket_t
 * @brief
 *	Telemetry data sent to the DSP to be recorded with magnetic data.
 *
 * @par Packet Type
 *	TYPE_TELEMETRY, TYPE_TELEMETRY_LOG
 */
typedef struct
{
	PacketHeader_t Header;	/**< Packet header */
	float EKF_north_pos;	/**< Estimated north coordinate from EKF */
	float EKF_east_pos;		/**< Estimated east coordinate from EKF */
	float depth_cm;			/**< Measured depth (cm) */
	float Roll_deg;			/**< Measured vehicle roll (degrees) */
	float Pitch_deg;		/**< Measured vehicle pitch (degrees) */
	float CompassHeading_deg;	/**< Digital compass heading (degrees) */
	char Pad[UAVNET_PACKET_SIZE - (6*sizeof(float) + sizeof(PacketHeader_t))];
} DspTelemetryPacket_t;





//------------------------------------------
/** @typedef DspTelemetryPacket_t
 * @brief
 *	Packet used to send WHOI modem synchronous navigation ranges from UHURA to
 *	KIRK
 *
 * @par Packet Type
 *	TYPE_SYNCH_RANGE, TYPE_SYNCH_RANGE_LOG
 */
typedef struct
{
	PacketHeader_t Header;	/**< Packet header */
	uint32_t TimeStamp;		/**< Timestamp of position info */
	float ArrivalTime_sec;	/**< TOA value (seconds) */
	float Range_m;			/**< Distance to the target beacon in meters. */
	int16_t BeaconId;		/**< ID of the target beacon */
	int16_t CycleId;		/**< Cycle */
	int16_t ClockMode;		/**< Synch Mode needs to be 3 */
	int16_t RxQuality;		/**< Receive quality reported by WHOI modem */
	float Doppler;			/**< Doppler value reported by WHOI modem */

	char Pad[UAVNET_PACKET_SIZE - (sizeof(PacketHeader_t) + sizeof(int32_t) +
			 	 	 	 	 	   3*sizeof(float) + 4*sizeof(int16_t))]; // 36
} SynchronousRangePacket_t;




typedef union
{
	GenericPacket_t* AsGenericPacket;
	SensorPacket_t* AsSensorPacket;
	ControlsPacket_t* AsControlsPacket;
	CommandPacket_t* AsCommandPacket;
	LblEstPositionPacket_t* AsLblEstPositionPacket;
	WhoiMsgPacket_t* AsWhoiMsgPacket;
	GpsGPGGAPacket_t* AsGpsGPGGAPacket;
	LblBeaconLocationPacket_t* AsLblBeaconLocationPacket;
	VehicleEKFPacket_t* AsVehicleEKFPacket;
	ShipEKFPacket_t* AsShipEKFPacket;
	DepthPacket_t* AsDepthPacket;
	ImuPacket_t* AsImuPacket;
	DspTelemetryPacket_t* AsDspTelemetryPacket;
	SynchronousRangePacket_t* SynchronousRangePacket;
	void* AsRawBytes;
} PacketPointer_t;


#pragma pack(pop)	// Restore default/previous structure packing

//#endif
