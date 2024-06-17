//
//
// MHandle.h: interface for the MHandle class.
//
// © Copyright 2007 - 2012 by Miroslav Bonchev Bonchev. All rights reserved.
// 
//
//************************************************************************************


#pragma once


#include "Common.h"
#include "WinInet.h"


class MHandle
{
private:
   HANDLE hHandle;

public:
   MHandle() : hHandle( INVALID_HANDLE_VALUE )
   {
   }

   MHandle( const HANDLE& hHandle ) : hHandle( hHandle )
   {
   }

private:
   //does not work
   MHandle( MHandle& mHandle ) : hHandle( INVALID_HANDLE_VALUE )
   {
      DuplicateHandle( GetCurrentProcess(), mHandle.hHandle, GetCurrentProcess(), &hHandle, 0, TRUE, DUPLICATE_SAME_ACCESS );
   }

   //does not work
   MHandle& operator=( MHandle& mHandle )
   {
      DuplicateHandle( GetCurrentProcess(), mHandle.hHandle, GetCurrentProcess(), &hHandle, 0, TRUE, DUPLICATE_SAME_ACCESS );

      return( *this );
   }

public:
   MHandle& operator=( const HANDLE& handle )
   {
      Close();

      this->hHandle = handle;

      return( *this );
   }

   ~MHandle()
   {
      Close();
   }

   void Close()
   {
      if( !IsValid() )
      {
         return;
      }
      
      CloseHandle( hHandle );

      hHandle = INVALID_HANDLE_VALUE;
   }

   operator HANDLE () throw()
   {
      return( hHandle );
   }

   operator const HANDLE () const throw()
   {
      return( hHandle );
   }

   operator HANDLE* () throw()
   {
      return( &hHandle );
   }


   bool IsValid() const
   {
      if( INVALID_HANDLE_VALUE == hHandle )
      {
         return( false );
      }
      
      return( NULL != hHandle );
   }

   bool HasMoreData() const
   {
      LARGE_INTEGER fpFileSize = { 0 };
      LARGE_INTEGER fpFilePointer;
      
      SetFilePointerEx( hHandle, fpFileSize, &fpFilePointer, FILE_CURRENT );

      GetFileSizeEx( hHandle, &fpFileSize );

      return( fpFilePointer.QuadPart < fpFileSize.QuadPart );
   }

   HANDLE ExportHandle()
   {
      HANDLE h = hHandle;

      hHandle = INVALID_HANDLE_VALUE;

      return( h );
   }

   void SetNoCloseHandle() const
   {
      *const_cast< HANDLE* >( &hHandle ) = INVALID_HANDLE_VALUE;
   }
};


class MModuleHandle
{
private:
   HMODULE hHandle;


public:
   MModuleHandle() : hHandle( NULL )
   {
   }


   MModuleHandle( const HMODULE& hHandle ) : hHandle( hHandle )
   {
   }


public:
   MModuleHandle& operator=( const HMODULE& handle )
   {
      Close();

      this->hHandle = handle;

      return( *this );
   }


   ~MModuleHandle()
   {
      Close();
   }


   void Close()
   {
      if( !IsValid() )
      {
         return;
      }
      
      FreeLibrary( hHandle );

      hHandle = NULL;
   }


   operator HMODULE () const throw()
   {
      return( hHandle );
   }


   bool IsValid() const
   {
      return( NULL != hHandle );
   }


   HMODULE ExportHandle()
   {
      HMODULE h = hHandle;

      hHandle = NULL;

      return( h );
   }
};


class MInternetHandle
{
private:
   HINTERNET hHandle;


public:
   MInternetHandle() : hHandle( NULL )
   {
   }


   MInternetHandle( const HINTERNET& hHandle ) : hHandle( hHandle )
   {
   }


public:
   MInternetHandle& operator=( const HINTERNET& handle )
   {
      Close();

      this->hHandle = handle;

      return( *this );
   }


   ~MInternetHandle()
   {
      Close();
   }


   void Close()
   {
      if( !IsValid() )
      {
         return;
      }
      
      InternetCloseHandle( hHandle );

      hHandle = NULL;
   }


   operator HINTERNET () const throw()
   {
      return( hHandle );
   }


   bool IsValid() const
   {
      return( NULL != hHandle );
   }


   HINTERNET ExportHandle()
   {
      HINTERNET h = hHandle;

      hHandle = NULL;

      return( h );
   }
};


class MFindHandle
{
   HANDLE hFind;


public:
   MFindHandle() :  hFind( INVALID_HANDLE_VALUE )
   {
   }


   MFindHandle( const HANDLE hFind ) : hFind( hFind )
   {
   }
   

   ~MFindHandle()
   {
      Close();
   }


   void Close()
   {
      if( IsValid() )
      {
         FindClose( hFind );

         hFind = INVALID_HANDLE_VALUE;
      }
   }
   

   bool IsValid() const
   {
      return( (INVALID_HANDLE_VALUE != hFind) && (NULL != hFind) );
   }


   bool FindNextFile( WIN32_FIND_DATA& wfd )
   {
      return( 0 != ::FindNextFile( hFind, &wfd ) );
   }
};


class MChangeHandle
{
   HANDLE hChange;


public:
   MChangeHandle() : hChange( INVALID_HANDLE_VALUE )
   {
   }


   MChangeHandle( const HANDLE hChange ) : hChange( hChange )
   {
   }
   

   ~MChangeHandle()
   {
      Close();
   }


   HANDLE operator=( const HANDLE& h_change )
   {
      Close();

      this->hChange = h_change;

      return( this->hChange );
   }


   void Close()
   {
      if( IsValid() )
      {
         FindCloseChangeNotification( hChange );

         hChange = INVALID_HANDLE_VALUE;
      }
   }
   

   bool IsValid() const
   {
      return( (INVALID_HANDLE_VALUE != hChange) && (NULL != hChange) );
   }


   bool FindNextChange()
   {
      return( 0 != ::FindNextChangeNotification( hChange ) );
   }


   HANDLE ExportHandle()
   {
      HANDLE h = hChange;

      hChange = INVALID_HANDLE_VALUE;

      return( h );
   }


   operator const HANDLE () const
   {
      return( hChange );
   }
};
