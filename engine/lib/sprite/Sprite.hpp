/*
 * Sprite.hpp
 *
 *  Created on: 20 Apr 2021
 *      Author: mathieu
 */

#ifndef LIB_SPRITE_SPRITE_HPP_
#define LIB_SPRITE_SPRITE_HPP_

#include "../arith.hpp"
#include "../texture.hpp"
#include "../linal.hpp"

namespace ue
{

  class Sprite
  {
  public:
    Texture texture;
    Vector2 scale;
    Vector2 position;
    Real rotation; //Trigonometric angle in rad
  };

} /* namespace ue */

#endif /* LIB_SPRITE_SPRITE_HPP_ */
