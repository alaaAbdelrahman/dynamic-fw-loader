/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
 * File Name : r_c3sap_g3_buffsize.h
 * Description : Calculation of request binary buffer size and
 *               callback structure size
 ******************************************************************************/

#ifndef R_C3SAP_G3_BUFFSIZE_H
#define R_C3SAP_G3_BUFFSIZE_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_g3ctrl_binstruct.h"
#include "r_g3mac_binstruct.h"
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#include "r_g3rfmac_binstruct.h"
#endif
#include "r_adp_binstruct.h"
#include "r_eap_binstruct.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* request binary buffer size */
typedef union
{
    r_g3_mac_init_req_bin_t       mac_init;
    r_g3_adp_init_req_bin_t       adp_init;
    r_g3_eap_init_req_bin_t       eap_init;
    r_g3_mac_set_config_req_bin_t mac_set;
    r_g3_adp_set_config_req_bin_t adp_set;
    r_g3_clear_info_req_bin_t     clear;
    r_g3_get_info_req_bin_t       get_info;
} r_g3_req_bin_t;


typedef union
{
    r_g3mac_mcps_data_req_bin_t  data;
    r_g3mac_mlme_reset_req_bin_t reset;
    r_g3mac_mlme_get_req_bin_t   get;
    r_g3mac_mlme_set_req_bin_t   set;
    r_g3mac_mlme_scan_req_bin_t  scan;
    r_g3mac_mlme_start_req_bin_t start;
} r_g3mac_req_bin_t;

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
typedef union
{
    r_g3rfmac_mcps_data_req_bin_t   data;
    r_g3rfmac_mlme_reset_req_bin_t  reset;
    r_g3rfmac_mlme_get_req_bin_t    get;
    r_g3rfmac_mlme_set_req_bin_t    set;
    r_g3rfmac_mlme_scan_req_bin_t   scan;
    r_g3rfmac_mlme_start_req_bin_t  start;
} u_g3rfmac_req_bin_t;
#endif

typedef union
{
    r_adp_adpd_data_req_bin_t       data;
    r_adp_adpm_discovery_req_bin_t  disc;
    r_adp_adpm_nwk_start_req_bin_t  nwks;
    r_adp_adpm_nwk_join_req_bin_t   nwkj;
    r_adp_adpm_get_req_bin_t        get;
    r_adp_adpm_set_req_bin_t        set;
    r_adp_adpm_route_disc_req_bin_t rout;
    r_adp_adpm_path_disc_req_bin_t  path;
    r_adp_adpm_lbp_req_bin_t        lbp;
} r_adp_req_bin_t;

typedef union
{
    r_eap_eapm_get_req_bin_t       get;
    r_eap_eapm_set_req_bin_t       set;
    r_eap_eapm_network_req_bin_t   nwk;
    r_eap_eapm_set_cinfo_req_bin_t cinfo;
} r_eap_req_bin_t;

typedef union
{
    r_g3_req_bin_t      g3;
    r_g3mac_req_bin_t   mac;
    r_adp_req_bin_t     adp;
    r_eap_req_bin_t     eap;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    u_g3rfmac_req_bin_t rfmac;
#endif
} r_g3_req_bin_size_t;


/* callback size */
typedef union
{
    r_g3_set_config_cnf_t    set_config;
    r_g3_get_config_cnf_t    get_config;
    r_g3_get_info_cnf_t      get_info;
    r_g3_clear_info_cnf_t    clear_info;
    r_g3_dump_cnf_t          dump_cnf;
    r_g3_dump_abort_cnf_t    abort;
    r_g3_dump_ind_t          dump_ind;
    r_g3_event_ind_t         event;
    r_g3_init_cnf_t          init;
    r_g3_deinit_cnf_t        deinit;
} r_g3_cb_str_t;


