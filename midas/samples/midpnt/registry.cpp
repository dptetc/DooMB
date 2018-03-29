/*      Registry.cpp
 *
 * Registry handling class
 *
 * $Id: registry.cpp,v 1.4 1997/07/10 18:41:28 pekangas Exp $
 *
 * Copyright 1996 Petteri Kangaslampi
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#include "midasdll.h"
#include "MidpNT.h"
#include "registry.h"


static void Error(char *msg)
{
    Panic(msg);
}


Registry::Registry()
{
    key = NULL;
}


Registry::~Registry()
{
    if ( key != NULL )
        CloseKey();
}


int Registry::KeyExists(const char *name)
{
    HKEY tempKey;
    LONG        err;

    err = RegOpenKeyEx(HKEY_CURRENT_USER, name, 0, KEY_ALL_ACCESS, &tempKey);
    if ( err != ERROR_SUCCESS )
        return 0;
    
    RegCloseKey(tempKey);
    
    return 1;
}


void Registry::CreateKey(const char *name)
{
    LONG        err;
    DWORD       createStatus;

    err = RegCreateKeyEx(HKEY_CURRENT_USER, name, 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &createStatus);
    if ( err != ERROR_SUCCESS )
        Error("Registry key creation failed");
}



void Registry::OpenKey(const char *name)
{
    LONG        err;

    if ( key != NULL )
        CloseKey();

    err = RegOpenKeyEx(HKEY_CURRENT_USER, name, 0, KEY_ALL_ACCESS, &key);
    if ( err != ERROR_SUCCESS )
        Error("Registry key opening failed");
}


void Registry::CloseKey()
{
    assert(key != NULL);
    if ( RegCloseKey(key) != ERROR_SUCCESS )
        Error("Registry key close failed");
    key = NULL;
}


void Registry::Value(const char *name, void *data, DWORD *dataLength, DWORD
    bufferLength, DWORD *dataType)
{
    LONG        err;
    DWORD       len;
    DWORD       type;

    len = bufferLength;
    err = RegQueryValueEx(key, (LPSTR) name, 0, &type, (LPBYTE) data,
        &len);
    if ( err != ERROR_SUCCESS )
    {
        len = *dataLength;
        type = *dataType;
        err = RegSetValueEx(key, (LPSTR) name, 0, type,
            (CONST BYTE*) data, len);
        if ( err != ERROR_SUCCESS )
            Error("Registry value creation failed");
    }

    *dataLength = len;
    *dataType = type;
}


void Registry::ValueString(const char *name, const char *defaultData,
    char *dest, int bufferLength)
{
    DWORD       len;
    DWORD       type = REG_SZ;

    if ( (unsigned) bufferLength < (strlen(defaultData) + 1) )
        Error("Registry::ValueString() - too long default");

    strcpy(dest, defaultData);
    len = strlen(dest) + 1;

    Value(name, (void*) dest, &len, bufferLength, &type);

    if ( type != REG_SZ )
        Error("Registry::ValueString() - illegal value type");
}



void Registry::WriteString(const char *name, const char *string)
{
    DWORD       err;

    err = RegSetValueEx(key, (LPSTR) name, 0, REG_SZ, (CONST BYTE*) string,
        strlen(string)+1);
    if ( err != ERROR_SUCCESS )
        Error("Registry::WriteString(): value creation failed");
}



DWORD Registry::ValueDWORD(const char *name, DWORD defaultData)
{
    DWORD       len, type, buf;

    buf = defaultData;
    len = sizeof(DWORD);
    type = REG_DWORD;

    Value(name, (void*) &buf, &len, sizeof(DWORD), &type);

    if ( type != REG_DWORD )
        Error("Registy::ValueDWORD() - illegal value type");

    return buf;
}



void Registry::WriteDWORD(const char *name, const DWORD data)
{
    DWORD       err;
    DWORD       buf = data;

    err = RegSetValueEx(key, (LPSTR) name, 0, REG_DWORD, (CONST BYTE*) &buf,
        sizeof(DWORD));
    if ( err != ERROR_SUCCESS )
        Error("Registry::WriteDWORD(): value creation failed");
}



int Registry::ValueExists(const char *name)
{
    LONG        err;

    err = RegQueryValueEx(key, (LPSTR) name, 0, NULL, NULL, 0);
    if ( err != ERROR_SUCCESS )
        return 0;
    return 1;
}


unsigned Registry::GetNumSubKeys()
{
    char className[4];
    DWORD classLen = 3;
    DWORD subKeys, maxSubKeyLen, maxClassLen, numValues, maxValueNameLen;
    DWORD maxValueDataLen, securityDescLen;
    FILETIME lastModified;

    assert(key);
    if ( RegQueryInfoKey(key, className, &classLen, NULL, &subKeys,
                         &maxSubKeyLen, &maxClassLen, &numValues,
                         &maxValueNameLen, &maxValueDataLen, &securityDescLen,
                         &lastModified) != ERROR_SUCCESS )
        Error("RegQueryInfoKey failed");
    
    return subKeys;
}


void Registry::DeleteSubKey(const char *name)
{
    assert(key);
    
    if ( RegDeleteKey(key, name) != ERROR_SUCCESS )
        Error("RegDeleteKey failed");    
}


void Registry::GetSubKeyName(unsigned num, char *nameBuffer,
                             unsigned bufferLen)
{
    DWORD nameLen = bufferLen;
    FILETIME lastModified;
    
    assert(key);

    if ( RegEnumKeyEx(key, num, nameBuffer, &nameLen, NULL, NULL, NULL,
                      &lastModified) != ERROR_SUCCESS )
        Error("RegEnumKeyEx failed");
}




/*
 * $Log: registry.cpp,v $
 * Revision 1.4  1997/07/10 18:41:28  pekangas
 * Added echo effect editor and echo effect support
 *
 * Revision 1.3  1997/01/14 17:42:08  pekangas
 * Changed to use MIDAS DLL API
 *
 * Revision 1.2  1996/07/16 19:37:31  pekangas
 * Fixed to compile with Visual C, converted to LFs and added RCS keywords
 *
*/
