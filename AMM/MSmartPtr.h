//
//
// MSmartPtr.h: interface for the MSmartPtr class.
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase One'
//
// © Copyright 2008 - 2014 by Miroslav Bonchev Bonchev. All rights reserved.
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


class MSmartPtrError
{
public:
   enum PointerError
   {
      errInvalidPointer,
      errSystemError
   };

private:
   PointerError eGeneralFault;
   DWORD        dwSystemError;


public:
   MSmartPtrError( const PointerError errPointer ) : eGeneralFault( errPointer ) {}
   MSmartPtrError( const PointerError errPointer, const DWORD dwSystemERROR ) : eGeneralFault( errPointer ), dwSystemError( dwSystemERROR ) {}
   ~MSmartPtrError() {}

   bool operator==( const MSmartPtrError& errPointerException ) const throw() { return( errPointerException.eGeneralFault == eGeneralFault ); }
   bool operator!=( const MSmartPtrError& errPointerException ) const throw() { return( !operator==( errPointerException ) ); }

   friend bool operator==( const MSmartPtrError& errPointerException, const PointerError& errPointerError ) throw() { return( errPointerError == errPointerException.eGeneralFault ); }
   friend bool operator==( const PointerError& errPointerError, const MSmartPtrError& errPointerException ) throw() { return( errPointerError == errPointerException.eGeneralFault ); }

   friend bool operator!=( const MSmartPtrError& errPointerException, const PointerError& errPointerError ) throw() { return( errPointerError != errPointerException.eGeneralFault ); }
   friend bool operator!=( const PointerError& errPointerError, const MSmartPtrError& errPointerException ) throw() { return( errPointerError != errPointerException.eGeneralFault ); }

   PointerError GetGeneralFault() const throw() { return( eGeneralFault ); }
   DWORD        GetSystemError()  const throw() { return( dwSystemError ); }
};


template< class tPointer >
class MSmartPtr
{
private:
   tPointer* tPtr;


public:
   MSmartPtr() : tPtr( NULL )
   {
   }


   MSmartPtr( tPointer* ptr ) throw() : tPtr( ptr )
   {
   }


   void Close() throw()
   {
      if( NULL != tPtr )
      {
         delete tPtr;

         tPtr = NULL;
      }
   }


   ~MSmartPtr() throw()
   {
      Close();
   }


   MSmartPtr& operator=( tPointer* ptrNew ) throw()
   {
      Close();

      tPtr = ptrNew;

      return( *this );
   }


   void Empty() throw()
   {
      Close();
   }


   void EmptyNoDestroy() throw()
   {
      tPtr = NULL;
   }


   bool IsEmpty() const throw()
   {
      return( NULL == tPtr );
   }


   tPointer* ExportObject()
   {
      tPointer* tPtr2 = tPtr;

      tPtr = NULL;

      return( tPtr2 );
   }


   operator tPointer* ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }


   operator const tPointer* () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }


   operator tPointer& ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }


   operator const tPointer& () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }


   tPointer* operator -> ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }


   const tPointer* operator -> () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }


   bool operator< ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr < *tPtrR );
   }


   bool operator> ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr > *tPtrR );
   }


   bool operator<= ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr <= *tPtrR );
   }


   bool operator>= ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr >= *tPtrR );
   }


   bool operator==( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr == *tPtrR );
   }


   bool operator!= ( const tPointer* tPtrR )
   {
      return( !operator==( tPtrR ) );
   }
};


template< class tPointer >
class MRefPtr
{
private:
   tPointer* tPtr;

public:
   MRefPtr() : tPtr( NULL )
   {
   }


   MRefPtr( const MRefPtr& refPtr ) : tPtr( refPtr.tPtr )
   {
      if( NULL != tPtr )
      {
         tPtr->AddRef();
      }
   }


   MRefPtr( tPointer* ptr ) throw() : tPtr( ptr )
   {
      if( NULL != tPtr )
      {
         tPtr->AddRef();
      }
   }


   void Close() throw()
   {
      if( NULL != tPtr )
      {
         tPtr->Release();

         tPtr = NULL;
      }
   }

   ~MRefPtr()
   {
      Close();
   }


   MRefPtr& operator=( tPointer* ptrNew ) throw()
   {
      Close();

      MASSERT( NULL != ptrNew );

      tPtr = ptrNew;

      return( *this );
   }

   bool IsEmpty() const throw()
   {
      return( NULL == tPtr );
   }

   tPointer* ExportObject()
   {
      tPointer* tPtr2 = tPtr;

      tPtr = NULL;

      return( tPtr2 );
   }

   operator tPointer* ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }

   operator const tPointer* () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }

   operator tPointer& ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }

   operator const tPointer& () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }

   tPointer* operator -> ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }

   const tPointer* operator -> () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }

   bool operator< ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr < *tPtrR );
   }

   bool operator> ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr > *tPtrR );
   }

   bool operator<= ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr <= *tPtrR );
   }

   bool operator>= ( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr >= *tPtrR );
   }

   friend bool operator==( const MRefPtr< tPointer >& prtLeft, const MRefPtr< tPointer >& ptrRight )
   {
      return( prtLeft.tPtr == ptrRight.tPtr );
   }

   bool operator==( const tPointer* tPtrR )
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      if( NULL == tPtrR )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr == *tPtrR );
   }

   bool operator!= ( const tPointer* tPtrR )
   {
      return( !operator==( tPtrR ) );
   }
};
