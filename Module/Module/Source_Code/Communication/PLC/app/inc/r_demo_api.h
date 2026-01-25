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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_demo_api.h
*    @version
*        $Rev: 7243 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2021-07-30 17:02:34 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_API_H_
#define R_DEMO_API_H_

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Functions prototype
******************************************************************************/


/*===========================================================================*/
/*    G3CTRL APIs                                                            */
/*===========================================================================*/
r_result_t R_DEMO_G3SetConfig (uint8_t chId, r_g3_set_config_req_t * config,
                               r_g3_set_config_cnf_t ** cnf);
r_result_t R_DEMO_G3GetInfo (uint8_t chId, r_g3_get_info_req_t * req,
                             r_g3_get_info_cnf_t ** cnf);
r_result_t R_DEMO_G3ClearInfo (uint8_t chId, r_g3_clear_info_req_t * req,
                               r_g3_clear_info_cnf_t ** cnf);
r_result_t R_DEMO_DeInit (uint8_t chId);
r_result_t R_DEMO_G3Dump ( uint32_t dump_timeout );
/*===========================================================================*/
/*    MAC APIs                                                               */
/*===========================================================================*/
r_result_t R_DEMO_MacInit (uint8_t chId);
r_result_t R_DEMO_McpsData (uint8_t chId, const r_g3mac_mcps_data_req_t * mcpsDataReq,
                            r_g3mac_mcps_data_cnf_t ** cnf);
r_result_t R_DEMO_MlmeReset (uint8_t chId, const r_g3mac_mlme_reset_req_t * mlmeResetReq,
                             r_g3mac_mlme_reset_cnf_t ** cnf);
r_result_t R_DEMO_MlmeStart (uint8_t chId, const r_g3mac_mlme_start_req_t * mlmeStartReq,
                             r_g3mac_mlme_start_cnf_t ** cnf);
r_result_t R_DEMO_MlmeScan (uint8_t chId, const r_g3mac_mlme_scan_req_t * mlmeScanReq,
                            r_g3mac_mlme_scan_cnf_t ** cnf);

