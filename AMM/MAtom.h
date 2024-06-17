//
//
// MAtom.h: memory atoms definitions as example of implementation of:
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase Two'
//
// © Copyright 2005 - 2012 by Miroslav Bonchev Bonchev. All rights reserved.
//
//
//******************************************************************************************************


// Open Source License – The MIT License
//
//
// {your product} uses the Atomic Memory Model by Miroslav Bonchev Bonchev.
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


#include "Common.h"
#include "MException.h"
#include "MMemory.h"
#include "MHandle.h"
#include "MSmartPtr.h"
#include "Objidl.h"


#pragma warning( push )
#pragma warning( disable : 4290 )   // warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)


extern HMODULE hDefaultTextResourceModule;


template< class tMemClass >
class MUnit
{
protected:
   MUI muiUnits;


public:
   MUnit() : muiUnits( 0 )
   {
   }


   MUnit( typename const MUnit< tMemClass >& objMUnit ) : muiUnits( objMUnit.muiUnits )
   {
   }


   explicit MUnit( const MUI muiNumberOfUnits ) : muiUnits( muiNumberOfUnits )
   {
   }


   ~MUnit()
   {
   }


   MUnit< tMemClass >& operator=( typename const MUnit< tMemClass >& objMUnit )
   {
      muiUnits = objMUnit.muiUnits;
      
      return( *this );
   }


   MUnit< tMemClass >& operator=( const MUI muiNumberOfUnits )
   {
      muiUnits = muiNumberOfUnits;
      
      return( *this );
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
   template< class tmCast > MUnit< tmCast > As() const throw( MUnitException )
   {
      MUnitException::TestDiv( InBytes(), sizeof( tmCast ) );
      
      return( MUnit< tmCast >( InBytes() / sizeof( tmCast ) ) );
   }


   // Arithmetic operators
   MUnit< tMemClass > operator++() throw( MUnitException )
   {
      muiUnits++; MUnitException::TestPPOverflow( muiUnits );
      
      return( *this );
   }


   MUnit< tMemClass > operator++( int ) throw( MUnitException )
   {
      const MUnit muPostFix( *this );
      
      operator++();

      return( muPostFix );
   }


   MUnit< tMemClass > operator--() throw( MUnitException )
   {
      muiUnits--;
      
      MUnitException::TestMMOverflow( muiUnits );
      
      return( *this );
   }


   MUnit< tMemClass > operator--( int ) throw( MUnitException )
   {
      const MUnit muPostFix( *this );
      
      operator--();
      
      return( muPostFix );
   }


   MUnit< tMemClass >& operator+=( typename const MUnit< tMemClass >& muSummand ) throw( MUnitException )
   {
      MUnitException::TestAdd( muiUnits, muSummand.muiUnits );
      
      muiUnits += muSummand.muiUnits;
      
      return( *this );
   }


   MUnit< tMemClass >& operator*=( typename const MUnit< tMemClass >& muFactor ) throw( MUnitException )
   {
      MUnitException::TestMul( muiUnits, muFactor.muiUnits );
      
      muiUnits *= muFactor.muiUnits;
      
      return( *this );
   }
   
   
   MUnit< tMemClass >& operator-=( typename const MUnit< tMemClass >& muSubtrahend ) throw( MUnitException )
   {
      MUnitException::TestSub( muiUnits, muSubtrahend.muiUnits );
      
      muiUnits -= muSubtrahend.muiUnits;
      
      return( *this );
   }
   
   
   MUnit< tMemClass >& operator/=( typename const MUnit< tMemClass >& muDivisor ) throw( MUnitException )
   {
      MUnitException::TestDiv( muiUnits, muDivisor.muiUnits );
      
      muiUnits /= muDivisor.muiUnits;
      
      return( *this );
   }


   friend MUnit< tMemClass > operator +( const MUI muiSummand, typename const MUnit< tMemClass >& muSummand ) throw( MUnitException )
   {
      MUnitException::TestAdd( muiSummand, muSummand.muiUnits );
      
      return( MUnit< tMemClass >( muiSummand + muSummand.muiUnits ) );
   }


   friend MUnit< tMemClass > operator +( typename const MUnit< tMemClass >& muSummand, const MUI muiSummand ) throw( MUnitException )
   {
      return( operator+( muiSummand, muSummand ) );
   }


   friend MUnit< tMemClass > operator +( typename const MUnit< tMemClass >& muSummandL, typename const MUnit< tMemClass >& muSummandR ) throw( MUnitException )
   {
      return( operator+( muSummandL.muiUnits, muSummandR ) );
   }


   friend MUnit< tMemClass > operator *( const MUI muiFactor, typename const MUnit< tMemClass >& muFactor ) throw( MUnitException )
   {
      const MUI muiResult( muiFactor * muFactor.muiUnits );
      
      MUnitException::TestMul( muiFactor, muFactor.muiUnits );
      
      return( MUnit< tMemClass >( muiResult ) );
   }


   friend MUnit< tMemClass > operator *( typename const MUnit< tMemClass >& muFactor, const MUI muiFactor ) throw( MUnitException )
   {
      return( operator*( muiFactor, muFactor ) );
   }


   friend MUnit< tMemClass > operator *( typename const MUnit< tMemClass >& muFactorL, typename const MUnit< tMemClass >& muFactorR ) throw( MUnitException )
   {
      return( operator*( muFactorL.muiUnits, muFactorR ) );
   }


   friend MUnit< tMemClass > operator -( const MUI muiMinuend, typename const MUnit< tMemClass >& muSubtrahend ) throw( MUnitException )
   {
      MUnitException::TestSub( muiMinuend, muSubtrahend.muiUnits );
      
      return( MUnit< tMemClass >( muiMinuend - muSubtrahend.muiUnits ) );
   }


   friend MUnit< tMemClass > operator -( typename const MUnit< tMemClass >& muMinuend, const MUI muiSubtrahend ) throw( MUnitException )
   {
      return( operator-( muMinuend.muiUnits, MUnit< tMemClass >( muiSubtrahend ) ) );
   }
   
   
   friend MUnit< tMemClass > operator -( typename const MUnit< tMemClass >& muMinuend, typename const MUnit< tMemClass >& muSubtrahend  ) throw( MUnitException )
   {
      return( operator-( muMinuend.muiUnits, muSubtrahend ) );
   }


   friend MUnit< tMemClass > operator /( const MUI muiDividend, typename const MUnit< tMemClass >& muDivisor ) throw( MUnitException )
   {
      MUnitException::TestDiv( muiDividend, muDivisor.muiUnits );
      
      return( MUnit( muiDividend / muDivisor.muiUnits ) );
   }


   friend MUnit< tMemClass > operator /( typename const MUnit< tMemClass >& muDividend, const MUI muiDivisor ) throw( MUnitException )
   {
      return( operator/( muDividend.muiUnits, MUnit< tMemClass >( muiDivisor) ) );
   }


   friend MUnit< tMemClass > operator /( typename const MUnit< tMemClass >& muDividend, typename const MUnit< tMemClass >& muDivisor ) throw( MUnitException )
   {
      return( operator/( muDividend.muiUnits, muDivisor ) );
   }


   friend bool operator  <(          const MUI                 muiUnits, typename const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          <  muUnits.muiUnits  ); }
   friend bool operator  <( typename const MUnit< tMemClass >& muUnits,           const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  <  muiUnits          ); }
   friend bool operator  <( typename const MUnit< tMemClass >& muUnitsL, typename const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits <  muUnitsR.muiUnits ); }


   friend bool operator  >(          const MUI                 muiUnits, typename const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          >  muUnits.muiUnits  ); }
   friend bool operator  >( typename const MUnit< tMemClass >& muUnits,           const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  >  muiUnits          ); }
   friend bool operator  >( typename const MUnit< tMemClass >& muUnitsL, typename const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits >  muUnitsR.muiUnits ); }


   friend bool operator <=(          const MUI                 muiUnits, typename const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          <= muUnits.muiUnits  ); }
   friend bool operator <=( typename const MUnit< tMemClass >& muUnits,           const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  <= muiUnits          ); }
   friend bool operator <=( typename const MUnit< tMemClass >& muUnitsL, typename const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits <= muUnitsR.muiUnits ); }


   friend bool operator >=(          const MUI                 muiUnits, typename const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          >= muUnits.muiUnits  ); }
   friend bool operator >=( typename const MUnit< tMemClass >& muUnits,           const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  >= muiUnits          ); }
   friend bool operator >=( typename const MUnit< tMemClass >& muUnitsL, typename const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits >= muUnitsR.muiUnits ); }


   friend bool operator ==(          const MUI                 muiUnits, typename const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          == muUnits.muiUnits  ); }
   friend bool operator ==( typename const MUnit< tMemClass >& muUnits,           const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  == muiUnits          ); }
   friend bool operator ==( typename const MUnit< tMemClass >& muUnitsL, typename const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits == muUnitsR.muiUnits ); }


   friend bool operator !=(          const MUI                 muiUnits, typename const MUnit< tMemClass >& muUnits  ) throw() { return( muiUnits          != muUnits.muiUnits  ); }
   friend bool operator !=( typename const MUnit< tMemClass >& muUnits,           const MUI                 muiUnits ) throw() { return( muUnits.muiUnits  != muiUnits          ); }
   friend bool operator !=( typename const MUnit< tMemClass >& muUnitsL, typename const MUnit< tMemClass >& muUnitsR ) throw() { return( muUnitsL.muiUnits != muUnitsR.muiUnits ); }
};


template< class tMemClass > class ShellMemory;


// The fundamental abstract class defining common interface for all memory atoms MAtom< tMemClass >.
template< class tMemClass >
class MAtom
{
   template< class tMemClass > friend class MemoryPH;
   template< class tChar >     friend class MStringEx;
   template< class tMemClass > friend class MAtom;


protected:
   tMemClass*         pData;
   MUnit< tMemClass > muSize;

   
private:
   // For internal use.
   MUnit< tMemClass >* GetSizePtr() throw()
   {
      return( &muSize );
   }


public:
   MAtom() : pData( NULL ), muSize( 0 )
   {
   }


   MAtom( tMemClass *pSrcData, typename const MUnit< tMemClass >& muUnits ) : pData( pSrcData ), muSize( muUnits )
   {
      MAtomException::TestNonNullPointer( pSrcData );
   }


   MAtom( const MAtom< tMemClass >& maSource ) : pData( maSource.pData ), muSize( maSource.muSize )
   {
   }


   MAtom( MAtom< tMemClass >& maSource, const MUnit< tMemClass >& muOffset, const MUnit< tMemClass >& muUnits )
   {
      MAtomException::TestSourceMemorySpace< tMemClass >( muOffset, muSize, maSource.GetSize() );

      pData  = &maSource[muOffset];
      muSize = muUnits;
   }


   virtual ~MAtom()
   {
   };


   // Replace the content of the memory atom with that of the parameter.
   virtual MAtom< tMemClass >& operator=( typename const MAtom< tMemClass >& maSource ) = 0;


   enum MemoryOrigin
   {
      Heap,
      GlobalAllocFree,
      VariableUnidentifiable
   };


   // Pure polymorphic method. Returns the Memory Origin.
   virtual MemoryOrigin GetMemoryOrigin() const = 0;

   
   // Pure polymorphic method. Returns true if the memory atom is able to (re)allocate memory, and false if not.
   // For example, memory atoms with memory from a heap are typically able to (re)allocate, whilst shell memory atoms cannot.
   virtual bool CanReAllocate()     const throw() = 0;
   

public:
   template< class tMemClassSwap >
   void ExchangeMemory( MAtom< tMemClassSwap >& memorySwapWith )
   {
      MAtomException::TestCompatibleCall( memorySwapWith.GetMemoryOrigin() == GetMemoryOrigin() );
      MAtomException::TestCompatibleCall( memorySwapWith.CanItBe< tMemClass >() && CanItBe< tMemClassSwap >() );

      tMemClassSwap* pThis           = (tMemClassSwap*)pData;
      pData                          = (tMemClass*)memorySwapWith.GetMemory();
      *memorySwapWith.GetMemoryPtr() = pThis;

      MUnit< tMemClass > muThis    = muSize;
      muSize                       = memorySwapWith.GetSize().As< tMemClass >();
      *memorySwapWith.GetSizePtr() = muThis.As< tMemClassSwap >();
   }


   // Pure polymorphic method. Returns true if the memory atom is empty, and false otherwise.
   virtual bool IsEmpty() const throw( MAtomException ) { return( 0 == muSize.GetUnits() ); }


   // Pure polymorphic method. Empties the memory atom from any contained memory.
   virtual MAtom< tMemClass >& Empty() throw( MAtomException ) = 0;


   // Returns the size of the memory contained by the atom memory it memory units.
   MUnit< tMemClass > GetSize() const throw()
   {
      return( muSize );
   }
   

   // Returns the size of the memory unit specializing the atom, i.e. size of the Type::Class property.
   static size_t GetUnitSize() throw()
   {
      return( sizeof( tMemClass ) );
   }


   // Returns the size of the class specializing the memory atom.
   static size_t GetClassSize() throw()
   {
      return( sizeof( tMemClass ) );
   }


   // Compare two memory atoms to see if they have the same content byte by byte.
   // Returns true if the atoms are identical and false otherwise.
   virtual bool operator==( const MAtom< tMemClass >& mtAtom ) const throw()
   {
      return( (mtAtom.muSize == muSize) && (0 == memcmp( pData, mtAtom.pData, muSize.InBytes() )) );
   }


