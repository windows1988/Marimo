﻿#include <string>

#include "clstd.h"
#include "clString.h"
//#include "clmemory.h"
//#include "clFile.h"
//#include "clBuffer.h"
#include "clUtility.h"

#ifdef _CRT_FINDFILE
# include <io.h>
#endif

//#ifdef _WINDOWS
//#pragma warning(disable:4355)
//#endif // #ifdef _WINDOWS

namespace clstd
{
#if defined(_WINDOWS) || defined(_WIN32)

  STATIC_ASSERT(File::DA_Read  == GENERIC_READ);
  STATIC_ASSERT(File::DA_Write  == GENERIC_WRITE);

  STATIC_ASSERT(File::SM_Delete  == FILE_SHARE_DELETE);
  STATIC_ASSERT(File::SM_Read  == FILE_SHARE_READ);
  STATIC_ASSERT(File::SM_Write  == FILE_SHARE_WRITE);

  STATIC_ASSERT(File::CD_CreateNew      == CREATE_NEW);
  STATIC_ASSERT(File::CD_CreateAlways    == CREATE_ALWAYS);
  STATIC_ASSERT(File::CD_OpenExisting    == OPEN_EXISTING);
  STATIC_ASSERT(File::CD_OpenAlways      == OPEN_ALWAYS);
  STATIC_ASSERT(File::CD_TruncateExisting  == TRUNCATE_EXISTING);

  STATIC_ASSERT(File::A_Archive    == FILE_ATTRIBUTE_ARCHIVE);
  STATIC_ASSERT(File::A_Compressed  == FILE_ATTRIBUTE_COMPRESSED);
  STATIC_ASSERT(File::A_Hidden    == FILE_ATTRIBUTE_HIDDEN);
  STATIC_ASSERT(File::A_Normal    == FILE_ATTRIBUTE_NORMAL);
  STATIC_ASSERT(File::A_Offline    == FILE_ATTRIBUTE_OFFLINE);
  STATIC_ASSERT(File::A_ReadOnly  == FILE_ATTRIBUTE_READONLY);
  STATIC_ASSERT(File::A_System    == FILE_ATTRIBUTE_SYSTEM);
  STATIC_ASSERT(File::A_Temporary  == FILE_ATTRIBUTE_TEMPORARY);

  STATIC_ASSERT(FILEATTRIBUTE_READONLY   == FILE_ATTRIBUTE_READONLY);
  STATIC_ASSERT(FILEATTRIBUTE_HIDDEN     == FILE_ATTRIBUTE_HIDDEN);
  STATIC_ASSERT(FILEATTRIBUTE_SYSTEM     == FILE_ATTRIBUTE_SYSTEM);
  STATIC_ASSERT(FILEATTRIBUTE_DIRECTORY  == FILE_ATTRIBUTE_DIRECTORY);

#endif // #if defined(_WINDOWS) || defined(_WIN32)

  void CreateFileCreationDistributionToStdOpenCode(
    File::CreationDistribution eCreationDistribution, 
    File::DesiredAccess eDesiredAccess, 
    ch* szOpenMode, s32 nLength)
  {
    memset(szOpenMode, 0, nLength);

    switch(eCreationDistribution)
    {
    case File::CD_CreateNew:
      ASSERT(0);
      break;
    case File::CD_CreateAlways:
      szOpenMode[0] = 'w';
      szOpenMode[1] = 'b';
      if(eDesiredAccess & File::DA_Read)
        szOpenMode[2] = '+';
      break;
    case File::CD_OpenExisting:
      szOpenMode[0] = 'r';
      szOpenMode[1] = 'b';
      if(eDesiredAccess & File::DA_Write)
        szOpenMode[2] = '+';
      break;
    case File::CD_OpenAlways:
      ASSERT(0);
      break;
    case File::CD_TruncateExisting:
      ASSERT(0);
      break;
    }
  }

  File::File()
    : m_hFile(NULL)
    //, m_TextStream(this)
  {
  }

