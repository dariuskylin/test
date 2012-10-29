/*
 * =====================================================================================
 *
 *       Filename:  func.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/20/2012 11:17:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#ifndef  _FUNC_H_
#define  _FUNC_H_
#if defined(ACE_INLINED)
#define ACE_INLINE  inline
#else
#define ACE_INLINE 
#endif
class functor{
    public:
        int getLength() const;
    private:
        int length;
};
#if defined(ACE_INLINED)
#include"func.inl"
#endif
#endif