   // Compare two memory atoms to see if they have different content byte by byte.
   // Returns true if the atoms have different content and false otherwise.
   virtual bool operator!=( const MAtom< tMemClass >& mtAtom ) const throw()
   {
      return( !operator==( mtAtom ) );
   }


   // Returns direct access to the memory. This is relaxation of the Atomic Memory Model rule 2 
   // allowing the use of memory atoms in non Atomic Memory Model compliant environment.
   const tMemClass*   GetMemory()    const throw() { return( pData  ); }
         tMemClass*   GetMemory()          throw() { return( pData  ); }
   const tMemClass**  GetMemoryPtr() const throw() { return( &pData ); }
         tMemClass**  GetMemoryPtr()       throw() { return( &pData ); }

         
   // Returns direct access to the memory. This is relaxation of the Atomic Memory Model rule 2 
   // allowing the use of memory atoms in non Atomic Memory Model compliant environment.
   const tMemClass* operator->() const throw() { return( pData ); }
         tMemClass* operator->()       throw() { return( pData ); }


   // Returns direct access to the memory. This is relaxation of the Atomic Memory Model rule 2 
   // allowing the use of memory atoms in non Atomic Memory Model compliant environment.
   operator       tMemClass& ()         throw() { return( *pData ); }
   operator const tMemClass& ()   const throw() { return( *pData ); }
   operator       tMemClass* ()         throw() { return(  pData ); }
   operator const tMemClass* ()   const throw() { return(  pData ); }


   // Indexer returning reference to a unit contained in the memory atom. 
   tMemClass& operator[]( const int iIndex ) throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( iIndex, GetSize().GetUnits() );

