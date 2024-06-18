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


// Open Source License - The MIT License
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
#include "MException.h"


#pragma warning( push )
#pragma warning( disable : 4290 )   // warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)


template< class tMemClass >
class MUnit
{
protected:
   MUI muiUnits;


public:
   MUnit() : muiUnits( 0 )
   {
   }


   MUnit( const MUnit< tMemClass >& objMUnit ) : muiUnits( objMUnit.muiUnits )
   {
   }


   explicit MUnit( const MUI muiNumberOfUnits ) : muiUnits( muiNumberOfUnits )
   {
   }


   ~MUnit()
   {
   }


   MUnit< tMemClass >& operator=( const MUnit< tMemClass >& objMUnit )
   {
      muiUnits = objMUnit.muiUnits;
      
      return( *this );
   }


   MUnit< tMemClass >& operator=( const MUI muiNumberOfUnits )
   {
      muiUnits = muiNumberOfUnits;
      
      return( *this );
   }


   template< class tNumberType > tNumberType GetAs() const throw()
   {
	   return( (tNumberType)muiUnits );
   }


   MUI GetUnits() const throw()
   {
      return( muiUnits );
   }


   DWORD GetUnits32() const throw()
   {
      MUnitException::TestTargetSize32( muiUnits );

      return( (DWORD)muiUnits );
   }


   MUI InBytes() const throw()
   {
      return( muiUnits * sizeof( tMemClass ) );
   }


   DWORD InBytes32() const
   {
      MUnitException::TestTargetSize32( muiUnits * sizeof( tMemClass ) );

      return( (DWORD)(muiUnits * sizeof( tMemClass )) );
   }


   // Returns true if the casting conversion is possible and false otherwise.
   // Typical use: MUnit< BYTE > mu( 4 );
   // bool bCanConverttoDword = mu.CanItBe< DWORD >();
   template< class tmCast > bool CanItBe() const throw()
   {
      return( 0 == (InBytes() % sizeof( tmCast )) );
   }


   // Returns the number of memory units of the specified class, which are contained in memory chunk of another class.
   // The function throws exception if the conversion yields fraction.
   // Typical use: MUnit< DWORD > m1( MUnit< __int64 >( 256 ).As< DWORD >() );
   template< class tmCast > MUnit< tmCast > As() const
   {
      MUnitException::TestDiv( InBytes(), sizeof( tmCast ) );
      
      return( MUnit< tmCast >( InBytes() / sizeof( tmCast ) ) );
   }


   // Arithmetic operators
   MUnit< tMemClass > operator++()
   {
      muiUnits++;
      
      MUnitException::TestPPOverflow( muiUnits );
      
      return( *this );
   }


   MUnit< tMemClass > operator++( int )
   {
      const MUnit muPostFix( *this );
      
      operator++();

      return( muPostFix );
   }


   MUnit< tMemClass > operator--()
   {
      muiUnits--;
      
      MUnitException::TestMMOverflow( muiUnits );
      
      return( *this );
   }


   MUnit< tMemClass > operator--( int )
   {
      const MUnit muPostFix( *this );
      
      operator--();
      
      return( muPostFix );
   }


   MUnit< tMemClass >& operator+=( const MUnit< tMemClass >& muSummand )
   {
      MUnitException::TestAdd( muiUnits, muSummand.muiUnits );
      
      muiUnits += muSummand.muiUnits;
      
      return( *this );
   }


   MUnit< tMemClass >& operator+=( const tMemClass& muSummand )
   {
      MUnitException::TestAdd( muiUnits, muSummand );
      
      muiUnits += muSummand.muiUnits;
      
      return( *this );
   }


   MUnit< tMemClass >& operator*=( const MUnit< tMemClass >& muFactor )
   {
      MUnitException::TestMul( muiUnits, muFactor.muiUnits );
      
      muiUnits *= muFactor.muiUnits;
      
      return( *this );
   }
   
   
   MUnit< tMemClass >& operator-=( const MUnit< tMemClass >& muSubtrahend )
   {
      MUnitException::TestSub( muiUnits, muSubtrahend.muiUnits );
      
      muiUnits -= muSubtrahend.muiUnits;
      
      return( *this );
   }
   
   
   MUnit< tMemClass >& operator/=( const MUnit< tMemClass >& muDivisor )
   {
      MUnitException::TestDiv( muiUnits, muDivisor.muiUnits );
      
      muiUnits /= muDivisor.muiUnits;
      
      return( *this );
   }


   friend MUnit< tMemClass > operator +( const MUI muiSummand, const MUnit< tMemClass >& muSummand )
   {
      MUnitException::TestAdd( muiSummand, muSummand.muiUnits );
      
      return( MUnit< tMemClass >( muiSummand + muSummand.muiUnits ) );
   }


