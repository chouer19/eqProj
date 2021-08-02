#ifndef GL8_STRUCTURES_H
#define GL8_STRUCTURES_H


/* Can Frame Type Define */
struct CAN_FRM_TYPE
{
    CAN_FRM_TYPE()
    :canid(0), RTR(0), IDE(0), length(0), MB_NUM(0)
    {
        for(uint8_t i = 0; i < 8; i++)
            data[i] = 0;
    }
    CAN_FRM_TYPE(const uint32_t &_id,
                 const uint8_t &_rtr,
                 const uint8_t &_ide,
                 const uint8_t *_data,
                 const uint32_t &_len,
                 const uint8_t _num)
    :canid(_id), RTR(_rtr), IDE(_ide), length(_len), MB_NUM(_num)
    {
        for(uint8_t i = 0; i < 8; i++)
            data[i] = _data[i];
    }
    uint32_t canid;
    uint8_t RTR;
    uint8_t IDE;
    uint8_t data[8];
    uint32_t length;
    uint8_t MB_NUM;
};

struct CAN_FRM_WITH_TIME
{
    CAN_FRM_WITH_TIME()
    :time_stamp(0)
    {}
    CAN_FRM_WITH_TIME(const uint32_t &t, const CAN_FRM_TYPE &frm)
    :time_stamp(t), can_frame(frm)
    {}
    uint32_t time_stamp;
    CAN_FRM_TYPE can_frame;
};



//车辆相关
struct VEH_FRM
{
    struct{
        size_t time_stamp;
        //Byte0~1
        bool vehicle_speed_driven_invalid;//Byte0.bit7_len1, 1 = true, 0 = false
        float vehicle_speed_driven;//Byte0.bit6_len15, E = N * 0.015625, kmph
        //Byte2
        uint8_t vehicle_speed_driven_src;//Byte2.bit7_len1, 0 = transmission, 1 = wheel speed
        float distance_rolling_count_driven_invalid;//Byte2.bit6_len1, 1 = true, 0 = false
        bool distance_rolling_count_driven_reset_occured;//Byte2.bit5_len1, 1 = true, 0 = false
        float distance_rolling_count_driven;//Byte2.bit4_len13, E = N * 0.125, m
        //Byte4
        bool vehicle_speed_non_driven_invalid;//Byte4.bit7_len1, 1 = true, 0 = false
        float vehicle_speed_non_driven;//Byte4.bit6_len15, E = N * 0.015625, kmph
        //Byte6
        uint8_t vehicle_speed_non_driven_src;//Byte6.bit7_len1, 0 = transmission, 1 = wheel speed
        float distance_rolling_count_non_driven_invalid;//Byte6.bit6_len1, 1 = true, 0 = false
        bool distance_rolling_count_non_driven_reset_occured;//Byte6.bit5_len1, 1 = true, 0 = false
        float distance_rolling_count_non_driven;//Byte6.bit4_len13, E = N * 0.125, m
    } _0x3E9;

    struct{
        size_t time_stamp;
        bool brake_lamp_act;//Byte0.bit6_len1, 1 = true, 0 = false
        bool left_turn_lamp_act;//Byte2.bit2_len1, 1 = true, 0 = false
        bool right_turn_lamp_act;//Byte2.bit3_len1, 1 = true, 0 = false
    } _0x140;

    struct{
        size_t time_stamp;
        //Byte0
        bool wiper_act;        //Byte0.bit6_len1, 1 = true, 0 = false
        bool windshield_washer_switch_active;//Byte0.bit5_len1, 1 = true, 0 = false
        bool tcs_switch_act;//Byte0.bit4_len1, 1 = true, 0 = false
        bool tcs_switch_act_valid;//Byte0.bit3_len1, 1 = true, 0 = false
        uint8_t vehicle_move_status;//Byte0.bit2_len3, 0 = parked, 1 = neural, 2 = forward, 3 = reverse, 4 = invalid
        //Byte1
        uint8_t collision_preparation_sys_custom_request;//Byte1.bit7_len3, 0 = no action, 1 = off, 2 = alert, 3 = alert and brake
        bool rear_closure_ajar_switch_active;//Byte1.bit4_len1, 1 = true, 0 = false
        //Byte2
        uint8_t seat_belt_tight_custom_request;//Byte2.bit7_len3, 0 = no action, 1 = low, 2 = nomal, 3 = performance, 4 = locked
        uint8_t calender_day;//Byte2.bit4_len5, E = N * 1
        //Byte4
        uint8_t calender_month;//Byte4.bit7_len4, E = N * 1
        //Byte5
        uint16_t calender_year;//Byte5.bit7_len8, E = N * 1 + 2000
        //Byte7
        bool steer_angle_virtual_dev_available;//Byte7.bit7_len1, 1 = true, 0 = false
        bool seconds_invalid;//Byte7.bit6_len1, 1 = true, 0 = false
        uint8_t seconds;//Byte7.bit5_len6, E = N * 1

    } _0x135;