      return( pData[(MUI)iIndex] );
   }


   // Indexer returning reference to a unit contained in the memory atom. 
   const tMemClass& operator[]( const int iIndex ) const throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( iIndex, GetSize().GetUnits() );
      
      return( pData[(MUI)iIndex] );
   }


   // Indexer returning reference to a unit contained in the memory atom. 
   tMemClass& operator[]( const unsigned long iIndex ) throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( iIndex, GetSize().GetUnits() );
            
      return( pData[(MUI)iIndex] );
   }


   // Indexer returning reference to a unit contained in the memory atom. 
   const tMemClass& operator[]( const unsigned long iIndex ) const throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( iIndex, GetSize().GetUnits() );
      
      return( pData[(MUI)iIndex] );
   }


   // Indexer returning reference to a unit contained in the memory atom. 
   tMemClass& operator[]( const MUI muiIndex ) throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( muiIndex, GetSize().GetUnits() );
      
      return( pData[muiIndex] );
   }


   // Indexer returning reference to a unit contained in the memory atom. 
   const tMemClass& operator[]( const MUI muiIndex ) const throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( muiIndex, GetSize().GetUnits() );
      
      return( pData[muiIndex] );
   }

   
   // Indexer returning reference to a unit contained in the memory atom. 
   tMemClass& operator[]( typename const MUnit< tMemClass >& muIndex ) throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( muIndex.GetUnits(), GetSize().GetUnits() );
      
      return( pData[muIndex.GetUnits()] );
   }
   
   
   // Indexer returning reference to a unit contained in the memory atom. 
   const tMemClass& operator[]( typename const MUnit< tMemClass >& muIndex ) const throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, false >( muIndex.GetUnits(), GetSize().GetUnits() );
      
      return( pData[muIndex.GetUnits()] );
   }


   // Wraps number of units inside a memory atom and returns a shell memory atom containing them.
   // const MUnit< tMemType >& muFrom – starting memory unit, zero based index.
   // const MUnit< tmCast >& muSize   - number of units to be included in the new memory atom
   // Return ShellMemory atom referencing the requested number of memory units in the source (this) memory atom starting from the appointed position.
   template< class tmCast > ShellMemory< tmCast > SubMemory( typename const MUnit< tMemClass >& muFrom, typename const MUnit< tmCast >& muSize ) const throw( MAtomException )
   {
      MAtomException::TestSourceMemorySpace< tMemClass >( muFrom, muSize.As< tMemClass >(), GetSize() );

      return( ShellMemory< tmCast >( (tmCast*)((MUI)pData + muFrom.InBytes()), muSize ) );
   }


   // Wraps number of units inside a memory atom and returns a shell memory atom containing them.
   // const MUnit< tMemType >& muFrom – starting memory unit, zero based index.
   // const MUnit< tmCast >& muSize   - number of units to be included in the new memory atom
   // Return ShellMemory atom referencing the requested number of memory units in the source (this) memory atom starting from the appointed position.
   template< class tmCast > ShellMemory< tmCast > SubMemory( typename const MUnit< tMemClass >& muFrom, typename const MUnit< tmCast >& muSize ) throw( MAtomException )
   {
      MAtomException::TestSourceMemorySpace< tMemClass >( muFrom, muSize.As< tMemClass >(), GetSize() );

      return( ShellMemory< tmCast >( (tmCast*)((MUI)pData + muFrom.InBytes()), muSize ) );
   }


   // Automatic Casting Operations of non compatible classes, e.g. cast [MAtom Derivate]< BYTE > to [MAtom Derivate]< DWORD >.
   //
   // These methods returns an instance of ShellMemory because it is impossible to return re-specialized MAtom referencing "this" without encountering
   // serious problems. Returning a re-specialized MAtom would require an expression similar to return( *(MAtom< tmCast >*)this );, which is 
   // clearly wrong since the muSize will be the same value but in a different class of units. For example, Memory< BYTE >( MUnit< BYTE >( 10 ) ) 
   // classed to DWORD will appear as MAtom< DWORD >::pData pointing to the same data, and having the same amount of units = 10 but of type DWORD, 
   // so the object would incorrectly claim total size of 10 * sizeof( DWORD ) instead of 10 * sizeof( BYTE ).
   //
   // In the first Atomic Memory Model implementation, the memory size was held in bytes, and this conversion was possible. However in this 
   // implementation (2) such immediate casting is impossible for the above reason as well as for reason that while virtual functions will 
   // be called correctly on the return object, non virtual functions will be called inconsistently. Consider the following example:
   //
   //   class X                               | class A : public X                              | class B : public X
   //   {                                     | {                                               | {
   //   public:                               | public:                                         | public:
   //      virtual void f1() = 0;             |    virtual void f1() { printf( "A::f1()\n" ); } |    virtual void f1() { printf( "B::f1()\n" ); }
   //      void f2() { printf( "X::f2()" ); } |    void f2() { printf( "A::f2()\n" ); }         |    void f2() { printf( "B::f2()\n" ); }
   //   };                                    | };                                              | };
   //
   //   void main()
   //   {
   //      A  a;
   //      B& b( *(B*)&a ); - effectively what AppearAs() in Atomic Memory Model Phase One does
   //      a.f1();
   //      a.f2();
   //      b.f1();
   //      b.f2();
   //   }
   // results:    A::f1()
   //             A::f2()
   //             A::f1() --same object with different "identity" - inconsistent behavior.
   //             B::f2() -/
   //
   // In order to maintain consistency, the Atomic Memory Model implementation 2 uses appropriately constructed and specialized shell memory 
   // objects which are returned by value for any automatic casting operations.


   // Cast (specialize) the memory atom with another class. Returns a shell memory atom classed (specialized) with the new cast type.
   template< class tmCast > ShellMemory< tmCast > ClassAs() throw( MAtomException )
   {
      return( ShellMemory< tmCast >( (tmCast*)pData, muSize.As< tmCast >() ) );
   }


   // Cast (specialize) the memory atom with another class. Returns a constant shell memory atom classed (specialized) with the new cast type.
   template< class tmCast > const ShellMemory< tmCast > ClassAs() const throw( MAtomException )
   {
      return( ShellMemory< tmCast >( (tmCast*)pData, muSize.As< tmCast >() ) );
   }


   // Cast (specialize) the memory atom with another class. Returns a shell memory atom classed (specialized) with the new cast type.
   // const MUnit< tMemClass > muOffsetThis - the beginning of the memory referenced by the returned shell memory atom in the units of the referenced (this) memory atom.
   // const MUnit< tmCast >    muUnitsCast  - the size of the memory referenced in the returned shell memory atom in memory units of the returned shell memory atom.
   template< class tmCast > ShellMemory< tmCast > ClassAs( typename const MUnit< tMemClass > muOffsetThis, typename const MUnit< tmCast > muUnitsCast ) throw( MAtomException )
   {
      MAtomException::TestSourceMemorySpaceCast< tMemClass, tmCast >( muOffsetThis, muUnitsCast, GetSize() );

      return( ShellMemory< tmCast >( (tmCast*)((size_t)pData + muOffsetThis.InBytes()), muUnitsCast ) );
   }

   
   // Cast (specialize) the memory atom with another class. Returns a constant shell memory atom classed (specialized) with the new cast type.
   // const MUnit< tMemClass > muOffsetThis - the beginning of the memory referenced by the returned shell memory atom in the units of the referenced (this) memory atom.
   // const MUnit< tmCast >   muUnitsCast  - the size of the memory referenced in the returned shell memory atom in memory units of the returned shell memory atom.
   template< class tmCast > const ShellMemory< tmCast > ClassAs( typename const MUnit< tMemClass > muOffsetThis, typename const MUnit< tmCast > muUnitsCast ) const throw( MAtomException )
   {
      MAtomException::TestSourceMemorySpaceCast< tMemClass, tmCast >( muOffsetThis, muUnitsCast, GetSize() );

      return( ShellMemory< tmCast >( (tmCast*)((size_t)pData + muOffsetThis.InBytes()), muUnitsCast ) );
   }


   // Test if a memory atom as a whole can be casted directly with another memory class.
   template< class tmCast > bool CanItBe() const throw()
   {
      return( muSize.CanItBe< tmCast >() );
   }


   // Pure polymorphic method. Release the contained memory and allocate the new requested amount. Not all types of memory atoms are able to re-allocate memory.
   virtual MAtom< tMemClass >& ReAllocate( typename const MUnit< tMemClass >& muUnits ) throw( MAtomException ) = 0;


   // Copy part or the entire memory content of the passed memory atom to the memory contained in this memory atom. Memory is copied in a byte by byte fashion.
   // MUnit< tMemClass >& muOffsetThis     - indicates the offset from where the new data will be written in units of (this) to the target memory atom.
   // const bool bUpdateOffsetThis         - indicates whether muOffsetThis will be updated after successful data transfer with the number of copied memory units.
   // const MAtom< tMemClass >& maSource   - source memory atom. 
   // MUnit< tMemClass >& muOffsetSource   - indicates the offset from where to begin the data transfer in the source memory atom in its own units.
   // const bool bUpdateOffsetSource       - indicates whether muOffsetSource will be updated after successful data transfer with the number of copied memory units.
   // const MUnit< tMemClass >& muTransfer - indicates the number of memory units to be copied from the source memory atom to this (target) one.
   // There must be enough memory in both the source and target memory atoms for the request to be completed. The return value is reference to this (target) memory atom.
   virtual MAtom< tMemClass >& Transfer( typename MUnit< tMemClass >& muOffsetThis,
                                         const bool bUpdateOffsetThis,
                                         typename const MAtom< tMemClass >& maSource,
                                         typename MUnit< tMemClass >& muOffsetSource,
                                         const bool bUpdateOffsetSource,
                                         typename const MUnit< tMemClass >& muTransfer ) throw( MAtomException )
   {
      if( 0 != muTransfer )
      {
         MAtomException::TestTargetMemorySpace< tMemClass >( muOffsetThis,   muTransfer,          GetSize() );
         MAtomException::TestSourceMemorySpace< tMemClass >( muOffsetSource, muTransfer, maSource.GetSize() );

         memcpy( (void*)((size_t)pData + muOffsetThis.InBytes()), &(maSource[muOffsetSource]), muTransfer.InBytes() );

         // No need to test these operations since the first Tests will catch any overflow.
         if( bUpdateOffsetThis   ) { muOffsetThis   += muTransfer; }
         if( bUpdateOffsetSource ) { muOffsetSource += muTransfer; }
      }

      return( *this );
   }


   // Sequentially copy part or the entire memory content of the passed memory atom to the memory contained in this memory atom starting at the target's beginning.
   // MAtom< tMemClass >& maSource         - source memory atom. 
   // MUnit< tMemClass >& muOffsetSource   - indicates the offset from where to begin the data transfer in the source memory atom in its own units.
   // const bool bUpdateOffsetSource       - indicates whether muOffsetSource will be updated after successful data transfer with the number of copied memory units.
   // const MUnit< tMemClass >& muTransfer - indicates the number of memory units to be copied from the source memory atom to this (target) one.
   // There must be enough memory in both the source and target memory atoms for the request to be completed. The return value is reference to this (target) memory atom.
   virtual MAtom< tMemClass >& Transfer( typename const MAtom< tMemClass >& maSource,
                                         typename MUnit< tMemClass >& muOffsetSource,
                                         const bool bUpdateOffsetSource,
                                         typename const MUnit< tMemClass >& muTransfer ) throw( MAtomException )
   {
      MUnit< tMemClass > muOffsetThis( 0 );
      const bool bUpdateOffsetThis( false );

      return( Transfer( muOffsetThis, bUpdateOffsetThis, maSource, muOffsetSource, bUpdateOffsetSource, muTransfer ) );
   }


   // Sequentially copy part or the entire memory content of the passed memory atom to the memory contained in this memory atom starting from the sources's beginning.
   // MUnit< tMemClass >& muOffsetThis     - indicates the offset from where the new data will be written in units of (this) to the target memory atom.
   // const bool bUpdateOffsetThis         - indicates whether muOffsetThis will be updated after successful data transfer with the number of copied memory units.
   // const MAtom< tMemClass >& maSource   - source memory atom. 
   // const MUnit< tMemClass >& muTransfer - indicates the number of memory units to be copied from the source memory atom to this (target) one.
   // There must be enough memory in both the source and target memory atoms for the request to be completed. The return value is reference to this (target) memory atom.
   virtual MAtom< tMemClass >& Transfer( typename MUnit< tMemClass >& muOffsetThis,
                                         const bool bUpdateOffsetThis,
                                         typename const MAtom< tMemClass >& maSource,
                                         typename const MUnit< tMemClass >& muTransfer ) throw( MAtomException )
   {
      MUnit< tMemClass > muOffsetSource( 0 );
      const bool bUpdateOffsetSource( false );

      return( Transfer( muOffsetThis, bUpdateOffsetThis, maSource, muOffsetSource, bUpdateOffsetSource, muTransfer ) );
   }


   // Sequentially copy the entire memory content of the passed memory atom starting at its beginning, to the memory contained by the target (this).
   // MUnit< tMemClass >& muOffsetThis   - indicates the offset from where the new data will be written in units of (this) to the target memory atom.
   // const MAtom< tMemClass >& maSource - source memory atom. 
   // There must be enough memory in the target memory atom for the request to be completed. The return value is reference to this (target) memory atom.
   virtual MAtom< tMemClass >& Transfer( typename MUnit< tMemClass > muOffsetThis, typename const MAtom< tMemClass >& maSource ) throw( MAtomException )
   {
      if( 0 != maSource.GetSize() )
      {
         MAtomException::TestTargetMemorySpace< tMemClass >( muOffsetThis, maSource.GetSize(), GetSize() );

         memcpy( (void*)((size_t)pData + muOffsetThis.InBytes()), maSource.pData, maSource.GetSize().InBytes() );
      }

      return( *this );
   }


   // Sequentially copy the entire memory content of the passed memory atom starting at its beginning, to the memory contained by the target (this).
   // MUnit< tMemClass >& muOffsetThis   - indicates the offset from where the new data will be written in units of (this) to the target memory atom.
   // const bool bUpdateOffsetThis       - indicates whether muOffsetThis will be updated after successful data transfer with the number of copied memory units.
   // const MAtom< tMemClass >& maSource - source memory atom. 
   // There must be enough memory in the target memory atom for the request to be completed. The return value is reference to this (target) memory atom.
   virtual MAtom< tMemClass >& Transfer( typename MUnit< tMemClass >& muOffsetThis, const bool bUpdateOffsetThis, typename const MAtom< tMemClass >& maSource ) throw( MAtomException )
   {
      if( 0 != maSource.GetSize() )
      {
         MAtomException::TestTargetMemorySpace< tMemClass >( muOffsetThis, maSource.GetSize(), GetSize() );

         memcpy( (void*)((size_t)pData + muOffsetThis.InBytes()), maSource.pData, maSource.GetSize().InBytes() );

         // No need to test this operation since the first Tests will catch any overflow.
         if( bUpdateOffsetThis ) { muOffsetThis += maSource.GetSize(); }
      }

      return( *this );
   }


   // Sequentially copy the entire memory content of the passed memory atom to the memory contained by the target (this), starting at their beginnings.
   // ShellMemory< tMemClass >& maSource - source memory atom. 
   // Both memory atoms must have the same size in order for the request to be completed. The return value is reference to this (target) memory atom.
   virtual MAtom< tMemClass >& Transfer( typename const MAtom< tMemClass >& maSource ) throw( MAtomException )
   {
      if( 0 != maSource.GetSize() )
      {
         MUnit< tMemClass > muOffsetThis( 0 );

         MAtomException::TestTargetMemorySpace< tMemClass >( muOffsetThis, maSource.GetSize(), GetSize() );

         memcpy( (void*)pData, maSource.pData, maSource.GetSize().InBytes() );
      }

      return( *this );
   }


   // Sequentially copy part or the entire memory content of the passed memory atom to the memory contained by the target (this) memory atom.
   // const MUnit< tMemClass >  muOffsetThis   - indicates the offset from where the new data will be written in units of (this) to the target memory atom.
   // const MAtom< tMemClass >& maSource       - source memory atom. 
   // const MUnit< tMemClass >  muOffsetSource - indicates the offset from where to begin the data transfer in the source memory atom in its own units.
   // const MUnit< tMemClass >  muTransfer     - indicates the number of memory units to be copied from the source memory atom to this (target) one.
   // There must be enough memory in both the source and target memory atoms for the request to be completed. The return value is reference to this (target) memory atom.
   virtual MAtom< tMemClass >& Transfer( typename const MUnit< tMemClass >  muOffsetThis,
                                         typename const MAtom< tMemClass >& maSource,
                                         typename const MUnit< tMemClass >  muOffsetSource,
                                         typename const MUnit< tMemClass >  muTransfer ) throw( MAtomException )
   {
      if( 0 != muTransfer )
      {
         MAtomException::TestTargetMemorySpace< tMemClass >( muOffsetThis,   muTransfer,          GetSize() );
         MAtomException::TestSourceMemorySpace< tMemClass >( muOffsetSource, muTransfer, maSource.GetSize() );

         memcpy( (void*)((size_t)pData + muOffsetThis.InBytes()), &(maSource[muOffsetSource]), muTransfer.InBytes() );
      }

      return( *this );
   }


   // Pure polymorphic method. Release contained memory, allocate new memory with the size of the passed memory atom and copy its content to the newly allocated memory 
   // of the target (this). Not all types of memory atoms are able to re-allocate memory and thus some types of memory atoms may always throw exception from this method.
   // const MAtom< tMemType >& maSource – size of the new memory to allocate in memory units.
   // Returns reference to the modified this memory atom.
   virtual MAtom< tMemClass >& ReAllocateTransfer( typename const MAtom< tMemClass >& maSource )    throw( MAtomException ) = 0;


   // Load resource in the memory atom from the hDefaultTextResourceModule module. The HMODULE hDefaultTextResourceModule must be defined in a cpp file and 
   // loaded with a valid module handle (or set to null to load the resource from the current executable module) from which the resource will be loaded.
   // const TCHAR* strType - resource type.
   // const DWORD dwName   - resource name.
   // The function returns reference to this with the loaded resource. The function throws an exception if an invalid call is made, and returns an empty object if it fails to load the resource.
   virtual MAtom< tMemClass >& LoadResource( const TCHAR* strType, const DWORD dwName ) throw( MAtomException );


   // Load resource in the memory atom from the hDefaultTextResourceModule module. The HMODULE hDefaultTextResourceModule must be defined in a cpp file and 
   // loaded with a valid module handle (or set to null to load the resource from the current executable module) from which the resource will be loaded.
   // const TCHAR* strModule - path to the exe/dll containing the resource to be loaded.
   // const TCHAR* strType   - resource type.
   // const DWORD dwName     - resource name.
   // The function returns reference to this with the loaded resource. The function throws an exception if an invalid call is made, and returns an empty object if it fails to load the resource.
   virtual MAtom< tMemClass >& LoadResource( const TCHAR* strModule, const TCHAR* strType, const DWORD dwName ) throw( MAtomException );


   // Load resource in the memory atom from the hDefaultTextResourceModule module. The HMODULE hDefaultTextResourceModule must be defined in a cpp file and 
   // loaded with a valid module handle (or set to null to load the resource from the current executable module) from which the resource will be loaded.
   // const HMODULE hModule - handle to the module containing the resource to be loaded.
   // const TCHAR* strType  - resource type.
   // const DWORD dwName    - resource name.
   // The function returns reference to this with the loaded resource. The function throws an exception if an invalid call is made, and returns an empty object if it fails to load the resource.
   virtual MAtom< tMemClass >& LoadResource( const HMODULE hModule, const TCHAR* strType, const DWORD dwName ) throw( MAtomException );


   // Store the memory atom content as a resource in a module. The calling process must have sufficient rights to write in the module, and the module must be available for writing.
   // LPCTSTR strModule                   - filename and path to the module where the content of the memory atom will be stored as a resource.
   // LPCTSTR strType                     - type of the stored resource.
   // const DWORD dwName                  - name of the stored resource.
   // const USHORT usLanguage             - language code for the stored resource, e.g. LANG_ENGLISH.
   // const USHORT usSubLanguage          - sub-language code for the stored resource, e.g. SUBLANG_ENGLISH_US.
   // const BOOL bDeleteExistingResources - "true" deletes any existing resource with the specified name; "false" leaves existing resources intact unless they are overwritten.
   // The function returns true if the request is processed successfully and false in case of failure. Use GetLastError() or string( string::eGetSystemError ) to review the error occurred.
   virtual bool StoreResource( LPCTSTR strModule, LPCTSTR strType, const DWORD dwName, const USHORT usLanguage, const USHORT usSubLanguage, const BOOL bDeleteExistingResources ) const;


   // Move the contained memory up/down a memory stream. This method is meaningful only for memory types which represent a memory segment within a stream, such as the ShellMemory memory type.
   // const MUnit< tMemClass >& muOffset - memory units to move the beginning of the "window" in the stream.
   // const bool bPositiveDirection      - true makes the beginning of the "window" move towards the end of the stream, and false towards the beginning.
   // const bool bConstantSize           - true maintains the size of the memory contained by the memory atom constant, while false shrinks/expands the contained memory with muOffset.
   // The function returns a reference to the modified memory atom ("window") in the other stream. The function throws exception in case of invalid call as usual.
   virtual MAtom< tMemClass >& Offset( typename const MUnit< tMemClass >& muOffset, const bool bPositiveDirection, const bool bConstantSize ) throw( MAtomException );


   // Decreases the number of the contained memory units to the muSize passed as parameter.
   // The function returns a reference to the modified memory atom.
   virtual MAtom< tMemClass >& LimitSizeTo( typename const MUnit< tMemClass >& muSize ) throw( MAtomException );


   // Writes the content of the memory held by the memory atom at the beginning of the opened file represented with the hFile handle. The method returns retaining
   // the file handle hFile opened and pointing just after the newly written data. This method and virtual MAtom< tMemClass >& AddToFile( const HANDLE hFile ) can
   // be used in conjunction to serialize number of memory atoms storing them sequentially in a file.
   // The function returns reference to this memory atom.
   virtual const MAtom< tMemClass >& SaveAsFile( const HANDLE hFile ) const throw();


   // Writes the content of the memory held by the memory atom as a file.
   // const TCHAR *ptcFilename          - filename to be used for saving the data
   // const DWORD dwCreationDisposition - same meaning as the dwCreationDisposition in CreateFile file function.
   // const DWORD dwFlagsAndAttributes  - same meaning as the dwFlagsAndAttributes in CreateFile file function.
   // DWORD* pdwLastError               - indicates the required behavior in case of error. If this parameter is not NULL and an error occurs the function 
   //                                     will place the error code in the variable pointed by pdwLastError and return false. If pdwLastError is NULL the 
   //                                     function will throw an exception which will contain the error code indicating the error.
   //                                     Using a returned error code instead of exception helps creating a more concise logic in certain cases, e.g. when using 
   //                                     disposition flag - CREATE_NEW, and inspecting the returned code, and other cases such as sequences of if blocks.
   // The method requires GENERIC_WRITE access rights, exclusive sharing rights and uses default security attributes.
   // The function returns true in case of success, and false or throws an exception in case of error depending on the content of pdwLastError.
   virtual bool SaveAsFile( const TCHAR *ptcFilename, const DWORD dwCreationDisposition, const DWORD dwFlagsAndAttributes, DWORD* pdwLastError ) const throw();


   // Writes the content of the memory held by the memory atom as a file.
   // const TCHAR *ptcFilename          - filename to be used for saving the data
   // const DWORD dwCreationDisposition - same meaning as the dwCreationDisposition in CreateFile file function.
   // const DWORD dwFlagsAndAttributes  - same meaning as the dwFlagsAndAttributes in CreateFile file function.
   // DWORD* pdwLastError               - indicates the required behavior in case of error. If this parameter is not NULL and an error occurs the function 
   //                                     will place the error code in the variable pointed by pdwLastError and return false. If pdwLastError is NULL the 
   //                                     function will throw an exception which will contain the error code indicating the error.
   //                                     Using a returned error code instead of exception helps creating a more concise logic in certain cases, e.g. when using 
   //                                     disposition flag - CREATE_NEW, and inspecting the returned code, and other cases such as sequences of if blocks.
   // The method requires GENERIC_WRITE access rights, exclusive sharing rights and uses default security attributes.
   // The function returns true in case of success, and false or throws an exception in case of error depending on the content of pdwLastError.
   virtual bool SaveAsFile( const TCHAR *ptcFilename, const DWORD dwFlagsAndAttributes, DWORD* pdwLastError ) const throw();


   // Writes the content of the memory held by the memory atom at the end a file. If the file does not exist the function creates it.
   // const TCHAR *ptcFilename          - filename to be used for saving the data.
   // The method requires GENERIC_WRITE access rights, exclusive sharing rights and uses default security attributes.
   // The function returns reference to this memory atom.
   virtual const MAtom< tMemClass >& AppendToFile( const TCHAR *ptcFilename ) const throw();


   // Writes the content of the memory held by the memory atom at the end of opened file.
   // const HANDLE hFile               - handle to file at the end of which to add the memory atom content. The handle must have write permission attributes.
   // The function returns reference to this memory atom.
   virtual const MAtom< tMemClass >& AppendToFile( const HANDLE hFile ) const throw();


   // Writes the content of the memory held by the memory atom in file.
   // const HANDLE hFile               - handle to file where to write the memory atom content. The data will be written at the current file pointer position.
   //                                    The handle must have write permission attributes.
   // The function returns reference to this memory atom.
   virtual const MAtom< tMemClass >& AddToFile( const HANDLE hFile ) const throw();


   // Load an entire file in a memory atom.
   // const TCHAR *ptcFilename - path to the file to be loaded.
   // The size of the loaded file must be a multiple of the granularity of the memory atom. For example a file with size 6 bytes cannot be loaded in memory atom of class DWORD.
   // The function returns reference to this memory atom loaded with the data from the file.
   virtual MAtom< tMemClass >& LoadFile( const TCHAR *ptcFilename ) throw();


   // Load an entire file in a memory atom and returns an open handle to the file.
   // const TCHAR *ptcFilename         - path to the file to be loaded.
   // MHandle& hFile                   - reference to a HANDLE wrapper which will assume the handle to the open file. The caller must close the handle, 
   //                                    which MHandle does automatically. After the function returns the handle points at the end of the file.
   // The size of the loaded file must be a multiple of the granularity of the memory atom. For example a file with size 6 bytes cannot be loaded in memory atom of class DWORD.
   // The function returns reference to this memory atom loaded with the data from the file.
   virtual MAtom< tMemClass >& LoadFile( const TCHAR *ptcFilename, MHandle& hFile ) throw();


   // Load an entire file in a memory atom and returns an open handle to the file.
   // const TCHAR *ptcFilename                   - path to the file to be loaded.
   // const MUnit< tMemClass >& muMaxUnits2Load  - the maximum number of memory units to load. 
   // MHandle* phFile                            - optional reference to a HANDLE wrapper which if supplied will assume the handle to the open file. Pass NULL to avoid returning
   //                                             file handle, pass to a valid MHandle pointer to request returning of a valid file handle. The caller must close the handle 
   //                                             when it is no longer needed, which MHandle does automatically. After the function returns the handle points at the end of the file.
   // The size of the loaded file must be a multiple of the granularity of the memory atom or be bigger than muMaxUnits2Load in bytes. For example a file with size 6 bytes cannot be 
   // loaded in memory atom of class DWORD where muMaxUnits2Load is 2 units, but its first DWORD can be loaded if muMaxUnits2Load is 1.
   // The function returns reference to this memory atom loaded with the data from the file.
   virtual MAtom< tMemClass >& LoadFile( const TCHAR *ptcFilename, const MUnit< tMemClass >& muMaxUnits2Load, MHandle* phFile ) throw();


   // Loads a memory atom with data from file reading at the current position onwards.
   // const MHandle& hFile - handle of open file. The data put into the memory atom is read from the current pointer position of this handle. If there is not enough data in the file 
   // (from the current pointer position to the end of the file) to fill up the memory atom, then the memory atom will adjust itself to reflect the amount of data available. The available 
   // data must be multiple to the granularity of the memory atom. For example a file with size 6 bytes, and handle pointer at 3, cannot be loaded in memory atom of class DWORD.
   // The function returns reference to this memory atom loaded with the data from the file.
   virtual MAtom< tMemClass >& LoadFromFile( const MHandle& hFile ) throw( MAtomException );


   // Fill the content of the memory atom with random data.
   // const bool bSeed          - set to true to use the uiSeed to restart the random generator, set to false to ignore the uiSeed parameter and use the random generator at its current state.
   // const unsigned int uiSeed - restarting value of the random generator. This parameter is ignored if bSeed is false.
   // The function returns reference to this memory atom loaded with random data.
   MAtom< tMemClass >& FillNoise( const bool bSeed, const unsigned int uiSeed ) throw();


   // Fill the content of the memory atom with random data.
   // const unsigned int uiSeed - restart the random generator using this value.
   // The function returns reference to this memory atom loaded with random data.
   MAtom< tMemClass >& FillNoise( const unsigned int uiSeed )                   throw();


   // Fill the content of the memory atom with random data, using the current state of the random generator.
   // The function returns reference to this memory atom loaded with random data.
   MAtom< tMemClass >& FillNoise()                                              throw();


   // Unit-wise XOR of this memory atom and a memory atom of the same type and size, with result of every per-unit XOR saved in this.
   // const MAtom< tMemClass >& maXORMemory - memory atom to XOR this. The operation is this-atom[index] = this-atom[index] XOR maXORMemory[index]
   // The function returns reference to this memory atom unit-wise XOR-ed with maXORMemory.
   MAtom< tMemClass >& XOR( const MAtom< tMemClass >& maXORMemory )              throw();


   // Unit-wise XOR of this memory atom and a memory unit of the same type, with result of every per-unit XOR saved in this.
   // const tMemClass& mtXORUnit - memory unit to XOR this. The operation is this-atom[index] = this-atom[index] XOR mtXORUnit
   // The function returns reference to this memory atom unit-wise XOR-ed with mtXORUnit.
   MAtom< tMemClass >& XOR( const tMemClass& mtXORUnit )                         throw();


   // Fill the content of a memory atom with the value.
   // const tMemClass& objPattern - value used to set every unit in the memory atom.
   // The function returns reference to this memory atom filled with objPattern.
   MAtom< tMemClass >& Set( typename const tMemClass& objPattern ) throw();


   // Byte-wise fill the content of a memory atom with the value.
   // const BYTE b1Wipe - byte value used to set every byte in the memory atom to. Every byte of the contained memory is set to this value.
   // The function returns reference to this memory atom filled with b1Wipe.
   MAtom< tMemClass >& Reset( typename const BYTE b1Wipe )        throw();


   // Unit-wise reversing of the content of the memory atom.
   // The function returns reference to the modified (this) memory atom.
   MAtom< tMemClass >& Reverse() throw();


   // Replace every occurrence of a value in the contained memory units with another value.
   // const tMemClass tReplaceValue - value to replace.
   // const tMemClass tNewValue     - new value to replace all occurrences of tReplaceValue.
   // The function returns reference to the modified (this) memory atom.
   MAtom< tMemClass >& Replace( const tMemClass tReplaceValue, const tMemClass tNewValue ) throw();


   // Remove all units with a particular value, found on the left hand side of the memory contained in a memory atom.
   // The function returns reference to the modified (this) memory atom.
   MAtom< tMemClass >& TrimLeft( const tMemClass tTrimValue ) throw();


   // Remove all units with a particular value, found on the right hand side of the memory contained in a memory atom.
   // The function returns reference to the modified (this) memory atom.
   MAtom< tMemClass >& TrimRight( const tMemClass tTrimValue ) throw();


   // Remove all units with a particular value, found on both sides of the memory contained in a memory atom.
   // The function returns reference to the modified (this) memory atom.
   MAtom< tMemClass >& Trim( const tMemClass tTrimValue ) throw();


   // Find the zero based index of the first occurrence of a particular value with type of the class of the atom.
   // const tMemClass tSearchValue - value to search.
   // The function returns the zero based index of the first match. If there is no match the function returns a memory 
   // unit with the size of the memory atom in class units, which is the first/smallest invalid index.
   MUnit< tMemClass >  Find( const tMemClass tSearchValue ) const throw();


   // Search for a memory match in this memory atom from left to right (from the beginning towards the end).
   // const MAtom< tMemClass >& memSearchFor - memory atom for the content of which to look for in this memory atom.
   // const bool bPartialMatchAtTheEnd       - flag whether a partial match at the end is considered a match, true - yes, false - no.
   // Returns an empty shell memory atom if there is no match. Returns a non empty shell memory atom if there is a complete match, or if there is an incomplete match 
   // and the partial memory flag is set to true. For example a partial match in the middle of the memory is meaningless and an empty shell object will be always returned. 
   // If the partial flag is set to true and there is a partial match at the end of the memory, there may be a full match is there is more data behind the end of the memory 
   // atom, so the method will return a partial match, i.e. a shell memory atom wrapping the memory from the beginning of the match to the end of the memory in this atom.
   // To distinguish a full from a partial match use the size of the returned shell memory atom and memSearchFor.
   const ShellMemory< tMemClass > Search_L2R( typename const MAtom< tMemClass >& memSearchFor, const bool bPartialMatchAtTheEnd ) const;


   // Search for a memory match in this memory atom from left to right (from the beginning towards the end). This method is applicable only for memory atoms with class sizes of only one or two bytes.
   // const MAtom< tMemClass >& memSearchFor - memory atom for the content of which to look for in this memory atom.
   // const bool bPartialMatchAtTheEnd       - flag whether a partial match at the end is considered a match, true - yes, false - no.
   // const bool bCaseSensitive              - set to true for case sensitive search, and false otherwise.
   // Returns an empty shell memory atom if there is no match. Returns a non empty shell memory atom if there is a complete match, or if there is an incomplete match 
   // and the partial memory flag is set to true. For example a partial match in the middle of the memory is meaningless and an empty shell object will be always returned. 
   // If the partial flag is set to true and there is a partial match at the end of the memory, there may be a full match is there is more data behind the end of the memory 
   // atom, so the method will return a partial match, i.e. a shell memory atom wrapping the memory from the beginning of the match to the end of the memory in this atom.
   // To distinguish a full from a partial match use the size of the returned shell memory atom and memSearchFor.
   const ShellMemory< tMemClass > Search_L2R( typename const MAtom< tMemClass >& memSearchFor, const bool bPartialMatchAtTheEnd, const bool bCaseSensitive ) const;


   // Find this memory atom content in a file.
   // const MHandle& hFile                - handle to an open file to search in. The handle must have read and seek permissions.
   // const unsigned __int64 uiSearchFrom - starting position, in bytes, to start the search in the file.
   // unsigned __int64* puiFoundAt        - optional parameter to hold the starting position of the first found match from the beginning of the file in bytes. NULL - starting position is not required.
   // const HANDLE heStop                 - synchronization event handle to asynchronously terminate the search.
   // Returns true if the file contains data identical to the content of this memory atom, and false if a match was not found.
   bool  FindInFile( const MHandle& hFile, const unsigned __int64 uiSearchFrom, unsigned __int64* puiFoundAt, const HANDLE heStop ) const;


   // Encode the content of the memory atom as ASCII in chars. Every byte is split to more/less significant 4 bits, which number is converted to ASCII. For example ABCD\0,
   // i.e. 0x41 0x42 0x43 0x44 0x00 becomes 0x34 0x31 0x34 0x32 0x34 0x33 0x34 0x34 0x30 0x30. The function returns a new memory atom with the encoded data from this.
   MemoryPH< char >   Bin2Ascii() const;


   // Decode ASCII encoded binary data. This function is inverse of MemoryPH< char > Bin2Ascii() const;.
   MAtom< tMemClass >& Ascii2Bin();


   // Creates and returns an 8 byte integer hash code for the content of the memory atom.
   unsigned __int64 MHash() const;


   // Creates and returns a hash code of the contained by the memory atom memory as a floating point number with double precision.
   virtual double GetHash1() const; // throw( may throw an overflow exception in future )
};


