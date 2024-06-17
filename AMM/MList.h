//
//
// MList.h: MList< typename > - generic list class used in:
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase One'
//
// © Copyright 2001 - 2012 by Miroslav Bonchev Bonchev. All rights reserved.
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
#include "MHandle.h"


#ifdef _WIN64
   typedef unsigned __int64 MLISTINDEX;
#else
   typedef DWORD MLISTINDEX;
#endif


#define MLIST_LAST_INDEX (MLISTINDEX)-1
#define each( indexIdentifier, mlist ) MLISTINDEX indexIdentifier = 0; indexIdentifier < mlist.GetCount(); indexIdentifier++


template< class typeMembers >
class MList
{
   // Common Type Comparators
public:
   class AlphabeticalOrder
   {
   private:
      bool bAscending;

   public:
      AlphabeticalOrder( const bool bAscending ) : bAscending( bAscending ) {}
      ~AlphabeticalOrder() {}

   public:
      bool IsOrdered( const typeMembers& tmLeft, const typeMembers& tmRight ) const
      {
         return( bAscending ? tmLeft < tmRight : tmLeft > tmRight );
      }
   };

   class AlphabeticalOrderOfPairByLabel
   {
   private:
      bool bAscending;

   public:
      AlphabeticalOrderOfPairByLabel( const bool bAscending ) : bAscending( bAscending ) {}
      ~AlphabeticalOrderOfPairByLabel() {}

   public:
      bool IsOrdered( const typeMembers& tmLeft, const typeMembers& tmRight ) const
      {
         return( bAscending ? tmLeft.GetLabelRef() < tmRight.GetLabelRef() : tmLeft.GetLabelRef() > tmRight.GetLabelRef() );
      }
   };


public:
   class ObjectContainer
   {
      friend class MList< typeMembers >;

   private:
      ObjectContainer* pPrevious;
      ObjectContainer* pNext;
      typeMembers*     pObject;

   private:
      ObjectContainer( ObjectContainer *pPrevious_P, typeMembers *pObject_P, ObjectContainer *pNext_P )
      {
         pPrevious   = pPrevious_P;
         pObject     = pObject_P;
         pNext       = pNext_P;
      }
      
      ObjectContainer( const ObjectContainer& objObjectContainer )
      {
         pPrevious = objObjectContainer.pPrevious;
         pNext     = objObjectContainer.pNext;
         pObject   = objObjectContainer.pObject;
      }

      const ObjectContainer& operator=( const ObjectContainer& objObjectContainer )
      {
         pPrevious = objObjectContainer.pPrevious;
         pNext     = objObjectContainer.pNext;
         pObject   = objObjectContainer.pObject;
         
         return( *this );
      }

   public:
      ~ObjectContainer()
      {
      }

   public:
      ObjectContainer* GetNext() const
      {
         return( pNext );
      }

      ObjectContainer* GetPrevious() const
      {
         return( pPrevious );
      }

      typeMembers* GetObject() const
      {
         return( pObject );
      }

      typeMembers* ReplaceObject( typeMembers* pNewObject )
      {
         typeMembers* pRemplacedObject( pObject );

         pObject = pNewObject;

         return( pRemplacedObject );
      }
   };


protected:
   MLISTINDEX            uiNumberOfMembers;
   ObjectContainer*      pTail;
   ObjectContainer*      pHead;
   mutable typeMembers** ppListArray;


public:
   MList()
      :  pTail( NULL ),
         pHead( NULL ),
         ppListArray( NULL ),
         uiNumberOfMembers( 0 )
   {
   }


   // Constructor with just one element which will be duplicated for the list.
   MList( const typeMembers &tMemberAddHead )
      :  pTail( NULL ),
         pHead( NULL ),
         ppListArray( NULL ),
         uiNumberOfMembers( 0 )
   {
      AddHead( new typeMembers( tMemberAddHead ) );
   }


   MList( const MList< typeMembers > &listSrc )
      :  pTail( NULL ),
         pHead( NULL ),
         ppListArray( NULL ),
         uiNumberOfMembers( 0 )
   {
      for( ObjectContainer* pObjectContainer = listSrc.pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext )
      {
         AddTail( new typeMembers( *pObjectContainer->pObject ) );
      }

      if( NULL != listSrc.ppListArray )
      {
         Accelerate();
      }
      else
      {
         DeAccelerate();
      }
   }

   // Constructor from list of pointers. The last pointer MUST be NULL. Pounters are "swallowed" by the list.
   MList( typeMembers* ptmHeadObject, ... )
      :  pTail( NULL ),
         pHead( NULL ),
         ppListArray( NULL ),
         uiNumberOfMembers( 0 )
   {
      va_list   listArg;
      va_start( listArg, ptmHeadObject );

      typeMembers* pObject( ptmHeadObject );

      while( NULL != pObject )
      {
         AddTail( pObject );

         pObject = va_arg( listArg, typeMembers* );
      }

      va_end( listArg );
   }


   MList< typeMembers > &operator=( const MList< typeMembers > &listSrc )
   {
      Empty();
      DeAccelerate();

      for( ObjectContainer* pObjectContainer = listSrc.pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext )
      {
         AddTail( new typeMembers( *pObjectContainer->pObject ) );
      }

      if( NULL != listSrc.ppListArray )
      {
         Accelerate();
      }
      else
      {
         DeAccelerate();
      }

      return( *this );
   }


