//
//
// MMemory.h: memory atoms definitions as example of implementation of:
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


#include "CommonAMM.h"
#include "MException.h"
#include "MList.h"


#ifdef _WIN32
// MMemory is the original first phase of the Atomic Memory Model. This class is no longer developed however
// it may still contain certain functionality required which MAtom, the currently developped technology does 
// not have, e.g. the Segmented-Memory class.


#include "typeinfo.h"
#include "Wininet.h"


// Requires to link your project with Wininet.lib. Search for INTERNET_CONNECTIVITY in this file for more details.
//#define INTERNET_CONNECTIVITY


template< class tMemType >
class MMemory
{
public:
   class MemUnits
   {
      size_t dwUnits;

   public:
      MemUnits() : dwUnits( 0 ) {}
      MemUnits( const size_t dwNumberOfUnits ) : dwUnits( dwNumberOfUnits ) {}
      MemUnits( const MemUnits&  objMemUnits ) : dwUnits( objMemUnits.dwUnits ) {}
      virtual ~MemUnits() {}

      MemUnits& operator=( const MemUnits &objMemUnits ) { dwUnits = objMemUnits.dwUnits; return( *this ); }

      size_t GetUnits() const throw()
      {
         return( dwUnits );
      }

      template< bool bExceptions > DWORD GetUnits32() const
      {
         MUnitException::TestTargetSize32( dwUnits );

         return( (DWORD)dwUnits );
      }

      size_t InBytes()  const throw()
      {
         return( dwUnits * sizeof( tMemType ) );
      }

      template< bool bExceptions > DWORD InBytes32() const
      {
         MUnitException::TestTargetSize32( dwUnits * sizeof( tMemType ) );

         return( (DWORD)(dwUnits * sizeof( tMemType )) );
      }


      template< typename tMemType > bool     IsTransformable() const throw() { return(  0 == (dwUnits % sizeof( tMemType )) ); }
      template< typename tMemType > MemUnits AppearAs()        const throw() { MASSERT( 0 == (dwUnits % sizeof( tMemType )) ); return( dwUnits / sizeof( tMemType ) ); }

      MemUnits& operator+=( const MemUnits& memUnits   )       throw() { dwUnits += memUnits.dwUnits; return( *this ); }
      MemUnits& operator+=( const size_t    dwMemUnits )       throw() { dwUnits += dwMemUnits; return( *this ); }
      MemUnits  operator++()                                   throw() { dwUnits++; return( *this ); }
      MemUnits  operator++( int )                              throw() { const MemUnits muPostFix( dwUnits ); dwUnits++; return( muPostFix ); }
      MemUnits  operator--()                                   throw() { dwUnits--; MASSERT( (size_t)-1 != dwUnits ); return( *this ); }
      MemUnits  operator--( int )                              throw() { const MemUnits muPostFix( dwUnits ); dwUnits--; MASSERT( (size_t)-1 != dwUnits ); return( muPostFix ); }

      MemUnits  operator-(  const MemUnits& memUnits   ) const throw() { MASSERT( dwUnits >= memUnits.dwUnits ); return( MemUnits( (size_t)(dwUnits - memUnits.dwUnits) ) ); }
      MemUnits  operator/(  const size_t    dwDivisor  ) const throw() { return( MemUnits( (size_t)( dwUnits / dwDivisor ) ) ); }


      friend MemUnits operator +( const size_t dwAdder,     const MemUnits& muAdder  ) throw() { return( MemUnits( dwAdder + muAdder.dwUnits ) ); }
      friend MemUnits operator +( const MemUnits& muAdder,  const size_t dwAdder     ) throw() { return( MemUnits( muAdder.dwUnits + dwAdder ) ); }
      friend MemUnits operator +( const MemUnits& muAdderA, const MemUnits& muAdderB ) throw() { return( MemUnits( muAdderA.dwUnits + muAdderB.dwUnits ) ); }


      friend MemUnits operator *( const size_t dwMultiplier,     const MemUnits& muMultiplier  ) throw() { return( MemUnits( dwMultiplier * muMultiplier.dwUnits ) ); }
      friend MemUnits operator *( const MemUnits& muMultiplier,  const size_t    dwMultiplier  ) throw() { return( MemUnits( dwMultiplier * muMultiplier.dwUnits ) ); }
      friend MemUnits operator *( const MemUnits& muMultiplierA, const MemUnits& muMultiplierB ) throw() { return( MemUnits( muMultiplierA.dwUnits * muMultiplierB.dwUnits ) ); }


      friend bool operator <(  const size_t dwUnits,     const MemUnits& muUnits  ) throw() { return( dwUnits          <  muUnits.dwUnits  ); }
      friend bool operator <(  const MemUnits& muUnits,  const size_t dwUnits     ) throw() { return( muUnits.dwUnits  <  dwUnits          ); }
      friend bool operator <(  const MemUnits& muUnitsL, const MemUnits& muUnitsR ) throw() { return( muUnitsL.dwUnits <  muUnitsR.dwUnits ); }

      friend bool operator >(  const size_t dwUnits,     const MemUnits& muUnits  ) throw() { return( dwUnits          >  muUnits.dwUnits  ); }
      friend bool operator >(  const MemUnits& muUnits,  const size_t dwUnits     ) throw() { return( muUnits.dwUnits  >  dwUnits          ); }
      friend bool operator >(  const MemUnits& muUnitsL, const MemUnits& muUnitsR ) throw() { return( muUnitsL.dwUnits >  muUnitsR.dwUnits ); }

      friend bool operator <=( const size_t dwUnits,     const MemUnits& muUnits  ) throw() { return( dwUnits          <= muUnits.dwUnits  ); }
      friend bool operator <=( const MemUnits& muUnits,  const size_t dwUnits     ) throw() { return( muUnits.dwUnits  <= dwUnits          ); }
      friend bool operator <=( const MemUnits& muUnitsL, const MemUnits& muUnitsR ) throw() { return( muUnitsL.dwUnits <= muUnitsR.dwUnits ); }

      friend bool operator >=( const size_t dwUnits,     const MemUnits& muUnits  ) throw() { return( dwUnits          >= muUnits.dwUnits  ); }
      friend bool operator >=( const MemUnits& muUnits,  const size_t dwUnits     ) throw() { return( muUnits.dwUnits  >= dwUnits          ); }
      friend bool operator >=( const MemUnits& muUnitsL, const MemUnits& muUnitsR ) throw() { return( muUnitsL.dwUnits >= muUnitsR.muUnits ); }

      friend bool operator ==( const size_t dwUnits,     const MemUnits& muUnits  ) throw() { return( dwUnits          == muUnits.dwUnits  ); }
      friend bool operator ==( const MemUnits& muUnits,  const size_t dwUnits     ) throw() { return( muUnits.dwUnits  == dwUnits          ); }
      friend bool operator ==( const MemUnits& muUnitsL, const MemUnits& muUnitsR ) throw() { return( muUnitsL.dwUnits == muUnitsR.dwUnits ); }

      friend bool operator !=( const size_t dwUnits,     const MemUnits& muUnits  ) throw() { return( dwUnits          != muUnits.dwUnits  ); }
      friend bool operator !=( const MemUnits& muUnits,  const size_t dwUnits     ) throw() { return( muUnits.dwUnits  != dwUnits          ); }
      friend bool operator !=( const MemUnits& muUnitsL, const MemUnits& muUnitsR ) throw() { return( muUnitsL.dwUnits != muUnitsR.dwUnits ); }
   };


protected:
   tMemType *pData;
   size_t   stSize;

public:
   MMemory() : pData( NULL ), stSize( 0 ) {}
   MMemory( tMemType *pDataSource, size_t stMemory ) : pData( pDataSource ), stSize( stMemory ) { MASSERT( pDataSource != NULL ); }
   MMemory( tMemType *pDataSource, const MemUnits& memUnits ) : pData( pDataSource ), stSize( memUnits.InBytes() ) { MASSERT( pDataSource != NULL ); }
   MMemory( const MMemory< tMemType >& memSource ) : pData( memSource.pData ), stSize( memSource.stSize ) {}

   virtual ~MMemory() {};

   virtual  MMemory< tMemType >& operator=( const MMemory< tMemType >& memSource ) = 0;


   const tMemType*   GetMemory()    const { return( pData  ); }
         tMemType*   GetMemory()          { return( pData  ); }
   const tMemType**  GetMemoryPtr() const { return( &pData ); }
         tMemType**  GetMemoryPtr()       { return( &pData ); }

   virtual bool      IsEmpty()          const throw() { MASSERT( NULL != pData ); return( 0 == stSize ); }
           size_t    GetSizeBytes()     const throw() { return( stSize ); }

           template< bool bExceptions > DWORD GetSizeBytes32() const
           {
              MUnitException::TestTargetSize32( stSize );

              return( (DWORD)stSize );
           }


