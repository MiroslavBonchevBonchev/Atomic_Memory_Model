//
//
// MHandle.h: interface for the MHandle class.
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase One'
//
// © Copyright 2007 - 2014 by Miroslav Bonchev Bonchev. All rights reserved.
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


#include "CommonAMM.h"


class MHandle;


class MRefHandle
{
protected:
   HANDLE hHandle;

public:
   MRefHandle() : hHandle( INVALID_HANDLE_VALUE )
   {
   }

   MRefHandle( HANDLE hHandle ) : hHandle( hHandle )
   {
   }

   MRefHandle( MRefHandle& mHandle ) : hHandle( mHandle.hHandle )
   {
   }

   MRefHandle( MHandle& mHandle );


   MRefHandle& operator=( MRefHandle& mHandle )
   {
      Close();

      this->hHandle = mHandle.hHandle;

      return( *this );
   }

   MRefHandle& operator=( MHandle& mHandle );

   MRefHandle& operator=( HANDLE& handle )
   {
      Close();

      this->hHandle = handle;

      return( *this );
   }

   virtual ~MRefHandle()
   {
      Close();
   }

   virtual void Close()
   {
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
};


class MHandle : public MRefHandle
{
public:
   MHandle()
   {
   }

   MHandle( HANDLE hHandle ) : MRefHandle( hHandle )
   {
   }

   MHandle( MRefHandle& mHandle ) : MRefHandle( mHandle )
   {
   }

public:
   MHandle& operator=( HANDLE handle )
   {
      Close();

      this->hHandle = handle;

      return( *this );
   }

   virtual ~MHandle()
   {
      Close();
   }

   virtual void Close()
   {
      if( !IsValid() )
      {
         return;
      }
      
#ifdef _WIN32
      CloseHandle( hHandle );
#else
      fclose( hHandle );
#endif

      hHandle = INVALID_HANDLE_VALUE;
   }
};


inline MRefHandle::MRefHandle( MHandle& mHandle ) : hHandle( (HANDLE)mHandle )
{
}


inline MRefHandle& MRefHandle::operator=( MHandle& mHandle )
{
    Close();

    this->hHandle = (HANDLE)mHandle;

    return( *this );
}


class MModuleHandle
{
private:
   HMODULE hHandle;


public:
   MModuleHandle() : hHandle( NULL )
   {
   }


   MModuleHandle( HMODULE hHandle ) : hHandle( hHandle )
   {
   }


public:
   MModuleHandle& operator=( HMODULE handle )
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
      
#ifdef _WIN32
      FreeLibrary( hHandle );
#endif

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