   MList< typeMembers > operator+( const MList< typeMembers > &listRight ) const
   {
      MList< typeMembers > listSum;

      listSum = *this;

      for( ObjectContainer* pObjectContainer = listRight.pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext )
      {
         listSum.AddTail( new typeMembers( *pObjectContainer->pObject ) );
      }

      return( listSum );
   }


   MList< typeMembers > &operator+=( const MList< typeMembers > &listRight )
   {
      for( ObjectContainer* pObjectContainer = listRight.pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext )
      {
         AddTail( new typeMembers( *pObjectContainer->pObject ) );
      }

      if( NULL != ppListArray )
      {
         Accelerate( true );
      }

      return( *this );
   }


	virtual ~MList()
   {
      Empty();
   }


   // Operator== <- does not consider the mutable variables. e.g 2 identical lists,
   // but the first is accelerated and the second is not will be considered equal.
   bool operator==( const MList< typeMembers > &listAnother ) const throw()
   {
      if( GetCount() != listAnother.GetCount() ) { return( false ); }

      ObjectContainer* pObjectContainer_this( pHead );
      ObjectContainer* pObjectContainer_other( listAnother.pHead );

      for( ; NULL != pObjectContainer_this; pObjectContainer_this = pObjectContainer_this->pNext, pObjectContainer_other = pObjectContainer_other->pNext )
      {
         if( (*pObjectContainer_this->pObject) != (*pObjectContainer_other->pObject) ) { return( false ); }
      }

      return( true );
   }


   // Operator=! <- does not consider the mutable variables. e.g 2 identical lists,
   // but the first is accelerated and the second is not will be considered equal.
   bool operator!=( const MList< typeMembers > &listAnother ) const throw()
   {
      return( !operator==( listAnother ) );
   }


   // Empties the list releasing the members.
   virtual MList< typeMembers >& Empty()
   {
      while( NULL != pTail )
      {
         ObjectContainer*  pTailDueToBeDestroyed  = pTail;
         typeMembers*      pObjectToReturn        = pTail->pObject;

         if( NULL == (pTail = pTail->pPrevious) )
         {
            pHead = NULL;
         }
         else
         {
            pTail->pNext = NULL;
         }


         // deletes the ObjectContainer
         delete pTailDueToBeDestroyed;

         uiNumberOfMembers--;

         delete pObjectToReturn;
      }

      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         
         ppListArray = NULL;
      }

