//======================================================================                          
/*! \file eqPacketProtocl.hpp
 *                                                                 
 * \copydoc Copyright                                              
 * \author Xue Chong(xc)                                           
 * \date July 29, 2019                                             
 *                                                                 
 * protocl for encode and decode packets between cloud and endpoint 
 *///-------------------------------------------------------------------
#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <boost/array.hpp>
#include <boost/endian/arithmetic.hpp>
using namespace std;                                                                              
using namespace boost::endian;

#define AN_PACKET_HEADER_SIZE 28
#define an_packet_pointer(packet) packet->header
#define an_packet_size(packet) (packet->length + AN_PACKET_HEADER_SIZE)*sizeof(uint8_t)
#define an_packet_bcc(packet) ((packet->header[4]<<8) | packet->header[3])

#define an_decoder_pointer(an_decoder) &(an_decoder)->buffer[(an_decoder)->buffer_length]
#define an_decoder_size(an_decoder) (sizeof((an_decoder)->buffer) - (an_decoder)->buffer_length)
#define an_decoder_increment(an_decoder, bytes_received) (an_decoder)->buffer_length += bytes_received

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

typedef struct
{
    //uint8_t buffer[1024];
    uint8_t buffer[1024000];
    big_uint64_t buffer_length;
    uint32_t bcc_errors;
} an_decoder_t;

typedef struct
{
    uint16_t header;// 0
    uint8_t cmd_id;// 2
    uint8_t request_id; // 3
    uint8_t vehicle_id[17]; // 4
    big_uint16_t count;
    uint8_t encryption; //23
    big_uint32_t data_length;
    uint8_t data[0]; //26
} an_packet_t;


an_packet_t *an_packet_allocate(size_t length);
void an_packet_free(an_packet_t **an_packet);
void an_decoder_initialise(an_decoder_t *an_decoder);
an_packet_t *an_packet_decode(an_decoder_t *an_decoder);
void an_packet_encode(an_packet_t *an_packet);
void bytes_encode(uint8_t *bytes, size_t length);
void bytes_encode(vector<uint8_t> &bytes);