    struct{
        size_t time_stamp;
        //Byte0
        bool front_fog_lamp_act;//Byte0.bit7_len1, 1 = true, 0 = false
        uint8_t turn_switch_act;//Byte0.bit6_len2, 0 = no act, 1 = left, 2 = right
        bool park_light_right_indication_on;//Byte0.bit4_len1, 1 = true, 0 = false
        bool park_light_left_indication_on;//Byte0.bit3_len1, 1 = true, 0 = false
        bool hazard_switch_act;//Byte0.bit2_len1, 1 = true, 0 = false
        bool daytime_lamp_act;//Byte0.bit1_len1, 1 = true, 0 = false
        bool outside_ambient_light_level_status_valid;//Byte0.bit0_len1, 1 = true, 0 = false
        //Byte1
        bool brake_light_act;//Byte1.bit7_len1, 1 = true, 0 = false
        bool parl_light_indication_on;//Byte1.bit6_len1, 1 = true, 0 = false
        bool high_beam_indication_on;//Byte1.bit5_len1, 1 = true, 0 = false
        bool rear_fog_light_indication_on;//Byte1.bit4_len1, 1 = true, 0 = false
        bool auto_light_act_indication_on;//Byte1.bit3_len1, 1 = true, 0 = false
        bool auto_light_inact_indication_on;//Byte1.bit2_len1, 1 = true, 0 = false
        bool rfront_fog_light_indication_on;//Byte1.bit1_len1, 1 = true, 0 = false
        bool park_light_left_on;//Byte1.bit0_len1, 1 = true, 0 = false
        //Byte2
        bool surveillance_mode_act;//Byte2.bit7_len1, 1 = true, 0 = false
        bool park_light_right_on;//Byte2.bit6_len1, 1 = true, 0 = false
        bool auto_beam_sel_allowed;//Byte2.bit5_len1, 1 = true, 0 = false
        bool high_beam_requested;//Byte2.bit4_len1, 1 = true, 0 = false
        bool rear_fog_lamp_act;//Byte2.bit3_len1, 1 = true, 0 = false
        bool park_lights_act;//Byte2.bit2_len1, 1 = true, 0 = false
        bool reverse_lamp;//Byte2.bit1_len1, 1 = true, 0 = false
        bool flash2pass_switch_act;//Byte2.bit0_len1, 1 = true, 0 = false
        //Byte3
        uint8_t main_light_switch;//Byte3.bit7_len2, 0 = Auto, 1 = OFF, 2 = Park Lamp, 3 = Head lamp
        uint8_t left_turn_lamp_act;//Byte3.bit5_len2, 0 = OFF, 1 = On without telltale, 2 = On with telltale
        uint8_t outside_ambient_light_level_status;//Byte3.bit3_len2, 0 = unknown, 1 = Night, 2 = Day
        uint8_t right_turn_lamp_act;//Byte3.bit1_len2, 0 = OFF, 1 = On without telltale, 2 = On with telltale
        //Byte4
        uint8_t headlamp_beam_sel_status;//Byte4.bit1_len2, 0 = unknown, 1 = Low Beam, 2 = High Beam
        //Byte5
        bool high_beam_act;    //Byte5.bit7_len1, 1 = true, 0 = false
        bool auto_light_control;//Byte5.bit4_len1, 1 = true, 0 = false
        bool advanced_front_light_sys_enabled;//Byte5.bit3_len1, 1 = true, 0 = false
        bool low_beam_act;     //Byte5.bit1_len1, 1 = true, 0 = false
    } _0x106;