           size_t    GetSizeUnits()     const throw() { return( stSize / sizeof( tMemType ) ); }
           DWORD     GetSizeUnit()      const throw() { return( sizeof( tMemType ) ); }
           MemUnits  GetSize()          const throw() { MASSERT( 0 == (stSize % sizeof( tMemType )) ); return( (size_t)(stSize / sizeof( tMemType )) ); }
   
   template< typename tmCast > size_t     GetSizeUnitsCast() const throw() { return( stSize / sizeof( tmCast ) ); }


   template< class tVarType > tVarType  SetMember( const size_t stOffset2Var, const tVarType& tValue ) throw()
   {
      for( size_t dwItem = 0; dwItem < GetSizeUnits(); dwItem++ )
      {
         *(tVarType*)(((size_t)&(pData[dwItem])) + stOffset2Var) = tValue;
      }
      
      return( tValue );
   }

   virtual bool operator==( const MMemory< tMemType >& mtMemory ) const throw()
   {
      if( stSize == mtMemory.stSize )
      {
         return( !memcmp( pData, mtMemory.pData, stSize ) );
      }
      
      return( false );
   }
   
   virtual bool operator!=( const MMemory< tMemType >& mtMemory ) const throw()
   {
      return( !operator==( mtMemory ) );
   }


   virtual MMemory< tMemType >* Clone()   const throw() = 0;
   virtual MMemory< tMemType >& Release()       throw() { ReAllocate( size_t( 0 ) ); return( *this ); }

   virtual bool      CanReAllocate() const throw()          = 0;
   virtual tMemType* ReAllocate( const size_t stMemAmount ) = 0;
   virtual tMemType* ReAllocate( const typename MMemory< tMemType >::MemUnits &memUnits   ) = 0;
   virtual tMemType* ReAllocateTransfer( const tMemType *pSrc, const typename MMemory< tMemType >::MemUnits &memUnits ) = 0;

   operator tMemType& ()               throw() { return( *pData ); }
   operator const tMemType& ()   const throw() { return( *pData ); }
   operator tMemType* ()               throw() { return(  pData ); }
   operator const tMemType* ()   const throw() { return(  pData ); }

#ifdef WIN32
         tMemType& operator[]( const int iIndex )              throw() { MASSERT( (0 <= iIndex) && ((DWORD)iIndex < GetSizeUnits()) ); return( pData[iIndex] ); }
   const tMemType& operator[]( const int iIndex )        const throw() { MASSERT( (0 <= iIndex) && ((DWORD)iIndex < GetSizeUnits()) ); return( pData[iIndex] ); }

         tMemType& operator[]( const UINT uiIndex )            throw() { MASSERT( uiIndex < GetSizeUnits() ); return( pData[uiIndex] ); }
   const tMemType& operator[]( const UINT uiIndex )      const throw() { MASSERT( uiIndex < GetSizeUnits() ); return( pData[uiIndex] ); }

         tMemType& operator[]( const DWORD dwIndex )           throw() { MASSERT( dwIndex < GetSizeUnits() ); return( pData[dwIndex] ); }
   const tMemType& operator[]( const DWORD dwIndex )     const throw() { MASSERT( dwIndex < GetSizeUnits() ); return( pData[dwIndex] ); }
#else
         tMemType& operator[]( const size_t dwIndex )           throw() { MASSERT( dwIndex < GetSizeUnits() ); return( pData[dwIndex] ); }
   const tMemType& operator[]( const size_t dwIndex )     const throw() { MASSERT( dwIndex < GetSizeUnits() ); return( pData[dwIndex] ); }
#endif

         tMemType& operator[]( const MemUnits& muIndex )       throw() { MASSERT( muIndex < GetSizeUnits() ); return( pData[muIndex.GetUnits()] ); }
   const tMemType& operator[]( const MemUnits& muIndex ) const throw() { MASSERT( muIndex < GetSizeUnits() ); return( pData[muIndex.GetUnits()] ); }


   template< typename tmCast >                                 tmCast& GetItemCast( const DWORD dwIndex )       throw() { MASSERT( dwIndex < GetSizeUnitsCast< tmCast >() ); return( ((tmCast*)pData)[dwIndex] ); }
   template< typename tmCast >                           const tmCast& GetItemCast( const DWORD dwIndex ) const throw() { MASSERT( dwIndex < GetSizeUnitsCast< tmCast >() ); return( ((tmCast*)pData)[dwIndex] ); }

   template< typename tmPackType, typename tmCast >            tmCast& GetItemOffsetCast( const DWORD dwPackNumber, const DWORD dwIndex )       throw() { MASSERT( (dwPackNumber * sizeof( tmPackType ) + (dwIndex + 1) * sizeof( tmCast )) <= GetSizeBytes() ); return( ((tmCast*)((size_t)pData + dwPackNumber * sizeof( tmPackType )))[dwIndex] ); }
   template< typename tmPackType, typename tmCast >      const tmCast& GetItemOffsetCast( const DWORD dwPackNumber, const DWORD dwIndex ) const throw() { MASSERT( (dwPackNumber * sizeof( tmPackType ) + (dwIndex + 1) * sizeof( tmCast )) <= GetSizeBytes() ); return( ((tmCast*)((size_t)pData + dwPackNumber * sizeof( tmPackType )))[dwIndex] ); }

   template< const size_t stPackSize, typename tmCast >        tmCast& GetItemOffsetCast( const DWORD dwPackNumber, const size_t stInPackOffset )       throw() { MASSERT( (dwPackNumber * stPackSize + stInPackOffset + sizeof( tmCast )) <= GetSizeBytes() ); return( *((tmCast*)((size_t)pData + dwPackNumber * stPackSize + stInPackOffset)) ); }
   template< const size_t stPackSize, typename tmCast >  const tmCast& GetItemOffsetCast( const DWORD dwPackNumber, const size_t stInPackOffset ) const throw() { MASSERT( (dwPackNumber * stPackSize + stInPackOffset + sizeof( tmCast )) <= GetSizeBytes() ); return( *((tmCast*)((size_t)pData + dwPackNumber * stPackSize + stInPackOffset)) ); }


   template< typename tmCast >       tmCast& AppearAs()        throw() { return( *(tmCast*)this ); }
   template< typename tmCast > const tmCast& AppearAs()  const throw() { return( *(tmCast*)this ); }


         tMemType* operator->()        throw() { return(  pData ); }
   const tMemType* operator->()  const throw() { return(  pData ); }

         tMemType* GetItemPtr( const DWORD dwItem )       throw() { return( pData[dwItem] ); }
   const tMemType* GetItemPtr( const DWORD dwItem ) const throw() { return( pData[dwItem] ); }

   MMemory< tMemType >& Invert() throw()
   {
      const MMemory< tMemType >::MemUnits muItems( GetSize() );

      for( MMemory< tMemType >::MemUnits muItem = 0; muItem < muItems; muItem++ )
      {
         pData[muItem.GetUnits()] = ~pData[muItem.GetUnits()];
      }

      return( *this );
   }

   MMemory< tMemType >& Replace( typename const tMemType& twOld, typename const tMemType& twNew, typename const MMemory< tMemType >::MemUnits& muStartFrom, typename const MMemory< tMemType >::MemUnits& muItems ) throw()
   {
      if( GetSize() > muStartFrom )
      {
         const MMemory< tMemType >::MemUnits muLastItem( muStartFrom + MMIN< MMemory< tMemType >::MemUnits, MMemory< tMemType >::MemUnits, MMemory< tMemType >::MemUnits >( GetSize() - muStartFrom, muItems ) );

         for( MMemory< tMemType >::MemUnits muItem = muStartFrom; muItem < muLastItem; muItem++ )
         {
            if( twOld == pData[muItem.GetUnits()] )
            {
               pData[muItem.GetUnits()] = twNew;
            }
         }
      }

      return( *this );
   }

   MMemory< tMemType >& operator^=( const MMemory< tMemType >& memRight ) throw()
   {
      // Not tested - test on first use.
      MASSERT( FALSE );

      const MMemory< tMemType >::MemUnits muItems( MMIN< MemUnits, MemUnits, MemUnits >( GetSize(), memRight.stSize ) );

      for( MemUnits muItem = 0; muItem < muItems; muItem++ )
      {
         pData[muItems.GetUnits() - muItem.GetUnits() - 1] ^= memRight.pData[muItems.GetUnits() - muItem.GetUnits() - 1];
      }

      return( *this );
   }

   MMemory< tMemType >& ShiftLeft( const MemUnits& muUnits ) throw()
   {
      // Works with zero too, but in memmove transfers the whole chunk over itself.
      if( 0 != muUnits.GetUnits() )
      {
         const tMemType* pSource( (tMemType*)((size_t)pData + muUnits.InBytes()) );
         const size_t    stShift( stSize < muUnits.InBytes() ? 0 : stSize - muUnits.InBytes() );

         memmove( pData, pSource, stShift );
         memset( (void*)((size_t)pData + stShift), 0, stSize - stShift );
      }

      return( *this );
   }

