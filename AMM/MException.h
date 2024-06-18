//
//
// MAtom.h: memory atoms definitions as example of implementation of:
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase Two'
//
// © Copyright 2005 - 2014 by Miroslav Bonchev Bonchev. All rights reserved.
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


#pragma warning( push )
#pragma warning( disable : 4290 )   // warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)


typedef size_t MUI;


template< class tMin, bool bIncludingMin, class tArg, bool bIncludingMax, class tMax > bool IsInRange( const tMin &objMin, const tArg &objArg, const tMax &objMax ) throw()
{
   return( ( bIncludingMin ? objMin <= objArg : objMin < objArg ) && ( bIncludingMax ? objArg <= objMax : objArg < objMax ) );   
}


template< class tRet, class tV1, class tV2 > tRet MMAX( const tV1& v1, const tV2& v2 )
{
   if( v1 > v2 ) { return( (tRet)v1 ); } return( (tRet)v2 );
}


template< class tRet, class tV1, class tV2 > tRet MMIN( const tV1& v1, const tV2& v2 )
{
   if( v1 < v2 ) { return( (tRet)v1 ); } return( (tRet)v2 );
}


template< class tRet, class tV1, class tV2 > tRet MABSmat( const tV1& v1, const tV2& v2 )
{
   return( v1 > v2 ? v1 - v2 : v2 - v1  );
}


template< class tRet, class tArg > tRet MABSsat( const tArg& tArgument )
{
   return( (tRet)( tArgument < 0 ? (-tArgument) : tArgument ) );
}


// The MUnit exception is defined outside of the MUnit (not nested, i.e. name-spaced) because if it was nested there
// would be need from a catch( MUnit< tMemType >::MUnitException ) for every tMemType that is being used.
// Use catch( const MUnitException e ) to capture MUnitExceptions.
class MUnitException
{
public:
   enum Error
   {
      ePPOverflow,
      eMMOverflow,
      eAddOverflow,
      eMulOverflow,
      eSubOverflow,
      eDivOverflow,
      eIntegerDivisionByZero,
      InsufficientTarget
   };


private:
   Error eError;


private:
   MUnitException( const Error excError )                    throw() : eError( excError )
   {
   }


public:
   MUnitException( const MUnitException& objMUnitException ) throw() : eError( objMUnitException.eError )
   {
   }


   ~MUnitException()
   {
   }


   MUnitException& operator=( const MUnitException& objMUnitException ) throw()
   {
      eError = objMUnitException.eError;

      return( *this );
   }


public:
   __forceinline static void TestTargetSize32( const MUI muiUnits ) DECLARE_THROW( MUnitException )
   {
      if( muiUnits > ((DWORD)-1) )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::InsufficientTarget ) );
      }
   }


   __forceinline static void TestPPOverflow( const MUI muiUnits ) DECLARE_THROW( MUnitException )
   {
      if( 0 == muiUnits )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::ePPOverflow ) );
      }
   }


   __forceinline static void TestMMOverflow( const MUI muiUnits ) DECLARE_THROW( MUnitException )
   {
      if( (MUI)-1 == muiUnits )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::eMMOverflow ) );
      }
   }


   __forceinline static void TestAdd( const MUI muiSummandL, const MUI muiSummandR   ) DECLARE_THROW( MUnitException )
   {
      if( ((MUI)-1 - muiSummandL) < muiSummandR )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::eAddOverflow ) );
      }
   }
   
   
   __forceinline static void TestMul( const MUI muiFactorL,  const MUI muiFactorR ) DECLARE_THROW( MUnitException )
   {
      if( 0 == muiFactorL )
      {
         return;
      }

      if( 0 == muiFactorR )
      {
         return;
      }

      const MUI muiResult( muiFactorL * muiFactorR );

      if( (muiResult < muiFactorL) || (muiResult < muiFactorR) )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::eMulOverflow ) );
      }
   }
   
   
   __forceinline static void TestSub( const MUI muiMinuend,  const MUI muiSubtrahend ) DECLARE_THROW( MUnitException )
   {
      if( muiMinuend < muiSubtrahend )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::eSubOverflow ) );
      }
   }
   
   
   __forceinline static void TestDiv( const MUI muiDividend, const MUI muiDivisor    ) DECLARE_THROW( MUnitException )
   {
      if( 0 == muiDivisor )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::eIntegerDivisionByZero ) );
      }
      
      if( 0 != (muiDividend % muiDivisor) )
      {
         MASSERT( FALSE );

         // MUnitException_throw( MUnitException( MUnitException::eDivOverflow ) );
      }
   }


public:
   Error GetExceptionErrorCode() const throw() { return( eError ); }
};


template< class tMemClass > class MUnit;


// Exceptions thrown by memory atoms if errors are signaled using exception handling.
// The MAtom exception is defined outside of the MAtom class to avoid an unnecessary exception handler
// for every specialization of memory atoms used in the try block e.g. catch( MAtom< tMemClass >::MAtomException ).
// Use catch( const MAtomException e ) to capture MAtomExceptions.
// Use MAtomException::Error MAtomException::GetExceptionError() const to retrieve the exception error for bad calls.
// Use DWORD MAtomException::GetSystemError() const to retrieve system errors.
class MAtomException
{
public:
   enum Error
   {
      eInvalidPointerNull,
      eZero2UpperLimitOutOfRange,
      eIlligalMethodCall,
      eInvalidCall,
      eFailedToOperate,
      eIncompatibleCall,
      eOutofTargetMemorySpace,
      eOutofSourceMemorySpace,
      eOutOfMemory,
      eAccessViolation,
      eInvalidData
   };


private:
   Error eError;
   DWORD dwSysError;


private:
   MAtomException( const Error excError ) throw() : eError( excError ), dwSysError( ERROR_SUCCESS )
   {
   }


