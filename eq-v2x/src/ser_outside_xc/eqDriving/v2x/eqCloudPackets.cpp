//======================================================================
/*! \file eqCloudPackets.cpp
 *
 * \copydoc Copyright
 * \author Xue Chong(xc)
 * \date July 29, 2019
 *
 * packets definition between cloud and endpoint
 *///-------------------------------------------------------------------
 
#include <stdint.h>
#include <string.h>
#include "ser_outside_xc/eqDriving/v2x/eqPacketProtocol.hpp"
#include "ser_outside_xc/eqDriving/v2x/eqCloudPackets.hpp"

using namespace std;

void reset_message_report_data(message_report_data_t *message_data_packet){
    message_data_packet->vehicle_ego_data.data_length = 0;
    message_data_packet->appendix_data.data_length = 0;
    message_data_packet->error_data.data_length = 0;
    message_data_packet->pos_data.data_length = 0;
    message_data_packet->engine_data.data_length = 0;
}

int decode_params_inquiry_packet(params_inquiry_packet_t *params_inquiry_packet, an_packet_t *an_packet){
    if(!(an_packet->cmd_id == 0x80 && an_packet->request_id == 0xFE)){return -1;}
    if(an_packet->data[8] + 8 + 1 > an_packet->data_length){
        return 0;
    }
    params_inquiry_packet->count = an_packet->count;
    memcpy(&params_inquiry_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t));
    params_inquiry_packet->number = an_packet->data[8];
    params_inquiry_packet->ids.resize(params_inquiry_packet->number,0x00);
    memcpy(&params_inquiry_packet->ids[0], &an_packet->data[9], params_inquiry_packet->number*sizeof(uint8_t));
    return 1;
}

int decode_calibration_time(uint8_t *time, an_packet_t *an_packet){
    if(!(an_packet->cmd_id == 0x08 && an_packet->request_id == 0x01)){return -1;}
    if(an_packet->data_length != 8){ return 0;}
    memcpy(time, &an_packet->data[0], 8);
}

int decode_map_data_packet(map_data_packet_t *map_data_packet, an_packet_t *an_packet){
    map_data_packet->count = an_packet->count;
    memcpy(map_data_packet->time, &an_packet->data[0], 8);
    memcpy(map_data_packet->version, &an_packet->data[8], 23);
    //map_data_packet->number = an_packet->data[8];
    map_data_packet->number = an_packet->data[31];
    map_data_packet->roads.clear();
    map_data_packet->real_number = 0;
    //int iterator = 0;
    int iterator = 32;
    big_int64_t int64value;
    big_int32_t int32value;
    big_int16_t int16value;
    for(int i=0; i< map_data_packet->number && iterator < an_packet->data_length; i++){
        //for(int j=iterator; j< iterator+60;j++){
        //    printf("%02x ", an_packet->data[j]);
        //} cout << endl;
        //cout << "start reading one road : " << iterator << endl;
        map_road this_map_road;
        memcpy(&this_map_road.data_length, &an_packet->data[iterator],4);iterator += 4;
        //memcpy(&this_map_road.id[0], &an_packet->data[iterator],10); iterator += 10;
        memcpy(&this_map_road.id[0], &an_packet->data[iterator],36); iterator += 36;
        this_map_road.type = an_packet->data[iterator]; iterator ++;
        this_map_road.format = an_packet->data[iterator]; iterator ++;
        memcpy(&this_map_road.number, &an_packet->data[iterator], 4); iterator += 4;
        this_map_road.real_number = 0;
        this_map_road.points.clear();
        big_uint32_t iner_iterator = 42;
        //cout << "map_road data_length : " << this_map_road.data_length << endl;
        //cout << "start reading one road point: " << iterator << endl;
        //cout << "map_road data numbers: " << this_map_road.number << endl;
        while(iner_iterator < this_map_road.data_length && iterator < an_packet->data_length){
            map_road_point this_map_road_point;
            memcpy(&int64value,&an_packet->data[iterator],8); iterator += 8;
            this_map_road_point.x = int64value * 0.001;
            memcpy(&int64value,&an_packet->data[iterator],8); iterator += 8;
            this_map_road_point.y = int64value * 0.001;
            memcpy(&int32value,&an_packet->data[iterator],4); iterator += 4;
            this_map_road_point.z = int32value * 0.0001 - 1000;
            //this_map_road_point.z = int32value * 0.0001;
            memcpy(&int32value,&an_packet->data[iterator],4); iterator += 4;
            this_map_road_point.c = int32value * 0.00001;
            iner_iterator += 24;
            this_map_road.points.push_back(this_map_road_point);
            this_map_road.real_number++;
        }
        //cout << "done reading one road: " << iterator << endl;

        map_data_packet->roads.push_back(this_map_road);
        map_data_packet->real_number++;
    }

    return 1;
}