   //virtual MMemory< tMemType >& Replace( const MMemory< tMemType >& memReplacer )
   //{
   //   // Works with memory units as oppose to bytes.
   //   if( GetSize() >= memReplacer.GetSize() )
   //   {
   //      const MMemory< tMemType >::MemUnits muSearchRange( GetSize() - memReplacer.GetSize() + 1 );

   //      for( MMemory< tMemType >::MemUnits muTarget = 0; muTarget < muSearchRange; muTarget++ )
   //      {
   //         if( (*this)[muTarget] != memReplacer[0] )
   //         {
   //            continue;
   //         }

   //         for( MMemory< tMemType >::MemUnits muCompare = 0; muCompare < memReplacer.GetSize(); muCompare++ )
   //         {
   //            if( (*this)[muTarget + muCompare] != memReplacer[muCompare] )
   //            {
   //               break;
   //            }
   //         }

   //         ... if( muCompare == 
   //            ... 
   //            to be complete
   //      }
   //   }

   //   return( *this );
   //}

   virtual MMemory< tMemType >& PostFix( typename const MMemory< tMemType >& memToPostFix, const DWORD dwAppendTimes )
   {
      MASSERT( CanReAllocate() );

      const MMemory< tMemType >* pmemClone( Clone() );

      ReAllocate( stSize + dwAppendTimes * memToPostFix.stSize );

      memcpy( (void*)pData, pmemClone->GetMemory(), pmemClone->stSize );

      for( DWORD dwAppend = 0; dwAppend < dwAppendTimes; dwAppend++ )
      {
         memcpy( (void*)((size_t)pData + pmemClone->stSize + dwAppend * memToPostFix.stSize), memToPostFix.GetMemory(), memToPostFix.stSize );
      }

      delete pmemClone;

      return( *this );
   }

   virtual MMemory< tMemType >& CutSize( typename const MMemory< tMemType >::MemUnits muNewSize ) throw()
   {
      // DO NOT make this finction to free memory and allocate new smaller chunk, there is
      // ReAllocate function for this case. Consider for example use of CutSize for MShellMemory.

      MASSERT( muNewSize.InBytes() <= stSize );

      stSize = muNewSize.InBytes();

      return( *this );
   }

   virtual MMemory< tMemType >& DecrementSize( typename const MMemory< tMemType >::MemUnits muDecrementUnits ) throw()
   {
      // DO NOT make this finction to free memory and allocate new smaller chunk, there is
      // ReAllocate function for this case. Consider for example use of CutSize for MShellMemory.

      MASSERT( muDecrementUnits.InBytes() <= stSize );

      stSize -= muDecrementUnits.InBytes();

      return( *this );
   }

   MMemory< tMemType >& MemSet( int iValue ) throw()
   {
      memset( GetMemory(), iValue, GetSizeBytes() );
      
      return( *this );
   }

   MMemory< tMemType >& MemSet(  const tMemType& tTemplate ) throw()
   {
      for( MemUnits muIndex = 0; muIndex < GetSize(); muIndex++ )
      {
         pData[muIndex.GetUnits()] = tTemplate;
      }

      return( *this );
   }

   MMemory< tMemType >& FillNoise( const bool bSeed, const unsigned int uiSeed = 0 ) throw()
   {
      if( bSeed ) { srand( uiSeed ); }

      for( size_t stIndex = 0; stIndex < GetSizeBytes(); stIndex++ )
      {
         ((BYTE*)pData)[stIndex] = (BYTE)rand();
      }

      return( *this );
   }

   MMemory< tMemType >& Reverse() throw()
   {
      const MUI muiElements( GetSizeUnits() );

      for( MUI muiIndex = 0; muiIndex < muiElements / 2; muiIndex++ )
      {
         tMemType dtElement = pData[muiIndex];

         pData[muiIndex] = pData[muiElements-muiIndex-1];

         pData[muiElements-muiIndex-1] = dtElement;
      }

      return( *this );
   }

   // NOTE for next version of MMemory:
   // This prototype is bad - the least iStartPos should be in memory units and the type of memSearch should
   // be the type of this or the function is template and the type is passed as template parameter.
   int Find( const int iStartPos, const MMemory< BYTE >& memSearch ) const throw()
   {
      MASSERT( 0 <= iStartPos );
      MASSERT( 0 != memSearch.GetSizeBytes() );


      const int iLastPos( GetSizeBytes() - memSearch.GetSizeBytes() + 1 );

      switch( memSearch.GetSizeBytes() )
      {
      case 1:
         {
            const BYTE b1Byte( memSearch[0] );

            for( int iPos = iStartPos; iPos < iLastPos; iPos++ )
            {
               if( *(BYTE*)((size_t)pData + iPos) == b1Byte )
               {
                  return( iPos );
               }
            }
         }
      break;

      case 2:
      case 3:
         {
            const short sShort( memSearch.GetItemCast< short >( 0 ) );

            for( int iPos = iStartPos; iPos < iLastPos; iPos++ )
            {
               if( *(short*)((size_t)pData + iPos) == sShort )
               {
                  if( 0 == memcmp( (void*)((size_t)pData + iPos), memSearch.GetMemory(), memSearch.GetSizeBytes() ) )
                  {
                     return( iPos );
                  }
               }
            }
         }
         break;

      default:
         {
            const DWORD dwDword( memSearch.GetItemCast< DWORD >( 0 ) );

            for( int iPos = iStartPos; iPos < iLastPos; iPos++ )
            {
               if( *(DWORD*)((size_t)pData + iPos) == dwDword )
               {
                  if( 0 == memcmp( (void*)((size_t)pData + iPos), memSearch.GetMemory(), memSearch.GetSizeBytes() ) )
                  {
                     return( iPos );
                  }
               }
            }
         }
         break;
      }

      return( -1 );
   }

   MMemory< tMemType >& Transfer( const MemUnits &muOffsetTarget, const MMemory< tMemType >& memSource, const MemUnits &muOffsetSource, const MemUnits muSize2Transfer ) throw()
   {
      if( 0 != muSize2Transfer.GetUnits() )
      {
         MASSERT( (muOffsetTarget + muSize2Transfer) <= GetSize() );
         MASSERT( (muOffsetSource + muSize2Transfer) <= memSource.GetSize() );

         memcpy( (tMemType*)((size_t)pData + muOffsetTarget.InBytes()), &(memSource[muOffsetSource]), muSize2Transfer.InBytes() );
      }

      return( *this );
   }

   MMemory< tMemType >& Transfer( MemUnits &muOffset, const tMemType *ptSource, const MemUnits muSize2Transfer, const bool bUpdateOffset ) throw()
   {
      if( 0 != muSize2Transfer.GetUnits() )
      {
         MASSERT( (muOffset.GetUnits() + muSize2Transfer.GetUnits()) <= GetSize().GetUnits() );

         memcpy( (tMemType*)((size_t)pData + muOffset.InBytes()), ptSource, muSize2Transfer.InBytes() );

         if( bUpdateOffset )
         {
            muOffset += muSize2Transfer;
         }
      }

      return( *this );
   }

   MMemory< tMemType >& Transfer( const MemUnits &muOffset, const tMemType *ptSource, const MemUnits muSize2Transfer ) throw()
   {
      if( 0 != muSize2Transfer.GetUnits() )
      {
         MASSERT( (muOffset + muSize2Transfer) <= GetSize() );

         memcpy( (tMemType*)((size_t)pData + muOffset.InBytes()), ptSource, muSize2Transfer.InBytes() );
      }

      return( *this );
   }

   // void *operator new( size_t stObjectSize )     { return( HeapAlloc( GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, stObjectSize ) ); }
   // void operator delete( void *pReleasedObject ) { HeapFree( GetProcessHeap(), 0, pReleasedObject ); }

   virtual bool AddToFile( const HANDLE hFile )  const throw()
   {
      if( (INVALID_HANDLE_VALUE != hFile) && (NULL != hFile) )
      {
         SetFilePointer( hFile, 0, 0, FILE_END );

         DWORD dwBytesWritten( 0 );

         if( WriteFile( hFile, pData, (DWORD)stSize, &dwBytesWritten, NULL ) )
         {
            return( stSize == dwBytesWritten );
         }
      }

      return( false );
   }

   virtual bool AddToFile( const TCHAR *ptcFilename )  const throw()
   {
      MHandle hFile( CreateFile( ptcFilename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) );

      if( !hFile.IsValid() )
      {
         return( false );
      }

      LARGE_INTEGER li = {0};
      if( !SetFilePointerEx( hFile, (li.QuadPart = 0, li), NULL, FILE_END ) )
      {
         return( false );
      }


      unsigned __int64 uiWrittenTotal( 0 );

      BYTE* ptrDataCopy( (BYTE*)pData );
      const BYTE* ptrLastData( (BYTE*)((size_t)pData + stSize) );

      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
      {
         DWORD dwBytesWritten( 0 );
         if( !WriteFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesWritten, NULL ) )
         {
            return( false );
         }

         if( dwChunkSize != dwBytesWritten )
         {
            return( false );
         }

         uiWrittenTotal += dwBytesWritten;
      }