    struct{
        size_t time_stamp;
        bool yaw_rate_invalid; //Byte4.bit4_len1, 1 = true, 0 = false
        float yaw_rate;        //Byte4.bit3_len12, E = N * 0.0625 - 128, deg/sec
    } _0x1E9;

    struct{
        size_t time_stamp;
        float lon_accel_sen_value;//Byte0.bit7_len16, E = N * 0.00012207 - 3.99998976, g's
        float lat_accel_sen_value;//Byte2.bit7_len16, E = N * 0.00012207 - 3.99998976, g's
    } _0x1FC;

    struct{
        size_t time_stamp;
        bool wheel_ground_velocity_left_driven_invalid;//Byte0.bit6_len1, 1 = true, 0 = false
        float wheel_ground_velocity_left_driven;//Byte0.bit5_len14, E = N * 0.03125, kmph
        bool wheel_ground_velocity_right_driven_invalid;//Byte2.bit6_len1, 1 = true, 0 = false
        float wheel_ground_velocity_right_driven;//Byte2.bit5_len14, E = N * 0.03125, kmph
        uint8_t wheel_rotat_direct_status_left_driven;//Byte4.bit5_len3, 0 = unknown, 1 = forward, 2 = reverse, 3 = not supported, 4 = failed, 5-7 = reserved
        uint8_t wheel_rotat_direct_status_right_driven;//Byte4.bit2_len3, 0 = unknown, 1 = forward, 2 = reverse, 3 = not supported, 4 = failed, 5-7 = reserved

    } _0x348;

    struct{
        size_t time_stamp;
        bool wheel_ground_velocity_left_non_driven_invalid;//Byte0.bit6_len1, 1 = true, 0 = false
        float wheel_ground_velocity_left_non_driven;//Byte0.bit5_len14, E = N * 0.03125, kmph
        bool wheel_ground_velocity_right_non_driven_invalid;//Byte2.bit6_len1, 1 = true, 0 = false
        float wheel_ground_velocity_right_non_driven;//Byte2.bit5_len14, E = N * 0.03125, kmph
        uint8_t wheel_rotat_direct_status_left_non_driven;//Byte4.bit5_len3, 0 = unknown, 1 = forward, 2 = reverse, 3 = not supported, 4 = failed, 5-7 = reserved
        uint8_t wheel_rotat_direct_status_right_non_driven;//Byte4.bit2_len3, 0 = unknown, 1 = forward, 2 = reverse, 3 = not supported, 4 = failed, 5-7 = reserved

    } _0x34A;
};
//ESP
struct ESP_FRM
{
    struct
    {
        size_t time_stamp;
        //Byte0
        bool brk_pedal_moderate_travel_achieved_invalid;//Byte0.bit7_len1, 1 = true, 0 = false
        bool brk_pedal_moderate_travel_achieved;//Byte0.bit6_len1, 1 = true, 0 = false
        uint8_t brk_pedal_pos_alive_roll_cnt;//Byte0.bit5_len2, E = N * 1
        uint8_t brk_pedal_initial_travel_achieved_protection;//Byte0.bit3_len2, E = N * 1
        bool brk_pedal_initial_travel_achieved;//Byte0.bit1_len1, 1 = true, 0 = false
        bool brk_pedal_initial_travel_achieved_invalid;//Byte0.bit0_len1, 1 = true, 0 = false
        //Byte1
        float brk_pedal_pos;//Byte1.bit7_len8, E = N * 0.392157, %
        //Byte2
        int16_t brk_pedal_pos_gradient;//Byte2.bit7_len8, E = N * 40 - 5120, %/s
        //Byte3~5
        bool brk_apply_sensor_home_pos_learned;//Byte3.bit6_len1, 1 = true, 0 = false
        uint8_t notification_event_status;/* Byte3.bit5_len2
                                           * 0 = no event
                                           * 1 = near deployment event
                                           * 2 = pretensioner deployment event
                                           * 3 = airbag deployment event
                                           */
        uint32_t brk_pedal_check_data;//Byte3.bit0_len17, E = N * 1
    }_0x0F1;

