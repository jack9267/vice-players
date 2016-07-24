/*	see copyright notice in squirrel.h */
#include "sqstackhandler.h"

SQStackHandler::SQStackHandler(SQVM * pVM)
{
	m_pVM = pVM;
	m_iStackTop = sq_gettop(pVM);
}

SQVM * SQStackHandler::GetVM()
{
	return m_pVM;
}

SQInteger SQStackHandler::GetTop()
{
	return m_iStackTop;
}

SQInteger SQStackHandler::GetParameterCount()
{
	return (m_iStackTop - 1);
}

bool SQStackHandler::CheckStackIndex(SQInteger iStackIndex, SQObjectType type)
{
	if(iStackIndex <= 0 || iStackIndex > m_iStackTop)
		return false;

	if(type != OT_NULL && sq_gettype(m_pVM, iStackIndex) != type)
	{
		return false;
	}

	return true;
}

SQObjectType SQStackHandler::GetType(SQInteger iStackIndex)
{
	if(!CheckStackIndex((iStackIndex + 1), OT_NULL))
		return OT_NULL;

	return sq_gettype(m_pVM, (iStackIndex + 1));
}

SQBool SQStackHandler::GetSQBool(SQInteger iStackIndex)
{
	if(!CheckStackIndex((iStackIndex + 1), OT_BOOL))
		return 0;

	SQBool bValue;
	sq_getbool(m_pVM, (iStackIndex + 1), &bValue);
	return bValue;
}

bool SQStackHandler::GetBool(SQInteger iStackIndex)
{
	if(!CheckStackIndex((iStackIndex + 1), OT_BOOL))
		return 0;

	SQBool bValue;
	sq_getbool(m_pVM, (iStackIndex + 1), &bValue);
	return (bValue != 0);
}

SQInteger SQStackHandler::GetInteger(SQInteger iStackIndex)
{
	if(!CheckStackIndex((iStackIndex + 1), OT_INTEGER))
		return 0;

	SQInteger iValue;
	sq_getinteger(m_pVM, (iStackIndex + 1), &iValue);
	return iValue;
}

SQFloat SQStackHandler::GetFloat(SQInteger iStackIndex)
{
	if(!CheckStackIndex((iStackIndex + 1), OT_FLOAT))
		return 0.0f;

	SQFloat fValue;
	sq_getfloat(m_pVM, (iStackIndex + 1), &fValue);
	return fValue;
}

const SQChar * SQStackHandler::GetString(SQInteger iStackIndex)
{
	if(!CheckStackIndex((iStackIndex + 1), OT_STRING))
		return NULL;

	const SQChar * szValue;
	sq_getstring(m_pVM, (iStackIndex + 1), &szValue);
	return szValue;
}

SQUserPointer SQStackHandler::GetUserPointer(SQInteger iStackIndex)
{
	if(!CheckStackIndex((iStackIndex + 1), OT_USERPOINTER))
		return 0;

	SQUserPointer pValue;
	sq_getuserpointer(m_pVM, (iStackIndex + 1), &pValue);
	return pValue;
}

int SQStackHandler::ReturnBool(SQBool bValue)
{
	sq_pushbool(m_pVM, bValue);
	return 1;
}

int SQStackHandler::ReturnInteger(SQInteger iValue)
{
	sq_pushinteger(m_pVM, iValue);
	return 1;
}

int SQStackHandler::ReturnFloat(SQFloat fValue)
{
	sq_pushfloat(m_pVM, fValue);
	return 1;
}

int SQStackHandler::ReturnString(const SQChar * szValue)
{
	sq_pushstring(m_pVM, szValue, -1);
	return 1;
}

int SQStackHandler::ReturnUserPointer(SQUserPointer pValue)
{
	sq_pushuserpointer(m_pVM, pValue);
	return 1;
}

int SQStackHandler::ReturnNull()
{
	sq_pushnull(m_pVM);
	return 1;
}

int SQStackHandler::Return()
{
	return 0;
}