      return( stSize == uiWrittenTotal );
   }


   virtual bool SaveAsFile( const TCHAR *ptcFilename, const DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL )  const throw()
   {
      MHandle hFile( CreateFile( ptcFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, dwFlagsAndAttributes, NULL ) );

      if( !hFile.IsValid() )
      {
         return( false );
      }


      unsigned __int64 uiWrittenTotal( 0 );

      BYTE* ptrDataCopy( (BYTE*)pData );
      const BYTE* ptrLastData( (BYTE*)((size_t)pData + stSize) );

      for( DWORD dwChunkSize( MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) ); ptrDataCopy < ptrLastData; ptrDataCopy += dwChunkSize, dwChunkSize = MMIN< DWORD, MUI, DWORD >( ptrLastData - ptrDataCopy, 1U << 30 ) )
      {
         DWORD dwBytesWritten( 0 );
         if( !WriteFile( hFile, ptrDataCopy, dwChunkSize, &dwBytesWritten, NULL ) )
         {
            return( false );
         }

         if( dwChunkSize != dwBytesWritten )
         {
            return( false );
         }

         uiWrittenTotal += dwBytesWritten;
      }


      return( stSize == uiWrittenTotal );
   }

// To use this function you need to uncomment the #define INTERNET_CONNECTIVITY above and link your project with wininet.lib.
#if defined( INTERNET_CONNECTIVITY )
   virtual bool SaveAsFileFTP( LPCTSTR strServer, LPCTSTR strAgent, LPCTSTR strFolder, LPCTSTR strFilename, LPCTSTR strUser, LPCTSTR strPassword ) const throw()
   {
      bool bSuccess( false );

      // Open Internet Handle.
      HINTERNET hiInternet = InternetOpen( strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

      if( NULL != hiInternet )
      {
         // Open Session Handle.
         HINTERNET hiSession = InternetConnect( hiInternet, strServer, INTERNET_DEFAULT_FTP_PORT, strUser, strPassword, INTERNET_SERVICE_FTP, 0, 0 );

         if( NULL != hiSession )
         {
            if( FtpSetCurrentDirectory( hiSession, strFolder ) )
            {
               HINTERNET hiFile = FtpOpenFile( hiSession, strFilename, GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0 );

               if( NULL != hiFile )
               {
                  DWORD dwBytesWritten( 0 );
                  
                  if( InternetWriteFile( hiFile, (void*)GetMemory(), GetSizeBytes(), &dwBytesWritten ) )
                  {
                     bSuccess = dwBytesWritten == stSize;
                  }

                  InternetCloseHandle( hiFile );
               }
            }

            InternetCloseHandle( hiSession );
         }

         InternetCloseHandle( hiInternet );
      }

      return( bSuccess );
   }
#endif

   virtual MMemory< tMemType >& Empty() throw() = 0;


   virtual MMemory< tMemType >& LoadFile( const HANDLE hFile ) throw()
   {
      MASSERT( CanReAllocate() );

      if( (INVALID_HANDLE_VALUE != hFile) && (NULL != hFile) )
      {
         DWORD dwSizeHigh( 0 );
         DWORD dwSizeLow( GetFileSize( hFile, &dwSizeHigh ) );

         if( !((INVALID_FILE_SIZE == dwSizeLow) && (NO_ERROR != GetLastError())) )
         {
            // HeapAlloc can't allocate more then DWORD = 4 GB.
            if( 0 == dwSizeHigh )
            {
               // Cannot accept data that is not fit for this memory.
               if( 0 == dwSizeLow % sizeof( tMemType ) )
               {
                  // Allocate 
                  ReAllocate( (size_t)dwSizeLow );

                  SetFilePointer( hFile, 0, 0, FILE_BEGIN );

                  DWORD dwBytesRead( 0 );
                  if( ReadFile( hFile, (void*)pData, (DWORD)stSize, &dwBytesRead, NULL ) )
                  {
                     if( dwBytesRead == stSize )
                     {
                        return( *this );
                     }
                  }
               }
            }
         }
      }

      Empty();

      return( *this );
   }


// To use this function you need to uncomment the #define INTERNET_CONNECTIVITY above and link your project with wininet.lib.
#if defined( INTERNET_CONNECTIVITY )
   virtual MMemory< tMemType >& LoadFileFTP( LPCTSTR strServer, LPCTSTR strAgent, LPCTSTR strFolder, LPCTSTR strFilename, LPCTSTR strUser, LPCTSTR strPassword ) throw()
   {

      MASSERT( CanReAllocate() );

      Empty();

      // Open Internet Handle.
      HINTERNET hiInternet = InternetOpen( strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

      if( NULL != hiInternet )
      {
         // Open Session Handle.
         HINTERNET hiSession = InternetConnect( hiInternet, strServer, INTERNET_DEFAULT_FTP_PORT, strUser, strPassword, INTERNET_SERVICE_FTP, 0, 0 );

         if( NULL != hiSession )
         {
            if( FtpSetCurrentDirectory( hiSession, strFolder ) )
            {
               HINTERNET hiFile = FtpOpenFile( hiSession, strFilename, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0 );

               if( NULL != hiFile )
               {
                  MList< MMemoryPH< BYTE, HEAP_ZERO_MEMORY > > listData;

                  do
                  {
                     DWORD dwBytesAvailable( 0 );

                     if( InternetQueryDataAvailable( hiFile, &dwBytesAvailable, 0, 0 ) )
                     {
                        listData.AddTail( new MMemoryPH< BYTE, HEAP_ZERO_MEMORY >( MMemory< BYTE >::MemUnits( dwBytesAvailable ) ) );

                        DWORD dwBytesRead( 0 );

                        if( InternetReadFile( hiFile, listData.GetTail()->GetMemory(), listData.GetTail()->GetSizeBytes(), &dwBytesRead ) )
                        {
                           listData.GetTail()->CutSize( MMemory< BYTE >::MemUnits( dwBytesRead ) );
                        }
                        else
                        {
                           listData.Empty();
                        }
                     }
                     else
                     {
                        listData.Empty();
                     }
                  }
                  while( listData.IsEmpty() ? false : 0 != listData.GetTail()->GetSize().InBytes() );

                  if( !listData.IsEmpty() )
                  {
                     listData.Accelerate();

                     MMemory< BYTE >::MemUnits muOffset;

                     for( DWORD dwIndex = 0; dwIndex < listData.GetCount(); dwIndex++ )
                     {
                        muOffset += listData[dwIndex]->GetSize();
                     }

                     AppearAs< MMemory< BYTE > >().ReAllocate( muOffset );

                     muOffset = 0;

                     for( DWORD dwIndex = 0; dwIndex < listData.GetCount(); dwIndex++ )
                     {
                        AppearAs< MMemory< BYTE > >().Transfer( muOffset, listData[dwIndex]->GetMemory(), listData[dwIndex]->GetSize(), true );
                     }
                  }
               }
            }

            InternetCloseHandle( hiSession );
         }

         InternetCloseHandle( hiInternet );
      }

      return( *this );
   }
#endif

   virtual MMemory< tMemType >& LoadResource( LPCTSTR strModule, LPCTSTR strType, const DWORD dwName ) throw()
   {
      MASSERT( CanReAllocate() );
      MASSERT( NULL != strType );

      const HMODULE hmModule( NULL == strModule ? GetModuleHandle( NULL ) : LoadLibrary( strModule ) );

      if( NULL != hmModule )
      {
         // Locate the resource.
         const HRSRC hResFound( FindResource( hmModule, MAKEINTRESOURCE( dwName ), strType ) );

         if( NULL != hResFound )
         {
            // Load the resource.
            const MMemory< BYTE >::MemUnits muSize( ::SizeofResource( hmModule, hResFound ) );
            const HRSRC                     hrRes( (HRSRC)::LoadResource( hmModule, hResFound ) );

            if( NULL != hrRes )
            {
               // Cannot accept resource that is not fit for this memory.
               if( 0 == muSize.GetUnits() % sizeof( tMemType ) )
               {
                  // Lock the resource.
                  const BYTE* pb1ResLock( (BYTE*)LockResource( hrRes ) );

                  if( NULL != pb1ResLock )
                  {
                     AppearAs< MMemory< BYTE > >().ReAllocateTransfer( pb1ResLock, muSize.AppearAs< tMemType >() );

                     if( NULL != strModule ) { FreeLibrary( hmModule ); }

                     return( *this );
                  }
               }
            }
         }

         if( NULL != strModule ) { FreeLibrary( hmModule ); }
      }

      Empty();

      return( *this );
   }

   // NULL == hmModule implies the handle of the file that was used to create the current process.
   virtual MMemory< tMemType >& LoadResource( HMODULE hmModule, LPCTSTR strType, const DWORD dwName ) throw()
   {
      MASSERT( CanReAllocate() );
      MASSERT( NULL != strType );

      if( NULL == hmModule )
      {
         hmModule = GetModuleHandle( NULL );
      }

      // Locate the resource.
      const HRSRC hResFound( FindResource( hmModule, MAKEINTRESOURCE( dwName ), strType ) );

      if( NULL != hResFound )
      {
         // Load the resource.
         const MMemory< BYTE >::MemUnits muSize( ::SizeofResource( hmModule, hResFound ) );
         const HRSRC                     hrRes( (HRSRC)::LoadResource( hmModule, hResFound ) );

         if( NULL != hrRes )
         {
            // Cannot accept resource that is not fit for this memory.
            if( 0 == muSize.GetUnits() % sizeof( tMemType ) )
            {
               // Lock the resource.
               const BYTE* pb1ResLock( (BYTE*)LockResource( hrRes ) );

               if( NULL != pb1ResLock )
               {
                  AppearAs< MMemory< BYTE > >().ReAllocateTransfer( pb1ResLock, muSize.AppearAs< tMemType >() );

                  return( *this );
               }
            }
         }
      }

      Empty();

      return( *this );
   }
   virtual bool StoreResource( LPCTSTR strModule, LPCTSTR strType, const DWORD dwName, const USHORT usLanguage, const USHORT usSubLanguage, const BOOL bDeleteExistingResources ) const
   {
      MASSERT( NULL != strModule );
      MASSERT( NULL != strType   );

      HANDLE hUpdateRes( BeginUpdateResource( strModule, bDeleteExistingResources ) );

      if( NULL != hUpdateRes )
      {
         if( UpdateResource( hUpdateRes, strType, MAKEINTRESOURCE( dwName ), MAKELANGID( usLanguage, usSubLanguage ), (void*)GetMemory(), GetSizeBytes32< true >() ) )
         {
            return( FALSE != EndUpdateResource( hUpdateRes, FALSE ) );
         }
      }

      return( false );
   }

   virtual double GetHash1() const // throw( may throw an overflow exception in future )
   {
      double dCompressed( 0 );

      for( DWORD dwIndex = 0; dwIndex < GetSizeBytes(); dwIndex++ )
      {
         dCompressed += double( dwIndex + ((BYTE*)pData)[dwIndex] ) / double( 256 + dwIndex );
      }

      return( dCompressed );
   }

   // Returns signature of the memory object. Every muBytesRow will generate one double sum of that muBytesRow elements.
   virtual const MMemory< tMemType >& Signature1( typename MMemory< double >& memSignature, typename const MMemory< tMemType >::MemUnits& muBytesRow ) const throw()
   {
      MASSERT( 1 < muBytesRow.InBytes() );
      MASSERT( memSignature.CanReAllocate() );

      const DWORD dwSumCount( (GetSizeBytes32< true >() + muBytesRow.InBytes32< true >() - 1) / muBytesRow.InBytes32< true >() );

      memSignature.ReAllocate( MMemory< double >::MemUnits( dwSumCount ) );

      for( DWORD dwSum = 0; dwSum < dwSumCount; dwSum++ )
      {
         memSignature[dwSum] = 0;

         const DWORD dwBase( dwSum * muBytesRow.InBytes32< true >() );
         const DWORD dwLine( MMIN< DWORD, DWORD >( muBytesRow.InBytes32< true >(), GetSizeBytes32< true >() - dwBase ) );

         for( DWORD dwIndex = 0; dwIndex < dwLine; dwIndex++ )
         {
            memSignature[dwSum] += (dwIndex + double( GetItemCast< BYTE >( dwBase + dwIndex ) ) / double( (double)0x100 ) );
         }
      }

      return( *this );
   }

   virtual bool Verify1( typename const MMemory< double >& memSignature, typename const MMemory< tMemType >::MemUnits& muBytesRow ) const throw()
   {
      MASSERT( 1 < muBytesRow.InBytes() );
      MASSERT( memSignature.CanReAllocate() );

      // I just need some memory type taht can reallocate.
      MMemory< double > *pMemVerify( memSignature.Clone() );

      Signature1( *pMemVerify, muBytesRow );

      const bool bSame = *pMemVerify == memSignature;

      delete pMemVerify;

      return( bSame );
   }

   // muFirst is zero based index
   virtual void InsertIdentity( const MemUnits& muFirst, const MemUnits& muCount, const MemUnits& muInsertIdentityPosition ) throw() = 0;
   // Returns true if the data is consistent and false is data is NOT consistent.
   // If returned value is true the object is changed and the identity data is removed.
   // If the returned value is false the object is not changed.
   virtual bool RemoveIdentity( const MemUnits& muFirst, const MemUnits& muCount, const MemUnits& muInsertIdentityPosition ) throw()
   {
      // the necessary size to store the data + the size for the check sum
      const size_t stNewDataSize( GetSizeBytes() - sizeof( double ) );

      if( 0 > (int)stNewDataSize )
      {
         return( false );
      }

      const BYTE* pIdentityCheckStream( (BYTE*)((size_t)pData + muFirst.InBytes()) );

      // check the check sum
      double* const pdStreamAtTheCheckSum( (double*)((size_t)pIdentityCheckStream + muInsertIdentityPosition.InBytes() ) );
      double dCheckSumCandidate = *pdStreamAtTheCheckSum;

      *pdStreamAtTheCheckSum = double( 0x4D + 0x57 );

      const double dDeNumenator( (double)0x100 );

      const size_t stFirstOffset( muFirst.InBytes() );
      const size_t stCountBytes(  muCount.InBytes() + sizeof( double ) );

      for( size_t stIndex = stCountBytes - 1; 0 <= (int)stIndex; stIndex-- )
      {
         double dNumber( *(BYTE*)const_cast< tMemType* >( &(pData[stFirstOffset + stIndex]) ) );

         dCheckSumCandidate -= (stIndex + dNumber / dDeNumenator);
      }

      if( 0 != dCheckSumCandidate )
      {
         // Data corrupted
         return( false );
      }

      // Everything seems to be Ok, move the Data.
      memcpy( (void*)pdStreamAtTheCheckSum, (void*)((size_t)pdStreamAtTheCheckSum + sizeof( double )), stNewDataSize - ((size_t)pdStreamAtTheCheckSum - (size_t)pData) );

      stSize = stNewDataSize;

      return( true );
   }
   static size_t GetIdentitySize() throw() { return( sizeof( double ) ); }
};

template< class tMemType = BYTE, DWORD dwMemFlags = HEAP_GENERATE_EXCEPTIONS >
class MMemoryPH : public MMemory< tMemType >
{
protected:
   const HANDLE hHeap;

public:
   MMemoryPH() : hHeap( GetProcessHeap() )
   {
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = 0 );
   }

   MMemoryPH( const MMemory< tMemType > &objSrcMMemory ) : hHeap( GetProcessHeap() )
   {
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = objSrcMMemory.GetSizeBytes() );
      memcpy( pData, objSrcMMemory.GetMemory(), stSize );
   }

   MMemoryPH( const tMemType &objSource ) : hHeap( GetProcessHeap() )
   {
      pData  = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = sizeof( tMemType ) );
      *pData = objSource;
   }

   MMemoryPH( const HANDLE hFile ) : hHeap( GetProcessHeap() )
   {
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = 0 );

      LoadFile( hFile );
   }

   MMemoryPH( const MMemory< tMemType > &objSourceA, const MMemory< tMemType > &objSourceB ) : hHeap( GetProcessHeap() )
   {
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = objSourceA.GetSizeBytes() + objSourceB.GetSizeBytes() );
      memcpy( pData, objSourceA.GetMemory(), objSourceA.GetSizeBytes() );
      memcpy( (void*)((size_t)pData + objSourceA.GetSizeBytes()), objSourceB.GetMemory(), objSourceB.GetSizeBytes() );
   }

   MMemoryPH( const MMemoryPH< tMemType, dwMemFlags > &objSrcMMemoryPH ) : hHeap( GetProcessHeap() )
   {
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = objSrcMMemoryPH.GetSizeBytes() );
      memcpy( pData, objSrcMMemoryPH.GetMemory(), stSize );
   }

   MMemoryPH( const MMemory< tMemType >::MemUnits &muNumberOfUnits ) : hHeap( GetProcessHeap() )
   {
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = muNumberOfUnits.InBytes() );
   }

   MMemoryPH( const size_t stMemory ) : hHeap( GetProcessHeap() )
   {
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = stMemory );
   }

   MMemoryPH( const tMemType *pDataSource, const size_t stMemory ) : hHeap( GetProcessHeap() )
   {
      MASSERT( (0 == stMemory) || (NULL != pDataSource) );
      
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = stMemory );

      memcpy( pData, pDataSource, stSize );
   }

   MMemoryPH( const tMemType *pDataSource, const MMemory< tMemType >::MemUnits& memUnits ) : hHeap( GetProcessHeap() )
   {
      MASSERT( (0 == memUnits.GetUnits()) || (NULL != pDataSource) );
      
      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = memUnits.InBytes() );

      memcpy( pData, pDataSource, stSize );
   }

   MMemoryPH( const tMemType *pDataSource, const tMemType& tmSeparator, const bool bIncluding ) : hHeap( GetProcessHeap() )
   {
      DWORD dwCount = 0;

      for( ; pDataSource[dwCount] != tmSeparator; dwCount++ );

      if( bIncluding )
      {
         dwCount++;
      }

      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = dwCount * sizeof( tMemType ) );

      memcpy( pData, pDataSource, stSize );
   }

   virtual ~MMemoryPH()
   {
      MASSERT( NULL != pData );
      HeapFree( hHeap, 0, pData );
   }