// Generic Memory Atom using the Process Heap for memory atom Type::Origin property, i.e. as memory resource. Inherits MAtom< tMemClass >.
// class tMemClass - defines the Type::Class property of the memory atom, i.e. granularity and type of the memory units contained by the memory atom.
template< class tMemClass >
class MemoryPH : public MAtom< tMemClass >
{
protected:
   void AllocateMemorySize()
   {
      pData = new tMemClass[muSize.GetUnits()];

      if( nullptr == pData )
      {
         MAtomException::ExceptionInsufficientMemory();
      }
   }


public:
   // Pure polymorphic method.
   // Returns the Memory Origin.
   virtual MemoryOrigin GetMemoryOrigin() const
   {
      return( Heap );
   }


   // Standard default constructor - allocates zero bytes.
   MemoryPH()
   {
      AllocateMemorySize();
   }


   // Standard copy constructor - creates a new memory object using the source object.
   // const MemoryPH< tMemClass >& maSource - source memory atom used to construct this object.
   // The constructor allocates exactly as much memory as maSource holds and copies its content onto the newly allocated memory.
   MemoryPH( typename const MemoryPH< tMemClass >& maSource )
   {
      muSize = maSource.muSize;

      AllocateMemorySize();

      memcpy( pData, maSource.pData, muSize.InBytes() );
   }


   // Copy constructor from any MAtom< tMemClass > descendent memory atom type - creates a new memory object using the source object.
   // const MAtom< tMemClass >& maSource - source memory atom used to construct this object.
   // The constructor allocates exactly as much memory as maSource holds and copies its content onto the newly allocated memory.
   MemoryPH( typename const MAtom< tMemClass >& maSource )
   {
      muSize = maSource.GetSize();

      AllocateMemorySize();

      memcpy( pData, maSource.GetMemory(), muSize.InBytes() );
   }


   // Constructor allocating required number of memory units.
   // const MUnit< tMemClass >& muUnits - number of memory units that the newly created memory atom must allocate.
   // The template specialization parameters determine the class of the allocated memory, which must match to the class of muUnits, and whether allocated/reallocated memory must be zeroed or not.
   // Typical use: MemoryPH< DWORD, true > ma100ZeroedDWords( MUnit< DWORD >( 100 ) ); // allocate 100 * sizeof( DWORD ) bytes.
   MemoryPH( typename const MUnit< tMemClass >& muUnits )
   {
      muSize = muUnits;

      AllocateMemorySize();
   }


