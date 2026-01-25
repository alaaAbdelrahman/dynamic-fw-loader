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
 * File Name : r_g3_api.h
 * Description : G3 API prototype
 ******************************************************************************/

#ifndef R_G3_API_H
#define R_G3_API_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_g3_sap.h"
#include "r_g3mac_sap.h"
#include "r_g3hybrid.h"
#include "r_g3rfmac_sap.h"
#ifndef R_DEFINE_DISABLE_APP_HYBRID
#include "r_rflmac_sap.h"
#endif
#include "r_adp_sap.h"
#include "r_eap_sap.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/*!****************************************************************************
 * @addtogroup group_cpx_api_callbacks
 * @{
 *****************************************************************************/
typedef struct
{
    void (* R_EAP_EapmResetCnf)         (const r_eap_eapm_reset_cnf_t * pcnf);          /**< Callback function for confirmation EAPM-RESET: Request of resetting EAP layer */
    void (* R_EAP_EapmStartCnf)         (const r_eap_eapm_start_cnf_t * pcnf);          /**< Callback function for confirmation EAPM-START: Request of enabling transmission/reception of LoWPAN protocol (LBP) message */
    void (* R_EAP_EapmGetCnf)           (const r_eap_eapm_get_cnf_t * pcnf);            /**< Callback function for confirmation EAPM-GET: Request of obtaining EAP IB attribute */
    void (* R_EAP_EapmSetCnf)           (const r_eap_eapm_set_cnf_t * pcnf);            /**< Callback function for confirmation EAPM-SET: Request of setting EAP IB attribute  */
    void (* R_EAP_EapmNetworkCnf)       (const r_eap_eapm_network_cnf_t * pcnf);        /**< Callback function for confirmation EAPM-NETWORK: Request of transmission request of kick processing and GMK-related processing */
    void (* R_EAP_EapmSetClientinfoCnf) (const r_eap_eapm_set_clientinfo_cnf_t * pcnf); /**< Callback function for confirmation EAPM-SETCLIENTINFO: Request of transmission request of operating client info */
    void (* R_EAP_EapmNetworkJoinInd)   (const r_eap_eapm_network_join_ind_t * pind);   /**< Callback function for indication EAPM-NETWORK-JOIN: Notification that peer participates in PAN */
    void (* R_EAP_EapmNetworkLeaveInd)  (const r_eap_eapm_network_leave_ind_t * pind);  /**< Callback function for indication EAPM-NETWORK-LEAVE: Notification that peer secedes from PAN */
    void (* R_EAP_EapmNewDeviceInd)     (const r_eap_eapm_newdevice_ind_t * pind);      /**< Callback function for indication EAPM-NEWDEVICE: Notification that: @li PAN device which is not registered in eapClientInformation sends a PAN join request @li EAPM-NETWORK.request is called for a PAN device which is not registered in eapClientTable */
    void (* R_EAP_EapmEapKeyInd)        (const r_eap_eapm_eap_key_ind_t * pind);        /**< Callback function for indication EAPM-EAP_KEY: Notification that: @li PAN device which is not registered in eapClientInformation sends a PAN join request @li EAPM-NETWORK.request is called for a PAN device which is not registered in eapClientTable */
    void (* R_ADP_AdpdDataCnf)          (const r_adp_adpd_data_cnf_t * pcnf);           /**< Callback function for confirmation ADPD-DATA: Request of data transmission */
    void (* R_ADP_AdpmResetCnf)         (const r_adp_adpm_reset_cnf_t * pcnf);          /**< Callback function for confirmation ADPM-RESET: Request of resetting ADP layer and the lower layer */
    void (* R_ADP_AdpmDiscoveryCnf)     (const r_adp_adpm_discovery_cnf_t * pcnf);      /**< Callback function for confirmation ADPM-DISCOVERY: Request of searching PAN */
    void (* R_ADP_AdpmNetworkStartCnf)  (const r_adp_adpm_network_start_cnf_t * pcnf);  /**< Callback function for confirmation ADPM-NETWORK-START: Request of starting PAN */
    void (* R_ADP_AdpmNetworkJoinCnf)   (const r_adp_adpm_network_join_cnf_t * pcnf);   /**< Callback function for confirmation ADPM-NETWORK-JOIN: Request of participating in a PAN */
    void (* R_ADP_AdpmNetworkLeaveCnf)  (const r_adp_adpm_network_leave_cnf_t * pcnf);  /**< Callback function for confirmation ADPM-NETWORK-LEAVE: Request of seceding from PAN */
    void (* R_ADP_AdpmGetCnf)           (const r_adp_adpm_get_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-GET: Request of obtaining ADP IB attribute */
    void (* R_ADP_AdpmSetCnf)           (const r_adp_adpm_set_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-SET: Request of setting ADP IB attribute */
    void (* R_ADP_AdpmRouteDiscoveryCnf)(const r_adp_adpm_route_disc_cnf_t * pcnf);     /**< Callback function for confirmation ADPM-ROUTE-DISCOVERY: Request of searching route */
    void (* R_ADP_AdpmPathDiscoveryCnf) (const r_adp_adpm_path_discovery_cnf_t * pcnf); /**< Callback function for confirmation ADPM-PATH-DISCOVERY: Request of searching network path */
    void (* R_ADP_AdpdDataInd)          (const r_adp_adpd_data_ind_t * pind);           /**< Callback function for indication ADPD-DATA: Notification that data was received */
    void (* R_ADP_AdpmNetworkLeaveInd)  (void);                                         /**< Callback function for indication ADPM-NETWORK-LEAVE: Notification that CPX left the PAN */
    void (* R_ADP_AdpmPathDiscoveryInd) (const r_adp_adpm_path_discovery_ind_t * pind); /**< Callback function for indication ADPM-PATH-DISCOVERY: Notification of reception of the path discovery request message to oneself */
    void (* R_ADP_AdpmNetworkStatusInd) (const r_adp_adpm_network_status_ind_t * pind); /**< Callback function for indication ADPM-NETWORK-STATUS: Notification of reception of MLME-COMM-STATUS.indication */
    void (* R_ADP_AdpmBufferInd)        (const r_adp_adpm_buffer_ind_t * pind);         /**< Callback function for indication ADPM-BUFFER: Notification of the state change of the internal data transmission buffer */
    void (* R_ADP_AdpmKeyStateInd)      (const r_adp_adpm_key_state_ind_t * pind);      /**< Callback function for indication ADPM-KEY-STATE: Notification that GMK-related processing request is granted */
    void (* R_ADP_AdpmRouteErrorInd)    (const r_adp_adpm_route_error_ind_t * pind);    /**< Callback function for indication ADPM-ROUTE-ERROR: Notification of reception route error packet */
    void (* R_ADP_AdpmEapKeyInd)        (const r_adp_adpm_eap_key_ind_t * pind);        /**< Callback function for indication ADPM-EAP-KEY: Notification of a key generated in the process of EAP-PSK  */
    void (* R_ADP_AdpmFrameCounterInd)  (const r_adp_adpm_framecounter_ind_t * pind);   /**< Callback function for indication ADPM-FRAME-COUNTER: Notification of reception of the MLME-FRAMECOUNT.indication */
    void (* R_ADP_AdpmRouteUpdateInd)   (const r_adp_adpm_route_update_ind_t * pind);   /**< Callback function for indication ADPM-ROUTE-UPDATE: Notification of route update */
    void (* R_ADP_AdpmLoadSeqNumInd)    (const r_adp_adpm_load_seq_num_ind_t * pind);
    void (* R_ADP_AdpmRrepInd)          (const r_adp_adpm_rrep_ind_t * pind);           /**< Callback function for indication ADPM-RREP: Notification of route update */
    void (* R_ADP_AdpmBeaconNotifyInd)  (const r_adp_adpm_bcn_notify_ind_t * pind);     /**< Callback function for indication ADPM-BEACON-NOTIFY: Notification of receiving beacon response.   */
    void (* R_G3MAC_MlmeGetCnf)         (const r_g3mac_mlme_get_cnf_t * pcnf);          /**< Callback function for confirmation MLME-GET: Request of obtaining MAC PIB attribute */
    void (* R_G3MAC_MlmeSetCnf)         (const r_g3mac_mlme_set_cnf_t * pcnf);          /**< Callback function for confirmation MLME-SET: Request of setting MAC PIB attribute */
    void (* R_G3MAC_MlmeTmrReceiveInd)  (const r_g3mac_mlme_tmr_receive_ind_t * pind);
    void (* R_G3MAC_MlmeTmrTransmitInd) (const r_g3mac_mlme_tmr_transmit_ind_t * pind);
} r_g3_eap_callback_t;

typedef struct
{
    void (* R_ADP_AdpdDataCnf)          (const r_adp_adpd_data_cnf_t * pcnf);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpdDataCnf */
    void (* R_ADP_AdpmResetCnf)         (const r_adp_adpm_reset_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmResetCnf */
    void (* R_ADP_AdpmDiscoveryCnf)     (const r_adp_adpm_discovery_cnf_t * pcnf);      /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmDiscoveryCnf */
    void (* R_ADP_AdpmNetworkStartCnf)  (const r_adp_adpm_network_start_cnf_t * pcnf);  /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkStartCnf */
    void (* R_ADP_AdpmNetworkJoinCnf)   (const r_adp_adpm_network_join_cnf_t * pcnf);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkJoinCnf */
    void (* R_ADP_AdpmNetworkLeaveCnf)  (const r_adp_adpm_network_leave_cnf_t * pcnf);  /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkLeaveCnf */
    void (* R_ADP_AdpmGetCnf)           (const r_adp_adpm_get_cnf_t * pcnf);            /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmGetCnf */
    void (* R_ADP_AdpmSetCnf)           (const r_adp_adpm_set_cnf_t * pcnf);            /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmSetCnf */
    void (* R_ADP_AdpmRouteDiscoveryCnf)(const r_adp_adpm_route_disc_cnf_t * pcnf);     /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteDiscoveryCnf */
    void (* R_ADP_AdpmPathDiscoveryCnf) (const r_adp_adpm_path_discovery_cnf_t * pcnf); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmPathDiscoveryCnf */
    void (* R_ADP_AdpmLbpCnf)           (const r_adp_adpm_lbp_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-LBP: Request of transmitting LoWPAN protocol (LBP) message */
    void (* R_ADP_AdpdDataInd)          (const r_adp_adpd_data_ind_t * pind);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpdDataInd */
    void (* R_ADP_AdpmNetworkLeaveInd)  (void);                                         /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkLeaveInd */
    void (* R_ADP_AdpmPathDiscoveryInd) (const r_adp_adpm_path_discovery_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmPathDiscoveryInd */
    void (* R_ADP_AdpmLbpInd)           (const r_adp_adpm_lbp_ind_t * pind);            /**< Callback function for indication ADPM-LBP: Notification of reception of LoWPAN protocol (LBP) message */
    void (* R_ADP_AdpmNetworkStatusInd) (const r_adp_adpm_network_status_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkStatusInd */
    void (* R_ADP_AdpmBufferInd)        (const r_adp_adpm_buffer_ind_t * pind);         /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmBufferInd */
    void (* R_ADP_AdpmKeyStateInd)      (const r_adp_adpm_key_state_ind_t * pind);      /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmKeyStateInd */
    void (* R_ADP_AdpmRouteErrorInd)    (const r_adp_adpm_route_error_ind_t * pind);    /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteErrorInd */
    void (* R_ADP_AdpmEapKeyInd)        (const r_adp_adpm_eap_key_ind_t * pind);        /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmEapKeyInd */
    void (* R_ADP_AdpmFrameCounterInd)  (const r_adp_adpm_framecounter_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmFrameCounterInd */
    void (* R_ADP_AdpmRouteUpdateInd)   (const r_adp_adpm_route_update_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteUpdateInd */
    void (* R_ADP_AdpmLoadSeqNumInd)    (const r_adp_adpm_load_seq_num_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmLoadSeqNumInd */
    void (* R_ADP_AdpmRrepInd)          (const r_adp_adpm_rrep_ind_t * pind);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRrepInd */
    void (* R_ADP_AdpmBeaconNotifyInd)  (const r_adp_adpm_bcn_notify_ind_t * pind);     /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmBeaconNotifyInd */
    void (* R_G3MAC_MlmeGetCnf)         (const r_g3mac_mlme_get_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeGetCnf */
    void (* R_G3MAC_MlmeSetCnf)         (const r_g3mac_mlme_set_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeSetCnf */
    void (* R_G3MAC_MlmeTmrReceiveInd)  (const r_g3mac_mlme_tmr_receive_ind_t * pind);  /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeTmrReceiveInd */
    void (* R_G3MAC_MlmeTmrTransmitInd) (const r_g3mac_mlme_tmr_transmit_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeTmrTransmitInd */
} r_g3_adp_callback_t;

typedef struct
{
    void (* R_G3MAC_McpsDataCnf)        (const r_g3mac_mcps_data_cnf_t * pcnf);         /**< Callback function for confirmation MCPS-DATA: Request of data transmission */
    void (* R_G3MAC_MlmeResetCnf)       (const r_g3mac_mlme_reset_cnf_t * pcnf);        /**< Callback function for confirmation MLME-RESET: Request of resetting UMAC layer and the lower layer  */
    void (* R_G3MAC_MlmeGetCnf)         (const r_g3mac_mlme_get_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeGetCnf */
    void (* R_G3MAC_MlmeSetCnf)         (const r_g3mac_mlme_set_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeSetCnf */
    void (* R_G3MAC_MlmeScanCnf)        (const r_g3mac_mlme_scan_cnf_t * pcnf);         /**< Callback function for confirmation MLME-SCAN: Request of searching PAN */
    void (* R_G3MAC_MlmeStartCnf)       (const r_g3mac_mlme_start_cnf_t * pcnf);        /**< Callback function for confirmation MLME-START: Request of constructing PAN */
    void (* R_G3MAC_McpsDataInd)        (const r_g3mac_mcps_data_ind_t * pind);         /**< Callback function for indication MCPS-DATA: Notification of data reception */
    void (* R_G3MAC_MlmeBeaconNotifyInd)(const r_g3mac_mlme_bcn_notify_ind_t * pind);   /**< Callback function for indication MLME-BEACON-NOTIFY: Notification of receiving beacon response.  */
    void (* R_G3MAC_MlmeCommStatusInd)  (const r_g3mac_mlme_comm_status_ind_t * pind);  /**< Callback function for indication MLME-COMM-STATUS: Notification of communication status */
    void (* R_G3MAC_MlmeFrameCountInd)  (const r_g3mac_mlme_framecount_ind_t * pind);   /**< Callback function for indication MLME-FRAMECOUNT: Notification of frame counter  */
    void (* R_G3MAC_MlmeTmrReceiveInd)  (const r_g3mac_mlme_tmr_receive_ind_t * pind);  /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeTmrReceiveInd */
    void (* R_G3MAC_MlmeTmrTransmitInd) (const r_g3mac_mlme_tmr_transmit_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeTmrTransmitInd */
} r_g3_mac_callback_t;


#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
typedef struct
{
    void (* R_EAP_EapmResetCnf)         (const r_eap_eapm_reset_cnf_t * pcnf);          /**< Callback function for confirmation EAPM-RESET: Request of resetting EAP layer */
    void (* R_EAP_EapmStartCnf)         (const r_eap_eapm_start_cnf_t * pcnf);          /**< Callback function for confirmation EAPM-START: Request of enabling transmission/reception of LoWPAN protocol (LBP) message */
    void (* R_EAP_EapmGetCnf)           (const r_eap_eapm_get_cnf_t * pcnf);            /**< Callback function for confirmation EAPM-GET: Request of obtaining EAP IB attribute */
    void (* R_EAP_EapmSetCnf)           (const r_eap_eapm_set_cnf_t * pcnf);            /**< Callback function for confirmation EAPM-SET: Request of setting EAP IB attribute  */
    void (* R_EAP_EapmNetworkCnf)       (const r_eap_eapm_network_cnf_t * pcnf);        /**< Callback function for confirmation EAPM-NETWORK: Request of transmission request of kick processing and GMK-related processing */
    void (* R_EAP_EapmSetClientinfoCnf) (const r_eap_eapm_set_clientinfo_cnf_t * pcnf); /**< Callback function for confirmation EAPM-SETCLIENTINFO: Request of transmission request of operating client info */
    void (* R_EAP_EapmNetworkJoinInd)   (const r_eap_eapm_network_join_ind_t * pind);   /**< Callback function for indication EAPM-NETWORK-JOIN: Notification that peer participates in PAN */
    void (* R_EAP_EapmNetworkLeaveInd)  (const r_eap_eapm_network_leave_ind_t * pind);  /**< Callback function for indication EAPM-NETWORK-LEAVE: Notification that peer secedes from PAN */
    void (* R_EAP_EapmNewDeviceInd)     (const r_eap_eapm_newdevice_ind_t * pind);      /**< Callback function for indication EAPM-NEWDEVICE: Notification that: @li PAN device which is not registered in eapClientInformation sends a PAN join request @li EAPM-NETWORK.request is called for a PAN device which is not registered in eapClientTable */
    void (* R_EAP_EapmEapKeyInd)        (const r_eap_eapm_eap_key_ind_t * pind);        /**< Callback function for indication EAPM-EAP_KEY: Notification that: @li PAN device which is not registered in eapClientInformation sends a PAN join request @li EAPM-NETWORK.request is called for a PAN device which is not registered in eapClientTable */
    void (* R_ADP_AdpdDataCnf)          (const r_adp_adpd_data_cnf_t * pcnf);           /**< Callback function for confirmation ADPD-DATA: Request of data transmission */
    void (* R_ADP_AdpmResetCnf)         (const r_adp_adpm_reset_cnf_t * pcnf);          /**< Callback function for confirmation ADPM-RESET: Request of resetting ADP layer and the lower layer */
    void (* R_ADP_AdpmDiscoveryCnf)     (const r_adp_adpm_discovery_cnf_t * pcnf);      /**< Callback function for confirmation ADPM-DISCOVERY: Request of searching PAN */
    void (* R_ADP_AdpmNetworkStartCnf)  (const r_adp_adpm_network_start_cnf_t * pcnf);  /**< Callback function for confirmation ADPM-NETWORK-START: Request of starting PAN */
    void (* R_ADP_AdpmNetworkJoinCnf)   (const r_adp_adpm_network_join_cnf_t * pcnf);   /**< Callback function for confirmation ADPM-NETWORK-JOIN: Request of participating in a PAN */
    void (* R_ADP_AdpmNetworkLeaveCnf)  (const r_adp_adpm_network_leave_cnf_t * pcnf);  /**< Callback function for confirmation ADPM-NETWORK-LEAVE: Request of seceding from PAN */
    void (* R_ADP_AdpmGetCnf)           (const r_adp_adpm_get_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-GET: Request of obtaining ADP IB attribute */
    void (* R_ADP_AdpmSetCnf)           (const r_adp_adpm_set_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-SET: Request of setting ADP IB attribute */
    void (* R_ADP_AdpmRouteDiscoveryCnf)(const r_adp_adpm_route_disc_cnf_t * pcnf);     /**< Callback function for confirmation ADPM-ROUTE-DISCOVERY: Request of searching route */
    void (* R_ADP_AdpmPathDiscoveryCnf) (const r_adp_adpm_path_discovery_cnf_t * pcnf); /**< Callback function for confirmation ADPM-PATH-DISCOVERY: Request of searching network path */
    void (* R_ADP_AdpdDataInd)          (const r_adp_adpd_data_ind_t * pind);           /**< Callback function for indication ADPD-DATA: Notification that data was received */
    void (* R_ADP_AdpmNetworkLeaveInd)  (void);                                         /**< Callback function for indication ADPM-NETWORK-LEAVE: Notification that CPX left the PAN */
    void (* R_ADP_AdpmPathDiscoveryInd) (const r_adp_adpm_path_discovery_ind_t * pind); /**< Callback function for indication ADPM-PATH-DISCOVERY: Notification of reception of the path discovery request message to oneself */
    void (* R_ADP_AdpmNetworkStatusInd) (const r_adp_adpm_network_status_ind_t * pind); /**< Callback function for indication ADPM-NETWORK-STATUS: Notification of reception of MLME-COMM-STATUS.indication */
    void (* R_ADP_AdpmBufferInd)        (const r_adp_adpm_buffer_ind_t * pind);         /**< Callback function for indication ADPM-BUFFER: Notification of the state change of the internal data transmission buffer */
    void (* R_ADP_AdpmKeyStateInd)      (const r_adp_adpm_key_state_ind_t * pind);      /**< Callback function for indication ADPM-KEY-STATE: Notification that GMK-related processing request is granted */
    void (* R_ADP_AdpmRouteErrorInd)    (const r_adp_adpm_route_error_ind_t * pind);    /**< Callback function for indication ADPM-ROUTE-ERROR: Notification of reception route error packet */
    void (* R_ADP_AdpmEapKeyInd)        (const r_adp_adpm_eap_key_ind_t * pind);        /**< Callback function for indication ADPM-EAP-KEY: Notification of a key generated in the process of EAP-PSK  */
    void (* R_ADP_AdpmFrameCounterInd)  (const r_adp_adpm_framecounter_ind_t * pind);   /**< Callback function for indication ADPM-FRAME-COUNTER: Notification of reception of the MLME-FRAMECOUNT.indication */
    void (* R_ADP_AdpmRouteUpdateInd)   (const r_adp_adpm_route_update_ind_t * pind);   /**< Callback function for indication ADPM-ROUTE-UPDATE: Notification of route update */
    void (* R_ADP_AdpmLoadSeqNumInd)    (const r_adp_adpm_load_seq_num_ind_t * pind);   
    void (* R_ADP_AdpmRrepInd)          (const r_adp_adpm_rrep_ind_t * pind);           /**< Callback function for indication ADPM-RREP: Notification of route update */
    void (* R_ADP_AdpmBeaconNotifyInd)  (const r_adp_adpm_bcn_notify_ind_t * pind);     /**< Callback function for indication ADPM-BEACON-NOTIFY: Notification of receiving beacon response.   */
    void (* R_G3MAC_MlmeGetCnf)         (const r_g3mac_mlme_get_cnf_t * pcnf);          /**< Callback function for confirmation MLME-GET: Request of obtaining MAC PIB attribute */
    void (* R_G3MAC_MlmeSetCnf)         (const r_g3mac_mlme_set_cnf_t * pcnf);          /**< Callback function for confirmation MLME-SET: Request of setting MAC PIB attribute */
    void (* R_G3MAC_MlmeTmrReceiveInd)  (const r_g3mac_mlme_tmr_receive_ind_t * pind);
    void (* R_G3MAC_MlmeTmrTransmitInd) (const r_g3mac_mlme_tmr_transmit_ind_t * pind);
    void (* R_G3RFMAC_MlmeGetCnf)       (const r_g3rfmac_mlme_get_cnf_t * pcnf);        /**< Callback function for confirmation MLME-GET: Request of obtaining MAC PIB attribute */
    void (* R_G3RFMAC_MlmeSetCnf)       (const r_g3rfmac_mlme_set_cnf_t * pcnf);        /**< Callback function for confirmation MLME-SET: Request of setting MAC PIB attribute */
    void (* R_RFLMAC_LmlDataInd)        (const r_rflmac_lml_data_ind_t * pind);         /**< Callback function for indication LML-DATA: Notification of data reception */
} r_g3_hyeap_callback_t;

typedef struct
{
    void (* R_ADP_AdpdDataCnf)          (const r_adp_adpd_data_cnf_t * pcnf);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpdDataCnf */
    void (* R_ADP_AdpmResetCnf)         (const r_adp_adpm_reset_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmResetCnf */
    void (* R_ADP_AdpmDiscoveryCnf)     (const r_adp_adpm_discovery_cnf_t * pcnf);      /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmDiscoveryCnf */
    void (* R_ADP_AdpmNetworkStartCnf)  (const r_adp_adpm_network_start_cnf_t * pcnf);  /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkStartCnf */
    void (* R_ADP_AdpmNetworkJoinCnf)   (const r_adp_adpm_network_join_cnf_t * pcnf);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkJoinCnf */
    void (* R_ADP_AdpmNetworkLeaveCnf)  (const r_adp_adpm_network_leave_cnf_t * pcnf);  /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkLeaveCnf */
    void (* R_ADP_AdpmGetCnf)           (const r_adp_adpm_get_cnf_t * pcnf);            /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmGetCnf */
    void (* R_ADP_AdpmSetCnf)           (const r_adp_adpm_set_cnf_t * pcnf);            /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmSetCnf */
    void (* R_ADP_AdpmRouteDiscoveryCnf)(const r_adp_adpm_route_disc_cnf_t * pcnf);     /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteDiscoveryCnf */
    void (* R_ADP_AdpmPathDiscoveryCnf) (const r_adp_adpm_path_discovery_cnf_t * pcnf); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmPathDiscoveryCnf */
    void (* R_ADP_AdpmLbpCnf)           (const r_adp_adpm_lbp_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-LBP: Request of transmitting LoWPAN protocol (LBP) message */
    void (* R_ADP_AdpdDataInd)          (const r_adp_adpd_data_ind_t * pind);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpdDataInd */
    void (* R_ADP_AdpmNetworkLeaveInd)  (void);                                         /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkLeaveInd */
    void (* R_ADP_AdpmPathDiscoveryInd) (const r_adp_adpm_path_discovery_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmPathDiscoveryInd */
    void (* R_ADP_AdpmLbpInd)           (const r_adp_adpm_lbp_ind_t * pind);            /**< Callback function for indication ADPM-LBP: Notification of reception of LoWPAN protocol (LBP) message */
    void (* R_ADP_AdpmNetworkStatusInd) (const r_adp_adpm_network_status_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkStatusInd */
    void (* R_ADP_AdpmBufferInd)        (const r_adp_adpm_buffer_ind_t * pind);         /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmBufferInd */
    void (* R_ADP_AdpmKeyStateInd)      (const r_adp_adpm_key_state_ind_t * pind);      /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmKeyStateInd */
    void (* R_ADP_AdpmRouteErrorInd)    (const r_adp_adpm_route_error_ind_t * pind);    /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteErrorInd */
    void (* R_ADP_AdpmEapKeyInd)        (const r_adp_adpm_eap_key_ind_t * pind);        /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmEapKeyInd */
    void (* R_ADP_AdpmFrameCounterInd)  (const r_adp_adpm_framecounter_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmFrameCounterInd */
    void (* R_ADP_AdpmRouteUpdateInd)   (const r_adp_adpm_route_update_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteUpdateInd */
    void (* R_ADP_AdpmLoadSeqNumInd)    (const r_adp_adpm_load_seq_num_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmLoadSeqNumInd */
    void (* R_ADP_AdpmRrepInd)          (const r_adp_adpm_rrep_ind_t * pind);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRrepInd */
    void (* R_ADP_AdpmBeaconNotifyInd)  (const r_adp_adpm_bcn_notify_ind_t * pind);     /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmBeaconNotifyInd */
    void (* R_G3MAC_MlmeGetCnf)         (const r_g3mac_mlme_get_cnf_t * pcnf);          /**< Callback function for confirmation MLME-GET: Request of obtaining MAC PIB attribute */
    void (* R_G3MAC_MlmeSetCnf)         (const r_g3mac_mlme_set_cnf_t * pcnf);          /**< Callback function for confirmation MLME-SET: Request of setting MAC PIB attribute */
    void (* R_G3MAC_MlmeTmrReceiveInd)  (const r_g3mac_mlme_tmr_receive_ind_t * pind);
    void (* R_G3MAC_MlmeTmrTransmitInd) (const r_g3mac_mlme_tmr_transmit_ind_t * pind);
    void (* R_G3RFMAC_MlmeGetCnf)       (const r_g3rfmac_mlme_get_cnf_t * pcnf);        /**< Callback function for confirmation MLME-GET: Request of obtaining MAC PIB attribute */
    void (* R_G3RFMAC_MlmeSetCnf)       (const r_g3rfmac_mlme_set_cnf_t * pcnf);        /**< Callback function for confirmation MLME-SET: Request of setting MAC PIB attribute */
    void (* R_RFLMAC_LmlDataInd)        (const r_rflmac_lml_data_ind_t * pind);         /**< Callback function for indication LML-DATA: Notification of data reception */
} r_g3_hyadp_callback_t;

typedef struct
{
    void (* R_G3MAC_McpsDataCnf)          (const r_g3mac_mcps_data_cnf_t * pcnf);             /**< Callback function for confirmation MCPS-DATA: Request of data transmission */
    void (* R_G3MAC_MlmeResetCnf)         (const r_g3mac_mlme_reset_cnf_t * pcnf);            /**< Callback function for confirmation MLME-RESET: Request of resetting UMAC layer and the lower layer  */
    void (* R_G3MAC_MlmeGetCnf)           (const r_g3mac_mlme_get_cnf_t * pcnf);              /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeGetCnf */
    void (* R_G3MAC_MlmeSetCnf)           (const r_g3mac_mlme_set_cnf_t * pcnf);              /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeSetCnf */
    void (* R_G3MAC_MlmeScanCnf)          (const r_g3mac_mlme_scan_cnf_t * pcnf);             /**< Callback function for confirmation MLME-SCAN: Request of searching PAN */
    void (* R_G3MAC_MlmeStartCnf)         (const r_g3mac_mlme_start_cnf_t * pcnf);            /**< Callback function for confirmation MLME-START: Request of constructing PAN */
    void (* R_G3MAC_McpsDataInd)          (const r_g3mac_mcps_data_ind_t * pind);             /**< Callback function for indication MCPS-DATA: Notification of data reception */
    void (* R_G3MAC_MlmeBeaconNotifyInd)  (const r_g3mac_mlme_bcn_notify_ind_t * pind);       /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeBeaconNotifyInd */
    void (* R_G3MAC_MlmeCommStatusInd)    (const r_g3mac_mlme_comm_status_ind_t * pind);      /**< Callback function for indication MLME-COMM-STATUS: Notification of communication status */
    void (* R_G3MAC_MlmeFrameCountInd)    (const r_g3mac_mlme_framecount_ind_t * pind);       /**< Callback function for indication MLME-FRAMECOUNT: Notification of frame counter  */
    void (* R_G3MAC_MlmeTmrReceiveInd)    (const r_g3mac_mlme_tmr_receive_ind_t * pind);      /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeTmrReceiveInd */
    void (* R_G3MAC_MlmeTmrTransmitInd)   (const r_g3mac_mlme_tmr_transmit_ind_t * pind);     /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeTmrTransmitInd */
    void (* R_G3RFMAC_McpsDataCnf)        (const r_g3rfmac_mcps_data_cnf_t * pcnf);         /**< Callback function for confirmation MCPS-DATA: Request of data transmission */
    void (* R_G3RFMAC_MlmeResetCnf)       (const r_g3rfmac_mlme_reset_cnf_t * pcnf);        /**< Callback function for confirmation MLME-RESET: Request of resetting UMAC layer and the lower layer  */
    void (* R_G3RFMAC_MlmeGetCnf)         (const r_g3rfmac_mlme_get_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeGetCnf */
    void (* R_G3RFMAC_MlmeSetCnf)         (const r_g3rfmac_mlme_set_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeSetCnf */
    void (* R_G3RFMAC_MlmeScanCnf)        (const r_g3rfmac_mlme_scan_cnf_t * pcnf);         /**< Callback function for confirmation MLME-SCAN: Request of searching PAN */
    void (* R_G3RFMAC_MlmeStartCnf)       (const r_g3rfmac_mlme_start_cnf_t * pcnf);        /**< Callback function for confirmation MLME-START: Request of constructing PAN */
    void (* R_G3RFMAC_McpsDataInd)        (const r_g3rfmac_mcps_data_ind_t * pind);         /**< Callback function for indication MCPS-DATA: Notification of data reception */
    void (* R_G3RFMAC_MlmeBeaconNotifyInd)(const r_g3rfmac_mlme_bcn_notify_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeBeaconNotifyInd */
    void (* R_G3RFMAC_MlmeCommStatusInd)  (const r_g3rfmac_mlme_comm_status_ind_t * pind);  /**< Callback function for indication MLME-COMM-STATUS: Notification of communication status */
    void (* R_G3RFMAC_MlmeFrameCountInd)  (const r_g3rfmac_mlme_framecount_ind_t * pind);   /**< Callback function for indication MLME-FRAMECOUNT: Notification of frame counter  */
    void (* R_G3RFMAC_MlmeWarningInd)     (const r_g3rfmac_mlme_warning_ind_t * pind);      /**< Callback function for indication MLME-WARNING: Notification of warning  */
    void (* R_RFLMAC_LmlDataInd)          (const r_rflmac_lml_data_ind_t * pind);         /**< Callback function for indication LML-DATA: Notification of data reception */
} r_g3_hymac_callback_t;

typedef struct
{
    void (* R_EAP_EapmResetCnf)         (const r_eap_eapm_reset_cnf_t * pcnf);          /**< Callback function for confirmation EAPM-RESET: Request of resetting EAP layer */
    void (* R_EAP_EapmStartCnf)         (const r_eap_eapm_start_cnf_t * pcnf);          /**< Callback function for confirmation EAPM-START: Request of enabling transmission/reception of LoWPAN protocol (LBP) message */
    void (* R_EAP_EapmGetCnf)           (const r_eap_eapm_get_cnf_t * pcnf);            /**< Callback function for confirmation EAPM-GET: Request of obtaining EAP IB attribute */
    void (* R_EAP_EapmSetCnf)           (const r_eap_eapm_set_cnf_t * pcnf);            /**< Callback function for confirmation EAPM-SET: Request of setting EAP IB attribute  */
    void (* R_EAP_EapmNetworkCnf)       (const r_eap_eapm_network_cnf_t * pcnf);        /**< Callback function for confirmation EAPM-NETWORK: Request of transmission request of kick processing and GMK-related processing */
    void (* R_EAP_EapmSetClientinfoCnf) (const r_eap_eapm_set_clientinfo_cnf_t * pcnf); /**< Callback function for confirmation EAPM-SETCLIENTINFO: Request of transmission request of operating client info */
    void (* R_EAP_EapmNetworkJoinInd)   (const r_eap_eapm_network_join_ind_t * pind);   /**< Callback function for indication EAPM-NETWORK-JOIN: Notification that peer participates in PAN */
    void (* R_EAP_EapmNetworkLeaveInd)  (const r_eap_eapm_network_leave_ind_t * pind);  /**< Callback function for indication EAPM-NETWORK-LEAVE: Notification that peer secedes from PAN */
    void (* R_EAP_EapmNewDeviceInd)     (const r_eap_eapm_newdevice_ind_t * pind);      /**< Callback function for indication EAPM-NEWDEVICE: Notification that: @li PAN device which is not registered in eapClientInformation sends a PAN join request @li EAPM-NETWORK.request is called for a PAN device which is not registered in eapClientTable */
    void (* R_EAP_EapmEapKeyInd)        (const r_eap_eapm_eap_key_ind_t * pind);        /**< Callback function for indication EAPM-EAP_KEY: Notification that: @li PAN device which is not registered in eapClientInformation sends a PAN join request @li EAPM-NETWORK.request is called for a PAN device which is not registered in eapClientTable */
    void (* R_ADP_AdpdDataCnf)          (const r_adp_adpd_data_cnf_t * pcnf);           /**< Callback function for confirmation ADPD-DATA: Request of data transmission */
    void (* R_ADP_AdpmResetCnf)         (const r_adp_adpm_reset_cnf_t * pcnf);          /**< Callback function for confirmation ADPM-RESET: Request of resetting ADP layer and the lower layer */
    void (* R_ADP_AdpmDiscoveryCnf)     (const r_adp_adpm_discovery_cnf_t * pcnf);      /**< Callback function for confirmation ADPM-DISCOVERY: Request of searching PAN */
    void (* R_ADP_AdpmNetworkStartCnf)  (const r_adp_adpm_network_start_cnf_t * pcnf);  /**< Callback function for confirmation ADPM-NETWORK-START: Request of starting PAN */
    void (* R_ADP_AdpmNetworkJoinCnf)   (const r_adp_adpm_network_join_cnf_t * pcnf);   /**< Callback function for confirmation ADPM-NETWORK-JOIN: Request of participating in a PAN */
    void (* R_ADP_AdpmNetworkLeaveCnf)  (const r_adp_adpm_network_leave_cnf_t * pcnf);  /**< Callback function for confirmation ADPM-NETWORK-LEAVE: Request of seceding from PAN */
    void (* R_ADP_AdpmGetCnf)           (const r_adp_adpm_get_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-GET: Request of obtaining ADP IB attribute */
    void (* R_ADP_AdpmSetCnf)           (const r_adp_adpm_set_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-SET: Request of setting ADP IB attribute */
    void (* R_ADP_AdpmRouteDiscoveryCnf)(const r_adp_adpm_route_disc_cnf_t * pcnf);     /**< Callback function for confirmation ADPM-ROUTE-DISCOVERY: Request of searching route */
    void (* R_ADP_AdpmPathDiscoveryCnf) (const r_adp_adpm_path_discovery_cnf_t * pcnf); /**< Callback function for confirmation ADPM-PATH-DISCOVERY: Request of searching network path */
    void (* R_ADP_AdpdDataInd)          (const r_adp_adpd_data_ind_t * pind);           /**< Callback function for indication ADPD-DATA: Notification that data was received */
    void (* R_ADP_AdpmNetworkLeaveInd)  (void);                                         /**< Callback function for indication ADPM-NETWORK-LEAVE: Notification that CPX left the PAN */
    void (* R_ADP_AdpmPathDiscoveryInd) (const r_adp_adpm_path_discovery_ind_t * pind); /**< Callback function for indication ADPM-PATH-DISCOVERY: Notification of reception of the path discovery request message to oneself */
    void (* R_ADP_AdpmNetworkStatusInd) (const r_adp_adpm_network_status_ind_t * pind); /**< Callback function for indication ADPM-NETWORK-STATUS: Notification of reception of MLME-COMM-STATUS.indication */
    void (* R_ADP_AdpmBufferInd)        (const r_adp_adpm_buffer_ind_t * pind);         /**< Callback function for indication ADPM-BUFFER: Notification of the state change of the internal data transmission buffer */
    void (* R_ADP_AdpmKeyStateInd)      (const r_adp_adpm_key_state_ind_t * pind);      /**< Callback function for indication ADPM-KEY-STATE: Notification that GMK-related processing request is granted */
    void (* R_ADP_AdpmRouteErrorInd)    (const r_adp_adpm_route_error_ind_t * pind);    /**< Callback function for indication ADPM-ROUTE-ERROR: Notification of reception route error packet */
    void (* R_ADP_AdpmEapKeyInd)        (const r_adp_adpm_eap_key_ind_t * pind);        /**< Callback function for indication ADPM-EAP-KEY: Notification of a key generated in the process of EAP-PSK  */
    void (* R_ADP_AdpmFrameCounterInd)  (const r_adp_adpm_framecounter_ind_t * pind);   /**< Callback function for indication ADPM-FRAME-COUNTER: Notification of reception of the MLME-FRAMECOUNT.indication */
    void (* R_ADP_AdpmRouteUpdateInd)   (const r_adp_adpm_route_update_ind_t * pind);   /**< Callback function for indication ADPM-ROUTE-UPDATE: Notification of route update */
    void (* R_ADP_AdpmLoadSeqNumInd)    (const r_adp_adpm_load_seq_num_ind_t * pind);
    void (* R_ADP_AdpmRrepInd)          (const r_adp_adpm_rrep_ind_t * pind);           /**< Callback function for indication ADPM-RREP: Notification of route update */
    void (* R_ADP_AdpmBeaconNotifyInd)  (const r_adp_adpm_bcn_notify_ind_t * pind);     /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmBeaconNotifyInd */
    void *reserved[4];
    void (* R_G3RFMAC_MlmeGetCnf)       (const r_g3rfmac_mlme_get_cnf_t * pcnf);        /**< Callback function for confirmation MLME-GET: Request of obtaining MAC PIB attribute */
    void (* R_G3RFMAC_MlmeSetCnf)       (const r_g3rfmac_mlme_set_cnf_t * pcnf);        /**< Callback function for confirmation MLME-SET: Request of setting MAC PIB attribute */
    void (* R_RFLMAC_LmlDataInd)        (const r_rflmac_lml_data_ind_t * pind);         /**< Callback function for indication LML-DATA: Notification of data reception */
} r_g3_rfeap_callback_t;


typedef struct
{
    void (* R_ADP_AdpdDataCnf)          (const r_adp_adpd_data_cnf_t * pcnf);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpdDataCnf */
    void (* R_ADP_AdpmResetCnf)         (const r_adp_adpm_reset_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmResetCnf */
    void (* R_ADP_AdpmDiscoveryCnf)     (const r_adp_adpm_discovery_cnf_t * pcnf);      /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmDiscoveryCnf */
    void (* R_ADP_AdpmNetworkStartCnf)  (const r_adp_adpm_network_start_cnf_t * pcnf);  /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkStartCnf */
    void (* R_ADP_AdpmNetworkJoinCnf)   (const r_adp_adpm_network_join_cnf_t * pcnf);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkJoinCnf */
    void (* R_ADP_AdpmNetworkLeaveCnf)  (const r_adp_adpm_network_leave_cnf_t * pcnf);  /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkLeaveCnf */
    void (* R_ADP_AdpmGetCnf)           (const r_adp_adpm_get_cnf_t * pcnf);            /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmGetCnf */
    void (* R_ADP_AdpmSetCnf)           (const r_adp_adpm_set_cnf_t * pcnf);            /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmSetCnf */
    void (* R_ADP_AdpmRouteDiscoveryCnf)(const r_adp_adpm_route_disc_cnf_t * pcnf);     /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteDiscoveryCnf */
    void (* R_ADP_AdpmPathDiscoveryCnf) (const r_adp_adpm_path_discovery_cnf_t * pcnf); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmPathDiscoveryCnf */
    void (* R_ADP_AdpmLbpCnf)           (const r_adp_adpm_lbp_cnf_t * pcnf);            /**< Callback function for confirmation ADPM-LBP: Request of transmitting LoWPAN protocol (LBP) message */
    void (* R_ADP_AdpdDataInd)          (const r_adp_adpd_data_ind_t * pind);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpdDataInd */
    void (* R_ADP_AdpmNetworkLeaveInd)  (void);                                         /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkLeaveInd */
    void (* R_ADP_AdpmPathDiscoveryInd) (const r_adp_adpm_path_discovery_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmPathDiscoveryInd */
    void (* R_ADP_AdpmLbpInd)           (const r_adp_adpm_lbp_ind_t * pind);            /**< Callback function for indication ADPM-LBP: Notification of reception of LoWPAN protocol (LBP) message */
    void (* R_ADP_AdpmNetworkStatusInd) (const r_adp_adpm_network_status_ind_t * pind); /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmNetworkStatusInd */
    void (* R_ADP_AdpmBufferInd)        (const r_adp_adpm_buffer_ind_t * pind);         /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmBufferInd */
    void (* R_ADP_AdpmKeyStateInd)      (const r_adp_adpm_key_state_ind_t * pind);      /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmKeyStateInd */
    void (* R_ADP_AdpmRouteErrorInd)    (const r_adp_adpm_route_error_ind_t * pind);    /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteErrorInd */
    void (* R_ADP_AdpmEapKeyInd)        (const r_adp_adpm_eap_key_ind_t * pind);        /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmEapKeyInd */
    void (* R_ADP_AdpmFrameCounterInd)  (const r_adp_adpm_framecounter_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmFrameCounterInd */
    void (* R_ADP_AdpmRouteUpdateInd)   (const r_adp_adpm_route_update_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRouteUpdateInd */
    void (* R_ADP_AdpmLoadSeqNumInd)    (const r_adp_adpm_load_seq_num_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmLoadSeqNumInd */
    void (* R_ADP_AdpmRrepInd)          (const r_adp_adpm_rrep_ind_t * pind);           /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmRrepInd */
    void (* R_ADP_AdpmBeaconNotifyInd)  (const r_adp_adpm_bcn_notify_ind_t * pind);     /**< @copydoc r_g3_eap_callback_t::R_ADP_AdpmBeaconNotifyInd */
    void *reserved[4];
    void (* R_G3RFMAC_MlmeGetCnf)       (const r_g3rfmac_mlme_get_cnf_t * pcnf);        /**< Callback function for confirmation MLME-GET: Request of obtaining MAC PIB attribute */
    void (* R_G3RFMAC_MlmeSetCnf)       (const r_g3rfmac_mlme_set_cnf_t * pcnf);        /**< Callback function for confirmation MLME-SET: Request of setting MAC PIB attribute */
    void (* R_RFLMAC_LmlDataInd)        (const r_rflmac_lml_data_ind_t * pind);         /**< Callback function for indication LML-DATA: Notification of data reception */
} r_g3_rfadp_callback_t;

typedef struct
{
    void (* R_G3RFMAC_McpsDataCnf)        (const r_g3rfmac_mcps_data_cnf_t * pcnf);         /**< Callback function for confirmation MCPS-DATA: Request of data transmission */
    void (* R_G3RFMAC_MlmeResetCnf)       (const r_g3rfmac_mlme_reset_cnf_t * pcnf);        /**< Callback function for confirmation MLME-RESET: Request of resetting UMAC layer and the lower layer  */
    void (* R_G3RFMAC_MlmeGetCnf)         (const r_g3rfmac_mlme_get_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeGetCnf */
    void (* R_G3RFMAC_MlmeSetCnf)         (const r_g3rfmac_mlme_set_cnf_t * pcnf);          /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeSetCnf */
    void (* R_G3RFMAC_MlmeScanCnf)        (const r_g3rfmac_mlme_scan_cnf_t * pcnf);         /**< Callback function for confirmation MLME-SCAN: Request of searching PAN */
    void (* R_G3RFMAC_MlmeStartCnf)       (const r_g3rfmac_mlme_start_cnf_t * pcnf);        /**< Callback function for confirmation MLME-START: Request of constructing PAN */
    void (* R_G3RFMAC_McpsDataInd)        (const r_g3rfmac_mcps_data_ind_t * pind);         /**< Callback function for indication MCPS-DATA: Notification of data reception */
    void (* R_G3RFMAC_MlmeBeaconNotifyInd)(const r_g3rfmac_mlme_bcn_notify_ind_t * pind);   /**< @copydoc r_g3_eap_callback_t::R_G3MAC_MlmeBeaconNotifyInd */
    void (* R_G3RFMAC_MlmeCommStatusInd)  (const r_g3rfmac_mlme_comm_status_ind_t * pind);  /**< Callback function for indication MLME-COMM-STATUS: Notification of communication status */
    void (* R_G3RFMAC_MlmeFrameCountInd)  (const r_g3rfmac_mlme_framecount_ind_t * pind);   /**< Callback function for indication MLME-FRAMECOUNT: Notification of frame counter  */
    void (* R_G3RFMAC_MlmeWarningInd)     (const r_g3rfmac_mlme_warning_ind_t * pind);      /**< Callback function for indication MLME-WARNING: Notification of warning  */
    void (* R_RFLMAC_LmlDataInd)          (const r_rflmac_lml_data_ind_t * pind);           /**< Callback function for indication LML-DATA: Notification of data reception */
} r_g3_rfmac_callback_t;

#endif /* #ifdef R_DEFINE_DISABLE_HYBRID R_DEFINE_OPEN_ARM */

typedef union
{
    r_g3_eap_callback_t    eap;
    r_g3_adp_callback_t    adp;
    r_g3_mac_callback_t    mac;
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    r_g3_hyeap_callback_t  hyeap;
    r_g3_hyadp_callback_t  hyadp;
    r_g3_hymac_callback_t  hymac;
    r_g3_rfeap_callback_t  rfeap;
    r_g3_rfadp_callback_t  rfadp;
    r_g3_rfmac_callback_t  rfmac;
#endif
} r_g3_layer_callback_t;

typedef struct
{
    void (* R_G3_SetConfigCnf)          (const r_g3_set_config_cnf_t * pcnf);           /**< Callback function for confirmation G3-SETCONFIG: Request of setting G3 configuration parameter */
    void (* R_G3_GetConfigCnf)          (const r_g3_get_config_cnf_t * pcnf);           /**< Callback function for confirmation G3-GETCONFIG: Request of obtaining G3 configuration parameter */
    void (* R_G3_GetInfoCnf)            (const r_g3_get_info_cnf_t * pcnf);             /**< Callback function for confirmation G3-GETINFO: Request of obtaining statistics and log */
    void (* R_G3_ClearInfoCnf)          (const r_g3_clear_info_cnf_t * pcnf);           /**< Callback function for confirmation G3-CLEARINFO: Request of erasing statistics and log  */
    void (* R_G3_DumpCnf)               (const r_g3_dump_cnf_t* pcnf);                  /**< Callback function for confirmation G3-DUMP: Request of dumping wave data */
    void (* R_G3_DumpAbortCnf)          (const r_g3_dump_abort_cnf_t* pcnf);            /**< Callback function for confirmation G3-DUMP-ABORT: Request of aborting dump */
    void (* R_G3_DumpInd)               (const r_g3_dump_ind_t* pind);                  /**< Callback function for indication G3-DUMP: Notification od dump data */
    void (* R_G3_EventInd)              (const r_g3_event_ind_t * pind);                /**< Callback function for indication G3-EVENT: Notification of G3 channel event */
    r_g3_layer_callback_t mode;
} r_g3_callback_t;

/*!***************************************************************************
 * @} (end addtogroup group_cpx_api_callbacks)
 ****************************************************************************/


/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/*===========================================================================*/
/* For G3 SAP IF FUNCTION                                                    */
/*===========================================================================*/
r_result_t R_G3_Init (uint8_t ch, r_g3_callback_t * pcallbacks, r_g3_init_req_t * preq, uint16_t time_out);
r_result_t R_G3_Deinit (uint8_t ch, uint16_t time_out);
r_result_t R_G3_GetConfigReq (uint8_t ch);
r_result_t R_G3_SetConfigReq (uint8_t ch, r_g3_set_config_req_t * preq);
r_result_t R_G3_GetInfoReq (uint8_t ch, r_g3_get_info_req_t * preq);
r_result_t R_G3_ClearInfoReq (uint8_t ch, r_g3_clear_info_req_t * preq);
r_result_t R_G3_DumpReq (uint8_t ch, r_g3_dump_req_t *preq);
r_result_t R_G3_DumpAbortReq ( uint8_t ch);
/*===========================================================================*/
/* For EAP SAP IF FUNCTION                                                   */
/*===========================================================================*/
r_result_t R_EAP_EapmResetReq (uint8_t ch);
r_result_t R_EAP_EapmStartReq (uint8_t ch);
r_result_t R_EAP_EapmGetReq (uint8_t ch, r_eap_eapm_get_req_t * preq);
r_result_t R_EAP_EapmSetReq (uint8_t ch, r_eap_eapm_set_req_t * preq);
r_result_t R_EAP_EapmNetworkReq (uint8_t ch, r_eap_eapm_network_req_t * preq);
r_result_t R_EAP_EapmSetClientinfoReq (uint8_t ch, r_eap_eapm_set_clientinfo_req_t * preq);

/*===========================================================================*/
/* For ADP SAP IF FUNCTION                                                   */
/*===========================================================================*/
r_result_t R_ADP_AdpdDataReq (uint8_t ch, r_adp_adpd_data_req_t * preq);
r_result_t R_ADP_AdpmResetReq (uint8_t ch);
r_result_t R_ADP_AdpmDiscoveryReq (uint8_t ch, r_adp_adpm_discovery_req_t * preq);
r_result_t R_ADP_AdpmNetworkStartReq (uint8_t ch, r_adp_adpm_network_start_req_t * preq);
r_result_t R_ADP_AdpmNetworkJoinReq (uint8_t ch, r_adp_adpm_network_join_req_t * preq);
r_result_t R_ADP_AdpmNetworkLeaveReq (uint8_t ch);
r_result_t R_ADP_AdpmGetReq (uint8_t ch, r_adp_adpm_get_req_t * preq);
r_result_t R_ADP_AdpmSetReq (uint8_t ch, r_adp_adpm_set_req_t * preq);
r_result_t R_ADP_AdpmRouteDiscoveryReq (uint8_t ch, r_adp_adpm_route_disc_req_t * preq);
r_result_t R_ADP_AdpmPathDiscoveryReq (uint8_t ch, r_adp_adpm_path_discovery_req_t * preq);
r_result_t R_ADP_AdpmLbpReq (uint8_t ch, r_adp_adpm_lbp_req_t * preq);

/*===========================================================================*/
/* For G3MAC SAP API                                                         */
/*===========================================================================*/
#ifndef R_DEFINE_DISABLE_UMAC
r_result_t R_G3MAC_McpsDataReq (uint8_t ch, r_g3mac_mcps_data_req_t * preq);
r_result_t R_G3MAC_MlmeResetReq (uint8_t ch, r_g3mac_mlme_reset_req_t * preq);
#endif
r_result_t R_G3MAC_MlmeGetReq (uint8_t ch, r_g3mac_mlme_get_req_t * preq);
r_result_t R_G3MAC_MlmeSetReq (uint8_t ch, r_g3mac_mlme_set_req_t * preq);
#ifndef R_DEFINE_DISABLE_UMAC
r_result_t R_G3MAC_MlmeScanReq (uint8_t ch, r_g3mac_mlme_scan_req_t * preq);
r_result_t R_G3MAC_MlmeStartReq (uint8_t ch, r_g3mac_mlme_start_req_t * preq);
#endif

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#ifndef R_DEFINE_DISABLE_UMAC
r_result_t R_G3RFMAC_McpsDataReq (uint8_t ch, r_g3rfmac_mcps_data_req_t * preq);
r_result_t R_G3RFMAC_MlmeResetReq (uint8_t ch, r_g3rfmac_mlme_reset_req_t * preq);
#endif
r_result_t R_G3RFMAC_MlmeGetReq (uint8_t ch, r_g3rfmac_mlme_get_req_t * preq);
r_result_t R_G3RFMAC_MlmeSetReq (uint8_t ch, r_g3rfmac_mlme_set_req_t * preq);
#ifndef R_DEFINE_DISABLE_UMAC
r_result_t R_G3RFMAC_MlmeScanReq (uint8_t ch, r_g3rfmac_mlme_scan_req_t * preq);
r_result_t R_G3RFMAC_MlmeStartReq (uint8_t ch, r_g3rfmac_mlme_start_req_t * preq);
#endif
#endif


#endif /* R_G3_API_H */

