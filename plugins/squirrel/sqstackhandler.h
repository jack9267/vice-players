/*	see copyright notice in squirrel.h */
#pragma once

#include "sqpcheader.h"

struct SQStackHandler
{
private:
	SQVM * m_pVM;
	SQInteger m_iStackTop;

public:
	SQStackHandler(SQVM * pVM);

	SQVM * GetVM();
	SQInteger GetTop();
	SQInteger GetParameterCount();
	bool CheckStackIndex(SQInteger iStackIndex, SQObjectType type);
	SQObjectType GetType(SQInteger iStackIndex);
	SQBool GetSQBool(SQInteger iStackIndex);
	bool GetBool(SQInteger iStackIndex);
	SQInteger GetInteger(SQInteger iStackIndex);
	SQFloat GetFloat(SQInteger iStackIndex);
	const SQChar * GetString(SQInteger iStackIndex);
	SQUserPointer GetUserPointer(SQInteger iStackIndex);
	int ReturnBool(SQBool bValue);
	int ReturnInteger(SQInteger iValue);
	int ReturnFloat(SQFloat fValue);
	int ReturnString(const SQChar * szValue);
	int ReturnUserPointer(SQUserPointer pValue);
	int ReturnNull();
	int Return();
	/*SQFloat GetFloat(int idx) {
		SQFloat x = 0.0f;
		if(idx > 0 && idx <= _top) {
			sq_getfloat(v,idx,&x);
		}
		return x;
	}
	SQInteger GetInt(int idx) {
		SQInteger x = 0;
		if(idx > 0 && idx <= _top) {
			sq_getinteger(v,idx,&x);
		}
		return x;
	}
	HSQOBJECT GetObjectHandle(int idx) {
		HSQOBJECT x;
		if(idx > 0 && idx <= _top) {
			sq_resetobject(&x);
			sq_getstackobj(v,idx,&x);
		}
		return x;
	}
	const SQChar *GetString(int idx)
	{
		const SQChar *x = NULL;
		if(idx > 0 && idx <= _top) {
			sq_getstring(v,idx,&x);
		}
		return x;
	}
	SQUserPointer GetUserPointer(int idx)
	{
		SQUserPointer x = 0;
		if(idx > 0 && idx <= _top) {
			sq_getuserpointer(v,idx,&x);
		}
		return x;
	}
	SQUserPointer GetInstanceUp(int idx,SQUserPointer tag)
	{
		SQUserPointer self;
		if(SQ_FAILED(sq_getinstanceup(v,idx,(SQUserPointer*)&self,tag)))
			return NULL;
		return self;
	}
	SQUserPointer GetUserData(int idx,SQUserPointer tag=0)
	{
		SQUserPointer otag;
		SQUserPointer up;
		if(idx > 0 && idx <= _top) {
			if(SQ_SUCCEEDED(sq_getuserdata(v,idx,&up,&otag))) {
				if(tag == otag)
					return up;
			}
		}
		return NULL;
	}
	BOOL GetBool(int idx)
	{
		SQBool ret;
		if(idx > 0 && idx <= _top) {
			if(SQ_SUCCEEDED(sq_getbool(v,idx,&ret)))
				return ret;
		}
		return FALSE;
	}
	int GetType(int idx)
	{
		if(idx > 0 && idx <= _top) {
			return sq_gettype(v,idx);
		}
		return -1;
	}

	int GetParamCount() {
		return _top;
	}
	int Return(const SQChar *s)
	{
		sq_pushstring(v,s,-1);
		return 1;
	}
	int Return(FLOAT f)
	{
		sq_pushfloat(v,f);
		return 1;
	}
	int Return(INT i)
	{
		sq_pushinteger(v,i);
		return 1;
	}
	int Return(bool b)
	{
		sq_pushbool(v,b);
		return 1;
	}
	int Return(SQUserPointer p) {
		sq_pushuserpointer(v,p);
		return 1;
	}
	int Return(SquirrelObject &o)
	{
		sq_pushobject(v,o.GetObjectHandle());
		return 1;
	}
	int Return() { return 0; }
	int ThrowError(const SQChar *error) {
		return sq_throwerror(v,error);
	}
	HSQUIRRELVM GetVMPtr() { return v; }*/
};