public:
   virtual bool      CanReAllocate() const throw() { return( true ); }

   virtual MMemory< tMemType >* Clone() const throw()
   {
      return( new MMemoryPH< tMemType, dwMemFlags >( *this ) );
   }

   virtual tMemType* ReAllocate( const size_t stMemAmount )
   {
      HeapFree( hHeap, dwMemFlags, pData );

      return( pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = stMemAmount ) );
   }

   virtual tMemType* ReAllocate( const MMemory< tMemType >::MemUnits &memUnits )
   {
      HeapFree( hHeap, dwMemFlags, pData );

      return( pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = memUnits.InBytes() ) );
   }

   virtual tMemType *ReAllocateTransfer( const tMemType *pSrc, const MMemory< tMemType >::MemUnits &memUnits )
   {
      // Make sure the blocks are not overlapping.
      MASSERT( bool( !IsInRange< size_t, true, size_t, false, size_t >( (size_t)pData, (size_t)pSrc, (size_t)pData + stSize ) ) );
      MASSERT( bool( !IsInRange< size_t, true, size_t, false, size_t >( (size_t)pSrc, (size_t)pData, (size_t)pSrc + memUnits.InBytes() )) );

      HeapFree( hHeap, dwMemFlags, pData );

      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = memUnits.InBytes() );

      memcpy( pData, pSrc, stSize );

      return( pData );
   }

   virtual MMemory< tMemType >& Empty() throw()
   {
      HeapFree( hHeap, dwMemFlags, pData );

      pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = 0 );

      return( *this );
   }

   virtual MMemory< tMemType >& operator=( const MMemory< tMemType > &objSource )
   {
      if( objSource.GetSizeBytes() != stSize )
      {
         HeapFree( hHeap, dwMemFlags, pData );

         pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = objSource.GetSizeBytes() );
      }

      memcpy( pData, objSource.GetMemory(), stSize );

      return( *this );
   }

   MMemoryPH< tMemType, dwMemFlags >& operator=( const MMemoryPH< tMemType, dwMemFlags > &objSource )
   {
      if( objSource.GetSizeBytes() != stSize )
      {
         HeapFree( hHeap, dwMemFlags, pData );

         pData = (tMemType*)HeapAlloc( hHeap, dwMemFlags, stSize = objSource.GetSizeBytes() );
      }

      memcpy( pData, objSource.GetMemory(), stSize );

      return( *this );
   }

   void *operator new( size_t stObjectSize )
   {
      return( HeapAlloc( GetProcessHeap(), dwMemFlags, stObjectSize ) );
   }

   void operator delete( void *pReleasedObject )
   {
      if( NULL == pReleasedObject )
      {
         return;
      }

      HeapFree( GetProcessHeap(), 0, pReleasedObject );
   }

   bool operator==( const MMemoryPH< tMemType, dwMemFlags > &objAnotherMMemoryPH ) const throw()
   {
      if( GetSizeBytes() != objAnotherMMemoryPH.GetSizeBytes() )
      {
         return( false );
      }

      if( typeid( (*this) ).name() != typeid( objAnotherMMemoryPH ).name() )
      {
         return( false );
      }

      return( 0 == memcmp( (void*)GetMemory(), (void*)objAnotherMMemoryPH.GetMemory(), GetSizeBytes() ) );
   }

   bool operator!=( const MMemoryPH< tMemType, dwMemFlags > &objAnotherMMemoryPH ) const throw()
   {
      return( !operator==( objAnotherMMemoryPH ) );
   }

   template< DWORD dwRetMemFlags >
   MMemoryPH< char, dwMemFlags > Bin2Ascii() const
   {
      MMemoryPH< char, dwMemFlags > memDestination( 2 * stSize );
      BYTE *pDestination = (BYTE*)memDestination.GetMemory();

      for( DWORD dwIndex = 0; dwIndex < stSize; dwIndex++ )
      {
         const BYTE b1Byte( ((BYTE*)pData)[dwIndex] );

         const BYTE b1MSHB( b1Byte >> 0x04 );
         const BYTE b1LSHB( b1Byte &  0x0F );

         pDestination[ 2 * dwIndex     ] = (BYTE)(0x30 + (b1MSHB < 0x0A ? b1MSHB : b1MSHB + 0x07));
         pDestination[ 2 * dwIndex + 1 ] = (BYTE)(0x30 + (b1LSHB < 0x0A ? b1LSHB : b1LSHB + 0x07));
      }

      return( memDestination );
   }

   template< typename tReturn > tReturn GetSumOfElements() const throw()
   {
      typename tReturn tResult( 0 );

      for( DWORD dwItem = 0; dwItem < GetSize(); dwItem++ )
      {
         tResult = tResult + (tReturn)(pData[dwItem]);
      }

      return( tResult );
   }

   virtual void InsertIdentity( const MMemory< tMemType >::MemUnits& muFirst, const MMemory< tMemType >::MemUnits& muCount, const MMemory< tMemType >::MemUnits& muInsertIdentityPosition ) throw()
   {
      MASSERT( muFirst.InBytes()  <  GetSizeBytes() );
      MASSERT( (muFirst.InBytes() +  muCount.InBytes()) <= GetSizeBytes() );
      MASSERT( (muFirst.InBytes() <= muInsertIdentityPosition.InBytes()) && (muInsertIdentityPosition.InBytes() <= (muFirst.InBytes() + muCount.InBytes())) );

      // keep the precission MASSERT( muCount < X * GetSizeBytes() );

      // the necessary size to store the data + the size for the check sum
      const size_t stNecessaryStorageSize( sizeof( double ) + GetSizeBytes() );

      // put the check sun in the requested position
      const size_t stCheckSumPlaceOffset( muFirst.InBytes() + muInsertIdentityPosition.InBytes() );

      BYTE *pNewData( (BYTE*)HeapAlloc( hHeap, dwMemFlags, stNecessaryStorageSize ) );

      // transfer the memory from the beginning to the check sum position
      memcpy( pNewData, pData, stCheckSumPlaceOffset );

      // place check the checksum
      double* const pdStreamAtTheCheckSum( (double*)((size_t)pNewData + stCheckSumPlaceOffset ) );
      *pdStreamAtTheCheckSum = double( 0x4D + 0x57 );

      // copy the rest of the data
      memcpy( (void*)((size_t)pNewData + stCheckSumPlaceOffset + sizeof( double )), (void*)((size_t)pData + stCheckSumPlaceOffset), GetSizeBytes() - stCheckSumPlaceOffset );

      const double dDeNumenator( (double)0x100 );
      double dNewCheckSum( 0 );
      const size_t stFirstOffset( muFirst.InBytes() );
      const size_t stCountBytes(  muCount.InBytes() + sizeof( double ) );

      for( size_t stIndex = 0; stIndex < stCountBytes; stIndex++ )
      {
         double dNumber( pNewData[stFirstOffset + stIndex] );

         dNewCheckSum += (stIndex + dNumber / dDeNumenator);
      }

      *pdStreamAtTheCheckSum = dNewCheckSum;

      HeapFree( hHeap, dwMemFlags, pData );

      stSize = stNecessaryStorageSize;
      pData  = (tMemType*)pNewData;
   }
};