    struct
    {
        size_t time_stamp;
        //Byte0
        uint8_t elec_park_brk_switch_status;/* Byte0.bit7_len2
                                             * 0 = not active
                                             * 1 = release
                                             * 2 = apply
                                             * 3 = active
                                             */
        bool elec_park_brk_switch_status_invalid;//Byte0.bit5_len1, 1 = true, 0 = false
        uint8_t elec_park_brk_sys_display_msg_req;/* Byte0.bit4_len3
                                                   * 0 = off
                                                   * 1~7 = message #1~#7
                                                   */
        uint8_t elec_park_sys_warn_indication_req;/* Byte0.bit1_len2
                                                   * 0 = no indication
                                                   * 1 = continous indication
                                                   * 2 = flash rate #1 indication
                                                   * 3 = flash rate #2 indication
                                                   */
        //Byte1
        uint8_t elec_park_brk_status;/* Byte1.bit5_len2
                                      * 0 = release
                                      * 1 = applied
                                      * 2 = fault
                                      */
        uint8_t elec_park_brk_sys_audible_warn_req;/* Byte1.bit3_len2
                                                    * 0 = off
                                                    * 1 = warning #1
                                                    * 2 = warning #2
                                                    */
        //Byte2
        bool elec_park_brk_sys_brk_light_req;//Byte2.bit6_len1, 1 = true, 0 = false
        uint8_t elec_park_brk_command_alive_roll_cnt;//Byte2.bit5_len2, E = N * 1
        uint8_t elec_park_brk_sys_status_indication_req;/* Byte2.bit1_len2
                                                         * 0 = no indication
                                                         * 1 = continous indication
                                                         * 2 = flash rate #1 indication
                                                         * 3 = flash rate #2 indication
                                                         */
        //Byte3
        uint8_t all_nodes_virtual_netwark_act;//Byte3.bit7_len1, 1 = activation, 0 = no activation
        bool step_on_brk_to_release_park_brk_indication_on;//Byte3.bit6_len1, 1 = true, 0 = false
        bool elec_park_brk_cruise_control_cancel_req;//Byte3.bit4_len1, 1 = true, 0 = false
        //Byte4~5
        bool elec_park_brk_auto_brk_req;//Byte4.bit4_len1, 1 = true, 0 = false
        float elec_park_brk_acceleration_req;//Byte4.bit3_len12, E = N * 0.01 - 20.48, m/s^2
        uint16_t elec_park_brk_command_protection;//Byte6.bit4_len13, E = N * 1
    }_0x230;


};
//EPS
struct EPS_FRM
{
    struct{
        size_t time_stamp;
        //Byte0
        bool steer_wheel_ang_invalid;//Byte0.bit7_len1, 1 = true, 0 = false
        uint8_t steer_wheel_ang_sensor_calib_status;/* Byte0.bit6_len2
                                                     * 0 = unknown
                                                     * 1 = estimated
                                                     * 2 = calibrated
                                                     */
        uint8_t steer_wheel_ang_sensor_type;//Byte0.bit4_len2, 0~3 = type #1~#4
        uint8_t steer_wheel_ang_mask;//Byte0.bit2_len1, 0 = don't use data, 1 = use data
        //Byte1~2
        float steer_wheel_ang;//Byte1.bit7_len16, E = N * 0.0625 - 2048
        //Byte3~4
        bool steer_wheel_ang_gradient_invalid;//Byte3.bit7_len1, 1 = true, 0 = false
        uint8_t steer_wheel_ang_alive_roll_cnt;//Byte3.bit6_len2, E = N * 1
        uint8_t steer_wheel_ang_gradient_mask;//Byte3.bit4_len1, 0 = don't use data, 1 = use data
        int16_t steer_wheel_ang_gradient;//Byte3.bit3_len12, E = N * 1 - 2048
        //Byte6~7
        uint16_t steer_wheel_ang_sensor_checksum;//Byte6.bit2_len11, E = N * 1
    } _0x1E5;
};
//Throt
struct THROT_FRM
{
    struct{
        size_t time_stamp;
        //Byte0
        bool engine_run_act;//Byte0.bit7_len1, 1 = true, 0 = false
        bool powertrain_crank_act;//Byte0.bit6_len1, 1 = true, 0 = false
        bool powertrain_crank_aborted;//Byte0.bit5_len1, 1 = true, 0 = false
        bool powertrain_run_aborted;//Byte0.bit4_len1, 1 = true, 0 = false
        bool power_take_off_powertrain_aboted;//Byte0.bit3_len1, 1 = true, 0 = false
        bool engine_idle_act;//Byte0.bit2_len1, 1 = true, 0 = false
        uint8_t engine_speed_status;//Byte0.bit1_len2, 0 = normal operation, 1 = degraded operation, 3 = invalid
        //Byte1~2
        float engine_speed;//Byte1.bit7_len16, E = N * 0.25, rpm
        //Byte3
        bool accelerator_actual_pos_invalid;//Byte3.bit7_len1, 1 = true, 0 = false
        bool cruise_control_act;//Byte3.bit6_len1, 1 = true, 0 = false
        bool cruise_control_enabled;//Byte3.bit5_len1, 1 = true, 0 = false
        bool driver_throt_override_detected;//Byte3.bit4_len1, 1 = true, 0 = false
        uint8_t driver_throt_override_detect_alive_roll_count;//Byte3.bit3_len2, E = N * 1
        uint8_t driver_throt_override_detected_protection_value;//Byte3.bit1_len2, E = N * 1
        //Byte4
        float accelerator_actual_pos;//Byte4.bit7_len8, E = N * 0.392157, %
        //Byte5
        bool speed_limiter_warn_enabled;//Byte5.bit7_len1, 1 = true, 0 = false
        bool platform_engine_speed_command_seperceded;//Byte5.bit5_len1, 1 = true, 0 = false
        bool platform_engine_speed_command_inhibit_request;//Byte5.bit4_len1, 1 = true, 0 = false
        uint8_t engine_cylinder_deactivation_mode;/* Byte5.bit3_len2
                                                   * 0 = all cylinders active
                                                   * 1 = deactivation in process
                                                   * 2 = half of total cylinders active
                                                   * 3 = reactivation in process
                                                   */
        bool powertrain_brk_pedal_discrete_input_status_invalid;//Byte5.bit1_len1, 1 = true, 0 = false
        uint8_t powertrain_brk_pedal_discrete_input_status;//Byte5.bit0_len1, 1 = brake applied, 0 = brake not applied
        //Byte6
        bool remote_vehicle_start_engine_running;//Byte6.bit7_len1, 1 = true, 0 = false
        bool engine_cylinder_deactivation_event_pending;//Byte6.bit6_len1, 1 = true, 0 = false
        bool engine_12v_starter_motor_commanded_on;//Byte6.bit5_len1, 1 = true, 0 = false
        uint8_t engine_controller_run_crank_terminal_status;//Byte6.bit4_len1, 1 = active, 0 = inactive
        bool engine_intake_air_boost_pressure_invalid;//Byte6.bit3_len1, 1 = true, 0 = false
        uint8_t personalization_elevated_idle_control_status;//Byte6.bit1_len1, 1 = enabled, 0 = disabled
        //Byte7
        int8_t engine_intake_air_boost_pressure;//Byte7.bit7_len8, E = N * 1 - 128
    } _0x0C9;


