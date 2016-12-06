/*****************************************
** File:    slVector.cpp
** Project: CMSC 435 Assignment 3, Fall 2015
** Authoer: Jin Hui Xu
** Date:    10/15/2014
** E-mail:  ac39537@umbc.edu
**
**    This file contains the source code of slVector class for assignment 3.
** This program contains the implementations of methods and member functions
** for slVector class.
**
*****************************************/

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Simple Spring Mass System
// -- vector lib
//
// Primary Author: James F. O'Brien (obrienj@cc.gatech.edu)
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Copyright (c) 2003-2005, Regents of the University of California.  All
// rights reserved.
//
// This software is part of the Berkeley Fluid Animation & Simulation
// Toolkit.  The name "Berkeley Fluid Animation & Simulation Toolkit" is
// a trademark of the Regents of the University of California.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//   Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//
//  Redistributions in binary form as an executable program, or part of
//  an executable program must, when run for the first time by a given
//  user, prominently display the above copyright notice, this list of
//  conditions and the following disclaimer.
//
//  Neither the name of the University of California, Berkeley nor the
//  names of its contributors may be used to endorse or promote products
//  derived from this software without specific prior written
//  permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include "slVector.H"
#include "slIO.H"

using std::istream;
using std::ostream;
using std::ios;

//-------------------------------------------------------------------

istream &operator>>(istream &strm,SlVector3 &v) {
  ios::fmtflags orgFlags = strm.setf(ios::skipws);
  eatChar('[',strm);
  strm >> v[0];
  eatChar(',',strm);
  strm >> v[1];
  eatChar(',',strm);
  strm >> v[2];
  eatChar(']',strm);
  strm.flags(orgFlags);
  return strm;
}

ostream &operator<<(ostream &strm,const SlVector3 &v) {
  strm << "[";
  strm << v[0]; strm << ",";
  strm << v[1]; strm << ",";
  strm << v[2]; strm << "]";
  return strm;
}

//-------------------------------------------------------------------


istream &operator>>(istream &strm,SlVector2 &v) {
  ios::fmtflags orgFlags = strm.setf(ios::skipws);
  eatChar('[',strm);
  strm >> v[0];
  eatChar(',',strm);
  strm >> v[1];
  eatChar(']',strm);
  strm.flags(orgFlags);
  return strm;
}
  

ostream &operator<<(ostream &strm,const SlVector2 &v) {
  strm << "[";
  strm << v[0]; strm << ",";
  strm << v[1]; strm << "]";
  return strm;
}

//-------------------------------------------------------------------


