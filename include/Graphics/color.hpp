//
//  color.hpp
//  Graphics
//
//  Created by Indiana Kernick on 1/9/19.
//  Copyright © 2019 Indiana Kernick. All rights reserved.
//

#ifndef graphics_color_hpp
#define graphics_color_hpp

#include <cstdint>

namespace gfx {

struct Color {
  std::uint8_t r, g, b, a;
};

constexpr std::uint8_t gray(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) noexcept {
  // https://poynton.ca/notes/colour_and_gamma/ColorFAQ.html#RTFToC9
  return (54 * r + 183 * g + 19 * b) / 256;
}

constexpr std::uint8_t gray(const Color color) noexcept {
  return gray(color.r, color.g, color.b);
}

}

#endif
