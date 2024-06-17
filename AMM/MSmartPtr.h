//
//
// MSmartPtr.h: interface for the MSmartPtr class.
//
// © Copyright 2008 - 2012 by Miroslav Bonchev Bonchev. All rights reserved.
// 
//
//************************************************************************************


#pragma once


#include "Common.h"


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
   typename tPointer* tPtr;


public:
   MSmartPtr() : tPtr( NULL )
   {
   }


   MSmartPtr( typename tPointer* ptr ) throw() : tPtr( ptr )
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


   MSmartPtr& operator=( typename tPointer* ptrNew ) throw()
   {
      Close();

      tPtr = ptrNew;

      return( *this );
   }


   void Empty() throw()
   {
      Close();
   }


   bool IsEmpty() const throw()
   {
      return( NULL == tPtr );
   }


   typename tPointer* ExportObject()
   {
      typename tPointer* tPtr2 = tPtr;

      tPtr = NULL;

      return( tPtr2 );
   }


   operator typename tPointer* ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }


   operator typename const tPointer* () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }


   operator typename tPointer& ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }


   operator typename const tPointer& () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }


   typename tPointer* operator -> ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }


   typename const tPointer* operator -> () const
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
   typename tPointer* tPtr;

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


   MRefPtr( typename tPointer* ptr ) throw() : tPtr( ptr )
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


   MRefPtr& operator=( typename tPointer* ptrNew ) throw()
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

   typename tPointer* ExportObject()
   {
      typename tPointer* tPtr2 = tPtr;

      tPtr = NULL;

      return( tPtr2 );
   }

   operator typename tPointer* ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }

   operator typename const tPointer* () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }

   operator typename tPointer& ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }

   operator typename const tPointer& () const
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( *tPtr );
   }

   typename tPointer* operator -> ()
   {
      if( NULL == tPtr )
      {
         throw( MSmartPtrError( MSmartPtrError::errInvalidPointer ) );
      }

      return( tPtr );
   }

   typename const tPointer* operator -> () const
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
