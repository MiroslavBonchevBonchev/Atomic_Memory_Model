//
//
// ResId.h: ResId - Windows Resource ID class used in:
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


class ResId    // Resource ID
{
public:
   enum ResIdSPECIAL { Invalid = -1 };

private:
   DWORD dwID;

public:
   explicit ResId()          : dwID( (DWORD)Invalid ) {}
   explicit ResId( int iID ) : dwID( (DWORD)iID ) {}
   explicit ResId( unsigned int uiID ) : dwID( (DWORD)uiID ) {}
   explicit ResId( long lID ) : dwID( (DWORD)lID ) {}
   explicit ResId( unsigned long ulID ) : dwID( (DWORD)ulID ) {}
   ResId( const ResId &ResId ) : dwID( ResId.dwID ) {}
   ResId &operator=( const ResId &ResId ) { dwID = ResId.dwID; return( *this ); }
   ~ResId() {}

   bool IsValid()   const throw() { return( dwID != (DWORD)Invalid ); }
   bool IsInvalid() const throw() { return( dwID == (DWORD)Invalid ); }

   void Invalidate() { dwID = (DWORD)Invalid; }
   void SetWR( ResId ResId ) { dwID = ResId; }

   operator unsigned int() const { MASSERT( Invalid != dwID ); return( (unsigned int) dwID ); }

   DWORD GetId() const throw() { return( dwID ); }

   friend bool operator==( const ResId& ResIdLeft, const ResId& ResIdRight ) throw() { return( ResIdLeft.dwID == ResIdRight.dwID ); }
   friend bool operator!=( const ResId& ResIdLeft, const ResId& ResIdRight ) throw() { return( !operator==( ResIdLeft, ResIdRight ) ); }

   friend bool operator==( const ResIdSPECIAL& ResIdSpec, const ResId&        ResIdRight ) throw() { return( (DWORD)ResIdSpec == ResIdRight.dwID ); }
   friend bool operator==( const ResId&        ResIdLeft, const ResIdSPECIAL& ResIdSpec  ) throw() { return( operator==( ResIdSpec, ResIdLeft ) ); }

   friend bool operator!=( const ResIdSPECIAL& ResIdSpec, const ResId&        ResIdRight ) throw() { return( !operator==( ResIdSpec, ResIdRight ) ); }
   friend bool operator!=( const ResId&        ResIdLeft, const ResIdSPECIAL& ResIdSpec  ) throw() { return( !operator==( ResIdLeft, ResIdSpec  ) ); }
};