typedef union
{
    r_g3mac_mcps_data_cnf_t         mcps_data_cnf;
    r_g3mac_mlme_reset_cnf_t        mlme_reset_cnf;
    r_g3mac_mlme_get_cnf_t          mlme_get_cnf;
    r_g3mac_mlme_set_cnf_t          mlme_set_cnf;
    r_g3mac_mlme_scan_cnf_t         mlme_scan_cnf;
    r_g3mac_mlme_start_cnf_t        mlme_start_cnf;
    r_g3mac_mcps_data_ind_t         mcps_data_ind;
    r_g3mac_mlme_bcn_notify_ind_t   mlme_beacon_notify_ind;
    r_g3mac_mlme_comm_status_ind_t  mlme_comm_status_ind;
    r_g3mac_mlme_framecount_ind_t   mlme_frame_count_ind;
    r_g3mac_mlme_tmr_receive_ind_t  mlme_tmr_receve_ind;
    r_g3mac_mlme_tmr_transmit_ind_t mlme_tmr_transmit_ind;
} r_g3mac_cb_str_t;

typedef union
{
    r_adp_adpd_data_cnf_t           adpd_data_cnf;
    r_adp_adpm_reset_cnf_t          adpm_reset_cnf;
    r_adp_adpm_discovery_cnf_t      adpm_discovery_cnf;
    r_adp_adpm_network_start_cnf_t  adpm_network_start_cnf;
    r_adp_adpm_network_join_cnf_t   adpm_network_join_cnf;
    r_adp_adpm_network_leave_cnf_t  adpm_network_leave_cnf;
    r_adp_adpm_get_cnf_t            adpm_get_cnf;
    r_adp_adpm_set_cnf_t            adpm_set_cnf;
    r_adp_adpm_route_disc_cnf_t     adpm_route_discovery_cnf;
    r_adp_adpm_path_discovery_cnf_t adpm_path_discovery_cnf;
    r_adp_adpm_lbp_cnf_t            adpm_lbp_cnf;
    r_adp_adpd_data_ind_t           adpd_data_ind;
    r_adp_adpm_network_status_ind_t adpm_network_status_ind;
    r_adp_adpm_path_discovery_ind_t adpm_path_discovery_ind;
    r_adp_adpm_lbp_ind_t            adpm_lbp_ind;
    r_adp_adpm_buffer_ind_t         adpm_buffer_ind;
    r_adp_adpm_key_state_ind_t      adpm_key_state_ind;
    r_adp_adpm_route_error_ind_t    adpm_route_error_ind;
    r_adp_adpm_eap_key_ind_t        adpm_eap_key_ind;
    r_adp_adpm_framecounter_ind_t   adpm_frame_counter_ind;
    r_adp_adpm_route_update_ind_t   adpm_route_update_ind;
    r_adp_adpm_load_seq_num_ind_t   adpm_load_seq_num_ind;
    r_adp_adpm_rrep_ind_t           adpm_rrep_ind;
    r_adp_adpm_bcn_notify_ind_t     adpm_bcn_notify_ind;
    r_g3mac_mlme_get_cnf_t          mlme_get_cnf;
    r_g3mac_mlme_set_cnf_t          mlme_set_cnf;
    r_g3mac_mlme_tmr_receive_ind_t  mlme_tmr_receve_ind;
    r_g3mac_mlme_tmr_transmit_ind_t mlme_tmr_transmit_ind;
} r_adp_cb_str_t;

