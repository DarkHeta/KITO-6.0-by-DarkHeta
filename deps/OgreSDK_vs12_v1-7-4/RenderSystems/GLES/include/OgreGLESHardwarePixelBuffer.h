/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2008 Renato Araujo Oliveira Filho <renatox@gmail.com>
Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef __GLESHardwarePixelBuffer_H__
#define __GLESHardwarePixelBuffer_H__

#include "OgreGLESPrerequisites.h"
#include "OgreHardwarePixelBuffer.h"

namespace Ogre {
    class _OgreGLESExport GLESHardwarePixelBuffer: public HardwarePixelBuffer
    {
        protected:
            /// Lock a box
            PixelBox lockImpl(const Image::Box lockBox,  LockOptions options);

            /// Unlock a box
            void unlockImpl(void);

            // Internal buffer; either on-card or in system memory, freed/allocated on demand
            // depending on buffer usage
            PixelBox mBuffer;
            GLenum mGLInternalFormat; // GL internal format
            LockOptions mCurrentLockOptions;

            // Buffer allocation/freeage
            void allocateBuffer();

            void freeBuffer();

            // Upload a box of pixels to this buffer on the card
            virtual void upload(const PixelBox &data, const Image::Box &dest);

            // Download a box of pixels from the card
            virtual void download(const PixelBox &data);

        public:
            /// Should be called by HardwareBufferManager
            GLESHardwarePixelBuffer(size_t mWidth, size_t mHeight, size_t mDepth,
                                  PixelFormat mFormat,
                                  HardwareBuffer::Usage usage);

            /// @copydoc HardwarePixelBuffer::blitFromMemory
            void blitFromMemory(const PixelBox &src, const Image::Box &dstBox);

            /// @copydoc HardwarePixelBuffer::blitToMemory
            void blitToMemory(const Image::Box &srcBox, const PixelBox &dst);

            virtual ~GLESHardwarePixelBuffer();

            /** Bind surface to frame buffer. Needs FBO extension.
            */
            virtual void bindToFramebuffer(GLenum attachment, size_t zoffset);
            GLenum getGLFormat() { return mGLInternalFormat; }
    };

    /** Texture surface.
    */
    class _OgreGLESExport GLESTextureBuffer: public GLESHardwarePixelBuffer
    {
        public:
            /** Texture constructor */
            GLESTextureBuffer(const String &baseName, GLenum target, GLuint id, GLint width, GLint height, GLint internalFormat, 
                            GLenum format, GLint face, GLint level, Usage usage, bool softwareMipmap, bool writeGamma, uint fsaa);
            virtual ~GLESTextureBuffer();

            /// @copydoc HardwarePixelBuffer::bindToFramebuffer
            virtual void bindToFramebuffer(GLenum attachment, size_t zoffset);

            /// @copydoc HardwarePixelBuffer::getRenderTarget
            RenderTexture* getRenderTarget(size_t);

            /// Upload a box of pixels to this buffer on the card
            virtual void upload(const PixelBox &data, const Image::Box &dest);

            /// Download a box of pixels from the card
            virtual void download(const PixelBox &data);

            /// Hardware implementation of blitFromMemory
            virtual void blitFromMemory(const PixelBox &src_orig, const Image::Box &dstBox);

            /// Notify TextureBuffer of destruction of render target
            void _clearSliceRTT(size_t zoffset)
            {
                mSliceTRT[zoffset] = 0;
            }

            // Copy from framebuffer
            void copyFromFramebuffer(size_t zoffset);

            /// @copydoc HardwarePixelBuffer::blit
            void blit(const HardwarePixelBufferSharedPtr &src, const Image::Box &srcBox, const Image::Box &dstBox);
            // Blitting implementation
            void blitFromTexture(GLESTextureBuffer *src, const Image::Box &srcBox, const Image::Box &dstBox);
        
        protected:
            // In case this is a texture level
            GLenum mTarget;
            GLenum mFaceTarget; // same as mTarget in case of GL_TEXTURE_xD, but cubemap face for cubemaps
            GLuint mTextureID;
            GLint mFace;
            GLint mLevel;
            bool mSoftwareMipmap;

            typedef std::vector<RenderTexture*> SliceTRT;
            SliceTRT mSliceTRT;

            static void buildMipmaps(const PixelBox &data);
    };

     /** Renderbuffer surface.  Needs FBO extension.
     */
    class _OgreGLESExport GLESRenderBuffer: public GLESHardwarePixelBuffer
    {
        public:
            GLESRenderBuffer(GLenum format, size_t width, size_t height, GLsizei numSamples);
            virtual ~GLESRenderBuffer();

            /// @copydoc GLHardwarePixelBuffer::bindToFramebuffer
            virtual void bindToFramebuffer(GLenum attachment, size_t zoffset);

        protected:
            // In case this is a render buffer
            GLuint mRenderbufferID;
    };
};

#endif