  File::~File()
  {
    Close();
  }

  b32 File::OpenExisting(CLLPCSTR pszFileName)
  {
    Close();
    const b32 bval = CreateFile(pszFileName, DA_Read, SM_Read, CD_OpenExisting, A_Archive);
    return bval;
  }

  b32 File::CreateAlways(CLLPCSTR pszFileName)
  {
    Close();
    const b32 bval = CreateFile(pszFileName, DA_Write, SM_Read, CD_CreateAlways, A_Archive);
    return bval;
  }

  b32 File::CreateNew(CLLPCSTR pszFileName)
  {
    Close();
    const b32 bval = CreateFile(pszFileName, DA_Write, SM_Read, CD_CreateNew, A_Archive);
    return bval;
  }

  b32 File::CreateFile(
    CLLPCSTR        pszFileName, 
    DesiredAccess      eDesiredAccess, 
    ShareMode        eShareMode, 
    CreationDistribution  eCreationDistribution, 
    FlagsAndAttributes    eFlagAttr)
  {
    Close();
#ifdef _CL_SYSTEM_WINDOWS
    m_hFile = ::CreateFileA(pszFileName, eDesiredAccess, eShareMode, NULL, 
      eCreationDistribution, eFlagAttr, NULL);
    //if (m_hFile == INVALID_HANDLE_VALUE) {
    //  _cl_WinVerifyFailure("Win32::CreateFileA()", __FILE__, __LINE__, GetLastError());
    //}
    return (m_hFile != INVALID_HANDLE_VALUE);
#else
    // TODO: 这个没测试过
    ch szOpenMode[8];
    CreateFileCreationDistributionToStdOpenCode(
      eCreationDistribution, eDesiredAccess, szOpenMode, sizeof(szOpenMode));
    m_hFile = fopen(pszFileName, szOpenMode);
    return (m_hFile != NULL);
#endif // _WINDOWS

  }
  b32 File::OpenExisting(CLLPCWSTR pszFileName)
  {
    Close();
    const b32 bval = CreateFile(pszFileName, DA_Read, SM_Read, CD_OpenExisting, A_Archive);
    return bval;
  }

  b32 File::CreateAlways(CLLPCWSTR pszFileName)
  {
    Close();
    const b32 bval = CreateFile(pszFileName, DA_Write, SM_Read, CD_CreateAlways, A_Archive);
    return bval;
  }

  b32 File::CreateNew(CLLPCWSTR pszFileName)
  {
    Close();
    const b32 bval = CreateFile(pszFileName, DA_Write, SM_Read, CD_CreateNew, A_Archive);
    return bval;
  }

  b32 File::CreateFile(
    CLLPCWSTR             pszFileName, 
    DesiredAccess         eDesiredAccess, 
    ShareMode             eShareMode, 
    CreationDistribution  eCreationDistribution, 
    FlagsAndAttributes    eFlagAttr)
  {
    Close();
#ifdef _CL_SYSTEM_WINDOWS
    m_hFile = ::CreateFileW(pszFileName, eDesiredAccess, eShareMode, NULL, 
      eCreationDistribution, eFlagAttr, NULL);

    //if (m_hFile == INVALID_HANDLE_VALUE) {
    //  _cl_WinVerifyFailure("Win32::CreateFileW()", __FILE__, __LINE__, GetLastError());
    //}
    return (m_hFile != INVALID_HANDLE_VALUE);
#else
    // TODO: 这个没测试过
    ch szOpenMode[8];
    CreateFileCreationDistributionToStdOpenCode(
      eCreationDistribution, eDesiredAccess, szOpenMode, sizeof(szOpenMode));

    clStringA strFile(pszFileName);
    m_hFile = fopen(strFile, szOpenMode);
    return (m_hFile != NULL);
#endif // _WINDOWS
  }

  void File::Close()
  {
    if(m_hFile != NULL)
    {
#ifdef _CL_SYSTEM_WINDOWS
      CloseHandle(m_hFile);
#else
      fclose(m_hFile);
#endif // _WINDOWS
      m_hFile = NULL;
    }
  }

