/*************************************************************************
* File Name - web_interface.h
* File Description - This file contains the header definitions for Web Interface
*
* Date    Author        Reference       Reason
* 10/03/06 XYZ SRS 1.1 Initial creation/ Bug description
*
* Copyright 2007-2008 Nethra, Inc. All Rights Reserved
**************************************************************************/

#include "nethra_sys_user.h"

#ifndef _WEB_INTERFACE_H_
#define _WEB_INTERFACE_H_

/* Extern */
extern int cli_set_sensitivity(uchar lcSensitivityLevel, int liMsgId);
extern int cli_get_sensitivity(uchar *lpSensitivityLevel, int liMsgId);

extern int cli_set_white_balance(uchar lcWhiteBalance, int liMsgId);
extern int cli_get_white_balance(uchar *lpWhiteBalance, int liMsgId);

extern int cli_set_exposure(uchar lcAutoExposure, int liMsgId);
extern int cli_get_exposure(uchar *lpAutoExposure, int liMsgId);

extern int cli_set_light_frequency(uchar lcLightFrequency, int liMsgId);
extern int cli_get_light_frequency(uchar *lpLightFrequency, int liMsgId);

extern int cli_set_brightness(uchar lcBrightnessLevel, int liMsgId);
extern int cli_get_brightness(uchar *lpBrightnessLevel, int liMsgId);

extern int cli_set_exposure_compensation(uchar lcBrightnessLevel, int liMsgId);
extern int cli_get_exposure_compensation(uchar *lpBrightnessLevel, int liMsgId);

extern int cli_set_frame_rate(uchar lcFrameRate, int liMsgId);
extern int cli_get_frame_rate(uchar *lpFrameRate, int liMsgId);

extern int cli_set_lens_shading_correction(uchar lcLensShadingCorrection, int liMsgId);
extern int cli_get_lens_shading_correction(uchar *lpLensShadingCorrection, int liMsgId);

extern int cli_set_shot_mode(uchar lcShotMode, int liMsgId);
extern int cli_get_shot_mode(uchar *lpShotMode, int liMsgId);

extern int cli_set_resolution(uchar lcResolutionType, int liMsgId);
extern int cli_get_resolution(uchar *lpResolutionType, int liMsgId);

extern int cli_set_saturation(uchar lcSaturationLevel, int liMsgId);
extern int cli_get_saturation(uchar *lpSaturationLevel, int liMsgId);

extern int cli_set_sharpness(uchar lcSharpnessLevel, int liMsgId);
extern int cli_get_sharpness(uchar *lpSharpnessLevel, int liMsgId);

extern int cli_set_contrast(uchar lcContrastLevel, int liMsgId);
extern int cli_get_contrast(uchar *lpContrastLevel, int liMsgId);

extern int cli_set_smoothness(uchar lcContrastLevel, int liMsgId);
extern int cli_get_smoothness(uchar *lpContrastLevel, int liMsgId);

extern int cli_set_backlit(uchar lcLightMode, int liMsgId);
extern int cli_get_backlit(uchar *lpLightMode, int liMsgId);

extern int cli_set_dhcp(uchar *lpDHCPServer, int liMsgId);
extern int cli_get_dhcp(uchar *lpDHCPEnable, int liMsgId);

extern int cli_set_network(uchar lcNetworkCommand, uchar *lpAddress, int liMsgId);
extern int cli_get_network(uchar lcNetworkCommand, uchar *lpAddress, int liMsgId);

extern int cli_save_current_config(long llMsgId);
extern int cli_set_default_config(long llMsgId);

extern int cli_firmware_upgrade(char *lpLoaderFile, char *lpFirmwareFile, char *lpCtrlTblFile, long llMsgId);

int cli_imx_firmware_upgrade(uchar lcFlashType, uchar lcImageType, uchar lcPrimarySecondary, char *lpFileName, long llMsgId);
//int cli_imx_firmware_upgrade(char *lpFileName, long llMsgId);

extern int cli_set_tv_out(uchar lcTVOut, long llMsgId);
extern int cli_get_tv_out(uchar *lpTVOut, long llMsgId);

extern int cli_set_video_format(uchar lcTVFormat, long llMsgId);
extern int cli_get_video_format(uchar *lpTVFormat, long llMsgId);

extern int cli_set_imagetype(uchar lcImageType, long llMsgId);
extern int cli_get_imagetype(uchar *lpImageType, long llMsgId);

extern int cli_start_streamer(char *pRTPIpAddress, char multicastMode, ushort rtpPort,
        uchar ttl, ushort lBitrate, long llMsgId);
extern int cli_stop_streamer(long llMsgId);
extern int cli_get_streamer_status(int* pStatus, char *pRTPIpAddress, char *pMulticastMode, 
        ushort *pRtpPort, uchar *pTtl, ushort *lpBitrate, long llMsgId);

extern int cli_set_pan(uchar lcPanMovement,uchar lcPanSpeed, long llMsgId);
extern int cli_get_pan(uchar *lpPanStatus, long llMsgId);

extern int cli_set_tilt(uchar lcTiltMovement,uchar lcTiltSpeed, long llMsgId);
extern int cli_get_tilt(uchar *lpTiltStatus, long llMsgId);

extern int cli_set_zoom(uchar lcZoom, long llMsgId);
extern int cli_get_zoom(uchar *lpZoomStatus, long llMsgId);

extern int cli_set_mirroring(uchar lMirroring, long llMsgId);
extern int cli_get_mirroring(uchar *plMirroring, long llMsgId);