   MAtomException( const Error excError, const DWORD dwSysError ) throw() : eError( excError ), dwSysError( dwSysError)
   {
   }


public:
   MAtomException( const MAtomException& objMAtomException ) throw() : eError( objMAtomException.eError ), dwSysError( objMAtomException.dwSysError )
   {
   }


   ~MAtomException()
   {
   }


   MAtomException& operator=( const MAtomException& objMAtomException ) throw()
   {
      eError     = objMAtomException.eError;
      dwSysError = objMAtomException.dwSysError;

      return( *this );
   }


public:
   Error GetExceptionErrorCode() const
   {
      return( eError );
   }


   DWORD GetSystemErrorCode() const
   {
      return( dwSysError );
   }


public:
   __forceinline static void TestNonNullPointer( const void* pvPointer )  DECLARE_THROW( MAtomException )
   {
      if( NULL == pvPointer )
      {
         MASSERT( FALSE );
      
         MAtomException_throw( MAtomException( MAtomException::eInvalidPointerNull ) );
      }
   }
   
   
   __forceinline static void TestCompatibleCall( const bool bCompatible ) DECLARE_THROW( MAtomException )
   {
      if( !bCompatible )
      {
         MASSERT( FALSE );
         
         MAtomException_throw( MAtomException( MAtomException::eIncompatibleCall ) );
      }
   }
   
   
   __forceinline static void ExceptionIlligalMethodCall() DECLARE_THROW( MAtomException )
   {
      MASSERT( FALSE );
      
      MAtomException_throw( MAtomException( MAtomException::eIlligalMethodCall ) );
   }


   __forceinline static void ExceptionInvaldCall( const DWORD 
      #if defined( ENABLE_MAtomException )
         dwSystemError
      #endif
      ) DECLARE_THROW( MAtomException )
   {
      MASSERT( FALSE );

      MAtomException_throw( MAtomException( MAtomException::eInvalidCall, dwSystemError ) );
   }
   
   
   __forceinline static void ExceptionFailedToOperate( const DWORD 
      #if defined( ENABLE_MAtomException )
            dwSystemError
      #endif
      ) DECLARE_THROW( MAtomException )
   {
      MASSERT( FALSE );

      MAtomException_throw( MAtomException( MAtomException::eFailedToOperate, dwSystemError ) );
   }


   __forceinline static void ExceptionInsufficientMemory() DECLARE_THROW( MAtomException )
   {
      MASSERT( FALSE );
      
      MAtomException_throw( MAtomException( MAtomException::eOutOfMemory, ERROR_NOT_ENOUGH_MEMORY ) );
   }


   __forceinline static void ExceptionAccessViolation() DECLARE_THROW( MAtomException )
   {
      MASSERT( FALSE );
      
      MAtomException_throw( MAtomException( MAtomException::eAccessViolation, ERROR_ACCESS_DENIED ) );
   }


   __forceinline static void ExceptionInvalidData() DECLARE_THROW( MAtomException )
   {
      MASSERT( FALSE );
      
      MAtomException_throw( MAtomException( MAtomException::eInvalidData, ERROR_INVALID_DATA ) );
   }


   template< bool bZeroInRange, bool bUpperLimitInRange > __forceinline static void TestInRangeZero2UpperLimit( const MUI muiIndex, const MUI muiUpperLimit ) DECLARE_THROW( MAtomException )
   {
      if( !IsInRange< MUI, bZeroInRange, MUI, bUpperLimitInRange, MUI >( 0, muiIndex, muiUpperLimit ) )
      {
         MASSERT( FALSE );

         MAtomException_throw( MAtomException( MAtomException::eZero2UpperLimitOutOfRange ) );
      }
   }


   template< class tMemClass > __forceinline static void TestTargetMemorySpace( const MUnit< tMemClass >& muOffset, const MUnit< tMemClass >& muTransfer, const MUnit< tMemClass >& muSize ) DECLARE_THROW( MAtomException )
   {
      if( (muOffset + muTransfer) > muSize )
      {
         MASSERT( FALSE );
         
         MAtomException_throw( MAtomException( MAtomException::eOutofTargetMemorySpace ) );
      }
   }


   template< class tMemClass > __forceinline static void TestSourceMemorySpace( const MUnit< tMemClass >& muOffset, const MUnit< tMemClass >& muTransfer, const MUnit< tMemClass >& muSize ) DECLARE_THROW( MAtomException )
   {
      if( (muOffset + muTransfer) > muSize )
      {
         MASSERT( FALSE );
         
         MAtomException_throw( MAtomException( MAtomException::eOutofSourceMemorySpace ) );
      }
   }


   template< class tMemClass, class tMemCast > __forceinline static void TestSourceMemorySpaceCast( const MUnit< tMemClass >& muOffset, const MUnit< tMemCast >& muUnitsCast, const MUnit< tMemClass >& muSize ) DECLARE_THROW( MAtomException )
   {
      if( (muOffset.InBytes() + muUnitsCast.InBytes()) > muSize.InBytes() )
      {
         MASSERT( FALSE );
         
         MAtomException_throw( MAtomException( MAtomException::eOutofSourceMemorySpace ) );
      }
   }
};


// MStringEx exception
class MStringExException
{
public:
   enum Exception
   {
      CouldNotLoadResource,
      CouldNotDereferenceFileSystemObject,
      CouldNotCreateFileSystemLink,
      InvalidCharacter,
      InvalidNumeralSystemBase
   } eException;


public:
   MStringExException( const Exception eException ) : eException( eException ) {}
   ~MStringExException() {}


public:
   Exception GetExceptionErrorCode() const throw() { return( eException ); }
};


#pragma warning( pop )