      return( *this );
   }


   // Empties the list WITHOUT releasing the members -> useful for lists of shared pointers.
   void EmptyNoDelete()
   {
      while( !IsEmpty() )
      {
         RemoveTail();
      }

      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         
         ppListArray = NULL;
      }
   }


   // Returns true if the list is empty and false if tehre is at leats one member.
   bool IsEmpty() const
   {
      return( NULL == pHead );
   }


   // Removes the tail object from the list and returns a pointer to that object - which is
   // valid pointer to valid object. Returns NULL if the list is already empty.
   // This function will disrupt the list enumeration.
   typeMembers *RemoveTail()
   {
      if( NULL == pTail )
      {
         return( NULL );
      }

      ObjectContainer*  pTailDueToBeDestroyed  = pTail;
      typeMembers*      pObjectToReturn        = pTail->pObject;

      if( NULL == (pTail = pTail->pPrevious) )
      {
         pHead = NULL;
      }
      else
      {
         pTail->pNext = NULL;
      }


      // deletes the ObjectContainer
      delete pTailDueToBeDestroyed;

      uiNumberOfMembers--;

      // deallocates acceleration
      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         
         ppListArray = NULL;
      }

      return( pObjectToReturn );
   }


   // Removes the head object from the list and returns a pointer to that object - which is
   // valid pointer to valid object. Returns NULL if the list is already empty.
   // This function will disrupt the list enumeration.
   typeMembers *RemoveHead()
   {
      if( NULL == pHead )
      {
         return( NULL );
      }

      ObjectContainer*  pHeadDueToBeDestroyed  = pHead;
      typeMembers*      pObjectToReturn        = pHead->pObject;

      if( NULL == (pHead = pHeadDueToBeDestroyed->pNext) )
      {
         pTail = NULL;
      }
      else
      {
         pHead->pPrevious = NULL;
      }

      // deletes the ObjectContainer
      delete pHeadDueToBeDestroyed;

      uiNumberOfMembers--;

      // deallocates acceleration
      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         
         ppListArray = NULL;
      }

      return( pObjectToReturn );
   }


   typeMembers *RemoveMember( const MLISTINDEX uiIndex )
   {
      typeMembers *pObjectToReturn = NULL;

      if( 0 == uiIndex )
      {
         pObjectToReturn = RemoveHead();
      }
      else if( (uiNumberOfMembers - 1) == uiIndex )
      {
         pObjectToReturn = RemoveTail();
      }
      else if( uiNumberOfMembers < uiIndex - 1 )
      {
         pObjectToReturn = NULL;
      }
      else
      {
         ObjectContainer* pObjectContainer = pHead;

         for( MLISTINDEX uiCounter = 0; uiCounter < uiIndex; uiCounter++ )
         {
            pObjectContainer = pObjectContainer->pNext;
         }

         pObjectContainer->pNext->pPrevious = pObjectContainer->pPrevious;
         pObjectContainer->pPrevious->pNext = pObjectContainer->pNext;

         pObjectToReturn = pObjectContainer->pObject;
         uiNumberOfMembers--;

         delete pObjectContainer;

         // deallocates acceleration
         if( NULL != ppListArray )
         {
            HeapFree( GetProcessHeap(), 0, ppListArray );
            
            ppListArray = NULL;
         }
      }

      return( pObjectToReturn );
   }


   // Add new Tail to the list.
   typeMembers* AddTail( typeMembers *pNewTailObject )
   {
      if( NULL == pNewTailObject )
      {
         return( NULL );
      }

      ObjectContainer* pNewListMember = new ObjectContainer( pTail, pNewTailObject, NULL );

      if( NULL != pTail )
      {
         pTail->pNext = pNewListMember;
      }

      pTail = pNewListMember;

      if( NULL == pHead )
      {
         pHead = pTail;
      }

      uiNumberOfMembers++;

      // deallocates acceleration
      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         
         ppListArray = NULL;
      }

      return( pTail->pObject );
   }


   // Add new Head to the list.
   typeMembers* AddHead( typeMembers *pNewHeadObject )
   {
      if( NULL == pNewHeadObject )
      {
         return( NULL );
      }

      ObjectContainer* pNewListMember = new ObjectContainer( NULL, pNewHeadObject, pHead );

      if( NULL != pHead )
      {
         pHead->pPrevious = pNewListMember;
      }

      pHead = pNewListMember;

      if( NULL == pTail )
      {
         pTail = pHead;
      }

      uiNumberOfMembers++;

      // deallocates acceleration
      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );

         ppListArray = NULL;
      }

      return( pHead->pObject );
   }


   // Adds New Object into the list, at position uiIndex, where uiIndex is zero based index.
   // The added new member of the list will be the new member with the passed Index.
   bool AddMember( typeMembers *pNewObject, const MLISTINDEX uiIndex )
   {
      if( NULL == pNewObject )
      {
         return( false );
      }

      bool bResult( true );

      if( 0 == uiIndex )
      {
         AddHead( pNewObject );
      }
      else if( uiIndex >= uiNumberOfMembers )
      {
         AddTail( pNewObject );
      }
      else if( uiIndex < uiNumberOfMembers )
      {
         ObjectContainer* pObjectContainer = pHead;

         // start counter from 1, to make the number of loops 1 less, because later it will say pNext
         for( MLISTINDEX uiCounter = 1; uiCounter < uiIndex; uiCounter++ )
         {
            pObjectContainer = pObjectContainer->pNext;
         }

         ObjectContainer* pNewListMember = new ObjectContainer( pObjectContainer, pNewObject, pObjectContainer->pNext );

         pObjectContainer->pNext->pPrevious  = pNewListMember;
         pObjectContainer->pNext                 = pNewListMember;

         uiNumberOfMembers++;

         // deallocates acceleration
         if( NULL != ppListArray )
         {
            HeapFree( GetProcessHeap(), 0, ppListArray );
            
            ppListArray = NULL;
         }
      }
      else
      {
         bResult = false;
      }

      return( bResult );
   }


   ObjectContainer* AddBehind( ObjectContainer* pFrontMember, typeMembers* pNewObject )
   {
      if( NULL == pFrontMember )
      {
         AddHead( pNewObject );

         return( pHead );
      }

      if( pTail == pFrontMember )
      {
         AddTail( pNewObject );

         return( pTail );
      }

      pFrontMember->pNext = pFrontMember->pNext->pPrevious = new ObjectContainer( pFrontMember, pNewObject, pFrontMember->pNext );

      uiNumberOfMembers++;

      // deallocates acceleration
      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         
         ppListArray = NULL;
      }

      return( pFrontMember->pNext );
   }


   // Add new member to the list at position that reflects the value of the element. This function is useful
   // for cretion of sorted lists. The position at which to plug the new element is found by comparing with
   // operator> or operator< for the list type according to the bMax2Head_Min2Tail parameter, therefore in
   // order the new element position to be correct the list state must be sorted accordingly or empty before
   // calling this function. This function will disrupt the list enumeration.In case of equal new and already 
   // existing element the new member will be added after the already existing one.
   typeMembers* AddMemberSorted( typeMembers *ptNewObject, const bool bMax2Head_Min2Tail )
   {
      if( NULL != ptNewObject )
      {
         ObjectContainer* pObjectContainer( pHead );
         MLISTINDEX       uiIndex( 0 );

         // I will enumerate  from the head, so the worce case is when the new element will become
         // the new tail. I can save at least this case by first comparing to the current tail.
         if( NULL != pTail )
         {
            if( bMax2Head_Min2Tail )
            {
               // Need to be >= to keep the adding order - if new == old - add the new behind the old.
               if( *ptNewObject <= *pTail->pObject )
               {
                  // The following while will quit, and the control will go stright to AddTail.
                  pObjectContainer = NULL;
               }
            }
            else
            {
               // Need to be >= to keep the adding order - if new == old - add the new behind the old.
               if( *ptNewObject >= *pTail->pObject )
               {
                  // The following while will quit, and the control will go stright to AddTail.
                  pObjectContainer = NULL;
               }
            }
         }

         while( NULL != pObjectContainer )
         {
            if( bMax2Head_Min2Tail )
            {
               if( *ptNewObject > *pObjectContainer->pObject )
               {
                  AddMember( ptNewObject, uiIndex );

                  return( ptNewObject );
               }
            }
            else
            {
               if( *ptNewObject < *pObjectContainer->pObject )
               {
                  AddMember( ptNewObject, uiIndex );

                  return( ptNewObject );
               }
            }

            uiIndex++;
            pObjectContainer = pObjectContainer->pNext;
         }

         AddTail( ptNewObject );
      }

      return( ptNewObject );
   }


   // Add new member to the list at position that reflects the value of the element. This function is useful
   // for cretion of sorted lists. The position at which to plug the new element is found by comparing with
   // operator> or operator< for the list type according to the bMax2Head_Min2Tail parameter, therefore in
   // order the new element position to be correct the list state must be sorted accordingly or empty before
   // calling this function. This function will disrupt the list enumeration. In case of equal new and already 
   // existing element the new member will be added after the already existing one.
   template< class tComparator > 
   typeMembers* AddSorted( typename const tComparator& tcComparator, typename typeMembers *ptNewObject )
   {
      if( NULL == ptNewObject )
      {
         return( ptNewObject );
      }

      for( MLISTINDEX uiIndex = 0; uiIndex < uiNumberOfMembers; uiIndex++ )
      {
         // IsOrdered MUST perform comparison with > and < respectivelly, AND NOT with( >= or <=).
         // THIS IS DUE if the class that is compared have more properties than the one currently to compare
         // the "=" part of ">=" & "<=" loaded to compare - (depending if all or single property is compared)
         // at once, so if multy propertyes but one compared at time the "=" part of ">=" & "<=" will swap
         // the order unexpectedly. Less important there will be also unnecessary performance overhead.
         // Also note the order of the parameters.
         if( !tcComparator.IsOrdered( *operator[]( uiIndex ), *(const typeMembers*)ptNewObject ) )
         {
            return( AddMember( ptNewObject, uiIndex ), ptNewObject );
         }
      }

      return( AddTail( ptNewObject ) );
   }


   // Returns the number of the members of the list.
   MLISTINDEX GetCount() const throw()
   {
      return( uiNumberOfMembers );
   }


   // Empties the acceleratin table.
   void DeAccelerate()
   {
      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         ppListArray = NULL;
      }
   }


   // Returns true/false answer of the question.
   bool IsAccelerated() const throw()
   {
      return( NULL != ppListArray );
   }


   // Creates table - array of pointers to the objects contained in the list.
   // Use this member function to prepare the list for fast access - as fast as access to array.
   // Call this function before the first use of operator[], after the list has been modified.
   // The parameter bForceAcceleration makes the acceleration to be performed no matter if the
   // list is already accelerated.
   void Accelerate( bool bForceAcceleration = false ) const throw()
   {
      if( NULL != ppListArray )
      {
         if( !bForceAcceleration )
         {
            return;
         }

         HeapFree( GetProcessHeap(), 0, ppListArray );
      }

      ppListArray = (typeMembers **)HeapAlloc( GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, uiNumberOfMembers * sizeof( typeMembers * ) );

      ObjectContainer* pObjectContainer = pHead;
      MLISTINDEX       uiCounter( 0 );

      while( NULL != pObjectContainer )
      {
         ppListArray[uiCounter++] = pObjectContainer->pObject;
         pObjectContainer = pObjectContainer->pNext;
      }
   }


   // Returns pointer to the array table holding the addresses of the pointers to the
   // elements of the list in order of their indeces.
   const typeMembers** GetArray() const
   {
      if( !IsAccelerated() )
      {
         Accelerate();
      }

      return( ppListArray );
   }

   
   // Returns pointer to the array table holding the addresses of the pointers to the
   // elements of the list in order of their indeces.
   typeMembers** GetArray()
   {
      if( !IsAccelerated() )
      {
         Accelerate();
      }

      return( ppListArray );
   }


   // Fast access to the list members - as fast as access to array. USE Accelerate
   // member function before the first use of operator[], after the list has been mofified.
   // The acces is ZERO based index - as usually in C++ array.
   typeMembers *operator[]( const MLISTINDEX mliIndex ) const throw()
   {
      MASSERT( mliIndex < uiNumberOfMembers );

      if( !IsAccelerated() )
      {
         Accelerate();
      }

      return( ppListArray[mliIndex] );
   }


   // Returns pointer to the object in the Head of the list.
   // If the list is empty the function returns NULL;
   typeMembers *GetHead() const throw()
   {
      return( NULL != pHead ? pHead->pObject : NULL );
   }


   // Returns pointer to the object in the Tail of the list.
   // If the list is empty the function returns NULL;
   typeMembers *GetTail() const throw()
   {
      return( NULL != pTail ? pTail->pObject : NULL );
   }


   // Replaces the zero based index of a list with a new object. The replaced object pointer is returned to the caller.
   typeMembers* ReplaceObject( const MLISTINDEX uiIndex, typeMembers* pNewObject )
   {
      DeAccelerate();

      if( uiIndex >= uiNumberOfMembers )
      {
         MASSERT( FALSE );

         return( NULL );
      }

      return( GetContainer( uiIndex )->ReplaceObject( pNewObject ) );
   }

   
