//
// Copyright 2015 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// texture_format_table:
//   Queries for full textureFormat information based on internalFormat
//

#ifndef LIBANGLE_RENDERER_D3D_D3D11_TEXTUREFORMATTABLE_H_
#define LIBANGLE_RENDERER_D3D_D3D11_TEXTUREFORMATTABLE_H_

#include <map>

#include "common/angleutils.h"
#include "common/platform.h"
#include "libANGLE/renderer/renderer_utils.h"
#include "libANGLE/renderer/d3d/formatutilsD3D.h"
#include "libANGLE/renderer/d3d/d3d11/texture_format_table_autogen.h"

namespace rx
{

struct Renderer11DeviceCaps;

namespace d3d11
{

struct LoadImageFunctionInfo
{
    LoadImageFunctionInfo() : loadFunction(nullptr), requiresConversion(false) {}
    LoadImageFunctionInfo(LoadImageFunction loadFunction, bool requiresConversion)
        : loadFunction(loadFunction), requiresConversion(requiresConversion)
    {
    }

    LoadImageFunction loadFunction;
    bool requiresConversion;
};

struct ANGLEFormatSet final : angle::NonCopyable
{
    ANGLEFormatSet();
    ANGLEFormatSet(ANGLEFormat format,
                   GLenum glInternalFormat,
                   GLenum fboImplementationInternalFormat,
                   DXGI_FORMAT texFormat,
                   DXGI_FORMAT srvFormat,
                   DXGI_FORMAT rtvFormat,
                   DXGI_FORMAT dsvFormat,
                   DXGI_FORMAT blitSRVFormat,
                   ANGLEFormat swizzleFormat,
                   MipGenerationFunction mipGenerationFunction,
                   ColorReadFunction colorReadFunction);

    ANGLEFormat format;

    // The closest matching GL internal format for the DXGI formats this format uses. Note that this
    // may be a different internal format than the one this ANGLE format is used for.
    GLenum glInternalFormat;

    // The format we should report to the GL layer when querying implementation formats from a FBO.
    // This might not be the same as the glInternalFormat, since some DXGI formats don't have
    // matching GL format enums, like BGRA4, BGR5A1 and B5G6R6.
    GLenum fboImplementationInternalFormat;

    DXGI_FORMAT texFormat;
    DXGI_FORMAT srvFormat;
    DXGI_FORMAT rtvFormat;
    DXGI_FORMAT dsvFormat;

    DXGI_FORMAT blitSRVFormat;

    ANGLEFormat swizzleFormat;

    MipGenerationFunction mipGenerationFunction;
    ColorReadFunction colorReadFunction;
};

struct TextureFormat : public angle::NonCopyable
{
    TextureFormat(GLenum internalFormat,
                  const ANGLEFormat angleFormat,
                  InitializeTextureDataFunction internalFormatInitializer,
                  const Renderer11DeviceCaps &deviceCaps);

    const ANGLEFormatSet *formatSet;
    const ANGLEFormatSet *swizzleFormatSet;

    InitializeTextureDataFunction dataInitializerFunction;
    typedef std::map<GLenum, LoadImageFunctionInfo> LoadFunctionMap;

    LoadFunctionMap loadFunctions;
};

const ANGLEFormatSet &GetANGLEFormatSet(ANGLEFormat angleFormat,
                                        const Renderer11DeviceCaps &deviceCaps);

const TextureFormat &GetTextureFormatInfo(GLenum internalformat,
                                          const Renderer11DeviceCaps &renderer11DeviceCaps);

}  // namespace d3d11

}  // namespace rx

#endif  // LIBANGLE_RENDERER_D3D_D3D11_TEXTUREFORMATTABLE_H_
