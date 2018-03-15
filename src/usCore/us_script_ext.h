///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_script_ext.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : script_ext class
//	Reference : http://ly4cn.cnblogs.com/archive/2005/11/27/285439.html
//				http://www.cnblogs.com/Files/ly4cn/LuaPlusHelper.rar
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SCRIPT_EXT_H_
#define _US_SCRIPT_EXT_H_
//#include <usCore\Export.h>

#include <usLua/LuaObject.h>
#include <usLua/LuaState.h>
#include <usUtil/us_common_file.h>

#include <string>
#include <vector>
#include <basetsd.h>

using namespace LuaPlus;

namespace uniscope_globe
{
	class LuaConvert
	{
	public:
		LuaConvert(LuaPlus::LuaObject& obj)
			: mObj(obj)
		{
		}

		operator int()
		{
			return mObj.GetInteger();
		}

		operator ulong()
		{
			return mObj.GetInteger();
		}

		operator float()
		{
			return mObj.GetFloat();
		}

		operator double()
		{
			return mObj.GetDouble();
		}

		operator const char* ()
		{
			return mObj.GetString();
		}

		operator const wchar_t* ()
		{
			return (wchar_t*)mObj.GetWString();
		}

		operator std::string()
		{
			return std::string(mObj.GetString());
		}

		operator void* ()
		{
			return mObj.GetUserData();
		}

		template<typename ObjectType>
		operator ObjectType* ()
		{
			return (ObjectType*)mObj.GetUserData();
		}

		//template<typename R>
		//operator LuaFunction<R> ()
		//{
		//	return LuaFunction<R>(mObj);
		//}

	private:
		LuaPlus::LuaObject mObj;
	};

	template<typename ObjectType>
	class LuaConstructor
	{
	private:
		static int ConstructorHelper(LuaPlus::LuaState* state, ObjectType* pObj)
		{
			std::string metaname("MetaClass_");
			metaname += typeid(ObjectType).raw_name();

			LuaObject obj = state->BoxPointer(pObj);
			obj.SetMetaTable(state->GetGlobal(metaname.c_str()));
			obj.Push();
			return 1;
		}

	public:
		static int Constructor(LuaPlus::LuaState* state)
		{
			return ConstructorHelper(state, new ObjectType());
		}

		template<typename A1>
		static int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaPlus::LuaObject(state, 1);
			return ConstructorHelper(state, new ObjectType((A1)a1));
		}