protected:
   // Returns pointer to object member of the list. The uiIndex is zero based index.
   // You can use the eBearing - Next, This and Previous, for relative request.
   // Note that if yo supply invalid index or non eBearing[Next/This/Previous] value the
   // function will return NULL. The function will return NULL also in case you
   // supply eBearing value [Next/This/Previous], but the list enumeration has beed disripte
   // or not being initialized.
   typename MList::ObjectContainer* GetContainer( const MLISTINDEX uiIndex ) const throw()
   {
      if( 0 == uiNumberOfMembers )
      {
         return( NULL );
      }

      if( uiIndex >= uiNumberOfMembers )
      {
         return( NULL );
      }

      ObjectContainer* pObjectContainer = pHead;

      for( MLISTINDEX uiCounter = 0; uiCounter < uiIndex; uiCounter++ )
      {
         pObjectContainer = pObjectContainer->pNext;
      }

      return( pObjectContainer );
   }


   void SwapElements( ObjectContainer* pObjectContainer_A, ObjectContainer* pObjectContainer_B )
   {
      if( (NULL == pObjectContainer_A) || (NULL == pObjectContainer_B) )
      {
         return;
      }


      typeMembers* ptrObjectB = pObjectContainer_B->pObject;

      pObjectContainer_B->pObject = pObjectContainer_A->pObject;
      pObjectContainer_A->pObject = ptrObjectB;

      
      // deallocates acceleration
      if( NULL != ppListArray )
      {
         HeapFree( GetProcessHeap(), 0, ppListArray );
         
         ppListArray = NULL;
      }
   }


