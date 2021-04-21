/*
 * Mask.hpp
 *
 *  Created on: 20 Apr 2021
 *      Author: mathieu
 */

#ifndef LIB_TEXTURE_MASK_HPP_
#define LIB_TEXTURE_MASK_HPP_

#include <bitset>
#include <cstdint>

namespace ue
{
  typedef std::bitset<1> BitMask;

  class Mask
  {
  public:
    BitMask* rawMask;
    uint32_t width;
    uint32_t height;

    inline BitMask& operator[] (int i);
  };


  inline BitMask& Mask::operator[] (int i)
  {
    return rawMask[i]; //Divide by 8
  }
} /* namespace ue */

#endif /* LIB_TEXTURE_MASK_HPP_ */
