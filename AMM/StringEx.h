//
//
// StringEx.h: memory atom string class derived from more generic type of memory atom, part of:
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase One'
//
// © Copyright 2001 - 2014 by Miroslav Bonchev Bonchev. All rights reserved.
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


#include "MAtom.h"
#include "MList.h"
#include <math.h>
#include <time.h>
#include <float.h>
#include "ResId.h"
#include <stdio.h>

#ifdef _WIN32
   #include "oleauto.h"
   #include "shlobj.h"
   #include "wininet.h"
   #include "atlbase.h"
   #include "atltypes.h"
   #include "shobjidl_core.h"
#endif


#ifdef _WIN32
   #pragma warning( push )
   #pragma warning( disable : 4290 )   // warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#endif


#ifdef _WIN32
   class MBStr
   {
   private:
      BSTR bstr;

   public:
      MBStr() throw()
      {
         bstr = SysAllocString( L"" );
      }

      MBStr( const OLECHAR* ochText ) throw()
      {
         bstr = SysAllocString( ochText );
      }

      ~MBStr()
      {
         SysFreeString( bstr );
      }

      MBStr operator=( const BSTR bstrText )
      {
         SysFreeString( bstr );

         bstr = SysAllocString( bstrText );

         return( *this );
      }

      MBStr operator=( const MBStr& bstrText )
      {
         SysFreeString( bstr );

         bstr = SysAllocString( bstrText.bstr );

         return( *this );
      }

      operator const wchar_t*() const throw() { return(  bstr ); }
      operator const BSTR()     const throw() { return(  bstr ); }

      operator BSTR*()                throw() { return( &bstr ); }
      operator const BSTR*()    const throw() { return( &bstr ); }

      operator BSTR&()                throw() { return( bstr ); }
      operator const BSTR&()    const throw() { return( bstr ); }

      UINT GetLength() const throw()
      {
         return( SysStringLen( bstr ) );
      }
   };


   extern DWORD dwDefaultLanguageId;
#endif


#define MSTRING_ITEM_NOT_FOUND (MUI)-1


template< class tChar > class MStringEx : public MemoryPH< tChar >
{
public:
   enum NumericBase
   {
      Binary      = 2,
      Octal       = 8,
      Decimal     = 10,
      Hexadecimal = 16,
   };

   mutable NumericBase eNumericBase;

public:
   enum StringFromMemory{ SFM };
   enum FromFormat{ FF };

#ifdef _WIN32
   enum TypeSystemError { eGetSystemError };
   enum TypeInternetError { eGetInternetError };
   enum FromTypeBSTR{ FromBSTR };
   enum StringFromMuduleInfo{ SFMI };
#endif


public:
   MStringEx()
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;
   }


   MStringEx( const MStringEx< tChar > &strMStringEx )
      :  MemoryPH< tChar >( strMStringEx ),
         eNumericBase( strMStringEx.eNumericBase )
   {
   }


   // If false == bSearchForZeroEnd - then the constructor presumes that the memory IS NOT ZERO terminated, so a zero terminator will be added.
   MStringEx( StringFromMemory, const ShellMemory< tChar > maString, const bool bSearchForZeroEnd )
      :  MemoryPH< tChar >( bSearchForZeroEnd ? MUnit< tChar >( 0 ) : maString.GetSize() + 1 ),
         eNumericBase( Decimal )
   {
      if( bSearchForZeroEnd )
      {
         for( MUnit< tChar > muChar; muChar < maString.GetSize(); muChar++ )
         {
            if( 0 == maString[muChar] )
            {
               muChar++;

               ReAllocate( muChar );
               
               Transfer( maString.SubMemory( MUnit< tChar >( 0 ), muChar ) );

               return;
            }
         }

         ReAllocate( maString.GetSize() + 1 );
      }

      Transfer( maString );

      this->pData[maString.GetSize().GetUnits()] = 0;
   }


   // If false == bSearchForZeroEnd - then the constructor presumes that the memory IS NOT ZERO terminated, so a zero terminator will be added.
   MStringEx( StringFromMemory, const MAtom< tChar >& maString, const bool bSearchForZeroEnd )
      :  MemoryPH< tChar >( bSearchForZeroEnd ? MUnit< tChar >( 0 ) : maString.GetSize() + 1 ),
         eNumericBase( Decimal )
   {
      if( bSearchForZeroEnd )
      {
         for( MUnit< tChar > muChar; muChar < maString.GetSize(); muChar++ )
         {
            if( 0 == maString[muChar] )
            {
               muChar++;

               ReAllocate( muChar );
               
               Transfer( maString.SubMemory( MUnit< tChar >( 0 ), muChar ) );

               return;
            }
         }

         ReAllocate( maString.GetSize() + 1 );
      }

      Transfer( maString );

      this->pData[maString.GetSize().GetUnits()] = 0;
   }


   MStringEx( const tChar tchChar )
      :  MemoryPH< tChar >( MUnit< tChar >( 0 == tchChar ? 1 : 2 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = tchChar;

      if( 0 != tchChar )
      {
         *(this->pData + 1) = 0;
      }
   }


   MStringEx( const tChar *pchString )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      if( NULL != pchString )
      {
         MUI muiIndex = 0;

         for( ; 0 != pchString[muiIndex]; muiIndex++ );

         this->ReAllocateTransfer( ShellMemory< tChar >( (tChar*)pchString, MUnit< tChar >( muiIndex + 1 ) ) );
      }
   }


#ifdef _WIN32
   MStringEx( const FromTypeBSTR, const BSTR bstrString )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      if( NULL != bstrString )
      {
         MStringEx< wchar_t > strThis( MStringEx< wchar_t >::SFM, ShellMemory< wchar_t >( (wchar_t*)bstrString, MUnit< wchar_t >( SysStringLen( bstrString ) ) ), false );

         if( IsWchar() )
         {
            this->ExchangeMemory< wchar_t >( strThis );
         }
         else
         {
            MStringEx< char > strThisChar( strThis.GetAsMultiByte() );

            this->ExchangeMemory< char >( strThisChar );
         }
      }
   }
#endif


   MStringEx( const FromFormat, const tChar *pchSpecifier, ... )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      MASSERT( NULL != pchSpecifier );

      if( NULL != pchSpecifier )
      {
         va_list arglist;

#ifdef _WIN32
         va_start( arglist, pchSpecifier );
         ReAllocate( MUnit< tChar >( (IsWchar() ? _vscwprintf( (const wchar_t*)pchSpecifier, arglist ) : _vscprintf( (const char*)pchSpecifier, arglist )) + 1 ) );
         va_end( arglist );
#else
         MUnit< tChar > muRequredSize;
         va_start( arglist, pchSpecifier );
         if( IsWchar() ) {
            int lenTest = 1024;
            wchar_t *p = new wchar_t[lenTest];
            while( (MUI)-1 == (muRequredSize = MUnit< tChar >( vswprintf( p, lenTest, (const wchar_t*)pchSpecifier, arglist ) )) ) {
               va_end( arglist );
               va_start( arglist, pchSpecifier );
               delete[] p;
               lenTest += 1024;
               p = new wchar_t[lenTest]; }
            delete[] p; }
         else { muRequredSize = MUnit< tChar >( _vscprintf( (const char*)pchSpecifier, arglist ) ); }
         va_end( arglist );
         this->ReAllocate( muRequredSize + 1 );
#endif      

         va_start( arglist, pchSpecifier );
         IsWchar() ? vswprintf_s( (wchar_t*)this->pData, this->muSize.GetUnits(), (const wchar_t*)pchSpecifier, arglist ) : vsprintf_s( (char*)this->pData, this->muSize.GetUnits(), (const char*)pchSpecifier, arglist );
         va_end( arglist );
      }
   }


#ifdef _WIN32
   MStringEx( ResId ResId )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      LoadString( ResId, hDefaultTextResourceModule, 0 );
   }


   MStringEx( ResId ResId, HMODULE hmModule )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      LoadString( ResId, hmModule, 0 );
   }


   MStringEx( ResId ResId, HMODULE hmModule, WORD wLanguage )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      LoadString( ResId, hmModule, wLanguage );
   }


   MStringEx( const FromFormat, const ResId wrSpecifier, ... )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      const MStringEx< tChar > strSpecifier( wrSpecifier );

      va_list arglist;
      va_start( arglist, wrSpecifier );
      ReAllocate( MUnit< tChar >( (IsWchar() ? _vscwprintf( (const wchar_t*)strSpecifier.pData, arglist ) : _vscprintf( (const char*)strSpecifier.pData, arglist )) + 1 ) );
      va_end( arglist );
         
      va_start( arglist, wrSpecifier );
      IsWchar() ? vswprintf_s( (wchar_t*)this->pData, this->muSize.GetUnits(), (const wchar_t*)strSpecifier.pData, arglist ) : vsprintf_s( (char*)this->pData, this->muSize.GetUnits(), (const char*)strSpecifier.pData, arglist );
      va_end( arglist );
   }


   MStringEx( StringFromMuduleInfo, const LPCTSTR strModuleInfo, HMODULE hModule )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;


      MemoryPH< TCHAR > memModuleFileName( MUnit< TCHAR >( 40000 ) );
      GetModuleFileName( hModule, memModuleFileName, memModuleFileName.GetSize().GetUnits32() );


      DWORD dwHandle( 0 );
      MemoryPH< BYTE >  memVersionInfo( MUnit< BYTE >( GetFileVersionInfoSize( memModuleFileName, &dwHandle ) ) );
      

      if( 0 == memVersionInfo.GetSize() )
      {
         throw( MStringExException( MStringExException::CouldNotLoadResource ) );
      }


      if( !GetFileVersionInfo( memModuleFileName, 0, memVersionInfo.GetSize().InBytes32(), memVersionInfo.GetMemory() ) )
      {
         throw( MStringExException( MStringExException::CouldNotLoadResource ) );
      }


      // Structure used to store enumerated languages and code pages.
      struct LANGANDCODEPAGE
      {
         WORD wLanguage;
         WORD wCodePage;
      } *lpTranslate;

      
      UINT cbTranslate;


      // Read the list of languages and code pages.
      VerQueryValue( memVersionInfo.GetMemory(), TEXT( "\\VarFileInfo\\Translation" ), (LPVOID*)&lpTranslate, &cbTranslate );

      // Read the file description for each language and code page.
      for( DWORD dwIndexCP = 0; dwIndexCP < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); dwIndexCP++ )
      {
         UINT     uiBytes;
         LPVOID   lpBuffer;

         MStringEx< TCHAR > strDataRequest( MStringEx< TCHAR >::FF, TEXT( "\\StringFileInfo\\%04x%04x\\%s" ), lpTranslate[dwIndexCP].wLanguage, lpTranslate[dwIndexCP].wCodePage, strModuleInfo );

         if( VerQueryValue( memVersionInfo.pData, strDataRequest, &lpBuffer, &uiBytes ) )
         {
            *this = MStringEx< TCHAR >( (LPCTSTR)lpBuffer ).GetAs< tChar >();

            return;
         }
      }

      throw( MStringExException( MStringExException::CouldNotLoadResource ) );
   }
#endif


   MStringEx( const MHandle& hStream )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;


      if( !hStream.IsValid() )
      {
         MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
      }


      MemoryPH< tChar > memString( MUnit< tChar >( 8192 ) );
      DWORD dwBytesRead( 0 );

      while( ReadFile( hStream, (void*)memString.GetMemory(), memString.GetSize().InBytes32(), &dwBytesRead, NULL ) )
      {
         if( 0 == dwBytesRead )
         {
            return;
         }


         MStringEx< tChar > strString( MStringEx< tChar >::SFM, memString, true );

         if( 0 == strString.GetLength() )
         {
            return;
         }


         *this += strString;
      }
   }


protected:
   // The value is the length of the string without the ending zero, so for empty string ask for 0.
   MStringEx( const MUnit< tChar >& mutChars )
      :  MemoryPH< tChar >( mutChars + 1 ),
         eNumericBase( Decimal )
   {
      *(this->pData + GetLength()) = 0;
   }

   
public:
   // The value is the length of the string without the ending zero, so for empty string ask for 0.
   // tchInitialFillUp must be a valid character and must NOT be zero.
   MStringEx( const MUnit< tChar >& mutChars, const tChar tchInitialFillUp )
      :  MemoryPH< tChar >( mutChars + 1, 0 != tchInitialFillUp ? tchInitialFillUp : 1 ),
         eNumericBase( Decimal )
   {
      MASSERT( 0 != tchInitialFillUp );

      *(this->pData + GetLength()) = 0;
   }


#ifdef _WIN32
   MStringEx( const HWND hwWindow )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      DWORD_PTR dwPtr( 0 );

      if( IsWchar() )
      {
         if( ::SendMessageTimeoutW( hwWindow, WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG, 100, &dwPtr ) )
         {
            ReAllocate( MUnit< tChar >( (MUI)dwPtr + 1 ) );

            ::SendMessageTimeoutW( hwWindow, WM_GETTEXT, this->muSize.GetUnits(), (LPARAM)this->pData, SMTO_ABORTIFHUNG, 100, &dwPtr );
         }

         return;
      }


      if( ::SendMessageTimeoutA( hwWindow, WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG, 100, &dwPtr ) )
      {
         ReAllocate( MUnit< tChar >( (MUI)dwPtr + 1 ) );

         ::SendMessageTimeoutA( hwWindow, WM_GETTEXT, this->muSize.GetUnits(), (LPARAM)this->pData, SMTO_ABORTIFHUNG, 100, &dwPtr );
      }
   }


   MStringEx( TypeSystemError, const DWORD dwLastError )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;
      tChar* wstrMessage( NULL );

      if( 0 != ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwLastError, dwDefaultLanguageId, (tChar*)&wstrMessage, 0, NULL ) )
      {
         *this = MStringEx< wchar_t >( wstrMessage ).GetAs< tChar >();

         LocalFree( wstrMessage );

         Trim();

         return;
      }

      if( ERROR_RESOURCE_LANG_NOT_FOUND == ::GetLastError() )
      {
         if( 0 != ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwLastError, 0, (tChar*)&wstrMessage, 0, NULL ) )
         {
            *this = MStringEx< wchar_t >( wstrMessage ).GetAs< tChar >();

            LocalFree( wstrMessage );

            Trim();

            return;
         }
      }

      *this = MStringEx< wchar_t >( L"Unknown." ).GetAs< tChar >();
   }


   MStringEx( TypeSystemError )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      *this = MStringEx< tChar >( MStringEx< tChar >::eGetSystemError, ::GetLastError() );
   }