  u32 File::GetPointer()
  {
    return SetPointer(0, 1);
  }

  u32 File::SetPointer(u32 uMove, u32 uMode)
  {
#ifdef _CL_SYSTEM_WINDOWS
    return ::SetFilePointer(m_hFile, uMove, NULL, uMode);
#else
    return fseek(m_hFile, uMove, uMode);
    ASSERT(0);
#endif // #ifdef _WINDOWS
  }

  u64 File::GetPointer64()
  {
    return SetPointer64(0, 1);
  }

  u64 File::SetPointer64(u64 uMove, u32 uMode)
  {
#ifdef _CL_SYSTEM_WINDOWS
    union {
      struct {
        LONG lLow;
        LONG lHigh;
      };
      u64 v;
    }d;
    d.v = uMove;
    d.lLow = ::SetFilePointer(m_hFile, d.lLow, &d.lHigh, uMode);
    return d.v;
#else
    return fseek(m_hFile, (long)uMove, uMode);
#endif // #ifdef _WINDOWS
  }


  u32 File::GetSize(u32* pdwFileSizeHight) const
  {
#ifdef _CL_SYSTEM_WINDOWS
    return ::GetFileSize(m_hFile, (LPDWORD)pdwFileSizeHight);
#else
    u32 dwLength;
    long current = ftell(m_hFile);
    fseek(m_hFile, 0, SEEK_END);
    dwLength = ftell(m_hFile);
    fseek(m_hFile, current, SEEK_SET);
    if(pdwFileSizeHight != NULL)
      *pdwFileSizeHight = 0;
    return dwLength;
#endif
  }

  void File::GetTime(TIME* lpCreationTime, TIME* lpLastAccessTime, TIME* lpLastWriteTime) const
  {
#ifdef _CL_SYSTEM_WINDOWS
    FILETIME sCreationTime;
    FILETIME sLastAccessTime;
    FILETIME sLastWriteTime;
    GetFileTime(m_hFile, &sCreationTime, &sLastAccessTime, &sLastWriteTime);
    if(lpCreationTime)
    {
      lpCreationTime->dwHighDateTime = sCreationTime.dwHighDateTime;
      lpCreationTime->dwLowDateTime  = sCreationTime.dwLowDateTime;
    }
    if(lpLastAccessTime)
    {
      lpLastAccessTime->dwHighDateTime = sLastAccessTime.dwHighDateTime;
      lpLastAccessTime->dwLowDateTime  = sLastAccessTime.dwLowDateTime;
    }
    if(lpLastWriteTime)
    {
      lpLastWriteTime->dwHighDateTime = sLastWriteTime.dwHighDateTime;
      lpLastWriteTime->dwLowDateTime  = sLastWriteTime.dwLowDateTime;
    }
#else
		ASSERT(0);
//#error not implement!
#endif // #ifdef _WINDOWS
  }

  b32 File::Read(CLLPVOID lpBuffer, u32 nNumOfBytesToRead,  u32* lpNumberOfBytesRead)
  {
    u32 dwNumRead;
#ifdef _CL_SYSTEM_WINDOWS
    const b32 bRet = ::ReadFile(m_hFile, lpBuffer, nNumOfBytesToRead, (LPDWORD)&dwNumRead, NULL);
#else
    dwNumRead = fread(lpBuffer, nNumOfBytesToRead, 1, m_hFile);
    ASSERT(dwNumRead == 1); // 如果出现断言则说明发生错误
    dwNumRead = nNumOfBytesToRead;
    const b32 bRet = (dwNumRead != 0);
#endif // _WINDOWS
    if(lpNumberOfBytesRead != NULL)
      *lpNumberOfBytesRead = dwNumRead;
    return bRet;
  }

