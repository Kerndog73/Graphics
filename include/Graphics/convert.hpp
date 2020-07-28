//
//  convert.hpp
//  Graphics
//
//  Created by Indiana Kernick on 8/9/19.
//  Copyright © 2019 Indiana Kernick. All rights reserved.
//

#ifndef graphics_convert_hpp
#define graphics_convert_hpp

#include "traits.hpp"
#include "format.hpp"
#include "surface.hpp"
#include "iterator.hpp"

namespace gfx {

template <typename Format, std::uint8_t Threshold>
Surface<std::uint8_t> convertToMono(const Surface<Pixel<Format>> src) noexcept {
  Surface<std::uint8_t> dst{
    src.template dataAs<std::uint8_t>(),
    src.template pitchAs<std::uint8_t>(),
    (src.width() + 7) / 8,
    src.height()
  };
  std::uint8_t *const dstRowEnd = dst.data() + dst.pitch() * dst.height();
  const std::ptrdiff_t width = src.width() / 8;
  const int extraBits = src.width() % 8;
  
  for (std::uint8_t *dstRowIter = dst.data(); dstRowIter != dstRowEnd; dstRowIter += dst.pitch()) {
    std::uint8_t *dstColIter = dstRowIter;
    const auto *srcColIter = reinterpret_cast<Pixel<Format> *>(dstColIter);
    std::uint8_t *const dstPixelEnd = dstColIter + width;
    
    const auto fillByte = [&srcColIter](const int bits) {
      std::uint8_t combinedPixel = 0;
      for (int i = 7; i > 7 - bits; --i) {
        const std::uint8_t gray = Format::color(*srcColIter++).r;
        combinedPixel |= (gray < Threshold ? 0 : 1) << i;
      }
      return combinedPixel;
    };
    
    while (dstColIter != dstPixelEnd) {
      *dstColIter++ = fillByte(8);
    }
    if (extraBits) {
      *dstColIter = fillByte(extraBits);
    }
  }
  
  return dst;
}

template <typename DstFormat, typename SrcFormat>
Surface<Pixel<DstFormat>> convertInplace(
  Surface<Pixel<SrcFormat>> src,
  DstFormat dstFmt,
  SrcFormat srcFmt
) noexcept {
  if constexpr (std::is_same_v<DstFormat, SrcFormat>) {
    return src;
  }

  using SrcPixel = Pixel<SrcFormat>;
  using DstPixel = Pixel<DstFormat>;
  static_assert(sizeof(DstPixel) <= sizeof(SrcPixel));
  
  Surface<DstPixel> dst{
    src.template dataAs<DstPixel>(),
    src.template pitchAs<DstPixel>(),
    src.width(),
    src.height()
  };
  
  for (auto dstRow : range(dst)) {
    const auto *srcColIter = reinterpret_cast<SrcPixel *>(dstRow.begin());
    for (DstPixel &dstPixel : dstRow) {
      dstPixel = dstFmt.pixel(srcFmt.color(*srcColIter++));
    }
  }
  
  return dst;
}

}

#endif
