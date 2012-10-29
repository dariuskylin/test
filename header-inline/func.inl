/*
 * =====================================================================================
 *
 *       Filename:  func.inl
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/20/2012 11:20:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
ACE_INLINE int functor::getLength() const{
#if defined(ACE_INLINED)
    return 10;
#else
    return 1000;
#endif
}