public:
   // This method is to help when the typeMembers is mere than one level of indirection to the TYPE.
   // For example if MList< int** > the bool SortList( parameters ) will compare *(int**), i.e. int*.
   // This may be useful in some cases but in other cases I want to compare the int not their locations.
   template< class tComparator > void SortList( const tComparator& tcComparator ) throw()
   {
      if( NULL == pHead )
      {
         return;
      }


      ObjectContainer* pObjectContainer_CurrentPosition = pHead;

      do
      {
         ObjectContainer* pObjectContainer_Extremum   = pObjectContainer_CurrentPosition;
         ObjectContainer* pObjectContainer_Challenger = pObjectContainer_Extremum->pNext;

         if( NULL != pObjectContainer_Challenger )
         {
            do
            {
               // IsOrdered MUST perform comparison with > and < respectively, AND NOT with( >= or <=).
               // THIS IS DUE if the class that is compared have more properties than the one currently to compare
               // the "=" part of ">=" & "<=" loaded to compare - (depending if all or single property is compared)
               // at once, so if multi properties but one compared at time the "=" part of ">=" & "<=" will swap
               // the order unexpectedly. Less important there will be also unnecessary performance overhead.
               // Also note the order of the parameters.
               if( !tcComparator.IsOrdered( *(const typeMembers*)pObjectContainer_Extremum->pObject, *(const typeMembers*)pObjectContainer_Challenger->pObject ) )
               {
                  pObjectContainer_Extremum = pObjectContainer_Challenger;
               }
            }
            while( NULL != (pObjectContainer_Challenger = pObjectContainer_Challenger->pNext) );

            if( pObjectContainer_CurrentPosition != pObjectContainer_Extremum )
            {
               SwapElements( pObjectContainer_CurrentPosition, pObjectContainer_Extremum );
            }
         }
      }
      while( NULL != (pObjectContainer_CurrentPosition = pObjectContainer_CurrentPosition->pNext) );
   }


   class PointerExplorer
   {
   private:
      const MLISTINDEX mliStart;
      const MLISTINDEX mliEnd;
      const typeMembers* ptmSearchFor;

   public:
      MLISTINDEX GetStart() const { return( mliStart ); }
      MLISTINDEX GetEnd()   const { return( mliEnd   ); }

   public:
      PointerExplorer( typename const typeMembers* ptmSearchFor, const MLISTINDEX mliStart, const MLISTINDEX mliEnd ) : ptmSearchFor( ptmSearchFor ), mliStart( mliStart ), mliEnd( mliEnd ) {}

      bool IsMatching( typename const typeMembers& tmMember ) const
      {
         return( ptmSearchFor == &tmMember );
      }
   };


   // Searches the list for member poineting the same object. Returns true/false, and a zero based index of found member when true.
   // Pass pmliFoundZBIndex = NULL when the actual index is not required. The search is performed from the head to the tail starting 
   // from position mliStart (zero based index) and complets with position mliEnd (i.e. inclusive) also zero based index.
   // The tExplorer is a type that must have bool tExplorer::IsMatching( const typeMembers& ) const member. The parameter tSearchFor
   // holds the data that the tExplorer uses for the compare operation within tExplorer::IsMatching( const typeMembers& ) const.
   // The Explorer object must be loaded with the Starting and Ending value for the search within the list, where 0 is the beginning
   // of the list and MLIST_LAST_INDEX is the ultimate end, i.e. Start = 0 - is the first index, and End = MLIST_LAST_INDEX is "End of the List".
   template< class tExplorer > bool Find_H2T( MLISTINDEX* pmliFoundZBIndex, const tExplorer &teExplorer ) const
   {
      MLISTINDEX mliIndex = 0;
      const MLISTINDEX mliStart( teExplorer.GetStart() );
      const MLISTINDEX mliEnd(   teExplorer.GetEnd()   );

      MASSERT( !(mliStart > mliEnd) );

      for( ObjectContainer* pObjectContainer = pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext, mliIndex++ )
      {
         if( mliIndex < mliStart ) { continue; }
         if( mliIndex > mliEnd   ) { break;    }

         if( teExplorer.IsMatching( *pObjectContainer->pObject ) )
         {
            if( NULL != pmliFoundZBIndex )
            {
               *pmliFoundZBIndex = mliIndex;
            }

            return( true );
         }
      }

      return( false );
   }

   
   // Searches the list for member poineting the same object. Returns true/false, and a zero based index of found member when true.
   // Pass pmliFoundZBIndex = NULL when the actual index is not required. The search is performed from the head to the tail starting 
   // from position mliStart (zero based  index) and complets with position mliEnd (i.e. inclusive) also zero based index.
   bool Find_H2T( MLISTINDEX* pmliFoundZBIndex, const typeMembers &tSearchFor, const MLISTINDEX mliStart, const MLISTINDEX mliEnd ) const
   {
      MASSERT( !(mliStart > mliEnd) );

      MLISTINDEX mliIndex = 0;

      for( ObjectContainer* pObjectContainer = pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext, mliIndex++ )
      {
         if( mliIndex < mliStart ) { continue; }
         if( mliIndex > mliEnd   ) { break;    }

         if( tSearchFor == (*pObjectContainer->pObject) )
         {
            if( NULL != pmliFoundZBIndex )
            {
               *pmliFoundZBIndex = mliIndex;
            }

            return( true );
         }
      }

      return( false );
   }


   // Searches the list for member poineting the same object. Returns true/false, and a zero based index of found member when true.
   // Pass pmliFoundZBIndex = NULL when the actual index is not required. The search is performed from the head to the tail starting 
   // from position mliStart (zero based  index) and complets with position mliEnd (i.e. inclusive) also zero based index.
   bool Find_H2T( MLISTINDEX* pmliFoundZBIndex, const typeMembers* ptSearchFor, const MLISTINDEX mliStart, const MLISTINDEX mliEnd ) const
   {
      MASSERT( !(mliStart > mliEnd) );

      MLISTINDEX mliIndex = 0;

      for( ObjectContainer* pObjectContainer = pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext, mliIndex++ )
      {
         if( mliIndex < mliStart ) { continue; }
         if( mliIndex > mliEnd   ) { break;    }

         if( ptSearchFor == pObjectContainer->pObject )
         {
            if( NULL != pmliFoundZBIndex )
            {
               *pmliFoundZBIndex = mliIndex;
            }

            return( true );
         }
      }

      return( false );
   }



   // Searches the list for member smaller the paramer. Returns Zero based index of found member and Invalid in not found.
   // The search is performed from head to tail starting from position mliStart, which is zero based index as well, and
   // compleating with position mliEnd (i.e. inclusive), which is zero based index too.
   bool Find_Smaller_H2T( MLISTINDEX* pmliFoundZBIndex, const typeMembers &tSearchFor, const MLISTINDEX mliStart, const MLISTINDEX mliEnd ) const
   {
      MASSERT( !(mliStart > mliEnd) );

      MLISTINDEX mliIndex = 0;

      for( ObjectContainer* pObjectContainer = pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext, mliIndex++ )
      {
         if( mliIndex < mliStart ) { continue; }
         if( mliIndex > mliEnd   ) { break;    }

         if( tSearchFor > (*pObjectContainer->pObject) )
         {
            if( NULL != pmliFoundZBIndex )
            {
               *pmliFoundZBIndex = mliIndex;
            }

            return( true );
         }
      }

      return( false );
   }


   // Searches the list for member smaller the paramer. Returns Zero based index of found member and Invalid in not found.
   // The search is performed from head to tail starting from position mliStart, which is zero based index as well, and
   // compleating with position mliEnd (i.e. inclusive), which is zero based index too.
   // The Comparator type must have the methods bool IsSmaller( const typeMembers& ), MLISTINDEX GetStart() const; MLISTINDEX GetEnd() const.
   template< class tComparator > bool Find_Smaller_H2T( MLISTINDEX* pmliFoundZBIndex, const tComparator &tcComparator ) const
   {
      MASSERT( !(mliStart > mliEnd) );

      MLISTINDEX mliIndex( 0 );
      const MLISTINDEX mliStart( tcComparator.GetStart() );
      const MLISTINDEX mliEnd(   tcComparator.GetEnd()   );

      for( ObjectContainer* pObjectContainer = pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext, mliIndex++ )
      {
         if( mliIndex < mliStart ) { continue; }
         if( mliIndex > mliEnd   ) { break;    }

         if( tcComparator.IsSmaller( *pObjectContainer->pObject ) )
         {
            if( NULL != pmliFoundZBIndex )
            {
               *pmliFoundZBIndex = mliIndex;
            }

            return( true );
         }
      }

      return( false );
   }


   // The RotateToHead( parameters ) function rotates group of members of the list with given number of positions.
   // The group of members is determined by mliIndexFirst and mliRotationGroupCount parameters, where mliIndexFirst
   // is ZERO based index and mliRotationGroupCount is the number of elements to be rotated. The mliPositionsToShift
   // parameter gives the number of positions that the rotation group members are to be shifted. The shift is performed
   // towards the head of the list. When an element is due to pass beyond the head border (mliIndexFirst) it is set
   // to the end of the rotation group. After call to this function the list may be deaccelerated and the enumeraton
   // invalidated or not depending wether rotation was applied or not.
   //
   // Example: MList< typename >::RotateToHead( 7, 29, 3 );
   // the element that has zerobased index 7 before the function call will be moved to zero based position 33,
   // ZBI{ BC[8] -> AC[34]}; ZBI{ BC[9] -> AC[35]}; ZBI{ BC[10] -> AC[7]}; ....
   //
   // If the mliIndexFirst is out of the limits of the list - no operation will be performed.
   // if mliIndexFirst + mliRotationGroupCount exceeds the limit of the list the rotation group will be determined
   // by mliIndexFirst and the number of available elements after mliIndexFirst so the rotation group will be all
   // elements starting from mliIndexFirst to the tail element of the list inclusive.
   //
   // Note that the MList<template> behave as zero based index.
   void RotateToHead( const MLISTINDEX mliIndexFirst, const MLISTINDEX mliRotationGroupCount, const MLISTINDEX mliPositionsToShift ) throw()
   {
      if( mliIndexFirst >= GetCount() )
      {
         return;
      }
      
      if( 0 == mliRotationGroupCount )
      {
         return;
      }
      
      if( IsEmpty() )
      {
         return;
      }

      // Notmalize the incomming parameters
      *const_cast< MLISTINDEX* >( &mliRotationGroupCount ) = min( mliRotationGroupCount, GetCount() - mliIndexFirst );
      *const_cast< MLISTINDEX* >( &mliPositionsToShift   ) = mliPositionsToShift % mliRotationGroupCount;
      *const_cast< MLISTINDEX* >( &mliIndexFirst         ) = mliIndexFirst + 1;

      if( 0 == mliPositionsToShift )
      {
         return;
      }

      Rotate( mliIndexFirst, mliRotationGroupCount, mliPositionsToShift );
   }


   // The RotateToTail( parameters ) function rotates group of members of the list with given number of positions.
   // The group of members is determined by mliIndexFirst and mliRotationGroupCount parameters, where mliIndexFirst
   // is ZERO based index and mliRotationGroupCount is the number of elements to be rotated. The mliPositionsToShift
   // parameter gives the number of positions that the rotation group members are to be shifted. The shift is performed
   // towards the tail of the list. When an element is due to pass beyond the tail border (mliIndexFirst + mliRotationGroupCount)
   // it is set to the front of the rotation group. After call to this function the list may be deaccelerated and the enumeraton
   // invalidated or not depending wether rotation was applied or not.
   //
   // Example: MList< typename >::RotateToTail( 7, 29, 3 );
   // the element that has zerobased index 7 before the function call will be moved to zero based position 10,
   // ZBI{ BC[8] -> AC[11]}; ZBI{ BC[9] -> AC[12]}; ...; ZBI{ BC[36] -> AC[7]};
   //
   // If the mliIndexFirst is out of the limits of the list - no operation will be performed.
   // if mliIndexFirst + mliRotationGroupCount exceeds the limit of the list the rotation group will be determined
   // by mliIndexFirst and the number of available elements after mliIndexFirst so the rotation group will be all
   // elements starting from mliIndexFirst to the tail element of the list inclusive.
   //
   // Note that the MList<template> behave as zero based index.
   void RotateToTail( const MLISTINDEX mliIndexFirst, const MLISTINDEX mliRotationGroupCount, const MLISTINDEX mliPositionsToShift ) throw()
   {

      if( mliIndexFirst >= GetCount() ) { return; }
      if( 0 == mliRotationGroupCount )         { return; }
      if( IsEmpty() )                          { return; }

      // Notmalize the incomming parameters
      *const_cast< MLISTINDEX* >( &mliRotationGroupCount ) = min( mliRotationGroupCount, GetCount() - mliIndexFirst );
      *const_cast< MLISTINDEX* >( &mliPositionsToShift   ) = mliPositionsToShift % mliRotationGroupCount;
      *const_cast< MLISTINDEX* >( &mliIndexFirst         ) = mliIndexFirst + 1;

      if( 0 == mliPositionsToShift ) { return; }

      Rotate( mliIndexFirst, mliRotationGroupCount, mliRotationGroupCount - mliPositionsToShift );
   }