/***********************************************************************
* Function Name     : R_DEMO_MlmeSet
* Description       : API function for MLME-SET
* Argument          : mlmeSetReq : Pointer to request structure
*                   : mlmeSetCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_MlmeSet (uint8_t chId, const r_g3mac_mlme_set_req_t * mlmeSetReq,
                           r_g3mac_mlme_set_cnf_t ** mlmeSetCfm);

/***********************************************************************
* Function Name     : R_DEMO_MlmeGet
* Description       : API function for MLME-GET
* Argument          : mlmeGetReq : Pointer to request structure
*                   : mlmeGetCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_MlmeGet (uint8_t chId, const r_g3mac_mlme_get_req_t * mlmeGetReq,
                           r_g3mac_mlme_get_cnf_t ** mlmeGetCfm);

r_g3mac_status_t R_DEMO_MlmeGetWrap (uint8_t chId, uint16_t id, uint16_t index, uint8_t * val);
r_g3mac_status_t R_DEMO_MlmeSetWrap (uint8_t chId, uint16_t id, uint16_t index, uint8_t * val);

r_g3mac_status_t R_DEMO_MacSetConfig (uint8_t chId);

/*===========================================================================*/
/*    ADP APIs                                                               */
/*===========================================================================*/
/***********************************************************************
* Function Name     : R_DEMO_AdpInit
* Description       : Initializes the demo application API
* Argument          : None
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpInit (uint8_t chId);

/***********************************************************************
* Function Name     : R_DEMO_AdpmSet
* Description       : API function for ADPM-SET
* Argument          : setReq : Pointer to request structure
*                   : setCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmSet (uint8_t chId, const r_adp_adpm_set_req_t * setReq,
                           r_adp_adpm_set_cnf_t ** setCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmGet
* Description       : API function for ADPM-GET
* Argument          : getReq : Pointer to request structure
*                   : getCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmGet (uint8_t chId, const r_adp_adpm_get_req_t * getReq,
                           r_adp_adpm_get_cnf_t ** getCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmNetworkStart
* Description       : API function for ADPM-NETWORK-START
* Argument          : nwsReq : Pointer to request structure
*                   : nwsCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmNetworkStart (uint8_t chId, const r_adp_adpm_network_start_req_t * nwsReq,
                                    r_adp_adpm_network_start_cnf_t ** nwsCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpdData
* Description       : API function for ADPD-DATA
* Argument          : dataReq : Pointer to request structure
*                   : dataCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpdData (uint8_t chId, const r_adp_adpd_data_req_t * dataReq,
                            r_adp_adpd_data_cnf_t ** dataCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmDiscovery
* Description       : API function for ADPM-DISCOVERY
* Argument          : disReq : Pointer to request structure
*                   : disCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmDiscovery (uint8_t chId, const r_adp_adpm_discovery_req_t * disReq,
                                 r_adp_adpm_discovery_cnf_t ** disCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmNetworkJoin
* Description       : API function for ADPM-NETWORK-JOIN
* Argument          : nwjReq : Pointer to request structure
*                   : nwjCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmNetworkJoin (uint8_t chId, const r_adp_adpm_network_join_req_t * nwjReq,
                                   r_adp_adpm_network_join_cnf_t ** nwjCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmNetworkLeave
* Description       : API function for ADPM-NETWORK-LEAVE
* Argument          : nwlCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmNetworkLeave (uint8_t chId, r_adp_adpm_network_leave_cnf_t ** nwlCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmLbp
* Description       : API function for ADPM-LBP
* Argument          : lbpReq : Pointer to request structure
*                   : lbpCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmLbp (uint8_t chId, const r_adp_adpm_lbp_req_t * lbpReq,
                           r_adp_adpm_lbp_cnf_t ** lbpCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmRouteDiscovery
* Description       : API function for ADPM-ROTUE-DISCOVERY
* Argument          : rdisReq : Pointer to request structure
*                   : rdisCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmRouteDiscovery (uint8_t chId, const r_adp_adpm_route_disc_req_t * rdisReq,
                                      r_adp_adpm_route_disc_cnf_t ** rdisCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmPathDiscovery
* Description       : API function for ADPM-PATH-DISCOVERY
* Argument          : rdisReq : Pointer to request structure
*                   : rdisCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmPathDiscovery (uint8_t chId, const r_adp_adpm_path_discovery_req_t * rdisReq,
                                     r_adp_adpm_path_discovery_cnf_t ** rdisCfm);

/***********************************************************************
* Function Name     : R_DEMO_AdpmReset
* Description       : API function for ADPM-RESET
* Argument          : resCfm : Pointer to confirm structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
r_result_t R_DEMO_AdpmReset (uint8_t chId, r_adp_adpm_reset_cnf_t ** resCfm);



r_adp_status_t R_DEMO_AdpmGetWrap (uint8_t chId, r_adp_ib_id_t id, uint16_t index, uint8_t * val);
r_adp_status_t R_DEMO_AdpmSetWrap (uint8_t chId, r_adp_ib_id_t id, uint16_t index, uint8_t * val);

r_adp_status_t R_DEMO_AdpSetConfig (uint8_t chId);

void       R_DEMO_InitG3CallBack (uint8_t chId, r_g3_callback_t * pCallBack);
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_DEMO_InitMacCallBack (uint8_t chId, r_g3_callback_t * pCallBack, uint8_t cfgMode);
r_result_t R_DEMO_InitAdpCallBack (uint8_t chId, r_g3_callback_t * pCallBack, uint8_t cfgMode);
r_result_t R_DEMO_InitEapCallBack (uint8_t chId, r_g3_callback_t * pCallBack, uint8_t cfgMode);
#else
r_result_t R_DEMO_InitMacCallBack (uint8_t chId, r_g3_callback_t * pCallBack);
r_result_t R_DEMO_InitAdpCallBack (uint8_t chId, r_g3_callback_t * pCallBack);
r_result_t R_DEMO_InitEapCallBack (uint8_t chId, r_g3_callback_t * pCallBack);
#endif


#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_DEMO_RfMlmeSet(uint8_t chId, const r_g3rfmac_mlme_set_req_t * mlmeSetReq, r_g3rfmac_mlme_set_cnf_t ** cnf);
r_result_t R_DEMO_RfMlmeGet(uint8_t chId, const r_g3rfmac_mlme_get_req_t * mlmeGetReq, r_g3rfmac_mlme_get_cnf_t ** cnf);
r_g3rfmac_status_t R_DEMO_RfMlmeGetWrap(uint8_t chId, uint16_t id, uint16_t index, uint8_t * val);
r_g3rfmac_status_t R_DEMO_RfMlmeSetWrap(uint8_t chId, uint16_t id, uint16_t index, uint8_t * val);

r_result_t R_DEMO_RfMcpsData(uint8_t chId, const r_g3rfmac_mcps_data_req_t * mcpsDataReq, r_g3rfmac_mcps_data_cnf_t ** cnf);
r_result_t R_DEMO_RfMlmeReset(uint8_t chId, const r_g3rfmac_mlme_reset_req_t * mlmeResetReq, r_g3rfmac_mlme_reset_cnf_t ** cnf);


#endif

#endif /* R_DEMO_API_H_ */