typedef union
{
    r_eap_eapm_reset_cnf_t          eapm_reset_cnf;
    r_eap_eapm_start_cnf_t          eapm_start_cnf;
    r_eap_eapm_get_cnf_t            eapm_get_cnf;
    r_eap_eapm_set_cnf_t            eapm_set_cnf;
    r_eap_eapm_network_cnf_t        eapm_network_cnf;
    r_eap_eapm_set_clientinfo_cnf_t eapm_set_client_info_cnf;
    r_eap_eapm_network_join_ind_t   eapm_network_join_ind;
    r_eap_eapm_network_leave_ind_t  eapm_network_leave_ind;
    r_eap_eapm_newdevice_ind_t      eapm_new_device_ind;
    r_eap_eapm_eap_key_ind_t        eapm_eap_key_ind;
    r_adp_adpd_data_cnf_t           adpd_data_cnf;
    r_adp_adpm_reset_cnf_t          adpm_reset_cnf;
    r_adp_adpm_discovery_cnf_t      adpm_discovery_cnf;
    r_adp_adpm_network_start_cnf_t  adpm_network_start_cnf;
    r_adp_adpm_network_join_cnf_t   adpm_network_join_cnf;
    r_adp_adpm_network_leave_cnf_t  adpm_network_leave_cnf;
    r_adp_adpm_get_cnf_t            adpm_get_cnf;
    r_adp_adpm_set_cnf_t            adpm_set_cnf;
    r_adp_adpm_route_disc_cnf_t     adpm_route_discovery_cnf;
    r_adp_adpm_path_discovery_cnf_t adpm_path_discovery_cnf;
    r_adp_adpd_data_ind_t           adpd_data_ind;
    r_adp_adpm_network_status_ind_t adpm_network_status_ind;
    r_adp_adpm_path_discovery_ind_t adpm_path_discovery_ind;
    r_adp_adpm_buffer_ind_t         adpm_buffer_ind;
    r_adp_adpm_key_state_ind_t      adpm_key_state_ind;
    r_adp_adpm_route_error_ind_t    adpm_route_error_ind;
    r_adp_adpm_eap_key_ind_t        adpm_eap_key_ind;
    r_adp_adpm_framecounter_ind_t   adpm_frame_counter_ind;
    r_adp_adpm_route_update_ind_t   adpm_route_update_ind;
    r_adp_adpm_load_seq_num_ind_t   adpm_load_seq_num_ind;
    r_adp_adpm_rrep_ind_t           adpm_rrep_ind;
    r_adp_adpm_bcn_notify_ind_t     adpm_bcn_notify_ind;
    r_g3mac_mlme_get_cnf_t          mlme_get_cnf;
    r_g3mac_mlme_set_cnf_t          mlme_set_cnf;
    r_g3mac_mlme_tmr_receive_ind_t  mlme_tmr_receve_ind;
    r_g3mac_mlme_tmr_transmit_ind_t mlme_tmr_transmit_ind;
} r_eap_cb_str_t;

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
typedef union
{
    r_g3mac_mcps_data_cnf_t             mcps_data_cnf;
    r_g3mac_mlme_reset_cnf_t            mlme_reset_cnf;
    r_g3mac_mlme_get_cnf_t              mlme_get_cnf;
    r_g3mac_mlme_set_cnf_t              mlme_set_cnf;
    r_g3mac_mlme_scan_cnf_t             mlme_scan_cnf;
    r_g3mac_mlme_start_cnf_t            mlme_start_cnf;
    r_g3mac_mcps_data_ind_t             mcps_data_ind;
    r_g3mac_mlme_bcn_notify_ind_t       mlme_beacon_notify_ind;
    r_g3mac_mlme_comm_status_ind_t      mlme_comm_status_ind;
    r_g3mac_mlme_framecount_ind_t       mlme_frame_count_ind;
    r_g3mac_mlme_tmr_receive_ind_t      mlme_tmr_receve_ind;
    r_g3mac_mlme_tmr_transmit_ind_t     mlme_tmr_transmit_ind;

    r_g3rfmac_mcps_data_cnf_t           rf_mcps_data_cnf;
    r_g3rfmac_mlme_reset_cnf_t          rf_mlme_reset_cnf;
    r_g3rfmac_mlme_get_cnf_t            rf_mlme_get_cnf;
    r_g3rfmac_mlme_set_cnf_t            rf_mlme_set_cnf;
    r_g3rfmac_mlme_scan_cnf_t           rf_mlme_scan_cnf;
    r_g3rfmac_mlme_start_cnf_t          rf_mlme_start_cnf;
    r_g3rfmac_mcps_data_ind_t           rf_mcps_data_ind;
    r_g3rfmac_mlme_bcn_notify_ind_t     rf_mlme_beacon_notify_ind;
    r_g3rfmac_mlme_comm_status_ind_t    rf_mlme_comm_status_ind;
    r_g3rfmac_mlme_framecount_ind_t     rf_mlme_frame_count_ind;
    r_g3rfmac_mlme_warning_ind_t        rf_mlme_warning_ind;
    r_rflmac_lml_data_ind_t             rf_lml_data_ind;

} r_g3hymac_cb_str_t;