   // Constructor allocating required number of memory units, and initializing them with a value.
   // const MUnit< tMemClass >& muUnits - number of memory units that the newly created memory atom must allocate.
   // const tMemClass& tobjInitValue    - value used to initialize the allocated memory unites.
   // The template specialization parameters determine the class of the allocated memory, which must match to the class of muUnits, and whether reallocated memory must be zeroed or not.
   // Typical use: MemoryPH< DWORD, true > ma100ZeroedDWords( MUnit< DWORD >( 100 ), 0xA5A5A5A5 ); // allocate 100 * sizeof( DWORD ) bytes, and initialize every unit in the memory atom with 0xA5A5A5A5.
   MemoryPH( typename const MUnit< tMemClass >& muUnits, typename const tMemClass& tobjInitValue )
   {
      muSize = muUnits;

      AllocateMemorySize();

      Set( tobjInitValue );
   }


   // Constructor creating a memory atom coping non Atomic Memory Model managed memory.
   // const tMemClass* pSrcData         - pointer to a non Atomic Memory Model managed memory to be copied in the memory atom.
   // const MUnit< tMemClass >& muUnits - size of the source memory in memory units.
   // The template specialization parameter determines the class of the allocated memory.
   // Typical use: MemoryPH< DWORD > ma100DWords( pDWordSourceMemory, MUnit< DWORD >( 100 ) ); // allocate 100 * sizeof( DWORD ) bytes, and copy the content of 100 * sizeof( DWORD ) bytes pointed by pDWordSourceMemory.
   MemoryPH( const tMemClass* pSrcData, typename const MUnit< tMemClass >& muUnits )
   {
      muSize = muUnits;

      AllocateMemorySize();

      memcpy( pData, pSrcData, muSize.InBytes() );
   }


   // Destructor releasing the memory held by the memory atom.
   virtual ~MemoryPH()
   {
      MAtomException::TestNonNullPointer( pData );
      
      if( nullptr != pData )
      {
         delete[] pData;

         pData = nullptr;
      }
   }


   // Empty the memory atom. The memory contained by the atom is released to the process heap, and a new zero size chunk is allocated.
   // The method returns reference to this memory atom emptied.
   virtual MAtom< tMemClass >& Empty() throw( MAtomException )
   {
      MAtomException::TestNonNullPointer( pData );

      if( nullptr != pData )
      {
         delete[] pData;

         pData = nullptr;
      }

      muSize = 0;

      AllocateMemorySize();

      return( *this );
   }


   // Operator equal replicating the content of this object with that of any MAtom< tMemClass > descendent memory atom type.
   // const MAtom< tMemClass >& maSource - source memory atom used to modify this object.
   // The operator frees the contained memory and then allocates exactly as much memory as maSource holds, after which it copies its content onto the newly allocated memory of this atom.
   // The method returns reference to the modified (this) memory atom.
   virtual MAtom< tMemClass >& operator=( typename const MAtom< tMemClass >& maSource )
   {
      if( nullptr != pData )
      {
         delete[] pData;

         pData = nullptr;
      }

      muSize = maSource.GetSize();

      AllocateMemorySize();

      memcpy( pData, maSource.GetMemory(), muSize.InBytes() );

      return( *this );
   }


   // Standard operator equal for the type, replicating the content of maSource into this object.
   // const MemoryPH< tMemClass >& maSource - source memory atom used to modify this object.
   // The operator frees the contained memory and then allocates exactly as much memory as maSource holds, after which it copies its content onto the newly allocated memory of this atom.
   // The method returns reference to the modified (this) memory atom.
   virtual MemoryPH< tMemClass >& operator=( typename const MemoryPH< tMemClass >& maSource )
   {
      if( nullptr != pData )
      {
         delete[] pData;

         pData = nullptr;
      }

      muSize = maSource.muSize;

      AllocateMemorySize();

      memcpy( pData, maSource.pData, muSize.InBytes() );

      return( *this );
   }


   // Virtual override function always returning true for this type as it is able to reallocate memory.
   virtual bool CanReAllocate() const throw()
   {
      return( true );
   }


   // Free the contained memory and reallocate new memory with the required size.
   // const MUnit< tMemClass >& muUnits - size of the new memory to allocate in memory units.
   // The method returns reference to the modified (this) memory atom.
   virtual MAtom< tMemClass >& ReAllocate( typename const MUnit< tMemClass >& muUnits ) throw( MAtomException )
   {
      if( nullptr != pData )
      {
         delete[] pData;

         pData = nullptr;
      }

      muSize = muUnits;

      AllocateMemorySize();

      return( *this );
   }


   // Free the contained memory and replicate the content of the MAtom< tMemClass > descendent memory atom passed as parameter.
   // const MAtom< tMemClass >& maSource - source memory atom used to modify this object.
   // The method frees the contained memory and then allocates exactly as much memory as maSource holds, after which it copies its content onto the newly allocated memory of this atom.
   // The method returns reference to the modified (this) memory atom.
   virtual MAtom< tMemClass >& ReAllocateTransfer( typename const MAtom< tMemClass >& maSource ) throw( MAtomException )
   {
      ReAllocate( maSource.GetSize() );

      MUnit< tMemClass > muOffsets( 0 );

      Transfer( muOffsets, false, maSource, muOffsets, false, GetSize() );

      return( *this );
   }


   // Concatenates two memory atoms into a new MemoryPH memory atom.
   // const MAtom< tMemClass >& maSummandL - the content of this memory atom will be placed at the beginning of the result memory atom.
   // const MAtom< tMemClass >& maSummandR - the content of this memory atom will be placed after the content of maSummandL in the result memory atom.
   // The method returns a new MemoryPH< tMemClass > memory atom holding copy of the contents of the parameters as follow: [maSummandL][maSummandr].
   // Note that the returned memory atom is passed back in a standard manner through a standard copy constructor, and thus this is not a speed and memory efficient 
   // function, which should be used primarily for memory atoms not containing large amounts of memory. For such cases use the Transfer( parameters ) functions.
   friend inline MemoryPH< tMemClass > operator +( typename const MAtom< tMemClass >& maSummandL, typename const MAtom< tMemClass >& maSummandR ) throw( MAtomException )
   {
      MemoryPH< tMemClass > memResult( maSummandL.GetSize() + maSummandR.GetSize() );

      MUnit< tMemClass > muOffset;

      memResult.Transfer( muOffset, true, maSummandL ); 
      memResult.Transfer( muOffset, true, maSummandR ); 

      return( memResult );
   }
};


// Memory Atom not holding own memory but instead wrapping/referencing memory owned by another entity. Inherits MAtom< tMemClass >.
// class tMemClass - defines the Type::Class property of the memory atom, i.e. granularity and type of the memory units referenced by the memory atom.
// This memory atom is useful to help supply the MAtom< tMemClass > interface to non Atomic Memory Model owned memory, as well as otherwise helper class.
template< class tMemClass >
class ShellMemory : public MAtom< tMemClass >
{
public:
   // Pure polymorphic method. Returns the Memory Origin. 
   virtual MemoryOrigin GetMemoryOrigin() const
   {
      return( VariableUnidentifiable );
   }


   // Default constructor - no memory is allocated. The memory object is set to point to NULL memory which has zero size.
   ShellMemory()
   {
   }


   // Copy constructor from any MAtom< tMemClass > descendent memory atom type - creates a new memory object using the source object.
   // const MAtom< tMemClass >& maSource - source memory atom used to construct this object.
   // The newly created object does not allocate any memory, instead it points to the same memory, with the same size, which maSource owns/references.
   ShellMemory( const MAtom< tMemClass >& maSource ) : MAtom< tMemClass >( maSource )
   {
   }


   // Constructor initializing a new ShellMemory memory atom representing a portion of memory owned/referenced by another memory atom.
   // MAtom< tMemClass >& maSource       - the memory atom part of whose content will be represented by the newly created memory atom.
   // const MUnit< tMemClass >& muOffset - zero based offset of the first memory unit in the source memory atom which will be represented by this.
   // const MUnit< tMemClass >& muSize   - number of memory units which will be referenced by the newly created memory atom.
   // This constructor does not allocate any memory. Instead it simply creates a ShellMemory atom which points memory with certain size owned by another entity.
   ShellMemory( MAtom< tMemClass >& maSource, const MUnit< tMemClass >& muOffset, const MUnit< tMemClass >& muSize ) : MAtom< tMemClass >( maSource, muOffset, muSize )
   {
   }


   // Standard copy constructor - creates a new memory object using the source object.
   // const ShellMemory< tMemClass >& maSource - source memory atom used to construct this object.
   // This constructor does not allocate any memory. Instead it simply creates a ShellMemory atom which points the memory with the same size pointed by the source object.
   ShellMemory( const ShellMemory< tMemClass >& maSource ) : MAtom< tMemClass >( maSource )
   {
   }


   // Constructor initializing a new ShellMemory memory atom using the abstract parent of all memory atom classes in the Atomic Memory Atom implementation I.
   // MMemory< tMemClass >& maSource - source memory atom used to construct this object.
   // This constructor does not allocate any memory. Instead it simply creates a ShellMemory atom which points the memory with the same size pointed by the source object.
   // This constructor is useful for creating interfacing memory atoms for use in code utilizing memory atoms from implementation one and two. 
   ShellMemory( MMemory< tMemClass >& maSource ) : MAtom< tMemClass >( maSource.GetMemory(), MUnit< tMemClass >( maSource.GetSizeUnits() ) )
   {
   }


   // Constructor creating a memory atom referencing non Atomic Memory Model managed memory.
   // tMemClass* pSrcData               - pointer to a non Atomic Memory Model managed memory to be referenced by the memory atom.
   // const MUnit< tMemClass >& muUnits - size of the source memory in memory units.
   // The template specialization parameter determines the class of the assumed memory.
   // Typical use: ShellMemory< DWORD > ma100DWords( pDWordSourceMemory, MUnit< DWORD >( 100 ) ); // references 100 * sizeof( DWORD ) bytes pointed by pDWordSourceMemory.
   ShellMemory( tMemClass* pSrcData, typename const MUnit< tMemClass >& muUnits ) : MAtom< tMemClass >( pSrcData, muUnits )
   {
   }


   // Destructor - does not release any resources as the memory atom does not hold any.
   virtual ~ShellMemory()
   {
   }


   // Empty the memory atom. The memory object is set to point to NULL memory which has zero size.
   // The method returns reference to this memory atom pointing to NULL memory with zero size.
   virtual MAtom< tMemClass >& Empty() throw( MAtomException )
   {
      pData  = NULL;
      muSize = 0;

      return( *this );
   }


   // Operator equal assuming the content of any MAtom< tMemClass > descendent memory atom type by this object.
   // MAtom< tMemClass >& maSource - source memory atom used to modify this object.
   // The operator assumes the memory owned/referenced by the source memory atom. Notice that the passed memory object is NOT const since this memory atom is 
   // mutable and assumes the same memory chunk which the source atom holds and it could change it. The method returns reference to the modified (this) memory atom.
   virtual MAtom< tMemClass >& operator=( typename MAtom< tMemClass >& maSource )
   {
      pData  = maSource.GetMemory();
      muSize = maSource.GetSize();

      return( *this );
   }


   // Standard operator equal for the type, assuming the memory referenced by maSource.
   // ShellMemory< tMemClass >& maSource - source memory atom used to modify this object.
   // The operator assumes the memory referenced by the source memory atom. Notice that the passed memory object is NOT const since this memory atom is mutable 
   // and assumes the same memory chunk which the source atom references and it could change it. The method returns reference to the modified (this) memory atom.
   virtual ShellMemory< tMemClass >& operator=( typename ShellMemory< tMemClass >& maSource )
   {
      pData  = maSource.GetMemory();
      muSize = maSource.GetSize();

      return( *this );
   }


   // Virtual override function always returns false for this type as it is not able to reallocate memory.
   virtual bool CanReAllocate() const throw()
   {
      return( false );
   }


   // This ReAllocate is permitted only if this object refers to NULL memory, allowing it to be used as information carrier.
   // const MUnit< tMemClass >& muNewSize - new size of the MULL memory.
   // Generally this method is inconsistent since const memory assumed by mutable object may be changed, hence the above limitation.
   // The method returns reference to the modified (this) memory atom.
   virtual MAtom< tMemClass >& ReAllocate( typename const MUnit< tMemClass >& muNewSize ) throw( MAtomException )
   {
      if( NULL != pData )
      {
         MAtomException::ExceptionIlligalMethodCall();
      }

      muSize = muNewSize;

      return( *this );
   }


   // Assumes content of any MAtom< tMemClass > descendent memory atom passed as parameter.
   // const MAtom< tMemClass >& maSource - source memory atom used to modify this object.
   // Generally this method is inconsistent since const memory assumed by mutable object may be changed. However there are cases where the method can be helpful, hence the
   // unambiguous name ReAllocateTransferMutableAssumedMemory clarifying the resolution of the issue. The method returns reference to the modified (this) memory atom.
   virtual MAtom< tMemClass >& ReAllocateTransferMutableAssumedMemory( typename const MAtom< tMemClass >& maSource ) throw( MAtomException )
   {
      pData  = (tMemClass*)maSource.GetMemory();
      muSize = maSource.GetSize();

      return( *this );
   }


private:
   // Impossible: Operator equal assuming constant reference to the content of any MAtom< tMemClass > descendent memory atom type by this object.
   // Reason: This operator cannot exist because it is impossible to guarantee that the object (this) will remain constant after passing the const source to it.
   virtual MAtom< tMemClass >& operator=( typename const MAtom< tMemClass >& /*maSource*/ )
   {
      MAtomException::ExceptionIlligalMethodCall();

#ifdef _DEBUG  // clear warning C4702: unreachable code
      return( *this );
#endif
   }


