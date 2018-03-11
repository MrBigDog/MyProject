///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_script_module.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : script_module class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SCRIPT_MODULE_H_
#define _US_SCRIPT_MODULE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT script_module
	{
	public:
		script_module(void);

		virtual ~script_module(void);

	public:
		virtual void _define_script_interface(LuaEnvironment& lua_env) {}

		virtual void _load_module(const char* module_file_string)
		{
			m_lua_state->DoFile(module_file_string);
		}

		virtual void _invoke(const char* func_string)
		{
			m_lua_state->DoString(func_string);
		}

	protected:
		LuaPlus::LuaState* m_lua_state;
	};

#define US_SCRIPT_REGISTER_CLASS(env, class_name, script_class_name) { class_name##_wrapper::register_class(env, script_class_name);}

#define US_SCRIPT_REGISTER_MODULE(env, class_name, object_pointer, script_module_name) { class_name##_wrapper::register_module(env, object_pointer, script_module_name);}

#define US_SCRIPT_CLASS_BEGIN( class_name )		public:\
													typedef class_name _BaseClass;\
													virtual void _define_script_interface( LuaEnvironment& lua_env )\
													{\
														m_lua_state = lua_env.mState;\
														LuaClass<class_name>(m_lua_state)
#define US_SCRIPT_CONSTRUCT( name )					.CreateObject( name )
#define US_SCRIPT_DESTRUCT( name )					.DestroyObject( name )
#define US_SCRIPT_MEMBER_FUNC( name, func )			.DefineFunction( name, &_BaseClass::func )
#define US_SCRIPT_CLASS_END()					;}


#define US_SCRIPT_MODULE_BEGIN( module_name, class_name )		public:\
																typedef class_name _ModuleClass;\
																virtual void _define_script_interface( LuaEnvironment& lua_env )\
																{\
																	m_lua_state = lua_env.mState;\
																	LuaModule(m_lua_state, module_name)
#define US_SCRIPT_CALLBACK(name, func)							.DefineFunction(name, func)
#define US_SCRIPT_FUNCTION(name, func)							.DefineFunction(name, *this, &_ModuleClass::func)
#define US_SCRIPT_FUNCTION2(name, obj, func)					.DefineFunction(name, obj, &_ModuleClass::func)
#define US_SCRIPT_MODULE_END()								;}



#define US_SCRIPT_CLASS_PTR_CONVERTER(base_class)			namespace LPCD\
																{\
																	inline void Push(lua_State* L, const base_class##* value)\
																	{\
																		LuaState* state = LuaState::CastState(L);\
																		LuaObject obj = state->BoxPointer((void*)value);\
																		string metaname = string("MetaClass_") + typeid(base_class).raw_name();\
																		obj.SetMetaTable(state->GetGlobal(metaname.c_str()));\
																	}\
																	inline bool	Match(TypeWrapper<base_class##*>, lua_State* L, int idx)\
																	{\
																		LuaState* state = LuaState::CastState(L);\
																		LuaObject obj = state->Stack(idx);\
																		string metaname = string("MetaClass_") + typeid(base_class).raw_name();\
																		return obj.GetMetaTable() == state->GetGlobal(metaname.c_str());\
																	}\
																	inline base_class##* Get(TypeWrapper<base_class##*>, lua_State* L, int idx)\
																	{\
																		return (base_class##*)lua_unboxpointer(L, idx);\
																	}\
																}
}

#endif // _US_SCRIPT_MODULE_H_