  b32 File::Write(CLLPCVOID lpBuffer, u32 nNumberOfBytesToWrite, u32* lpNumberOfBytesWritten)
  {
    u32 dwNumWrite;
#ifdef _CL_SYSTEM_WINDOWS
    const b32 bRet = ::WriteFile(m_hFile, lpBuffer, nNumberOfBytesToWrite, (LPDWORD)&dwNumWrite, NULL);
#else
    dwNumWrite = fwrite(lpBuffer, nNumberOfBytesToWrite, 1, m_hFile);
    const b32 bRet = (dwNumWrite != 0);
#endif // _WINDOWS
    if(lpNumberOfBytesWritten != NULL)
      *lpNumberOfBytesWritten = dwNumWrite;
    return bRet;
  }

  //int File::Writef(const tch* format, ...)
  //{
  //  va_list arglist;

  //  va_start(arglist, format);
  //  
  //  u32 dwNumWrite;
  //  clString buffer;
  //  buffer.VarFormat(format, arglist);
  //  Write(buffer, (u32)buffer.GetLength(), &dwNumWrite);

  //  va_end(arglist);
  //  return (int)dwNumWrite;
  //}


  int File::WritefA(const ch* format, ...)
  {
    va_list arglist;

    va_start(arglist, format);

    u32 dwNumWrite;
    clStringA buffer;
    buffer.VarFormat(format, arglist);
    Write(buffer, (u32)buffer.GetLength(), &dwNumWrite);

    va_end(arglist);
    return (int)dwNumWrite;
  }

  int File::WritefW(const wch* format, ...)
  {
    va_list arglist;

    va_start(arglist, format);

    u32 dwNumWrite;
    clStringW buffer;
    buffer.VarFormat(format, arglist);
    Write(buffer, (u32)buffer.GetLength(), &dwNumWrite);

    va_end(arglist);
    return (int)dwNumWrite;
  }


  b32 File::MapToBuffer(CLBYTE** ppBuffer, int nFileOffset, int cbSize, u32* pcbSize)
  {
    u32 dwSizeLow, dwSizeHigh;
    CLBYTE* pBuffer = NULL;

    if(m_hFile == NULL || ppBuffer == NULL || pcbSize == NULL)
      goto FALSE_RET;

    if(cbSize < 0) {  // 如果为负, 则说明(u32)cbSize > 2GB
      goto FALSE_RET;
    }

    dwSizeLow = GetSize(&dwSizeHigh);

    // 如果映射全部文件且文件大于 2GB,则返回 FALSE
    if(cbSize == 0 && (dwSizeHigh != 0 && dwSizeLow > 0x80000000))
      goto FALSE_RET;

    if(dwSizeLow == 0)
      goto FALSE_RET;

    cbSize = cbSize == 0 ? dwSizeLow :cbSize;

    pBuffer = new CLBYTE[cbSize];
    if(pBuffer == NULL)
      goto FALSE_RET;

    if(Read(pBuffer, cbSize, pcbSize) == FALSE)
    {
      SAFE_DELETE_ARRAY(pBuffer);
      goto FALSE_RET;
    }
    *ppBuffer = pBuffer;
    //*pdwSize = dwSizeLow;
    return TRUE;

FALSE_RET:
    if(pBuffer != NULL) {
      *ppBuffer = NULL;
    }
    if(pcbSize != NULL) {
      *pcbSize = 0;
    }
    return FALSE;
  }

  b32 File::MapToBuffer(Buffer** ppBuffer, int nFileOffset, int cbSize)
  {
    CLBYTE*   pBuffer    = NULL;
    u32       dwSize     = 0;
    Buffer* pBufferObj = NULL;

    if(MapToBuffer(&pBuffer, nFileOffset, cbSize, &dwSize) == FALSE || ppBuffer == NULL)
      return FALSE;

    pBufferObj = new Buffer;
    pBufferObj->Append(pBuffer, dwSize);

    *ppBuffer = pBufferObj;
    SAFE_DELETE(pBuffer);
    dwSize = 0;
    return TRUE;
  }