typedef union
{
    r_adp_adpd_data_cnf_t               adpd_data_cnf;
    r_adp_adpm_reset_cnf_t              adpm_reset_cnf;
    r_adp_adpm_discovery_cnf_t          adpm_discovery_cnf;
    r_adp_adpm_network_start_cnf_t      adpm_network_start_cnf;
    r_adp_adpm_network_join_cnf_t       adpm_network_join_cnf;
    r_adp_adpm_network_leave_cnf_t      adpm_network_leave_cnf;
    r_adp_adpm_get_cnf_t                adpm_get_cnf;
    r_adp_adpm_set_cnf_t                adpm_set_cnf;
    r_adp_adpm_route_disc_cnf_t         adpm_route_discovery_cnf;
    r_adp_adpm_path_discovery_cnf_t     adpm_path_discovery_cnf;
    r_adp_adpm_lbp_cnf_t                adpm_lbp_cnf;
    r_adp_adpd_data_ind_t               adpd_data_ind;
    r_adp_adpm_network_status_ind_t     adpm_network_status_ind;
    r_adp_adpm_path_discovery_ind_t     adpm_path_discovery_ind;
    r_adp_adpm_lbp_ind_t                adpm_lbp_ind;
    r_adp_adpm_buffer_ind_t             adpm_buffer_ind;
    r_adp_adpm_key_state_ind_t          adpm_key_state_ind;
    r_adp_adpm_route_error_ind_t        adpm_route_error_ind;
    r_adp_adpm_eap_key_ind_t            adpm_eap_key_ind;
    r_adp_adpm_framecounter_ind_t       adpm_frame_counter_ind;
    r_adp_adpm_route_update_ind_t       adpm_route_update_ind;
    r_adp_adpm_load_seq_num_ind_t       adpm_load_seq_num_ind;
    r_adp_adpm_rrep_ind_t               adpm_rrep_ind;
    r_adp_adpm_bcn_notify_ind_t         adpm_bcn_notify_ind;
    r_g3mac_mlme_get_cnf_t              mlme_get_cnf;
    r_g3mac_mlme_set_cnf_t              mlme_set_cnf;
    r_g3mac_mlme_tmr_receive_ind_t      mlme_tmr_receve_ind;
    r_g3mac_mlme_tmr_transmit_ind_t     mlme_tmr_transmit_ind;
    r_g3rfmac_mlme_get_cnf_t            rf_mlme_get_cnf;
    r_g3rfmac_mlme_set_cnf_t            rf_mlme_set_cnf;
    r_rflmac_lml_data_ind_t             rf_lml_data_ind;
} r_hyadp_cb_str_t;

