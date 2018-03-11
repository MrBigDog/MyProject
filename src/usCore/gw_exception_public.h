///////////////////////////////////////////////////////////
//  gw_exception_public.h
//  Implementation of the Class gw_exception_public
//  Created on:      10-十一月-2015 11:26:52
//  Original author: felix
///////////////////////////////////////////////////////////
#pragma once

#ifndef gw_exception_public_h__
#define gw_exception_public_h__

#include "exception_base.h"
//#include "gw_structured_exception.h"
#include "struct_exception_base.h"///<测试通过的
#include "SimpleSymbolEngine.h"

///定义GeniusWorld基本异常

DECLARE_EXCEPTION_CLASS_EX(gw_render_exception)
DECLARE_EXCEPTION_CLASS_EX(gw_parser_exception)
DECLARE_EXCEPTION_CLASS_EX(gw_trigger_exception)
DECLARE_EXCEPTION_CLASS_EX(gw_command_exception)
DECLARE_EXCEPTION_CLASS_EX(gw_mission_exception)
DECLARE_EXCEPTION_CLASS_EX(gw_network_exception)
DECLARE_EXCEPTION_CLASS_EX(gw_particle_exception)
DECLARE_EXCEPTION_CLASS_EX(gw_logic_exception)

#define AUTO_DECLEAR_STRUCT_EXCEPTION gw_exception::struct_exception_base::MapSHEtoCE();

#define AUTO_CHECK_MEMORY_LEAK gw_exception::gw_memerory_leak_check::init();
//DECLARE_EXCEPTION_CLASS(myexception,exception_base)
#endif // gw_exception_public_h__