#ifdef _WIN32
   MStringEx( TypeInternetError, const DWORD dwLastError )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;
      LPWSTR wstrMessage( NULL );

      if( 0 != ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwLastError, dwDefaultLanguageId, (LPTSTR)&wstrMessage, 0, NULL ) )
      {
         *this = MStringEx< wchar_t >( wstrMessage ).GetAs< tChar >();

         LocalFree( wstrMessage );

         Trim();

         return;
      }

      if( ERROR_RESOURCE_LANG_NOT_FOUND == ::GetLastError() )
      {
         if( 0 != ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwLastError, 0, (LPTSTR)&wstrMessage, 0, NULL ) )
         {
            *this = MStringEx< wchar_t >( wstrMessage ).GetAs< tChar >();

            LocalFree( wstrMessage );

            Trim();

            return;
         }
      }


      DWORD dwError;
      DWORD dwBufferLength( 0 );

      InternetGetLastResponseInfo( &dwError, NULL, &dwBufferLength );

      if( 0 != dwBufferLength )
      {
         MemoryPH< TCHAR > memText( MUnit< TCHAR >( (MUI)(dwBufferLength + 1) ) );

         dwBufferLength = memText.GetSize().GetUnits32();

         if( InternetGetLastResponseInfo( &dwError, memText.GetMemory(), &dwBufferLength ) )
         {
            *this = memText;

            this->Trim();

            return;
         }
      }


      *this = MStringEx< wchar_t >( L"Unknown." ).GetAs< tChar >();
   }
#endif


   MStringEx( TypeInternetError )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      *this = MStringEx< tChar >( MStringEx< tChar >::eGetInternetError, ::GetLastError() );
   }


   MStringEx( const GUID& guid )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      LPOLESTR lpGuid = NULL;

      StringFromCLSID( guid, &lpGuid );
      
      *this = MStringEx< wchar_t >( lpGuid ).GetAs< tChar >();

      CoTaskMemFree( lpGuid );
   }
#endif


   MStringEx( const __time64_t& tmTime )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ) ),
         eNumericBase( Decimal )
   {
      *this->pData = 0;

      tChar tchTime[26];

      if( IsWchar() )
      {
         _wctime64_s( (wchar_t*)tchTime, sizeof( tchTime ) / sizeof( tChar ), &tmTime );
      }
      else
      {
         _ctime64_s( (char*)tchTime, sizeof( tchTime ) / sizeof( tChar ), &tmTime );
      }

      *this = tchTime;
   }


   enum FormattedUnsignedInteger
   {
      NumberNoCommas,
      NumberWithCommas
   };


   MStringEx( const FormattedUnsignedInteger eFormattedUnsignedInteger, const unsigned __int64 ui64Number, const NumericBase eNumericBase )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ), 0 ),
         eNumericBase( eNumericBase )
   {
      tChar ch[65];

      if( IsWchar() )
      {
         _ui64tow_s( ui64Number, (wchar_t*)ch, 65, eNumericBase );
      }
      else
      {
         _ui64toa_s( ui64Number, (char*)ch, 65, eNumericBase );
      }

      *this = ch;

      if( NumberNoCommas == eFormattedUnsignedInteger )
      {
         return;
      }

      this->InsertRepeatedly( TEXT(","), 3, false );
   }


   enum FormattedSignedInteger
   {
      SignedNumberNoCommas,
      SignedNumberWithCommas
   };


   MStringEx( const FormattedSignedInteger eFormattedSignedInteger, const __int64 i64Number, const NumericBase eNumericBase )
      :  MemoryPH< tChar >( MUnit< tChar >( 1 ), 0 ),
         eNumericBase( eNumericBase )
   {
      tChar ch[65];

      if( IsWchar() )
      {
         _i64tow_s( i64Number, (wchar_t*)ch, 65, eNumericBase );
      }
      else
      {
         _i64toa_s( i64Number, (char*)ch, 65, eNumericBase );
      }

      *this = ch;

      if( SignedNumberNoCommas == eFormattedSignedInteger )
      {
         return;
      }

      *this = ((i64Number < 0 ? TEXT("-") : TEXT("")) + this->TrimLeft( TCHAR('-') ).InsertRepeatedly( TEXT(","), 3, false ));
   }


   virtual ~MStringEx()
   {
   }


   __forceinline bool IsWchar() const throw()
   {
      #pragma warning( push )
      #pragma warning( disable : 4127 )

      return( sizeof( tChar ) == sizeof( wchar_t ) );

      #pragma warning( pop )
   }

   
   ShellMemory< tChar > GetPureStringContent()
   {
      return( ShellMemory< tChar >( this->pData, this->muSize - 1 ) );
   }


   template< class ExportCharType > MStringEx< ExportCharType > GetAs() const throw()
   {
      #pragma warning( push )
      #pragma warning( disable : 4127 )

      if( sizeof( tChar ) == sizeof( ExportCharType ) )
      {
         // The object is already this type.
         return( MStringEx< ExportCharType >( (ExportCharType*)this->GetMemory() ) );
      }
 
      #pragma warning( pop )


      // The object is from the other type, so return the other type.
      if( IsWchar() )
      {
         return( MStringEx< ExportCharType >( (ExportCharType*)GetAsMultiByte().GetMemory() ) );
      }


      return( MStringEx< ExportCharType >( (ExportCharType*)GetAsUnicode().GetMemory() ) );
   }


   MStringEx< wchar_t > GetAsUnicode( UINT uiCodePage = CP_ACP ) const throw()
   {
      if( IsWchar() )
      {
         // The object is Unicode itself.
         return( MStringEx< wchar_t >( (wchar_t*)this->pData ) );
      }

#ifdef _WIN32
      if( !IsValidCodePage( uiCodePage) )
      {
         uiCodePage = CP_ACP;
      }


      MUnit< wchar_t > muSizeRequired;


      BYTE* ptrDataCopy( (BYTE*)this->pData );
      const BYTE* ptrLastData( (BYTE*)((MUI)this->pData + this->muSize.InBytes()) );


      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
      {
         muSizeRequired += MUnit< wchar_t >( MultiByteToWideChar( uiCodePage, 0, (LPCSTR)ptrDataCopy, dwChunkSize, NULL, 0 ) );
      }


      MStringEx< wchar_t > strString( muSizeRequired - 1 );
      muSizeRequired = 0;
      ptrDataCopy = (BYTE*)this->pData;


      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
      {
         wchar_t* pNewString = &strString[muSizeRequired.GetUnits()];

         muSizeRequired += MUnit< wchar_t >( MultiByteToWideChar( uiCodePage, 0, (LPCSTR)ptrDataCopy, dwChunkSize, pNewString, MMIN< MUnit< wchar_t >, MUnit< wchar_t >, MUnit< wchar_t > >( strString.GetSize() - muSizeRequired, MUnit< wchar_t >( dwChunkSize ) ).GetUnits32() ) );
      }


      return( strString );
#else
      MUnit< BYTE > muLength( (sizeof( wchar_t ) / sizeof( char )) * this->GetLength() );
      
      MStringEx< wchar_t > strString( muLength.template As< wchar_t >(), 0 );
      
      mbstowcs( strString.GetMemory(), (const char*)this->pData, strString.GetSize().GetUnits() );
      
      strString.GetMemory()[strString.GetSize().GetUnits() - 1] = 0;
      
      return( strString );
#endif
   }


   MStringEx< char > GetAsMultiByte( UINT uiCodePage = CP_ACP ) const throw()
   {
      if( !IsWchar() )
      {
         // The object is multi-byte itself.
         return( MStringEx< char >( (char*)this->pData ) );
      }

#ifdef _WIN32
      if( !IsValidCodePage( uiCodePage) )
      {
         uiCodePage = CP_ACP;
      }


      MUnit< char > muSizeRequired;


      BYTE* ptrDataCopy( (BYTE*)this->pData );
      const BYTE* ptrLastData( (BYTE*)((MUI)this->pData + this->muSize.InBytes()) );


      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
      {
         muSizeRequired += MUnit< char >( WideCharToMultiByte( uiCodePage, 0, (LPCWSTR)ptrDataCopy, dwChunkSize / 2, NULL, 0, NULL, NULL ) );
      }


      MStringEx< char > strString( muSizeRequired - 1, 0x20 );
      muSizeRequired = 0;
      ptrDataCopy = (BYTE*)this->pData;


      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
      {
         char* pNewString = &strString[muSizeRequired.GetUnits()];

         muSizeRequired += MUnit< char >( WideCharToMultiByte( uiCodePage, 0, (LPCWSTR)ptrDataCopy, dwChunkSize / 2, pNewString, MMIN< MUnit< char >, MUnit< char >, MUnit< char > >( strString.GetSize() - muSizeRequired, MUnit< char >( dwChunkSize / 2 ) ).GetUnits32(), NULL, NULL ) );
      }


      return( strString );
#else
      MStringEx< char > strString( MUnit< char >( this->GetLength() ), 0 );
      
      wcstombs( strString.GetMemory(), (const wchar_t*)this->pData, strString.GetSize().GetUnits() );
      
      strString.GetMemory()[strString.GetSize().GetUnits() - 1] = 0;
      
      return( strString );
#endif
   }


   MStringEx< tChar >& Reverse() throw()
   {
      for( MUI muiIndex = 0; muiIndex < GetLength() / 2; muiIndex++ )
      {
         const tChar tcBufferedChar( this->pData[muiIndex] );

         this->pData[muiIndex] = this->pData[GetLength() - muiIndex - 1];
         this->pData[GetLength() - muiIndex - 1] = tcBufferedChar;
      }

      return( *this );
   }


   MStringEx< tChar >& Replace( const MUI muiIndex, const tChar tcNewChar ) throw()
   {
      MASSERT( muiIndex < GetLength() );

      this->pData[muiIndex] = tcNewChar;

      return( *this );
   }


   MStringEx< tChar >& Replace( const tChar tcTargetChar, const tChar tcNewChar ) throw()
   {
      for( MUI muiIndex = 0; muiIndex < GetLength(); muiIndex++ )
      {
         if( tcTargetChar == this->pData[muiIndex] )
         {
            this->pData[muiIndex] = tcNewChar;
         }
      }

      return( *this );
   }


   MStringEx< tChar >& SwapChars( const MUI muiIndexCharA, const MUI muiIndexCharB ) throw()
   {
      if( muiIndexCharA != muiIndexCharB )
      {
         if( GetLength() <= MMAX< MUI, MUI, MUI >( muiIndexCharA, muiIndexCharB ) )
         {
            MASSERT( FALSE );
      
            return( *this );
         }

         const tChar tcCopy = this->pData[muiIndexCharA];

         this->pData[muiIndexCharA] = this->pData[muiIndexCharB];
         this->pData[muiIndexCharB] = tcCopy;
      }

      return( *this );
   }


   MList< MStringEx< tChar > > Split( const MStringEx< tChar > strSplitBy, const bool bCaseSensitive, const bool bNoSplitMarkers ) const throw()
   {
      MList< MStringEx< tChar > > listResult;
      const MUI muiSplitLen( strSplitBy.GetLength() );
      
      // If empty split pattern - split each char
      if( 0 == muiSplitLen )
      {
         for( MUI muiIndex = 0; muiIndex < GetLength(); muiIndex++ )
         {
            listResult.AddTail( new MStringEx< tChar >( this->pData[muiIndex] ) );

            if( !bNoSplitMarkers )
            {
               listResult.AddTail( new MStringEx< tChar >() );
            }
         }

         return( listResult );
      }


      // Not an empty Split pattern - Append the split pattern at the end of the string to simplyfy the processing.         
      MStringEx< tChar > strThis( *this + strSplitBy );

      MUI muiOld = 0;
      MUI muiNew = 0;

      while( MSTRING_ITEM_NOT_FOUND != (muiNew = strThis.Find_L2R( strSplitBy, muiOld, bCaseSensitive )) )
      {
         const MUI muiLength = muiNew - muiOld;

         if( 0 != muiLength )
         {
            listResult.AddTail( new MStringEx< tChar >( strThis.Mid( muiOld, muiLength ) ) );
         }

         if( !bNoSplitMarkers )
         {
            listResult.AddTail( new MStringEx< tChar >() );
         }

         muiOld = muiNew + muiSplitLen; 
      }

      // Remove the last split marker which I added at the top.
      if( !bNoSplitMarkers )
      {
         delete listResult.RemoveTail();
      }

      return( listResult );
   }


   MStringEx< tChar >& Replace( const MStringEx< tChar >& strOld, const MStringEx< tChar >& strNew, const bool bCaseSensitive ) throw()
   {
      MList< MStringEx< tChar > > listAllChunks( Split( strOld, bCaseSensitive, false ) );

      MStringEx< tChar > strThisNew;

      for( MUI muiIndex = 0; muiIndex < listAllChunks.GetCount(); muiIndex++ )
      {
         if( 0 != listAllChunks[muiIndex]->GetLength() )
         {
            // This is a string - put the original string.
            strThisNew += *listAllChunks[muiIndex];
         }
         else
         {
            // This is a split marker - put the new string.
            strThisNew += strNew;
         }
      }

      if( MSTRING_ITEM_NOT_FOUND != strThisNew.Find_L2R( strOld, 0, bCaseSensitive ) )
      {
         // TEST - why do I need this if-block - it is liable to create lock & crash.
         MASSERT( FALSE );

         return( *this = strThisNew.Replace( strOld, strNew, bCaseSensitive ) );
      }

      this->ExchangeMemory( strThisNew );

      return( *this );
   }


   MStringEx< tChar >& Remove( const tChar tcRemove ) throw()
   {
      const MUI muiChars( GetLength() );

      MUI muiSource( 0 );
      MUI muiDestination( 0 );

      for( ; muiSource < muiChars; muiSource++ )
      {
         if( tcRemove != this->pData[muiSource] )
         {
            this->pData[muiDestination] = this->pData[muiSource];
            muiDestination++;
         }
      }

      if( muiDestination != muiSource )
      {
         this->pData[muiDestination] = 0;

         LimitSizeTo( MUnit< tChar >( muiDestination + 1 ) );

         MASSERT( GetLength() == FindLength() );
      }

      return( *this );
   }


   MStringEx< tChar >& RemoveChars( const MStringEx< tChar >& strChars2Remove, const MUI muiStartPos, const MUI muiCharsCount ) throw()
   {
      MStringEx< tChar > strNewString( MUnit< tChar >( this->GetLength() ) );

      MUI muiDstIndex = 0;
      const MUI muiChars2RemoveLength( strChars2Remove.GetLength() );
      const unsigned __int64 uiLastCharPos( (unsigned __int64)muiStartPos + (unsigned __int64)muiCharsCount );


      for( MUI muiThisIndex = 0; muiThisIndex < GetLength(); muiThisIndex++ )
      {
         bool bRemove = false;

         if( (muiStartPos <= muiThisIndex) && (muiThisIndex < uiLastCharPos) )
         {
            for( MUI muiChars = 0; muiChars < muiChars2RemoveLength; muiChars++ )
            {
               if( strChars2Remove[muiChars] == this->pData[muiThisIndex] )
               {
                  bRemove = true;

                  break;
               }
            }
         }

         if( bRemove )
         {
            continue;
         }

         strNewString.pData[muiDstIndex++] = this->pData[muiThisIndex];
      }

      strNewString.pData[muiDstIndex] = 0;

      strNewString.LimitSizeTo( MUnit< tChar >( muiDstIndex + 1 ) );

      MASSERT( strNewString.GetLength() == strNewString.FindLength() );

      this->ExchangeMemory( strNewString );

      return( *this );
   }


   MAtom< tChar >& Empty()
   {
      *this = MStringEx< tChar >();

      return( *this );
   }


   MStringEx< tChar > &Format( const tChar *pchSpecifier, ... )
   {
      MASSERT( NULL != pchSpecifier );

      if( NULL != pchSpecifier )
      {
         va_list arglist;

#ifdef _WIN32
         va_start( arglist, pchSpecifier );
         ReAllocate( MUnit< tChar >( (IsWchar() ? _vscwprintf( (const wchar_t*)pchSpecifier, arglist ) : _vscprintf( (const char*)pchSpecifier, arglist )) + 1 ) );\
         va_end( arglist );
#else
         MUnit< tChar > muRequredSize;
         va_start( arglist, pchSpecifier );
         if( IsWchar() ) {
            int lenTest = 1024;
            wchar_t *p = new wchar_t[lenTest];
            while( (MUI)-1 == (muRequredSize = MUnit< tChar >( vswprintf( p, lenTest, (const wchar_t*)pchSpecifier, arglist ) )) ) {
               va_end( arglist );
               va_start( arglist, pchSpecifier );
               delete[] p;
               lenTest += 1024;
               p = new wchar_t[lenTest]; }
            delete[] p; }
         else { muRequredSize = MUnit< tChar >( _vscprintf( (const char*)pchSpecifier, arglist ) ); }
         va_end( arglist );
         this->ReAllocate( muRequredSize + 1 );
#endif      

         va_start( arglist, pchSpecifier );
         IsWchar() ? vswprintf_s( (wchar_t*)this->pData, this->muSize.GetUnits(), (const wchar_t*)pchSpecifier, arglist ) : vsprintf_s( (char*)this->pData, this->muSize.GetUnits(), (const char*)pchSpecifier, arglist );
         va_end( arglist );
      }
      else
      {
         Empty();
      }

      return( *this );
   }


   bool Ascii2Bin( MemoryPH< BYTE >& memTarget ) const
   {
      if( 0 != GetLength() % 2 )
      {
         return( false );
      }

      MStringEx< char > strSource( GetAsMultiByte().ToUpper() );

      memTarget.ReAllocate( GetLength() / 2 );

      for( MUI muiIndex = 0; muiIndex < strSource.GetLength(); muiIndex += 2 )
      {
         unsigned char sb1MSB( (unsigned char)(strSource[muiIndex    ] - 0x30) );
         unsigned char sb1LSB( (unsigned char)(strSource[muiIndex + 1] - 0x30) );

         if( 0x09 < sb1MSB ) { sb1MSB -= 0x07; }
         if( 0x09 < sb1LSB ) { sb1LSB -= 0x07; }

         if( (0x0F < sb1LSB) || (0x0F < sb1MSB) )
         {
            return( false );
         }

         memTarget[ muiIndex / 2 ] = (char)((sb1MSB << 4) | sb1LSB);
      }

      return( true );
   }


   // Pass ShellMemory< BYTE >( NULL, MUnit< BYTE >( any ) ) to obtain the required size for converting the data.
   bool Ascii2Bin( MAtom< BYTE >& memTarget, const bool bIfOddSize_TheLastByteIsLessSignificantHalfByte ) const
   {
      if( !memTarget.CanReAllocate() )
      {
         // Some kind of a Shell Memory, check if they just enquire for the requited size or want to data converted?
         if( NULL == memTarget.GetMemory() )
         {
            // They only want the required size.
            memTarget.ReAllocate( MUnit< BYTE >( (GetLength()+1) / 2 ) );

            return( true );
         }
         else
         {
            // They want the data converted. Check if target memory has correct size.
            if( memTarget.GetSize().InBytes() != ((GetLength()+1) / 2) )
            {
               return( false );
            }
         }
      }
      else
      {
         // Get the necessary amount of memory.
         if( memTarget.GetSize().InBytes() != ((GetLength()+1) / 2) )
         {
            memTarget.ReAllocate( MUnit< BYTE >( (GetLength()+1) / 2 ) );
         }
      }

      MStringEx< char > strSource( GetAsMultiByte().ToUpper() );

      for( MUI muiIndex = 0; (muiIndex + 1) < strSource.GetLength(); muiIndex += 2 )
      {
         unsigned char sb1MSB( (unsigned char)(strSource[ muiIndex     ] - 0x30) );
         unsigned char sb1LSB( (unsigned char)(strSource[ muiIndex + 1 ] - 0x30) );

         if( 0x09 < sb1MSB ) { sb1MSB -= 0x07; }
         if( 0x09 < sb1LSB ) { sb1LSB -= 0x07; }

         if( (0x0F < sb1LSB) || (0x0F < sb1MSB) )
         {
            return( false );
         }

         memTarget[ muiIndex / 2 ] = (char)((sb1MSB << 4) | sb1LSB);
      }


      // If the data size is odd I have one byte which is unattended.
      if( 1 == (GetLength() & 1) )
      {
         unsigned char chLastByte( (unsigned char)(strSource[GetLength() - 1] - 0x30) );

         if( 0x09 < chLastByte ) { chLastByte -= 0x07; }

         if( 0x0F < chLastByte )
         {
            return( false );
         }

         memTarget[memTarget.GetSize()-1] = (char)(chLastByte << (bIfOddSize_TheLastByteIsLessSignificantHalfByte ? 0 : 4));
      }

      return( true );
   }


   const ShellMemory< tChar > GetPureText() const
   {
      return( ShellMemory< tChar >( this->pData, this->muSize - 1 ) );
   }


   // Returns number of tChars without the terminating zero tChar.
   MUI GetLength() const
   {
      return( (this->muSize - 1).GetUnits() );
   }


   DWORD GetLength32() const
   {
      return( (this->muSize - 1).GetUnits32() );
   }


   MUnit< tChar > GetLengthUnits() const
   {
      return( this->muSize - 1 );
   }


   bool IsLastChar( const tChar tcChar ) const
   {
      if( IsEmpty() )
      {
         return( false );
      }

      return( tcChar == this->pData[GetLength()-1] );
   }


   // Returns false if the object is not empty and true if the object is empty.
   bool IsEmpty() const
   {
      return( 1 == this->muSize );
   }