extern int cli_set_rotation(ushort lRotation, long llMsgId);
extern int cli_get_rotation(ushort *plRotation, long llMsgId);

extern int cli_get_privacy_masking_status(uchar *lcStatus, long llMsgId);
extern int cli_set_privacy_masking_status(uchar cStatus, long llMsgId);
extern int cli_get_privacy_masking_area(uint *lArea, long llMsgId);
extern int cli_set_privacy_masking_area(uint Area, long llMsgId);

//extern int cli_set_config(struct sys_config sysConf, long llMsgId);
extern int cli_set_config(unsigned char command, unsigned char *str, long llMsgId);
//extern int cli_get_config(struct sys_config *pStSysConf, long llMsgId);
extern int cli_get_config(struct sys_config *pSysConf, struct ROsys_config *pROSysConf, long llMsgId);

extern int cli_get_motion_detection_status(unsigned char *pStatus, long llMsgId);
extern int cli_set_motion_detection_status(uchar uStatus, long llMsgId);
extern int cli_get_motion_detection_sensitivity(unsigned char *pSensitivity, long llMsgId);
extern int cli_set_motion_detection_sensitivity(uchar uSensitivity, long llMsgId);
extern int cli_get_motion_detection_area(unsigned int *pArea, long llMsgId);
extern int cli_set_motion_detection_area(uint uArea, long llMsgId);
extern int cli_get_motion_detection_skip_frames(unsigned char *pSkipFrames, long llMsgId);
extern int cli_set_motion_detection_skip_frames(uchar uSkipFrames, long llMsgId);
extern int cli_set_motion_detection_smtp_server(char* pSmtpServer, long llMsgId);
extern int cli_get_motion_detection_smtp_server(char *pSmtpServer, long llMsgId);
extern int cli_set_motion_detection_email_id(char* pEmailId, long llMsgId);
extern int cli_get_motion_detection_email_id(char *pEmailId, long llMsgId);
extern int cli_set_io_alarm_status(uchar uStatus, long llMsgId);
extern int cli_get_io_alarm_status(uchar *pStatus, long llMsgId);
extern int cli_set_email_alarm(uchar lcEmailAlarm, long llMsgId);
extern int cli_get_email_alarm(uchar *lpEmailAlarm, long llMsgId);
extern int cli_marking_required(uchar lcMarkingEnable, long llMsgId);
extern int cli_get_marking(uchar *lpMarkingStatus, long llMsgId);
extern int cli_set_percentage_area(uchar lcPercentageArea, long llMsgId);
extern int cli_get_percentage_area(uchar *lpPercentageArea, long llMsgId);

extern int cli_reset_system(long llMsgId);

extern int cli_set_focus_mode(uchar lcFocusMode, long llMsgId);
extern int cli_get_focus_mode(uchar *lpFocusMode, long llMsgId);

extern int cli_set_color_effect(uchar lcColorEffect, long llMsgId);
extern int cli_get_color_effect(uchar *lpColorEffect, long llMsgId);

extern int cli_set_eis(uchar lcEisMode, long llMsgId);
extern int cli_get_eis(uchar *lpEisMode, long llMsgId);

extern int cli_set_llnf_mode(uchar lcLlnfMode, long llMsgId);
extern int cli_get_llnf_mode(uchar *lpLlnfMode, long llMsgId);

extern int cli_set_osd_logo(uchar lcOsdLogo, long llMsgId);
extern int cli_get_osd_logo(uchar *lpOsdLogo, long llMsgId);

extern int cli_set_day_night(uchar lcDayNightMode, long llMsgId);
extern int cli_get_day_night(uchar *lpDayNightMode, long llMsgId);

extern int cli_set_ir_day_mode(uchar lcIrDayMode, long llMsgId);
extern int cli_get_ir_day_mode(uchar *lpIrDayMode, long llMsgId);

extern int cli_set_ir_night_mode(uchar lcIrNightMode, long llMsgId);
extern int cli_get_ir_night_mode(uchar *lpIrNightMode, long llMsgId);

extern int cli_set_iris(uchar lcIris, long llMsgId);
extern int cli_get_iris(uchar *lpIris, long llMsgId);

extern int cli_read_temp(int *lpTemp, long llMsgId);

extern int cli_set_red_gain(uchar lcRedGainEnable, uint liRGain, long llMsgId);
extern int cli_get_red_gain(uchar *lpcRedGainEnable, uint *lpRGain, long llMsgId);

extern int cli_set_blue_gain(uchar lcBlueGainEnable, uint liBGain, long llMsgId);
extern int cli_get_blue_gain(uchar *lpcBlueGainEnable, uint *lpBGain, long llMsgId);

extern int cli_set_sensor_win(uchar lcSensorWindowEnable, uchar lcSensorWindow, long llMsgId);
extern int cli_get_sensor_win(uchar *lpSensorWindowEnable, uchar *lpSensorWindow, long llMsgId);

extern int cli_set_crop_etpz(uchar lcCroppingEnable, uchar *pcCropping, long llMsgId);
extern int cli_get_crop_etpz(uchar *lpCroppingEnable, uchar *lpCropping, long llMsgId);

extern int cli_set_factory_default(long llMsgId);

extern int cli_set_alarm(int liAlarm, long llMsgId);
extern int cli_imx_read_temp(int *lpTemp, long llMsgId);

#endif /* _WEB_INTERFACE_H_ */