template< class MMemoryType, class tMemType >
class MSecureMemory : public MMemoryType
{
private:
   __forceinline void DisqualifyContent() throw()
   {
      memset( pData, 0, stSize );
      
      if( 0 < stSize )
      {
         volatile BYTE b1Data( *(BYTE*)pData );

         *(BYTE*)pData = b1Data;
      }
   }

public:
   MSecureMemory() : MMemoryType() {}
   MSecureMemory( const MMemory< tMemType >& memSource ) : MMemoryType( memSource.GetMemory(), memSource.GetSizeBytes() ) {}
   //MSecureMemory( const tMemType& objSource ) : MMemoryType( objSource ) {}
   MSecureMemory( const MMemoryType& memSource ) : MMemoryType( memSource ) {}
   MSecureMemory( const tMemType *pDataSource, const size_t stMemory ) : MMemoryType( pDataSource, stMemory ) {}
   MSecureMemory( const tMemType *pDataSource, typename const MMemory< tMemType >::MemUnits& memUnits   ) : MMemoryType( pDataSource, memUnits  ) {}
   MSecureMemory( typename const MMemory< tMemType >& memSourceA, typename const MMemory< tMemType >& memSourceB ) : MMemoryType( memSourceA, memSourceB ) {}
   MSecureMemory( const tMemType *pDataSource, const tMemType& tmSeparator, const bool bIncluding ) : MMemoryType( pDataSource, tmSeparator, bIncluding ) {}
   MSecureMemory( HWND hWnd ) : MMemoryType( hWnd ) {}


