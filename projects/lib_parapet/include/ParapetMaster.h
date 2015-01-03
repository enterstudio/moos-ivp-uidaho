//=============================================================================
/** @file ParapetMaster.h
 *
 * @brief
 *	Declaration of a base class for objects that implement a parapet Master
 *	device
 *
 * @author Dave Billin (david.billin@vandals.uidaho.edu)
 */
//=============================================================================

#ifndef PARAPET_MASTER_H_
#define PARAPET_MASTER_H_

#include <stdint.h>
//#include <time.h>
//#include <unistd.h>
//#include <tr1/unordered_map>

#include "parapet.h"
#include "ParapetEventBroadcaster.h"


/** Abstract base class that implements Master-side communication using parapet
 * format serial communications packets
 */
namespace parapet
{

class ParapetMaster : public ParapetEventBroadcaster
{
public:

    /** Constructor */
    ParapetMaster();

    /** Destructor */
    virtual ~ParapetMaster();


    //=============================
    // Virtual Methods
    //=============================

    //=========================================================================
    /** Transmits a parapet packet and registers it to receive a corresponding
     *  response
     *
     * @param [in] Packet
     *  Reference to the packet to transmit
     */
    virtual bool Transmit( parapet_packet_t& Packet ) = 0;

    //=========================================================================
    /** Called to receive and process data from parapet Slave devices
     *
     * @return
     *  true if one or more packets were received and processed
     */
    virtual bool Receive( void ) = 0;


    //=============================
    // END Virtual Methods
    //=============================

    enum eParapetMasterEvents
    {
        INVALID_NUMBYTESTOFOLLOW = -100,    /**< Received a packet header with
                                                 an invalid NumBytesToFollow
                                                 field */
        DEVICEID_ZERO_REPLY = -101,         /**< Received a packet from a Slave
                                                 device with a DeviceID value
                                                 of zero */
        SLAVE_M_BIT_SET = -102,             /**< Received a packet from a Slave
                                                 device with its M (master) bit
                                                 set */
    };

    //=========================================================================
    /** Composes a parapet EXECUTE request packet and applies the running
     *  sequence ID generated by the master object
     *
     * @details
     *  This function populates the header and FunctionID of a parapet EXECUTE
     *  packet.  EXECUTE parameters may be added to the packet by calling
     *  AddParam()
     *
     * @param [out] Packet        
     *  A reference to the packet to be operated on
     *
     * @param [in] DeviceID      
     *  ID of the device the request is addressed to
     *
     * @param [in] FunctionID    
     *  ID of the function to execute
     *
     * @param [in] Params
     *  Pointer to one or more parapet_param_t structures to be added to the
     *  function; or NULL if no parameters should be included
     *
     * @param [in] NumParams
     *  Number of params pointed to by Params
     */
    void MakeExecuteRequest( parapet_packet_t& Packet, uint8_t DeviceID,
                             uint32_t FunctionID,
                             parapet_param_t* const Params,
                             uint32_t NumParams );


    //=========================================================================
    /** Composes a parapet READ request packet
     *
     * @details
     *  This function populates the header structure and and one or more 
     *  parameter ID's in a parapet READ packet.  Additional READ parameter 
     *  ID's may be added to the packet by calling AddParam()
     *
     * @param [out] Packet
     *  A reference to the packet to be operated on
     *
     * @param [in] DeviceID
     *  ID of the device the request is addressed to
     *
     * @param [in] ParamIDs
     *  Pointer to an array of Parameter ID's to be specified in the read 
     *  operation
     *
     * @param NumParamIDs   
     *  The number of parameters pointed to by ParamIDs (must be greater than
     *  zero)
     */
     void MakeReadRequest( parapet_packet_t& Packet, uint8_t DeviceID,
                           uint16_t const* ParamIDs, uint32_t NumParamIDs );


    //=========================================================================
    /** Composes a parapet WRITE request packet
     *
     * @details
     *  This function populates the header structure and a single param of a 
     *  parapet WRITE packet.  Additional WRITE params may be added to the 
     *  packet by calling AddParam()
     *
     * @param [out] Packet     
     *  A reference to the packet to be operated on
     *
     * @param [in] DeviceID      
     *  ID of the device the request is addressed to
     *
     * @param [in] Params
     *  Pointer to an array of parapet_param_t structures to be included in
     *  the packet
     *
     * @param [in] NumParams
     *  The number of parapet_param_t structures pointed to by Params 
     */
     void MakeWriteRequest( parapet_packet_t& Packet, uint8_t DeviceID,
                            parapet_param_t* const Params,
                            uint32_t NumParams );


protected:

