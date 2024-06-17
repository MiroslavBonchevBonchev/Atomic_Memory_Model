//
//
// Common.h: Common definitions used in:
//
//                   ATOMIC MEMORY MODEL - Implementation Examples 'Phase One' and 'Phase Two'
//
// © Copyright 2004 - 2012 by Miroslav Bonchev Bonchev. All rights reserved.
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


#if defined( _DEBUG )
   #define START_DEBUG          __asm { int 3 };

   #if defined( __AFX_H__ )
      #define MASSERT(f)         ASSERT(f)
   #else
      #include                  <assert.h>
      #define MASSERT(f)         assert(f)
   #endif
#else
   #define MASSERT(f)            ((void)0)
#endif