protected:
   void Rotate( const MLISTINDEX mliIndexFirst, const MLISTINDEX mliRotationGroupCount, const MLISTINDEX mliPositionsToShift ) throw()
   {
      // Add service new Head and tail so whatever the rotation will be in the middle of the list so I will have borders.
      ObjectContainer coServiceHead( NULL, NULL, pHead );
      ObjectContainer coServiceTail( pTail, NULL, NULL );

      if( NULL != pHead ) { pHead->pPrevious = &coServiceHead; }
      if( NULL != pTail ) { pTail->pNext = &coServiceTail; }

      pHead = &coServiceHead;
      pTail = &coServiceTail;


      // Find the last ObjectContainer member not to rotate on the left side.
      ObjectContainer* pObjectContainer_Pre_RotGroup_Member( pHead );

      for( MLISTINDEX mliIndex = 0; mliIndex < mliIndexFirst - 1; mliIndex++ )
      {
         pObjectContainer_Pre_RotGroup_Member = pObjectContainer_Pre_RotGroup_Member->pNext;
      }

      // Find the first ObjectContainer member not to rotate on the right side.
      ObjectContainer* pObjectContainer_Post_RotGroup_Member( pObjectContainer_Pre_RotGroup_Member->pNext );

      for( MLISTINDEX mliIndex = 0; mliIndex < mliRotationGroupCount; mliIndex++ )
      {
         pObjectContainer_Post_RotGroup_Member = pObjectContainer_Post_RotGroup_Member->pNext;
      }

      // Find the first ObjectContainer from the group to be rotated.
      ObjectContainer* pObjectContainer( pObjectContainer_Pre_RotGroup_Member->pNext );

      MList< MList< typeMembers >::ObjectContainer > listObjectContainersA;
      MList< MList< typeMembers >::ObjectContainer > listObjectContainersB;


      for( MLISTINDEX mliIndex = 0; mliIndex < mliPositionsToShift; mliIndex++ )
      {
         listObjectContainersA.AddTail( pObjectContainer );
         pObjectContainer = pObjectContainer->pNext;
      }

      for( MLISTINDEX mliIndex = 0; mliIndex < (mliRotationGroupCount - mliPositionsToShift); mliIndex++ )
      {
         listObjectContainersB.AddTail( pObjectContainer );
         pObjectContainer = pObjectContainer->pNext;
      }


      // Finally put the B and A groups back in the original list
      pObjectContainer = pObjectContainer_Pre_RotGroup_Member;

      while( !listObjectContainersB.IsEmpty() )
      {
         pObjectContainer->pNext = listObjectContainersB.RemoveHead();
         pObjectContainer->pNext->pPrevious = pObjectContainer;

         pObjectContainer = pObjectContainer->pNext;
      }


      while( !listObjectContainersA.IsEmpty() )
      {
         pObjectContainer->pNext = listObjectContainersA.RemoveHead();
         pObjectContainer->pNext->pPrevious = pObjectContainer;

         pObjectContainer = pObjectContainer->pNext;
      }


      pObjectContainer->pNext = pObjectContainer_Post_RotGroup_Member;
      pObjectContainer_Post_RotGroup_Member->pPrevious = pObjectContainer;


      // Remove the service Head and Tail
      pTail = pTail->pPrevious;
      pTail->pNext = NULL;

      pHead = pHead->pNext;
      pHead->pPrevious = NULL;


      // deallocates acceleration
      if( NULL != ppListArray ) { HeapFree( GetProcessHeap(), 0, ppListArray ); ppListArray = NULL; }
   }


