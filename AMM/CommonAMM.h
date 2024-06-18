//
//
// CommonAMM.h: Common definitions used in:
//
//                   ATOMIC MEMORY MODEL - Implementation Examples 'Phase One' and 'Phase Two'
//
// © Copyright 2004 - 2014 by Miroslav Bonchev Bonchev. All rights reserved.
// 
//
//******************************************************************************************************


// Open Source License – The MIT License
//
//
// Atomic Memory Model © Copyright 2001 - 2014 by Miroslav Bonchev Bonchev.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated  documentation files  (the "Software"),  to deal  in the Software without restriction,
// including  without  limitation the rights  to use,  copy,  modify,  merge,  publish,  distribute,
// sublicense,  and/or sell copies of the Software,  and to permit persons to  whom the  Software is
// furnished to do so, subject to the following conditions:
//
// The  above  copyright  notice  and  this  permission  notice  shall be  included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  PARTICULAR  PURPOSE AND 
// NONINFRINGEMENT.  IN NO EVENT SHALL  THE  AUTHORS  OR  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT,  TORT OR OTHERWISE,  ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


//__________________________________________________________________________________________________
// This software is OSI Certified Open Source Software.
// OSI Certified is a certification mark of the Open Source Initiative.


#pragma once


#if defined( _DEBUG )
   #define START_DEBUG          __asm { int 3 };

   #if defined( __AFX_H__ )
      #define MASSERT(f)         ASSERT(f)
   #else
      #include                  <assert.h>
      #define MASSERT(f)         assert(f)
   #endif


   #if defined( ENABLE_MUnitException )
      #define MUnitException_throw(f)  throw( f )
   #else
      #define MUnitException_throw(f)  ((void)0)
   #endif

   #if defined( ENABLE_MAtomException )
      #define MAtomException_throw(f)  throw( f )
   #else
      #define MAtomException_throw(f)  ((void)0)
   #endif

#else
   #define MASSERT(f)               ((void)0)
   #define MUnitException_throw(f)  ((void)0)
   #define MAtomException_throw(f)  ((void)0)
#endif


#ifdef _WIN32
    // Usage
    // TRACE( "my string" ) 
    // TRACE( string( string::FF, "%-c-style-text-formatting", params, ... ) )
    // Respectively for single and composite strings to be outputed to the debug-output console.
    #define TRACE( str ) OutputDebugString( str );
#endif


// #define UNICODE
#ifdef UNICODE
   #define SCHAR         wchar_t
   #define LPCSSTR const SCHAR*
   #define STEXT(quote)  L##quote
#else
   #define SCHAR         char
   #define LPCSSTR const SCHAR*
   #define STEXT(quote)  quote
#endif


#ifdef _WIN32
   #include "windows.h"
   #define DECLARE_THROW( x )