   // Impossible: ReAllocateTransfer on constant source memory.
   // Reason: This method cannot exist because the const attribute cannot be enforced after the const source memory is delegated to a mutable object.
   virtual MAtom< tMemClass >& ReAllocateTransfer( typename const MAtom< tMemClass >& /*maSource*/ ) throw( MAtomException )
   {
      MAtomException::ExceptionIlligalMethodCall();
      
#ifdef _DEBUG  // clear warning C4702: unreachable code
      return( *this );
#endif
   }
};


// Generic Memory Atom using the GlobalAlloc for memory atom Type::Origin property, i.e. as memory resource, and is implementing the system IStream interface. Inherits MAtom< tMemClass >.
// class tMemClass - defines the Type::Class property of the memory atom, i.e. granularity and type of the memory units contained by the memory atom.
template< class tMemClass >
class HandleMemory : public MAtom< tMemClass >
{
   HGLOBAL  hMemory;
   IStream* pStream;


private:
   void ConstructObject( const HMODULE hmModule, const TCHAR* strType, const DWORD dwName )
   {
      MASSERT( NULL != hmModule );

      // Locate the resource.
      const HRSRC hResFound( FindResource( hmModule, MAKEINTRESOURCE( dwName ), strType ) );

      if( NULL != hResFound )
      {
         // Load the resource.
         const MUnit< BYTE > muSize( ::SizeofResource( hmModule, hResFound ) );
         const HRSRC         hrRes( (HRSRC)::LoadResource( hmModule, hResFound ) );

         if( NULL != hrRes )
         {
            // Cannot accept resource that is not fit for this memory.
            if( muSize.CanItBe< tMemClass >() )
            {
               // Lock the resource.
               const BYTE* pb1ResLock( (BYTE*)LockResource( hrRes ) );

               if( NULL != pb1ResLock )
               {
                  hMemory = ::GlobalAlloc( GMEM_MOVEABLE, muSize.InBytes() );

                  if( NULL != hMemory )
                  {
                     pData = (tMemClass*)::GlobalLock( hMemory );

                     if( NULL != pData )
                     {
                        CopyMemory( pData, pb1ResLock, muSize.InBytes() );

                        this->muSize = muSize;

                        ::GlobalUnlock( hMemory );

                        ::CreateStreamOnHGlobal( hMemory, FALSE, &pStream );
                     }
                  }
               }
            }
         }
      }
   }


public:
   // Pure polymorphic method. Returns the Memory Origin.
   virtual MemoryOrigin GetMemoryOrigin() const
   {
      return( GlobalAllocFree );
   }


   // Constructor from resource located in the current executable module, the resource type and name.
   // const TCHAR* strType   - resource type.
   // const DWORD dwName     - resource name.
   explicit HandleMemory( const TCHAR* strType, const DWORD dwName ) : hMemory( NULL ), pStream( NULL )
   {
      ConstructObject( GetModuleHandle( NULL ), strType, dwName );
   }


   // Constructor from handle to module, resource type and name.
   // const HMODULE hmModule - handle to the module containing the resource to be loaded in the newly created memory atom.
   // const TCHAR* strType   - resource type.
   // const DWORD dwName     - resource name.
   explicit HandleMemory( const HMODULE hmModule, const TCHAR* strType, const DWORD dwName ) : hMemory( NULL ), pStream( NULL )
   {
      ConstructObject( NULL != hmModule ? hmModule : GetModuleHandle( NULL ), strType, dwName );
   }


   // Constructor from path to module, resource type and name.
   // const TCHAR* strType   - resource type.
   // const DWORD dwName     - resource name.
   // const TCHAR* strModule - path to the module containing the resource to be loaded in the newly created memory atom.
   explicit HandleMemory( const TCHAR* strModule, const TCHAR* strType, const DWORD dwName ) : hMemory( NULL ), pStream( NULL )
   {
      const HMODULE hmModule( NULL == strModule ? GetModuleHandle( NULL ) : LoadLibrary( strModule ) );

      if( NULL != hmModule )
      {
         ConstructObject( hmModule, strType, dwName );

         if( NULL != strModule )
         {
            FreeLibrary( hmModule );
         }
      }
   }


   // Destructor releasing the memory held by the memory atom.
   virtual ~HandleMemory()
   {
      Empty();
   }


   // Return IStream pointer to the allocated memory.
   IStream* GetStream()
   {
      return( pStream );
   }


   // Empty the memory atom. The memory contained by the atom is released, and the memory object is set to point to NULL memory which has zero size.
   // The method returns reference to this memory atom pointing to NULL memory with zero size.
   virtual MAtom< tMemClass >& Empty() throw( MAtomException )
   {
      if( NULL != pStream )
      {
         pStream->Release();
      }

      ::GlobalFree( hMemory );

      pStream = NULL;
      hMemory = NULL;
      pData   = NULL;
      muSize  = 0;

      return( *this );
   }


   // Virtual override function always returns false for this type as it is not able to reallocate memory.
   virtual bool CanReAllocate() const throw()
   {
      return( false );
   }


   // Writes the content of the memory held by the memory atom at the beginning of the opened file represented with the hFile handle. The method returns retaining
   // the file handle hFile opened and pointing just after the newly written data.
   // The method returns reference to this memory atom.
   template< class tMemClass > const MAtom< tMemClass >& SaveAsFile( const HANDLE hFile ) const throw()
   {
      if( (INVALID_HANDLE_VALUE == hFile) || (NULL == hFile) )
      {
         MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
      }


      LARGE_INTEGER li;
      li.QuadPart = 0;

      if( !SetFilePointerEx( hFile, li, NULL, FILE_BEGIN ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }

      if( FAILED( pStream->Seek( li,STREAM_SEEK_SET, NULL ) ) )
      {
         MAtomException::ExceptionFailedToOperate( ERROR_SEEK );
      }

      BYTE b1Data[8192];
      ULONG ulProcessed( 0 );

      do
      {
         if( FAILED( pStream->Read( b1Data, sizeof( b1Data ), &ulProcessed ) ) )
         {
            SetFilePointerEx( hFile, li, NULL, FILE_BEGIN );

            MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
         }

         if( 0 != ulProcessed )
         {
            if( !WriteFile( hFile, b1Data, sizeof( b1Data ), &ulProcessed, NULL ) )
            {
               const DWORD dwLastError( ::GetLastError() );

               SetFilePointerEx( hFile, li, NULL, FILE_BEGIN );

               MAtomException::ExceptionFailedToOperate( dwLastError );
            }
         }
      }
      while( 0 != ulProcessed );
   
   
      return( *this );
   }


private:
   // Not implemented.
   virtual MAtom< tMemClass >& operator=( typename const MAtom< tMemClass >& /*maSource*/ )
   {
      MAtomException::ExceptionIlligalMethodCall();
      
      return( *this );
   }


   // Not implemented.
   virtual MAtom< tMemClass >& ReAllocate( typename const MUnit< tMemClass >& /*muUnits*/  ) throw( MAtomException )
   {
      MAtomException::ExceptionIlligalMethodCall();
      
      return( *this );
   }


   // Not implemented.
   virtual MAtom< tMemClass >& ReAllocateTransfer( typename const MAtom< tMemClass >& /*maSource*/ ) throw( MAtomException )
   {
      MAtomException::ExceptionIlligalMethodCall();
      
      return( *this );
   }


   // Not implemented.
   virtual MAtom< tMemClass >& operator=( typename MAtom< tMemClass >& /*maSource*/ )
   {
      MAtomException::ExceptionIlligalMethodCall();
      
      return( *this );
   }
};


// Specialized Memory Atom derivate of any MAtom< tMemClass > insatiable memory atom type, with Type::Semantics to zero any memory before it is released. 
// class MemoryType - must be a MAtom< tMemClass > derivate memory atom which does the memory management and performs the fundamental memory handling and operations.
// class tMemClass  - defines the Type::Class property of the memory atom, and must be the same as the Type::Class property of the superclass MemoryType.
template< class MemoryType, class tMemClass >
class SecureMemory : public MemoryType
{
   template< class tChar > friend class MSecureString;


private:
   __forceinline void DisqualifyContent() throw()
   {
      if( NULL != pData )
      {
         SecureZeroMemory( (void*)pData, muSize.InBytes() );
      }
   }


public:
   // Standard default constructor - allocates zero bytes.
   SecureMemory() : MemoryType()
   {
   }


   // Effectively standard copy constructor - creates a new memory object using the source object.
   // const MemoryType maSource - source memory atom used to construct this object.
   // The constructor passes the control to the standard copy constructor of the specializing parent class which handles the memory operations.
   SecureMemory( const MemoryType maSource ) : MemoryType( maSource )
   {
   }


   // Copy constructor from any MAtom< tMemClass > descendent memory atom type - creates a new memory object using the source object.
   // const MAtom< tMemClass >& maSource - source memory atom used to construct this object.
   // The constructor passes the control to the same prototype copy constructor of the specializing parent class which handles the memory operations.
   SecureMemory( const MAtom< tMemClass >& maSource ) : MemoryType( maSource )
   {
   }


   // Constructor creating a memory atom coping non Atomic Memory Model managed memory.
   // const tMemClass* pSrcData         - pointer to a non Atomic Memory Model managed memory to be copied in the memory atom.
   // const MUnit< tMemClass >& muUnits - size of the source memory in memory units.
   // The template specialization parameter determines the class of the allocated memory.
   // Typical use: SecureMemory< MemoryPH< BYTE >, BYTE > memData( MUnit< BYTE >( 40 ) );
   SecureMemory( const tMemClass* pSrcData, typename const MUnit< tMemClass >& muUnits ) : MemoryType( pSrcData, muUnits )
   {
   }


   // Destructor forcing zeroing of the memory before releasing it appropriately via the specializing parent destructor.
   virtual ~SecureMemory()
   {
      DisqualifyContent();
   }


   // Empty the memory atom. First erases the memory by forcing zeroing and then releases it via the specializing parent Empty() operation.
   // The method returns reference to this memory atom emptied.
   virtual MAtom< tMemClass >& Empty() throw( MAtomException )
   {
      DisqualifyContent();

      MemoryType::Empty();

      return( *this );
   }


   // Operator equal replicating the content of this object with that of any MAtom< tMemClass > descendent memory atom type.
   // const MAtom< tMemClass >& maSource - source memory atom used to modify this object.
   // The operator frees the contained memory after erasing it, and then passes the control to the identical operator= of the 
   // specializing parent class which handles the memory operations to appropriately handle the allocation and copy operations.
   // The method returns reference to the modified (this) memory atom.
   virtual MAtom< tMemClass >& operator=( typename MAtom< tMemClass >& maSource )
   {
      DisqualifyContent();

      return( MemoryType::operator=( maSource ) );
   }