public:
   // The AddMembers( parameters ) function removes numbers of members from the donor list and adds them to this.
   // The function returns true if the operation is compleated successfully and false if not. If
   // the function return false none of the both listes are changed. The conditions for failture are
   // incorrect incomming parametes such as:
   // 1. the uiDonorFirst is neither of the predefined ePosition[ Head, Tail, This] nore a valid index value -
   // between 0 and (uiNumberOfMembers - 1) for the donor list.
   // 2. if the parameter uiCountToMove added to the estemated first member to be removed outdistance
   // the number of members in the donor list.
   // 3. the uiInsertAt is neither of the predefined ePosition[ Head, Tail, This] nor a valid index value -
   // between 0 and (uiNumberOfMembers - 1) for the target list.
   // In case of corrects parametrs, the function will insert the moved objects into the targer list
   // at position uiInsertAt. The moved objects will be from position uiIndexFirst from the donor list, and
   // uiCountToMove number of moved elements.
   bool AddMembers( MList< typeMembers > &listDonorMembers, const MLISTINDEX uiDonorFirst, const MLISTINDEX uiCountToMove, const MLISTINDEX uiInsertAt )
   {
      if( uiInsertAt > uiNumberOfMembers )
      {
         return( false );
      }

      if( uiDonorFirst >= listDonorMembers.uiNumberOfMembers )
      {
         return( false );
      }

      if( uiDonorFirst > listDonorMembers.uiNumberOfMembers )
      {
         return( false );
      }

      if( (uiDonorFirst + uiCountToMove) > listDonorMembers.uiNumberOfMembers )
      {
         return( false );
      }

      MLISTINDEX uiCounter;

      for( uiCounter = 0; uiCounter < uiCountToMove; uiCounter++ )
      {
         AddMember( listDonorMembers.RemoveMember( uiDonorFirst + uiCountToMove - 1 - uiCounter ), uiInsertAt );
      }

      return( true );
   }


   bool Serialize( const MHandle& hStream, const MLISTINDEX mliStart, const MLISTINDEX mliCount ) const throw()
   {
      MASSERT( hStream.IsValid() );

      if( !hStream.IsValid() )
      {
         return( false );
      }

      MLISTINDEX mliIndex = 0;
      const MLISTINDEX mliEnd( mliStart + mliCount );

      MASSERT( mliStart <= mliEnd );

      for( ObjectContainer* pObjectContainer = pHead; NULL != pObjectContainer; pObjectContainer = pObjectContainer->pNext, mliIndex++ )
      {
         if( mliIndex < mliStart ) { continue; }
         if( mliIndex > mliEnd   ) { break;    }

         if( !pObjectContainer->pObject->Serialize( hStream ) )
         {
            return( false );
         }
      }

      // this means - return false if starting index is bigger than the list count
      return( mliIndex > mliStart );
   }


   void Extract( MList< typeMembers >& listExtracted, const MLISTINDEX mliFirst, const MLISTINDEX mliCount )
   {
      MASSERT( listExtracted.IsEmpty() );
      MASSERT( mliFirst + mliCount <= GetCount() );

      for( MLISTINDEX mliIndex = 0; mliIndex < mliCount; mliIndex++ )
      {
         listExtracted.AddTail( RemoveMember( mliFirst ) );
      }
   }
};