   virtual ~MSecureMemory()
   {
      DisqualifyContent();
   }


   virtual tMemType* ReAllocate( const size_t stMemAmount )
   {
      DisqualifyContent();
      
      return( MMemoryType::ReAllocate( stMemAmount ) );
   }


   virtual tMemType* ReAllocate( typename const MMemory< tMemType >::MemUnits &memUnits )
   {
      DisqualifyContent();

      return( MMemoryType::ReAllocate( memUnits ) );
   }


   virtual tMemType *ReAllocateTransfer( const tMemType *pSrc, typename const MMemory< tMemType >::MemUnits &memUnits )
   {
      DisqualifyContent();

      return( MMemoryType::ReAllocateTransfer( pSrc, memUnits ) );
   }


   virtual MMemory< tMemType >& Empty() throw()
   {
      DisqualifyContent();

      return( MMemoryType::Empty() );
   }


   virtual MMemory< tMemType >& operator=( typename const MMemory< tMemType >& objSource )
   {
      DisqualifyContent();

      return( MMemoryType::operator=( objSource ) );
   }


   MMemoryType& operator=( const MMemoryType& objSource )
   {
      DisqualifyContent();

      return( MMemoryType::operator=( objSource ) );
   }


   virtual MMemory< tMemType >& CutSize( typename const MMemory< tMemType >::MemUnits muNewSize ) throw()
   {
      MASSERT( muNewSize.InBytes() <= stSize );

      tMemType* pData2( pData );

      pData  = (tMemType*)((size_t)pData + muNewSize.InBytes());
      stSize = stSize - muNewSize.InBytes();

      DisqualifyContent();

      pData  = pData2;
      stSize = muNewSize.InBytes();

      return( *this );
   }


   virtual MMemory< tMemType >& DecrementSize( typename const MMemory< tMemType >::MemUnits muDecrementUnits ) throw()
   {
      MASSERT( muDecrementUnits.InBytes() <= stSize );

      tMemType* pData2( pData );
      size_t    stSize2( stSize );

      pData  = (tMemType*)((size_t)pData + stSize - muDecrementUnits.InBytes());
      stSize = muDecrementUnits.InBytes();

      DisqualifyContent();

      pData  = pData2;
      stSize = stSize2 - muDecrementUnits.InBytes();

      return( *this );
   }


private:
   // Do not let access to casting as non secure memory.
   operator       MMemoryType  () const throw() { return( *this ); }
   operator       MMemoryType  ()       throw() { return( *this ); }
   operator const MMemoryType& () const throw() { return( *this ); }
   operator       MMemoryType& ()       throw() { return( *this ); }
   operator const MMemoryType* () const throw() { return(  this ); }
   operator       MMemoryType* ()       throw() { return(  this ); }
};

template< class MMemoryType, class tMemType >
class MSegmentedMemory : public MMemoryType
{
private:
   size_t               stSegmentsSize;
   MList< MMemoryType > listAddedSegments;

protected:
   void ReleaseSegments()
   {
      stSegmentsSize = 0;
      listAddedSegments.Empty();
   }

   void IncorporateSegments()
   {
      listAddedSegments.Accelerate();

      CloneAsChild( stSegmentsSize );

      for( DWORD dwSegment = 0; dwSegment < listAddedSegments.GetCount(); dwSegment++ )
      {
      }
   }

public:
   MSegmentedMemory() : stSegmentsSize( 0 ) { MASSERT( MMemoryType< tMemType >::CanReAllocate() ); }
   virtual ~MSegmentedMemory() {}

   MMemory< tMemType >& AddSegment( typename const MMemory< tMemType >::MemUnits& muSize )
   {
      listAddedSegments.AddTail( new MMemoryType( muSize ) );

      stSegmentsSize += muSize.InBytes();

      return( *this );
   }

   MMemory< tMemType >& GetFirst() throw() { return( listAddedSegments.IsEmpty() ? *this : *listAddedSegments.GetHead() ); }
   MMemory< tMemType >& GetLast()  throw() { return( listAddedSegments.IsEmpty() ? *this : *listAddedSegments.GetTail() ); }

   // IsEmpty
   // AppearAs
   // GetMemory
   // ...

   virtual tMemType* ReAllocate( const size_t stMemAmount )
   {
      ReleaseSegments();
      
      return( MMemoryType::ReAllocate( stMemAmount ) );
   }
   virtual tMemType* ReAllocate( typename const MMemory< tMemType >::MemUnits &memUnits )
   {
      ReleaseSegments();

      return( MMemoryType::ReAllocate( memUnits ) );
   }
   virtual tMemType *ReAllocateTransfer( const tMemType *pSrc, typename const MMemory< tMemType >::MemUnits &memUnits )
   {
      ReleaseSegments();

      return( MMemoryType::ReAllocateTransfer( pSrc, memUnits ) );
   }
   virtual MMemory< tMemType >& Empty() throw()
   {
      ReleaseSegments();

      return( MMemoryType::Empty() );
   }
   virtual MMemory< tMemType >& operator=( typename const MMemory< tMemType >& objSource )
   {
      ReleaseSegments();

      return( MMemoryType::operator=( objSource ) );
   }
   MMemoryType& operator=( const MMemoryType& objSource )
   {
      ReleaseSegments();

      return( MMemoryType::operator=( objSource ) );
   }
   virtual MMemory< tMemType >& CutSize( typename const MMemory< tMemType >::MemUnits muNewSize ) throw()
   {
      // havent got time
      MASSERT( FASLE );

      return( *this );
   }
   virtual MMemory< tMemType >& DecrementSize( typename const MMemory< tMemType >::MemUnits muDecrementUnits ) throw()
   {
      // havent got time
      MASSERT( FALSE );

      return( *this );
   }

private:
   // These are possible but need to be written - when casting to MMemory the compiler will cast to the parent memory.
   // This is not what I want - what I want in such case is to collect all segments and the parent memory and to
   // Reallocate and transfer to the parent memory.
   operator const MMemory< tMemType >& () const throw() { return( *this ); }
   operator       MMemory< tMemType >& ()       throw() { return( *this ); }
   operator const MMemory< tMemType >* () const throw() { return(  this ); }
   operator       MMemory< tMemType >* ()       throw() { return(  this ); }

   operator       MMemoryType  () const throw() { return( *this ); }
   operator       MMemoryType  ()       throw() { return( *this ); }
   operator const MMemoryType& () const throw() { return( *this ); }
   operator       MMemoryType& ()       throw() { return( *this ); }
   operator const MMemoryType* () const throw() { return(  this ); }
   operator       MMemoryType* ()       throw() { return(  this ); }
};

template< class tMemType = BYTE, DWORD dwMemFlags = HEAP_GENERATE_EXCEPTIONS >
class MShellMemory : public MMemory< tMemType >
{
   friend class MSecureMemory< MShellMemory< tMemType, dwMemFlags >, tMemType >;

public:
   MShellMemory() {}
   MShellMemory( tMemType *pDataSource, size_t stMemory ) : MMemory< tMemType >( pDataSource, stMemory ) {}
   MShellMemory( tMemType *pDataSource, const MMemory< tMemType >::MemUnits &memUnits = 1 ) : MMemory< tMemType >( pDataSource, memUnits ) {}
   MShellMemory( const MMemory< tMemType >& memSource ) : MMemory< tMemType >( memSource ) {}
   MShellMemory( const MShellMemory< tMemType >& memSource ) : MMemory< tMemType >( memSource ) {}
   virtual ~MShellMemory() {}

   virtual bool                 CanReAllocate() const throw() { return( false ); }
   virtual MMemory< tMemType >* Clone()         const throw() { return( new MShellMemory< tMemType, dwMemFlags >( *this ) ); }

private:
   // Can't exist because of obvious reasons - it is agains tha nature ot the type.
   virtual tMemType* ReAllocate( const size_t )                                                    { MASSERT( FALSE ); return( pData ); }
   virtual tMemType* ReAllocate( const MMemory< tMemType >::MemUnits & )                           { MASSERT( FALSE ); return( pData ); }
   virtual tMemType* ReAllocateTransfer( const tMemType *, const MMemory< tMemType >::MemUnits & ) { MASSERT( FALSE ); return( pData ); }
   // Can't exist because I can't guarantie that the object will remain const, as I pass const one.
   virtual MMemory< tMemType >& operator=( const MMemory< tMemType > & )                           { MASSERT( FALSE ); return( *this ); }

public:
   virtual MMemory< tMemType >& Empty() throw()
   {
      pData  = NULL;
      stSize = 0;

      return( *this );
   }
   virtual MMemory< tMemType >& operator=( MMemory< tMemType >& memSource )
   {
      pData  = memSource.GetMemory();
      stSize = memSource.GetSizeBytes();

      return( *this );
   }
   MMemory< tMemType >& operator=( MShellMemory< tMemType >/*&*/ memSource ) // Can't be const => can't be refernce - language limitation.
   {
      pData  = memSource.GetMemory();
      stSize = memSource.GetSizeBytes();

      return( *this );
   }

private:
   virtual void InsertIdentity( const MemUnits&, const MemUnits&, const MemUnits& ) throw() { MASSERT( FALSE ); }
};