#ifdef _WIN32
   // Loads string from the application instance
   MStringEx< tChar >& LoadString( ResId ResId, HMODULE hmModule, WORD wLanguage )
   {
      return( LoadString( ResId.GetId(), hmModule, wLanguage ) );
   }


   // Loads string from the application instance
   MStringEx< tChar > &LoadString( unsigned int uiID, HMODULE hmModule, WORD /*wLanguage*/ )
   {
      if( NULL == hmModule )
      {
         hmModule = GetModuleHandle( NULL );
      }

      HRSRC hResource = ::FindResourceW( hmModule, MAKEINTRESOURCEW( (((uiID>>4)+1) & static_cast<WORD>(~0)) ), (LPWSTR) RT_STRING);

      if( NULL == hResource )
      {
         throw( MStringExException( MStringExException::CouldNotLoadResource ) );
      }


      const MUnit< BYTE > muSize( ::SizeofResource( hmModule, hResource ) );


      HGLOBAL hGlobal = ::LoadResource( hmModule, hResource );

      if( NULL == hGlobal )
      {
         throw( MStringExException( MStringExException::CouldNotLoadResource ) );
      }


      const BYTE* pImageStart = (const BYTE*)::LockResource( hGlobal );

      if( NULL == pImageStart )
      {
         throw( MStringExException( MStringExException::CouldNotLoadResource ) );
      }


      const BYTE* pImageEnd = (BYTE*)pImageStart + muSize.InBytes();


      for( DWORD dwIndex = 0; dwIndex < (uiID & 0x000f); dwIndex++ )
      {
         pImageStart = pImageStart + sizeof( WORD ) * ( 1 + *(const WORD*)pImageStart );

         if( pImageStart >= pImageEnd )
         {
            throw( MStringExException( MStringExException::CouldNotLoadResource ) );
         }
      }


      if( 0 == *(const WORD*)pImageStart )
      {
         throw( MStringExException( MStringExException::CouldNotLoadResource ) );
      }


      MUnit< TCHAR >       muStringSize( (*(WORD*)pImageStart) );
      ShellMemory< TCHAR > memString( (TCHAR*)(pImageStart + sizeof( WORD )), muStringSize );
      MStringEx< TCHAR >   strString( MStringEx< TCHAR >::SFM, memString, true );


      *this = strString.GetAs< tChar >();

      return( *this );
   }
