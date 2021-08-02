//==================================================================aaa
/*! \file eqPacketProtocl.cpp                                      
 *                                                                 
 * \copydoc Copyright                                              
 * \author Xue Chong(xc)                                           
 * \date July 29, 2019                                             
 *                                                                 
 * protocl for encode and decode packets between cloud and endpoint
 *///---------------------------------------------------------------aaa


#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include "ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp"
using namespace std;

/*
 * Function to calculate the CRC16 of data
 * CRC16-CCITT
 * Initial value = 0xFFFF
 * Polynomial = x^16 + x^12 + x^5 + x^0
 */

uint8_t calculate_bcc(const void *data, size_t length)
{
	const uint8_t *bytes = (uint8_t *) data;
	uint8_t bcc = 0x00;
    for (size_t i = 0; i < length; i++) {bcc ^= bytes[i]; } 
	return bcc;
}

/*
 * Function to dynamically allocate an an_packet
 */
an_packet_t *an_packet_allocate(size_t length)
{
	an_packet_t *an_packet = (an_packet_t *) malloc(sizeof(an_packet_t) + length * sizeof(uint8_t) + 1);
	if (an_packet != NULL)
	{
		an_packet->data_length = length;
	}
	return an_packet;
}

/*
 * Function to free an an_packet
 */
void an_packet_free(an_packet_t **an_packet)
{
	free(*an_packet);
	*an_packet = NULL;
}

/*
 * Initialise the decoder
 */
void an_decoder_initialise(an_decoder_t *an_decoder)
{
	an_decoder->buffer_length = 0;
	an_decoder->bcc_errors = 0;
}

/*
 * Function to decode an_packets from raw data
 * Returns a pointer to the packet decoded or NULL if no packet was decoded
 */
an_packet_t *an_packet_decode(an_decoder_t *an_decoder)
{
    size_t decode_iterator = 0;
    an_packet_t *an_packet = NULL;
    big_uint32_t data_length;
    uint8_t bcc;

    while (decode_iterator + AN_PACKET_HEADER_SIZE + 1 <= an_decoder->buffer_length){
        if ((an_decoder->buffer[decode_iterator++] == 0x23) && // pointing at 0, inc to 1
		    (an_decoder->buffer[decode_iterator] == 0x23) )  // pointing at 1, no inc
        {
            memcpy(&data_length , &an_decoder->buffer[decode_iterator + 23], 4 * sizeof(uint8_t));
            decode_iterator++; // pointing at 1, inc to 2, cmd_id
            /// if length is too short, this is not a whole packet, if tail out of this buff
            /// now iterator pointing at 2, cmd_id
            /// only complete packet if bcc index < buffer_length,
            if (decode_iterator + AN_PACKET_HEADER_SIZE - 2/*skip header, pointing at 28, start of data*/
               + data_length/*skip data pointing at bcc*/ >= an_decoder->buffer_length)
            {
                decode_iterator -= 2; // back to starting
                break; // now an_packet is NULL
            } /// if bcc error , this packet is wrong
            /// now iterator point at 2 ,cmd_id
            //// test end.
            bcc = an_decoder->buffer[decode_iterator+ AN_PACKET_HEADER_SIZE - 2+ data_length];
            /// if bcc correct, extract this packet, now pointing at 2, cmd_id
            if (bcc == calculate_bcc(&an_decoder->buffer[decode_iterator], data_length + AN_PACKET_HEADER_SIZE - 2)){
                an_packet = an_packet_allocate(data_length);
                if (an_packet != NULL)
                {
                    /// header
                    an_packet->cmd_id = an_decoder->buffer[decode_iterator++]; // at 2,inc to 3
                    an_packet->request_id = an_decoder->buffer[decode_iterator++]; // at 3, inc to 4
                    memcpy(an_packet->vehicle_id, &an_decoder->buffer[decode_iterator],17 * sizeof(uint8_t));
                    decode_iterator += 17; // inc to 21
                    an_packet->count = an_decoder->buffer[decode_iterator++];  // 21, inc to 22
                    an_packet->count = an_packet->count * 256 + an_decoder->buffer[decode_iterator++]; // 22, inc to 23
                    an_packet->encryption = an_decoder->buffer[decode_iterator++]; //23 , inc to 24
                    //decode_iterator += 2; // inc from 24 to 26
                    decode_iterator += 4; // inc from 24 to 26
                    memcpy(an_packet->data, &an_decoder->buffer[decode_iterator], data_length * sizeof(uint8_t));
                }
                // whatever if bcc correct, pointer must be moved forward,
                decode_iterator += data_length; // skip data tail, pointing at bcc
                decode_iterator ++; // skip bcc error, pointing at next packet start
                break;
            }else{ // bcc errors
                //decode_iterator -= 2;
                cout << "bcc error \n" ;
                an_decoder->bcc_errors++;
                /// ??????????????????????
                // whatever if bcc correct, pointer must be moved forward,
                decode_iterator += data_length; // skip data tail, pointing at bcc
                decode_iterator ++; // skip bcc error, pointing at next packet start
            }
        }else{// decode_iterator has incremented, loop while
        }// if not header 0x23 0x23
    }// while loop end
    /// move the left data forward to index 0, cover old data
    if (decode_iterator < an_decoder->buffer_length) {
        if (decode_iterator > 0){
            memmove(&an_decoder->buffer[0], &an_decoder->buffer[decode_iterator], (an_decoder->buffer_length - decode_iterator) * sizeof(uint8_t));
            an_decoder->buffer_length -= decode_iterator;
        }
    }else {an_decoder->buffer_length = 0;}
    return an_packet;
}

/*
 * Function to encode an an_packet
 */
void an_packet_encode(an_packet_t *an_packet)
{
	uint8_t *bytes = (uint8_t *) an_packet;
	uint8_t bcc;
	bcc = calculate_bcc(bytes + 2, AN_PACKET_HEADER_SIZE - 2 +  an_packet->data_length);
    an_packet->data[an_packet->data_length] = bcc;
}

void bytes_encode(uint8_t *bytes, size_t length){
	uint8_t bcc;
	bcc = calculate_bcc(bytes + 2, length - 2 - 1);
    bytes[length-1] = bcc;
}
void bytes_encode(vector<uint8_t> &bytes){
	uint8_t bcc;
    if(bytes.size() < AN_PACKET_HEADER_SIZE){return ;}
	bcc = calculate_bcc(bytes.data() + 2, bytes.size() - 2 - 1);
    bytes[bytes.size()-1] = bcc;
}
