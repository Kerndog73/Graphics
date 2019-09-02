//
//  geometry.hpp
//  Animera
//
//  Created by Indi Kernick on 1/9/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#ifndef graphics_geometry_hpp
#define graphics_geometry_hpp

#include <algorithm>

namespace gfx {

struct Size;

struct Point {
  int x = 0;
  int y = 0;
  
  Size size() const noexcept;
  
  constexpr Point operator+(const Point other) const noexcept {
    return {x + other.x, y + other.y};
  }
  // @TODO should this return a Size?
  constexpr Point operator-(const Point other) const noexcept {
    return {x - other.x, y - other.y};
  }
};

struct Size {
  int w = 0;
  int h = 0;
  
  Point point() const noexcept {
    return {w, h};
  }
  
  constexpr bool empty() const noexcept {
    return w <= 0 || h <= 0;
  }
};

inline Size Point::size() const noexcept {
  return {x, y};
}

struct Rect {
  Point p{};
  Size s{};
  
  constexpr Point topLeft() const noexcept {
    return p;
  }
  constexpr Point topRight() const noexcept {
    return {p.x + s.w, p.y};
  }
  constexpr Point bottomRight() const noexcept {
    return {p.x + s.w, p.y + s.h};
  }
  constexpr Point bottomLeft() const noexcept {
    return {p.x, p.y + s.h};
  }
  
  constexpr bool empty() const noexcept {
    return s.empty();
  }
  
  constexpr Rect intersected(const Rect other) const noexcept {
    if (empty() || other.empty()) {
      return {};
    }
    
    const Point topLeft = {
      std::max(p.x, other.p.x),
      std::max(p.y, other.p.y)
    };
    const Point bottomRight = {
      std::min(p.x + s.w, other.p.x + other.s.w),
      std::min(p.y + s.h, other.p.y + other.s.h)
    };
    
    if (bottomRight.x > topLeft.x && bottomRight.y > topLeft.y) {
      return {topLeft, (bottomRight - topLeft).size()};
    } else {
      return {};
    }
  }
};

}

#endif