    //=========================================================================
    /** Parses received data into parapet packets
     *
     * @details
     *  Derived classes can call this function to process data received on the
     *  communications channel connected to a parapet slave device.  This
     *  function will parse the received data into packets and pass the packet
     *  data to registered ParapetListener objects.
     *
     * @param RxData
     *  Pointer to a buffer of received data to be processed
     *
     * @param NumBytes
     *  Number of data Bytes pointed to by RxData
     *
     * @return
     *  true if one or more complete packets were received and processed
     */
    bool ProcessRxData( char const* RxData, uint32_t NumBytesToProcess );


    //=========================================================================
    /** Returns the number of incoming Bytes that must be processed before a
     *  complete parapet packet has been received
     *
     * @return
     *  The number of Bytes required before a full packet has been received
     */
    inline uint32_t NumRxBytesNeeded( void ) const
    { return m_NumRxBytesNeeded; }

private:

    /** States that the Rx FSM may take on */
    typedef enum
    {
        RX_SYNC = 0,    /**< Receiving for SYNC Byte */
        RX_HEADER,      /**< Receiving the remainder of the packet header */
        RX_PAYLOAD,     /**< Receiving packet payload Bytes */
        NUM_RX_STATES   /**< NOT A VALID STATE - used for bounds-checking */
    } rx_state_t;

    rx_state_t m_RxState;        /**< Current state of the Rx FSM */
    uint32_t m_NumBytesReceived; /**< Count of Bytes received in the Rx packet
                                      buffer */
    uint32_t m_NumRxBytesNeeded; /**< Number of received Bytes needed for the 
                                      Rx FSM to transition to the next state */

    uint8_t m_RequestID;    /**< Counter used to generate a RequestID for
                                 outgoing packets */

    parapet_packet_t m_RxPacket;    /**< Buffer used to receive packets */


    /*
    class PendingRequest
    {
    public:
        uint16_t RequestID;
        struct timespec TimeSent;

        PendingRequest( parapet_packet_t& RequestPacket )
        {
            // Capture the request ID and timestamp to
            // record when the request was issued
            RequestID = RequestPacket.ContinueRequest.Header.fields.RequestID;
            clock_gettime( CLOCK_MONOTONIC, &TimeSent );
        }

        ~PendingRequest() {}

        //=====================================================================
        /// Calculates a difference in time separating two timespec's
        static struct timespec timespec_difference(
                                                struct timespec& reference,
                                                struct timespec& target )
        {
            struct timespec temp;
            long int diff_nsec = target.tv_nsec - reference.tv_nsec;
            if ( diff_nsec < 0 )
            {
                temp.tv_sec = target.tv_sec - reference.tv_sec - 1;
                temp.tv_nsec = target.tv_nsec - reference.tv_nsec
                               + 1000000000L;
            }
            else
            {
                temp.tv_sec = target.tv_sec - reference.tv_sec;
                temp.tv_nsec = target.tv_nsec - reference.tv_nsec;
            }
            return temp;
        }

        //=====================================================================
        /// Returns the age of the request in milliseconds
        uint32_t Age_msec( void ) const
        {
            struct timespec TimeNow;
            clock_gettime( CLOCK_MONOTONIC, &TimeNow );
            struct timespec TimeDiff = timespec_difference( TimeSent,
                                                             TimeNow );

            return TimeDiff.tv_sec + (TimeDiff.tv_nsec * 1000000UL);
        }


    };
    std::tr1::unordered_map<uint8_t, parapet_header_t> m_PendingRequestMap;
    */



};



//=============================================================================
inline void ParapetMaster::MakeExecuteRequest( parapet_packet_t& Packet,
                                               uint8_t DeviceID,
                                               uint32_t FunctionID,
                                               parapet_param_t* const Params,
                                               uint32_t NumParams )
{
    parapet::BuildExecuteRequest( Packet, DeviceID, m_RequestID++, FunctionID,
                                  Params, NumParams );
    m_RequestID &= 0x1f;    // Mod request ID
}



//=============================================================================
inline void ParapetMaster::MakeReadRequest( parapet_packet_t& Packet,
                                            uint8_t DeviceID,
                                            uint16_t const* ParamIDs,
                                            uint32_t NumParamIDs )
{
    parapet::BuildReadRequest( Packet, DeviceID, m_RequestID++, ParamIDs,
                               NumParamIDs );
    m_RequestID &= 0x1f;    // Mod request ID
}



//=============================================================================
inline void ParapetMaster::MakeWriteRequest( parapet_packet_t& Packet,
                                             uint8_t DeviceID,
                                             parapet_param_t* const Params,
                                             uint32_t NumParams )
{
    parapet::BuildWriteRequest( Packet, DeviceID, m_RequestID++, Params,
                                NumParams );
    m_RequestID &= 0x1f;    // Mod request ID
}



}   // END namespace parapet;

#endif /* PARAPET_MASTER_H_ */