    struct{
        size_t time_stamp;
        //Byte0
        bool throt_pos_invalid;//Byte0.bit7_len1, 1 = true, 0 = false
        uint8_t auto_mode_speed_limit_status;/* Byte0.bit6_len2
                                              * 0 = no speed limit change
                                              * 1 = proposed speed limit change
                                              * 2 = accept speed limit change
                                              * 3 = decline speed limit change
                                              */
        bool speed_limiter_warn_act;//Byte0.bit4_len1, 1 = true, 0 = false
        bool engine_non_emissions_related_malfunction_act;//Byte0.bit3_len1, 1 = true, 0 = false
        uint8_t engine_emissions_related_malfunction_indication_request;/* Byte0.bit2_len3
                                                                         * 0 = continuous indication
                                                                         * 1 = no indication
                                                                         * 2 = flashing 1 Hz indication
                                                                         * 3 = flashing 2 Hz indication
                                                                         * 4 = flashing 0.5 Hz indication
                                                                         */
        //Byte1
        float throt_pos;//Byte1.bit7_len8, E = N * 0.392157, %
        //Byte2~3
        bool engine_boost_pressure_indication_invalid;//Byte2.bit7_len1, 1 = true, 0 = false
        bool engine_coast_fuel_cut_off_active;//Byte2.bit6_len1, 1 = true, 0 = false
        bool engine_oil_starvation_indication_on;//Byte2.bit5_len1, 1 = true, 0 = false
        float cruise_speed_limiter_driver_sel_speed;//Byte2.bit3_len12, E = N * 0.0625, kmph
        //Byte4~5
        bool cruise_control_driver_sel_speed_act;//Byte4.bit7_len1, 1 = true, 0 = false
        bool fuel_filter_life_reset_performed;//Byte4.bit6_len1, 1 = true, 0 = false
        uint8_t air_condition_compress_command;//Byte4.bit5_len1, 1 = on, 0 = off
        bool engine_oil_life_reset_performed;//Byte4.bit4_len1, 1 = true, 0 = false
        float instantaneous_fuel_consumption_rate;//Byte4.bit3_len12, E = N * 0.025
        //Byte6
        float engine_boost_pressure_indication;//Byte6.bit7_len8, E = N * 0.392157, %
        //Byte7
        float air_condition_compress_normilized_load_gradient_allowed;//Byte7.bit7_len8, E = N * 0.1, dm3/m/s

    } _0x3D1;
};
//Gear
struct GEAR_FRM
{
    struct{
        size_t time_stamp;
        //Byte0
        uint8_t trans_torque_convert_clutch_command_mode;//Byte0.bit7_len3, 0 = unlocked, 1 = transitioning, 2 = controlled slip, 3 = locked, 7 = not supported
        bool trans_estimated_gear_invalid;//Byte0.bit4_len1, 1 = true, 0 = false
        uint8_t trans_estimated_gear;//Byte0.bit3_len4,0 = not surported, 1~8 = gear, A = EVT mode1, B = EVT mode2, C = CVT gear D, D = gear N, E = gear R, F = gear P
        //Byte1
        bool top_travel_clutch_switch_act_invalid;//Byte1.bit7_len1, 1 = true, 0 = false
        bool top_travel_clutch_switch_act;//Byte1.bit6_len1, 1 = true, 0 = false
        uint8_t auto_trans_gear_shift_direct;//Byte1.bit5_len2, 0 = no shift in process, 1 = up shift in process, 2 = down shift in process
        uint8_t auto_trans_command_gear;//Byte1.bit3_len4, 0 = not surported, 1~8 = gear, A = EVT mode1, B = EVT mode2, C = CVT gear D, D = gear N, E = gear R, F = gear P
        //Byte2
        bool trans_skip_shift_indication_on;//Byte2.bit7_len1, 1 = true, 0 = false
        bool engine_recommend_upshift_indication_on;//Byte2.bit6_len1, 1 = true, 0 = false
        bool driver_shift_request_denied_indication_on;//Byte2.bit5_len1, 1 = true, 0 = false
        uint8_t driver_shift_target_gear;//Byte2.bit3_len4,0 = not surported, 1~8 = gear
        //Byte3
        uint8_t trans_range_inhibit_status;//Byte3.bit7_len3, 0 = no range inhibit active, 1 = transmission inhibit active, 2 = transmission range re-selection req, 3 = driveline not engaged, 4 = diagnostic inhibit active
        bool trans_shift_lever_pos_invalid;//Byte3.bit4_len1, 1 = true, 0 = false
        uint8_t trans_shift_lever_pos;//Byte3.bit3_len4, 0 = between ranges, 1 = park range, 2 = reverse range, 3 = neutral range, 4~B = forward range A~H, F = unknown
        //Byte4
        bool clutch_start_switch_act_invalid;//Byte4.bit6_len1, 1 = true, 0 = false
        bool clutch_start_switch_act;//Byte4.bit5_len1, 1 = true, 0 = false
        uint8_t trans_shift_mode_status;//Byte4.bit2_len3, 0 = no override mode, 1 = performance mode, 2 = loft_foot cornering mode, 3 = auto gear braking mode
        //Byte5
        bool trans_shift_lever_lock_request;//Byte5.bit7_len1, 1 = true, 0 = false
        bool trans_creep_mode_act;//Byte5.bit6_len1, 1 = true, 0 = false
        uint8_t trans_shift_pattern_act_status;//Byte5.bit4_len3, 0 = default shift pattern active, 1~4 = shift pattern 1~4, 5 = PT non-protection pattern active, 6 = PT protection pattern active
        uint8_t trans_tap_up_down_mode_status;//Byte5.bit1_len2, 0 = no action, 1 = driver shift control active, 2 = electric range select active
        //Byte6
        bool clutch_pedal_actual_pos_invalid;//Byte6.bit7_len1, 1 = true, 0 = false
        bool engine_recommand_downshift_indication_on;//Byte6.bit6_len1, 1 = true, 0 = false
        bool trans_pattern_sel_economy_mode_override_command;//Byte6.bit4_len1, 1 = true, 0 = false
        bool trans_fluid_pressure_present;//Byte6.bit3_len1, 1 = true, 0 = false
        bool trans_engaged_state_invalid;//Byte6.bit2_len1, 1 = true, 0 = false
        uint8_t trans_engaged_state;//Byte6.bit1_len2, 0 = transmission not engaged, 1 = transmission engaged in forward 2 = transmission engaged in reverse, 3 = operating state not reached
        //Byte7
        float clutch_pedal_actual_pos;//Byte7.bit7_len8, E = N * 0.392157
    } _0x1F5;
};