   // Decreases the number of the contained memory units to the muSize passed as parameter, after cleaning the extra memory.
   // The function returns a reference to the modified memory atom.
   virtual MAtom< tMemClass >& LimitSizeTo( typename const MUnit< tMemClass >& muSize ) throw( MAtomException )
   {
      MAtomException::TestInRangeZero2UpperLimit< true, true >( muSize.GetUnits(), this->muSize.GetUnits() );

      tMemClass*               pData2( pData );
      const MUnit< tMemClass > muSize2( this->muSize );

      pData         = (tMemClass*)((size_t)pData + muSize.InBytes());
      this->muSize -= muSize;

      DisqualifyContent();

      pData        = pData2;
      this->muSize = muSize2;

      return( MemoryType::LimitSizeTo( muSize ) );
   }


private:
   // Do not let access to casting as non secure memory.
   operator       MemoryType  () const throw() { return( *this ); }
   operator       MemoryType  ()       throw() { return( *this ); }
   operator const MemoryType& () const throw() { return( *this ); }
   operator       MemoryType& ()       throw() { return( *this ); }
   operator const MemoryType* () const throw() { return(  this ); }
   operator       MemoryType* ()       throw() { return(  this ); }
};


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LoadResource( const TCHAR* strType, const DWORD dwName ) throw( MAtomException )
{
   return( LoadResource( hDefaultTextResourceModule, strType, dwName ) );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LoadResource( const TCHAR* strModule, const TCHAR* strType, const DWORD dwName ) throw( MAtomException )
{
   const MModuleHandle hmModule( LoadLibrary( strModule ) );

   return( LoadResource( hmModule, strType, dwName ) );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LoadResource( const HMODULE hModule, const TCHAR* strType, const DWORD dwName ) throw( MAtomException )
{
   MASSERT( NULL != hModule );

   MAtomException::TestCompatibleCall( CanReAllocate() );
   MAtomException::TestNonNullPointer( strType );

   if( NULL != hModule )
   {
      // Locate the resource.
      const HRSRC hResFound( FindResource( hModule, MAKEINTRESOURCE( dwName ), strType ) );

      if( NULL != hResFound )
      {
         // Load the resource.
         const MUnit< BYTE > mu_size( ::SizeofResource( hModule, hResFound ) );
         const HRSRC         hrRes( (HRSRC)::LoadResource( hModule, hResFound ) );

         if( NULL != hrRes )
         {
            // Cannot accept resource that is not fit for this memory.
            if( mu_size.CanItBe< tMemClass >() )
            {
               // Lock the resource.
               const BYTE* pb1ResLock( (BYTE*)LockResource( hrRes ) );

               if( NULL != pb1ResLock )
               {
                  ReAllocateTransfer( ShellMemory< tMemClass >( (tMemClass*)pb1ResLock, mu_size.As< tMemClass >() ) );

                  return( *this );
               }
            }
         }
      }
   }

   Empty();

   return( *this );
}


template< class tMemClass > bool MAtom< tMemClass >::StoreResource( LPCTSTR strModule, LPCTSTR strType, const DWORD dwName, const USHORT usLanguage, const USHORT usSubLanguage, const BOOL bDeleteExistingResources ) const
{
   MASSERT( NULL != strModule );
   MASSERT( NULL != strType   );

   HANDLE hUpdateRes( BeginUpdateResource( strModule, bDeleteExistingResources ) );

   if( NULL != hUpdateRes )
   {
      if( UpdateResource( hUpdateRes, strType, MAKEINTRESOURCE( dwName ), MAKELANGID( usLanguage, usSubLanguage ), (void*)GetMemory(), GetSize().InBytes32() ) )
      {
         return( FALSE != EndUpdateResource( hUpdateRes, FALSE ) );
      }
   }

   return( false );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LoadFile( const TCHAR *ptcFilename ) throw()
{
   MAtomException::TestCompatibleCall( CanReAllocate() );


   MHandle hFile( CreateFile( ptcFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) );


   if( !hFile.IsValid() )
   {
      Empty();

      MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
   }


   LARGE_INTEGER ilFileSize;
   if( !GetFileSizeEx( hFile, &ilFileSize ) )
   {
      const DWORD dwLastError( ::GetLastError() );

      Empty();

      MAtomException::ExceptionFailedToOperate( dwLastError );
   }


   const MUnit< BYTE > muFileSize( (MUI)ilFileSize.QuadPart );

   // Cannot accept resource that is not fit for this memory.
   if( !muFileSize.CanItBe< tMemClass >() )
   {
      Empty();

      MAtomException::TestCompatibleCall( false );

      return( *this );
   }


   ReAllocate( muFileSize.As< tMemClass >() );


   unsigned __int64 uiReadTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesRead( 0 );
      if( !ReadFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesRead, NULL ) )
      {
         const DWORD dwLastError( ::GetLastError() );

         Empty();

         MAtomException::ExceptionFailedToOperate( dwLastError );
      }

      if( dwChunkSize != dwBytesRead )
      {
         Empty();

         MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
      }

      uiReadTotal += dwBytesRead;
   }


   if( muSize.InBytes() != uiReadTotal )
   {
      Empty();

      MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
   }


   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LoadFile( const TCHAR *ptcFilename, MHandle& hFile ) throw()
{
   MAtomException::TestCompatibleCall( CanReAllocate() );


   hFile = CreateFile( ptcFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );


   if( !hFile.IsValid() )
   {
      Empty();

      MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
   }


   LARGE_INTEGER ilFileSize;
   if( !GetFileSizeEx( hFile, &ilFileSize ) )
   {
      const DWORD dwLastError( ::GetLastError() );

      Empty();

      MAtomException::ExceptionFailedToOperate( dwLastError );
   }


   const MUnit< BYTE > muFileSize( (MUI)ilFileSize.QuadPart );

   // Cannot accept resource that is not fit for this memory.
   if( !muFileSize.CanItBe< tMemClass >() )
   {
      Empty();

      MAtomException::TestCompatibleCall( false );

      return( *this );
   }


   ReAllocate( muFileSize.As< tMemClass >() );


   unsigned __int64 uiReadTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesRead( 0 );
      if( !ReadFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesRead, NULL ) )
      {
         const DWORD dwLastError( ::GetLastError() );
         
         Empty();

         MAtomException::ExceptionFailedToOperate( dwLastError );
      }

      if( dwChunkSize != dwBytesRead )
      {
         Empty();

         MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
      }

      uiReadTotal += dwBytesRead;
   }


   if( muSize.InBytes() != uiReadTotal )
   {
      Empty();

      MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
   }


   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LoadFile( const TCHAR *ptcFilename, const MUnit< tMemClass >& muMaxUnits2Load, MHandle* phFile ) throw()
{
   MAtomException::TestCompatibleCall( CanReAllocate() );


   MSmartPtr< MHandle > hFile( NULL == phFile ? new MHandle : phFile );


   hFile->operator =( CreateFile( ptcFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) );


   if( !hFile->IsValid() )
   {
      const DWORD dwError( GetLastError() );

      Empty();

      if( NULL != phFile )
      {
         hFile.ExportObject();
      }

      MAtomException::ExceptionInvaldCall( dwError );
   }


   LARGE_INTEGER ilFileSize;
   if( !GetFileSizeEx( hFile->operator HANDLE(), &ilFileSize ) )
   {
      const DWORD dwLastError( ::GetLastError() );

      Empty();

      if( NULL != phFile )
      {
         hFile.ExportObject();
      }

      MAtomException::ExceptionFailedToOperate( dwLastError );
   }


   const MUnit< BYTE > muFileSize( (MUI)ilFileSize.QuadPart );


   // Cannot accept resource that is not fit for this memory.
   if( !muFileSize.CanItBe< tMemClass >() )
   {
      Empty();

      if( NULL != phFile )
      {
         hFile.ExportObject();
      }

      MAtomException::TestCompatibleCall( false );

      return( *this );
   }


   ReAllocate( MMIN< MUnit< tMemClass >, MUnit< tMemClass >, MUnit< tMemClass > >( muMaxUnits2Load, muFileSize.As< tMemClass >() ) );


   unsigned __int64 uiReadTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesRead( 0 );
      if( !ReadFile( hFile->operator HANDLE(), ptrDataCopy, dwChunkSize, &dwBytesRead, NULL ) )
      {
         const DWORD dwLastError( ::GetLastError() );
         
         Empty();

         if( NULL != phFile )
         {
            hFile.ExportObject();
         }

         MAtomException::ExceptionFailedToOperate( dwLastError );
      }

      if( dwChunkSize != dwBytesRead )
      {
         Empty();

         if( NULL != phFile )
         {
            hFile.ExportObject();
         }

         MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
      }

      uiReadTotal += dwBytesRead;
   }


   if( muSize.InBytes() != uiReadTotal )
   {
      Empty();

      if( NULL != phFile )
      {
         hFile.ExportObject();
      }

      MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
   }


   if( NULL != phFile )
   {
      hFile.ExportObject();
   }


   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LoadFromFile( const MHandle& hFile ) throw( MAtomException )
{
   MASSERT( hFile.IsValid() );

   if( !hFile.IsValid() )
   {
      MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
   }

   LARGE_INTEGER ilFileSize;
   if( !GetFileSizeEx( hFile, &ilFileSize ) )
   {
      MAtomException::ExceptionFailedToOperate( ::GetLastError() );
   }

   LARGE_INTEGER ilFilePosition;
   if( !SetFilePointerEx( hFile, (ilFilePosition.QuadPart = 0, ilFilePosition), &ilFilePosition, FILE_CURRENT ) )
   {
      MAtomException::ExceptionFailedToOperate( ::GetLastError() );
   }

   if( ((unsigned __int64)ilFileSize.QuadPart - (unsigned __int64)ilFilePosition.QuadPart) < muSize.InBytes() )
   {
      MAtomException::TestCompatibleCall( 0 == ((ilFileSize.QuadPart - ilFilePosition.QuadPart) % GetUnitSize()) );

      const MUnit< tMemClass > muNewSize( (MUI)((ilFileSize.QuadPart - ilFilePosition.QuadPart) / GetUnitSize()) );

      if( CanReAllocate() )
      {
         ReAllocate( muNewSize );
      }
      else
      {
         LimitSizeTo( muNewSize );
      }
   }


   unsigned __int64 uiReadTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesRead( 0 );
      if( !ReadFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesRead, NULL ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }

      if( dwChunkSize != dwBytesRead )
      {
         MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
      }

      uiReadTotal += dwBytesRead;
   }


   if( muSize.InBytes() != uiReadTotal )
   {
      MAtomException::ExceptionFailedToOperate( ERROR_READ_FAULT );
   }


   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::Offset( typename const MUnit< tMemClass >& muOffset, const bool bPositiveDirection, const bool bConstantSize ) throw( MAtomException )
{
   // Offsetting the beginning of memory is available only for memory types that do not reallocate memory, e.g. ShellMemory.
   MAtomException::TestCompatibleCall( !CanReAllocate() );
   MAtomException::TestNonNullPointer( pData );

   pData = (tMemClass*)((size_t)pData + (bPositiveDirection ? 1 : -1) * muOffset.InBytes() );

   if( !bConstantSize )
   {
      if( bPositiveDirection )
      {
         MAtomException::TestInRangeZero2UpperLimit< true, true >( muOffset.GetUnits(), muSize.GetUnits() );

         muSize -= muOffset;
      }
      else
      {
         muSize += muOffset;
      }
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::LimitSizeTo( typename const MUnit< tMemClass >& mu_size ) throw( MAtomException )
{
   MAtomException::TestInRangeZero2UpperLimit< true, true >( mu_size.GetUnits(), this->muSize.GetUnits() );

   this->muSize = mu_size;

   return( *this );
}


template< class tMemClass > const MAtom< tMemClass >& MAtom< tMemClass >::AppendToFile( const TCHAR *ptcFilename ) const throw()
{
   MHandle hFile( CreateFile( ptcFilename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) );

   if( !hFile.IsValid() )
   {
      MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
   }

   LARGE_INTEGER li = {0};
   if( !SetFilePointerEx( hFile, (li.QuadPart = 0, li), NULL, FILE_END ) )
   {
      MAtomException::ExceptionFailedToOperate( ::GetLastError() );
   }


   unsigned __int64 uiWrittenTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesWritten( 0 );
      if( !WriteFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesWritten, NULL ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }

      if( dwChunkSize != dwBytesWritten )
      {
         MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
      }

      uiWrittenTotal += dwBytesWritten;
   }


   if( muSize.InBytes() != uiWrittenTotal )
   {
      MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
   }


   return( *this );
}


template< class tMemClass > const MAtom< tMemClass >& MAtom< tMemClass >::AppendToFile( const HANDLE hFile ) const throw()
{
   if( (INVALID_HANDLE_VALUE == hFile) || (NULL == hFile) )
   {
      MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
   }


   LARGE_INTEGER li = { 0 };
   if( !SetFilePointerEx( hFile, (li.QuadPart = 0, li), 0, FILE_END ) )
   {
      MAtomException::ExceptionFailedToOperate( ::GetLastError() );
   }


   unsigned __int64 uiWrittenTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesWritten( 0 );
      if( !WriteFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesWritten, NULL ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }

      if( dwChunkSize != dwBytesWritten )
      {
         MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
      }

      uiWrittenTotal += dwBytesWritten;
   }


   if( muSize.InBytes() != uiWrittenTotal )
   {
      MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
   }


   return( *this );
}


template< class tMemClass > const MAtom< tMemClass >& MAtom< tMemClass >::AddToFile( const HANDLE hFile ) const throw()
{
   if( (INVALID_HANDLE_VALUE == hFile) || (NULL == hFile) )
   {
      MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
   }


   unsigned __int64 uiWrittenTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesWritten( 0 );
      if( !WriteFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesWritten, NULL ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }

      if( dwChunkSize != dwBytesWritten )
      {
         MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
      }

      uiWrittenTotal += dwBytesWritten;
   }


   if( muSize.InBytes() != uiWrittenTotal )
   {
      MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
   }


   return( *this );
}


template< class tMemClass > const MAtom< tMemClass >& MAtom< tMemClass >::SaveAsFile( const HANDLE hFile ) const throw()
{
   if( (INVALID_HANDLE_VALUE == hFile) || (NULL == hFile) )
   {
      MAtomException::ExceptionInvaldCall( ERROR_INVALID_HANDLE );
   }


   LARGE_INTEGER li = { 0 };
   if( !SetFilePointerEx( hFile, (li.QuadPart = 0, li), NULL, FILE_BEGIN ) )
   {
      MAtomException::ExceptionFailedToOperate( ::GetLastError() );
   }


   unsigned __int64 uiWrittenTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesWritten( 0 );
      if( !WriteFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesWritten, NULL ) )
      {
         MAtomException::ExceptionFailedToOperate( ::GetLastError() );
      }

      if( dwChunkSize != dwBytesWritten )
      {
         MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
      }