  b32 File::ReadToBuffer(Buffer* pBuffer, int nFileOffset, u32 cbSize)
  {
    u32 dwSizeHigh;
    const i32 nReadSize = cbSize == 0 ? GetSize(&dwSizeHigh) 
      : clMin((u32)(GetSize(&dwSizeHigh) - (u32)nFileOffset), cbSize);

    // return false if file size great than 2GB.
    if(nReadSize <= 0 || dwSizeHigh != 0) {
      return FALSE;
    }

    u32 nNumBytes = 0;
    pBuffer->Resize(nReadSize, FALSE);

    SetPointer64(nFileOffset, 0);
    return Read(pBuffer->GetPtr(), nReadSize, &nNumBytes) && (nReadSize == nNumBytes);
  }

  //File::TextStream& File::GetTextStream()
  //{
  //  return m_TextStream;
  //}

  //File::TextStream::TextStream(File* pFile)
  //  : m_pFile(pFile)
  //{
  //}

  //File::TextStream&  File::TextStream::operator <<(const wch* pszString)
  //{
  //  m_pFile->WritefW(pszString);
  //  return *this;
  //}

  //File::TextStream&  File::TextStream::operator <<(const ch* pszString)
  //{
  //  m_pFile->WritefA(pszString);
  //  return *this;
  //}

  //File::TextStream&  File::TextStream::operator <<(const int nValue)
  //{
  //  m_pFile->WritefW(_T("%d"), nValue);
  //  return *this;
  //}

  //File::TextStream&  File::TextStream::operator <<(const float fValue)
  //{
  //  m_pFile->WritefW(_T("%f"), fValue);
  //  return *this;
  //}

  //File::TextStream&  File::TextStream::operator <<(const clString& str)
  //{
  //  m_pFile->WritefW(str);
  //  return *this;
  //}
  //////////////////////////////////////////////////////////////////////////
#if (defined(_WINDOWS) || defined(_WIN32)) && ! defined(_CRT_FINDFILE)
  FindFile::FindFile()
    : hFind(INVALID_HANDLE_VALUE)
  {
    InlSetZeroT(wfd);
  }

  FindFile::FindFile(CLLPCSTR szFilename)
    : hFind(INVALID_HANDLE_VALUE)
  {
    NewFind(szFilename);
  }

  FindFile::FindFile(CLLPCWSTR szFilename)
    : hFind(INVALID_HANDLE_VALUE)
  {
    NewFind(szFilename);
  }

  b32 FindFile::NewFind(CLLPCWSTR szFilename)
  {
    if(hFind != INVALID_HANDLE_VALUE) {
      FindClose(hFind);
    }
    hFind = FindFirstFileW(szFilename, &wfd);
    return (hFind != INVALID_HANDLE_VALUE);
  }

  b32 FindFile::NewFind(CLLPCSTR szFilename)
  {
    clStringW strFilename = szFilename;
    return NewFind(strFilename);
  }

  b32 FindFile::GetFile(FINDFILEDATAW* FindFileData)
  {
    if(hFind == INVALID_HANDLE_VALUE) {
      return FALSE;
    }
    clstd::strcpynT(FindFileData->Filename, wfd.cFileName, MAX_PATH);
    FindFileData->dwAttributes  = IntTranslateAttr(wfd.dwFileAttributes);
    FindFileData->nFileSizeHigh = wfd.nFileSizeHigh;
    FindFileData->nFileSizeLow  = wfd.nFileSizeLow;
    if( ! FindNextFileW(hFind, &wfd))
    {
      FindClose(hFind);
      hFind = INVALID_HANDLE_VALUE;
    }
    return TRUE;
  }

  b32 FindFile::GetFile(FINDFILEDATAA* FindFileData)
  {
    FINDFILEDATAW ffdW;
    const b32 bval = GetFile(&ffdW);
    WideCharToMultiByte(CP_ACP, NULL, ffdW.Filename, -1, FindFileData->Filename, MAX_PATH, NULL, NULL);
    FindFileData->dwAttributes  = IntTranslateAttr(ffdW.dwAttributes);
    FindFileData->nFileSizeHigh = ffdW.nFileSizeHigh;
    FindFileData->nFileSizeLow  = ffdW.nFileSizeLow;
    return bval;
  }

