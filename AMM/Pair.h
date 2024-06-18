//
//
// Pair.h: interface for the Pair class.
//
//                   ATOMIC MEMORY MODEL - Implementation Example 'Phase One'
//
// © Copyright 2002 - 2014 by Miroslav Bonchev Bonchev. All rights reserved.
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

#include "MList.h"

namespace PairProperty
{
   template< class T >
   class PointerContainer
   {
   private:
      T*          ptObject;
      // the object is rectreated in the pair by the copy constructor
      // and I want to keep the contained object for the life of the pair
      // so I need this flag to keep track which object and when to destroy.
      // That is not all - the pait may call GetLabel()/GetValue() which will
      // call the copy constructor to create the return value. When that return value
      // is destroyed the instance of my contained object will be released as well,
      // which is a thing I DO NOT WANT. So I have to protect and keep track for
      // THE FIRST COPY CONSTRUCTOR as this is the ponter - BUT I cant do this,
      // so I stand at the point that the COPY CONSTRUCTOR WILL BE ONCE AND FOR EVER
      // for instance. Well I can but it seems not worthy now - I will need a
      // static MList< (void*)Pair, mliRefCount > htbl for all instances.
      const bool  bFromCopyPointerConstructor;

   public:
      PointerContainer( T* ptObject_P )                               throw() : ptObject( ptObject_P ), bFromCopyPointerConstructor( false ) {}
      PointerContainer( const PointerContainer &objPointerContainer ) throw() : ptObject( objPointerContainer.ptObject ), bFromCopyPointerConstructor( true ) {}
      virtual ~PointerContainer() { if( bFromCopyPointerConstructor ) { delete ptObject; } }

      T *GetContainedPtr() { return( ptObject ); }
      T &GetContainedRef() { return( *ptObject ); }

      bool operator==( const PointerContainer &objPointerContainer ) const throw() { return( (*ptObject) == (*objPointerContainer.ptObject) ); }
      bool operator!=( const PointerContainer &objPointerContainer ) const throw() { return( !operator==( objPointerContainer ) ); }
   };
};

template< class Label, class Value >
class Pair
{
public:
   enum Sorting{ sortByLabel, sortByValue, sortUnPrepared };

protected:
   Label tLabel;
   Value tValue;

   mutable Sorting      sSortBy;

public:
   Pair() : tLabel(), tValue() {}
   Pair( const Label& tLabel_P ) : tLabel( tLabel_P ), sSortBy( sortUnPrepared ) {}
   Pair( const Label& tLabel_P, const Value& tValue_P ) : tLabel( tLabel_P ), tValue( tValue_P ), sSortBy( sortUnPrepared ) {}
   Pair( const Label& tLabel_P, const Value& tValue_P, const Sorting& eSortBy_P ) : tLabel( tLabel_P ), tValue( tValue_P ), sSortBy( eSortBy_P ) {}

   Pair( Label *ptLabel_P ) : tLabel( ptLabel_P ), sSortBy( sortUnPrepared ) {}
   Pair( Label *ptLabel_P, Value *ptValue_P ) : tLabel( *ptLabel_P ), tValue( *ptValue_P ), sSortBy( sortUnPrepared ) {}

//   Pair( const Pair< Label, Value >& pair ) : tLabel( pair.tLabel ), tValue( pair.tValue ), sSortBy( sortUnPrepared ) {}
   virtual ~Pair() {}


   Pair< Label, Value >& operator=( const Pair< Label, Value >& pair )
   {
      tLabel = pair.tLabel;
      tValue = pair.tValue;
      
      sSortBy = pair.sSortBy;
      
      return( *this );
   }


   Label& SetLabel( const Label& tNewLabel )
   {
      tLabel = tNewLabel;
      
      return( tLabel );
   }


   Value& SetValue( const Value& tNewValue )
   {
      tValue = tNewValue;
      
      return( tValue );
   }

   
   template< class tLabelType > Label& SetLabel2( const tLabelType& tNewLabel )
   {
      tLabel = tNewLabel;
      
      return( tLabel );
   }
   
   
   template< class tValueType > Value& SetValue2( const tValueType& tNewValue )
   {
      tValue = tNewValue;
      
      return( tValue );
   }


   Label GetLabel() const { return( tLabel ); }
   Value GetValue() const { return( tValue ); }

   Label *GetLabelPtr() { return( &tLabel ); }
   Value *GetValuePtr() { return( &tValue ); }

   const Label *GetLabelPtr() const { return( &tLabel ); }
   const Value *GetValuePtr() const { return( &tValue ); }

   Label &GetLabelRef() { return( tLabel ); }
   Value &GetValueRef() { return( tValue ); }

