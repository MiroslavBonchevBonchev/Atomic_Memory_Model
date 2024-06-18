//
//
// ExampleUsage.cpp :      Defines the entry point for a console application demonstrating example
//                         usage of the ATOMIC MEMORY MODEL - Example Project Using 'Phase Two'.
//
// © Copyright 2005 - 2024 by Miroslav Bonchev Bonchev. All rights reserved.
//
//
//**********************************************************************************************************************


// Open Source License – The MIT License
//
//
// {your product} uses the: Atomic Memory Model by Miroslav Bonchev Bonchev.
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


#include <stdio.h>
#include "..\AMM\MAtom.h"
#include "..\AMM\StringEx.h"


// Only needed if the MStringEx< tChar > is included in the project.
// Set this handle if you use the string class to load resources.
HMODULE hDefaultTextResourceModule = NULL;


MemoryPH< DWORD > fun1( MemoryPH< DWORD > m );
void fun2( const MemoryPH< DWORD >& m );
void fun3( const MAtom< DWORD >& m );
void fun4( const MAtom< TCHAR >& m );


class A
{
protected:
   const ULONGLONG i;

public:
   A() : i( ::GetTickCount64() )
   {
      printf_s( "A::A()\n" );
      Sleep( 1 );
   }

   virtual ~A()
   {
      printf_s( "A::~A()\n" );
   }

   virtual void print() const
   {
      printf_s( "A::print( %llu )\n", i );
   }
};


class B : public A
{
   const ULONGLONG j;
public:
   B() : j( ::GetTickCount64() )
   {
      printf_s( "B::B()\n" );
      Sleep( 1 );
   }

   virtual ~B()
   {
      printf_s( "B::~B()\n" );
   }

   virtual void print() const
   {
      printf_s( "B::print( %llu - %llu )\n", i, j );
   }
};


int main()
{
   try
   {
      // Create a memory unit integer holding 5 in reference to BYTEs.
      MUnit< BYTE > mu( 5 );


      // Test if it can be converted to memory unit of DWORDs.
      // bool bCanConverttoDword = mu.CanItBe< DWORD >();


      // Create a 256 __int64 memory integer, and convert it to DWORD, i.e. 256 __int64 in DWORDs.
      MUnit< DWORD > m1( MUnit< __int64 >( 256 ).As< DWORD >() );


      // Create an empty memory atom on the process heap, to be holding bytes.
      MemoryPH< BYTE >  m;


      {
         MemoryPH< A >  a( MUnit< A >( 5 ) );
         MemoryPH< B >  b( MUnit< B >( 5 ) );

         for( int i = 0; i < 5; i++ )
         {
            a[i].print();
            b[i].print();
         }
      }


      // Create 2 empty memory atoms to be holding DWORDs and using the process heap memory.
      MemoryPH< DWORD > ma1, ma2;


      // Create a memory atom on the heap holding 3 DWORDs, each initialized with 0xA5A5A5A5.
      MemoryPH< DWORD > ma3( MUnit< DWORD >( 3 ), 0xA5A5A5A5 );


      // Create simple DWORD array on the stack.
      DWORD dwa[3] = { 1, 2, 3 };


      // Create a memory atom coping the content of the stack array.
      MemoryPH< DWORD > ma4( dwa, MUnit< DWORD >( 3 ) );


      // Create a shell memory atom, which refers to the stack array.
      ShellMemory< DWORD > ma5( dwa, MUnit< DWORD >( 3 ) );


      // Call fun1 passing the ma4 to print its content, and assign the returned modified memory to ma2.
      ma2 = fun1( ma4 );


      // Call fun2 passing the ma2 to print its content.
      fun2( ma2 );


      // Call fun3 passing the ma2 to print its content.
      fun3( ma5 );


      // Join the Stack content of ma5 and the heap content of ma2, and assign it to ma2.
      ma2 = ma5 + ma2;


      // Print the m2 content.
      fun3( ma2 );


      // Print the middle 4 DWORDS of m1
      fun3( ma2.SubMemory< DWORD >( MUnit< DWORD >( 1 ), MUnit< DWORD >( 4 ) ) );


      // Create a zero terminated string object using the typedef string. You can also use
      // MStringEx< char > strName( "string" );  to specialize the particular object using
      // wchat_t, TCHAR or another appropriate char definition.
      MStringEx< TCHAR > strName( "Hello World!!!" );


      // Print the strName string.
      fun4( strName );


      // Create a shell memory atom referencing the content of strName except the terminating zero.
      ShellMemory< TCHAR > maName( strName.SubMemory( MUnit< TCHAR >( 0 ), strName.GetSize() - 1 ) );


      // Print the maName memory atom.
      fun4( maName );


      //      const DWORD d( ma1[25] ); // Generate exception - index is outside of memory.


      DWORD dw( 6 );
      int   i = 3;
      unsigned int ui = 4;

      MUnit< DWORD > mu1( 2 );
      MUnit< DWORD > mu2( 3 );
      MUnit< DWORD > mu3( dw );
      MUnit< DWORD > mu4( i );
      MUnit< DWORD > mu5( ui );


      //      mu2 += MUnit< DWORD >( -3 ); // Generate exception - memory units overflow.


      mu5 = MUnit< DWORD >( 3 );


      mu2 = 12 / mu2;


      //      mu4 = mu5 - 3 * (3 + mu2); // Generate exception - negative memory units.
   }
   catch( const MUnitException e )
   {
      printf( TEXT( "Memory unit exception.\n\n" ) );
   }
   catch( MAtomException e )
   {
      printf( TEXT( "Memory atom exception.\n\n" ) );
   }
   catch( MStringExException e )
   {
      printf( TEXT( "String exception.\n\n" ) );
   }
   catch( ... )
   {
      printf( TEXT( "Unknown exception.\n\n" ) );
   }


   return(0);
}