#endif


   bool FindInFile( const MHandle& hFile, const unsigned __int64 uiSearchFrom, unsigned __int64* puiFoundAt, const HANDLE heStop, const bool bCaseSensitive ) const
   {
      // Make this class to look like MAtom derived.
      const MStringEx< tChar > strThis( bCaseSensitive ? *this : MStringEx< tChar >( *this ).ToUpper() );
      ShellMemory< tChar > memThis( (tChar*)strThis.pData, MUnit< tChar >( strThis.GetLength() ) );


      ULARGE_INTEGER liFileSize;
      if( !::GetFileSizeEx( hFile, (LARGE_INTEGER*)&liFileSize ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }

      if( (liFileSize.QuadPart < uiSearchFrom) || (0 != ((liFileSize.QuadPart - uiSearchFrom) % sizeof( tChar ) )) )
      {
         MAtomException::ExceptionIlligalMethodCall();
      }

      LARGE_INTEGER uiPosition;
      if( !SetFilePointerEx( hFile, (uiPosition.QuadPart = uiSearchFrom, uiPosition), &uiPosition, FILE_BEGIN ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }


      MASSERT( uiSearchFrom == (unsigned __int64)uiPosition.QuadPart );


      MemoryPH< tChar > memData( MUnit< tChar >( MMIN< MUI, ULONGLONG, ULONGLONG >( 2 << 20, liFileSize.QuadPart - uiSearchFrom ) ) );
      memData.LoadFromFile( hFile );

      ShellMemory< tChar > memSearcher( memData );

      while( !memData.IsEmpty() )
      {
         const ShellMemory< tChar > memMatch( memData.Search_L2R( memThis, true, bCaseSensitive ) );

         if( memMatch.IsEmpty() )
         {
            if( NULL != heStop )
            {
               if( WAIT_TIMEOUT != ::WaitForSingleObject( heStop, 0 ) )
               {
                  return( false );
               }
            }

            uiPosition.QuadPart += memData.GetSize().InBytes();

            memData.LoadFromFile( hFile );

            memSearcher = memData;

            continue;
         }

         if( memMatch.GetSize() == memThis.GetSize() )
         {
            // Matching.
            if( NULL != puiFoundAt )
            {
               *puiFoundAt = uiPosition.QuadPart + ((MUI)memMatch.pData - (MUI)memData.pData);
            }

            return( true );
         }
         else
         {
            // Partial match - need to check if the memory behind what is loaded continues to match.
            // So ... Load the next chunk from the file.
            uiPosition.QuadPart += memData.GetSize().InBytes();
            memData.LoadFromFile( hFile );

            const MUnit< tChar > muSize2Check( memThis.GetSize() - memMatch.GetSize() );

            if( memData.GetSize() < muSize2Check )
            {
               return( false );
            }

            if( memData.SubMemory( MUnit< tChar >( 0 ), muSize2Check ) == memThis.SubMemory( memMatch.GetSize(), muSize2Check ) )
            {
               // Matching.
               if( NULL != puiFoundAt )
               {
                  *puiFoundAt = uiPosition.QuadPart - memMatch.GetSize().InBytes();
               }

               return( true );
            }
            else
            {
               // There is no matching I need to search in the whole memory, so nothing to do in that case.
               memSearcher = memData;
            }
         }
      }

      return( false );
   }


   MUI Find_L2R( const tChar chCHAR, const MUI muiStartingPosition, const bool bCaseSensitive ) const throw()
   {
      MASSERT( 0           <= muiStartingPosition );
      MASSERT( GetLength() >= muiStartingPosition );

      const tChar* pThis( this->pData );
      const tChar  tchSearchFor[2] = { chCHAR, 0 };

      MStringEx< tChar > strThis;

      if( !bCaseSensitive )
      {
         strThis   = *this;
         strThis.ToLower();
         pThis      = strThis.pData;

         if( IsWchar() )
         {
            _wcslwr_s( (wchar_t*)tchSearchFor, sizeof( tchSearchFor ) / sizeof( tChar ) );
         }
         else
         {
            _strlwr_s( (char*)tchSearchFor, sizeof( tchSearchFor ) / sizeof( tChar ) );
         }
      }

      for( MUI muiIndex = muiStartingPosition; 0 != this->pData[muiIndex]; muiIndex++ )
      {
         if( tchSearchFor[0] == this->pData[muiIndex] )
         {
            return( muiIndex );
         }
      }

      return( MSTRING_ITEM_NOT_FOUND );
   }


   MUI Find_L2R( const MStringEx< tChar >& strSearchFor, const MUI muiStartingPos, const bool bCaseSensitive ) const throw()
   {
      if( muiStartingPos < GetLength() )
      {
         if( strSearchFor.GetLength() <= (GetLength() - muiStartingPos) )
         {
            const tChar* pThis( this->pData );
            const tChar* pSearchFor( strSearchFor.pData );

            MStringEx< tChar > strThis;
            MStringEx< tChar > strSearch;

            if( !bCaseSensitive )
            {
               strThis   = *this;
               strSearch = strSearchFor;

               strThis.ToLower();
               strSearch.ToLower();

               pThis      = strThis.pData;
               pSearchFor = strSearch.pData;
            }

            for( MUI muiIndex = muiStartingPos; muiIndex <= GetLength() - strSearchFor.GetLength(); muiIndex++ )
            {
               if( 0 == memcmp( &(pThis[muiIndex]), pSearchFor, strSearchFor.GetLength() * strSearchFor.GetUnitSize() ) )
               {
                  return( muiIndex );
               }
            }
         }

         return( MSTRING_ITEM_NOT_FOUND );
      }

      return( MSTRING_ITEM_NOT_FOUND );
   }


   MUI Find_L2R( const ResId ResId, const MUI muiPosition, const bool bCaseSensitive ) const throw()
   {
      return( Find_L2R( MStringEx< tChar >( ResId ), muiPosition, bCaseSensitive ) );
   }

   
   MUI Find_R2L( const MStringEx< tChar >& strSearchFor, const MUI muiStartingPos, const bool bCaseSensitive ) const throw()
   {
      if( strSearchFor.GetLength() <= GetLength() )
      {
         const MUI  muiStartPosition( MMIN< MUI, MUI, MUI >( GetLength() - strSearchFor.GetLength(), muiStartingPos ) );
         const tChar* pThis( this->pData );
         const tChar* pSearchFor( strSearchFor.pData );

         MStringEx< tChar > strThis;
         MStringEx< tChar > strSearch;

         if( !bCaseSensitive )
         {
            strThis   = *this;
            strSearch = strSearchFor;

            strThis.ToLower();
            strSearch.ToLower();

            pThis      = strThis.pData;
            pSearchFor = strSearch.pData;
         }

         for( MUI muiIndex = muiStartPosition; muiIndex != MSTRING_ITEM_NOT_FOUND; muiIndex-- )
         {
            if( 0 == memcmp( &(pThis[muiIndex]), pSearchFor, strSearchFor.GetLength() * strSearchFor.GetUnitSize() ) )
            {
               return( muiIndex );
            }
         }

         return( MSTRING_ITEM_NOT_FOUND );
      }

      return( MSTRING_ITEM_NOT_FOUND );
   }


   bool StartWith( const MStringEx< tChar >& strSearchFor, const bool bCaseSensitive ) const throw()
   {
      if( GetLength() < strSearchFor.GetLength() )
      {
         return( false );
      }

      if( strSearchFor.IsEmpty() )
      {
         return( false );
      }

      if( bCaseSensitive )
      {
         for( MUI muiIndex = 0; muiIndex < strSearchFor.GetLength(); muiIndex++ )
         {
            if( this->pData[muiIndex] != strSearchFor.pData[muiIndex] )
            {
               return( false );
            }
         }
      }
      else
      {
         MStringEx< tChar > strS1( *this );
         MStringEx< tChar > strS2( strSearchFor );

         strS1.ToUpper();
         strS2.ToUpper();

         for( MUI muiIndex = 0; muiIndex < strS2.GetLength(); muiIndex++ )
         {
            if( strS1.pData[muiIndex] != strS2.pData[muiIndex] )
            {
               return( false );
            }
         }
      }

      return( true );
   }


   bool IsEndingWith( const MStringEx< tChar >& strSearchFor, const bool bCaseSensitive ) const throw()
   {
      if( GetLength() < strSearchFor.GetLength() )
      {
         return( false );
      }

      if( strSearchFor.IsEmpty() )
      {
         return( false );
      }

      if( bCaseSensitive )
      {
         for( MUI muiIndex = 0; muiIndex < strSearchFor.GetLength(); muiIndex++ )
         {
            if( this->pData[GetLength() - strSearchFor.GetLength() + muiIndex] != strSearchFor.pData[muiIndex] )
            {
               return( false );
            }
         }
      }
      else
      {
         MStringEx< tChar > strS1( *this );
         MStringEx< tChar > strS2( strSearchFor );

         strS1.ToUpper();
         strS2.ToUpper();

         for( MUI muiIndex = 0; muiIndex < strS2.GetLength(); muiIndex++ )
         {
            if( strS1.pData[GetLength() - strSearchFor.GetLength() + muiIndex] != strS2.pData[muiIndex] )
            {
               return( false );
            }
         }
      }

      return( true );
   }


   // Returns the occurrence count of the specified character, from character with index muiStart for muiChars chars.
   MUI GetCharCount( const tChar tcChar, const MUI muiStart, const MUI muiChars )
   {
      MUI muiCharCount( 0 );

      const MUI muiLength = GetLength();

      for( MUI muiIndex = muiStart, muiCount = 0; muiIndex < muiLength && muiCount < muiChars; muiIndex++, muiCount++ )
      {
         if( tcChar == this->pData[muiIndex] )
         {
            muiCharCount++;
         }
      }

      return( muiCharCount );
   }


   // Returns the number of chars that the string ends with from the search, e.g. 
   // 1234( search for 3456 ) returns 2, while 123456( search for 3456 ) returns 4.
   MUI EndWith( MStringEx< tChar > strSearchFor, const bool bCaseSensitive ) const throw()
   {
      while( !strSearchFor.IsEmpty() )
      {
         if( strSearchFor.Compare( Right( strSearchFor.GetLength() ), bCaseSensitive ) )
         {
            return( strSearchFor.GetLength() );
         }

         strSearchFor = strSearchFor.Left( strSearchFor.GetLength() -1 );
      }

      return( strSearchFor.GetLength() );
   }


   bool HasOneOfChars( const tChar* strSearchChars, const MUI muiStartFrom, const MUI muiSearchCount, const bool bCaseSensitive ) const throw()
   {
      if( GetLength() <= muiStartFrom )
      {
         return( false );
      }

      const MUI muiLastSearch( muiStartFrom + MMIN< MUI, MUI, MUI >( muiSearchCount, GetLength() - muiStartFrom ) );

      MStringEx< tChar > strThis( *this );
      MStringEx< tChar > strChar( strSearchChars );

      if( !bCaseSensitive )
      {
         strThis.ToUpper();
         strChar.ToUpper();
      }

      for( MUI muiIndex = muiStartFrom; muiIndex < muiLastSearch; muiIndex++ )
      {
         for( MUI muiI2 = 0; muiI2 < strChar.GetLength(); muiI2++ )
         {
            if( strThis.pData[muiIndex] == strChar.pData[muiI2] )
            {
               return( true );
            }
         }
      }

      return( false );
   }


   bool HasCharOtherThan( const tChar* strSearchChars, const MUI muiStartFrom, const MUI muiSearchCount, const bool bCaseSensitive ) const throw()
   {
      if( GetLength() <= muiStartFrom )
      {
         return( false );
      }

      const MUI muiLastSearch( muiStartFrom + MMIN< MUI, MUI, MUI >( muiSearchCount, GetLength() - muiStartFrom ) );

      MStringEx< tChar > strThis( *this );
      MStringEx< tChar > strChar( strSearchChars );

      if( !bCaseSensitive )
      {
         strThis.ToUpper();
         strChar.ToUpper();
      }

      for( MUI muiIndex = muiStartFrom; muiIndex < muiLastSearch; muiIndex++ )
      {
         bool bCharFound( false );

         for( MUI muiI2 = 0; (muiI2 < strChar.GetLength()) && !bCharFound; muiI2++ )
         {
            bCharFound |= strThis.pData[muiIndex] == strChar.pData[muiI2];
         }

         if( !bCharFound )
         {
            return( true );   
         }
      }

      return( false );
   }


   template< class tcTestChar >
   static bool IsNewLineChar( const tcTestChar tcChar )
   {
      switch( tcChar )
      {
      case 0x000A: // LF:    Line Feed, U+000A
      case 0x000C: // FF:    Form Feed, U+000C
      case 0x000D: // CR:    Carriage Return, U+000D
      //case 0x000A:  CR+LF: CR (U+000D) followed by LF (U+000A)
      case 0x0085: // NEL:   Next Line, U+0085
      case 0x2028: // LS:    Line Separator, U+2028
      case 0x2029: // PS:    Paragraph Separator, U+2029
         return( true );
      }

      return( false );
   }
   

   template< class tcTestChar >
   static bool IsWhiteSpaceChar( const tcTestChar tcChar )
   {
      // Control characters, containing Tab, CR and LF.
      if( IsInRange< unsigned short, true, unsigned short, true, unsigned short >( 0x0009, (unsigned short)tcChar, 0x000D ) )
      {
         return( true );
      }

      // Different sorts of spaces.
      if( IsInRange< unsigned short, true, unsigned short, true, unsigned short >( 0x2000, (unsigned short)tcChar, 0x200A ) )
      {
         return( true );
      }

      switch( (unsigned short)tcChar )
      {
      case 0x0020: // SPACE
      case 0x0085: // NEL (control character next line)
      case 0x00A0: // NBSP (NO-BREAK SPACE)
      case 0x1680: // OGHAM SPACE MARK
      case 0x180E: // MONGOLIAN VOWEL SEPARATOR
      case 0x2028: // LS (LINE SEPARATOR)
      case 0x2029: // PS (PARAGRAPH SEPARATOR)
      case 0x202F: // NNBSP (NARROW NO-BREAK SPACE)
      case 0x205F: // MMSP (MEDIUM MATHEMATICAL SPACE)
      case 0x3000: // IDEOGRAPHIC SPACE
         return( true );
      }

      return( false );
   }


   MStringEx< tChar >& WildcardToRegex()
   {
      return( *this = TEXT("^") + EscapeRegEx().Replace( MStringEx< tChar >( TEXT("\\*") ), MStringEx< tChar >( TEXT(".*") ), true ).Replace( MStringEx< tChar >( TEXT("\\?") ), MStringEx< tChar >( TEXT(".") ), true ) + TEXT("$") );
   }


   MStringEx< tChar >& EscapeRegEx() throw()
   {
      MemoryPH< tChar > strNew( MUnit< tChar >( 2 * GetLength() + 1 ) );

      for( MUI muiIndexThis = 0, muiIndexNew = 0; muiIndexThis < GetLength(); muiIndexThis++, muiIndexNew++ )
      {
         if( IsWhiteSpaceChar( this->pData[muiIndexThis] ) )
         {
            strNew[muiIndexNew] = TCHAR('\\');
            strNew[++muiIndexNew] = this->pData[muiIndexThis];

            continue;
         }

         switch( this->pData[muiIndexThis] )
         {
         case TCHAR('\\'):
         case TCHAR('*'):
         case TCHAR('+'):
         case TCHAR('?'):
         case TCHAR('|'):
         case TCHAR('{'):
         case TCHAR('['):
         case TCHAR('('):
         case TCHAR(')'):
         case TCHAR('^'):
         case TCHAR('$'):
         case TCHAR('.'):
         case TCHAR('#'):
            strNew[muiIndexNew] = TCHAR('\\');
            strNew[++muiIndexNew] = this->pData[muiIndexThis];
            break;

         default:
            strNew[muiIndexNew] = this->pData[muiIndexThis];
            break;
         }
      }

      *this = strNew;

      return( *this );
   }


   MStringEx< tChar >& TrimLeft( const tChar tcChar ) throw()
   {
      MUI muiStart        = GetLength();
      const MUI muiLength = GetLength();

      for( MUI muiIndex = 0; muiIndex < muiLength; muiIndex++ )
      {
         if( tcChar != this->pData[muiIndex] )
         {
            muiStart = muiIndex;

            break;
         }
      }

      if( 0 == muiStart )
      {
         return( *this );
      }

      return( *this = Right( muiLength - muiStart ) );
   }


   MStringEx< tChar >& TrimLeft() throw()
   {
      MUI muiStart        = GetLength();
      const MUI muiLength = GetLength();

      for( MUI muiIndex = 0; (muiLength == muiStart) && (muiIndex < muiLength); muiIndex++ )
      {
         if( IsWhiteSpaceChar( this->pData[muiIndex] ) )
         {
            continue;
         }

         muiStart = muiIndex;
      }

      if( 0 == muiStart )
      {
         return( *this );
      }

      return( *this = Right( muiLength - muiStart ) );
   }


   MStringEx< tChar >& TrimRight( const tChar tcChar ) throw()
   {
      MUI muiStart  = GetLength();
      const MUI muiLength = GetLength();

      for( MUI muiIndex = 0; muiIndex < muiLength; muiIndex++ )
      {
         if( tcChar != this->pData[muiLength - muiIndex - 1] )
         {
            muiStart = muiIndex;

            break;
         }
      }

      if( 0 == muiStart )
      {
         return( *this );
      }

      return( *this = Left( muiLength - muiStart ) );
   }


   MStringEx< tChar >& TrimRight() throw()
   {
      MUI muiStart  = GetLength();
      const MUI muiLength = GetLength();

      for( MUI muiIndex = 0; (muiLength == muiStart) && (muiIndex < muiLength); muiIndex++ )
      {
         if( IsWhiteSpaceChar( this->pData[muiLength - muiIndex - 1] ) )
         {
            continue;
         }

         muiStart = muiIndex;
      }

      if( 0 == muiStart )
      {
         return( *this );
      }

      return( *this = Left( muiLength - muiStart ) );
   }


   MStringEx< tChar >& Trim() throw()
   {
      return( TrimLeftRight() );
   }


   MStringEx< tChar >& Trim( const tChar tcChar ) throw()
   {
      return( TrimLeftRight( tcChar ) );
   }


   MStringEx< tChar >& TrimLeftRight() throw()
   {
      // The temporary string object is necessary for the operator =.
      return( *this = MStringEx< tChar >( TrimLeft().TrimRight() ) );
   }


   MStringEx< tChar >& TrimLeftRight( const tChar tcChar ) throw()
   {
      // The temporary string object is necessary for the operator =.
      return( *this = MStringEx< tChar >( TrimLeft( tcChar ).TrimRight( tcChar ) ) );
   }


   MStringEx< tChar >& ToLower()
   {
      if( IsWchar() )
      {
         _wcslwr_s( (wchar_t*)this->pData, this->muSize.GetUnits() );
      }
      else
      {
         _strlwr_s( (char*)this->pData, this->muSize.GetUnits() );
      }

      return( *this );
   }


   MStringEx< tChar >& ToUpper()
   {
      if( IsWchar() )
      {
         _wcsupr_s( (wchar_t*)this->pData, this->muSize.GetUnits() );
      }
      else
      {
         _strupr_s( (char*)this->pData, this->muSize.GetUnits() );
      }

      return( *this );
   }


   MStringEx< tChar >& MakeUpperChar( const MUI muiIndex )
   {
      if( muiIndex < GetLength() )
      {
         if( IsWchar() )
         {
            *(wchar_t*)&this->pData[muiIndex] = towupper( this->pData[muiIndex] );
         }
         else
         {
            *(char*)&this->pData[muiIndex] = (char)toupper( this->pData[muiIndex] );
         }
      }

      return( *this );
   }


   operator const tChar*() const
   {
      return( this->pData );
   }


   operator tChar*()
   {
      return( this->pData );
   }


   operator MStringEx< tChar >()
   {
      return( *this );
   }



   operator long() const throw()
   {
      if( IsWchar() )
      {
         return( wcstol( (wchar_t*)this->pData, NULL, eNumericBase ) );
      }

      return( strtol( (char*)this->pData, NULL, eNumericBase ) );
   }


   operator unsigned long() const throw()
   {
      if( IsWchar() )
      {
         return( wcstoul( (wchar_t*)this->pData, NULL, eNumericBase ) );
      }

      return( strtoul( (char*)this->pData, NULL, eNumericBase ) );
   }


   operator __int64() const throw()
   {
      if( IsWchar() )
      {
         return( _wcstoi64( (wchar_t*)this->pData, NULL, eNumericBase ) );
      }

      return( _strtoi64( (char*)this->pData, NULL, eNumericBase ) );
   }


   operator unsigned __int64() const throw()
   {
      if( IsWchar() )
      {
         return( _wcstoui64( (wchar_t*)this->pData, NULL, eNumericBase ) );
      }

      return( _strtoui64( (char*)this->pData, NULL, eNumericBase ) );
   }


   operator double() const throw()
   {
      if( IsWchar() )
      {
         return( wcstod( (wchar_t*)this->pData, NULL ) );
      }

      return( strtod( (char*)this->pData, NULL ) );
   }


   const MStringEx< tChar >& PrepareNumericConversion( NumericBase eNewNumberBase ) const throw()
   {
      eNumericBase = eNewNumberBase;

      return( *this );
   }


   // Returns the size of string recounting it. Returns -1 and MASSERT( FALSE ) if the search fail
   // to find ending zero. The GetLength returns the memory contained by the string (excluding the
   // ending zero) which is the correct value in most cases, however it may be wrong if and access
   // to the string memory has been granted.
   MUI FindLength() const
   {
      MUI muiRealStringLength = 0;
      const MUI muiAssumedSize( GetLength() );

      for( ; 0 != this->pData[muiRealStringLength]; muiRealStringLength++ )
      {
         if( muiAssumedSize < muiRealStringLength )
         {
            // The string has no terminating zero.
            MASSERT( FALSE );

            return( MSTRING_ITEM_NOT_FOUND );
         }
      }

      return( muiRealStringLength );
   }


protected:
   static MUI FindCharSequenceAsStringLength( const tChar *ptchCSAsString )
   {
      MASSERT( NULL != ptchCSAsString );

      if( NULL == ptchCSAsString )
      {
         return( 0 );
      }

      MUI muiRealStringLength( 0 );

      for( ; 0 != ptchCSAsString[muiRealStringLength]; muiRealStringLength++ );

      return( muiRealStringLength );
   }


public:
   MStringEx< tChar >& ReSize()
   {
      MStringEx< tChar > strString( MUnit< tChar >( this->FindLength() ) );

      strString.Transfer( this->SubMemory( MUnit< tChar >( 0 ), strString.GetSize() ) );

      this->ExchangeMemory( strString );

      return( *this );
   }


   MStringEx< tChar >& operator=( const MStringEx< tChar >& objMStringEx )
   {
      if( objMStringEx.muSize != this->muSize )
      {
         this->ReAllocate( objMStringEx.muSize );
      }

      this->Transfer( objMStringEx );

      eNumericBase = objMStringEx.eNumericBase;

      return( *this );
   }


   MStringEx< tChar >& operator=( const tChar *ptrNewString )
   {
      MStringEx< tChar > strString( ptrNewString );

      this->ExchangeMemory( strString );

      return( *this );
   }


#ifdef _WIN32
   MStringEx< tChar > &operator=( const MBStr& bstrText )
   {
      MStringEx< tChar > strString( MStringEx< tChar >::FromBSTR, bstrText.operator const BSTR &() );

      ExchangeMemory( strString );

      return( *this );
   }
#endif


   // Case sensitive compare.
   bool operator==( const MStringEx< tChar >& strToCompare ) const
   {
      if( this->muSize != strToCompare.GetSize() )
      {
         return( false );
      }

      if( 0 != memcmp( this->pData, strToCompare.GetMemory(), this->muSize.InBytes() ) )
	  {
         return( false );
	  }

      return( eNumericBase == strToCompare.eNumericBase );
   }


   bool CompareNoCase( const MStringEx< tChar >& strRight ) const throw()
   {
      if( GetLength() != strRight.GetLength() )
      {
         return( false );
      }

      MStringEx< tChar > strLeftInternal( *this );
      MStringEx< tChar > strRightInternal( strRight );
      
      strLeftInternal.ToUpper();
      strRightInternal.ToUpper();

      return( strLeftInternal == strRightInternal );
   }


   bool Compare( const tChar* strRight, const bool bCaseSensitive ) const throw()
   {
      if( bCaseSensitive )
      {
         return( *this == MStringEx< tChar >( strRight ) );
      }

      return( CompareNoCase( strRight ) );
   }


   // Case sensitive compare.
   bool operator!=( const MStringEx< tChar > &strToCompare ) const
   {
      return( !(*this == strToCompare) );
   }


   // Case sensitive compare.
   friend bool operator==( const MStringEx< tChar >& strLeft, const tChar* strRight ) throw()
   {
      if( strLeft.GetLength() != FindCharSequenceAsStringLength( strRight ) )
      {
         return( false );
      }

      return( 0 == memcmp( strLeft.pData, strRight, strLeft.GetSize().InBytes() ) );
   }


   // Case sensitive compare.
   friend bool operator!=( const MStringEx< tChar >& strLeft, const tChar* strRight ) throw()
   {
      if( strLeft.GetLength() != FindCharSequenceAsStringLength( strRight ) )
      {
         return( true );
      }

      return( 0 != memcmp( strLeft.pData, strRight, strLeft.GetSize().InBytes() ) );
   }


   // Case sensitive compare.
   friend bool operator==( const tChar* strLeft, const MStringEx< tChar >& strRight ) throw()
   {
      if( FindCharSequenceAsStringLength( strLeft ) != strRight.GetLength() )
      {
         return( false );
      }

      return( 0 == memcmp( strLeft, strRight.pData, strRight.GetSize().InBytes() ) );
   }


   // Case sensitive compare.
   friend bool operator!=( const tChar* strLeft, const MStringEx< tChar >& strRight ) throw()
   {
      if( FindCharSequenceAsStringLength( strLeft ) != strRight.GetLength() )
      {
         return( true );
      }

      return( 0 != memcmp( strLeft, strRight.pData, strRight.GetSize().InBytes() ) );
   }


   // Case sensitive compare.
   friend bool operator==( const MStringEx< tChar >& strString, const ResId& ResId )
   {
      const MStringEx< tChar > strRightCompare( ResId );

      return( strString == strRightCompare );
   }


   // Case sensitive compare.
   friend bool operator==( const ResId& ridString, const MStringEx< tChar >& strString )
   {
      const MStringEx< tChar > strRIDCompare( ridString );

      return( strString == strRIDCompare );
   }


   // Case sensitive compare.
   friend bool operator!=( const MStringEx< tChar >& strString, const ResId& ridString )
   {
      const MStringEx< tChar > strRIDCompare( ridString );

      return( strString != strRIDCompare );
   }


   // Case sensitive compare.
   friend bool operator!=( const ResId& ridString, const MStringEx< tChar >& strString )
   {
      const MStringEx< tChar > strRIDCompare( ridString );

      return( strString != strRIDCompare );
   }


   // Compare two strings lexicographically e.g. "a" > "A".
   bool operator<( const MStringEx< tChar > &strToCompare ) const
   {
      if( IsWchar() )
      {
         return( wcscmp( (wchar_t*)this->pData, (wchar_t*)strToCompare.pData ) < 0 );
      }
      else
      {
         return( strcmp( (char*)this->pData, (char*)strToCompare.pData ) < 0 );
      }
   }


   // Compare two strings lexicographically e.g. "a" > "A".
   bool operator<=( const MStringEx< tChar > &strToCompare ) const
   {
      if( IsWchar() )
      {
         return( wcscmp( (wchar_t*)this->pData, (wchar_t*)strToCompare.pData ) <= 0 );
      }
      else
      {
         return( strcmp( (char*)this->pData, (char*)strToCompare.pData ) <= 0 );
      }
   }


   // Compare two strings lexicographically e.g. "a" > "A".
   bool operator>( const MStringEx< tChar > &strToCompare ) const
   {
      if( IsWchar() )
      {
         return( wcscmp( (wchar_t*)this->pData, (wchar_t*)strToCompare.pData ) > 0 );
      }
      else
      {
         return( strcmp( (char*)this->pData, (char*)strToCompare.pData ) > 0 );
      }
   }


   // Compare two strings lexicographically e.g. "a" > "A".
   bool operator>=( const MStringEx< tChar > &strToCompare ) const
   {
      if( IsWchar() )
      {
         return( wcscmp( (wchar_t*)this->pData, (wchar_t*)strToCompare.pData ) >= 0 );
      }
      else
      {
         return( strcmp( (char*)this->pData, (char*)strToCompare.pData ) >= 0 );
      }
   }


   MStringEx< tChar >& operator+=( const MStringEx< tChar > &strAdd2This )
   {
      MASSERT( FindLength() == GetLength() );
      MASSERT( strAdd2This.FindLength() == strAdd2This.GetLength() );

      MStringEx< tChar > strString( MUnit< tChar >( GetLength() + strAdd2This.GetLength() ) );

      strString.Transfer( *this );
      strString.Transfer( this->muSize - 1, strAdd2This );

      this->ExchangeMemory( strString );

      return( *this );
   }


   MStringEx< tChar >& operator+=( const tChar tchChar )
   {
      MASSERT( FindLength() == GetLength() );

      // If appending zero, then there is noting to do.
      if( 0 == tchChar )
      {
         return( *this );
      }


      MStringEx< tChar > strString( MUnit< tChar >( GetLength() + 1 ) );

      strString.Transfer( *this );
      strString[GetLength()] = tchChar;

      this->ExchangeMemory( strString );

      return( *this );
   }


   // Inserts strInsertion in the *this string at [Position]. Where [Position] is zero based index of
   // char == tchTarget found with MStringEx< tChar >Find_L2R( tChar, int, bool ) starting from muiSearchStart.
   // if Find_L2R( tCHar, int, bool ) fails - the strInsertionwill be added to the end of *this. Returns the new (&)(*this).
   MStringEx< tChar > Insert( const tChar tchTarget, const bool bCaseSensitive, const MUI muiSearchStart, const tChar* strInsertion )
   {
      MASSERT( FindLength() == GetLength() );

      const MUI muiInsertionLength( FindCharSequenceAsStringLength( strInsertion ) );

      if( 0 == muiInsertionLength )
      {
         return( *this );
      }


      MStringEx< tChar > strString( MUnit< tChar >( GetLength() + muiInsertionLength ) );

      const MUI muiInsertPosition( MMIN< MUI, MUI, MUI >( GetLength(), Find_L2R( tchTarget, muiSearchStart, bCaseSensitive ) ) );
      
      MUnit< tChar > muOffset;
      strString.Transfer( muOffset, true, ShellMemory< tChar >( this->pData, MUnit< tChar >( muiInsertPosition ) ) );
      strString.Transfer( muOffset, true, ShellMemory< tChar >( (tChar*)strInsertion, MUnit< tChar >( muiInsertionLength ) ) );
      strString.Transfer( muOffset, true, ShellMemory< tChar >( this->pData + muiInsertPosition, MUnit< tChar >( GetLength() - muiInsertPosition ) ) );

      this->ExchangeMemory( strString );

      return( *this );
   }


   // Inserts strInsertion in the *this string at muiPosition. Where muiPosition is zero based index.
   // if( muiPosition > this->GetLength() ) - the strInsertionwill be added to the end of *this. Returns the new (&)(*this).
   MStringEx< tChar > Insert( const MUI muiPosition, const MStringEx< tChar >& strInsertion )
   {
      MASSERT( FindLength() == GetLength() );
      MASSERT( strInsertion.FindLength() == strInsertion.GetLength() );

      if( 0 == strInsertion.GetLength() )
      {
         return( *this );
      }


      MStringEx< tChar > strString( MUnit< tChar >( GetLength() + strInsertion.GetLength() ) );

      const MUI muiInsertPosition( MMIN< MUI, MUI, MUI >( GetLength(), muiPosition ) );
      
      MUnit< tChar > muOffset;
      strString.Transfer( muOffset, true, ShellMemory< tChar >( this->pData, MUnit< tChar >( muiInsertPosition ) ) );
      strString.Transfer( muOffset, true, strInsertion );
      muOffset--;
      strString.Transfer( muOffset, true, ShellMemory< tChar >( this->pData + muiInsertPosition, MUnit< tChar >( GetLength() - muiInsertPosition ) ) );

      this->ExchangeMemory( strString );

      return( *this );
   }


   // Inserts strInsertionSeed in the *this string at muiPosition, muiTimes times. Where muiPosition is zero based index.
   // if( muiPosition > this->GetLength() ) - the strInsertionSeed will be added to the end of *this.
   MStringEx< tChar > Insert( const MUI muiPosition, const MStringEx< tChar > &strInsertionSeed, const MUI muiTimes )
   {
      MASSERT( FindLength() == GetLength() );
      MASSERT( strInsertionSeed.FindLength() == strInsertionSeed.GetLength() );

      if( (0 == strInsertionSeed.GetLength()) || (0 == muiTimes) )
      {
         return( *this );
      }


      MStringEx< tChar > strString( MUnit< tChar >( GetLength() + muiTimes * strInsertionSeed.GetLength() ) );

      const MUI muiInsertPosition( MMIN< MUI, MUI, MUI >( GetLength(), muiPosition ) );
      
      MUnit< tChar > muOffset;
      strString.Transfer( muOffset, true, ShellMemory< tChar >( this->pData, MUnit< tChar >( muiInsertPosition ) ) );

      for( MUI muiCount = 0; muiCount < muiTimes; muiCount++ )
      {
         strString.Transfer( muOffset, true, strInsertionSeed );
         muOffset--;
      }

      strString.Transfer( muOffset, true, ShellMemory< tChar >( this->pData + muiInsertPosition, MUnit< tChar >( GetLength() - muiInsertPosition ) ) );

      this->ExchangeMemory( strString );

      return( *this );
   }


   // Inserts the strSeparator string every muiDistanceChars symbols, WITHOUT appending the strSeparator to either end in any case.
   // This function is useful to convert a numeric string such as 1234567890 to 1,234,567,890
   MStringEx< tChar >& InsertRepeatedly( const MStringEx< tChar > &strSeparator, const MUI muiDistanceChars, const bool bLeftToRight ) throw()
   {
      MASSERT( 0 != muiDistanceChars );

      if( IsEmpty() )
      {
         return( *this );
      }

      if( strSeparator.IsEmpty() )
      {
         return( *this );
      }

      if( GetLength() <= muiDistanceChars )
      {
         return( *this );
      }

      const MUI muiTimes( (GetLength() - 1) / muiDistanceChars );

      MStringEx< tChar > strString( MUnit< tChar >( GetLength() + muiTimes * strSeparator.GetLength() ) );


      MUnit< tChar > muOffsetSrc;
      MUnit< tChar > muOffsetDst;

      for( MUI muiCount = 0; muiCount < muiTimes; muiCount++ )
      {
         const MUnit< tChar > muInsertPosition( 0 == muiCount ? (bLeftToRight ? muiDistanceChars : (1 + ((GetLength() - 1) % muiDistanceChars))) : muiDistanceChars );

         strString.Transfer( muOffsetDst, true, this->SubMemory( muOffsetSrc, muInsertPosition ) );
         muOffsetSrc += muInsertPosition;

         strString.Transfer( muOffsetDst, true, strSeparator );
         muOffsetDst--;
      }

      strString.Transfer( muOffsetDst, true, this->SubMemory( muOffsetSrc, MUnit< tChar >( GetLength() - muOffsetSrc.GetUnits() ) ) );

      this->ExchangeMemory( strString );

      return( *this );
   }


   MUI CharAppearenceCount( const tChar tchChar ) const throw()
   {
      MUI muiCount( 0 );

      for( MUI muiIndex = 0; NULL != this->pData[muiIndex]; muiIndex++ )
      {
         if( tchChar == this->pData[muiIndex] )
         {
            muiCount++;
         }
      }

      return( muiCount );
   }


   MStringEx< tChar > Left( const MUI muiChars ) const
   {
      MASSERT( GetLength() == FindLength() );

      MStringEx< tChar > strLeft( MUnit< tChar >( MMIN< MUI, MUI, MUI >( muiChars, GetLength() ) ) );

      strLeft.Transfer( this->SubMemory( MUnit< tChar >( 0 ), strLeft.muSize - 1 ) );

      MASSERT( strLeft.GetLength() == strLeft.FindLength() );

      return( strLeft );
   }


   MStringEx< tChar > LeftFromRight( const MUI muiCharsOnRightToOmmit ) const
   {
      MASSERT( GetLength() == FindLength() );

      if( GetLength() <= muiCharsOnRightToOmmit )
      {
         return( MStringEx< tChar >() );
      }

      MStringEx< tChar > strLeft( MUnit< tChar >( MMIN< MUI, MUI, MUI >( GetLength() - muiCharsOnRightToOmmit, GetLength() ) ) );

      strLeft.Transfer( this->SubMemory( MUnit< tChar >( 0 ), strLeft.muSize - 1 ) );

      MASSERT( strLeft.GetLength() == strLeft.FindLength() );

      return( strLeft );
   }


   // bImpliedL2RPreFix - when the searched character does not exists in the string then what? Return the entire string or return an empty sting?
   // In such case bImpliedL2RPreFix determines to return the entire string or an empty string, e.g. if the char does not exist the function with
   // bImpliedL2RPreFix = true will prefix the char before the string and the function will return the entire string. If false the function will
   // effectively post-fix the character after the string in the direction of search and the function will return an empty string.
   MStringEx< tChar > LeftFromL2RNthChar( const TCHAR tchChar, const MUI muiL2RNthCharApearence, const bool bImpliedL2RPreFix ) const
   {
      MUI muiCount( 0 );
      MUI muiIndex( 0 );

      for( ; NULL != this->pData[muiIndex]; muiIndex++ )
      {
         if( tchChar == this->pData[muiIndex] )
         {
            muiCount++;

            if( muiL2RNthCharApearence == muiCount )
            {
               break;
            }
         }
      }

      if( (NULL == this->pData[muiIndex]) && !bImpliedL2RPreFix )
      {
         return( Left( 0 ) );
      }

      return( Left( muiIndex ) );
   }


   // bImpliedR2LPreFix - when the searched character does not exists in the string then what? Return the entire string or return an empty sting?
   // In such case bImpliedR2LPreFix determines to return the entire string or an empty string, e.g. if the char does not exist the function with
   // bImpliedR2LPreFix = true will prefix the char before the string and the function will return the entire string. If false the function will
   // effectively post-fix the character after the string in the direction of search and the function will return an empty string.
   MStringEx< tChar > LeftFromR2LNthChar( const TCHAR tchChar, const MUI muiR2LNthCharApearence, const bool bImpliedR2LPreFix ) const
   {
      MUI muiCount( 0 );
      MUI muiIndex( 0 );

      const MUI muiNthCharApearence( CharAppearenceCount( tchChar ) - muiR2LNthCharApearence + 1 );

      for( ; NULL != this->pData[muiIndex]; muiIndex++ )
      {
         if( tchChar == this->pData[muiIndex] )
         {
            muiCount++;

            if( muiNthCharApearence == muiCount )
            {
               break;
            }
         }
      }

      if( (NULL == this->pData[muiIndex]) && !bImpliedR2LPreFix )
      {
         return( Left( 0 ) );
      }

      return( Left( muiIndex ) );
   }


   // bImpliedL2RPreFix - when the searched character does not exists in the string then what? Return the entire string or return an empty sting?
   // In such case bImpliedL2RPreFix determines to return the entire string or an empty string, e.g. if the char does not exist the function with
   // bImpliedL2RPreFix = true will prefix the char before the string and the function will return the entire string. If false the function will
   // effectively post-fix the character after the string in the direction of search and the function will return an empty string.
   MStringEx< tChar > RightFromL2RNthChar( const TCHAR tchChar, const MUI muiL2RNthCharApearence, const bool bImpliedL2RPreFix ) const
   {
      MUI muiCount( 0 );
      MUI muiIndex( 0 );

      for( ; NULL != this->pData[muiIndex]; muiIndex++ )
      {
         if( tchChar == this->pData[muiIndex] )
         {
            muiCount++;

            if( muiL2RNthCharApearence == muiCount )
            {
               break;
            }
         }
      }

      if( (NULL == this->pData[muiIndex]) && !bImpliedL2RPreFix )
      {
         return( Right( 0 ) );
      }

      return( Right( GetLength() - muiIndex - 1 ) );
   }


   // bImpliedR2LPreFix - when the searched character does not exists in the string then what? Return the entire string or return an empty sting?
   // In such case bImpliedR2LPreFix determines to return the entire string or an empty string, e.g. if the char does not exist the function with
   // bImpliedR2LPreFix = true will prefix the char before the string and the function will return the entire string. If false the function will
   // effectively post-fix the character after the string in the direction of search and the function will return an empty string.
   MStringEx< tChar > RightFromR2LNthChar( const TCHAR tchChar, const MUI muiR2LNthCharApearence, const bool bImpliedR2LPreFix ) const
   {
      MUI muiCount( 0 );
      MUI muiIndex( 0 );

      const MUI muiNthCharApearence( CharAppearenceCount( tchChar ) - muiR2LNthCharApearence + 1 );

      for( ; NULL != this->pData[muiIndex]; muiIndex++ )
      {
         if( tchChar == this->pData[muiIndex] )
         {
            muiCount++;

            if( muiNthCharApearence == muiCount )
            {
               break;
            }
         }
      }

      if( (NULL == this->pData[muiIndex]) && !bImpliedR2LPreFix )
      {
         return( Right( 0 ) );
      }

      return( Right( GetLength() - muiIndex - 1 ) );
   }


   MStringEx< tChar > Right( const MUI muiChars ) const
   {
      MASSERT( GetLength() == FindLength() );

      MStringEx< tChar > strRight( MUnit< tChar >( MMIN< MUI, MUI, MUI >( muiChars, GetLength() ) ) );

      strRight.Transfer( this->SubMemory( this->muSize - strRight.muSize, strRight.muSize - 1 ) );

      MASSERT( strRight.GetLength() == strRight.FindLength() );

      return( strRight );
   }


   MStringEx< tChar > RightFromLeft( const MUI muiCharsOnLeftToOmmit ) const
   {
      MASSERT( GetLength() == FindLength() );

      if( GetLength() <= muiCharsOnLeftToOmmit )
      {
         return( MStringEx< tChar >() );
      }

      MStringEx< tChar > strRight( MUnit< tChar >( MMIN< MUI, MUI, MUI >( GetLength() - muiCharsOnLeftToOmmit, GetLength() ) ) );

      strRight.Transfer( this->SubMemory( MUnit< tChar >( muiCharsOnLeftToOmmit ), strRight.muSize - 1 ) );

      MASSERT( strRight.GetLength() == strRight.FindLength() );

      return( strRight );
   }


   MStringEx< tChar > Mid( const MUI muiStart, const MUI muiChars ) const
   {
      MASSERT( GetLength() == FindLength() );

      if( (GetLength() <= muiStart) || (0 == muiChars) )
      {
         return( MStringEx< tChar >() );
      }

      MStringEx< tChar > strMid( MUnit< tChar >( MMIN< MUI, MUI, MUI >( muiChars, GetLength() - muiStart ) ) );

      strMid.Transfer( this->SubMemory( MUnit< tChar >( muiStart ), strMid.muSize - 1 ) );

      MASSERT( strMid.GetLength() == strMid.FindLength() );

      return( strMid );
   }


   bool IsInterger() const throw()
   {
      return( IsPositiveInterger() || IsNegativeInterger() );
   }


protected:
   BYTE GetBinaryValue( tChar chChar2Value ) const
   {
      switch( chChar2Value )
      {
      case tChar('0'): return( 0 );
      case tChar('1'): return( 1 );
      default:         throw( MStringExException( MStringExException::InvalidCharacter ) );
      }
   }


   BYTE GetOctalValue( tChar chChar2Value ) const
   {
      switch( chChar2Value )
      {
      case tChar('0'): return( 0 );
      case tChar('1'): return( 1 );
      case tChar('2'): return( 2 );
      case tChar('3'): return( 3 );
      case tChar('4'): return( 4 );
      case tChar('5'): return( 5 );
      case tChar('6'): return( 6 );
      case tChar('7'): return( 7 );
      default:         throw( MStringExException( MStringExException::InvalidCharacter ) );
      }
   }


   BYTE GetDecimalValue( tChar chChar2Value ) const
   {
      switch( chChar2Value )
      {
      case tChar('0'): return( 0 );
      case tChar('1'): return( 1 );
      case tChar('2'): return( 2 );
      case tChar('3'): return( 3 );
      case tChar('4'): return( 4 );
      case tChar('5'): return( 5 );
      case tChar('6'): return( 6 );
      case tChar('7'): return( 7 );
      case tChar('8'): return( 8 );
      case tChar('9'): return( 9 );
      default:         throw( MStringExException( MStringExException::InvalidCharacter ) );
      }
   }


   BYTE GetHexadecimalValue( tChar chChar2Value ) const
   {
      switch( chChar2Value )
      {
      case tChar('0'): return( 0 );
      case tChar('1'): return( 1 );
      case tChar('2'): return( 2 );
      case tChar('3'): return( 3 );
      case tChar('4'): return( 4 );
      case tChar('5'): return( 5 );
      case tChar('6'): return( 6 );
      case tChar('7'): return( 7 );
      case tChar('8'): return( 8 );
      case tChar('9'): return( 9 );
      case tChar('a'):
      case tChar('A'): return( 0x0A );
      case tChar('b'):
      case tChar('B'): return( 0x0B );
      case tChar('c'):
      case tChar('C'): return( 0x0C );
      case tChar('d'):
      case tChar('D'): return( 0x0D );
      case tChar('e'):
      case tChar('E'): return( 0x0E );
      case tChar('f'):
      case tChar('F'): return( 0x0F );
      default:         throw( MStringExException( MStringExException::InvalidCharacter ) );
      }
   }


   BYTE GetCharNumericValue( tChar chChar2Value ) const
   {
      switch( eNumericBase )
      {
      case Binary:
         return( GetBinaryValue( chChar2Value ) );

      case Octal:
         return( GetOctalValue( chChar2Value ) );

      case Decimal:
         return( GetDecimalValue( chChar2Value ) );

      case Hexadecimal:
         return( GetHexadecimalValue( chChar2Value ) );
      }
      
      MASSERT( FALSE );

      throw( MStringExException( MStringExException::InvalidNumeralSystemBase ) );
   }


public:
   bool IsSignLessInterger() const throw()
   {
      MASSERT( FindLength() == GetLength() );

      if( 0 == this->pData[0] )
      {
         return( false );
      }

      try
      {
         for( MUI muiIndex = 0; ((0 != this->pData[muiIndex]) && (muiIndex < GetLength())); muiIndex++ )
         {
            GetCharNumericValue( this->pData[muiIndex] );
         }
      }
      catch( ... )
      {
         return( false );
      }
      
      return( true );
   }


   bool IsPositiveInterger() const throw()
   {
      MASSERT( FindLength() == GetLength() );

      if( 0 == this->pData[0] )
      {
         return( false );
      }

      if( (1 == GetLength()) && (tChar('+') == this->pData[0]) )
      {
         return( false );
      }

      try
      {
         for( MUI muiIndex = (tChar('+') == this->pData[0]) ? 1 : 0; ((0 != this->pData[muiIndex]) && (muiIndex < GetLength())); muiIndex++ )
         {
            GetCharNumericValue( this->pData[muiIndex] );
         }
      }
      catch( ... )
      {
         return( false );
      }
      
      return( true );
   }

   bool IsNegativeInterger() const throw()
   {
      MASSERT( FindLength() == GetLength() );

      if( (0 == this->pData[0]) || (0 == this->pData[1]) )
      {
         return( false );
      }

      if( tChar('-') != this->pData[0] )
      {
         return( false );
      }

      try
      {
         for( MUI muiIndex = 1; ((0 != this->pData[muiIndex]) && (muiIndex < GetLength())); muiIndex++ )
         {
            GetCharNumericValue( this->pData[muiIndex] );
         }
      }
      catch( ... )
      {
         return( false );
      }
      
      return( true );
   }


   // Returns true and the value. Returns false and 0 (invalid character) -1 (overflow). The string must be signless.
   bool GetNumericValueNoSign( unsigned __int64 &ui64Result ) const throw()
   {
      try
      {
         const MUI         muiLength( GetLength() );
         unsigned __int64  uiBP( 1 );

         ui64Result = 0;

         for( MUI muiChar = 0; muiChar < muiLength; muiChar++ )
         {
            const unsigned __int64 uiAddPortion( uiBP * GetCharNumericValue( this->pData[muiLength - muiChar - 1] ) );

            if( (((unsigned __int64)-1) - ui64Result) < uiAddPortion )
            {
               ui64Result = (unsigned __int64)-1;

               return( false );
            }
            
            ui64Result += uiAddPortion;
            uiBP *= eNumericBase;
         }
      }
      catch( ... )
      {
         ui64Result  = 0;
         return( false );
      }

      return( true );
   }


#if defined( _AFX ) || defined( _ATL )
   // Returns the CSize object with the exact size of the string in compatible DC.
   CSize GetExtentPixels( HWND hWnd ) const throw()
   {
      HFONT    hWindowFont( (HFONT)::SendMessage( hWnd, WM_GETFONT, 0, 0 ) );

      HDC      hDC( GetDC( hWnd ) );
      HFONT    hfO( (HFONT)SelectObject( hDC, hWindowFont ) );

      CRect rc( 0, 0, 0, 0 );
      ::DrawText( hDC, this->GetMemory(), this->GetLength32(), &rc, DT_EXPANDTABS | DT_CALCRECT );

      SelectObject( hDC, hfO );
      ReleaseDC( hWnd, hDC );

      return( rc.Size() );
   }

   // Returns the bounding rectangle arrownd the text. For example "   MM   " will have bounding ractangle
   // just arround the "MM", or "+" will have bounding rectangle that will include the symbol only - no white
   // spaces (pixels) above, bellow, infront and behing the simbol. The bounding rectangle is in regard to the
   // rectangle determined from MStringEx< tChar >::GetExtentPixels( hWnd ). The result is CRect which must be
   // interpret as:
   //                CRect::left   - white pixels on the left side
   //                CRect::top    - white pixels above the text
   //                CRect::right  - white pixels on the right side
   //                CRect::bottom - white pixels on the bottm side
   // For example CRect::left = 0 means that there is colored pixel in the leftest (zero) colon, i.e. there are no
   //     "Free" (white) colons on the left side of the text
   CRect GetPureBounding( HWND hWnd ) const throw()
   {
      
      HFONT    hWindowFont( (HFONT)::SendMessage( hWnd, WM_GETFONT, 0, 0 ) );

      HDC      hDC( GetDC( hWnd ) );
      HFONT    hfO( (HFONT)SelectObject( hDC, hWindowFont ) );

      const CRect rc( GetPureBounding( hDC ) );

      SelectObject( hDC, hfO );
      ReleaseDC( hWnd, hDC );

      return( rc );
   }

   // For Description of this function see CRect GetPureBounding( const HWND hWnd ) const throw().
   // Please note that the DC must be loaded with the respective font before calling this function or
   // call CRect GetPureBounding( const HWND hWnd ) const throw().
   CRect GetPureBounding( const HDC hDC ) const throw()
   {
      
      CRect rcWhole( 0, 0, 0, 0 );
      ::DrawText( hDC, this->GetMemory(), this->GetLength(), &rcWhole, DT_EXPANDTABS | DT_CALCRECT );

      // Normalize for every case.
      rcWhole.NormalizeRect();

      HDC      hMemDC( CreateCompatibleDC( hDC ) );
      HBITMAP  hDDBitmap( CreateCompatibleBitmap( hDC, rcWhole.Width(), rcWhole.Height() ) );

      HBITMAP  hbOrMDC( (HBITMAP)SelectObject( hMemDC, hDDBitmap ) );
      HFONT    hfOrMDC( (HFONT)SelectObject( hMemDC, GetCurrentObject( hDC, OBJ_FONT ) ) );
      int      icOrTxC( SetTextColor( hMemDC, RGB( 0,       0,    0 ) ) );
      int      icOrBkC( SetBkColor(   hMemDC, RGB( 0xFF, 0xFF, 0xFF ) ) );

      ::FillRect( hMemDC, &rcWhole, (HBRUSH)GetStockObject( WHITE_BRUSH ) );

      const COLORREF rgbClean( GetPixel( hMemDC, 0, 0 ) );

      ::TabbedTextOut( hMemDC, 0, 0, this->GetMemory(), this->GetLength(), 0, NULL, 0 );

      // in this way set the rc to be min left, min top, max right and max bottom.
      CRect rc( rcWhole.right, rcWhole.bottom, rcWhole.left, rcWhole.top );

      for( int iY = rcWhole.top; iY < rcWhole.bottom; iY++ )
      {
         for( int iX = rcWhole.left; iX < rcWhole.right; iX++ )
         {
            if( rgbClean != GetPixel( hMemDC, iX, iY ) )
            {
               // the pixel if not clean so test if it is new extremum
               rc.left   = min( rc.left,   iX );
               rc.top    = min( rc.top,    iY );
               rc.right  = max( rc.right,  iX );
               rc.bottom = max( rc.bottom, iY );
            }
         }
      }

      rc.right  = rcWhole.right  - rc.right  - 1;
      rc.bottom = rcWhole.bottom - rc.bottom - 1;

      if( rcWhole.Width() < (rc.left + rc.right) )
      {
         rc.left  = rcWhole.Width() / 2;
         rc.right = rcWhole.Width() / 2;
      }

      if( rcWhole.Height() < (rc.top + rc.bottom) )
      {
         rc.top    = rcWhole.Height() / 2;
         rc.bottom = rcWhole.Height() / 2;
      }


      SetBkColor(   hMemDC, icOrBkC );
      SetTextColor( hMemDC, icOrTxC );
      SelectObject( hMemDC, hfOrMDC );
      SelectObject( hMemDC, hbOrMDC );

      DeleteObject( hDDBitmap );
      DeleteDC(     hMemDC    );

      return( rc );
   }

   // Returns the CSize object with the exact size of the string in compatible DC. 
   // Make sure you have selected the font to be used for drawing with the
   // Device Context before calling the function.
   CSize GetExtentPixels( HDC hDC ) const throw()
   {
      CRect rc( 0, 0, 0, 0 );

      ::DrawText( hDC, this->GetMemory(), this->GetLength32(), &rc, DT_EXPANDTABS | DT_CALCRECT );

      return( rc.Size() );
   }

   // Returns the CSize object with the exact size of the string in compatible DC.
   CSize GetExtentPixelsMeasure( HDC hDC ) const
   {
      CRect   rcMax( 0, 0, 0, 0 );

      ::DrawText( hDC, this->GetMemory(), this->GetLength(), &rcMax, DT_EXPANDTABS | DT_CALCRECT | DT_LEFT );

      CSize   szMax( rcMax.Size() );

      MASSERT( szMax.cx >= 0 );
      MASSERT( szMax.cy >= 0 );

      if( (szMax.cx <= 0) || (szMax.cy <= 0) ) { return( szMax ); }

      HDC      hdcMem( CreateCompatibleDC( hDC ) );
      HBITMAP  hbTest( CreateCompatibleBitmap( hDC, szMax.cx, szMax.cy ) );
      HBITMAP  hbOrig( (HBITMAP)SelectObject( hdcMem, hbTest ) );
      HFONT    hfOrig( (HFONT)SelectObject( hdcMem, (HFONT)GetCurrentObject( hDC, OBJ_FONT ) ) );

      const COLORREF rgbText( RGB( 0, 0, 0 ) );

      ::FillRect(     hdcMem, rcMax, (HBRUSH)GetStockObject( WHITE_BRUSH ) );

      ::SetTextColor( hdcMem, rgbText );
      ::SetBkColor(   hdcMem, rgbText );
      ::SetBkMode(    hdcMem, OPAQUE  );


      ::DrawText( hdcMem, this->GetMemory(), this->GetLength(), rcMax, DT_EXPANDTABS | DT_LEFT );

      const int iMidY( szMax.cy / 2 );
      rcMax.right -= 1;

      for( ; rcMax.right != 0; rcMax.right-- )
      {
         if( rgbText == GetPixel( hdcMem, rcMax.right, iMidY ) )
         {
            // To keep the returned size as BOUNDING size.
            rcMax.right += 1;

            break;
         }
      }


      if( NULL != hfOrig ) { SelectObject( hdcMem, hfOrig ); }
      if( NULL != hbOrig ) { SelectObject( hdcMem, hbOrig ); }

      if( NULL != hbTest ) { DeleteObject( hbTest ); }
      if( NULL != hdcMem ) { DeleteDC(     hdcMem ); }

      return( rcMax.Size() );
   }


   bool IsContainedInPath( MStringEx< tChar > strPath ) const
   {
      if( strPath.IsEmpty() || IsEmpty() )
      {
         return( false );
      }

      if( strPath.GetLength() == GetLength() )
      {
         if( CompareNoCase( strPath ) )
         {
            MASSERT( FALSE );
            return( true );
         }
      }

      if( TCHAR('\\') != strPath[strPath.GetLength()-1] )
      {
          strPath += TEXT("\\");
      }

      return( StartWith( strPath, false ) );
   }


   // returns true if the dwShellExecuteReturnValue means Ok and false otherwise. The string
   // object will be loaded with the shell execute error or "Ok".
   bool ShellExecuteError( DWORD dwShellExecuteReturnValue )
   {
      if( 32 < dwShellExecuteReturnValue )
      {
         Empty();

         return( true );
      }

      switch( dwShellExecuteReturnValue ) {

         case 0:
            *this = TEXT( "The operating system is out of memory or resources." );
            break;

         case ERROR_FILE_NOT_FOUND:
            // case SE_ERR_FNF:
            *this = TEXT( "The specified file was not found." );
            break;

         case ERROR_PATH_NOT_FOUND:
            // case SE_ERR_PNF:
            *this = TEXT( "The specified path was not found." );
            break;

         case ERROR_BAD_FORMAT:
            *this = TEXT( "The .exe file is invalid (non-Win32® .exe or error in .exe image)." );
            break;

         case SE_ERR_ACCESSDENIED:
            *this = TEXT( "The operating system denied access to the specified file." );
            break;

         case SE_ERR_ASSOCINCOMPLETE:
            *this = TEXT( "The file name association is incomplete or invalid." );
            break;

         case SE_ERR_DDEBUSY:
            *this = TEXT( "The DDE transaction could not be completed because other DDE transactions were being processed." );
            break;

         case SE_ERR_DDEFAIL:
            *this = TEXT( "The DDE transaction failed." );
            break;

         case SE_ERR_DDETIMEOUT:
            *this = TEXT( "The DDE transaction could not be completed because the request timed out." );
            break;

         case SE_ERR_DLLNOTFOUND:
            *this = TEXT( "The specified dynamic-link library was not found." );
            break;

         case SE_ERR_NOASSOC:
            *this = TEXT( "There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable." );
            break;

         case SE_ERR_OOM:
            *this = TEXT( "There was not enough memory to complete the operation." );
            break;

         case SE_ERR_SHARE:
            *this = TEXT( "A sharing violation occurred." );
            break;

         default:
            *this = TEXT( "Undefined Error." );
            break;
      }

      return( false );
   }
#endif


   friend MStringEx< tChar > operator+( const MStringEx< tChar >& strLeft, const tChar* pstrRight )
   {
      if( NULL == pstrRight )
      {
         return( strLeft );
      }

      const ShellMemory< tChar > memRight( (tChar*)pstrRight, MUnit< tChar >( MStringEx< tChar >::FindCharSequenceAsStringLength( pstrRight ) ) );

      MStringEx< tChar > strString( MUnit< tChar >( strLeft.GetLength() + memRight.GetSize().GetUnits() ) );

      strString.Transfer( strLeft );
      strString.Transfer( strLeft.GetSize() - 1, memRight );

      return( strString );
   }


   friend MStringEx< tChar > operator+( const tChar* pstrLeft, const MStringEx< tChar >& strRight )
   {
      if( NULL == pstrLeft )
      {
         return( strRight );
      }

      const ShellMemory< tChar > memLeft( (tChar*)pstrLeft, MUnit< tChar >( MStringEx< tChar >::FindCharSequenceAsStringLength( pstrLeft ) ) );

      MStringEx< tChar > strString( MUnit< tChar >( memLeft.GetSize().GetUnits() ) + strRight.GetLength() );

      strString.Transfer( memLeft );
      strString.Transfer( memLeft.GetSize(), strRight );

      return( strString );
   }


   friend MStringEx< tChar > operator+( const MStringEx< tChar >& strLeft, const MStringEx< tChar >& strRight )
   {
      MStringEx< tChar > strString( MUnit< tChar >( strLeft.GetLength() + strRight.GetLength() ) );

      strString.Transfer( strLeft );
      strString.Transfer( strLeft.GetSize() - 1, strRight );

      return( strString );
   }


#ifdef _WIN32
   MStringEx< tChar >& DerefernceFileSystemObject( const bool bCoInitialize )
   {
      if( bCoInitialize )
      {
         ::CoInitialize( NULL );
      }

      TCHAR tchPath[MAX_PATH];

      try
      {
         SHFILEINFO si = { 0 };
         si.dwAttributes = SFGAO_LINK;

         if( !SHGetFileInfo( this->pData, 0, &si, sizeof( si ), SHGFI_ATTR_SPECIFIED | SHGFI_ATTRIBUTES ) )
         {
            throw( MStringExException( MStringExException::CouldNotDereferenceFileSystemObject ) );
         }

         // if shortcut and request to add the target - dereference
         if( SFGAO_LINK & si.dwAttributes )
         {
            // Dereference the shortcut and see if it is a folder or file
            CComPtr< IShellLink > pShellLink;

            if( FAILED( CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pShellLink ) ) )
            {
               throw( MStringExException( MStringExException::CouldNotDereferenceFileSystemObject ) );
            }

            CComQIPtr< IPersistFile > pPersistFile( pShellLink );

            if( FAILED( pPersistFile->Load( *this, STGM_READ ) ) )
            {
               throw( MStringExException( MStringExException::CouldNotDereferenceFileSystemObject ) );
            }

            if( FAILED( pShellLink->Resolve( NULL, SLR_NOSEARCH | SLR_NOTRACK | SLR_NOUPDATE | SLR_NO_UI ) ) ) 
            {
               throw( MStringExException( MStringExException::CouldNotDereferenceFileSystemObject ) );
            }

            WIN32_FIND_DATA   wfd;

            if( FAILED( pShellLink->GetPath( tchPath, MAX_PATH, &wfd, SLGP_RAWPATH ) ) )
            {
               throw( MStringExException( MStringExException::CouldNotDereferenceFileSystemObject ) );
            }

            *this = tchPath;
         }
      }
      catch( ... )
      {
         if( bCoInitialize )
         {
            ::CoUninitialize();
         }

         throw( MStringExException( MStringExException::CouldNotDereferenceFileSystemObject ) );
      }

      if( bCoInitialize )
      {
         ::CoUninitialize();
      }

      return( *this );
   }


   // The string contains path to the file or folder.
   // const tChar* strPathLink    - filename of the newly created shortcut.
   // const tChar* strDescription - shortcut description.
   HRESULT CreateShortcut( const bool bCoInitialize, const tChar* strPathLink, const tChar* strDescription ) const
   {
      if( bCoInitialize )
      {
         ::CoInitialize( NULL );
      }

      try
      {
         CComPtr< IShellLink > ptrIShellLink; 

         if( FAILED( CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&ptrIShellLink ) ) ) 
         {
            throw( MStringExException( MStringExException::CouldNotCreateFileSystemLink ) );
         }


         ptrIShellLink->SetPath( GetAs< wchar_t >() ); 
         ptrIShellLink->SetDescription( MStringEx< tChar >( strDescription ).GetAs< wchar_t >() ); 

         CComPtr< IPersistFile > ptrIPersistFile; 
         if( FAILED( ptrIShellLink->QueryInterface( IID_IPersistFile, (LPVOID*)&ptrIPersistFile ) ) )
         {
            throw( MStringExException( MStringExException::CouldNotCreateFileSystemLink ) );
         }


         if( FAILED( ptrIPersistFile->Save( MStringEx< tChar >( strPathLink ).GetAs< wchar_t >(), TRUE ) ) )
         {
            throw( MStringExException( MStringExException::CouldNotCreateFileSystemLink ) );
         }

         if( bCoInitialize )
         {
            ::CoUninitialize();
         }
      }
      catch( ... )
      {
         if( bCoInitialize )
         {
            ::CoUninitialize();
         }

         throw( MStringExException( MStringExException::CouldNotCreateFileSystemLink ) );
      }

      return( *this );
   }