  CLDWORD FindFile::IntTranslateAttr(CLDWORD uNativeAttr)
  {
    CLDWORD dwAttr = 0;
    if(TEST_FLAG(uNativeAttr, FILEATTRIBUTE_READONLY)) {
      SET_FLAG(dwAttr, FileAttribute_ReadOnly);
    }
    if(TEST_FLAG(uNativeAttr, FILEATTRIBUTE_HIDDEN)) {
      SET_FLAG(dwAttr, FileAttribute_Hidden);
    }
    if(TEST_FLAG(uNativeAttr, FILEATTRIBUTE_SYSTEM)) {
      SET_FLAG(dwAttr, FileAttribute_System);
    }
    if(TEST_FLAG(uNativeAttr, FILEATTRIBUTE_DIRECTORY)) {
      SET_FLAG(dwAttr, FileAttribute_Directory);
    }
    return dwAttr;
  }

#else
  FindFile::FindFile()
    : handle(InvalidHandleValue)
  {
    InlSetZeroT(finddata);
  }

  FindFile::FindFile(CLLPCSTR szFilename)
    : handle(InvalidHandleValue)
  {
    NewFind(szFilename);
  }

  FindFile::FindFile(CLLPCWSTR szFilename)
    : handle(InvalidHandleValue)
  {
    NewFind(szFilename);
  }

  b32 FindFile::NewFind(CLLPCWSTR szFilename)
  {
    clStringA strFilename = szFilename;
    return NewFind(strFilename);
  }

  b32 FindFile::NewFind(CLLPCSTR szFilename)
  {
    if(handle != InvalidHandleValue) {
      _findclose(handle);
    }
    handle = _findfirst(szFilename, &finddata);
    return (handle != InvalidHandleValue);
  }

  b32 FindFile::GetFile(FINDFILEDATAW* FindFileData)
  {
    FINDFILEDATAA finddataA;
    if( ! GetFile(&finddataA)) {
      return FALSE;
    }
    clStringW str(finddataA.Filename);
    clstd::strcpynT(FindFileData->Filename, (const wch*)str, MAX_PATH);

    return TRUE;
  }

  b32 FindFile::GetFile(FINDFILEDATAA* FindFileData)
  {
    if(handle == InvalidHandleValue) {
      return FALSE;
    }

    clstd::strcpynT(FindFileData->Filename, finddata.name, MAX_PATH);
    FindFileData->dwAttributes  = IntTranslateAttr(finddata.attrib);
    FindFileData->nFileSizeHigh = 0;
    FindFileData->nFileSizeLow  = finddata.size;
    if(_findnext(handle, &finddata)) // 非0表示Done!
    {
      _findclose(handle);
      handle = InvalidHandleValue;
    }
    return TRUE;
  }

  CLDWORD FindFile::IntTranslateAttr(CLDWORD uNativeAttr)
  {
    CLDWORD dwAttr = 0;
    if(TEST_FLAG(uNativeAttr, _A_RDONLY)) {
      SET_FLAG(dwAttr, FileAttribute_ReadOnly);
    }
    if(TEST_FLAG(uNativeAttr, _A_HIDDEN)) {
      SET_FLAG(dwAttr, FileAttribute_Hidden);
    }
    if(TEST_FLAG(uNativeAttr, _A_SYSTEM)) {
      SET_FLAG(dwAttr, FileAttribute_System);
    }
    if(TEST_FLAG(uNativeAttr, _A_SUBDIR)) {
      SET_FLAG(dwAttr, FileAttribute_Directory);
    }
    return dwAttr;
  }
#endif // #if defined(_WINDOWS) || defined(_WIN32)
} // namespace clstd