template< class tMemType, DWORD dwItemCount, DWORD dwMemFlags >
class MStackMemory : public MMemory< tMemType >
{
   friend class MSecureMemory< MStackMemory< tMemType, dwItemCount, dwMemFlags >, tMemType >;

private:
   tMemType tData[dwItemCount];

public:
   MStackMemory() : MMemory< tMemType >( tData, dwItemCount * sizeof( tMemType ) )
   {
      #pragma warning( push )
      #pragma warning( disable : 4127 )
      if( bool( HEAP_ZERO_MEMORY & dwMemFlags ) )
      {
         MemSet( 0 );
      }
      #pragma warning( pop )
   }

   MStackMemory( const tMemType& tmTemplate ) : MMemory< tMemType >( tData, dwItemCount * sizeof( tMemType ) )
   {
      for( DWORD dwItem = 0; dwItem < dwItemCount; dwItem++ )
      {
         tData[dwItem] = tmTemplate;
      }
   }

   MStackMemory( const MStackMemory< tMemType, dwItemCount, dwMemFlags >& tmSource ) : MMemory< tMemType >( tData, dwItemCount * sizeof( tMemType ) )
   {
      for( DWORD dwItem = 0; dwItem < dwItemCount; dwItem++ )
      {
         tData[dwItem] = tmSource[dwItem];
      }
   }

   ~MStackMemory() {}

private: // private = BAND ACCESS
   void *operator new(      size_t ) { MASSERT( FALSE ); return( NULL ); }
   void *operator new[](    size_t ) { MASSERT( FALSE ); return( NULL ); }
   void  operator delete(   void * ) { MASSERT( FALSE ); }
   void  operator delete[]( void * ) { MASSERT( FALSE ); }


   virtual bool                 CanReAllocate() const throw() { return( false ); }
   virtual MMemory< tMemType >* Clone()         const throw() { MASSERT( FALSE ); return( NULL ); }

   virtual tMemType* ReAllocate( const size_t )                                                    { MASSERT( FALSE ); return( pData ); }
   virtual tMemType* ReAllocate( const MMemory< tMemType >::MemUnits & )                           { MASSERT( FALSE ); return( pData ); }
   virtual tMemType* ReAllocateTransfer( const tMemType *, const MMemory< tMemType >::MemUnits & ) { MASSERT( FALSE ); return( pData ); } // this may be possible for operations within the range of the target memory.
   virtual MMemory< tMemType >& Empty() throw()                                                    { MASSERT( FALSE ); return( *this ); }

public:
   virtual MMemory< tMemType > &operator=( const MMemory< tMemType >& memSource )
   {
      __if_exists( tMemType::operator= )
      {
         for( DWORD dwItem = 0; dwItem < dwItemCount; dwItem++ )
         {
            tData[dwItem] = memSource[dwItem];
         }
      }

      __if_not_exists( tMemType::operator= )
      {
         MASSERT( GetSizeUnit() == memSource.GetSizeUnit() );

         for( DWORD dwItem = 0; dwItem < dwItemCount; dwItem++ )
         {
            memcpy( &tData[dwItem], &memSource[dwItem], sizeof( tMemType ) );
         }
      }

      return( *this );
   }

   MStackMemory< tMemType, dwItemCount, dwMemFlags >& operator=( const MStackMemory< tMemType, dwItemCount, dwMemFlags >& memSource )
   {
      MASSERT( stSize = memSource.stSize );

      for( DWORD dwItem = 0; dwItem < dwItemCount; dwItem++ )
      {
         tData[dwItem] = memSource[dwItem];
      }

      return( *this );
   }

private:
   virtual void InsertIdentity( const MemUnits&, const MemUnits&, const MemUnits& ) throw() { MASSERT( FALSE ); }
};

template< class tMemType >
class MPointerMemory : public MMemory< tMemType >
{
public:
   MPointerMemory()
   {}

   // mempBitmap( new tBitmap( hDCcomp, dwWidth, dwHeight ) ),
   MPointerMemory( tMemType* ptHoldObject )
   {
      pData  = ptHoldObject;
      stSize = NULL != pData ? sizeof( tMemType* ) : 0;
      
      // The size - what it should be?
      //                               0 as the memory object allocates 0 bytes - useless
      //                               sizeof( ptr_type )                       - sort of useful and inambigious meaning.
      //                               size of the object pointed               - 1. not necessarily known, i.e. can be wrong; 2. Makes no difference from MMemoryPH< typename >
      // I have chosen the size of the pointer type, as the only inambigious and possibly useful.
   } 

   // MPointerMemory< MShellMemory< BYTE > > memp( MShellMemory< BYTE >( BYTE*, MemUnits ) ) -> for complex types.
   MPointerMemory( const tMemType& memSource )
   {
      pData  = new tMemType( memSource );
      stSize = NULL != pData ? sizeof( tMemType* ) : 0;
   }

   MPointerMemory( tMemType& memSource )
   {
      pData  = new tMemType( memSource );
      stSize = NULL != pData ? sizeof( tMemType* ) : 0;
   }

   MPointerMemory( const MMemory< tMemType >& memSource )
   {
      pData  = new tMemType( memSource );
      stSize = NULL != pData ? sizeof( tMemType* ) : 0;
   }

   MPointerMemory( const MPointerMemory< tMemType >& memSource )
   {
      pData  = new tMemType( *memSource.pData );
      stSize = NULL != pData ? sizeof( tMemType* ) : 0;
   }

   virtual ~MPointerMemory() throw()
   {
      Empty();
   }

   virtual MMemory< tMemType >& Empty() throw()
   {
      if( NULL != pData )
      {
         delete pData;
         pData = NULL;
      };

      stSize = 0;

      return( *this );
   }

   tMemType* ExportContent() throw()
   {
      tMemType* const pPtr( pData );
      
      pData  = NULL;
      stSize = 0;
      
      return( pPtr );
   }

   MPointerMemory< tMemType >& operator=( tMemType* ptHoldObject )
   {
      if( NULL != pData )
      {
         delete pData;
      }

      pData  = ptHoldObject;
      stSize = NULL != pData ? sizeof( tMemType* ) : 0;

      return( *this );
   }

   MPointerMemory< tMemType >& operator=( const tMemType& objSource )
   {
      *pData = objSource;
      stSize = NULL != pData ? sizeof( tMemType* ) : 0;

      return( *this );
   }

   bool operator==( const tMemType& objCompare ) const throw() { return( *pData == objCompare ); }
   bool operator!=( const tMemType& objCompare ) const throw() { return( *pData != objCompare ); }

   virtual bool IsEmpty() const throw() { return( (NULL == pData) || (0 == stSize) ); }
           bool IsValid() const throw() { return( NULL != pData ? pData->IsValid() : false ); }

public:
   operator       tMemType* ()       throw() { return( pData ); }
   operator const tMemType* () const throw() { return( pData ); }

   operator       tMemType& ()       throw() { return( *pData ); }
   operator const tMemType& () const throw() { return( *pData ); }

private:
   // These MUST be band to avoid implicit asignment. More correctly to force it explicitly.
   // Example: MPointerMemory< MBitmap > memPtrBitmap;
   //          HANDLE  h = memPtrBitmap; - error - if operator void* () is not defiend, the tMemType* () will be called implicitly,
   //                                      and will cast the MBitmap* to void*. What is wanted is MBitmap::HBITMAP.
   //          HGDIOBJ h = memPtrBitmap; - same as above
   operator       void*()               throw() { return( pData ); }
   operator const void*()         const throw() { return( pData ); }

public:
   virtual bool      CanReAllocate() const throw() { return( false ); }

   // At this time I am not exactly sure what i the appropriate behavior for this function ...
   virtual MMemory< tMemType >* Clone()         const throw() { MASSERT( FALSE ); return( NULL ); }

   virtual tMemType* ReAllocate( const size_t )                                                    { MASSERT( FALSE ); return( pData ); }
   virtual tMemType* ReAllocate( const MMemory< tMemType >::MemUnits & )                           { MASSERT( FALSE ); return( pData ); }
   virtual tMemType* ReAllocateTransfer( const tMemType *, const MMemory< tMemType >::MemUnits & ) { MASSERT( FALSE ); return( pData ); }
   virtual MMemory< tMemType > &operator=( const MMemory< tMemType > & )                           { MASSERT( FALSE ); return( *this ); } // this may be possible for equal sized memory chunck, so if( memChunkA.CanToBeEqual( memChunkB ) [{ return( stSize == memChunkB.stSize ); }] ) { memChunkA = memChunkB; }

   virtual void InsertIdentity( const MemUnits&, const MemUnits&, const MemUnits& ) throw() { MASSERT( FALSE ); }
};
#endif