MemoryPH< DWORD > fun1( MemoryPH< DWORD > m )
{
   printf( TEXT( "fun1: memory units = %llu, bytes = %llu   " ), m.GetSize().GetUnits(), m.GetSize().InBytes() );


   for( MUnit< DWORD > muCounter( 0 ); muCounter < m.GetSize(); muCounter++ )
   {
      printf( TEXT( " %d " ), m[muCounter] );
   }


   printf( TEXT( "\n\n" ) );


   // Modify the second DWORD.
   m[1] = 7;


   // Return the modified object.
   return(m);
}


void fun2( const MemoryPH< DWORD >& m )
{
   printf( TEXT( "fun2: memory units = %llu, bytes = %llu   " ), m.GetSize().GetUnits(), m.GetSize().InBytes() );


   for( MUnit< DWORD > muCounter( 0 ); muCounter < m.GetSize(); muCounter++ )
   {
      printf( TEXT( " %d " ), m[muCounter] );
   }


   printf( TEXT( "\n\n" ) );
}


void fun3( const MAtom< DWORD >& m )
{
   printf( TEXT( "fun3: memory units = %llu, bytes = %llu   " ), m.GetSize().GetUnits(), m.GetSize().InBytes() );


   for( MUnit< DWORD > muCounter( 0 ); muCounter < m.GetSize(); muCounter++ )
   {
      printf( TEXT( " %d " ), m[muCounter] );
   }


   printf( TEXT( "\n\n" ) );
}


void fun4( const MAtom< TCHAR >& m )
{
   printf( TEXT( "fun4: %s -> memory units = %llu, bytes = %llu   " ), m.GetMemory(), m.GetSize().GetUnits(), m.GetSize().InBytes() );


   for( MUnit< TCHAR > muCounter( 0 ); muCounter < m.GetSize(); muCounter++ )
   {
      printf( TEXT( " %d " ), m[muCounter] );
   }


   printf( TEXT( "\n\n" ) );
}