int decode_params_config_packet(params_config_packet_t *params_config_packet, an_packet_t *an_packet){
    if(!(an_packet->cmd_id == 0x81 && an_packet->request_id == 0xFE)){return -1;}
    memcpy(&params_config_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t));
    params_config_packet->number = an_packet->data[8];
    params_config_packet->count = an_packet->count;

    params_config_packet->report_cycle.clear();
    params_config_packet->domain_length.clear();
    params_config_packet->domain_params.clear();
    params_config_packet->cloud_port.clear();
    params_config_packet->software_version.clear();
    params_config_packet->hardware_version.clear();
    params_config_packet->vehicle_vin.clear();

    int count = 0;
    int iterator = 9; // pointing at first params id
    while(iterator < an_packet->data_length){
        if(an_packet->data[iterator] == 0x01){
            count ++;
            iterator++; // pointing at cycle value[0]
            params_config_packet->report_cycle.resize(1);
            params_config_packet->report_cycle[0] = 256 * an_packet->data[iterator++]; // pointing from value[0] to value[1]
            params_config_packet->report_cycle[0] +=  an_packet->data[iterator++]; // pointing from value[1] to next params id
        }else if(an_packet->data[iterator] == 0x02){
            /// read domain length
            count ++;
            iterator++;
            params_config_packet->domain_length.resize(1);
            //params_config_packet->domain_length[0] = an_packet->data[iterator++];// pointing at length to next params id
            params_config_packet->domain_length[0] = an_packet->data[iterator++];// pointing at start of domain
            // read domain string
            params_config_packet->domain_params.resize(params_config_packet->domain_length[0]);
            memcpy(&params_config_packet->domain_params[0], &an_packet->data[iterator], params_config_packet->domain_length[0] * sizeof(uint8_t));
            iterator += params_config_packet->domain_length[0];// pointing at from start of domain to next params id
        }else if(an_packet->data[iterator] == 0xff){
            // read domain string
            count ++;
            iterator++;
            params_config_packet->domain_params.resize(params_config_packet->domain_length[0]);
            memcpy(&params_config_packet->domain_params[0], &an_packet->data[iterator], params_config_packet->domain_length[0] * sizeof(uint8_t));
            iterator += params_config_packet->domain_length[0];// pointing at from start of domain to next params id
        }else if(an_packet->data[iterator] == 0x03){
            count ++;
            iterator++;
            params_config_packet->cloud_port.resize(1);
            params_config_packet->cloud_port[0] = 256 * an_packet->data[iterator++];
            params_config_packet->cloud_port[0] +=  an_packet->data[iterator++];
        }else if(an_packet->data[iterator] == 0x04){
            count ++;
            iterator++;
            params_config_packet->software_version.resize(10);
            memcpy(&params_config_packet->software_version[0], &an_packet->data[iterator], 10 * sizeof(uint8_t));
            iterator += 10;
        }else if(an_packet->data[iterator] == 0x05){
            count ++;
            iterator++;
            params_config_packet->hardware_version.resize(10);
            memcpy(&params_config_packet->hardware_version[0], &an_packet->data[iterator], 10 * sizeof(uint8_t));
            iterator += 10;
        }else if(an_packet->data[iterator] == 0x06){
            count ++;
            iterator++;
            params_config_packet->vehicle_vin.resize(20);
            memcpy(&params_config_packet->vehicle_vin[0], &an_packet->data[iterator], 20 * sizeof(uint8_t));
            iterator += 20;
        }else if(an_packet->data[iterator] == 0x07){
            count ++;
            iterator++;
        }else{
            iterator++;
        }
    }

    if(count == params_config_packet->number){
        ///  real number is not correct
        return 0;
    }

    return 1;
}

int decode_upgrade_packet(upgrade_packet_t *upgrade_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x01){return -1;}
    upgrade_packet->count = an_packet->count;
    memcpy(&upgrade_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t));
    // an_packet->data[8] is id
    upgrade_packet->url.resize(an_packet->data_length - 9);
    memcpy(&upgrade_packet->url[0], &an_packet->data[9], (an_packet->data_length - 9)*sizeof(uint8_t));
    return 1;
}