#endif


		 using MemoryPH<tChar>::AppendToFile;
   virtual const MStringEx< tChar >& AppendToFile( const TCHAR *ptcFilename, const bool bIncludeZeroTeminator )  const throw()
   {
      if( bIncludeZeroTeminator )
      {
         MemoryPH< tChar >::AppendToFile( ptcFilename );

         return( *this );
      }

      ShellMemory< tChar >( this->pData, this->muSize - 1 ).AppendToFile( ptcFilename );

      return( *this );
   }


   using MemoryPH<tChar>::SaveAsFile;
   virtual bool SaveAsFile( const TCHAR *ptcFilename, const bool bIncludeZeroTeminator, const DWORD dwCreationDisposition, const DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL )  const throw()
   {
      if( bIncludeZeroTeminator )
      {
         return( MemoryPH< tChar >::SaveAsFile( ptcFilename, dwCreationDisposition, dwFlagsAndAttributes, NULL ) );
      }

      return( ShellMemory< tChar >( this->pData, this->muSize - 1 ).SaveAsFile( ptcFilename, dwCreationDisposition, dwFlagsAndAttributes, NULL ) );
   }


   unsigned __int64 MHashString( const bool bCaseSensitive ) const
   {
      // 256 - ignore the BYTE, so there is a bijection
      if( 4000001 - 256 < GetLength() )
      {
         MASSERT( FALSE );

         return( 0 );
      }

      const MStringEx< char > strData( bCaseSensitive ? GetAsMultiByte() : GetAsMultiByte().ToUpper() );
      
      unsigned __int64 uiHash( 0 );

      for( MUI uiChar = 0; uiChar < strData.GetLength(); uiChar++ )
      {
         uiHash += primes4000001[(int)strData.pData[uiChar]] * primes4000001[256 + uiChar];
      }

      return( uiHash );
   }