      uiWrittenTotal += dwBytesWritten;
   }


   if( muSize.InBytes() != uiWrittenTotal )
   {
      MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
   }


   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::Replace( const tMemClass tReplaceValue, const tMemClass tNewValue ) throw()
{
   for( MUnit< tMemClass > muIndex( 0 ); muIndex < muSize; muIndex++ )
   {
      if( tReplaceValue == pData[muIndex.GetUnits()] )
      {
         pData[muIndex.GetUnits()] = tNewValue;
      }
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::TrimLeft( const tMemClass tTrimValue ) throw()
{
   for( MUnit< tMemClass > muIndex( 0 ); muIndex < muSize; muIndex++ )
   {
      if( tTrimValue != pData[muIndex.GetUnits()] )
      {
         muSize -= muIndex;

         memmove( pData, &pData[muIndex.GetUnits()], muSize.InBytes() );

         return( *this );
      }
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::TrimRight( const tMemClass tTrimValue ) throw()
{
   while( 0 != muSize )
   {
      if( tTrimValue != pData[muSize.GetUnits()-1] )
      {
         break;
      }

      muSize--;
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::Trim( const tMemClass tTrimValue ) throw()
{
   return( TrimRight( tTrimValue ).TrimLeft( tTrimValue ) );
}


template< class tMemClass > MUnit< tMemClass > MAtom< tMemClass >::Find( const tMemClass tSearchValue ) const throw()
{
   for( MUnit< tMemClass > muIndex( 0 ); muIndex < muSize; muIndex++ )
   {
      if( tSearchValue == pData[muIndex.GetUnits()] )
      {
         return( muIndex );
      }
   }

   return( muSize );
}


template< class tMemClass > const ShellMemory< tMemClass > MAtom< tMemClass >::Search_L2R( typename const MAtom< tMemClass >& memSearchFor, const bool bPartialMatchAtTheEnd ) const
{
   if( memSearchFor.IsEmpty() )
   {
      return( ShellMemory< tMemClass >( pData, muSize ) );
   }

   for( MUI muiIndex = 0; muiIndex < muSize; muiIndex++ )
   {
      MUI muiOuter( muiIndex );

      for( MUI muiInner = 0; muiInner < memSearchFor.muSize; muiOuter++, muiInner++ )
      {
         if( pData[muiOuter] == memSearchFor.pData[muiInner] )
         {
            if( muiInner == memSearchFor.muSize - 1 )
            {
               return( ShellMemory< tMemClass >( &pData[muiIndex], MUnit< tMemClass >( muiInner + 1 ) ) );
            }

            if( muiOuter + 1 >= muSize )
            {
               if( bPartialMatchAtTheEnd )
               {
                  return( ShellMemory< tMemClass >( &pData[muiIndex], MUnit< tMemClass >( muiInner + 1 ) ) );
               }

               return( ShellMemory< tMemClass >() );
            }
         }
         else
         {
            break;
         }
      }
   }

   return( ShellMemory< tMemClass >() );
}


template< class tMemClass > const ShellMemory< tMemClass > MAtom< tMemClass >::Search_L2R( typename const MAtom< tMemClass >& memSearchFor, const bool bPartialMatchAtTheEnd, const bool bCaseSensitive ) const
{
   if( memSearchFor.IsEmpty() )
   {
      return( ShellMemory< tMemClass >( pData, muSize ) );
   }

   for( MUI muiIndex = 0; muiIndex < muSize; muiIndex++ )
   {
      MUI muiOuter( muiIndex );

      for( MUI muiInner = 0; muiInner < memSearchFor.muSize; muiOuter++, muiInner++ )
      {
         if( pData[muiOuter] == memSearchFor.pData[muiInner] ? true : (bCaseSensitive ? false : toupper( pData[muiOuter] ) == toupper( memSearchFor.pData[muiInner] )) )
         {
            if( muiInner == memSearchFor.muSize - 1 )
            {
               return( ShellMemory< tMemClass >( &pData[muiIndex], MUnit< tMemClass >( muiInner + 1 ) ) );
            }

            if( muiOuter + 1 >= muSize )
            {
               if( bPartialMatchAtTheEnd )
               {
                  return( ShellMemory< tMemClass >( &pData[muiIndex], MUnit< tMemClass >( muiInner + 1 ) ) );
               }

               return( ShellMemory< tMemClass >() );
            }
         }
         else
         {
            break;
         }
      }
   }

   return( ShellMemory< tMemClass >() );
}


template< class tMemClass > bool MAtom< tMemClass >::FindInFile( const MHandle& hFile, const unsigned __int64 uiSearchFrom, unsigned __int64* puiFoundAt, const HANDLE heStop ) const
{
   ULARGE_INTEGER liFileSize;
   if( !::GetFileSizeEx( hFile, (LARGE_INTEGER*)&liFileSize ) )
   {
      MAtomException::ExceptionFailedToOperate( ::GetLastError() );
   }

   LARGE_INTEGER uiPosition;
   if( !SetFilePointerEx( hFile, (uiPosition.QuadPart = uiSearchFrom, uiPosition), &uiPosition, FILE_BEGIN ) )
   {
      MAtomException::ExceptionFailedToOperate( ::GetLastError() );
   }


   MASSERT( uiSearchFrom == (unsigned __int64)uiPosition.QuadPart );


   MemoryPH< BYTE > memData( MMIN< MUnit< BYTE >, MUI, MUI >( ((MUI)2)<<20, (MUI)(liFileSize.QuadPart - uiSearchFrom) ) );
   memData.LoadFromFile( hFile );

   ShellMemory< BYTE > memFinder( memData );

   while( !memData.IsEmpty() )
   {
      const ShellMemory< BYTE > memMatch( memData.Search_L2R( *this, true ) );

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

         memFinder = memData;
         
         continue;
      }

      if( memMatch.GetSize() == GetSize() )
      {
         // Matching.
         if( NULL != puiFoundAt )
         {
            *puiFoundAt = uiPosition.QuadPart + ((MUI)memMatch.GetMemory() - (MUI)memData.GetMemory());
         }

         return( true );
      }
      else
      {
         // Partial match - need to check if the memory behind what is loaded continues to match.
         // So ... Load the next chunk from the file.
         uiPosition.QuadPart += memData.GetSize().InBytes();
         memData.LoadFromFile( hFile );

         const MUnit< BYTE > muSize2Check( GetSize() - memMatch.GetSize() );

         if( memData.GetSize() < muSize2Check )
         {
            return( false );
         }

         if( memData.SubMemory( MUnit< BYTE >( 0 ), muSize2Check ) == SubMemory( memMatch.GetSize(), muSize2Check ) )
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
            memFinder = memData;
         }
      }
   }

   return( false );
}


template< class tMemClass > double MAtom< tMemClass >::GetHash1() const // throw( may throw an overflow exception in future )
{
   double dCompressed( 0 );

   const MUI muiSizeInBytes( GetSize().InBytes() );

   for( MUI muiIndex = 0; muiIndex < muiSizeInBytes; muiIndex++ )
   {
      dCompressed += double( muiIndex + ((BYTE*)pData)[muiIndex] ) / double( 256 + muiIndex );
   }

   return( dCompressed );
}


template< class tMemClass > unsigned __int64 MAtom< tMemClass >::MHash() const
{
   const MUI muiSizeData( GetSize().InBytes() );


   // 256 - ignore the ASCII, so there is a bijection.
   if( 4000001 - 256 < muiSizeData )
   {
      MASSERT( FALSE );

      return( 0 );
   }
   

   unsigned __int64 uiHash( 0 );
   BYTE* pThisData( (BYTE*)pData );


   for( MUI uiByte = 0; uiByte < muiSizeData; uiByte++ )
   {
      uiHash += primes4000001[pThisData[uiByte]] * primes4000001[256 + uiByte];
   }

   return( uiHash );
}


template< class tMemClass > bool MAtom< tMemClass >::SaveAsFile( const TCHAR *ptcFilename, const DWORD dwFlagsAndAttributes, DWORD* pdwLastError ) const throw()
{
   return( SaveAsFile( ptcFilename, CREATE_ALWAYS, dwFlagsAndAttributes, pdwLastError ) );
}


template< class tMemClass > bool MAtom< tMemClass >::SaveAsFile( const TCHAR *ptcFilename, const DWORD dwCreationDisposition, const DWORD dwFlagsAndAttributes, DWORD* pdwLastError ) const throw()
{
   MHandle hFile( CreateFile( ptcFilename, GENERIC_WRITE, 0, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL ) );


   if( !hFile.IsValid() )
   {
      const DWORD dwLastError( ::GetLastError() );

      if( NULL != pdwLastError )
      {
         *pdwLastError = dwLastError;

         return( false );
      }

      MAtomException::ExceptionFailedToOperate( dwLastError );
   }


   unsigned __int64 uiWrittenTotal( 0 );

   BYTE* ptrDataCopy( (BYTE*)pData );
   const BYTE* ptrLastData( (BYTE*)((size_t)pData + muSize.InBytes()) );

   for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
   {
      DWORD dwBytesWritten( 0 );
      if( !WriteFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesWritten, NULL ) )
      {
         const DWORD dwLastError( ::GetLastError() );

         if( NULL != pdwLastError )
         {
            *pdwLastError = dwLastError;

            return( false );
         }

         MAtomException::ExceptionFailedToOperate( dwLastError );
      }

      if( dwChunkSize != dwBytesWritten )
      {
         if( NULL != pdwLastError )
         {
            *pdwLastError = ERROR_WRITE_FAULT;

            return( false );
         }

         MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
      }

      uiWrittenTotal += dwBytesWritten;
   }


   if( muSize.InBytes() != uiWrittenTotal )
   {
      if( NULL != pdwLastError )
      {
         *pdwLastError = ERROR_WRITE_FAULT;

         return( false );
      }

      MAtomException::ExceptionFailedToOperate( ERROR_WRITE_FAULT );
   }


   if( NULL != pdwLastError )
   {
      *pdwLastError = ERROR_SUCCESS;
   }


   return( true );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::FillNoise( const bool bSeed, const unsigned int uiSeed ) throw()
{
   if( bSeed )
   {
      srand( uiSeed );
   }

   for( MUI muiIndex = 0; muiIndex < muSize.InBytes(); muiIndex++ )
   {
      ((BYTE*)pData)[muiIndex] = (BYTE)rand();
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::FillNoise( const unsigned int uiSeed ) throw()
{
   srand( uiSeed );

   for( MUI muiIndex = 0; muiIndex < muSize.InBytes(); muiIndex++ )
   {
      ((BYTE*)pData)[muiIndex] = (BYTE)rand();
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::FillNoise() throw()
{
   for( MUI muiIndex = 0; muiIndex < muSize.InBytes(); muiIndex++ )
   {
      ((BYTE*)pData)[muiIndex] = (BYTE)rand();
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::XOR( const MAtom< tMemClass >& maXORMemory ) throw()
{
   MAtomException::TestCompatibleCall( maXORMemory.GetSize() == muSize );

   for( MUI muiIndex = 0; muiIndex < muSize.GetUnits(); muiIndex++ )
   {
      pData[muiIndex] = pData[muiIndex] ^ maXORMemory.pData[muiIndex];
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::XOR( const tMemClass& mtXORUnit ) throw()
{
   for( MUI muiIndex = 0; muiIndex < muSize.GetUnits(); muiIndex++ )
   {
      pData[muiIndex] = pData[muiIndex] ^ mtXORUnit;
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::Set( typename const tMemClass& objPattern ) throw()
{
   for( MUI muiIndex = 0; muiIndex < muSize.GetUnits(); muiIndex++ )
   {
      pData[muiIndex] = objPattern;
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::Reset( typename const BYTE b1Wipe ) throw()
{
   BYTE *pByteData = (BYTE*)pData;
   const MUI muiSize( muSize.InBytes() );

   for( MUI muiIndex = 0; muiIndex < muiSize; muiIndex++ )
   {
      pByteData[muiIndex] = b1Wipe;
   }

   return( *this );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::Reverse() throw()
{
   const MUI muiElements( GetSize().GetUnits() );

   for( MUI muiIndex = 0; muiIndex < muiElements / 2; muiIndex++ )
   {
      tMemClass dtElement = pData[muiIndex];

      pData[muiIndex] = pData[muiElements-muiIndex-1];

      pData[muiElements-muiIndex-1] = dtElement;
   }

   return( *this );
}


template< class tMemClass > MemoryPH< char > MAtom< tMemClass >::Bin2Ascii() const
{
   MemoryPH< char > maDestination( MUnit< char >( 2 * GetSize().InBytes() ) );
   BYTE *pDestination = (BYTE*)maDestination.GetMemory();

   MUI muiSize( GetSize().InBytes() );

   for( DWORD dwIndex = 0; dwIndex < muiSize; dwIndex++ )
   {
      const BYTE b1Byte( ((BYTE*)pData)[dwIndex] );

      const BYTE b1MSHB( b1Byte >> 0x04 );
      const BYTE b1LSHB( b1Byte &  0x0F );

      pDestination[ 2 * dwIndex     ] = (BYTE)(0x30 + (b1MSHB < 0x0A ? b1MSHB : b1MSHB + 0x07));
      pDestination[ 2 * dwIndex + 1 ] = (BYTE)(0x30 + (b1LSHB < 0x0A ? b1LSHB : b1LSHB + 0x07));
   }

   return( maDestination );
}


template< class tMemClass > MAtom< tMemClass >& MAtom< tMemClass >::Ascii2Bin()
{
   const MUI muiSizeInBytes( GetSize().InBytes() );

   MAtomException::TestCompatibleCall( 0 == (muiSizeInBytes & 1) );

   for( MUI muiIndex = 0; muiIndex < muiSizeInBytes; muiIndex += 2 )
   {
      char sb1MSB( pData[muiIndex    ] - 0x30 );
      char sb1LSB( pData[muiIndex + 1] - 0x30 );

      if( 0x09 < sb1MSB ) { sb1MSB -= 0x07; }
      if( 0x09 < sb1LSB ) { sb1LSB -= 0x07; }

      if( ((sb1MSB < 0) || (sb1MSB > 0x0F )) || ((sb1LSB < 0) || (sb1LSB > 0x0F )) )
      {
         MAtomException::ExceptionInvalidData();
      }

      if( !bool( IsInRange< int, true, int, true, int >( 0, sb1MSB, 0x0F ) ) )
      {
         MAtomException::ExceptionInvalidData();
      }

      if( !bool( IsInRange< int, true, int, true, int >( 0, sb1LSB, 0x0F ) ) )
      {
         MAtomException::ExceptionInvalidData();
      }

      pData[muiIndex / 2] = (char)((sb1MSB << 4) | sb1LSB);
   }

   LimitSizeTo( GetSize() / 2 );

   return( *this );
}


#pragma warning( pop )