int decode_engine_packet(engine_packet_t *engine_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x02){return -1;}
    if(an_packet->data_length != 8 + 1 + 2){return 0;}
    engine_packet->count = an_packet->count;
    memcpy(&engine_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t));
    engine_packet->start = an_packet->data[9];
    engine_packet->redundancy = an_packet->data[10];
    return 1;
}
int decode_take_over_packet(take_over_packet_t *take_over_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x03){return -1;}
    if(an_packet->data_length != 8 + 1 + 2){return 0;}
    take_over_packet->count = an_packet->count;
    memcpy(&take_over_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t));
    take_over_packet->take_over = an_packet->data[9];
    take_over_packet->redundancy = an_packet->data[10];
    return 1;
}

int decode_take_over_cmd_packet(take_over_cmd_packet_t *take_over_cmd_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x04){return -1;}
    //if(an_packet->data_length != sizeof(take_over_cmd_packet_t) -1){return 0;}
    if(an_packet->data_length != 22){return 0;}
    take_over_cmd_packet->count = an_packet->count;
    memcpy(&take_over_cmd_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t));
    take_over_cmd_packet->gas_brake = an_packet->data[9];
    take_over_cmd_packet->hydraulic_retarder = an_packet->data[10];
    take_over_cmd_packet->pedal_percent = an_packet->data[11];
    memcpy(&take_over_cmd_packet->brake_acc, &an_packet->data[12], 2 * sizeof(uint8_t));
    memcpy(&take_over_cmd_packet->hand_wheel, &an_packet->data[14], 2 * sizeof(uint8_t));
    take_over_cmd_packet->push_up = an_packet->data[16];
    memcpy(&take_over_cmd_packet->ego_vehicle[0], &an_packet->data[17], 2 * sizeof(uint8_t));
    take_over_cmd_packet->gear = an_packet->data[19];
    take_over_cmd_packet->gear_limit = an_packet->data[20];
    take_over_cmd_packet->hand_brake = an_packet->data[21];

    return 1;
}
int decode_restrict_speed_packet(restrict_speed_packet_t *restrict_speed_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x05){return -1;}
    restrict_speed_packet->count = an_packet->count;
    big_uint16_t uint16value = 0;
    memcpy(&uint16value, &an_packet->data[9], 2*sizeof(uint8_t));
    restrict_speed_packet->speed = uint16value * 0.1;
    return 1;
}

int decode_estop_packet(estop_packet_t *estop_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x06){return -1;}
    estop_packet->count = an_packet->count;
    estop_packet->estop = an_packet->data[9];
    return 1;
}

int decode_auto_avoid_packet(auto_avoid_packet_t *auto_avoid_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x07){return -1;}
    auto_avoid_packet->count = an_packet->count;
    auto_avoid_packet->auto_avoid = an_packet->data[9];
    return 1;
}

int decode_login_respond_packet(login_respond_packet_t *login_respond_packet, an_packet_t *an_packet){
    if(!(an_packet->cmd_id == 0x83 && an_packet->request_id == 0x01)){return -1;}
    if(an_packet->data_length != 28){return 0; }
    memcpy(&login_respond_packet->jianquan[0], &an_packet->data[0], 20*sizeof(uint8_t));
    memcpy(&login_respond_packet->invalid_time[0], &an_packet->data[20], 8*sizeof(uint8_t));
    return 1;
}