#if defined( _DEBUG )
   template< class tMemClass > static void Trace( const MAtom< tMemClass >& mem, LPCTSTR strName, const bool bTransposed )
   {
      MStringEx< TCHAR > strTrace( strName );

      for( MUnit< tMemClass > w( 0 ); w < mem.GetSize(); w++ )
      {
         strTrace += ((bTransposed ? "\n" : "    ") +
            MStringEx< TCHAR >( MStringEx< TCHAR >::NumberNoCommas, ((USHORT)(*this)[w]) >> 8, MStringEx< TCHAR >::Hexadecimal ) +
            "-" +
            MStringEx< TCHAR >( MStringEx< TCHAR >::NumberNoCommas, (BYTE)(*this)[w], MStringEx< TCHAR >::Hexadecimal ));
      }

      TRACE( strTrace );
   }
#endif
};


template< class tChar >
class MStringEx2Z : protected MStringEx< tChar >
{
public:
   MStringEx2Z() : MStringEx< tChar >( MUnit< tChar >( 1 ) )
   {
      this->pData[0] = 0;
      this->pData[1] = 0;
   }


   MStringEx2Z( const MStringEx< tChar > strSource )
      :  MStringEx< tChar >( strSource.GetSize() )
   {
      Transfer( strSource );

      this->pData[this->muSize.GetUnits()-2] = 0;
      this->pData[this->muSize.GetUnits()-1] = 0;
   }


