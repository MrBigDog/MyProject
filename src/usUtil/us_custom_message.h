///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_bldg_builder_message.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : bldg_builder_message class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_BLDG_BUILDER_MESSAGE_H_
#define _US_BLDG_BUILDER_MESSAGE_H_

namespace uniscope_globe
{

// 自定义消息

// 编译 控制
#define WM_BUILD_START		WM_APP + 0x001
#define WM_BUILD_PAUSE		WM_APP + 0x002
#define WM_BUILD_CONTINUE	WM_APP + 0x003
#define WM_BUILD_STOP		WM_APP + 0x004

// 编译 状态
#define WM_BUILD_DEFAULT	WM_APP + 0x030
#define WM_BUILD_END		WM_APP + 0x031

#define WM_BUILD_SUCCEED	WM_APP + 0x050
#define WM_BUILD_ERROR		WM_APP + 0x051

#define WM_PUBLISH_START     WM_APP + 0x081
#define WM_PUBLISH_PROG_ALL  WM_APP + 0x082
#define WM_PUBLISH_PROG_STEP WM_APP + 0x083

#define WM_PUBLISH_END       WM_APP + 0x091


// 编译 过程
#define WM_PROGRESS0_START	WM_APP + 0x100
#define WM_PROGRESS0_UPDATE	WM_APP + 0x101

#define WM_PROGRESS1_START	WM_APP + 0x110
#define WM_PROGRESS1_UPDATE	WM_APP + 0x111

#define WM_PROGRESS2_START	WM_APP + 0x120
#define WM_PROGRESS2_UPDATE	WM_APP + 0x121

#define WM_PROGRESS3_START	WM_APP + 0x130
#define WM_PROGRESS3_UPDATE	WM_APP + 0x131

// check out & check in
#define WM_CHECKOUT_SUCCEED	WM_APP + 0x140
#define WM_CHECKOUT_NOFILE	WM_APP + 0x141
#define WM_CHECKOUT_LOCKED	WM_APP + 0x142
#define WM_CHECKOUT_FAILED	WM_APP + 0x143

#define WM_CHECKIN_SUCCEED	WM_APP + 0x145
#define WM_CHECKIN_FAILED	WM_APP + 0x146

#define WM_UNDOCHECKOUT_SUCCEED WM_APP + 148
#define WM_UNDOCHECKOUT_FAILED  WM_APP + 149

}

#endif // _US_BLDG_BUILDER_MESSAGE_H_