int decode_assignment_packet(assignment_packet_t *assignment_packet, an_packet_t *an_packet){
    if(an_packet->data[8] != 0x01) {return -1;}
    int iterator = 0; // pointing at start of time
    assignment_packet->count = an_packet->count;
    memcpy(&assignment_packet->time[0], &an_packet->data[iterator], 8*sizeof(uint8_t));
    iterator += 8; // pointing at assignment id
    iterator ++;  // poiting at numbers of task
    assignment_packet->number = an_packet->data[iterator];
    assignment_packet->tasks.clear();
    iterator++; // pointing at first task length
    int count = 0;
    while(iterator < an_packet->data_length){
        task this_task;
        memcpy(&this_task.data_length,&an_packet->data[iterator], 4*sizeof(uint8_t) );
        //iterator += 2; // pointing at task serial number
        iterator += 4; // pointing at task serial number
        this_task.serial_number = an_packet->data[iterator++]; // pointing at task id
        //memcpy(&this_task.id[0], &an_packet->data[iterator], 17*sizeof(uint8_t));
        //iterator += 17; // pointint at type
        memcpy(&this_task.id[0], &an_packet->data[iterator], 20*sizeof(uint8_t));
        iterator += 20; // pointint at type
        this_task.type = an_packet->data[iterator++]; // pointing at gear
        this_task.gear = an_packet->data[iterator++]; // pointing at format
        this_task.format = an_packet->data[iterator++]; // pointing at number of points
        memcpy(&this_task.point_number, &an_packet->data[iterator], 4*sizeof(uint8_t));
        this_task.points.clear();
        iterator += 4; // pointing at first point's latitude
        uint32_t point_count = 0;
        big_int64_t int64value;
        big_int32_t int32value;
        big_uint16_t int16value;
        for(int pointCount = 0; pointCount < this_task.point_number && iterator < an_packet->data_length; pointCount++){
            trajectory_point point;
            memcpy(&int64value,  &an_packet->data[iterator], 8*sizeof(uint8_t));
            point.lontitude = int64value * 0.001;
            iterator += 8; // pointing at point lontitude
            memcpy(&int64value,  &an_packet->data[iterator], 8*sizeof(uint8_t));
            point.latitude = int64value * 0.001;
            iterator += 8; // pointing at point height
            memcpy(&int32value,  &an_packet->data[iterator], 4*sizeof(uint8_t));
            point.height = int32value * 0.0001 - 1000;
            iterator += 4; // pointing at point limit speed
            point.limit_speed = an_packet->data[iterator++];  // pointing at heading
            memcpy(&int16value,  &an_packet->data[iterator], 2*sizeof(uint8_t));
            point.heading = int16value*0.01;
            iterator += 2; // pointing at point coefficient
            memcpy(&int32value,  &an_packet->data[iterator], 4*sizeof(uint8_t));
            point.coefficient = int32value * 0.0001;
            iterator += 4; // pointing at next point start, latitude
            this_task.points.push_back(point);
            point_count ++;
        }
        this_task.real_point_number = point_count;
        assignment_packet->tasks.push_back(this_task);
        count ++;
    }
    assignment_packet->real_number = count;
    return 1;
}

int decode_assignment_ajust_packet(assignment_ajust_packet_t *assignment_ajust_packet, an_packet_t *an_packet){
    //if(!(an_packet->cmd_id == 0x84 && an_packet->request_id == 0xFE)){ return -1;}
    if(an_packet->data[8] != 0x01) {return -1;}
    int iterator = 0; // pointing at start of time
    memcpy(&assignment_ajust_packet->time[0], &an_packet->data[iterator], 8*sizeof(uint8_t));
    iterator += 8; // pointing at assignment_ajust id
    iterator ++;  // poiting at type of assignment_ajust, update or append
    assignment_ajust_packet->type = an_packet->data[iterator];
    iterator ++;  // poiting at numbers of task
    assignment_ajust_packet->number = an_packet->data[iterator];
    //assignment_ajust_packet->tasks.resize(assignment_ajust_packet->number);
    assignment_ajust_packet->tasks.clear();
    iterator++; // pointing at first task length
    int count = 0;
    while(iterator < an_packet->data_length){
        task this_task;
        memcpy(&this_task.data_length,&an_packet->data[iterator], 2*sizeof(uint8_t) );
        iterator += 2; // pointing at task serial number
        this_task.serial_number = an_packet->data[iterator++]; // pointing at task id
        memcpy(&this_task.id[0], &an_packet->data[iterator], 20*sizeof(uint8_t));
        iterator += 20; // pointint at type
        this_task.type = an_packet->data[iterator++]; // pointing at gear
        this_task.gear = an_packet->data[iterator++]; // pointing at format
        this_task.format = an_packet->data[iterator++]; // pointing at number of points
        memcpy(&this_task.point_number, &an_packet->data[iterator], 4*sizeof(uint8_t));
        //this_task.points.resize(this_task.point_number);
        this_task.points.clear();
        iterator += 4; // pointing at first point
        uint32_t point_count = 0;
        for(int pointCount = 0; pointCount < this_task.point_number && iterator < an_packet->data_length; pointCount++){
            trajectory_point point;
            memcpy(&point.latitude,  &an_packet->data[iterator], 8*sizeof(uint8_t));
            iterator += 8; // pointing at point lontitude
            memcpy(&point.lontitude,  &an_packet->data[iterator], 8*sizeof(uint8_t));
            iterator += 4; // pointing at point height
            memcpy(&point.height,  &an_packet->data[iterator], 4*sizeof(uint8_t));
            iterator += 4; // pointing at point limit speed
            point.limit_speed = an_packet->data[iterator++];  // pointing at heading
            memcpy(&point.heading,  &an_packet->data[iterator], 2*sizeof(uint8_t));
            iterator += 2; // pointing at point coefficient
            memcpy(&point.coefficient,  &an_packet->data[iterator], 4*sizeof(uint8_t));
            iterator += 4; // pointing at next point start
            this_task.points[pointCount] = point;
            point_count ++;
        }
        this_task.real_point_number = point_count;
        //if(count < assignment_ajust_packet->tasks.size()){
        //    assignment_ajust_packet->tasks[count] = this_task;
        //}else{
        //    assignment_ajust_packet->tasks.push_back(this_task);
        //}
        assignment_ajust_packet->tasks.push_back(this_task);
        count ++;
    }
    return 1;
}