   friend MUnit< tMemClass > operator +( const MUnit< tMemClass >& muSummand, const MUI muiSummand )
   {
      return( operator+( muiSummand, muSummand ) );
   }


   friend MUnit< tMemClass > operator +( const MUnit< tMemClass >& muSummandL, const MUnit< tMemClass >& muSummandR )
   {
      return( operator+( muSummandL.muiUnits, muSummandR ) );
   }


   friend MUnit< tMemClass > operator *( const MUI muiFactor, const MUnit< tMemClass >& muFactor )
   {
      const MUI muiResult( muiFactor * muFactor.muiUnits );
      
      MUnitException::TestMul( muiFactor, muFactor.muiUnits );
      
      return( MUnit< tMemClass >( muiResult ) );
   }


   friend MUnit< tMemClass > operator *( const MUnit< tMemClass >& muFactor, const MUI muiFactor )
   {
      return( operator*( muiFactor, muFactor ) );
   }


   friend MUnit< tMemClass > operator *( const MUnit< tMemClass >& muFactorL, const MUnit< tMemClass >& muFactorR )
   {
      return( operator*( muFactorL.muiUnits, muFactorR ) );
   }


   friend MUnit< tMemClass > operator -( const MUI muiMinuend, const MUnit< tMemClass >& muSubtrahend )
   {
      MUnitException::TestSub( muiMinuend, muSubtrahend.muiUnits );
      
      return( MUnit< tMemClass >( muiMinuend - muSubtrahend.muiUnits ) );
   }


   friend MUnit< tMemClass > operator -( const MUnit< tMemClass >& muMinuend, const MUI muiSubtrahend )
   {
      return( operator-( muMinuend.muiUnits, MUnit< tMemClass >( muiSubtrahend ) ) );
   }
   
   
   friend MUnit< tMemClass > operator -( const MUnit< tMemClass >& muMinuend, const MUnit< tMemClass >& muSubtrahend  )
   {
      return( operator-( muMinuend.muiUnits, muSubtrahend ) );
   }


   friend MUnit< tMemClass > operator /( const MUI muiDividend, const MUnit< tMemClass >& muDivisor )
   {
      MUnitException::TestDiv( muiDividend, muDivisor.muiUnits );
      
      return( MUnit( muiDividend / muDivisor.muiUnits ) );
   }


   friend MUnit< tMemClass > operator /( const MUnit< tMemClass >& muDividend, const MUI muiDivisor )
   {
      return( operator/( muDividend.muiUnits, MUnit< tMemClass >( muiDivisor) ) );
   }


   friend MUnit< tMemClass > operator /( const MUnit< tMemClass >& muDividend, const MUnit< tMemClass >& muDivisor )
   {
      return( operator/( muDividend.muiUnits, muDivisor ) );
   }


   friend bool operator  <( const MUI                 muiUnits, const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          <  muUnits.muiUnits  ); }
   friend bool operator  <( const MUnit< tMemClass >& muUnits,  const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  <  muiUnits          ); }
   friend bool operator  <( const MUnit< tMemClass >& muUnitsL, const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits <  muUnitsR.muiUnits ); }


   friend bool operator  >( const MUI                 muiUnits, const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          >  muUnits.muiUnits  ); }
   friend bool operator  >( const MUnit< tMemClass >& muUnits,  const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  >  muiUnits          ); }
   friend bool operator  >( const MUnit< tMemClass >& muUnitsL, const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits >  muUnitsR.muiUnits ); }


   friend bool operator <=( const MUI                 muiUnits, const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          <= muUnits.muiUnits  ); }
   friend bool operator <=( const MUnit< tMemClass >& muUnits,  const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  <= muiUnits          ); }
   friend bool operator <=( const MUnit< tMemClass >& muUnitsL, const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits <= muUnitsR.muiUnits ); }


   friend bool operator >=( const MUI                 muiUnits, const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          >= muUnits.muiUnits  ); }
   friend bool operator >=( const MUnit< tMemClass >& muUnits,  const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  >= muiUnits          ); }
   friend bool operator >=( const MUnit< tMemClass >& muUnitsL, const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits >= muUnitsR.muiUnits ); }


   friend bool operator ==( const MUI                 muiUnits, const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          == muUnits.muiUnits  ); }
   friend bool operator ==( const MUnit< tMemClass >& muUnits,  const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  == muiUnits          ); }
   friend bool operator ==( const MUnit< tMemClass >& muUnitsL, const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits == muUnitsR.muiUnits ); }


   friend bool operator !=( const MUI                 muiUnits, const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          != muUnits.muiUnits  ); }
   friend bool operator !=( const MUnit< tMemClass >& muUnits,  const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  != muiUnits          ); }
   friend bool operator !=( const MUnit< tMemClass >& muUnitsL, const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits != muUnitsR.muiUnits ); }
};


#pragma warning( pop )