typedef union
{
    r_eap_eapm_reset_cnf_t              eapm_reset_cnf;
    r_eap_eapm_start_cnf_t              eapm_start_cnf;
    r_eap_eapm_get_cnf_t                eapm_get_cnf;
    r_eap_eapm_set_cnf_t                eapm_set_cnf;
    r_eap_eapm_network_cnf_t            eapm_network_cnf;
    r_eap_eapm_set_clientinfo_cnf_t     eapm_set_client_info_cnf;
    r_eap_eapm_network_join_ind_t       eapm_network_join_ind;
    r_eap_eapm_network_leave_ind_t      eapm_network_leave_ind;
    r_eap_eapm_newdevice_ind_t          eapm_new_device_ind;
    r_eap_eapm_eap_key_ind_t            eapm_eap_key_ind;
    r_adp_adpd_data_cnf_t               adpd_data_cnf;
    r_adp_adpm_reset_cnf_t              adpm_reset_cnf;
    r_adp_adpm_discovery_cnf_t          adpm_discovery_cnf;
    r_adp_adpm_network_start_cnf_t      adpm_network_start_cnf;
    r_adp_adpm_network_join_cnf_t       adpm_network_join_cnf;
    r_adp_adpm_network_leave_cnf_t      adpm_network_leave_cnf;
    r_adp_adpm_get_cnf_t                adpm_get_cnf;
    r_adp_adpm_set_cnf_t                adpm_set_cnf;
    r_adp_adpm_route_disc_cnf_t         adpm_route_discovery_cnf;
    r_adp_adpm_path_discovery_cnf_t     adpm_path_discovery_cnf;
    r_adp_adpd_data_ind_t               adpd_data_ind;
    r_adp_adpm_network_status_ind_t     adpm_network_status_ind;
    r_adp_adpm_path_discovery_ind_t     adpm_path_discovery_ind;
    r_adp_adpm_buffer_ind_t             adpm_buffer_ind;
    r_adp_adpm_key_state_ind_t          adpm_key_state_ind;
    r_adp_adpm_route_error_ind_t        adpm_route_error_ind;
    r_adp_adpm_eap_key_ind_t            adpm_eap_key_ind;
    r_adp_adpm_framecounter_ind_t       adpm_frame_counter_ind;
    r_adp_adpm_route_update_ind_t       adpm_route_update_ind;
    r_adp_adpm_load_seq_num_ind_t       adpm_load_seq_num_ind;
    r_adp_adpm_rrep_ind_t               adpm_rrep_ind;
    r_adp_adpm_bcn_notify_ind_t         adpm_bcn_notify_ind;
    r_g3mac_mlme_get_cnf_t              mlme_get_cnf;
    r_g3mac_mlme_set_cnf_t              mlme_set_cnf;
    r_g3mac_mlme_tmr_receive_ind_t      mlme_tmr_receve_ind;
    r_g3mac_mlme_tmr_transmit_ind_t     mlme_tmr_transmit_ind;
    r_g3rfmac_mlme_get_cnf_t            rf_mlme_get_cnf;
    r_g3rfmac_mlme_set_cnf_t            rf_mlme_set_cnf;
    r_rflmac_lml_data_ind_t             rf_lml_data_ind;
} r_hyeap_cb_str_t;

typedef union
{
    r_g3rfmac_mcps_data_cnf_t           rf_mcps_data_cnf;
    r_g3rfmac_mlme_reset_cnf_t          rf_mlme_reset_cnf;
    r_g3rfmac_mlme_get_cnf_t            rf_mlme_get_cnf;
    r_g3rfmac_mlme_set_cnf_t            rf_mlme_set_cnf;
    r_g3rfmac_mlme_scan_cnf_t           rf_mlme_scan_cnf;
    r_g3rfmac_mlme_start_cnf_t          rf_mlme_start_cnf;
    r_g3rfmac_mcps_data_ind_t           rf_mcps_data_ind;
    r_g3rfmac_mlme_bcn_notify_ind_t     rf_mlme_beacon_notify_ind;
    r_g3rfmac_mlme_comm_status_ind_t    rf_mlme_comm_status_ind;
    r_g3rfmac_mlme_framecount_ind_t     rf_mlme_frame_count_ind;
    r_g3rfmac_mlme_warning_ind_t        rf_mlme_warning_ind;
    r_rflmac_lml_data_ind_t             rf_lml_data_ind;
} r_g3rfmac_cb_str_t;

#endif

typedef union
{
    r_g3_cb_str_t       g3;
    r_g3mac_cb_str_t    mac;
    r_adp_cb_str_t      adp;
    r_eap_cb_str_t      eap;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_g3hymac_cb_str_t  hymac;
    r_hyadp_cb_str_t    hyadp;
    r_hyeap_cb_str_t    hyeap;
    r_g3rfmac_cb_str_t  rfmac;
#endif
} r_g3_cb_size_str_t;

/* calculation of request binary buffer size */
typedef struct
{
    r_g3_req_bin_size_t req;
    uint8_t             buff[R_MAX_VARIABLE_LENGTH];
} r_plc_req_buffsize_str_t;

/* calculation of callback structure size */
typedef struct
{
    r_g3_cb_size_str_t cb;
    uint8_t            buff[R_MAX_VARIABLE_LENGTH];
} r_plc_cb_buffsize_str_t;

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_C3SAP_BUFFSIZE_H */