   MStringEx2Z( const MStringEx2Z& strSource )
      :  MStringEx< tChar >( strSource.GetSize() - 1 )
   {
      Transfer( strSource );
   }


   MStringEx2Z< tChar >& operator+=( const MStringEx< tChar >& strAdd2This )
   {
      MStringEx< tChar > strString( MUnit< tChar >( GetLength() + 1 + strAdd2This.GetLength() + 1 ) );

      strString.Transfer( *this );
      strString.Transfer( this->muSize - 1, strAdd2This );

      this->ExchangeMemory( strString );

      return( *this );
   }


   MUI GetLength() const
   {
      return( (this->muSize - 2).GetUnits() );
   }


   bool IsEmpty() const
   {
      return( 2 == this->muSize );
   }


   const tChar* GetMemory() const
   {
      return( this->pData );
   }


   tChar* GetMemory()
   {
      return( this->pData );
   }


   operator const tChar*() const
   {
      return( this->pData );
   }


   operator tChar* ()
   {
      return( this->pData );
   }
};


template< class tChar >
class MSecureString : protected MStringEx< tChar >
{
private:
   SecureMemory< ShellMemory< tChar >, tChar > memSecureShell;


public:
   MSecureString()
   {
      memSecureShell = *this;
   }


   // the value is the length of the string without the ending zero, so for empty string ask for 0.
   MSecureString( const MUnit< tChar >& mutChars )
      :  MStringEx< tChar >( mutChars )
   {
      memSecureShell = *this;
   }


#ifdef _WIN32
   MSecureString( const HWND hWnd )
      :  MStringEx< tChar >( hWnd )
   {
      memSecureShell = *this;
   }
#endif