   template< class ContainerContainedType > ContainerContainedType *GetLabelPtrFromContainer() { return( tLabel.GetContainedPtr() ); }
   template< class ContainerContainedType > ContainerContainedType *GetValuePtrFromContainer() { return( tValue.GetContainedPtr() ); }

   template< class ContainerContainedType > ContainerContainedType &GetLabelRefFromContainer() { return( tLabel.GetContainedRef() ); }
   template< class ContainerContainedType > ContainerContainedType &GetValueRefFromContainer() { return( tValue.GetContainedRef() ); }

   const Label &GetLabelRef() const { return( tLabel ); }
   const Value &GetValueRef() const { return( tValue ); }

   void SetSorting( Sorting sSortBy_P ) const { sSortBy = sSortBy_P; }
   Sorting GetSorting() const { return( sSortBy ); }


   template< class typeTargetPair, class typeConvertorValue >
   typeTargetPair* GetConverted( const typeConvertorValue &tiConvertorValue )
   {
      return( new typeTargetPair( tiConvertorValue.From( GetLabelRef() ), tiConvertorValue.From( GetValueRef() ) ) );
   }

   // Search for first label - return true/false and (tvFound = value if true).
   // Does NOT change the tvFound is the search fails, i.e. return false.
   static bool ScanHashTable( const Label &tlLookFor, Value &tvFound, const MList< Pair< Label, Value > > &listHashTable )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tlLookFor == listHashTable[mliIndex]->GetLabelRef() )
         {
            tvFound = listHashTable[mliIndex]->GetValueRef();

            return( true );
         }
      }

      return( false );
   }

   // Search for first value - return true/false and (tlFound = label if true).
   // Deos NOT change tlFound in the search fails. i.e. return false.
   static bool ScanHashTable( const Value &tvLookFor, const MList< Pair< Label, Value > > &listHashTable, Label &tlFound )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tvLookFor == listHashTable[mliIndex]->GetValueRef() )
         {
            tlFound = listHashTable[mliIndex]->GetLabelRef();

            return( true );
         }
      }

      return( false );
   }

   // Search for first label - return zerobased index/-1 if the search fail and the value if Ok.
   static MLISTINDEX GetFromHashTable( const Label &tlLookFor, Value &tvFound, const MList< Pair< Label, Value > > &listHashTable )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tlLookFor == listHashTable[mliIndex]->GetLabelRef() )
         {
            tvFound = listHashTable[mliIndex]->GetValueRef();

            return( mliIndex );
         }
      }

      return( MLIST_LAST_INDEX );
   }

   // Search for first label - return zerobased index/-1 if the search fail and the value if Ok.
   static MLISTINDEX GetFromHashTable( const Label &tlLookFor, Value **pptvFound, const MList< Pair< Label, Value > > &listHashTable )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tlLookFor == listHashTable[mliIndex]->GetLabelRef() )
         {
            (*pptvFound) = listHashTable[mliIndex]->GetValuePtr();

            return( mliIndex );
         }
      }

      return( MLIST_LAST_INDEX );
   }

   // Search for first value - return zero based index/-1 if the search fail and the label if Ok.
   static MLISTINDEX GetFromHashTable( const Value &tvLookFor, const MList< Pair< Label, Value > > &listHashTable, Label &tlFound )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tvLookFor == listHashTable[mliIndex]->GetValueRef() )
         {
            tlFound = listHashTable[mliIndex]->GetLabelRef();

            return( mliIndex );
         }
      }

      return( MLIST_LAST_INDEX );
   }

   // Search for first label - return zerobased index/-1 if the search fail.
   static MLISTINDEX GetLabelIndexFromHashTable( const Label &tlLookFor, const MList< Pair< Label, Value > > &listHashTable )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tlLookFor == listHashTable[mliIndex]->GetLabelRef() )
         {
            return( mliIndex );
         }
      }

      return( MLIST_LAST_INDEX );
   }

   // Search for first value - return zero based index/-1.
   static MLISTINDEX GetValueIndexFromHashTable( const Value &tvLookFor, const MList< Pair< Label, Value > > &listHashTable )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tvLookFor == listHashTable[mliIndex]->GetValueRef() )
         {
            return( mliIndex );
         }
      }

      return( MLIST_LAST_INDEX );
   }

   // Search for first label - if succseed sets the new value for that pair in the htable and return zerobased index.
   // If fail to find the requested label returns -1 and does not change any value.
   static MLISTINDEX SetValueInHTable( const Label &tlLookFor, const Value &tvNewValue, MList< Pair< Label, Value > > &listHashTable )
   {
      listHashTable.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable.GetCount(); mliIndex++ )
      {
         if( tlLookFor == listHashTable[mliIndex]->GetLabelRef() )
         {
            listHashTable[mliIndex]->SetValue( tvNewValue );

            return( mliIndex );
         }
      }

      return( MLIST_LAST_INDEX );
   }

   // The function colones part of Htable. All nodes (pairs) are exaamined and ithose whose label
   // matches the labes passed as parameter are copioed to new HTable. the new HTable is passed as
   // reference parameter. It will be emptierd in all circumstances first. The function return the
   // number of cloned elements of the source HTable or (it is obviously the same) the number of
   // members of the destination HTable. The function uses the Pair copy constructor therefore the
   // cloned nodes will have the same properties (e.g. sorting). The order of the members in the
   // Target HTable is the same as the order of the source HTable.
   static MLISTINDEX CloneHTable_ByMatchingLabel( const Label &tlToBeReplicated, const MList< Pair< Label, Value > > &listHashTable_Source, MList< Pair< Label, Value > > &listHashTable_Cloned )
   {
      listHashTable_Cloned.Empty();
      listHashTable_Source.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable_Source.GetCount(); mliIndex++ )
      {
         if( tlToBeReplicated == listHashTable_Source[mliIndex]->GetLabelRef() )
         {
            // the operator= will compare the label.
            listHashTable_Cloned.AddTail( new Pair< Label, Value >( *(listHashTable_Source[mliIndex]) ) );
         }
      }

      return( listHashTable_Cloned.GetCount() );
   }

   // The function extracts part of Htable. All nodes (pairs) are exaamined and ithose whose label
   // matches the labes passed as parameter are removed form the source HTable and are added to the
   // extracted HTable. The new (extracted) HTable is passed as reference parameter. It will be
   // emptierd in all circumstances first. The function return the number of extracted elements from
   // the source HTable or (it is obviously the same) the number of members of the destination HTable.
   // The function uses the Pair copy constructor therefore the cloned nodes will have the same properties
   // (e.g. sorting). The order of the members in the Target HTable is the same as the order of the source HTable.
   static MLISTINDEX ExtractFromHTable_ByMatchingLabel( const Label &tlToBeExtracted, MList< Pair< Label, Value > > &listHashTable_Source, MList< Pair< Label, Value > > &listHashTable_Extracted )
   {
      listHashTable_Extracted.Empty();
      listHashTable_Source.Accelerate();

      //     indexes of matching elements
      MList< MLISTINDEX > listIndexes;


      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable_Source.GetCount(); mliIndex++ )
      {
         if( tlToBeExtracted == listHashTable_Source[mliIndex]->GetLabelRef() )
         {
            listIndexes.AddHead( new MLISTINDEX( mliIndex ) );
         }
      }

      listIndexes.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listIndexes.GetCount(); mliIndex++ )
      {
         listHashTable_Extracted.AddHead( listHashTable_Source.RemoveMember( *(listIndexes[mliIndex]) ) );
      }

      listHashTable_Source.Accelerate( true );

      return( listHashTable_Extracted.GetCount() );
   }

   // The function clones vector part of Htable. All labels are examined and the values of those who
   // match the label passed as parameter are copied to the destination vector.
   // The new (extracted) vector is passed as reference parameter. It will be
   // emptierd in all circumstances first. The function return the number of copied elements from
   // or (it is obviously the same) the number of members of the destination vector.
   // The order of the members in the Target HTable is the same as the order of the source HTable.
   static MLISTINDEX CloneVectorFromHTable( const Label &tlToBeExtracted, MList< Value > &listVector_Cloned, MList< Pair< Label, Value > > &listHashTable_Source )
   {
      listVector_Cloned.Empty();
      listHashTable_Source.Accelerate();

      for( MLISTINDEX mliIndex = 0; mliIndex < listHashTable_Source.GetCount(); mliIndex++ )
      {
         if( tlToBeExtracted == listHashTable_Source[mliIndex]->GetLabelRef() )
         {
            listVector_Cloned.AddHead( new Value( listHashTable_Source[mliIndex]->GetValueRef() ) );
         }
      }

      return( listVector_Cloned.GetCount() );
   }

   // The function clones vector of labels from Htable. The first label copied will be the one with
   // zero based index mliFilrst. There will be mliNumberToCopy copied  (cloned) elements.
   // The new (cloned) vector is passed as reference parameter. It will be
   // emptierd in all circumstances first. The function returns the number of copied elements from
   // or (it is obviously the same) the number of members of the destination vector.
   // The order of the members in the Target vector is the same as the order of the source HTable.
   // Use mliNumberToCopy == MLIST_LAST_INDEX to copy all elements starting from mliFirst to the last.
   static MLISTINDEX CloneVectorFromLabels( MList< Label > &listVector_ClonedLabels, const MList< Pair< Label, Value > > &listHashTable_Source, MLISTINDEX mliFirst, MLISTINDEX mliNumberToCopy )
   {
      listVector_ClonedLabels.Empty();
      listHashTable_Source.Accelerate();

      for( MLISTINDEX mliIndex = mliFirst; (mliNumberToCopy != listVector_ClonedLabels.GetCount()) && (mliIndex < listHashTable_Source.GetCount()); mliNumberToCopy++, mliIndex++ )
      {
         listVector_ClonedLabels.AddHead( new Label( listHashTable_Source[mliIndex]->GetLabelRef() ) );
      }

      return( listVector_ClonedLabels.GetCount() );
   }

   // The function clones vector of values from Htable. The cloned vector elements type is such that
   // can be produced from the Value type of the HTable. The Convertor type is supplied as helped for
   // the convesion, e.g HTABLE with Value == CString to be cloned vector MStringEx need a helper LPCTSTR.
   // The first value copied will be the one with zero based index mliFilrst. There will be mliNumberToCopy
   // copied  (cloned) elements. The new (cloned) vector is passed as reference parameter. It will be
   // emptierd in all circumstances first. The function returns the number of copied elements from
   // or (it is obviously the same) the number of members of the destination vector.
   // The order of the members in the Target vector is the same as the order of the source HTable.
   // Use mliNumberToCopy == MLIST_LAST_INDEX to copy all elements starting from mliFirst to the last.
   template< class Export, class Convertor >
   static MLISTINDEX CloneVectorFromValues( MList< Export > &listVector_ClonedValues, const MList< Pair< Label, Value > > &listHashTable_Source, MLISTINDEX mliFirst, MLISTINDEX mliNumberToCopy )
   {
      listVector_ClonedValues.Empty();
      listHashTable_Source.Accelerate();

      for( MLISTINDEX mliIndex = mliFirst; (mliNumberToCopy != listVector_ClonedValues.GetCount()) && (mliIndex < listHashTable_Source.GetCount()); mliNumberToCopy++, mliIndex++ )
      {
         listVector_ClonedValues.AddHead( new Export( (Convertor)listHashTable_Source[mliIndex]->GetValueRef() ) );
      }

      return( listVector_ClonedValues.GetCount() );
   }

   // Trys to add to hash table new pair. If the label already exist the function fails and returns false. The bAddToTail parameter
   // determines where to add the new member in case of success (to the tail if true and to the head if false).
   static bool AddToHashTable( const Pair< Label, Value > &pair, MList< Pair< Label, Value > > &listHTable, const bool bAddToTail = true )
   {
      listHTable.Accelerate();

      if( (MLIST_LAST_INDEX) == Pair< Label, Value >::GetLabelIndexFromHashTable( pair.GetLabelRef(), listHTable ) )
      {
         if( bAddToTail )
         {
            listHTable.AddTail( new Pair< Label, Value >( pair ) );
         }
         else
         {
            listHTable.AddHead( new Pair< Label, Value >( pair ) );
         }

         return( true );
      }

      return( false );
   }

   // Trys to remove from hash table a pair. Searches for the first label that matches the parameter nad removes it.
   // If the HTable is inconsistent (i.e.) there are more than one elements with the same label the later will remain.
   // The function returns pointer to the removed element or NULL if nothing is found and removed.
   static Pair< Label, Value > *RemoveFromHashTable( const Label &tLabel, MList< Pair< Label, Value > > &listHTable )
   {
      listHTable.Accelerate();

      MLISTINDEX mliIndex( Pair< Label, Value >::GetLabelIndexFromHashTable( tLabel, listHTable ) );
      
      if( (MLIST_LAST_INDEX) != (mliIndex = Pair< Label, Value >::GetLabelIndexFromHashTable( tLabel, listHTable )) )
      {
         return( listHTable.RemoveMember( mliIndex ) );
      }

      return( NULL );
   }

   bool operator==( const Pair< Label, Value > &pair ) const { return( ( tValue == pair.tValue ) && ( tLabel == pair.tLabel ) ); }
   bool operator!=( const Pair< Label, Value > &pair ) const { return( !operator==( pair ) ); }

   bool operator<( const Pair< Label, Value > &pair ) const { return( !operator>( pair ) ); }
   bool operator>( const Pair< Label, Value > &pair ) const
   {
      MASSERT( sSortBy != sortUnPrepared );

      if( sortByLabel == sSortBy )
      {
         return( tLabel > pair.tLabel );
      }

      return( tValue > pair.tValue );
   }
   bool operator>=( const Pair< Label, Value > &pair ) const
   {
      MASSERT( sSortBy != sortUnPrepared );

      if( sortByLabel == sSortBy )
      {
         return( tLabel >= pair.tLabel );
      }

      return( tValue >= pair.tValue );
   }
   bool operator<=( const Pair< Label, Value > &pair ) const
   {
      MASSERT( sSortBy != sortUnPrepared );

      if( sortByLabel == sSortBy )
      {
         return( tLabel <= pair.tLabel );
      }

      return( tValue <= pair.tValue );
   }
};