int decode_information_packet(information_packet_t *information_packet, an_packet_t *an_packet){
    memcpy(&information_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t) );
    information_packet->type = an_packet->data[8];
    information_packet->content.resize(an_packet->data_length - 9);
    memcpy(&information_packet->content[0],  &an_packet->data[9], (an_packet->data_length - 9) * sizeof(uint8_t) );
    return 1;
}
int decode_rtcm_packet(rtcm_packet_t *rtcm_packet, an_packet_t *an_packet){
    memcpy(&rtcm_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t) );
    memcpy(&rtcm_packet->data_length, &an_packet->data[8], 2*sizeof(uint8_t) );
    rtcm_packet->data.resize(rtcm_packet->data_length);
    memcpy(&rtcm_packet->data[0], &an_packet->data[10], rtcm_packet->data_length * sizeof(uint8_t) );
    return 1;
}
int decode_surrd_packet(surrd_vehicle_packet_t *surrd_vehicle_packet, an_packet_t *an_packet){
    memcpy(&surrd_vehicle_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t) );
    memcpy(&surrd_vehicle_packet->number, &an_packet->data[8], 1*sizeof(uint8_t) );
    int iterator = 9;
    surrd_vehicle_packet->vehicles.clear();
    for(int i=0; i<surrd_vehicle_packet->number;i++){
        one_vehicle vehicle;
        memcpy(&vehicle.id[0], &an_packet->data[iterator], 10*sizeof(uint8_t) );
        iterator += 10;
        big_int64_t int64value;
        memcpy(&int64value, &an_packet->data[iterator], 8*sizeof(uint8_t) );
        vehicle.x = int64value * 0.001;
        memcpy(&int64value, &an_packet->data[iterator], 8*sizeof(uint8_t) );
        vehicle.y = int64value * 0.001;
        big_int32_t int32value;
        memcpy(&int32value, &an_packet->data[iterator], 4*sizeof(uint8_t) );
        vehicle.y = int32value * 0.001;
        iterator += 4;
        vehicle.speed = an_packet->data[iterator++];
        big_uint16_t uint16value;
        memcpy(&uint16value, &an_packet->data[iterator], 2*sizeof(uint8_t) );
        vehicle.heading = uint16value * 0.01;
        iterator += 2;
        vehicle.type = an_packet->data[iterator++];
        vehicle.length = an_packet->data[iterator++];
        vehicle.width = an_packet->data[iterator++];
        vehicle.height = an_packet->data[iterator++];
        surrd_vehicle_packet->vehicles.push_back(vehicle);
    }
}

int decode_sample_gis_packet(sample_gis_packet_t *sample_gis_packet, an_packet_t *an_packet){
    memcpy(&sample_gis_packet->time[0], &an_packet->data[0], 8*sizeof(uint8_t) );
    sample_gis_packet->type = an_packet->data[8];
    memcpy(&sample_gis_packet->duration,  &an_packet->data[9], 2 * sizeof(uint8_t) );
    sample_gis_packet->area = an_packet->data[11];
    memcpy(&sample_gis_packet->unload_position[0],  &an_packet->data[12], 32 * sizeof(uint8_t) );
    sample_gis_packet->is_new = true;
    return 1;
}