#else
   #include <stdint.h>
   #include <stdio.h>
   #include <string.h>
   #include <cstdio>
   #include <errno.h>
   #include <time.h>
   #include <stdlib.h>
   #include <wchar.h>
   #include <ctype.h>
   #include <wctype.h>
   #include <stdarg.h>
   #include <limits.h>

   // Compiler attributes
   #define __forceinline                  inline// __attribute__((always_inline))
   #define DECLARE_THROW( x )
   #define EXCEPTION_EXECUTE_HANDLER      int

   // Define types
   #define HANDLE       FILE*
   #define HMODULE      void*

   #define LPCSSTR      const SCHAR*
   #define TEXT         STEXT

   #define TCHAR        SCHAR
   #define LPCTSTR      LPCSSTR
   #define LPCSTR       const char*
   #define LPCWSTR      const wchar_t*
   #define LPSTR        char*
   #define LPWSTR       wchar_t*
   #define LPCCH        const char*
   #define LPCWCH       const wchar_t*
   #define LPBOOL       const BOOL*

   #define DWORD        uint32_t
   #define LPDWORD      DWORD*
   #define INT          int32_t
   #define UINT         uint32_t

   #define USHORT       unsigned short
   #define WORD         unsigned short

   #define ULONGLONG    unsigned long long int
   #define __int64      long long int

   #define BOOL         bool
   #define BYTE         unsigned char

   #define __time64_t   time_t

   template< class T >
   struct TYPE_LARGE_INTEGER
   {
      TYPE_LARGE_INTEGER() : QuadPart( 0 ) {}
      TYPE_LARGE_INTEGER( const TYPE_LARGE_INTEGER& obj ) : QuadPart( obj.QuadPart ) {}
      TYPE_LARGE_INTEGER& operator=( const TYPE_LARGE_INTEGER& obj ) { QuadPart = obj.QuadPart; return( *this ); }
      ~TYPE_LARGE_INTEGER() {}

      operator __int64 () const { return( QuadPart ); }

      union
      {
         struct
         {
            uint32_t LowPart;
            int32_t  HighPart;
         } u;

         __int64 QuadPart;
      };
   };

   typedef TYPE_LARGE_INTEGER< __int64 >           LARGE_INTEGER;
   typedef TYPE_LARGE_INTEGER< unsigned __int64 >  ULARGE_INTEGER;

   struct GUID
   {
      unsigned long  Data1;
      unsigned short Data2;
      unsigned short Data3;
      unsigned char  Data4[8];
   };


   // System Constants
   #define GENERIC_READ             0x80000000
   #define GENERIC_WRITE            0x40000000
   #define GENERIC_ALL              0x10000000
   #define CREATE_NEW               0x00000001
   #define CREATE_ALWAYS            0x00000002
   #define OPEN_ALWAYS              0x00000003
   #define OPEN_EXISTING            0x00000004

   #define HEAP_GENERATE_EXCEPTIONS 0

   #define TRUE                     1
   #define FALSE                    0

   #define FILE_SHARE_READ          0
   #define FILE_ATTRIBUTE_NORMAL    0

   #define FILE_CURRENT             SEEK_CUR
   #define FILE_END                 SEEK_END
   #define FILE_BEGIN               SEEK_SET

   #define WAIT_TIMEOUT             1

   #define CP_ACP                   0

   // System errors
   #define ERROR_SUCCESS            0
   #define INVALID_HANDLE_VALUE     NULL
   #define ERROR_NOT_ENOUGH_MEMORY  ENOMEM
   #define STATUS_NO_MEMORY         ENOMEM
   #define ERROR_ACCESS_DENIED      EACCES
   #define ERROR_INVALID_DATA       ENODATA
   #define ERROR_READ_FAULT         EINVAL
   #define ERROR_INVALID_HANDLE     EBADF
   #define ERROR_WRITE_FAULT        EAGAIN


   // System functions
   __forceinline HANDLE GetProcessHeap()
   {
      return( NULL );
   }


   __forceinline DWORD WaitForSingleObject( const HANDLE /*hHandle*/, DWORD /*dwMilliseconds*/ )
   {
      MASSERT( FALSE );

      // BUILD UNIX WRAPPER
      //truct timespec ts;
      //clock_gettime(CLOCK_REALTIME, &ts);
      //ts.tv_sec += 10; // ten seconds
      //while (!some_condition && ret == 0)
      //   ret = pthread_cond_timedwait(&cond, &mutex, &ts);

      return( 0 );
   }


   __forceinline void* HeapAlloc( const HANDLE /*hHeap*/, const DWORD /*dwFlags*/, const size_t dwBytes )
   {
      void* p = malloc( dwBytes );

      if( NULL == p )
      {
         errno = STATUS_NO_MEMORY;

         throw( EXCEPTION_EXECUTE_HANDLER( errno ) );
      }

      return( p );
   }


   __forceinline BOOL HeapFree( const HANDLE /*hHeap*/, const DWORD /*dwFlags*/, void* lpMem )
   {
      free( lpMem );
	  
	  return( TRUE );
   }


   __forceinline void* SecureZeroMemory( void* pMemory, const size_t stBytes )
   {
      memset( pMemory, 0, stBytes );

      return( pMemory );
   }


   __forceinline DWORD GetLastError()
   {
      return( errno );
   }


   __forceinline DWORD GetExceptionCode()
   {
      return( errno );
   }

   
   __forceinline const char* WindowsToUnixFileAccessCode( const DWORD dwDesiredAccess, const DWORD dwCreationDisposition )
   {
      if( ((GENERIC_READ & dwDesiredAccess) && (GENERIC_WRITE & dwDesiredAccess)) || (dwDesiredAccess & GENERIC_ALL) )
      {
         // All access.
         switch( dwCreationDisposition )
         {
         case CREATE_NEW:
         case CREATE_ALWAYS:
            return( "wb+" );

         case OPEN_ALWAYS:
            return( "ab+" );

         case OPEN_EXISTING:
            return( "rb+" );

         default:
            MASSERT( FALSE );
            break;
         }
      }
      else
      {
         if( GENERIC_READ & dwDesiredAccess )
         {
            switch( dwCreationDisposition )
            {
            case OPEN_EXISTING:
               return( "rb" );

            case CREATE_NEW:
            case CREATE_ALWAYS:
            case OPEN_ALWAYS:
            default:
               MASSERT( FALSE );
               break;
            }
         }
         else
         {
            MASSERT( GENERIC_WRITE & dwDesiredAccess );

            switch( dwCreationDisposition )
            {
            case CREATE_NEW:
            case CREATE_ALWAYS:
               return( "wb" );

            case OPEN_ALWAYS:
            case OPEN_EXISTING:
               return( "ab" );

            default:
               MASSERT( FALSE );
               break;
            }
         }
      }
      
      MASSERT( FALSE );
      
      return( "" );
   }

   
   __forceinline HANDLE CreateFile( LPCWSTR lpFileName, const DWORD dwDesiredAccess, const DWORD /*dwShareMode*/, const void* /*lpSecurityAttributes*/, const DWORD dwCreationDisposition, const DWORD /*dwFlagsAndAttributes*/, const HANDLE /*hTemplateFile*/ )
   {
      char LoclaFilename[2*PATH_MAX];
      
      wcstombs( LoclaFilename, (wchar_t*)lpFileName, 2*PATH_MAX );

      return( fopen( LoclaFilename, WindowsToUnixFileAccessCode( dwDesiredAccess, dwCreationDisposition ) ) );
   }

   
   __forceinline HANDLE CreateFile( LPCSTR lpFileName, const DWORD dwDesiredAccess, const DWORD /*dwShareMode*/, const void* /*lpSecurityAttributes*/, const DWORD dwCreationDisposition, const DWORD /*dwFlagsAndAttributes*/, const HANDLE /*hTemplateFile*/ )
   {
      return( fopen( lpFileName, WindowsToUnixFileAccessCode( dwDesiredAccess, dwCreationDisposition ) ) );
   }
   

   __forceinline BOOL GetFileSizeEx( const HANDLE hFile, LARGE_INTEGER* pilFileSize )
   {
      if( NULL == hFile || NULL == pilFileSize )
      {
         errno = EINVAL;

         return( FALSE );
      }
      
      LARGE_INTEGER liCurrent;
      liCurrent.QuadPart = ftell( (FILE*)hFile );

      if( 0 != fseek( (FILE*)hFile, 0L, SEEK_END ) )
      {
         errno = ferror( (FILE*)hFile );
         
         return( FALSE );
      }

      pilFileSize->QuadPart = ftell( (FILE*)hFile );

      if( 0 != fseek( (FILE*)hFile, liCurrent.u.LowPart, SEEK_SET ) )
      {
         errno = ferror( (FILE*)hFile );
         
         return( FALSE );
      }

      return( TRUE );
   }


   __forceinline BOOL ReadFile( const HANDLE hFile, void* pReadDataBytes, const DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, void* /*lpOverlapped*/ )
   {
      fflush( (FILE*)hFile );

      const DWORD dwBytesRead = fread( pReadDataBytes, sizeof( BYTE ), nNumberOfBytesToRead, (FILE*)hFile );

      if( NULL != lpNumberOfBytesRead )
      {
         *lpNumberOfBytesRead = dwBytesRead;
      }

      if( dwBytesRead != nNumberOfBytesToRead )
      {
         return( feof( (FILE*)hFile ) );
      }

      return( TRUE );
   }


   __forceinline BOOL SetFilePointerEx( const HANDLE hFile, LARGE_INTEGER liDistanceToMove, LARGE_INTEGER* lpNewFilePointer, DWORD dwMoveMethod )
   {
      MASSERT( 0 == liDistanceToMove.HighPart );

      if( 0 != fseek( (FILE*)hFile, liDistanceToMove.u.LowPart, dwMoveMethod ) )
      {
         return( FALSE );
      }

      if( NULL != lpNewFilePointer )
      {
         lpNewFilePointer->u.HighPart = 0;
         lpNewFilePointer->u.LowPart  = ftell( (FILE*)hFile );

         if( (uint32_t)-1 == lpNewFilePointer->u.LowPart )
         {
            lpNewFilePointer->u.LowPart = 0;

            return( FALSE );
         }
      }

      return( TRUE );
   }


   __forceinline BOOL WriteFile( const HANDLE hFile, void* pWriteDataBytes, const DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, void * /*lpOverlapped*/ )
   {
      fflush( (FILE*)hFile );

      const DWORD dwFilesWritten = fwrite( pWriteDataBytes, 1, nNumberOfBytesToWrite, (FILE*)hFile );

      if( NULL != lpNumberOfBytesWritten )
      {
         *lpNumberOfBytesWritten = dwFilesWritten;
      }

      return( nNumberOfBytesToWrite == dwFilesWritten ? TRUE : FALSE );
   }


   __forceinline int _vscprintf( const char *format, va_list argptr )
   {
      return( vsnprintf( 0, 0, format, argptr ) );
   }


   __forceinline int vsprintf_s( char *buffer, size_t numberOfElements, const char *format, va_list argptr )
   {
      return( vsnprintf( buffer, numberOfElements, format, argptr ) );
   }


   __forceinline int vswprintf_s( wchar_t *buffer, size_t numberOfElements, const wchar_t *format, va_list argptr )
   {
      return( vswprintf( buffer, numberOfElements, format, argptr ) );
   }


   __forceinline int _strlwr_s( char *str, size_t /*numberOfElements*/ )
   {
      char* p = str;
      
      while( 0 != (*p = tolower( *p )) )
      {
         p++;
      }

      return( 0 );
   }


   __forceinline int _wcslwr_s( wchar_t *str, size_t /*numberOfElements*/ )
   {
      wchar_t* p = str;
      
      while( 0 != (*p = towlower( *p )) )
      {
         p++;
      }

      return( 0 );
   }


   __forceinline int _strupr_s( char *str, size_t /*numberOfElements*/ )
   {
      char* p = str;
      
      while( 0 != (*p = toupper( *p )) )
      {
         p++;
      }

      return( 0 );
   }


   __forceinline int _wcsupr_s( wchar_t *str, size_t /*numberOfElements*/ )
   {
      wchar_t* p = str;
      
      while( 0 != (*p = towupper( *p )) )
      {
         p++;
      }

      return( 0 );
   }


   __forceinline void _ctime64_s( char* buffer, size_t numberOfElements, const __time64_t *time )
   {
      memcpy( buffer, ctime( time ), numberOfElements );
   }

   
   __forceinline void _wctime64_s( wchar_t* buffer, size_t numberOfElements, const __time64_t *time )
   {
      char charTime[numberOfElements];
     
      _ctime64_s( charTime, numberOfElements, time );
     
      mbstowcs( buffer, charTime, numberOfElements );
   }


   __forceinline int _ui64tow_s( unsigned __int64 value, wchar_t *buffer, size_t sizeInCharacters, int radix )
   {
      MASSERT( 10 == radix );

      swprintf( buffer, sizeInCharacters, L"%llu", value );

      return( 0 );
   }


   __forceinline int _ui64toa_s( unsigned __int64 value, char *buffer, size_t sizeInCharacters, int radix )
   {
      MASSERT( 10 == radix );

      snprintf( buffer, sizeInCharacters, "%llu", value );

      return( 0 );
   }


   __forceinline int _i64tow_s( __int64 value, wchar_t *buffer, size_t sizeInCharacters, int radix )
   {
      MASSERT( 10 == radix );

      swprintf( buffer, sizeInCharacters, L"%lld", value );

      return( 0 );
   }


   __forceinline int _i64toa_s( __int64 value, char *buffer, size_t sizeInCharacters, int radix )
   {
      MASSERT( 10 == radix );

      snprintf( buffer, sizeInCharacters, "%lld", value );

      return( 0 );
   }


   __forceinline __int64 _wcstoi64( const wchar_t *nptr, wchar_t **endptr, int base )
   {
      return( wcstoll( nptr, endptr, base ) );
   }

   
   __forceinline __int64 _strtoi64( const char *nptr, char **endptr, int base )
   {
      return( strtoll( nptr, endptr, base ) );
   }


   __forceinline unsigned __int64 _wcstoui64( const wchar_t *nptr, wchar_t **endptr, int base )
   {
      return( wcstoull( nptr, endptr, base ) );
   }


   __forceinline unsigned __int64 _strtoui64( const char *nptr, char **endptr, int base )
   {
      return( strtoull( nptr, endptr, base ) );
   }
#endif