struct CAN_FRM
{
    VEH_FRM veh;
    ESP_FRM esp;
    EPS_FRM eps;
    THROT_FRM throt;
    GEAR_FRM gear;
};

struct DATA_COLLECTION
{
    uint8_t vehicle_speed_driven_src;
    bool vehicle_speed_driven_invalid;
    float vehicle_speed_driven;
    uint8_t vehicle_speed_non_driven_src;
    bool vehicle_speed_non_driven_invalid;
    float vehicle_speed_non_driven;
    bool yaw_rate_invalid;
    float yaw_rate;
    float lon_accel_sen_value;
    float lat_accel_sen_value;
    bool  wheel_ground_velocity_left_driven_invalid;
    float wheel_ground_velocity_left_driven;
    bool wheel_ground_velocity_right_driven_invalid;
    float wheel_ground_velocity_right_driven;
    uint8_t wheel_rotat_direct_status_left_driven;
    uint8_t wheel_rotat_direct_status_right_driven;
    bool wheel_ground_velocity_left_non_driven_invalid;
    float wheel_ground_velocity_left_non_driven;
    bool wheel_ground_velocity_right_non_driven_invalid;
    float wheel_ground_velocity_right_non_driven;
    uint8_t  wheel_rotat_direct_status_left_non_driven;
    uint8_t wheel_rotat_direct_status_right_non_driven;
    uint8_t   vehicle_move_status;
    bool  trans_estimated_gear_invalid;
    uint8_t trans_estimated_gear;
    uint8_t  auto_trans_gear_shift_direct;
    uint8_t auto_trans_command_gear;
    bool   trans_shift_lever_pos_invalid;
    uint8_t  trans_shift_lever_pos;
    uint8_t  engine_speed_status;
    float engine_speed;
    bool accelerator_actual_pos_invalid;
    float accelerator_actual_pos;
    bool throt_pos_invalid;
    float throt_pos;
    float brk_pedal_pos;
    int16_t brk_pedal_pos_gradient;
    bool steer_wheel_ang_invalid;
    uint8_t steer_wheel_ang_sensor_calib_status;
    uint8_t steer_wheel_ang_mask;
    float steer_wheel_ang;
    bool steer_wheel_ang_gradient_invalid;
    uint8_t steer_wheel_ang_gradient_mask;
    int16_t steer_wheel_ang_gradient;
};

#endif // GL8_STRUCTURES_H