		template<typename A1, typename A2>
		static	int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaObject(state, 1);
			LuaConvert a2 = LuaObject(state, 2);
			return ConstructorHelper(state, new ObjectType((A1)a1, (A2)a2));
		}

		template<typename A1, typename A2, typename A3>
		static	int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaObject(state, 1);
			LuaConvert a2 = LuaObject(state, 2);
			LuaConvert a3 = LuaObject(state, 3);
			return ConstructorHelper(state, new ObjectType((A1)a1, (A2)a2, (A3)a3));
		}

		template<typename A1, typename A2, typename A3, typename A4>
		static	int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaObject(state, 1);
			LuaConvert a2 = LuaObject(state, 2);
			LuaConvert a3 = LuaObject(state, 3);
			LuaConvert a4 = LuaObject(state, 4);
			return ConstructorHelper(state, new ObjectType((A1)a1, (A2)a2, (A3)a3, (A4)a4));
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5>
		static	int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaObject(state, 1);
			LuaConvert a2 = LuaObject(state, 2);
			LuaConvert a3 = LuaObject(state, 3);
			LuaConvert a4 = LuaObject(state, 4);
			LuaConvert a5 = LuaObject(state, 5);
			return ConstructorHelper(state, new ObjectType((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5));
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
		static	int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaObject(state, 1);
			LuaConvert a2 = LuaObject(state, 2);
			LuaConvert a3 = LuaObject(state, 3);
			LuaConvert a4 = LuaObject(state, 4);
			LuaConvert a5 = LuaObject(state, 5);
			LuaConvert a6 = LuaObject(state, 6);
			return ConstructorHelper(state, new ObjectType((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5, (A6)a6));
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
		static	int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaObject(state, 1);
			LuaConvert a2 = LuaObject(state, 2);
			LuaConvert a3 = LuaObject(state, 3);
			LuaConvert a4 = LuaObject(state, 4);
			LuaConvert a5 = LuaObject(state, 5);
			LuaConvert a6 = LuaObject(state, 6);
			LuaConvert a7 = LuaObject(state, 7);
			return ConstructorHelper(state, new ObjectType((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5, (A6)a6, (A7)a7));
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
		static	int Constructor(LuaPlus::LuaState* state)
		{
			LuaConvert a1 = LuaObject(state, 1);
			LuaConvert a2 = LuaObject(state, 2);
			LuaConvert a3 = LuaObject(state, 3);
			LuaConvert a4 = LuaObject(state, 4);
			LuaConvert a5 = LuaObject(state, 5);
			LuaConvert a6 = LuaObject(state, 6);
			LuaConvert a7 = LuaObject(state, 7);
			LuaConvert a8 = LuaObject(state, 8);
			return ConstructorHelper(state, new ObjectType((A1)a1, (A2)a2, (A3)a3, (A4)a4, (A5)a5, (A6)a6, (A7)a7, (A8)a8));
		}

		static int Destructor(LuaPlus::LuaState* state)
		{
			LuaObject o(state, 1);

			delete (ObjectType*)state->UnBoxPointer(1);

			LuaObject meta = state->GetGlobal("MetaClass_Nil");
			if (meta.IsNil())
			{
				meta = state->GetGlobals().CreateTable("MetaClass_Nil");
			}

			o.SetMetaTable(meta);
			return 0;
		}
	};

	template<typename ObjectType>
	class LuaClass
	{
	public:
		LuaClass(LuaPlus::LuaState* state)
		{
			mGlobals = state->GetGlobals();

			std::string metaname("MetaClass_");
			metaname += typeid(ObjectType).raw_name();

			mMetaTableObj = mGlobals.CreateTable(metaname.c_str());
			mMetaTableObj.SetObject("__index", mMetaTableObj);

			// 自动注册释放接口为Free
			//mMetaTableObj.Register("Free", &Destructor);
		}

		template<typename Func>
		inline LuaClass& DefineFunction(const char* name, Func func)
		{
			mMetaTableObj.RegisterObjectDirect(name, (ObjectType*)0, func);
			return *this;
		}

		//template<typename Func, typename A1>
		//inline LuaClass& DefineFunction(const char* name, Func func)
		//{
		//	//mMetaTableObj.
		//	mMetaTableObj.Register(name, LuaConstructor<ObjectType>::Constructor<A1>, func);
		//	return *this;
		//}

		//template<typename Func, typename A1, typename A2>
		//inline LuaClass& DefineFunction(const char* name, Func func)
		//{
		//	//mMetaTableObj.
		//	mMetaTableObj.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2>);
		//	return *this;
		//}

		////void Register( const char* funcName, const Callee& callee, int (Callee::*func)(LuaState*), int nupvalues = 0 );

		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor);
			return *this;
		}

		template<typename A1>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1>);
			return *this;
		}

		template<typename A1, typename A2>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2>);
			return *this;
		}

		template<typename A1, typename A2, typename A3>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2, A3>);
			return *this;
		}

		template<typename A1, typename A2, typename A3, typename A4>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2, A3, A4>);
			return *this;
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2, A3, A4, A5>);
			return *this;
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2, A3, A4, A5, A6>);
			return *this;
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2, A3, A4, A5, A6, A7>);
			return *this;
		}

		template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
		inline LuaClass& CreateObject(const char* name)
		{
			mGlobals.Register(name, LuaConstructor<ObjectType>::Constructor<A1, A2, A3, A4, A5, A6, A7, A8>);
			return *this;
		}

		inline LuaClass& DestroyObject(const char* name)
		{
			mMetaTableObj.Register(name, LuaConstructor<ObjectType>::Destructor);
			return *this;
		}

	private:
		LuaPlus::LuaObject mMetaTableObj;
		LuaPlus::LuaObject mGlobals;
	};

	class LuaModule
	{
	public:
		LuaModule(LuaPlus::LuaState* state)
		{
			mModuleObj = state->GetGlobals();
			state->GetGlobals().SetNumber("_this", (INT_PTR)state);
		}

		LuaModule(LuaPlus::LuaState* state, const char* name)
		{
			mModuleObj = state->GetGlobals().CreateTable(name);
			lua_Number num1 = (INT_PTR)state;
			INT_PTR num2 = (INT_PTR)state;
			lua_Integer num3 = (INT_PTR)state;
			state->GetGlobals().SetNumber("_this", (INT_PTR)state);
		}

		template<typename Func>
		inline LuaModule& DefineFunction(const char* name, Func func)
		{
			mModuleObj.RegisterDirect(name, func);
			return *this;
		}

		template<typename ObjectType, typename Func>
		inline LuaModule& DefineFunction(const char* name, ObjectType& o, Func func)
		{
			mModuleObj.RegisterDirect(name, o, func);
			return *this;
		}

	private:
		LuaPlus::LuaObject mModuleObj;
	};

	class LuaValueBinder
	{
	public:
		enum ValueType
		{
			VT_REAL,
			VT_INTEGER,
			VT_BOOLEAN
		};

		LuaValueBinder(LuaPlus::LuaState* state, char* name, int& value)
			: mGlobals(state->GetGlobals())
			, mName(name)
			, mValue(&value)
			, mType(VT_INTEGER)
		{
			SaveToLua();
		}

		LuaValueBinder(LuaPlus::LuaState* state, char* name, double& value)
			: mGlobals(state->GetGlobals())
			, mName(name)
			, mValue(&value)
			, mType(VT_REAL)
		{
			SaveToLua();
		}

		LuaValueBinder(LuaPlus::LuaState* state, char* name, bool& value)
			: mGlobals(state->GetGlobals())
			, mName(name)
			, mValue(&value)
			, mType(VT_BOOLEAN)
		{
			SaveToLua();
		}

		~LuaValueBinder()
		{
			LoadFromLua();
		}

	protected:
		inline void SaveToLua()
		{
			switch (mType)
			{
			case VT_INTEGER:
			{
				mGlobals.SetInteger(mName, *(int*)mValue);
			}
			break;

			case VT_BOOLEAN:
			{
				mGlobals.SetBoolean(mName, *(bool*)mValue);
			}
			break;

			case VT_REAL:
			{
				mGlobals.SetNumber(mName, *(double*)mValue);
			}
			break;
			}
		}

		inline void LoadFromLua()
		{
			switch (mType)
			{
			case VT_INTEGER:
			{
				*(int*)mValue = mGlobals[mName].GetInteger();
			}
			break;

			case VT_BOOLEAN:
			{
				*(bool*)mValue = mGlobals[mName].GetBoolean();
			}
			break;

			case VT_REAL:
			{
				*(double*)mValue = mGlobals[mName].GetNumber();
			}
			break;
			}
		}

	protected:
		int			mType;
		LuaPlus::LuaObject	mGlobals;
		char*		mName;
		void*		mValue;
	};

	class LuaEnvironment
	{
	public:
		LuaEnvironment()
		{
			mState = LuaPlus::LuaState::Create(true);
		}

		~LuaEnvironment()
		{
			for (int i = 0; i < (int)mValueBinders.size(); i++)
			{
				LuaValueBinder* pBinder = mValueBinders[i];
				if (NULL != pBinder)
				{
					delete pBinder;
				}
			}

			mValueBinders.clear();

			LuaPlus::LuaState::Destroy(mState);
		}


	public:
		LuaPlus::LuaState*						mState;
		std::vector<LuaValueBinder*>	mValueBinders;
	};
}


#endif // _US_SCRIPT_EXT_H_