   MSecureString( const tChar* strSecureString )
      :  MStringEx< tChar >( strSecureString )
   {
      memSecureShell = *this;
   }


   MSecureString( const MSecureString< tChar >& strSecureString )
      :  MStringEx< tChar >( strSecureString )
   {
      memSecureShell = *this;
   }


   ~MSecureString()
   {
   }


   MSecureString< tChar >& operator=( const MSecureString< tChar >& objMStringEx )
   {
      memSecureShell.Empty();

      if( objMStringEx.muSize != this->muSize )
      {
         ReAllocate( objMStringEx.muSize );
      }

      Transfer( objMStringEx );

      this->eNumericBase = objMStringEx.eNumericBase;

      memSecureShell = *this;

      return( *this );
   }


   operator const tChar*() const
   {
      return( this->pData );
   }


   bool IsEmpty() const
   {
      return( MStringEx< tChar >::IsEmpty() );
   }


   MAtom< tChar >& Empty()
   {
      memSecureShell.Empty();

      MStringEx< tChar >::Empty();

      memSecureShell = *this;

      return( *this );
   }


   const tChar* GetMemory() const
   {
      return( this->pData );
   }


   bool operator==( const MSecureString< tChar >& strSecureToCompare ) const
   {
      return( MStringEx< tChar >::operator==( strSecureToCompare ) );
   }


   bool operator!=( const MSecureString< tChar >& strSecureToCompare ) const
   {
      return( MStringEx< tChar >::operator!=( strSecureToCompare ) );
   }


   MSecureString< tChar >& ToLower()
   {
      MStringEx< tChar >::ToLower();

      return( *this );
   }


   MSecureString< tChar >& ToUpper()
   {
      MStringEx< tChar >::ToUpper();

      return( *this );
   }


   MSecureString< tChar >& RemoveChars( const MStringEx< tChar >& strChars2Remove, const MUI muiStartPos, const MUI muiCharsCount ) throw()
   {
      MStringEx< tChar > strNewString( MUnit< tChar >( this->GetLength() ) );

      MUI muiDstIndex = 0;
      const MUI muiChars2RemoveLength( strChars2Remove.GetLength() );
      const unsigned __int64 uiLastCharPos( (unsigned __int64)muiStartPos + (unsigned __int64)muiCharsCount );


      for( MUI muiThisIndex = 0; muiThisIndex < GetLength(); muiThisIndex++ )
      {
         bool bRemove = false;

         if( (muiStartPos <= muiThisIndex) && (muiThisIndex < uiLastCharPos) )
         {
            for( MUI muiChars = 0; muiChars < muiChars2RemoveLength; muiChars++ )
            {
               if( strChars2Remove[muiChars] == this->pData[muiThisIndex] )
               {
                  bRemove = true;

                  break;
               }
            }
         }

         if( bRemove )
         {
            continue;
         }

         strNewString[muiDstIndex++] = this->pData[muiThisIndex];
      }

      strNewString[muiDstIndex] = 0;

      strNewString.LimitSizeTo( MUnit< tChar >( muiDstIndex + 1 ) );

      MASSERT( strNewString.GetLength() == strNewString.FindLength() );

      this->ExchangeMemory( strNewString );

      memSecureShell = *this;

      return( *this );
   }


   MSecureString< char > GetAsMultiByte( UINT uiCodePage = CP_ACP ) const throw()
   {
      if( !this->IsWchar() )
      {
         // The object is multi-byte itself.
         return( MSecureString< char >( (char*)this->pData ) );
      }

#ifdef _WIN32
      if( !IsValidCodePage( uiCodePage) )
      {
         uiCodePage = CP_ACP;
      }


      MUnit< char > muSizeRequired;


      BYTE* ptrDataCopy( (BYTE*)this->pData );
      const BYTE* ptrLastData( (BYTE*)((MUI)this->pData + this->muSize.InBytes()) );


      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 2 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 2 ) )
      {
         muSizeRequired += MUnit< char >( WideCharToMultiByte( uiCodePage, 0, (LPCWSTR)ptrDataCopy, dwChunkSize / 2, NULL, 0, NULL, NULL ) );
      }


      MSecureString< char > strString( muSizeRequired - 1 );
      muSizeRequired = 0;
      ptrDataCopy = (BYTE*)this->pData;


      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 2 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 2 ) )
      {
         const char* pNewString = &strString.GetMemory()[muSizeRequired.GetUnits()];

         muSizeRequired += MUnit< char >( WideCharToMultiByte( uiCodePage, 0, (LPCWSTR)ptrDataCopy, dwChunkSize / 2, (char*)pNewString, MMIN< MUnit< char >, MUnit< char >, MUnit< char > >( strString.GetSize() - muSizeRequired, MUnit< char >( dwChunkSize / 2 ) ).GetUnits32(), NULL, NULL ) );
      }


      return( strString );
#else
      MSecureString< char > strString( MUnit< char >( this->GetLength() ) );
      
      wcstombs( (char*)strString.GetMemory(), (const wchar_t*)this->pData, strString.GetSize().GetUnits() );
      
      ((char*)strString.GetMemory())[strString.GetSize().GetUnits() - 1] = 0;
      
      return( strString );
#endif
   }


   MUI GetLength() const
   {
      return( MStringEx< tChar >::GetLength() );
   }


   MUnit< tChar > GetSize() const
   {
      return( MStringEx< tChar >::GetSize() );
   }


   template< class tClass >
   ShellMemory< tClass > GetMemoryAtom()
   {
      return( this->ShellMemory( (tClass*)this->pData, GetSize().template As< tClass >() ) );
   }
};


typedef MStringEx< char >    string_c;
typedef MStringEx< wchar_t > string_w;
typedef MStringEx2Z< char >  string2z;

typedef string_c str;


#ifdef _WIN32
   #pragma warning( pop )
#endif
