#include "glad.h"
#include "rgss.h"

#define NUM2BYTE(v) ((GLubyte) NUM2CHR(v))

VALUE rb_mGL;
VALUE rb_cSync;

static VALUE rb_glGenTexture(VALUE gl) {
    GLuint id;
    glGenTextures(1, &id);
    return UINT2NUM(id);
}

static VALUE rb_glGenQuery(VALUE gl) {
    GLuint id;
    glGenQueries(1, &id);
    return UINT2NUM(id);
}

static VALUE rb_glGenBuffer(VALUE gl) {
    GLuint id;
    glGenBuffers(1, &id);
    return UINT2NUM(id);
}

static VALUE rb_glGenRenderbuffer(VALUE gl) {
    GLuint id;
    glGenRenderbuffers(1, &id);
    return UINT2NUM(id);
}

static VALUE rb_glGenFramebuffer(VALUE gl) {
    GLuint id;
    glGenFramebuffers(1, &id);
    return UINT2NUM(id);
}

static VALUE rb_glGenSampler(VALUE gl) {
    GLuint id;
    glGenSamplers(1, &id);
    return UINT2NUM(id);
}

static VALUE rb_glGenVertexArray(VALUE gl) {
    GLuint id;
    glGenVertexArrays(1, &id);
    return UINT2NUM(id);
}

static VALUE rb_glDeleteTexture(VALUE gl, VALUE texture) {
    GLuint id = NUM2UINT(texture);
    glDeleteTextures(1, &id);
    return Qnil;
}

static VALUE rb_glDeleteQuery(VALUE gl, VALUE query) {
    GLuint id = NUM2UINT(query);
    glDeleteQueries(1, &id);
    return Qnil;
}

static VALUE rb_glDeleteBuffer(VALUE gl, VALUE buffer) {
    GLuint id = NUM2UINT(buffer);
    glDeleteBuffers(1, &id);
    return Qnil;
}

static VALUE rb_glDeleteRenderbuffer(VALUE gl, VALUE renderbuffer) {
    GLuint id = NUM2UINT(renderbuffer);
    glDeleteRenderbuffers(1, &id);
    return Qnil;
}

static VALUE rb_glDeleteFramebuffer(VALUE gl, VALUE framebuffer) {
    GLuint id = NUM2UINT(framebuffer);
    glDeleteFramebuffers(1, &id);
    return Qnil;
}

static VALUE rb_glDeleteVertexArray(VALUE gl, VALUE array) {
    GLuint id = NUM2UINT(array);
    glDeleteVertexArrays(1, &id);
    return Qnil;
}

static VALUE rb_glDeleteSampler(VALUE gl, VALUE sampler) {
    GLuint id = NUM2UINT(sampler);
    glDeleteSamplers(1, &id);
    return Qnil;
}


























static VALUE rb_glDeleteTextures(VALUE gl, VALUE n, VALUE textures) {
    glDeleteTextures(NUM2INT(n), RB2PTR(textures));
    return Qnil;
}

static VALUE rb_glDeleteQueries(VALUE gl, VALUE n, VALUE ids) {
    glDeleteQueries(NUM2INT(n), RB2PTR(ids));
    return Qnil;
}

static VALUE rb_glDeleteBuffers(VALUE gl, VALUE n, VALUE buffers) {
    glDeleteBuffers(NUM2INT(n), RB2PTR(buffers));
    return Qnil;
}

static VALUE rb_glDeleteRenderbuffers(VALUE gl, VALUE n, VALUE renderbuffers) {
    glDeleteRenderbuffers(NUM2INT(n), RB2PTR(renderbuffers));
    return Qnil;
}

static VALUE rb_glDeleteFramebuffers(VALUE gl, VALUE n, VALUE framebuffers) {
    glDeleteFramebuffers(NUM2INT(n), RB2PTR(framebuffers));
    return Qnil;
}

static VALUE rb_glDeleteVertexArrays(VALUE gl, VALUE n, VALUE arrays) {
    glDeleteVertexArrays(NUM2INT(n), RB2PTR(arrays));
    return Qnil;
}

static VALUE rb_glDeleteSamplers(VALUE gl, VALUE count, VALUE samplers) {
    glDeleteSamplers(NUM2INT(count), RB2PTR(samplers));
    return Qnil;
}

static VALUE rb_glGenTextures(VALUE gl, VALUE n, VALUE textures) {
    glGenTextures(NUM2INT(n), RB2PTR(textures));
    return Qnil;
}

static VALUE rb_glGenQueries(VALUE gl, VALUE n, VALUE ids) {
    glGenQueries(NUM2INT(n), RB2PTR(ids));
    return Qnil;
}

static VALUE rb_glGenBuffers(VALUE gl, VALUE n, VALUE buffers) {
    glGenBuffers(NUM2INT(n), RB2PTR(buffers));
    return Qnil;
}

static VALUE rb_glGenRenderbuffers(VALUE gl, VALUE n, VALUE renderbuffers) {
    glGenRenderbuffers(NUM2INT(n), RB2PTR(renderbuffers));
    return Qnil;
}

static VALUE rb_glGenFramebuffers(VALUE gl, VALUE n, VALUE framebuffers) {
    glGenFramebuffers(NUM2INT(n), RB2PTR(framebuffers));
    return Qnil;
}

static VALUE rb_glGenSamplers(VALUE gl, VALUE count, VALUE samplers) {
    glGenSamplers(NUM2INT(count), RB2PTR(samplers));
    return Qnil;
}

static VALUE rb_glGenVertexArrays(VALUE gl, VALUE n, VALUE arrays) {
    glGenVertexArrays(NUM2INT(n), RB2PTR(arrays));
    return Qnil;
}

static VALUE rb_glCullFace(VALUE gl, VALUE mode) {
    glCullFace(NUM2UINT(mode));
    return Qnil;
}

static VALUE rb_glFrontFace(VALUE gl, VALUE mode) {
    glFrontFace(NUM2UINT(mode));
    return Qnil;
}

static VALUE rb_glHint(VALUE gl, VALUE target, VALUE mode) {
    glHint(NUM2UINT(target), NUM2UINT(mode));
    return Qnil;
}

static VALUE rb_glLineWidth(VALUE gl, VALUE width) {
    glLineWidth(NUM2FLT(width));
    return Qnil;
}

static VALUE rb_glPointSize(VALUE gl, VALUE size) {
    glPointSize(NUM2FLT(size));
    return Qnil;
}

static VALUE rb_glPolygonMode(VALUE gl, VALUE face, VALUE mode) {
    glPolygonMode(NUM2UINT(face), NUM2UINT(mode));
    return Qnil;
}

static VALUE rb_glScissor(VALUE gl, VALUE x, VALUE y, VALUE width, VALUE height) {
    glScissor(NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return Qnil;
}

static VALUE rb_glTexParameterf(VALUE gl, VALUE target, VALUE pname, VALUE param) {
    glTexParameterf(NUM2UINT(target), NUM2UINT(pname), NUM2FLT(param));
    return Qnil;
}

static VALUE rb_glTexParameterfv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glTexParameterfv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glTexParameteri(VALUE gl, VALUE target, VALUE pname, VALUE param) {
    glTexParameteri(NUM2UINT(target), NUM2UINT(pname), NUM2INT(param));
    return Qnil;
}

static VALUE rb_glTexParameteriv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glTexParameteriv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glTexImage1D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE width, VALUE border, VALUE format, VALUE type, VALUE pixels) {
    glTexImage1D(NUM2UINT(target), NUM2INT(level), NUM2INT(internalformat), NUM2INT(width), NUM2INT(border), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glTexImage2D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE width, VALUE height, VALUE border, VALUE format, VALUE type, VALUE pixels) {
    glTexImage2D(NUM2UINT(target), NUM2INT(level), NUM2INT(internalformat), NUM2INT(width), NUM2INT(height), NUM2INT(border), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glDrawBuffer(VALUE gl, VALUE buf) {
    glDrawBuffer(NUM2UINT(buf));
    return Qnil;
}

static VALUE rb_glClear(VALUE gl, VALUE mask) {
    glClear(NUM2UINT(mask));
    return Qnil;
}

static VALUE rb_glClearColor(int argc, VALUE *argv, VALUE gl) {

    if (argc == 1)
    {
        if (RTEST(argv[0]))
        {
            float *color = DATA_PTR(argv[0]);
            glClearColor(color[0], color[1], color[2], color[3]);
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    else if (argc == 4)
    {
        glClearColor(NUM2FLT(argv[0]), NUM2FLT(argv[1]), NUM2FLT(argv[2]), NUM2FLT(argv[3]));
    }
    else
    {
        rb_raise(rb_eArgError, "wrong number of arguments (given 0, expected 1 or 4)");
    }
    
    return Qnil;
}

static VALUE rb_glClearStencil(VALUE gl, VALUE s) {
    glClearStencil(NUM2INT(s));
    return Qnil;
}

static VALUE rb_glClearDepth(VALUE gl, VALUE depth) {
    glClearDepth(NUM2DBL(depth));
    return Qnil;
}

static VALUE rb_glStencilMask(VALUE gl, VALUE mask) {
    glStencilMask(NUM2UINT(mask));
    return Qnil;
}

static VALUE rb_glColorMask(VALUE gl, VALUE red, VALUE green, VALUE blue, VALUE alpha) {
    glColorMask(RTEST(red), RTEST(green), RTEST(blue), RTEST(alpha));
    return Qnil;
}

static VALUE rb_glDepthMask(VALUE gl, VALUE flag) {
    glDepthMask(RTEST(flag));
    return Qnil;
}

static VALUE rb_glDisable(VALUE gl, VALUE cap) {
    glDisable(NUM2UINT(cap));
    return Qnil;
}

static VALUE rb_glEnable(VALUE gl, VALUE cap) {
    glEnable(NUM2UINT(cap));
    return Qnil;
}

static VALUE rb_glFinish(VALUE gl) {
    glFinish();
    return Qnil;
}

static VALUE rb_glFlush(VALUE gl) {
    glFlush();
    return Qnil;
}

static VALUE rb_glBlendFunc(VALUE gl, VALUE sfactor, VALUE dfactor) {
    glBlendFunc(NUM2UINT(sfactor), NUM2UINT(dfactor));
    return Qnil;
}

static VALUE rb_glLogicOp(VALUE gl, VALUE opcode) {
    glLogicOp(NUM2UINT(opcode));
    return Qnil;
}

static VALUE rb_glStencilFunc(VALUE gl, VALUE func, VALUE ref, VALUE mask) {
    glStencilFunc(NUM2UINT(func), NUM2INT(ref), NUM2UINT(mask));
    return Qnil;
}

static VALUE rb_glStencilOp(VALUE gl, VALUE fail, VALUE zfail, VALUE zpass) {
    glStencilOp(NUM2UINT(fail), NUM2UINT(zfail), NUM2UINT(zpass));
    return Qnil;
}

static VALUE rb_glDepthFunc(VALUE gl, VALUE func) {
    glDepthFunc(NUM2UINT(func));
    return Qnil;
}

static VALUE rb_glPixelStoref(VALUE gl, VALUE pname, VALUE param) {
    glPixelStoref(NUM2UINT(pname), NUM2FLT(param));
    return Qnil;
}

static VALUE rb_glPixelStorei(VALUE gl, VALUE pname, VALUE param) {
    glPixelStorei(NUM2UINT(pname), NUM2INT(param));
    return Qnil;
}

static VALUE rb_glReadBuffer(VALUE gl, VALUE src) {
    glReadBuffer(NUM2UINT(src));
    return Qnil;
}

static VALUE rb_glReadPixels(VALUE gl, VALUE x, VALUE y, VALUE width, VALUE height, VALUE format, VALUE type, VALUE pixels) {
    glReadPixels(NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glGetBooleanv(VALUE gl, VALUE pname, VALUE data) {
    glGetBooleanv(NUM2UINT(pname), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetDoublev(VALUE gl, VALUE pname, VALUE data) {
    glGetDoublev(NUM2UINT(pname), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetError(VALUE gl) {
    return UINT2NUM(glGetError());
}

static VALUE rb_glGetFloat(VALUE gl, VALUE pname) {
    GLfloat n;
    glGetFloatv(NUM2UINT(pname), &n);
    return DBL2NUM(n);
}

static VALUE rb_glGetFloatv(VALUE gl, VALUE pname, VALUE data) {
    glGetFloatv(NUM2UINT(pname), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetInteger(VALUE gl, VALUE pname) {
    GLint n;
    glGetIntegerv(NUM2UINT(pname), &n);
    return INT2NUM(n);
}

static VALUE rb_glGetIntegerv(VALUE gl, VALUE pname, VALUE data) {
    glGetIntegerv(NUM2UINT(pname), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetString(VALUE gl, VALUE name) {
    return rb_str_new_cstr((void*) glGetString(NUM2UINT(name)));
}

static VALUE rb_glGetTexImage(VALUE gl, VALUE target, VALUE level, VALUE format, VALUE type, VALUE pixels) {
    glGetTexImage(NUM2UINT(target), NUM2INT(level), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glGetTexParameterfv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetTexParameterfv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetTexParameteriv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetTexParameteriv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetTexLevelParameterfv(VALUE gl, VALUE target, VALUE level, VALUE pname, VALUE params) {
    glGetTexLevelParameterfv(NUM2UINT(target), NUM2INT(level), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetTexLevelParameteriv(VALUE gl, VALUE target, VALUE level, VALUE pname, VALUE params) {
    glGetTexLevelParameteriv(NUM2UINT(target), NUM2INT(level), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glIsEnabled(VALUE gl, VALUE cap) {
    return RB_BOOL(glIsEnabled(NUM2UINT(cap)));
}

static VALUE rb_glDepthRange(VALUE gl, VALUE n, VALUE f) {
    glDepthRange(NUM2DBL(n), NUM2DBL(f));
    return Qnil;
}

static VALUE rb_glViewport(VALUE gl, VALUE x, VALUE y, VALUE width, VALUE height) {
    glViewport(NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return Qnil;
}

static VALUE rb_glDrawArrays(VALUE gl, VALUE mode, VALUE first, VALUE count) {
    glDrawArrays(NUM2UINT(mode), NUM2INT(first), NUM2INT(count));
    return Qnil;
}

static VALUE rb_glDrawElements(VALUE gl, VALUE mode, VALUE count, VALUE type, VALUE indices) {
    glDrawElements(NUM2UINT(mode), NUM2INT(count), NUM2UINT(type), RB2PTR(indices));
    return Qnil;
}

static VALUE rb_glPolygonOffset(VALUE gl, VALUE factor, VALUE units) {
    glPolygonOffset(NUM2FLT(factor), NUM2FLT(units));
    return Qnil;
}

static VALUE rb_glCopyTexImage1D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE x, VALUE y, VALUE width, VALUE border) {
    glCopyTexImage1D(NUM2UINT(target), NUM2INT(level), NUM2UINT(internalformat), NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(border));
    return Qnil;
}

static VALUE rb_glCopyTexImage2D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE x, VALUE y, VALUE width, VALUE height, VALUE border) {
    glCopyTexImage2D(NUM2UINT(target), NUM2INT(level), NUM2UINT(internalformat), NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height), NUM2INT(border));
    return Qnil;
}

static VALUE rb_glCopyTexSubImage1D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE x, VALUE y, VALUE width) {
    glCopyTexSubImage1D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(x), NUM2INT(y), NUM2INT(width));
    return Qnil;
}

static VALUE rb_glCopyTexSubImage2D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE yoffset, VALUE x, VALUE y, VALUE width, VALUE height) {
    glCopyTexSubImage2D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(yoffset), NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return Qnil;
}

static VALUE rb_glTexSubImage1D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE width, VALUE format, VALUE type, VALUE pixels) {
    glTexSubImage1D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(width), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glTexSubImage2D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE yoffset, VALUE width, VALUE height, VALUE format, VALUE type, VALUE pixels) {
    glTexSubImage2D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(yoffset), NUM2INT(width), NUM2INT(height), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glBindTexture(VALUE gl, VALUE target, VALUE texture) {
    glBindTexture(NUM2UINT(target), NUM2UINT(texture));
    return Qnil;
}

static VALUE rb_glIsTexture(VALUE gl, VALUE texture) {
    return RB_BOOL(glIsTexture(NUM2UINT(texture)));
}

static VALUE rb_glDrawRangeElements(VALUE gl, VALUE mode, VALUE start, VALUE finish, VALUE count, VALUE type, VALUE indices) {
    glDrawRangeElements(NUM2UINT(mode), NUM2UINT(start), NUM2UINT(finish), NUM2INT(count), NUM2UINT(type), RB2PTR(indices));
    return Qnil;
}

static VALUE rb_glTexImage3D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE width, VALUE height, VALUE depth, VALUE border, VALUE format, VALUE type, VALUE pixels) {
    glTexImage3D(NUM2UINT(target), NUM2INT(level), NUM2INT(internalformat), NUM2INT(width), NUM2INT(height), NUM2INT(depth), NUM2INT(border), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glTexSubImage3D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE yoffset, VALUE zoffset, VALUE width, VALUE height, VALUE depth, VALUE format, VALUE type, VALUE pixels) {
    glTexSubImage3D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(yoffset), NUM2INT(zoffset), NUM2INT(width), NUM2INT(height), NUM2INT(depth), NUM2UINT(format), NUM2UINT(type), RB2PTR(pixels));
    return Qnil;
}

static VALUE rb_glCopyTexSubImage3D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE yoffset, VALUE zoffset, VALUE x, VALUE y, VALUE width, VALUE height) {
    glCopyTexSubImage3D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(yoffset), NUM2INT(zoffset), NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return Qnil;
}

static VALUE rb_glActiveTexture(VALUE gl, VALUE texture) {
    glActiveTexture(NUM2UINT(texture));
    return Qnil;
}

static VALUE rb_glSampleCoverage(VALUE gl, VALUE value, VALUE invert) {
    glSampleCoverage(NUM2FLT(value), RTEST(invert));
    return Qnil;
}

static VALUE rb_glCompressedTexImage3D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE width, VALUE height, VALUE depth, VALUE border, VALUE imageSize, VALUE data) {
    glCompressedTexImage3D(NUM2UINT(target), NUM2INT(level), NUM2UINT(internalformat), NUM2INT(width), NUM2INT(height), NUM2INT(depth), NUM2INT(border), NUM2INT(imageSize), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glCompressedTexImage2D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE width, VALUE height, VALUE border, VALUE imageSize, VALUE data) {
    glCompressedTexImage2D(NUM2UINT(target), NUM2INT(level), NUM2UINT(internalformat), NUM2INT(width), NUM2INT(height), NUM2INT(border), NUM2INT(imageSize), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glCompressedTexImage1D(VALUE gl, VALUE target, VALUE level, VALUE internalformat, VALUE width, VALUE border, VALUE imageSize, VALUE data) {
    glCompressedTexImage1D(NUM2UINT(target), NUM2INT(level), NUM2UINT(internalformat), NUM2INT(width), NUM2INT(border), NUM2INT(imageSize), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glCompressedTexSubImage3D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE yoffset, VALUE zoffset, VALUE width, VALUE height, VALUE depth, VALUE format, VALUE imageSize, VALUE data) {
    glCompressedTexSubImage3D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(yoffset), NUM2INT(zoffset), NUM2INT(width), NUM2INT(height), NUM2INT(depth), NUM2UINT(format), NUM2INT(imageSize), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glCompressedTexSubImage2D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE yoffset, VALUE width, VALUE height, VALUE format, VALUE imageSize, VALUE data) {
    glCompressedTexSubImage2D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(yoffset), NUM2INT(width), NUM2INT(height), NUM2UINT(format), NUM2INT(imageSize), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glCompressedTexSubImage1D(VALUE gl, VALUE target, VALUE level, VALUE xoffset, VALUE width, VALUE format, VALUE imageSize, VALUE data) {
    glCompressedTexSubImage1D(NUM2UINT(target), NUM2INT(level), NUM2INT(xoffset), NUM2INT(width), NUM2UINT(format), NUM2INT(imageSize), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetCompressedTexImage(VALUE gl, VALUE target, VALUE level, VALUE img) {
    glGetCompressedTexImage(NUM2UINT(target), NUM2INT(level), RB2PTR(img));
    return Qnil;
}

static VALUE rb_glBlendFuncSeparate(VALUE gl, VALUE sfactorRGB, VALUE dfactorRGB, VALUE sfactorAlpha, VALUE dfactorAlpha) {
    glBlendFuncSeparate(NUM2UINT(sfactorRGB), NUM2UINT(dfactorRGB), NUM2UINT(sfactorAlpha), NUM2UINT(dfactorAlpha));
    return Qnil;
}

static VALUE rb_glMultiDrawArrays(VALUE gl, VALUE mode, VALUE first, VALUE count, VALUE drawcount) {
    glMultiDrawArrays(NUM2UINT(mode), RB2PTR(first), RB2PTR(count), NUM2INT(drawcount));
    return Qnil;
}

static VALUE rb_glMultiDrawElements(VALUE gl, VALUE mode, VALUE count, VALUE type, VALUE indices, VALUE drawcount) {
    glMultiDrawElements(NUM2UINT(mode), RB2PTR(count), NUM2UINT(type), RB2PTR(indices), NUM2INT(drawcount));
    return Qnil;
}

static VALUE rb_glPointParameterf(VALUE gl, VALUE pname, VALUE param) {
    glPointParameterf(NUM2UINT(pname), NUM2FLT(param));
    return Qnil;
}

static VALUE rb_glPointParameterfv(VALUE gl, VALUE pname, VALUE params) {
    glPointParameterfv(NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glPointParameteri(VALUE gl, VALUE pname, VALUE param) {
    glPointParameteri(NUM2UINT(pname), NUM2INT(param));
    return Qnil;
}

static VALUE rb_glPointParameteriv(VALUE gl, VALUE pname, VALUE params) {
    glPointParameteriv(NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glBlendColor(VALUE gl, VALUE red, VALUE green, VALUE blue, VALUE alpha) {
    glBlendColor(NUM2FLT(red), NUM2FLT(green), NUM2FLT(blue), NUM2FLT(alpha));
    return Qnil;
}

static VALUE rb_glBlendEquation(VALUE gl, VALUE mode) {
    glBlendEquation(NUM2UINT(mode));
    return Qnil;
}

static VALUE rb_glIsQuery(VALUE gl, VALUE id) {
    return RB_BOOL(glIsQuery(NUM2UINT(id)));
}

static VALUE rb_glBeginQuery(VALUE gl, VALUE target, VALUE id) {
    glBeginQuery(NUM2UINT(target), NUM2UINT(id));
    return Qnil;
}

static VALUE rb_glEndQuery(VALUE gl, VALUE target) {
    glEndQuery(NUM2UINT(target));
    return Qnil;
}

static VALUE rb_glGetQueryiv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetQueryiv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetQueryObjectiv(VALUE gl, VALUE id, VALUE pname, VALUE params) {
    glGetQueryObjectiv(NUM2UINT(id), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetQueryObjectuiv(VALUE gl, VALUE id, VALUE pname, VALUE params) {
    glGetQueryObjectuiv(NUM2UINT(id), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glBindBuffer(VALUE gl, VALUE target, VALUE buffer) {
    glBindBuffer(NUM2UINT(target), NUM2UINT(buffer));
    return Qnil;
}

static VALUE rb_glIsBuffer(VALUE gl, VALUE buffer) {
    return RB_BOOL(glIsBuffer(NUM2UINT(buffer)));
}

static VALUE rb_glBufferData(VALUE gl, VALUE target, VALUE size, VALUE data, VALUE usage) {
    glBufferData(NUM2UINT(target), NUM2SIZET(size), RB2PTR(data), NUM2UINT(usage));
    return Qnil;
}

static VALUE rb_glBufferSubData(VALUE gl, VALUE target, VALUE offset, VALUE size, VALUE data) {
    glBufferSubData(NUM2UINT(target), NUM2LONG(offset), NUM2SIZET(size), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetBufferSubData(VALUE gl, VALUE target, VALUE offset, VALUE size, VALUE data) {
    glGetBufferSubData(NUM2UINT(target), NUM2LONG(offset), NUM2SIZET(size), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glMapBuffer(VALUE gl, VALUE target, VALUE access) {
    return PTR2NUM(glMapBuffer(NUM2UINT(target), NUM2UINT(access)));
}

static VALUE rb_glUnmapBuffer(VALUE gl, VALUE target) {
    return RB_BOOL(glUnmapBuffer(NUM2UINT(target)));
}

static VALUE rb_glGetBufferParameteriv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetBufferParameteriv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetBufferPointerv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetBufferPointerv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glBlendEquationSeparate(VALUE gl, VALUE modeRGB, VALUE modeAlpha) {
    glBlendEquationSeparate(NUM2UINT(modeRGB), NUM2UINT(modeAlpha));
    return Qnil;
}

static VALUE rb_glDrawBuffers(VALUE gl, VALUE n, VALUE bufs) {
    glDrawBuffers(NUM2INT(n), RB2PTR(bufs));
    return Qnil;
}

static VALUE rb_glStencilOpSeparate(VALUE gl, VALUE face, VALUE sfail, VALUE dpfail, VALUE dppass) {
    glStencilOpSeparate(NUM2UINT(face), NUM2UINT(sfail), NUM2UINT(dpfail), NUM2UINT(dppass));
    return Qnil;
}

static VALUE rb_glStencilFuncSeparate(VALUE gl, VALUE face, VALUE func, VALUE ref, VALUE mask) {
    glStencilFuncSeparate(NUM2UINT(face), NUM2UINT(func), NUM2INT(ref), NUM2UINT(mask));
    return Qnil;
}

static VALUE rb_glStencilMaskSeparate(VALUE gl, VALUE face, VALUE mask) {
    glStencilMaskSeparate(NUM2UINT(face), NUM2UINT(mask));
    return Qnil;
}

static VALUE rb_glAttachShader(VALUE gl, VALUE program, VALUE shader) {
    glAttachShader(NUM2UINT(program), NUM2UINT(shader));
    return Qnil;
}

static VALUE rb_glBindAttribLocation(VALUE gl, VALUE program, VALUE index, VALUE name) {
    glBindAttribLocation(NUM2UINT(program), NUM2UINT(index), StringValueCStr(name));
    return Qnil;
}

static VALUE rb_glCompileShader(VALUE gl, VALUE shader) {
    glCompileShader(NUM2UINT(shader));
    return Qnil;
}

static VALUE rb_glCreateProgram(VALUE gl) {
    return UINT2NUM(glCreateProgram());
}

static VALUE rb_glCreateShader(VALUE gl, VALUE type) {
    return UINT2NUM(glCreateShader(NUM2UINT(type)));
}

static VALUE rb_glDeleteProgram(VALUE gl, VALUE program) {
    glDeleteProgram(NUM2UINT(program));
    return Qnil;
}

static VALUE rb_glDeleteShader(VALUE gl, VALUE shader) {
    glDeleteShader(NUM2UINT(shader));
    return Qnil;
}

static VALUE rb_glDetachShader(VALUE gl, VALUE program, VALUE shader) {
    glDetachShader(NUM2UINT(program), NUM2UINT(shader));
    return Qnil;
}

static VALUE rb_glDisableVertexAttribArray(VALUE gl, VALUE index) {
    glDisableVertexAttribArray(NUM2UINT(index));
    return Qnil;
}

static VALUE rb_glEnableVertexAttribArray(VALUE gl, VALUE index) {
    glEnableVertexAttribArray(NUM2UINT(index));
    return Qnil;
}

static VALUE rb_glGetActiveAttrib(VALUE gl, VALUE program, VALUE index, VALUE bufSize, VALUE length, VALUE size, VALUE type, VALUE name) {
    glGetActiveAttrib(NUM2UINT(program), NUM2UINT(index), NUM2INT(bufSize), RB2PTR(length), RB2PTR(size), RB2PTR(type), RB2PTR(name));
    return Qnil;
}

static VALUE rb_glGetActiveUniform(VALUE gl, VALUE program, VALUE index, VALUE bufSize, VALUE length, VALUE size, VALUE type, VALUE name) {
    glGetActiveUniform(NUM2UINT(program), NUM2UINT(index), NUM2INT(bufSize), RB2PTR(length), RB2PTR(size), RB2PTR(type), RB2PTR(name));
    return Qnil;
}

static VALUE rb_glGetAttachedShaders(VALUE gl, VALUE program, VALUE maxCount, VALUE count, VALUE shaders) {
    glGetAttachedShaders(NUM2UINT(program), NUM2INT(maxCount), RB2PTR(count), RB2PTR(shaders));
    return Qnil;
}

static VALUE rb_glGetAttribLocation(VALUE gl, VALUE program, VALUE name) {
    return INT2NUM(glGetAttribLocation(NUM2UINT(program), StringValueCStr(name)));
}

static VALUE rb_glGetProgramiv(VALUE gl, VALUE program, VALUE pname, VALUE params) {
    glGetProgramiv(NUM2UINT(program), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetProgramInfoLog(VALUE gl, VALUE program, VALUE bufSize, VALUE length, VALUE infoLog) {
    glGetProgramInfoLog(NUM2UINT(program), NUM2INT(bufSize), RB2PTR(length), RB2PTR(infoLog));
    return Qnil;
}

static VALUE rb_glGetShaderiv(VALUE gl, VALUE shader, VALUE pname, VALUE params) {
    glGetShaderiv(NUM2UINT(shader), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetShaderInfoLog(VALUE gl, VALUE shader, VALUE bufSize, VALUE length, VALUE infoLog) {
    glGetShaderInfoLog(NUM2UINT(shader), NUM2INT(bufSize), RB2PTR(length), RB2PTR(infoLog));
    return Qnil;
}

static VALUE rb_glGetShaderSource(VALUE gl, VALUE shader, VALUE bufSize, VALUE length, VALUE source) {
    glGetShaderSource(NUM2UINT(shader), NUM2INT(bufSize), RB2PTR(length), RB2PTR(source));
    return Qnil;
}

static VALUE rb_glGetUniformLocation(VALUE gl, VALUE program, VALUE name) {
    return INT2NUM(glGetUniformLocation(NUM2UINT(program), StringValueCStr(name)));
}

static VALUE rb_glGetUniformfv(VALUE gl, VALUE program, VALUE location, VALUE params) {
    glGetUniformfv(NUM2UINT(program), NUM2INT(location), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetUniformiv(VALUE gl, VALUE program, VALUE location, VALUE params) {
    glGetUniformiv(NUM2UINT(program), NUM2INT(location), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetVertexAttribdv(VALUE gl, VALUE index, VALUE pname, VALUE params) {
    glGetVertexAttribdv(NUM2UINT(index), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetVertexAttribfv(VALUE gl, VALUE index, VALUE pname, VALUE params) {
    glGetVertexAttribfv(NUM2UINT(index), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetVertexAttribiv(VALUE gl, VALUE index, VALUE pname, VALUE params) {
    glGetVertexAttribiv(NUM2UINT(index), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetVertexAttribPointerv(VALUE gl, VALUE index, VALUE pname, VALUE pointer) {
    glGetVertexAttribPointerv(NUM2UINT(index), NUM2UINT(pname), RB2PTR(pointer));
    return Qnil;
}

static VALUE rb_glIsProgram(VALUE gl, VALUE program) {
    return RB_BOOL(glIsProgram(NUM2UINT(program)));
}

static VALUE rb_glIsShader(VALUE gl, VALUE shader) {
    return RB_BOOL(glIsShader(NUM2UINT(shader)));
}

static VALUE rb_glLinkProgram(VALUE gl, VALUE program) {
    glLinkProgram(NUM2UINT(program));
    return Qnil;
}

static VALUE rb_glShaderSource(VALUE gl, VALUE shader, VALUE string) {
    int len = RSTRING_LEN(string);
    const char *src = StringValueCStr(string);
    glShaderSource(NUM2UINT(shader), 1, &src, &len);
    return Qnil;
}

static VALUE rb_glUseProgram(VALUE gl, VALUE program) {
    glUseProgram(NUM2UINT(program));
    return Qnil;
}

static VALUE rb_glUniform1f(VALUE gl, VALUE location, VALUE v0) {
    glUniform1f(NUM2INT(location), NUM2FLT(v0));
    return Qnil;
}

static VALUE rb_glUniform2f(VALUE gl, VALUE location, VALUE v0, VALUE v1) {
    glUniform2f(NUM2INT(location), NUM2FLT(v0), NUM2FLT(v1));
    return Qnil;
}

static VALUE rb_glUniform3f(VALUE gl, VALUE location, VALUE v0, VALUE v1, VALUE v2) {
    glUniform3f(NUM2INT(location), NUM2FLT(v0), NUM2FLT(v1), NUM2FLT(v2));
    return Qnil;
}

static VALUE rb_glUniform4f(VALUE gl, VALUE location, VALUE v0, VALUE v1, VALUE v2, VALUE v3) {
    glUniform4f(NUM2INT(location), NUM2FLT(v0), NUM2FLT(v1), NUM2FLT(v2), NUM2FLT(v3));
    return Qnil;
}

static VALUE rb_glUniform1i(VALUE gl, VALUE location, VALUE v0) {
    glUniform1i(NUM2INT(location), NUM2INT(v0));
    return Qnil;
}

static VALUE rb_glUniform2i(VALUE gl, VALUE location, VALUE v0, VALUE v1) {
    glUniform2i(NUM2INT(location), NUM2INT(v0), NUM2INT(v1));
    return Qnil;
}

static VALUE rb_glUniform3i(VALUE gl, VALUE location, VALUE v0, VALUE v1, VALUE v2) {
    glUniform3i(NUM2INT(location), NUM2INT(v0), NUM2INT(v1), NUM2INT(v2));
    return Qnil;
}

static VALUE rb_glUniform4i(VALUE gl, VALUE location, VALUE v0, VALUE v1, VALUE v2, VALUE v3) {
    glUniform4i(NUM2INT(location), NUM2INT(v0), NUM2INT(v1), NUM2INT(v2), NUM2INT(v3));
    return Qnil;
}

static VALUE rb_glUniform1fv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform1fv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform2fv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform2fv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform3fv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform3fv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform4fv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform4fv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform1iv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform1iv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform2iv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform2iv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform3iv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform3iv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform4iv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform4iv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix2fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix2fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix3fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix3fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix4fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix4fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glValidateProgram(VALUE gl, VALUE program) {
    glValidateProgram(NUM2UINT(program));
    return Qnil;
}

static VALUE rb_glVertexAttrib1d(VALUE gl, VALUE index, VALUE x) {
    glVertexAttrib1d(NUM2UINT(index), NUM2DBL(x));
    return Qnil;
}

static VALUE rb_glVertexAttrib1dv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib1dv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib1f(VALUE gl, VALUE index, VALUE x) {
    glVertexAttrib1f(NUM2UINT(index), NUM2FLT(x));
    return Qnil;
}

static VALUE rb_glVertexAttrib1fv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib1fv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib1s(VALUE gl, VALUE index, VALUE x) {
    glVertexAttrib1s(NUM2UINT(index), NUM2SHORT(x));
    return Qnil;
}

static VALUE rb_glVertexAttrib1sv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib1sv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib2d(VALUE gl, VALUE index, VALUE x, VALUE y) {
    glVertexAttrib2d(NUM2UINT(index), NUM2DBL(x), NUM2DBL(y));
    return Qnil;
}

static VALUE rb_glVertexAttrib2dv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib2dv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib2f(VALUE gl, VALUE index, VALUE x, VALUE y) {
    glVertexAttrib2f(NUM2UINT(index), NUM2FLT(x), NUM2FLT(y));
    return Qnil;
}

static VALUE rb_glVertexAttrib2fv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib2fv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib2s(VALUE gl, VALUE index, VALUE x, VALUE y) {
    glVertexAttrib2s(NUM2UINT(index), NUM2SHORT(x), NUM2SHORT(y));
    return Qnil;
}

static VALUE rb_glVertexAttrib2sv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib2sv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib3d(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z) {
    glVertexAttrib3d(NUM2UINT(index), NUM2DBL(x), NUM2DBL(y), NUM2DBL(z));
    return Qnil;
}

static VALUE rb_glVertexAttrib3dv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib3dv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib3f(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z) {
    glVertexAttrib3f(NUM2UINT(index), NUM2FLT(x), NUM2FLT(y), NUM2FLT(z));
    return Qnil;
}

static VALUE rb_glVertexAttrib3fv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib3fv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib3s(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z) {
    glVertexAttrib3s(NUM2UINT(index), NUM2SHORT(x), NUM2SHORT(y), NUM2SHORT(z));
    return Qnil;
}

static VALUE rb_glVertexAttrib3sv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib3sv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4Nbv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4Nbv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4Niv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4Niv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4Nsv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4Nsv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4Nub(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z, VALUE w) {
    glVertexAttrib4Nub(NUM2UINT(index), NUM2BYTE(x), NUM2BYTE(y), NUM2BYTE(z), NUM2BYTE(w));
    return Qnil;
}

static VALUE rb_glVertexAttrib4Nubv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4Nubv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4Nuiv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4Nuiv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4Nusv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4Nusv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4bv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4bv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4d(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z, VALUE w) {
    glVertexAttrib4d(NUM2UINT(index), NUM2DBL(x), NUM2DBL(y), NUM2DBL(z), NUM2DBL(w));
    return Qnil;
}

static VALUE rb_glVertexAttrib4dv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4dv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4f(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z, VALUE w) {
    glVertexAttrib4f(NUM2UINT(index), NUM2FLT(x), NUM2FLT(y), NUM2FLT(z), NUM2FLT(w));
    return Qnil;
}

static VALUE rb_glVertexAttrib4fv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4fv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4iv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4iv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4s(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z, VALUE w) {
    glVertexAttrib4s(NUM2UINT(index), NUM2SHORT(x), NUM2SHORT(y), NUM2SHORT(z), NUM2SHORT(w));
    return Qnil;
}

static VALUE rb_glVertexAttrib4sv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4sv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4ubv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4ubv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4uiv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4uiv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttrib4usv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttrib4usv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribPointer(VALUE gl, VALUE index, VALUE size, VALUE type, VALUE normalized, VALUE stride, VALUE pointer) {
    glVertexAttribPointer(NUM2UINT(index), NUM2INT(size), NUM2UINT(type), RTEST(normalized), NUM2INT(stride), RB2PTR(pointer));
    return Qnil;
}

static VALUE rb_glUniformMatrix2x3fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix2x3fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix3x2fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix3x2fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix2x4fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix2x4fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix4x2fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix4x2fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix3x4fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix3x4fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniformMatrix4x3fv(VALUE gl, VALUE location, VALUE count, VALUE transpose, VALUE value) {
    glUniformMatrix4x3fv(NUM2INT(location), NUM2INT(count), RTEST(transpose), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glColorMaski(VALUE gl, VALUE index, VALUE r, VALUE g, VALUE b, VALUE a) {
    glColorMaski(NUM2UINT(index), RTEST(r), RTEST(g), RTEST(b), RTEST(a));
    return Qnil;
}

static VALUE rb_glGetBooleani_v(VALUE gl, VALUE target, VALUE index, VALUE data) {
    glGetBooleani_v(NUM2UINT(target), NUM2UINT(index), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetIntegeri_v(VALUE gl, VALUE target, VALUE index, VALUE data) {
    glGetIntegeri_v(NUM2UINT(target), NUM2UINT(index), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glEnablei(VALUE gl, VALUE target, VALUE index) {
    glEnablei(NUM2UINT(target), NUM2UINT(index));
    return Qnil;
}

static VALUE rb_glDisablei(VALUE gl, VALUE target, VALUE index) {
    glDisablei(NUM2UINT(target), NUM2UINT(index));
    return Qnil;
}

static VALUE rb_glIsEnabledi(VALUE gl, VALUE target, VALUE index) {
    return RB_BOOL(glIsEnabledi(NUM2UINT(target), NUM2UINT(index)));
}

static VALUE rb_glBeginTransformFeedback(VALUE gl, VALUE primitiveMode) {
    glBeginTransformFeedback(NUM2UINT(primitiveMode));
    return Qnil;
}

static VALUE rb_glEndTransformFeedback(VALUE gl) {
    glEndTransformFeedback();
    return Qnil;
}

static VALUE rb_glBindBufferRange(VALUE gl, VALUE target, VALUE index, VALUE buffer, VALUE offset, VALUE size) {
    glBindBufferRange(NUM2UINT(target), NUM2UINT(index), NUM2UINT(buffer), NUM2LONG(offset), NUM2SIZET(size));
    return Qnil;
}

static VALUE rb_glBindBufferBase(VALUE gl, VALUE target, VALUE index, VALUE buffer) {
    glBindBufferBase(NUM2UINT(target), NUM2UINT(index), NUM2UINT(buffer));
    return Qnil;
}

static VALUE rb_glTransformFeedbackVaryings(VALUE gl, VALUE program, VALUE count, VALUE varyings, VALUE bufferMode) {
    glTransformFeedbackVaryings(NUM2UINT(program), NUM2INT(count), RB2PTR(varyings), NUM2UINT(bufferMode));
    return Qnil;
}

static VALUE rb_glGetTransformFeedbackVarying(VALUE gl, VALUE program, VALUE index, VALUE bufSize, VALUE length, VALUE size, VALUE type, VALUE name) {
    glGetTransformFeedbackVarying(NUM2UINT(program), NUM2UINT(index), NUM2INT(bufSize), RB2PTR(length), RB2PTR(size), RB2PTR(type), RB2PTR(name));
    return Qnil;
}

static VALUE rb_glClampColor(VALUE gl, VALUE target, VALUE clamp) {
    glClampColor(NUM2UINT(target), NUM2UINT(clamp));
    return Qnil;
}

static VALUE rb_glBeginConditionalRender(VALUE gl, VALUE id, VALUE mode) {
    glBeginConditionalRender(NUM2UINT(id), NUM2UINT(mode));
    return Qnil;
}

static VALUE rb_glEndConditionalRender(VALUE gl) {
    glEndConditionalRender();
    return Qnil;
}

static VALUE rb_glVertexAttribIPointer(VALUE gl, VALUE index, VALUE size, VALUE type, VALUE stride, VALUE pointer) {
    glVertexAttribIPointer(NUM2UINT(index), NUM2INT(size), NUM2UINT(type), NUM2INT(stride), RB2PTR(pointer));
    return Qnil;
}

static VALUE rb_glGetVertexAttribIiv(VALUE gl, VALUE index, VALUE pname, VALUE params) {
    glGetVertexAttribIiv(NUM2UINT(index), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetVertexAttribIuiv(VALUE gl, VALUE index, VALUE pname, VALUE params) {
    glGetVertexAttribIuiv(NUM2UINT(index), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glVertexAttribI1i(VALUE gl, VALUE index, VALUE x) {
    glVertexAttribI1i(NUM2UINT(index), NUM2INT(x));
    return Qnil;
}

static VALUE rb_glVertexAttribI2i(VALUE gl, VALUE index, VALUE x, VALUE y) {
    glVertexAttribI2i(NUM2UINT(index), NUM2INT(x), NUM2INT(y));
    return Qnil;
}

static VALUE rb_glVertexAttribI3i(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z) {
    glVertexAttribI3i(NUM2UINT(index), NUM2INT(x), NUM2INT(y), NUM2INT(z));
    return Qnil;
}

static VALUE rb_glVertexAttribI4i(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z, VALUE w) {
    glVertexAttribI4i(NUM2UINT(index), NUM2INT(x), NUM2INT(y), NUM2INT(z), NUM2INT(w));
    return Qnil;
}

static VALUE rb_glVertexAttribI1ui(VALUE gl, VALUE index, VALUE x) {
    glVertexAttribI1ui(NUM2UINT(index), NUM2UINT(x));
    return Qnil;
}

static VALUE rb_glVertexAttribI2ui(VALUE gl, VALUE index, VALUE x, VALUE y) {
    glVertexAttribI2ui(NUM2UINT(index), NUM2UINT(x), NUM2UINT(y));
    return Qnil;
}

static VALUE rb_glVertexAttribI3ui(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z) {
    glVertexAttribI3ui(NUM2UINT(index), NUM2UINT(x), NUM2UINT(y), NUM2UINT(z));
    return Qnil;
}

static VALUE rb_glVertexAttribI4ui(VALUE gl, VALUE index, VALUE x, VALUE y, VALUE z, VALUE w) {
    glVertexAttribI4ui(NUM2UINT(index), NUM2UINT(x), NUM2UINT(y), NUM2UINT(z), NUM2UINT(w));
    return Qnil;
}

static VALUE rb_glVertexAttribI1iv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI1iv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI2iv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI2iv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI3iv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI3iv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI4iv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI4iv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI1uiv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI1uiv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI2uiv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI2uiv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI3uiv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI3uiv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI4uiv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI4uiv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI4bv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI4bv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI4sv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI4sv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI4ubv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI4ubv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glVertexAttribI4usv(VALUE gl, VALUE index, VALUE v) {
    glVertexAttribI4usv(NUM2UINT(index), RB2PTR(v));
    return Qnil;
}

static VALUE rb_glGetUniformuiv(VALUE gl, VALUE program, VALUE location, VALUE params) {
    glGetUniformuiv(NUM2UINT(program), NUM2INT(location), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glBindFragDataLocation(VALUE gl, VALUE program, VALUE color, VALUE name) {
    glBindFragDataLocation(NUM2UINT(program), NUM2UINT(color), StringValueCStr(name));
    return Qnil;
}

static VALUE rb_glGetFragDataLocation(VALUE gl, VALUE program, VALUE name) {
    return INT2NUM(glGetFragDataLocation(NUM2UINT(program), StringValueCStr(name)));
}

static VALUE rb_glUniform1ui(VALUE gl, VALUE location, VALUE v0) {
    glUniform1ui(NUM2INT(location), NUM2UINT(v0));
    return Qnil;
}

static VALUE rb_glUniform2ui(VALUE gl, VALUE location, VALUE v0, VALUE v1) {
    glUniform2ui(NUM2INT(location), NUM2UINT(v0), NUM2UINT(v1));
    return Qnil;
}

static VALUE rb_glUniform3ui(VALUE gl, VALUE location, VALUE v0, VALUE v1, VALUE v2) {
    glUniform3ui(NUM2INT(location), NUM2UINT(v0), NUM2UINT(v1), NUM2UINT(v2));
    return Qnil;
}

static VALUE rb_glUniform4ui(VALUE gl, VALUE location, VALUE v0, VALUE v1, VALUE v2, VALUE v3) {
    glUniform4ui(NUM2INT(location), NUM2UINT(v0), NUM2UINT(v1), NUM2UINT(v2), NUM2UINT(v3));
    return Qnil;
}

static VALUE rb_glUniform1uiv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform1uiv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform2uiv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform2uiv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform3uiv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform3uiv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glUniform4uiv(VALUE gl, VALUE location, VALUE count, VALUE value) {
    glUniform4uiv(NUM2INT(location), NUM2INT(count), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glTexParameterIiv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glTexParameterIiv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glTexParameterIuiv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glTexParameterIuiv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetTexParameterIiv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetTexParameterIiv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetTexParameterIuiv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetTexParameterIuiv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glClearBufferiv(VALUE gl, VALUE buffer, VALUE drawbuffer, VALUE value) {
    glClearBufferiv(NUM2UINT(buffer), NUM2INT(drawbuffer), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glClearBufferuiv(VALUE gl, VALUE buffer, VALUE drawbuffer, VALUE value) {
    glClearBufferuiv(NUM2UINT(buffer), NUM2INT(drawbuffer), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glClearBufferfv(VALUE gl, VALUE buffer, VALUE drawbuffer, VALUE value) {
    glClearBufferfv(NUM2UINT(buffer), NUM2INT(drawbuffer), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glClearBufferfi(VALUE gl, VALUE buffer, VALUE drawbuffer, VALUE depth, VALUE stencil) {
    glClearBufferfi(NUM2UINT(buffer), NUM2INT(drawbuffer), NUM2FLT(depth), NUM2INT(stencil));
    return Qnil;
}

static VALUE rb_glGetStringi(VALUE gl, VALUE name, VALUE index) {
    return rb_str_new_cstr((void*) glGetStringi(NUM2UINT(name), NUM2UINT(index)));
}

static VALUE rb_glIsRenderbuffer(VALUE gl, VALUE renderbuffer) {
    return RB_BOOL(glIsRenderbuffer(NUM2UINT(renderbuffer)));
}

static VALUE rb_glBindRenderbuffer(VALUE gl, VALUE target, VALUE renderbuffer) {
    glBindRenderbuffer(NUM2UINT(target), NUM2UINT(renderbuffer));
    return Qnil;
}

static VALUE rb_glRenderbufferStorage(VALUE gl, VALUE target, VALUE internalformat, VALUE width, VALUE height) {
    glRenderbufferStorage(NUM2UINT(target), NUM2UINT(internalformat), NUM2INT(width), NUM2INT(height));
    return Qnil;
}

static VALUE rb_glGetRenderbufferParameteriv(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetRenderbufferParameteriv(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glIsFramebuffer(VALUE gl, VALUE framebuffer) {
    return RB_BOOL(glIsFramebuffer(NUM2UINT(framebuffer)));
}

static VALUE rb_glBindFramebuffer(VALUE gl, VALUE target, VALUE framebuffer) {
    glBindFramebuffer(NUM2UINT(target), NUM2UINT(framebuffer));
    return Qnil;
}

static VALUE rb_glCheckFramebufferStatus(VALUE gl, VALUE target) {
    return UINT2NUM(glCheckFramebufferStatus(NUM2UINT(target)));
}

static VALUE rb_glFramebufferTexture1D(VALUE gl, VALUE target, VALUE attachment, VALUE textarget, VALUE texture, VALUE level) {
    glFramebufferTexture1D(NUM2UINT(target), NUM2UINT(attachment), NUM2UINT(textarget), NUM2UINT(texture), NUM2INT(level));
    return Qnil;
}

static VALUE rb_glFramebufferTexture2D(VALUE gl, VALUE target, VALUE attachment, VALUE textarget, VALUE texture, VALUE level) {
    glFramebufferTexture2D(NUM2UINT(target), NUM2UINT(attachment), NUM2UINT(textarget), NUM2UINT(texture), NUM2INT(level));
    return Qnil;
}

static VALUE rb_glFramebufferTexture3D(VALUE gl, VALUE target, VALUE attachment, VALUE textarget, VALUE texture, VALUE level, VALUE zoffset) {
    glFramebufferTexture3D(NUM2UINT(target), NUM2UINT(attachment), NUM2UINT(textarget), NUM2UINT(texture), NUM2INT(level), NUM2INT(zoffset));
    return Qnil;
}

static VALUE rb_glFramebufferRenderbuffer(VALUE gl, VALUE target, VALUE attachment, VALUE renderbuffertarget, VALUE renderbuffer) {
    glFramebufferRenderbuffer(NUM2UINT(target), NUM2UINT(attachment), NUM2UINT(renderbuffertarget), NUM2UINT(renderbuffer));
    return Qnil;
}

static VALUE rb_glGetFramebufferAttachmentParameteriv(VALUE gl, VALUE target, VALUE attachment, VALUE pname, VALUE params) {
    glGetFramebufferAttachmentParameteriv(NUM2UINT(target), NUM2UINT(attachment), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGenerateMipmap(VALUE gl, VALUE target) {
    glGenerateMipmap(NUM2UINT(target));
    return Qnil;
}

static VALUE rb_glBlitFramebuffer(VALUE gl, VALUE srcX0, VALUE srcY0, VALUE srcX1, VALUE srcY1, VALUE dstX0, VALUE dstY0, VALUE dstX1, VALUE dstY1, VALUE mask, VALUE filter) {
    glBlitFramebuffer(NUM2INT(srcX0), NUM2INT(srcY0), NUM2INT(srcX1), NUM2INT(srcY1), NUM2INT(dstX0), NUM2INT(dstY0), NUM2INT(dstX1), NUM2INT(dstY1), NUM2UINT(mask), NUM2UINT(filter));
    return Qnil;
}

static VALUE rb_glRenderbufferStorageMultisample(VALUE gl, VALUE target, VALUE samples, VALUE internalformat, VALUE width, VALUE height) {
    glRenderbufferStorageMultisample(NUM2UINT(target), NUM2INT(samples), NUM2UINT(internalformat), NUM2INT(width), NUM2INT(height));
    return Qnil;
}

static VALUE rb_glFramebufferTextureLayer(VALUE gl, VALUE target, VALUE attachment, VALUE texture, VALUE level, VALUE layer) {
    glFramebufferTextureLayer(NUM2UINT(target), NUM2UINT(attachment), NUM2UINT(texture), NUM2INT(level), NUM2INT(layer));
    return Qnil;
}

static VALUE rb_glMapBufferRange(VALUE gl, VALUE target, VALUE offset, VALUE length, VALUE access) {
    return PTR2NUM(glMapBufferRange(NUM2UINT(target), NUM2LONG(offset), NUM2SIZET(length), NUM2UINT(access)));
}

static VALUE rb_glFlushMappedBufferRange(VALUE gl, VALUE target, VALUE offset, VALUE length) {
    glFlushMappedBufferRange(NUM2UINT(target), NUM2LONG(offset), NUM2SIZET(length));
    return Qnil;
}

static VALUE rb_glBindVertexArray(VALUE gl, VALUE array) {
    glBindVertexArray(NUM2UINT(array));
    return Qnil;
}

static VALUE rb_glIsVertexArray(VALUE gl, VALUE array) {
    return RB_BOOL(glIsVertexArray(NUM2UINT(array)));
}

static VALUE rb_glDrawArraysInstanced(VALUE gl, VALUE mode, VALUE first, VALUE count, VALUE instancecount) {
    glDrawArraysInstanced(NUM2UINT(mode), NUM2INT(first), NUM2INT(count), NUM2INT(instancecount));
    return Qnil;
}

static VALUE rb_glDrawElementsInstanced(VALUE gl, VALUE mode, VALUE count, VALUE type, VALUE indices, VALUE instancecount) {
    glDrawElementsInstanced(NUM2UINT(mode), NUM2INT(count), NUM2UINT(type), RB2PTR(indices), NUM2INT(instancecount));
    return Qnil;
}

static VALUE rb_glTexBuffer(VALUE gl, VALUE target, VALUE internalformat, VALUE buffer) {
    glTexBuffer(NUM2UINT(target), NUM2UINT(internalformat), NUM2UINT(buffer));
    return Qnil;
}

static VALUE rb_glPrimitiveRestartIndex(VALUE gl, VALUE index) {
    glPrimitiveRestartIndex(NUM2UINT(index));
    return Qnil;
}

static VALUE rb_glCopyBufferSubData(VALUE gl, VALUE readTarget, VALUE writeTarget, VALUE readOffset, VALUE writeOffset, VALUE size) {
    glCopyBufferSubData(NUM2UINT(readTarget), NUM2UINT(writeTarget), NUM2LONG(readOffset), NUM2LONG(writeOffset), NUM2SIZET(size));
    return Qnil;
}

static VALUE rb_glGetUniformIndices(VALUE gl, VALUE program, VALUE uniformCount, VALUE uniformNames, VALUE uniformIndices) {
    glGetUniformIndices(NUM2UINT(program), NUM2INT(uniformCount), RB2PTR(uniformNames), RB2PTR(uniformIndices));
    return Qnil;
}

static VALUE rb_glGetActiveUniformsiv(VALUE gl, VALUE program, VALUE uniformCount, VALUE uniformIndices, VALUE pname, VALUE params) {
    glGetActiveUniformsiv(NUM2UINT(program), NUM2INT(uniformCount), RB2PTR(uniformIndices), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetActiveUniformName(VALUE gl, VALUE program, VALUE uniformIndex, VALUE bufSize, VALUE length, VALUE uniformName) {
    glGetActiveUniformName(NUM2UINT(program), NUM2UINT(uniformIndex), NUM2INT(bufSize), RB2PTR(length), RB2PTR(uniformName));
    return Qnil;
}

static VALUE rb_glGetUniformBlockIndex(VALUE gl, VALUE program, VALUE uniformBlockName) {
    return UINT2NUM(glGetUniformBlockIndex(NUM2UINT(program), StringValueCStr(uniformBlockName)));
}

static VALUE rb_glGetActiveUniformBlockiv(VALUE gl, VALUE program, VALUE uniformBlockIndex, VALUE pname, VALUE params) {
    glGetActiveUniformBlockiv(NUM2UINT(program), NUM2UINT(uniformBlockIndex), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetActiveUniformBlockName(VALUE gl, VALUE program, VALUE uniformBlockIndex, VALUE bufSize, VALUE length, VALUE uniformBlockName) {
    glGetActiveUniformBlockName(NUM2UINT(program), NUM2UINT(uniformBlockIndex), NUM2INT(bufSize), RB2PTR(length), RB2PTR(uniformBlockName));
    return Qnil;
}

static VALUE rb_glUniformBlockBinding(VALUE gl, VALUE program, VALUE uniformBlockIndex, VALUE uniformBlockBinding) {
    glUniformBlockBinding(NUM2UINT(program), NUM2UINT(uniformBlockIndex), NUM2UINT(uniformBlockBinding));
    return Qnil;
}

static VALUE rb_glDrawElementsBaseVertex(VALUE gl, VALUE mode, VALUE count, VALUE type, VALUE indices, VALUE basevertex) {
    glDrawElementsBaseVertex(NUM2UINT(mode), NUM2INT(count), NUM2UINT(type), RB2PTR(indices), NUM2INT(basevertex));
    return Qnil;
}

static VALUE rb_glDrawRangeElementsBaseVertex(VALUE gl, VALUE mode, VALUE start, VALUE finish, VALUE count, VALUE type, VALUE indices, VALUE basevertex) {
    glDrawRangeElementsBaseVertex(NUM2UINT(mode), NUM2UINT(start), NUM2UINT(finish), NUM2INT(count), NUM2UINT(type), RB2PTR(indices), NUM2INT(basevertex));
    return Qnil;
}

static VALUE rb_glDrawElementsInstancedBaseVertex(VALUE gl, VALUE mode, VALUE count, VALUE type, VALUE indices, VALUE instancecount, VALUE basevertex) {
    glDrawElementsInstancedBaseVertex(NUM2UINT(mode), NUM2INT(count), NUM2UINT(type), RB2PTR(indices), NUM2INT(instancecount), NUM2INT(basevertex));
    return Qnil;
}

static VALUE rb_glMultiDrawElementsBaseVertex(VALUE gl, VALUE mode, VALUE count, VALUE type, VALUE indices, VALUE drawcount, VALUE basevertex) {
    glMultiDrawElementsBaseVertex(NUM2UINT(mode), RB2PTR(count), NUM2UINT(type), RB2PTR(indices), NUM2INT(drawcount), RB2PTR(basevertex));
    return Qnil;
}

static VALUE rb_glProvokingVertex(VALUE gl, VALUE mode) {
    glProvokingVertex(NUM2UINT(mode));
    return Qnil;
}

static VALUE rb_glFenceSync(VALUE gl, VALUE condition, VALUE flags) {
    return Data_Wrap_Struct(rb_cSync, NULL, RUBY_DEFAULT_FREE, glFenceSync(NUM2UINT(condition), NUM2UINT(flags)));
}

static VALUE rb_glIsSync(VALUE gl, VALUE sync) {
    return RB_BOOL(glIsSync(DATA_PTR(sync)));
}

static VALUE rb_glDeleteSync(VALUE gl, VALUE sync) {
    glDeleteSync(DATA_PTR(sync));
    return Qnil;
}

static VALUE rb_glClientWaitSync(VALUE gl, VALUE sync, VALUE flags, VALUE timeout) {
    return UINT2NUM(glClientWaitSync(DATA_PTR(sync), NUM2UINT(flags), NUM2ULL(timeout)));
}

static VALUE rb_glWaitSync(VALUE gl, VALUE sync, VALUE flags, VALUE timeout) {
    glWaitSync(DATA_PTR(sync), NUM2UINT(flags), NUM2ULL(timeout));
    return Qnil;
}

static VALUE rb_glGetInteger64v(VALUE gl, VALUE pname, VALUE data) {
    glGetInteger64v(NUM2UINT(pname), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetSynciv(VALUE gl, VALUE sync, VALUE pname, VALUE count, VALUE length, VALUE values) {
    glGetSynciv(DATA_PTR(sync), NUM2UINT(pname), NUM2INT(count), RB2PTR(length), RB2PTR(values));
    return Qnil;
}

static VALUE rb_glGetInteger64i_v(VALUE gl, VALUE target, VALUE index, VALUE data) {
    glGetInteger64i_v(NUM2UINT(target), NUM2UINT(index), RB2PTR(data));
    return Qnil;
}

static VALUE rb_glGetBufferParameteri64v(VALUE gl, VALUE target, VALUE pname, VALUE params) {
    glGetBufferParameteri64v(NUM2UINT(target), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glFramebufferTexture(VALUE gl, VALUE target, VALUE attachment, VALUE texture, VALUE level) {
    glFramebufferTexture(NUM2UINT(target), NUM2UINT(attachment), NUM2UINT(texture), NUM2INT(level));
    return Qnil;
}

static VALUE rb_glTexImage2DMultisample(VALUE gl, VALUE target, VALUE samples, VALUE internalformat, VALUE width, VALUE height, VALUE fixedsamplelocations) {
    glTexImage2DMultisample(NUM2UINT(target), NUM2INT(samples), NUM2UINT(internalformat), NUM2INT(width), NUM2INT(height), RTEST(fixedsamplelocations));
    return Qnil;
}

static VALUE rb_glTexImage3DMultisample(VALUE gl, VALUE target, VALUE samples, VALUE internalformat, VALUE width, VALUE height, VALUE depth, VALUE fixedsamplelocations) {
    glTexImage3DMultisample(NUM2UINT(target), NUM2INT(samples), NUM2UINT(internalformat), NUM2INT(width), NUM2INT(height), NUM2INT(depth), RTEST(fixedsamplelocations));
    return Qnil;
}

static VALUE rb_glGetMultisamplefv(VALUE gl, VALUE pname, VALUE index, VALUE val) {
    glGetMultisamplefv(NUM2UINT(pname), NUM2UINT(index), RB2PTR(val));
    return Qnil;
}

static VALUE rb_glSampleMaski(VALUE gl, VALUE maskNumber, VALUE mask) {
    glSampleMaski(NUM2UINT(maskNumber), NUM2UINT(mask));
    return Qnil;
}

static VALUE rb_glBindFragDataLocationIndexed(VALUE gl, VALUE program, VALUE colorNumber, VALUE index, VALUE name) {
    glBindFragDataLocationIndexed(NUM2UINT(program), NUM2UINT(colorNumber), NUM2UINT(index), StringValueCStr(name));
    return Qnil;
}

static VALUE rb_glGetFragDataIndex(VALUE gl, VALUE program, VALUE name) {
    return INT2NUM(glGetFragDataIndex(NUM2UINT(program), StringValueCStr(name)));
}

static VALUE rb_glIsSampler(VALUE gl, VALUE sampler) {
    return RB_BOOL(glIsSampler(NUM2UINT(sampler)));
}

static VALUE rb_glBindSampler(VALUE gl, VALUE unit, VALUE sampler) {
    glBindSampler(NUM2UINT(unit), NUM2UINT(sampler));
    return Qnil;
}

static VALUE rb_glSamplerParameteri(VALUE gl, VALUE sampler, VALUE pname, VALUE param) {
    glSamplerParameteri(NUM2UINT(sampler), NUM2UINT(pname), NUM2INT(param));
    return Qnil;
}

static VALUE rb_glSamplerParameteriv(VALUE gl, VALUE sampler, VALUE pname, VALUE param) {
    glSamplerParameteriv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(param));
    return Qnil;
}

static VALUE rb_glSamplerParameterf(VALUE gl, VALUE sampler, VALUE pname, VALUE param) {
    glSamplerParameterf(NUM2UINT(sampler), NUM2UINT(pname), NUM2FLT(param));
    return Qnil;
}

static VALUE rb_glSamplerParameterfv(VALUE gl, VALUE sampler, VALUE pname, VALUE param) {
    glSamplerParameterfv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(param));
    return Qnil;
}

static VALUE rb_glSamplerParameterIiv(VALUE gl, VALUE sampler, VALUE pname, VALUE param) {
    glSamplerParameterIiv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(param));
    return Qnil;
}

static VALUE rb_glSamplerParameterIuiv(VALUE gl, VALUE sampler, VALUE pname, VALUE param) {
    glSamplerParameterIuiv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(param));
    return Qnil;
}

static VALUE rb_glGetSamplerParameteriv(VALUE gl, VALUE sampler, VALUE pname, VALUE params) {
    glGetSamplerParameteriv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetSamplerParameterIiv(VALUE gl, VALUE sampler, VALUE pname, VALUE params) {
    glGetSamplerParameterIiv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetSamplerParameterfv(VALUE gl, VALUE sampler, VALUE pname, VALUE params) {
    glGetSamplerParameterfv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetSamplerParameterIuiv(VALUE gl, VALUE sampler, VALUE pname, VALUE params) {
    glGetSamplerParameterIuiv(NUM2UINT(sampler), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glQueryCounter(VALUE gl, VALUE id, VALUE target) {
    glQueryCounter(NUM2UINT(id), NUM2UINT(target));
    return Qnil;
}

static VALUE rb_glGetQueryObjecti64v(VALUE gl, VALUE id, VALUE pname, VALUE params) {
    glGetQueryObjecti64v(NUM2UINT(id), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glGetQueryObjectui64v(VALUE gl, VALUE id, VALUE pname, VALUE params) {
    glGetQueryObjectui64v(NUM2UINT(id), NUM2UINT(pname), RB2PTR(params));
    return Qnil;
}

static VALUE rb_glVertexAttribDivisor(VALUE gl, VALUE index, VALUE divisor) {
    glVertexAttribDivisor(NUM2UINT(index), NUM2UINT(divisor));
    return Qnil;
}

static VALUE rb_glVertexAttribP1ui(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP1ui(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), NUM2UINT(value));
    return Qnil;
}

static VALUE rb_glVertexAttribP1uiv(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP1uiv(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glVertexAttribP2ui(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP2ui(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), NUM2UINT(value));
    return Qnil;
}

static VALUE rb_glVertexAttribP2uiv(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP2uiv(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glVertexAttribP3ui(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP3ui(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), NUM2UINT(value));
    return Qnil;
}

static VALUE rb_glVertexAttribP3uiv(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP3uiv(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glVertexAttribP4ui(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP4ui(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), NUM2UINT(value));
    return Qnil;
}

static VALUE rb_glVertexAttribP4uiv(VALUE gl, VALUE index, VALUE type, VALUE normalized, VALUE value) {
    glVertexAttribP4uiv(NUM2UINT(index), NUM2UINT(type), RTEST(normalized), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glVertexP2ui(VALUE gl, VALUE type, VALUE value) {
    glVertexP2ui(NUM2UINT(type), NUM2UINT(value));
    return Qnil;
}

static VALUE rb_glVertexP2uiv(VALUE gl, VALUE type, VALUE value) {
    glVertexP2uiv(NUM2UINT(type), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glVertexP3ui(VALUE gl, VALUE type, VALUE value) {
    glVertexP3ui(NUM2UINT(type), NUM2UINT(value));
    return Qnil;
}

static VALUE rb_glVertexP3uiv(VALUE gl, VALUE type, VALUE value) {
    glVertexP3uiv(NUM2UINT(type), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glVertexP4ui(VALUE gl, VALUE type, VALUE value) {
    glVertexP4ui(NUM2UINT(type), NUM2UINT(value));
    return Qnil;
}

static VALUE rb_glVertexP4uiv(VALUE gl, VALUE type, VALUE value) {
    glVertexP4uiv(NUM2UINT(type), RB2PTR(value));
    return Qnil;
}

static VALUE rb_glTexCoordP1ui(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP1ui(NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glTexCoordP1uiv(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP1uiv(NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glTexCoordP2ui(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP2ui(NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glTexCoordP2uiv(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP2uiv(NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glTexCoordP3ui(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP3ui(NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glTexCoordP3uiv(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP3uiv(NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glTexCoordP4ui(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP4ui(NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glTexCoordP4uiv(VALUE gl, VALUE type, VALUE coords) {
    glTexCoordP4uiv(NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP1ui(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP1ui(NUM2UINT(texture), NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP1uiv(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP1uiv(NUM2UINT(texture), NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP2ui(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP2ui(NUM2UINT(texture), NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP2uiv(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP2uiv(NUM2UINT(texture), NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP3ui(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP3ui(NUM2UINT(texture), NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP3uiv(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP3uiv(NUM2UINT(texture), NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP4ui(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP4ui(NUM2UINT(texture), NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glMultiTexCoordP4uiv(VALUE gl, VALUE texture, VALUE type, VALUE coords) {
    glMultiTexCoordP4uiv(NUM2UINT(texture), NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glNormalP3ui(VALUE gl, VALUE type, VALUE coords) {
    glNormalP3ui(NUM2UINT(type), NUM2UINT(coords));
    return Qnil;
}

static VALUE rb_glNormalP3uiv(VALUE gl, VALUE type, VALUE coords) {
    glNormalP3uiv(NUM2UINT(type), RB2PTR(coords));
    return Qnil;
}

static VALUE rb_glColorP3ui(VALUE gl, VALUE type, VALUE color) {
    glColorP3ui(NUM2UINT(type), NUM2UINT(color));
    return Qnil;
}

static VALUE rb_glColorP3uiv(VALUE gl, VALUE type, VALUE color) {
    glColorP3uiv(NUM2UINT(type), RB2PTR(color));
    return Qnil;
}

static VALUE rb_glColorP4ui(VALUE gl, VALUE type, VALUE color) {
    glColorP4ui(NUM2UINT(type), NUM2UINT(color));
    return Qnil;
}

static VALUE rb_glColorP4uiv(VALUE gl, VALUE type, VALUE color) {
    glColorP4uiv(NUM2UINT(type), RB2PTR(color));
    return Qnil;
}

static VALUE rb_glSecondaryColorP3ui(VALUE gl, VALUE type, VALUE color) {
    glSecondaryColorP3ui(NUM2UINT(type), NUM2UINT(color));
    return Qnil;
}

static VALUE rb_glSecondaryColorP3uiv(VALUE gl, VALUE type, VALUE color) {
    glSecondaryColorP3uiv(NUM2UINT(type), RB2PTR(color));
    return Qnil;
}

static VALUE rb_init_gl(VALUE gl)
{
    if (!gladLoadGL())
        rb_raise(rb_eRuntimeError, "failed to import OpenGL symbols");
    return Qnil;
}

void RGSS_Init_GL(VALUE parent) {
    rb_mGL = rb_define_module_under(parent, "GL");
    rb_cSync = rb_define_class_under(rb_mGL, "Sync", rb_cObject);
    rb_define_singleton_method0(rb_mGL, "init", rb_init_gl, 0);
    
    rb_define_module_function(rb_mGL, "glGenTexture", rb_glGenTexture, 0);
    rb_define_module_function(rb_mGL, "glGenQuery", rb_glGenQuery, 0);
    rb_define_module_function(rb_mGL, "glGenBuffer", rb_glGenBuffer, 0);
    rb_define_module_function(rb_mGL, "glGenRenderbuffer", rb_glGenRenderbuffer, 0);
    rb_define_module_function(rb_mGL, "glGenFramebuffer", rb_glGenFramebuffer, 0);
    rb_define_module_function(rb_mGL, "glGenSampler", rb_glGenSampler, 0);
    rb_define_module_function(rb_mGL, "glGenVertexArray", rb_glGenVertexArray, 0);
    rb_define_module_function(rb_mGL, "glDeleteTexture", rb_glDeleteTexture, 1);
    rb_define_module_function(rb_mGL, "glDeleteQuery", rb_glDeleteQuery, 1);
    rb_define_module_function(rb_mGL, "glDeleteBuffer", rb_glDeleteBuffer, 1);
    rb_define_module_function(rb_mGL, "glDeleteRenderbuffer", rb_glDeleteRenderbuffer, 1);
    rb_define_module_function(rb_mGL, "glDeleteFramebuffer", rb_glDeleteFramebuffer, 1);
    rb_define_module_function(rb_mGL, "glDeleteVertexArray", rb_glDeleteVertexArray, 1);
    rb_define_module_function(rb_mGL, "glDeleteSampler", rb_glDeleteSampler, 1);
    rb_define_module_function(rb_mGL, "glCullFace", rb_glCullFace, 1);
    rb_define_module_function(rb_mGL, "glFrontFace", rb_glFrontFace, 1);
    rb_define_module_function(rb_mGL, "glHint", rb_glHint, 2);
    rb_define_module_function(rb_mGL, "glLineWidth", rb_glLineWidth, 1);
    rb_define_module_function(rb_mGL, "glPointSize", rb_glPointSize, 1);
    rb_define_module_function(rb_mGL, "glPolygonMode", rb_glPolygonMode, 2);
    rb_define_module_function(rb_mGL, "glScissor", rb_glScissor, 4);
    rb_define_module_function(rb_mGL, "glTexParameterf", rb_glTexParameterf, 3);
    rb_define_module_function(rb_mGL, "glTexParameterfv", rb_glTexParameterfv, 3);
    rb_define_module_function(rb_mGL, "glTexParameteri", rb_glTexParameteri, 3);
    rb_define_module_function(rb_mGL, "glTexParameteriv", rb_glTexParameteriv, 3);
    rb_define_module_function(rb_mGL, "glTexImage1D", rb_glTexImage1D, 8);
    rb_define_module_function(rb_mGL, "glTexImage2D", rb_glTexImage2D, 9);
    rb_define_module_function(rb_mGL, "glDrawBuffer", rb_glDrawBuffer, 1);
    rb_define_module_function(rb_mGL, "glClear", rb_glClear, 1);
    rb_define_module_function(rb_mGL, "glClearColor", rb_glClearColor, -1);
    rb_define_module_function(rb_mGL, "glClearStencil", rb_glClearStencil, 1);
    rb_define_module_function(rb_mGL, "glClearDepth", rb_glClearDepth, 1);
    rb_define_module_function(rb_mGL, "glStencilMask", rb_glStencilMask, 1);
    rb_define_module_function(rb_mGL, "glColorMask", rb_glColorMask, 4);
    rb_define_module_function(rb_mGL, "glDepthMask", rb_glDepthMask, 1);
    rb_define_module_function(rb_mGL, "glDisable", rb_glDisable, 1);
    rb_define_module_function(rb_mGL, "glEnable", rb_glEnable, 1);
    rb_define_module_function(rb_mGL, "glFinish", rb_glFinish, 0);
    rb_define_module_function(rb_mGL, "glFlush", rb_glFlush, 0);
    rb_define_module_function(rb_mGL, "glBlendFunc", rb_glBlendFunc, 2);
    rb_define_module_function(rb_mGL, "glLogicOp", rb_glLogicOp, 1);
    rb_define_module_function(rb_mGL, "glStencilFunc", rb_glStencilFunc, 3);
    rb_define_module_function(rb_mGL, "glStencilOp", rb_glStencilOp, 3);
    rb_define_module_function(rb_mGL, "glDepthFunc", rb_glDepthFunc, 1);
    rb_define_module_function(rb_mGL, "glPixelStoref", rb_glPixelStoref, 2);
    rb_define_module_function(rb_mGL, "glPixelStorei", rb_glPixelStorei, 2);
    rb_define_module_function(rb_mGL, "glReadBuffer", rb_glReadBuffer, 1);
    rb_define_module_function(rb_mGL, "glReadPixels", rb_glReadPixels, 7);
    rb_define_module_function(rb_mGL, "glGetBooleanv", rb_glGetBooleanv, 2);
    rb_define_module_function(rb_mGL, "glGetDoublev", rb_glGetDoublev, 2);
    rb_define_module_function(rb_mGL, "glGetError", rb_glGetError, 0);
    rb_define_module_function(rb_mGL, "glGetFloat", rb_glGetFloat, 1);
    rb_define_module_function(rb_mGL, "glGetFloatv", rb_glGetFloatv, 2);
    rb_define_module_function(rb_mGL, "glGetInteger", rb_glGetInteger, 1);
    rb_define_module_function(rb_mGL, "glGetIntegerv", rb_glGetIntegerv, 2);
    rb_define_module_function(rb_mGL, "glGetString", rb_glGetString, 1);
    rb_define_module_function(rb_mGL, "glGetTexImage", rb_glGetTexImage, 5);
    rb_define_module_function(rb_mGL, "glGetTexParameterfv", rb_glGetTexParameterfv, 3);
    rb_define_module_function(rb_mGL, "glGetTexParameteriv", rb_glGetTexParameteriv, 3);
    rb_define_module_function(rb_mGL, "glGetTexLevelParameterfv", rb_glGetTexLevelParameterfv, 4);
    rb_define_module_function(rb_mGL, "glGetTexLevelParameteriv", rb_glGetTexLevelParameteriv, 4);
    rb_define_module_function(rb_mGL, "glIsEnabled", rb_glIsEnabled, 1);
    rb_define_module_function(rb_mGL, "glDepthRange", rb_glDepthRange, 2);
    rb_define_module_function(rb_mGL, "glViewport", rb_glViewport, 4);
    rb_define_module_function(rb_mGL, "glDrawArrays", rb_glDrawArrays, 3);
    rb_define_module_function(rb_mGL, "glDrawElements", rb_glDrawElements, 4);
    rb_define_module_function(rb_mGL, "glPolygonOffset", rb_glPolygonOffset, 2);
    rb_define_module_function(rb_mGL, "glCopyTexImage1D", rb_glCopyTexImage1D, 7);
    rb_define_module_function(rb_mGL, "glCopyTexImage2D", rb_glCopyTexImage2D, 8);
    rb_define_module_function(rb_mGL, "glCopyTexSubImage1D", rb_glCopyTexSubImage1D, 6);
    rb_define_module_function(rb_mGL, "glCopyTexSubImage2D", rb_glCopyTexSubImage2D, 8);
    rb_define_module_function(rb_mGL, "glTexSubImage1D", rb_glTexSubImage1D, 7);
    rb_define_module_function(rb_mGL, "glTexSubImage2D", rb_glTexSubImage2D, 9);
    rb_define_module_function(rb_mGL, "glBindTexture", rb_glBindTexture, 2);
    rb_define_module_function(rb_mGL, "glDeleteTextures", rb_glDeleteTextures, 2);
    rb_define_module_function(rb_mGL, "glGenTextures", rb_glGenTextures, 2);
    rb_define_module_function(rb_mGL, "glIsTexture", rb_glIsTexture, 1);
    rb_define_module_function(rb_mGL, "glDrawRangeElements", rb_glDrawRangeElements, 6);
    rb_define_module_function(rb_mGL, "glTexImage3D", rb_glTexImage3D, 10);
    rb_define_module_function(rb_mGL, "glTexSubImage3D", rb_glTexSubImage3D, 11);
    rb_define_module_function(rb_mGL, "glCopyTexSubImage3D", rb_glCopyTexSubImage3D, 9);
    rb_define_module_function(rb_mGL, "glActiveTexture", rb_glActiveTexture, 1);
    rb_define_module_function(rb_mGL, "glSampleCoverage", rb_glSampleCoverage, 2);
    rb_define_module_function(rb_mGL, "glCompressedTexImage3D", rb_glCompressedTexImage3D, 9);
    rb_define_module_function(rb_mGL, "glCompressedTexImage2D", rb_glCompressedTexImage2D, 8);
    rb_define_module_function(rb_mGL, "glCompressedTexImage1D", rb_glCompressedTexImage1D, 7);
    rb_define_module_function(rb_mGL, "glCompressedTexSubImage3D", rb_glCompressedTexSubImage3D, 11);
    rb_define_module_function(rb_mGL, "glCompressedTexSubImage2D", rb_glCompressedTexSubImage2D, 9);
    rb_define_module_function(rb_mGL, "glCompressedTexSubImage1D", rb_glCompressedTexSubImage1D, 7);
    rb_define_module_function(rb_mGL, "glGetCompressedTexImage", rb_glGetCompressedTexImage, 3);
    rb_define_module_function(rb_mGL, "glBlendFuncSeparate", rb_glBlendFuncSeparate, 4);
    rb_define_module_function(rb_mGL, "glMultiDrawArrays", rb_glMultiDrawArrays, 4);
    rb_define_module_function(rb_mGL, "glMultiDrawElements", rb_glMultiDrawElements, 5);
    rb_define_module_function(rb_mGL, "glPointParameterf", rb_glPointParameterf, 2);
    rb_define_module_function(rb_mGL, "glPointParameterfv", rb_glPointParameterfv, 2);
    rb_define_module_function(rb_mGL, "glPointParameteri", rb_glPointParameteri, 2);
    rb_define_module_function(rb_mGL, "glPointParameteriv", rb_glPointParameteriv, 2);
    rb_define_module_function(rb_mGL, "glBlendColor", rb_glBlendColor, 4);
    rb_define_module_function(rb_mGL, "glBlendEquation", rb_glBlendEquation, 1);
    rb_define_module_function(rb_mGL, "glGenQueries", rb_glGenQueries, 2);
    rb_define_module_function(rb_mGL, "glDeleteQueries", rb_glDeleteQueries, 2);
    rb_define_module_function(rb_mGL, "glIsQuery", rb_glIsQuery, 1);
    rb_define_module_function(rb_mGL, "glBeginQuery", rb_glBeginQuery, 2);
    rb_define_module_function(rb_mGL, "glEndQuery", rb_glEndQuery, 1);
    rb_define_module_function(rb_mGL, "glGetQueryiv", rb_glGetQueryiv, 3);
    rb_define_module_function(rb_mGL, "glGetQueryObjectiv", rb_glGetQueryObjectiv, 3);
    rb_define_module_function(rb_mGL, "glGetQueryObjectuiv", rb_glGetQueryObjectuiv, 3);
    rb_define_module_function(rb_mGL, "glBindBuffer", rb_glBindBuffer, 2);
    rb_define_module_function(rb_mGL, "glDeleteBuffers", rb_glDeleteBuffers, 2);
    rb_define_module_function(rb_mGL, "glGenBuffers", rb_glGenBuffers, 2);
    rb_define_module_function(rb_mGL, "glIsBuffer", rb_glIsBuffer, 1);
    rb_define_module_function(rb_mGL, "glBufferData", rb_glBufferData, 4);
    rb_define_module_function(rb_mGL, "glBufferSubData", rb_glBufferSubData, 4);
    rb_define_module_function(rb_mGL, "glGetBufferSubData", rb_glGetBufferSubData, 4);
    rb_define_module_function(rb_mGL, "glMapBuffer", rb_glMapBuffer, 2);
    rb_define_module_function(rb_mGL, "glUnmapBuffer", rb_glUnmapBuffer, 1);
    rb_define_module_function(rb_mGL, "glGetBufferParameteriv", rb_glGetBufferParameteriv, 3);
    rb_define_module_function(rb_mGL, "glGetBufferPointerv", rb_glGetBufferPointerv, 3);
    rb_define_module_function(rb_mGL, "glBlendEquationSeparate", rb_glBlendEquationSeparate, 2);
    rb_define_module_function(rb_mGL, "glDrawBuffers", rb_glDrawBuffers, 2);
    rb_define_module_function(rb_mGL, "glStencilOpSeparate", rb_glStencilOpSeparate, 4);
    rb_define_module_function(rb_mGL, "glStencilFuncSeparate", rb_glStencilFuncSeparate, 4);
    rb_define_module_function(rb_mGL, "glStencilMaskSeparate", rb_glStencilMaskSeparate, 2);
    rb_define_module_function(rb_mGL, "glAttachShader", rb_glAttachShader, 2);
    rb_define_module_function(rb_mGL, "glBindAttribLocation", rb_glBindAttribLocation, 3);
    rb_define_module_function(rb_mGL, "glCompileShader", rb_glCompileShader, 1);
    rb_define_module_function(rb_mGL, "glCreateProgram", rb_glCreateProgram, 0);
    rb_define_module_function(rb_mGL, "glCreateShader", rb_glCreateShader, 1);
    rb_define_module_function(rb_mGL, "glDeleteProgram", rb_glDeleteProgram, 1);
    rb_define_module_function(rb_mGL, "glDeleteShader", rb_glDeleteShader, 1);
    rb_define_module_function(rb_mGL, "glDetachShader", rb_glDetachShader, 2);
    rb_define_module_function(rb_mGL, "glDisableVertexAttribArray", rb_glDisableVertexAttribArray, 1);
    rb_define_module_function(rb_mGL, "glEnableVertexAttribArray", rb_glEnableVertexAttribArray, 1);
    rb_define_module_function(rb_mGL, "glGetActiveAttrib", rb_glGetActiveAttrib, 7);
    rb_define_module_function(rb_mGL, "glGetActiveUniform", rb_glGetActiveUniform, 7);
    rb_define_module_function(rb_mGL, "glGetAttachedShaders", rb_glGetAttachedShaders, 4);
    rb_define_module_function(rb_mGL, "glGetAttribLocation", rb_glGetAttribLocation, 2);
    rb_define_module_function(rb_mGL, "glGetProgramiv", rb_glGetProgramiv, 3);
    rb_define_module_function(rb_mGL, "glGetProgramInfoLog", rb_glGetProgramInfoLog, 4);
    rb_define_module_function(rb_mGL, "glGetShaderiv", rb_glGetShaderiv, 3);
    rb_define_module_function(rb_mGL, "glGetShaderInfoLog", rb_glGetShaderInfoLog, 4);
    rb_define_module_function(rb_mGL, "glGetShaderSource", rb_glGetShaderSource, 4);
    rb_define_module_function(rb_mGL, "glGetUniformLocation", rb_glGetUniformLocation, 2);
    rb_define_module_function(rb_mGL, "glGetUniformfv", rb_glGetUniformfv, 3);
    rb_define_module_function(rb_mGL, "glGetUniformiv", rb_glGetUniformiv, 3);
    rb_define_module_function(rb_mGL, "glGetVertexAttribdv", rb_glGetVertexAttribdv, 3);
    rb_define_module_function(rb_mGL, "glGetVertexAttribfv", rb_glGetVertexAttribfv, 3);
    rb_define_module_function(rb_mGL, "glGetVertexAttribiv", rb_glGetVertexAttribiv, 3);
    rb_define_module_function(rb_mGL, "glGetVertexAttribPointerv", rb_glGetVertexAttribPointerv, 3);
    rb_define_module_function(rb_mGL, "glIsProgram", rb_glIsProgram, 1);
    rb_define_module_function(rb_mGL, "glIsShader", rb_glIsShader, 1);
    rb_define_module_function(rb_mGL, "glLinkProgram", rb_glLinkProgram, 1);
    rb_define_module_function(rb_mGL, "glShaderSource", rb_glShaderSource, 2);
    rb_define_module_function(rb_mGL, "glUseProgram", rb_glUseProgram, 1);
    rb_define_module_function(rb_mGL, "glUniform1f", rb_glUniform1f, 2);
    rb_define_module_function(rb_mGL, "glUniform2f", rb_glUniform2f, 3);
    rb_define_module_function(rb_mGL, "glUniform3f", rb_glUniform3f, 4);
    rb_define_module_function(rb_mGL, "glUniform4f", rb_glUniform4f, 5);
    rb_define_module_function(rb_mGL, "glUniform1i", rb_glUniform1i, 2);
    rb_define_module_function(rb_mGL, "glUniform2i", rb_glUniform2i, 3);
    rb_define_module_function(rb_mGL, "glUniform3i", rb_glUniform3i, 4);
    rb_define_module_function(rb_mGL, "glUniform4i", rb_glUniform4i, 5);
    rb_define_module_function(rb_mGL, "glUniform1fv", rb_glUniform1fv, 3);
    rb_define_module_function(rb_mGL, "glUniform2fv", rb_glUniform2fv, 3);
    rb_define_module_function(rb_mGL, "glUniform3fv", rb_glUniform3fv, 3);
    rb_define_module_function(rb_mGL, "glUniform4fv", rb_glUniform4fv, 3);
    rb_define_module_function(rb_mGL, "glUniform1iv", rb_glUniform1iv, 3);
    rb_define_module_function(rb_mGL, "glUniform2iv", rb_glUniform2iv, 3);
    rb_define_module_function(rb_mGL, "glUniform3iv", rb_glUniform3iv, 3);
    rb_define_module_function(rb_mGL, "glUniform4iv", rb_glUniform4iv, 3);
    rb_define_module_function(rb_mGL, "glUniformMatrix2fv", rb_glUniformMatrix2fv, 4);
    rb_define_module_function(rb_mGL, "glUniformMatrix3fv", rb_glUniformMatrix3fv, 4);
    rb_define_module_function(rb_mGL, "glUniformMatrix4fv", rb_glUniformMatrix4fv, 4);
    rb_define_module_function(rb_mGL, "glValidateProgram", rb_glValidateProgram, 1);
    rb_define_module_function(rb_mGL, "glVertexAttrib1d", rb_glVertexAttrib1d, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib1dv", rb_glVertexAttrib1dv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib1f", rb_glVertexAttrib1f, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib1fv", rb_glVertexAttrib1fv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib1s", rb_glVertexAttrib1s, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib1sv", rb_glVertexAttrib1sv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib2d", rb_glVertexAttrib2d, 3);
    rb_define_module_function(rb_mGL, "glVertexAttrib2dv", rb_glVertexAttrib2dv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib2f", rb_glVertexAttrib2f, 3);
    rb_define_module_function(rb_mGL, "glVertexAttrib2fv", rb_glVertexAttrib2fv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib2s", rb_glVertexAttrib2s, 3);
    rb_define_module_function(rb_mGL, "glVertexAttrib2sv", rb_glVertexAttrib2sv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib3d", rb_glVertexAttrib3d, 4);
    rb_define_module_function(rb_mGL, "glVertexAttrib3dv", rb_glVertexAttrib3dv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib3f", rb_glVertexAttrib3f, 4);
    rb_define_module_function(rb_mGL, "glVertexAttrib3fv", rb_glVertexAttrib3fv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib3s", rb_glVertexAttrib3s, 4);
    rb_define_module_function(rb_mGL, "glVertexAttrib3sv", rb_glVertexAttrib3sv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4Nbv", rb_glVertexAttrib4Nbv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4Niv", rb_glVertexAttrib4Niv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4Nsv", rb_glVertexAttrib4Nsv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4Nub", rb_glVertexAttrib4Nub, 5);
    rb_define_module_function(rb_mGL, "glVertexAttrib4Nubv", rb_glVertexAttrib4Nubv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4Nuiv", rb_glVertexAttrib4Nuiv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4Nusv", rb_glVertexAttrib4Nusv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4bv", rb_glVertexAttrib4bv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4d", rb_glVertexAttrib4d, 5);
    rb_define_module_function(rb_mGL, "glVertexAttrib4dv", rb_glVertexAttrib4dv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4f", rb_glVertexAttrib4f, 5);
    rb_define_module_function(rb_mGL, "glVertexAttrib4fv", rb_glVertexAttrib4fv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4iv", rb_glVertexAttrib4iv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4s", rb_glVertexAttrib4s, 5);
    rb_define_module_function(rb_mGL, "glVertexAttrib4sv", rb_glVertexAttrib4sv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4ubv", rb_glVertexAttrib4ubv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4uiv", rb_glVertexAttrib4uiv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttrib4usv", rb_glVertexAttrib4usv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribPointer", rb_glVertexAttribPointer, 6);
    rb_define_module_function(rb_mGL, "glUniformMatrix2x3fv", rb_glUniformMatrix2x3fv, 4);
    rb_define_module_function(rb_mGL, "glUniformMatrix3x2fv", rb_glUniformMatrix3x2fv, 4);
    rb_define_module_function(rb_mGL, "glUniformMatrix2x4fv", rb_glUniformMatrix2x4fv, 4);
    rb_define_module_function(rb_mGL, "glUniformMatrix4x2fv", rb_glUniformMatrix4x2fv, 4);
    rb_define_module_function(rb_mGL, "glUniformMatrix3x4fv", rb_glUniformMatrix3x4fv, 4);
    rb_define_module_function(rb_mGL, "glUniformMatrix4x3fv", rb_glUniformMatrix4x3fv, 4);
    rb_define_module_function(rb_mGL, "glColorMaski", rb_glColorMaski, 5);
    rb_define_module_function(rb_mGL, "glGetBooleani_v", rb_glGetBooleani_v, 3);
    rb_define_module_function(rb_mGL, "glGetIntegeri_v", rb_glGetIntegeri_v, 3);
    rb_define_module_function(rb_mGL, "glEnablei", rb_glEnablei, 2);
    rb_define_module_function(rb_mGL, "glDisablei", rb_glDisablei, 2);
    rb_define_module_function(rb_mGL, "glIsEnabledi", rb_glIsEnabledi, 2);
    rb_define_module_function(rb_mGL, "glBeginTransformFeedback", rb_glBeginTransformFeedback, 1);
    rb_define_module_function(rb_mGL, "glEndTransformFeedback", rb_glEndTransformFeedback, 0);
    rb_define_module_function(rb_mGL, "glBindBufferRange", rb_glBindBufferRange, 5);
    rb_define_module_function(rb_mGL, "glBindBufferBase", rb_glBindBufferBase, 3);
    rb_define_module_function(rb_mGL, "glTransformFeedbackVaryings", rb_glTransformFeedbackVaryings, 4);
    rb_define_module_function(rb_mGL, "glGetTransformFeedbackVarying", rb_glGetTransformFeedbackVarying, 7);
    rb_define_module_function(rb_mGL, "glClampColor", rb_glClampColor, 2);
    rb_define_module_function(rb_mGL, "glBeginConditionalRender", rb_glBeginConditionalRender, 2);
    rb_define_module_function(rb_mGL, "glEndConditionalRender", rb_glEndConditionalRender, 0);
    rb_define_module_function(rb_mGL, "glVertexAttribIPointer", rb_glVertexAttribIPointer, 5);
    rb_define_module_function(rb_mGL, "glGetVertexAttribIiv", rb_glGetVertexAttribIiv, 3);
    rb_define_module_function(rb_mGL, "glGetVertexAttribIuiv", rb_glGetVertexAttribIuiv, 3);
    rb_define_module_function(rb_mGL, "glVertexAttribI1i", rb_glVertexAttribI1i, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI2i", rb_glVertexAttribI2i, 3);
    rb_define_module_function(rb_mGL, "glVertexAttribI3i", rb_glVertexAttribI3i, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribI4i", rb_glVertexAttribI4i, 5);
    rb_define_module_function(rb_mGL, "glVertexAttribI1ui", rb_glVertexAttribI1ui, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI2ui", rb_glVertexAttribI2ui, 3);
    rb_define_module_function(rb_mGL, "glVertexAttribI3ui", rb_glVertexAttribI3ui, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribI4ui", rb_glVertexAttribI4ui, 5);
    rb_define_module_function(rb_mGL, "glVertexAttribI1iv", rb_glVertexAttribI1iv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI2iv", rb_glVertexAttribI2iv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI3iv", rb_glVertexAttribI3iv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI4iv", rb_glVertexAttribI4iv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI1uiv", rb_glVertexAttribI1uiv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI2uiv", rb_glVertexAttribI2uiv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI3uiv", rb_glVertexAttribI3uiv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI4uiv", rb_glVertexAttribI4uiv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI4bv", rb_glVertexAttribI4bv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI4sv", rb_glVertexAttribI4sv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI4ubv", rb_glVertexAttribI4ubv, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribI4usv", rb_glVertexAttribI4usv, 2);
    rb_define_module_function(rb_mGL, "glGetUniformuiv", rb_glGetUniformuiv, 3);
    rb_define_module_function(rb_mGL, "glBindFragDataLocation", rb_glBindFragDataLocation, 3);
    rb_define_module_function(rb_mGL, "glGetFragDataLocation", rb_glGetFragDataLocation, 2);
    rb_define_module_function(rb_mGL, "glUniform1ui", rb_glUniform1ui, 2);
    rb_define_module_function(rb_mGL, "glUniform2ui", rb_glUniform2ui, 3);
    rb_define_module_function(rb_mGL, "glUniform3ui", rb_glUniform3ui, 4);
    rb_define_module_function(rb_mGL, "glUniform4ui", rb_glUniform4ui, 5);
    rb_define_module_function(rb_mGL, "glUniform1uiv", rb_glUniform1uiv, 3);
    rb_define_module_function(rb_mGL, "glUniform2uiv", rb_glUniform2uiv, 3);
    rb_define_module_function(rb_mGL, "glUniform3uiv", rb_glUniform3uiv, 3);
    rb_define_module_function(rb_mGL, "glUniform4uiv", rb_glUniform4uiv, 3);
    rb_define_module_function(rb_mGL, "glTexParameterIiv", rb_glTexParameterIiv, 3);
    rb_define_module_function(rb_mGL, "glTexParameterIuiv", rb_glTexParameterIuiv, 3);
    rb_define_module_function(rb_mGL, "glGetTexParameterIiv", rb_glGetTexParameterIiv, 3);
    rb_define_module_function(rb_mGL, "glGetTexParameterIuiv", rb_glGetTexParameterIuiv, 3);
    rb_define_module_function(rb_mGL, "glClearBufferiv", rb_glClearBufferiv, 3);
    rb_define_module_function(rb_mGL, "glClearBufferuiv", rb_glClearBufferuiv, 3);
    rb_define_module_function(rb_mGL, "glClearBufferfv", rb_glClearBufferfv, 3);
    rb_define_module_function(rb_mGL, "glClearBufferfi", rb_glClearBufferfi, 4);
    rb_define_module_function(rb_mGL, "glGetStringi", rb_glGetStringi, 2);
    rb_define_module_function(rb_mGL, "glIsRenderbuffer", rb_glIsRenderbuffer, 1);
    rb_define_module_function(rb_mGL, "glBindRenderbuffer", rb_glBindRenderbuffer, 2);
    rb_define_module_function(rb_mGL, "glDeleteRenderbuffers", rb_glDeleteRenderbuffers, 2);
    rb_define_module_function(rb_mGL, "glGenRenderbuffers", rb_glGenRenderbuffers, 2);
    rb_define_module_function(rb_mGL, "glRenderbufferStorage", rb_glRenderbufferStorage, 4);
    rb_define_module_function(rb_mGL, "glGetRenderbufferParameteriv", rb_glGetRenderbufferParameteriv, 3);
    rb_define_module_function(rb_mGL, "glIsFramebuffer", rb_glIsFramebuffer, 1);
    rb_define_module_function(rb_mGL, "glBindFramebuffer", rb_glBindFramebuffer, 2);
    rb_define_module_function(rb_mGL, "glDeleteFramebuffers", rb_glDeleteFramebuffers, 2);
    rb_define_module_function(rb_mGL, "glGenFramebuffers", rb_glGenFramebuffers, 2);
    rb_define_module_function(rb_mGL, "glCheckFramebufferStatus", rb_glCheckFramebufferStatus, 1);
    rb_define_module_function(rb_mGL, "glFramebufferTexture1D", rb_glFramebufferTexture1D, 5);
    rb_define_module_function(rb_mGL, "glFramebufferTexture2D", rb_glFramebufferTexture2D, 5);
    rb_define_module_function(rb_mGL, "glFramebufferTexture3D", rb_glFramebufferTexture3D, 6);
    rb_define_module_function(rb_mGL, "glFramebufferRenderbuffer", rb_glFramebufferRenderbuffer, 4);
    rb_define_module_function(rb_mGL, "glGetFramebufferAttachmentParameteriv", rb_glGetFramebufferAttachmentParameteriv, 4);
    rb_define_module_function(rb_mGL, "glGenerateMipmap", rb_glGenerateMipmap, 1);
    rb_define_module_function(rb_mGL, "glBlitFramebuffer", rb_glBlitFramebuffer, 10);
    rb_define_module_function(rb_mGL, "glRenderbufferStorageMultisample", rb_glRenderbufferStorageMultisample, 5);
    rb_define_module_function(rb_mGL, "glFramebufferTextureLayer", rb_glFramebufferTextureLayer, 5);
    rb_define_module_function(rb_mGL, "glMapBufferRange", rb_glMapBufferRange, 4);
    rb_define_module_function(rb_mGL, "glFlushMappedBufferRange", rb_glFlushMappedBufferRange, 3);
    rb_define_module_function(rb_mGL, "glBindVertexArray", rb_glBindVertexArray, 1);
    rb_define_module_function(rb_mGL, "glDeleteVertexArrays", rb_glDeleteVertexArrays, 2);
    rb_define_module_function(rb_mGL, "glGenVertexArrays", rb_glGenVertexArrays, 2);
    rb_define_module_function(rb_mGL, "glIsVertexArray", rb_glIsVertexArray, 1);
    rb_define_module_function(rb_mGL, "glDrawArraysInstanced", rb_glDrawArraysInstanced, 4);
    rb_define_module_function(rb_mGL, "glDrawElementsInstanced", rb_glDrawElementsInstanced, 5);
    rb_define_module_function(rb_mGL, "glTexBuffer", rb_glTexBuffer, 3);
    rb_define_module_function(rb_mGL, "glPrimitiveRestartIndex", rb_glPrimitiveRestartIndex, 1);
    rb_define_module_function(rb_mGL, "glCopyBufferSubData", rb_glCopyBufferSubData, 5);
    rb_define_module_function(rb_mGL, "glGetUniformIndices", rb_glGetUniformIndices, 4);
    rb_define_module_function(rb_mGL, "glGetActiveUniformsiv", rb_glGetActiveUniformsiv, 5);
    rb_define_module_function(rb_mGL, "glGetActiveUniformName", rb_glGetActiveUniformName, 5);
    rb_define_module_function(rb_mGL, "glGetUniformBlockIndex", rb_glGetUniformBlockIndex, 2);
    rb_define_module_function(rb_mGL, "glGetActiveUniformBlockiv", rb_glGetActiveUniformBlockiv, 4);
    rb_define_module_function(rb_mGL, "glGetActiveUniformBlockName", rb_glGetActiveUniformBlockName, 5);
    rb_define_module_function(rb_mGL, "glUniformBlockBinding", rb_glUniformBlockBinding, 3);
    rb_define_module_function(rb_mGL, "glDrawElementsBaseVertex", rb_glDrawElementsBaseVertex, 5);
    rb_define_module_function(rb_mGL, "glDrawRangeElementsBaseVertex", rb_glDrawRangeElementsBaseVertex, 7);
    rb_define_module_function(rb_mGL, "glDrawElementsInstancedBaseVertex", rb_glDrawElementsInstancedBaseVertex, 6);
    rb_define_module_function(rb_mGL, "glMultiDrawElementsBaseVertex", rb_glMultiDrawElementsBaseVertex, 6);
    rb_define_module_function(rb_mGL, "glProvokingVertex", rb_glProvokingVertex, 1);
    rb_define_module_function(rb_mGL, "glFenceSync", rb_glFenceSync, 2);
    rb_define_module_function(rb_mGL, "glIsSync", rb_glIsSync, 1);
    rb_define_module_function(rb_mGL, "glDeleteSync", rb_glDeleteSync, 1);
    rb_define_module_function(rb_mGL, "glClientWaitSync", rb_glClientWaitSync, 3);
    rb_define_module_function(rb_mGL, "glWaitSync", rb_glWaitSync, 3);
    rb_define_module_function(rb_mGL, "glGetInteger64v", rb_glGetInteger64v, 2);
    rb_define_module_function(rb_mGL, "glGetSynciv", rb_glGetSynciv, 5);
    rb_define_module_function(rb_mGL, "glGetInteger64i_v", rb_glGetInteger64i_v, 3);
    rb_define_module_function(rb_mGL, "glGetBufferParameteri64v", rb_glGetBufferParameteri64v, 3);
    rb_define_module_function(rb_mGL, "glFramebufferTexture", rb_glFramebufferTexture, 4);
    rb_define_module_function(rb_mGL, "glTexImage2DMultisample", rb_glTexImage2DMultisample, 6);
    rb_define_module_function(rb_mGL, "glTexImage3DMultisample", rb_glTexImage3DMultisample, 7);
    rb_define_module_function(rb_mGL, "glGetMultisamplefv", rb_glGetMultisamplefv, 3);
    rb_define_module_function(rb_mGL, "glSampleMaski", rb_glSampleMaski, 2);
    rb_define_module_function(rb_mGL, "glBindFragDataLocationIndexed", rb_glBindFragDataLocationIndexed, 4);
    rb_define_module_function(rb_mGL, "glGetFragDataIndex", rb_glGetFragDataIndex, 2);
    rb_define_module_function(rb_mGL, "glGenSamplers", rb_glGenSamplers, 2);
    rb_define_module_function(rb_mGL, "glDeleteSamplers", rb_glDeleteSamplers, 2);
    rb_define_module_function(rb_mGL, "glIsSampler", rb_glIsSampler, 1);
    rb_define_module_function(rb_mGL, "glBindSampler", rb_glBindSampler, 2);
    rb_define_module_function(rb_mGL, "glSamplerParameteri", rb_glSamplerParameteri, 3);
    rb_define_module_function(rb_mGL, "glSamplerParameteriv", rb_glSamplerParameteriv, 3);
    rb_define_module_function(rb_mGL, "glSamplerParameterf", rb_glSamplerParameterf, 3);
    rb_define_module_function(rb_mGL, "glSamplerParameterfv", rb_glSamplerParameterfv, 3);
    rb_define_module_function(rb_mGL, "glSamplerParameterIiv", rb_glSamplerParameterIiv, 3);
    rb_define_module_function(rb_mGL, "glSamplerParameterIuiv", rb_glSamplerParameterIuiv, 3);
    rb_define_module_function(rb_mGL, "glGetSamplerParameteriv", rb_glGetSamplerParameteriv, 3);
    rb_define_module_function(rb_mGL, "glGetSamplerParameterIiv", rb_glGetSamplerParameterIiv, 3);
    rb_define_module_function(rb_mGL, "glGetSamplerParameterfv", rb_glGetSamplerParameterfv, 3);
    rb_define_module_function(rb_mGL, "glGetSamplerParameterIuiv", rb_glGetSamplerParameterIuiv, 3);
    rb_define_module_function(rb_mGL, "glQueryCounter", rb_glQueryCounter, 2);
    rb_define_module_function(rb_mGL, "glGetQueryObjecti64v", rb_glGetQueryObjecti64v, 3);
    rb_define_module_function(rb_mGL, "glGetQueryObjectui64v", rb_glGetQueryObjectui64v, 3);
    rb_define_module_function(rb_mGL, "glVertexAttribDivisor", rb_glVertexAttribDivisor, 2);
    rb_define_module_function(rb_mGL, "glVertexAttribP1ui", rb_glVertexAttribP1ui, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribP1uiv", rb_glVertexAttribP1uiv, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribP2ui", rb_glVertexAttribP2ui, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribP2uiv", rb_glVertexAttribP2uiv, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribP3ui", rb_glVertexAttribP3ui, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribP3uiv", rb_glVertexAttribP3uiv, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribP4ui", rb_glVertexAttribP4ui, 4);
    rb_define_module_function(rb_mGL, "glVertexAttribP4uiv", rb_glVertexAttribP4uiv, 4);
    rb_define_module_function(rb_mGL, "glVertexP2ui", rb_glVertexP2ui, 2);
    rb_define_module_function(rb_mGL, "glVertexP2uiv", rb_glVertexP2uiv, 2);
    rb_define_module_function(rb_mGL, "glVertexP3ui", rb_glVertexP3ui, 2);
    rb_define_module_function(rb_mGL, "glVertexP3uiv", rb_glVertexP3uiv, 2);
    rb_define_module_function(rb_mGL, "glVertexP4ui", rb_glVertexP4ui, 2);
    rb_define_module_function(rb_mGL, "glVertexP4uiv", rb_glVertexP4uiv, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP1ui", rb_glTexCoordP1ui, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP1uiv", rb_glTexCoordP1uiv, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP2ui", rb_glTexCoordP2ui, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP2uiv", rb_glTexCoordP2uiv, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP3ui", rb_glTexCoordP3ui, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP3uiv", rb_glTexCoordP3uiv, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP4ui", rb_glTexCoordP4ui, 2);
    rb_define_module_function(rb_mGL, "glTexCoordP4uiv", rb_glTexCoordP4uiv, 2);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP1ui", rb_glMultiTexCoordP1ui, 3);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP1uiv", rb_glMultiTexCoordP1uiv, 3);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP2ui", rb_glMultiTexCoordP2ui, 3);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP2uiv", rb_glMultiTexCoordP2uiv, 3);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP3ui", rb_glMultiTexCoordP3ui, 3);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP3uiv", rb_glMultiTexCoordP3uiv, 3);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP4ui", rb_glMultiTexCoordP4ui, 3);
    rb_define_module_function(rb_mGL, "glMultiTexCoordP4uiv", rb_glMultiTexCoordP4uiv, 3);
    rb_define_module_function(rb_mGL, "glNormalP3ui", rb_glNormalP3ui, 2);
    rb_define_module_function(rb_mGL, "glNormalP3uiv", rb_glNormalP3uiv, 2);
    rb_define_module_function(rb_mGL, "glColorP3ui", rb_glColorP3ui, 2);
    rb_define_module_function(rb_mGL, "glColorP3uiv", rb_glColorP3uiv, 2);
    rb_define_module_function(rb_mGL, "glColorP4ui", rb_glColorP4ui, 2);
    rb_define_module_function(rb_mGL, "glColorP4uiv", rb_glColorP4uiv, 2);
    rb_define_module_function(rb_mGL, "glSecondaryColorP3ui", rb_glSecondaryColorP3ui, 2);
    rb_define_module_function(rb_mGL, "glSecondaryColorP3uiv", rb_glSecondaryColorP3uiv, 2);


    rb_define_const(rb_mGL, "GL_DEPTH_BUFFER_BIT", UINT2NUM(GL_DEPTH_BUFFER_BIT));
    rb_define_const(rb_mGL, "GL_STENCIL_BUFFER_BIT", UINT2NUM(GL_STENCIL_BUFFER_BIT));
    rb_define_const(rb_mGL, "GL_COLOR_BUFFER_BIT", UINT2NUM(GL_COLOR_BUFFER_BIT));
    rb_define_const(rb_mGL, "GL_FALSE", UINT2NUM(GL_FALSE));
    rb_define_const(rb_mGL, "GL_TRUE", UINT2NUM(GL_TRUE));
    rb_define_const(rb_mGL, "GL_POINTS", UINT2NUM(GL_POINTS));
    rb_define_const(rb_mGL, "GL_LINES", UINT2NUM(GL_LINES));
    rb_define_const(rb_mGL, "GL_LINE_LOOP", UINT2NUM(GL_LINE_LOOP));
    rb_define_const(rb_mGL, "GL_LINE_STRIP", UINT2NUM(GL_LINE_STRIP));
    rb_define_const(rb_mGL, "GL_TRIANGLES", UINT2NUM(GL_TRIANGLES));
    rb_define_const(rb_mGL, "GL_TRIANGLE_STRIP", UINT2NUM(GL_TRIANGLE_STRIP));
    rb_define_const(rb_mGL, "GL_TRIANGLE_FAN", UINT2NUM(GL_TRIANGLE_FAN));
    rb_define_const(rb_mGL, "GL_NEVER", UINT2NUM(GL_NEVER));
    rb_define_const(rb_mGL, "GL_LESS", UINT2NUM(GL_LESS));
    rb_define_const(rb_mGL, "GL_EQUAL", UINT2NUM(GL_EQUAL));
    rb_define_const(rb_mGL, "GL_LEQUAL", UINT2NUM(GL_LEQUAL));
    rb_define_const(rb_mGL, "GL_GREATER", UINT2NUM(GL_GREATER));
    rb_define_const(rb_mGL, "GL_NOTEQUAL", UINT2NUM(GL_NOTEQUAL));
    rb_define_const(rb_mGL, "GL_GEQUAL", UINT2NUM(GL_GEQUAL));
    rb_define_const(rb_mGL, "GL_ALWAYS", UINT2NUM(GL_ALWAYS));
    rb_define_const(rb_mGL, "GL_ZERO", UINT2NUM(GL_ZERO));
    rb_define_const(rb_mGL, "GL_ONE", UINT2NUM(GL_ONE));
    rb_define_const(rb_mGL, "GL_SRC_COLOR", UINT2NUM(GL_SRC_COLOR));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_SRC_COLOR", UINT2NUM(GL_ONE_MINUS_SRC_COLOR));
    rb_define_const(rb_mGL, "GL_SRC_ALPHA", UINT2NUM(GL_SRC_ALPHA));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_SRC_ALPHA", UINT2NUM(GL_ONE_MINUS_SRC_ALPHA));
    rb_define_const(rb_mGL, "GL_DST_ALPHA", UINT2NUM(GL_DST_ALPHA));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_DST_ALPHA", UINT2NUM(GL_ONE_MINUS_DST_ALPHA));
    rb_define_const(rb_mGL, "GL_DST_COLOR", UINT2NUM(GL_DST_COLOR));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_DST_COLOR", UINT2NUM(GL_ONE_MINUS_DST_COLOR));
    rb_define_const(rb_mGL, "GL_SRC_ALPHA_SATURATE", UINT2NUM(GL_SRC_ALPHA_SATURATE));
    rb_define_const(rb_mGL, "GL_NONE", UINT2NUM(GL_NONE));
    rb_define_const(rb_mGL, "GL_FRONT_LEFT", UINT2NUM(GL_FRONT_LEFT));
    rb_define_const(rb_mGL, "GL_FRONT_RIGHT", UINT2NUM(GL_FRONT_RIGHT));
    rb_define_const(rb_mGL, "GL_BACK_LEFT", UINT2NUM(GL_BACK_LEFT));
    rb_define_const(rb_mGL, "GL_BACK_RIGHT", UINT2NUM(GL_BACK_RIGHT));
    rb_define_const(rb_mGL, "GL_FRONT", UINT2NUM(GL_FRONT));
    rb_define_const(rb_mGL, "GL_BACK", UINT2NUM(GL_BACK));
    rb_define_const(rb_mGL, "GL_LEFT", UINT2NUM(GL_LEFT));
    rb_define_const(rb_mGL, "GL_RIGHT", UINT2NUM(GL_RIGHT));
    rb_define_const(rb_mGL, "GL_FRONT_AND_BACK", UINT2NUM(GL_FRONT_AND_BACK));
    rb_define_const(rb_mGL, "GL_NO_ERROR", UINT2NUM(GL_NO_ERROR));
    rb_define_const(rb_mGL, "GL_INVALID_ENUM", UINT2NUM(GL_INVALID_ENUM));
    rb_define_const(rb_mGL, "GL_INVALID_VALUE", UINT2NUM(GL_INVALID_VALUE));
    rb_define_const(rb_mGL, "GL_INVALID_OPERATION", UINT2NUM(GL_INVALID_OPERATION));
    rb_define_const(rb_mGL, "GL_OUT_OF_MEMORY", UINT2NUM(GL_OUT_OF_MEMORY));
    rb_define_const(rb_mGL, "GL_CW", UINT2NUM(GL_CW));
    rb_define_const(rb_mGL, "GL_CCW", UINT2NUM(GL_CCW));
    rb_define_const(rb_mGL, "GL_POINT_SIZE", UINT2NUM(GL_POINT_SIZE));
    rb_define_const(rb_mGL, "GL_POINT_SIZE_RANGE", UINT2NUM(GL_POINT_SIZE_RANGE));
    rb_define_const(rb_mGL, "GL_POINT_SIZE_GRANULARITY", UINT2NUM(GL_POINT_SIZE_GRANULARITY));
    rb_define_const(rb_mGL, "GL_LINE_SMOOTH", UINT2NUM(GL_LINE_SMOOTH));
    rb_define_const(rb_mGL, "GL_LINE_WIDTH", UINT2NUM(GL_LINE_WIDTH));
    rb_define_const(rb_mGL, "GL_LINE_WIDTH_RANGE", UINT2NUM(GL_LINE_WIDTH_RANGE));
    rb_define_const(rb_mGL, "GL_LINE_WIDTH_GRANULARITY", UINT2NUM(GL_LINE_WIDTH_GRANULARITY));
    rb_define_const(rb_mGL, "GL_POLYGON_MODE", UINT2NUM(GL_POLYGON_MODE));
    rb_define_const(rb_mGL, "GL_POLYGON_SMOOTH", UINT2NUM(GL_POLYGON_SMOOTH));
    rb_define_const(rb_mGL, "GL_CULL_FACE", UINT2NUM(GL_CULL_FACE));
    rb_define_const(rb_mGL, "GL_CULL_FACE_MODE", UINT2NUM(GL_CULL_FACE_MODE));
    rb_define_const(rb_mGL, "GL_FRONT_FACE", UINT2NUM(GL_FRONT_FACE));
    rb_define_const(rb_mGL, "GL_DEPTH_RANGE", UINT2NUM(GL_DEPTH_RANGE));
    rb_define_const(rb_mGL, "GL_DEPTH_TEST", UINT2NUM(GL_DEPTH_TEST));
    rb_define_const(rb_mGL, "GL_DEPTH_WRITEMASK", UINT2NUM(GL_DEPTH_WRITEMASK));
    rb_define_const(rb_mGL, "GL_DEPTH_CLEAR_VALUE", UINT2NUM(GL_DEPTH_CLEAR_VALUE));
    rb_define_const(rb_mGL, "GL_DEPTH_FUNC", UINT2NUM(GL_DEPTH_FUNC));
    rb_define_const(rb_mGL, "GL_STENCIL_TEST", UINT2NUM(GL_STENCIL_TEST));
    rb_define_const(rb_mGL, "GL_STENCIL_CLEAR_VALUE", UINT2NUM(GL_STENCIL_CLEAR_VALUE));
    rb_define_const(rb_mGL, "GL_STENCIL_FUNC", UINT2NUM(GL_STENCIL_FUNC));
    rb_define_const(rb_mGL, "GL_STENCIL_VALUE_MASK", UINT2NUM(GL_STENCIL_VALUE_MASK));
    rb_define_const(rb_mGL, "GL_STENCIL_FAIL", UINT2NUM(GL_STENCIL_FAIL));
    rb_define_const(rb_mGL, "GL_STENCIL_PASS_DEPTH_FAIL", UINT2NUM(GL_STENCIL_PASS_DEPTH_FAIL));
    rb_define_const(rb_mGL, "GL_STENCIL_PASS_DEPTH_PASS", UINT2NUM(GL_STENCIL_PASS_DEPTH_PASS));
    rb_define_const(rb_mGL, "GL_STENCIL_REF", UINT2NUM(GL_STENCIL_REF));
    rb_define_const(rb_mGL, "GL_STENCIL_WRITEMASK", UINT2NUM(GL_STENCIL_WRITEMASK));
    rb_define_const(rb_mGL, "GL_VIEWPORT", UINT2NUM(GL_VIEWPORT));
    rb_define_const(rb_mGL, "GL_DITHER", UINT2NUM(GL_DITHER));
    rb_define_const(rb_mGL, "GL_BLEND_DST", UINT2NUM(GL_BLEND_DST));
    rb_define_const(rb_mGL, "GL_BLEND_SRC", UINT2NUM(GL_BLEND_SRC));
    rb_define_const(rb_mGL, "GL_BLEND", UINT2NUM(GL_BLEND));
    rb_define_const(rb_mGL, "GL_LOGIC_OP_MODE", UINT2NUM(GL_LOGIC_OP_MODE));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER", UINT2NUM(GL_DRAW_BUFFER));
    rb_define_const(rb_mGL, "GL_READ_BUFFER", UINT2NUM(GL_READ_BUFFER));
    rb_define_const(rb_mGL, "GL_SCISSOR_BOX", UINT2NUM(GL_SCISSOR_BOX));
    rb_define_const(rb_mGL, "GL_SCISSOR_TEST", UINT2NUM(GL_SCISSOR_TEST));
    rb_define_const(rb_mGL, "GL_COLOR_CLEAR_VALUE", UINT2NUM(GL_COLOR_CLEAR_VALUE));
    rb_define_const(rb_mGL, "GL_COLOR_WRITEMASK", UINT2NUM(GL_COLOR_WRITEMASK));
    rb_define_const(rb_mGL, "GL_DOUBLEBUFFER", UINT2NUM(GL_DOUBLEBUFFER));
    rb_define_const(rb_mGL, "GL_STEREO", UINT2NUM(GL_STEREO));
    rb_define_const(rb_mGL, "GL_LINE_SMOOTH_HINT", UINT2NUM(GL_LINE_SMOOTH_HINT));
    rb_define_const(rb_mGL, "GL_POLYGON_SMOOTH_HINT", UINT2NUM(GL_POLYGON_SMOOTH_HINT));
    rb_define_const(rb_mGL, "GL_UNPACK_SWAP_BYTES", UINT2NUM(GL_UNPACK_SWAP_BYTES));
    rb_define_const(rb_mGL, "GL_UNPACK_LSB_FIRST", UINT2NUM(GL_UNPACK_LSB_FIRST));
    rb_define_const(rb_mGL, "GL_UNPACK_ROW_LENGTH", UINT2NUM(GL_UNPACK_ROW_LENGTH));
    rb_define_const(rb_mGL, "GL_UNPACK_SKIP_ROWS", UINT2NUM(GL_UNPACK_SKIP_ROWS));
    rb_define_const(rb_mGL, "GL_UNPACK_SKIP_PIXELS", UINT2NUM(GL_UNPACK_SKIP_PIXELS));
    rb_define_const(rb_mGL, "GL_UNPACK_ALIGNMENT", UINT2NUM(GL_UNPACK_ALIGNMENT));
    rb_define_const(rb_mGL, "GL_PACK_SWAP_BYTES", UINT2NUM(GL_PACK_SWAP_BYTES));
    rb_define_const(rb_mGL, "GL_PACK_LSB_FIRST", UINT2NUM(GL_PACK_LSB_FIRST));
    rb_define_const(rb_mGL, "GL_PACK_ROW_LENGTH", UINT2NUM(GL_PACK_ROW_LENGTH));
    rb_define_const(rb_mGL, "GL_PACK_SKIP_ROWS", UINT2NUM(GL_PACK_SKIP_ROWS));
    rb_define_const(rb_mGL, "GL_PACK_SKIP_PIXELS", UINT2NUM(GL_PACK_SKIP_PIXELS));
    rb_define_const(rb_mGL, "GL_PACK_ALIGNMENT", UINT2NUM(GL_PACK_ALIGNMENT));
    rb_define_const(rb_mGL, "GL_MAX_TEXTURE_SIZE", UINT2NUM(GL_MAX_TEXTURE_SIZE));
    rb_define_const(rb_mGL, "GL_MAX_VIEWPORT_DIMS", UINT2NUM(GL_MAX_VIEWPORT_DIMS));
    rb_define_const(rb_mGL, "GL_SUBPIXEL_BITS", UINT2NUM(GL_SUBPIXEL_BITS));
    rb_define_const(rb_mGL, "GL_TEXTURE_1D", UINT2NUM(GL_TEXTURE_1D));
    rb_define_const(rb_mGL, "GL_TEXTURE_2D", UINT2NUM(GL_TEXTURE_2D));
    rb_define_const(rb_mGL, "GL_TEXTURE_WIDTH", UINT2NUM(GL_TEXTURE_WIDTH));
    rb_define_const(rb_mGL, "GL_TEXTURE_HEIGHT", UINT2NUM(GL_TEXTURE_HEIGHT));
    rb_define_const(rb_mGL, "GL_TEXTURE_BORDER_COLOR", UINT2NUM(GL_TEXTURE_BORDER_COLOR));
    rb_define_const(rb_mGL, "GL_DONT_CARE", UINT2NUM(GL_DONT_CARE));
    rb_define_const(rb_mGL, "GL_FASTEST", UINT2NUM(GL_FASTEST));
    rb_define_const(rb_mGL, "GL_NICEST", UINT2NUM(GL_NICEST));
    rb_define_const(rb_mGL, "GL_BYTE", UINT2NUM(GL_BYTE));
    rb_define_const(rb_mGL, "GL_UNSIGNED_BYTE", UINT2NUM(GL_UNSIGNED_BYTE));
    rb_define_const(rb_mGL, "GL_SHORT", UINT2NUM(GL_SHORT));
    rb_define_const(rb_mGL, "GL_UNSIGNED_SHORT", UINT2NUM(GL_UNSIGNED_SHORT));
    rb_define_const(rb_mGL, "GL_INT", UINT2NUM(GL_INT));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT", UINT2NUM(GL_UNSIGNED_INT));
    rb_define_const(rb_mGL, "GL_FLOAT", UINT2NUM(GL_FLOAT));
    rb_define_const(rb_mGL, "GL_CLEAR", UINT2NUM(GL_CLEAR));
    rb_define_const(rb_mGL, "GL_AND", UINT2NUM(GL_AND));
    rb_define_const(rb_mGL, "GL_AND_REVERSE", UINT2NUM(GL_AND_REVERSE));
    rb_define_const(rb_mGL, "GL_COPY", UINT2NUM(GL_COPY));
    rb_define_const(rb_mGL, "GL_AND_INVERTED", UINT2NUM(GL_AND_INVERTED));
    rb_define_const(rb_mGL, "GL_NOOP", UINT2NUM(GL_NOOP));
    rb_define_const(rb_mGL, "GL_XOR", UINT2NUM(GL_XOR));
    rb_define_const(rb_mGL, "GL_OR", UINT2NUM(GL_OR));
    rb_define_const(rb_mGL, "GL_NOR", UINT2NUM(GL_NOR));
    rb_define_const(rb_mGL, "GL_EQUIV", UINT2NUM(GL_EQUIV));
    rb_define_const(rb_mGL, "GL_INVERT", UINT2NUM(GL_INVERT));
    rb_define_const(rb_mGL, "GL_OR_REVERSE", UINT2NUM(GL_OR_REVERSE));
    rb_define_const(rb_mGL, "GL_COPY_INVERTED", UINT2NUM(GL_COPY_INVERTED));
    rb_define_const(rb_mGL, "GL_OR_INVERTED", UINT2NUM(GL_OR_INVERTED));
    rb_define_const(rb_mGL, "GL_NAND", UINT2NUM(GL_NAND));
    rb_define_const(rb_mGL, "GL_SET", UINT2NUM(GL_SET));
    rb_define_const(rb_mGL, "GL_TEXTURE", UINT2NUM(GL_TEXTURE));
    rb_define_const(rb_mGL, "GL_COLOR", UINT2NUM(GL_COLOR));
    rb_define_const(rb_mGL, "GL_DEPTH", UINT2NUM(GL_DEPTH));
    rb_define_const(rb_mGL, "GL_STENCIL", UINT2NUM(GL_STENCIL));
    rb_define_const(rb_mGL, "GL_STENCIL_INDEX", UINT2NUM(GL_STENCIL_INDEX));
    rb_define_const(rb_mGL, "GL_DEPTH_COMPONENT", UINT2NUM(GL_DEPTH_COMPONENT));
    rb_define_const(rb_mGL, "GL_RED", UINT2NUM(GL_RED));
    rb_define_const(rb_mGL, "GL_GREEN", UINT2NUM(GL_GREEN));
    rb_define_const(rb_mGL, "GL_BLUE", UINT2NUM(GL_BLUE));
    rb_define_const(rb_mGL, "GL_ALPHA", UINT2NUM(GL_ALPHA));
    rb_define_const(rb_mGL, "GL_RGB", UINT2NUM(GL_RGB));
    rb_define_const(rb_mGL, "GL_RGBA", UINT2NUM(GL_RGBA));
    rb_define_const(rb_mGL, "GL_POINT", UINT2NUM(GL_POINT));
    rb_define_const(rb_mGL, "GL_LINE", UINT2NUM(GL_LINE));
    rb_define_const(rb_mGL, "GL_FILL", UINT2NUM(GL_FILL));
    rb_define_const(rb_mGL, "GL_KEEP", UINT2NUM(GL_KEEP));
    rb_define_const(rb_mGL, "GL_REPLACE", UINT2NUM(GL_REPLACE));
    rb_define_const(rb_mGL, "GL_INCR", UINT2NUM(GL_INCR));
    rb_define_const(rb_mGL, "GL_DECR", UINT2NUM(GL_DECR));
    rb_define_const(rb_mGL, "GL_VENDOR", UINT2NUM(GL_VENDOR));
    rb_define_const(rb_mGL, "GL_RENDERER", UINT2NUM(GL_RENDERER));
    rb_define_const(rb_mGL, "GL_VERSION", UINT2NUM(GL_VERSION));
    rb_define_const(rb_mGL, "GL_EXTENSIONS", UINT2NUM(GL_EXTENSIONS));
    rb_define_const(rb_mGL, "GL_NEAREST", UINT2NUM(GL_NEAREST));
    rb_define_const(rb_mGL, "GL_LINEAR", UINT2NUM(GL_LINEAR));
    rb_define_const(rb_mGL, "GL_NEAREST_MIPMAP_NEAREST", UINT2NUM(GL_NEAREST_MIPMAP_NEAREST));
    rb_define_const(rb_mGL, "GL_LINEAR_MIPMAP_NEAREST", UINT2NUM(GL_LINEAR_MIPMAP_NEAREST));
    rb_define_const(rb_mGL, "GL_NEAREST_MIPMAP_LINEAR", UINT2NUM(GL_NEAREST_MIPMAP_LINEAR));
    rb_define_const(rb_mGL, "GL_LINEAR_MIPMAP_LINEAR", UINT2NUM(GL_LINEAR_MIPMAP_LINEAR));
    rb_define_const(rb_mGL, "GL_TEXTURE_MAG_FILTER", UINT2NUM(GL_TEXTURE_MAG_FILTER));
    rb_define_const(rb_mGL, "GL_TEXTURE_MIN_FILTER", UINT2NUM(GL_TEXTURE_MIN_FILTER));
    rb_define_const(rb_mGL, "GL_TEXTURE_WRAP_S", UINT2NUM(GL_TEXTURE_WRAP_S));
    rb_define_const(rb_mGL, "GL_TEXTURE_WRAP_T", UINT2NUM(GL_TEXTURE_WRAP_T));
    rb_define_const(rb_mGL, "GL_REPEAT", UINT2NUM(GL_REPEAT));
    rb_define_const(rb_mGL, "GL_COLOR_LOGIC_OP", UINT2NUM(GL_COLOR_LOGIC_OP));
    rb_define_const(rb_mGL, "GL_POLYGON_OFFSET_UNITS", UINT2NUM(GL_POLYGON_OFFSET_UNITS));
    rb_define_const(rb_mGL, "GL_POLYGON_OFFSET_POINT", UINT2NUM(GL_POLYGON_OFFSET_POINT));
    rb_define_const(rb_mGL, "GL_POLYGON_OFFSET_LINE", UINT2NUM(GL_POLYGON_OFFSET_LINE));
    rb_define_const(rb_mGL, "GL_POLYGON_OFFSET_FILL", UINT2NUM(GL_POLYGON_OFFSET_FILL));
    rb_define_const(rb_mGL, "GL_POLYGON_OFFSET_FACTOR", UINT2NUM(GL_POLYGON_OFFSET_FACTOR));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_1D", UINT2NUM(GL_TEXTURE_BINDING_1D));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_2D", UINT2NUM(GL_TEXTURE_BINDING_2D));
    rb_define_const(rb_mGL, "GL_TEXTURE_INTERNAL_FORMAT", UINT2NUM(GL_TEXTURE_INTERNAL_FORMAT));
    rb_define_const(rb_mGL, "GL_TEXTURE_RED_SIZE", UINT2NUM(GL_TEXTURE_RED_SIZE));
    rb_define_const(rb_mGL, "GL_TEXTURE_GREEN_SIZE", UINT2NUM(GL_TEXTURE_GREEN_SIZE));
    rb_define_const(rb_mGL, "GL_TEXTURE_BLUE_SIZE", UINT2NUM(GL_TEXTURE_BLUE_SIZE));
    rb_define_const(rb_mGL, "GL_TEXTURE_ALPHA_SIZE", UINT2NUM(GL_TEXTURE_ALPHA_SIZE));
    rb_define_const(rb_mGL, "GL_DOUBLE", UINT2NUM(GL_DOUBLE));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_1D", UINT2NUM(GL_PROXY_TEXTURE_1D));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_2D", UINT2NUM(GL_PROXY_TEXTURE_2D));
    rb_define_const(rb_mGL, "GL_R3_G3_B2", UINT2NUM(GL_R3_G3_B2));
    rb_define_const(rb_mGL, "GL_RGB4", UINT2NUM(GL_RGB4));
    rb_define_const(rb_mGL, "GL_RGB5", UINT2NUM(GL_RGB5));
    rb_define_const(rb_mGL, "GL_RGB8", UINT2NUM(GL_RGB8));
    rb_define_const(rb_mGL, "GL_RGB10", UINT2NUM(GL_RGB10));
    rb_define_const(rb_mGL, "GL_RGB12", UINT2NUM(GL_RGB12));
    rb_define_const(rb_mGL, "GL_RGB16", UINT2NUM(GL_RGB16));
    rb_define_const(rb_mGL, "GL_RGBA2", UINT2NUM(GL_RGBA2));
    rb_define_const(rb_mGL, "GL_RGBA4", UINT2NUM(GL_RGBA4));
    rb_define_const(rb_mGL, "GL_RGB5_A1", UINT2NUM(GL_RGB5_A1));
    rb_define_const(rb_mGL, "GL_RGBA8", UINT2NUM(GL_RGBA8));
    rb_define_const(rb_mGL, "GL_RGB10_A2", UINT2NUM(GL_RGB10_A2));
    rb_define_const(rb_mGL, "GL_RGBA12", UINT2NUM(GL_RGBA12));
    rb_define_const(rb_mGL, "GL_RGBA16", UINT2NUM(GL_RGBA16));
    rb_define_const(rb_mGL, "GL_UNSIGNED_BYTE_3_3_2", UINT2NUM(GL_UNSIGNED_BYTE_3_3_2));
    rb_define_const(rb_mGL, "GL_UNSIGNED_SHORT_4_4_4_4", UINT2NUM(GL_UNSIGNED_SHORT_4_4_4_4));
    rb_define_const(rb_mGL, "GL_UNSIGNED_SHORT_5_5_5_1", UINT2NUM(GL_UNSIGNED_SHORT_5_5_5_1));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_8_8_8_8", UINT2NUM(GL_UNSIGNED_INT_8_8_8_8));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_10_10_10_2", UINT2NUM(GL_UNSIGNED_INT_10_10_10_2));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_3D", UINT2NUM(GL_TEXTURE_BINDING_3D));
    rb_define_const(rb_mGL, "GL_PACK_SKIP_IMAGES", UINT2NUM(GL_PACK_SKIP_IMAGES));
    rb_define_const(rb_mGL, "GL_PACK_IMAGE_HEIGHT", UINT2NUM(GL_PACK_IMAGE_HEIGHT));
    rb_define_const(rb_mGL, "GL_UNPACK_SKIP_IMAGES", UINT2NUM(GL_UNPACK_SKIP_IMAGES));
    rb_define_const(rb_mGL, "GL_UNPACK_IMAGE_HEIGHT", UINT2NUM(GL_UNPACK_IMAGE_HEIGHT));
    rb_define_const(rb_mGL, "GL_TEXTURE_3D", UINT2NUM(GL_TEXTURE_3D));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_3D", UINT2NUM(GL_PROXY_TEXTURE_3D));
    rb_define_const(rb_mGL, "GL_TEXTURE_DEPTH", UINT2NUM(GL_TEXTURE_DEPTH));
    rb_define_const(rb_mGL, "GL_TEXTURE_WRAP_R", UINT2NUM(GL_TEXTURE_WRAP_R));
    rb_define_const(rb_mGL, "GL_MAX_3D_TEXTURE_SIZE", UINT2NUM(GL_MAX_3D_TEXTURE_SIZE));
    rb_define_const(rb_mGL, "GL_UNSIGNED_BYTE_2_3_3_REV", UINT2NUM(GL_UNSIGNED_BYTE_2_3_3_REV));
    rb_define_const(rb_mGL, "GL_UNSIGNED_SHORT_5_6_5", UINT2NUM(GL_UNSIGNED_SHORT_5_6_5));
    rb_define_const(rb_mGL, "GL_UNSIGNED_SHORT_5_6_5_REV", UINT2NUM(GL_UNSIGNED_SHORT_5_6_5_REV));
    rb_define_const(rb_mGL, "GL_UNSIGNED_SHORT_4_4_4_4_REV", UINT2NUM(GL_UNSIGNED_SHORT_4_4_4_4_REV));
    rb_define_const(rb_mGL, "GL_UNSIGNED_SHORT_1_5_5_5_REV", UINT2NUM(GL_UNSIGNED_SHORT_1_5_5_5_REV));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_8_8_8_8_REV", UINT2NUM(GL_UNSIGNED_INT_8_8_8_8_REV));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_2_10_10_10_REV", UINT2NUM(GL_UNSIGNED_INT_2_10_10_10_REV));
    rb_define_const(rb_mGL, "GL_BGR", UINT2NUM(GL_BGR));
    rb_define_const(rb_mGL, "GL_BGRA", UINT2NUM(GL_BGRA));
    rb_define_const(rb_mGL, "GL_MAX_ELEMENTS_VERTICES", UINT2NUM(GL_MAX_ELEMENTS_VERTICES));
    rb_define_const(rb_mGL, "GL_MAX_ELEMENTS_INDICES", UINT2NUM(GL_MAX_ELEMENTS_INDICES));
    rb_define_const(rb_mGL, "GL_CLAMP_TO_EDGE", UINT2NUM(GL_CLAMP_TO_EDGE));
    rb_define_const(rb_mGL, "GL_TEXTURE_MIN_LOD", UINT2NUM(GL_TEXTURE_MIN_LOD));
    rb_define_const(rb_mGL, "GL_TEXTURE_MAX_LOD", UINT2NUM(GL_TEXTURE_MAX_LOD));
    rb_define_const(rb_mGL, "GL_TEXTURE_BASE_LEVEL", UINT2NUM(GL_TEXTURE_BASE_LEVEL));
    rb_define_const(rb_mGL, "GL_TEXTURE_MAX_LEVEL", UINT2NUM(GL_TEXTURE_MAX_LEVEL));
    rb_define_const(rb_mGL, "GL_SMOOTH_POINT_SIZE_RANGE", UINT2NUM(GL_SMOOTH_POINT_SIZE_RANGE));
    rb_define_const(rb_mGL, "GL_SMOOTH_POINT_SIZE_GRANULARITY", UINT2NUM(GL_SMOOTH_POINT_SIZE_GRANULARITY));
    rb_define_const(rb_mGL, "GL_SMOOTH_LINE_WIDTH_RANGE", UINT2NUM(GL_SMOOTH_LINE_WIDTH_RANGE));
    rb_define_const(rb_mGL, "GL_SMOOTH_LINE_WIDTH_GRANULARITY", UINT2NUM(GL_SMOOTH_LINE_WIDTH_GRANULARITY));
    rb_define_const(rb_mGL, "GL_ALIASED_LINE_WIDTH_RANGE", UINT2NUM(GL_ALIASED_LINE_WIDTH_RANGE));
    rb_define_const(rb_mGL, "GL_TEXTURE0", UINT2NUM(GL_TEXTURE0));
    rb_define_const(rb_mGL, "GL_TEXTURE1", UINT2NUM(GL_TEXTURE1));
    rb_define_const(rb_mGL, "GL_TEXTURE2", UINT2NUM(GL_TEXTURE2));
    rb_define_const(rb_mGL, "GL_TEXTURE3", UINT2NUM(GL_TEXTURE3));
    rb_define_const(rb_mGL, "GL_TEXTURE4", UINT2NUM(GL_TEXTURE4));
    rb_define_const(rb_mGL, "GL_TEXTURE5", UINT2NUM(GL_TEXTURE5));
    rb_define_const(rb_mGL, "GL_TEXTURE6", UINT2NUM(GL_TEXTURE6));
    rb_define_const(rb_mGL, "GL_TEXTURE7", UINT2NUM(GL_TEXTURE7));
    rb_define_const(rb_mGL, "GL_TEXTURE8", UINT2NUM(GL_TEXTURE8));
    rb_define_const(rb_mGL, "GL_TEXTURE9", UINT2NUM(GL_TEXTURE9));
    rb_define_const(rb_mGL, "GL_TEXTURE10", UINT2NUM(GL_TEXTURE10));
    rb_define_const(rb_mGL, "GL_TEXTURE11", UINT2NUM(GL_TEXTURE11));
    rb_define_const(rb_mGL, "GL_TEXTURE12", UINT2NUM(GL_TEXTURE12));
    rb_define_const(rb_mGL, "GL_TEXTURE13", UINT2NUM(GL_TEXTURE13));
    rb_define_const(rb_mGL, "GL_TEXTURE14", UINT2NUM(GL_TEXTURE14));
    rb_define_const(rb_mGL, "GL_TEXTURE15", UINT2NUM(GL_TEXTURE15));
    rb_define_const(rb_mGL, "GL_TEXTURE16", UINT2NUM(GL_TEXTURE16));
    rb_define_const(rb_mGL, "GL_TEXTURE17", UINT2NUM(GL_TEXTURE17));
    rb_define_const(rb_mGL, "GL_TEXTURE18", UINT2NUM(GL_TEXTURE18));
    rb_define_const(rb_mGL, "GL_TEXTURE19", UINT2NUM(GL_TEXTURE19));
    rb_define_const(rb_mGL, "GL_TEXTURE20", UINT2NUM(GL_TEXTURE20));
    rb_define_const(rb_mGL, "GL_TEXTURE21", UINT2NUM(GL_TEXTURE21));
    rb_define_const(rb_mGL, "GL_TEXTURE22", UINT2NUM(GL_TEXTURE22));
    rb_define_const(rb_mGL, "GL_TEXTURE23", UINT2NUM(GL_TEXTURE23));
    rb_define_const(rb_mGL, "GL_TEXTURE24", UINT2NUM(GL_TEXTURE24));
    rb_define_const(rb_mGL, "GL_TEXTURE25", UINT2NUM(GL_TEXTURE25));
    rb_define_const(rb_mGL, "GL_TEXTURE26", UINT2NUM(GL_TEXTURE26));
    rb_define_const(rb_mGL, "GL_TEXTURE27", UINT2NUM(GL_TEXTURE27));
    rb_define_const(rb_mGL, "GL_TEXTURE28", UINT2NUM(GL_TEXTURE28));
    rb_define_const(rb_mGL, "GL_TEXTURE29", UINT2NUM(GL_TEXTURE29));
    rb_define_const(rb_mGL, "GL_TEXTURE30", UINT2NUM(GL_TEXTURE30));
    rb_define_const(rb_mGL, "GL_TEXTURE31", UINT2NUM(GL_TEXTURE31));
    rb_define_const(rb_mGL, "GL_ACTIVE_TEXTURE", UINT2NUM(GL_ACTIVE_TEXTURE));
    rb_define_const(rb_mGL, "GL_MULTISAMPLE", UINT2NUM(GL_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_SAMPLE_ALPHA_TO_COVERAGE", UINT2NUM(GL_SAMPLE_ALPHA_TO_COVERAGE));
    rb_define_const(rb_mGL, "GL_SAMPLE_ALPHA_TO_ONE", UINT2NUM(GL_SAMPLE_ALPHA_TO_ONE));
    rb_define_const(rb_mGL, "GL_SAMPLE_COVERAGE", UINT2NUM(GL_SAMPLE_COVERAGE));
    rb_define_const(rb_mGL, "GL_SAMPLE_BUFFERS", UINT2NUM(GL_SAMPLE_BUFFERS));
    rb_define_const(rb_mGL, "GL_SAMPLES", UINT2NUM(GL_SAMPLES));
    rb_define_const(rb_mGL, "GL_SAMPLE_COVERAGE_VALUE", UINT2NUM(GL_SAMPLE_COVERAGE_VALUE));
    rb_define_const(rb_mGL, "GL_SAMPLE_COVERAGE_INVERT", UINT2NUM(GL_SAMPLE_COVERAGE_INVERT));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP", UINT2NUM(GL_TEXTURE_CUBE_MAP));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_CUBE_MAP", UINT2NUM(GL_TEXTURE_BINDING_CUBE_MAP));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP_POSITIVE_X", UINT2NUM(GL_TEXTURE_CUBE_MAP_POSITIVE_X));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP_NEGATIVE_X", UINT2NUM(GL_TEXTURE_CUBE_MAP_NEGATIVE_X));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP_POSITIVE_Y", UINT2NUM(GL_TEXTURE_CUBE_MAP_POSITIVE_Y));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y", UINT2NUM(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP_POSITIVE_Z", UINT2NUM(GL_TEXTURE_CUBE_MAP_POSITIVE_Z));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z", UINT2NUM(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_CUBE_MAP", UINT2NUM(GL_PROXY_TEXTURE_CUBE_MAP));
    rb_define_const(rb_mGL, "GL_MAX_CUBE_MAP_TEXTURE_SIZE", UINT2NUM(GL_MAX_CUBE_MAP_TEXTURE_SIZE));
    rb_define_const(rb_mGL, "GL_COMPRESSED_RGB", UINT2NUM(GL_COMPRESSED_RGB));
    rb_define_const(rb_mGL, "GL_COMPRESSED_RGBA", UINT2NUM(GL_COMPRESSED_RGBA));
    rb_define_const(rb_mGL, "GL_TEXTURE_COMPRESSION_HINT", UINT2NUM(GL_TEXTURE_COMPRESSION_HINT));
    rb_define_const(rb_mGL, "GL_TEXTURE_COMPRESSED_IMAGE_SIZE", UINT2NUM(GL_TEXTURE_COMPRESSED_IMAGE_SIZE));
    rb_define_const(rb_mGL, "GL_TEXTURE_COMPRESSED", UINT2NUM(GL_TEXTURE_COMPRESSED));
    rb_define_const(rb_mGL, "GL_NUM_COMPRESSED_TEXTURE_FORMATS", UINT2NUM(GL_NUM_COMPRESSED_TEXTURE_FORMATS));
    rb_define_const(rb_mGL, "GL_COMPRESSED_TEXTURE_FORMATS", UINT2NUM(GL_COMPRESSED_TEXTURE_FORMATS));
    rb_define_const(rb_mGL, "GL_CLAMP_TO_BORDER", UINT2NUM(GL_CLAMP_TO_BORDER));
    rb_define_const(rb_mGL, "GL_BLEND_DST_RGB", UINT2NUM(GL_BLEND_DST_RGB));
    rb_define_const(rb_mGL, "GL_BLEND_SRC_RGB", UINT2NUM(GL_BLEND_SRC_RGB));
    rb_define_const(rb_mGL, "GL_BLEND_DST_ALPHA", UINT2NUM(GL_BLEND_DST_ALPHA));
    rb_define_const(rb_mGL, "GL_BLEND_SRC_ALPHA", UINT2NUM(GL_BLEND_SRC_ALPHA));
    rb_define_const(rb_mGL, "GL_POINT_FADE_THRESHOLD_SIZE", UINT2NUM(GL_POINT_FADE_THRESHOLD_SIZE));
    rb_define_const(rb_mGL, "GL_DEPTH_COMPONENT16", UINT2NUM(GL_DEPTH_COMPONENT16));
    rb_define_const(rb_mGL, "GL_DEPTH_COMPONENT24", UINT2NUM(GL_DEPTH_COMPONENT24));
    rb_define_const(rb_mGL, "GL_DEPTH_COMPONENT32", UINT2NUM(GL_DEPTH_COMPONENT32));
    rb_define_const(rb_mGL, "GL_MIRRORED_REPEAT", UINT2NUM(GL_MIRRORED_REPEAT));
    rb_define_const(rb_mGL, "GL_MAX_TEXTURE_LOD_BIAS", UINT2NUM(GL_MAX_TEXTURE_LOD_BIAS));
    rb_define_const(rb_mGL, "GL_TEXTURE_LOD_BIAS", UINT2NUM(GL_TEXTURE_LOD_BIAS));
    rb_define_const(rb_mGL, "GL_INCR_WRAP", UINT2NUM(GL_INCR_WRAP));
    rb_define_const(rb_mGL, "GL_DECR_WRAP", UINT2NUM(GL_DECR_WRAP));
    rb_define_const(rb_mGL, "GL_TEXTURE_DEPTH_SIZE", UINT2NUM(GL_TEXTURE_DEPTH_SIZE));
    rb_define_const(rb_mGL, "GL_TEXTURE_COMPARE_MODE", UINT2NUM(GL_TEXTURE_COMPARE_MODE));
    rb_define_const(rb_mGL, "GL_TEXTURE_COMPARE_FUNC", UINT2NUM(GL_TEXTURE_COMPARE_FUNC));
    rb_define_const(rb_mGL, "GL_BLEND_COLOR", UINT2NUM(GL_BLEND_COLOR));
    rb_define_const(rb_mGL, "GL_BLEND_EQUATION", UINT2NUM(GL_BLEND_EQUATION));
    rb_define_const(rb_mGL, "GL_CONSTANT_COLOR", UINT2NUM(GL_CONSTANT_COLOR));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_CONSTANT_COLOR", UINT2NUM(GL_ONE_MINUS_CONSTANT_COLOR));
    rb_define_const(rb_mGL, "GL_CONSTANT_ALPHA", UINT2NUM(GL_CONSTANT_ALPHA));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_CONSTANT_ALPHA", UINT2NUM(GL_ONE_MINUS_CONSTANT_ALPHA));
    rb_define_const(rb_mGL, "GL_FUNC_ADD", UINT2NUM(GL_FUNC_ADD));
    rb_define_const(rb_mGL, "GL_FUNC_REVERSE_SUBTRACT", UINT2NUM(GL_FUNC_REVERSE_SUBTRACT));
    rb_define_const(rb_mGL, "GL_FUNC_SUBTRACT", UINT2NUM(GL_FUNC_SUBTRACT));
    rb_define_const(rb_mGL, "GL_MIN", UINT2NUM(GL_MIN));
    rb_define_const(rb_mGL, "GL_MAX", UINT2NUM(GL_MAX));
    rb_define_const(rb_mGL, "GL_BUFFER_SIZE", UINT2NUM(GL_BUFFER_SIZE));
    rb_define_const(rb_mGL, "GL_BUFFER_USAGE", UINT2NUM(GL_BUFFER_USAGE));
    rb_define_const(rb_mGL, "GL_QUERY_COUNTER_BITS", UINT2NUM(GL_QUERY_COUNTER_BITS));
    rb_define_const(rb_mGL, "GL_CURRENT_QUERY", UINT2NUM(GL_CURRENT_QUERY));
    rb_define_const(rb_mGL, "GL_QUERY_RESULT", UINT2NUM(GL_QUERY_RESULT));
    rb_define_const(rb_mGL, "GL_QUERY_RESULT_AVAILABLE", UINT2NUM(GL_QUERY_RESULT_AVAILABLE));
    rb_define_const(rb_mGL, "GL_ARRAY_BUFFER", UINT2NUM(GL_ARRAY_BUFFER));
    rb_define_const(rb_mGL, "GL_ELEMENT_ARRAY_BUFFER", UINT2NUM(GL_ELEMENT_ARRAY_BUFFER));
    rb_define_const(rb_mGL, "GL_ARRAY_BUFFER_BINDING", UINT2NUM(GL_ARRAY_BUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_ELEMENT_ARRAY_BUFFER_BINDING", UINT2NUM(GL_ELEMENT_ARRAY_BUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_READ_ONLY", UINT2NUM(GL_READ_ONLY));
    rb_define_const(rb_mGL, "GL_WRITE_ONLY", UINT2NUM(GL_WRITE_ONLY));
    rb_define_const(rb_mGL, "GL_READ_WRITE", UINT2NUM(GL_READ_WRITE));
    rb_define_const(rb_mGL, "GL_BUFFER_ACCESS", UINT2NUM(GL_BUFFER_ACCESS));
    rb_define_const(rb_mGL, "GL_BUFFER_MAPPED", UINT2NUM(GL_BUFFER_MAPPED));
    rb_define_const(rb_mGL, "GL_BUFFER_MAP_POINTER", UINT2NUM(GL_BUFFER_MAP_POINTER));
    rb_define_const(rb_mGL, "GL_STREAM_DRAW", UINT2NUM(GL_STREAM_DRAW));
    rb_define_const(rb_mGL, "GL_STREAM_READ", UINT2NUM(GL_STREAM_READ));
    rb_define_const(rb_mGL, "GL_STREAM_COPY", UINT2NUM(GL_STREAM_COPY));
    rb_define_const(rb_mGL, "GL_STATIC_DRAW", UINT2NUM(GL_STATIC_DRAW));
    rb_define_const(rb_mGL, "GL_STATIC_READ", UINT2NUM(GL_STATIC_READ));
    rb_define_const(rb_mGL, "GL_STATIC_COPY", UINT2NUM(GL_STATIC_COPY));
    rb_define_const(rb_mGL, "GL_DYNAMIC_DRAW", UINT2NUM(GL_DYNAMIC_DRAW));
    rb_define_const(rb_mGL, "GL_DYNAMIC_READ", UINT2NUM(GL_DYNAMIC_READ));
    rb_define_const(rb_mGL, "GL_DYNAMIC_COPY", UINT2NUM(GL_DYNAMIC_COPY));
    rb_define_const(rb_mGL, "GL_SAMPLES_PASSED", UINT2NUM(GL_SAMPLES_PASSED));
    rb_define_const(rb_mGL, "GL_SRC1_ALPHA", UINT2NUM(GL_SRC1_ALPHA));
    rb_define_const(rb_mGL, "GL_BLEND_EQUATION_RGB", UINT2NUM(GL_BLEND_EQUATION_RGB));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_ENABLED", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_ENABLED));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_SIZE", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_SIZE));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_STRIDE", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_STRIDE));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_TYPE", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_TYPE));
    rb_define_const(rb_mGL, "GL_CURRENT_VERTEX_ATTRIB", UINT2NUM(GL_CURRENT_VERTEX_ATTRIB));
    rb_define_const(rb_mGL, "GL_VERTEX_PROGRAM_POINT_SIZE", UINT2NUM(GL_VERTEX_PROGRAM_POINT_SIZE));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_POINTER", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_POINTER));
    rb_define_const(rb_mGL, "GL_STENCIL_BACK_FUNC", UINT2NUM(GL_STENCIL_BACK_FUNC));
    rb_define_const(rb_mGL, "GL_STENCIL_BACK_FAIL", UINT2NUM(GL_STENCIL_BACK_FAIL));
    rb_define_const(rb_mGL, "GL_STENCIL_BACK_PASS_DEPTH_FAIL", UINT2NUM(GL_STENCIL_BACK_PASS_DEPTH_FAIL));
    rb_define_const(rb_mGL, "GL_STENCIL_BACK_PASS_DEPTH_PASS", UINT2NUM(GL_STENCIL_BACK_PASS_DEPTH_PASS));
    rb_define_const(rb_mGL, "GL_MAX_DRAW_BUFFERS", UINT2NUM(GL_MAX_DRAW_BUFFERS));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER0", UINT2NUM(GL_DRAW_BUFFER0));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER1", UINT2NUM(GL_DRAW_BUFFER1));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER2", UINT2NUM(GL_DRAW_BUFFER2));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER3", UINT2NUM(GL_DRAW_BUFFER3));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER4", UINT2NUM(GL_DRAW_BUFFER4));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER5", UINT2NUM(GL_DRAW_BUFFER5));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER6", UINT2NUM(GL_DRAW_BUFFER6));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER7", UINT2NUM(GL_DRAW_BUFFER7));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER8", UINT2NUM(GL_DRAW_BUFFER8));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER9", UINT2NUM(GL_DRAW_BUFFER9));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER10", UINT2NUM(GL_DRAW_BUFFER10));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER11", UINT2NUM(GL_DRAW_BUFFER11));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER12", UINT2NUM(GL_DRAW_BUFFER12));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER13", UINT2NUM(GL_DRAW_BUFFER13));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER14", UINT2NUM(GL_DRAW_BUFFER14));
    rb_define_const(rb_mGL, "GL_DRAW_BUFFER15", UINT2NUM(GL_DRAW_BUFFER15));
    rb_define_const(rb_mGL, "GL_BLEND_EQUATION_ALPHA", UINT2NUM(GL_BLEND_EQUATION_ALPHA));
    rb_define_const(rb_mGL, "GL_MAX_VERTEX_ATTRIBS", UINT2NUM(GL_MAX_VERTEX_ATTRIBS));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_NORMALIZED", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED));
    rb_define_const(rb_mGL, "GL_MAX_TEXTURE_IMAGE_UNITS", UINT2NUM(GL_MAX_TEXTURE_IMAGE_UNITS));
    rb_define_const(rb_mGL, "GL_FRAGMENT_SHADER", UINT2NUM(GL_FRAGMENT_SHADER));
    rb_define_const(rb_mGL, "GL_VERTEX_SHADER", UINT2NUM(GL_VERTEX_SHADER));
    rb_define_const(rb_mGL, "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS", UINT2NUM(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_VERTEX_UNIFORM_COMPONENTS", UINT2NUM(GL_MAX_VERTEX_UNIFORM_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_VARYING_FLOATS", UINT2NUM(GL_MAX_VARYING_FLOATS));
    rb_define_const(rb_mGL, "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS", UINT2NUM(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS));
    rb_define_const(rb_mGL, "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", UINT2NUM(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS));
    rb_define_const(rb_mGL, "GL_SHADER_TYPE", UINT2NUM(GL_SHADER_TYPE));
    rb_define_const(rb_mGL, "GL_FLOAT_VEC2", UINT2NUM(GL_FLOAT_VEC2));
    rb_define_const(rb_mGL, "GL_FLOAT_VEC3", UINT2NUM(GL_FLOAT_VEC3));
    rb_define_const(rb_mGL, "GL_FLOAT_VEC4", UINT2NUM(GL_FLOAT_VEC4));
    rb_define_const(rb_mGL, "GL_INT_VEC2", UINT2NUM(GL_INT_VEC2));
    rb_define_const(rb_mGL, "GL_INT_VEC3", UINT2NUM(GL_INT_VEC3));
    rb_define_const(rb_mGL, "GL_INT_VEC4", UINT2NUM(GL_INT_VEC4));
    rb_define_const(rb_mGL, "GL_BOOL", UINT2NUM(GL_BOOL));
    rb_define_const(rb_mGL, "GL_BOOL_VEC2", UINT2NUM(GL_BOOL_VEC2));
    rb_define_const(rb_mGL, "GL_BOOL_VEC3", UINT2NUM(GL_BOOL_VEC3));
    rb_define_const(rb_mGL, "GL_BOOL_VEC4", UINT2NUM(GL_BOOL_VEC4));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT2", UINT2NUM(GL_FLOAT_MAT2));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT3", UINT2NUM(GL_FLOAT_MAT3));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT4", UINT2NUM(GL_FLOAT_MAT4));
    rb_define_const(rb_mGL, "GL_SAMPLER_1D", UINT2NUM(GL_SAMPLER_1D));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D", UINT2NUM(GL_SAMPLER_2D));
    rb_define_const(rb_mGL, "GL_SAMPLER_3D", UINT2NUM(GL_SAMPLER_3D));
    rb_define_const(rb_mGL, "GL_SAMPLER_CUBE", UINT2NUM(GL_SAMPLER_CUBE));
    rb_define_const(rb_mGL, "GL_SAMPLER_1D_SHADOW", UINT2NUM(GL_SAMPLER_1D_SHADOW));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D_SHADOW", UINT2NUM(GL_SAMPLER_2D_SHADOW));
    rb_define_const(rb_mGL, "GL_DELETE_STATUS", UINT2NUM(GL_DELETE_STATUS));
    rb_define_const(rb_mGL, "GL_COMPILE_STATUS", UINT2NUM(GL_COMPILE_STATUS));
    rb_define_const(rb_mGL, "GL_LINK_STATUS", UINT2NUM(GL_LINK_STATUS));
    rb_define_const(rb_mGL, "GL_VALIDATE_STATUS", UINT2NUM(GL_VALIDATE_STATUS));
    rb_define_const(rb_mGL, "GL_INFO_LOG_LENGTH", UINT2NUM(GL_INFO_LOG_LENGTH));
    rb_define_const(rb_mGL, "GL_ATTACHED_SHADERS", UINT2NUM(GL_ATTACHED_SHADERS));
    rb_define_const(rb_mGL, "GL_ACTIVE_UNIFORMS", UINT2NUM(GL_ACTIVE_UNIFORMS));
    rb_define_const(rb_mGL, "GL_ACTIVE_UNIFORM_MAX_LENGTH", UINT2NUM(GL_ACTIVE_UNIFORM_MAX_LENGTH));
    rb_define_const(rb_mGL, "GL_SHADER_SOURCE_LENGTH", UINT2NUM(GL_SHADER_SOURCE_LENGTH));
    rb_define_const(rb_mGL, "GL_ACTIVE_ATTRIBUTES", UINT2NUM(GL_ACTIVE_ATTRIBUTES));
    rb_define_const(rb_mGL, "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH", UINT2NUM(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH));
    rb_define_const(rb_mGL, "GL_FRAGMENT_SHADER_DERIVATIVE_HINT", UINT2NUM(GL_FRAGMENT_SHADER_DERIVATIVE_HINT));
    rb_define_const(rb_mGL, "GL_SHADING_LANGUAGE_VERSION", UINT2NUM(GL_SHADING_LANGUAGE_VERSION));
    rb_define_const(rb_mGL, "GL_CURRENT_PROGRAM", UINT2NUM(GL_CURRENT_PROGRAM));
    rb_define_const(rb_mGL, "GL_POINT_SPRITE_COORD_ORIGIN", UINT2NUM(GL_POINT_SPRITE_COORD_ORIGIN));
    rb_define_const(rb_mGL, "GL_LOWER_LEFT", UINT2NUM(GL_LOWER_LEFT));
    rb_define_const(rb_mGL, "GL_UPPER_LEFT", UINT2NUM(GL_UPPER_LEFT));
    rb_define_const(rb_mGL, "GL_STENCIL_BACK_REF", UINT2NUM(GL_STENCIL_BACK_REF));
    rb_define_const(rb_mGL, "GL_STENCIL_BACK_VALUE_MASK", UINT2NUM(GL_STENCIL_BACK_VALUE_MASK));
    rb_define_const(rb_mGL, "GL_STENCIL_BACK_WRITEMASK", UINT2NUM(GL_STENCIL_BACK_WRITEMASK));
    rb_define_const(rb_mGL, "GL_PIXEL_PACK_BUFFER", UINT2NUM(GL_PIXEL_PACK_BUFFER));
    rb_define_const(rb_mGL, "GL_PIXEL_UNPACK_BUFFER", UINT2NUM(GL_PIXEL_UNPACK_BUFFER));
    rb_define_const(rb_mGL, "GL_PIXEL_PACK_BUFFER_BINDING", UINT2NUM(GL_PIXEL_PACK_BUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_PIXEL_UNPACK_BUFFER_BINDING", UINT2NUM(GL_PIXEL_UNPACK_BUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT2x3", UINT2NUM(GL_FLOAT_MAT2x3));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT2x4", UINT2NUM(GL_FLOAT_MAT2x4));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT3x2", UINT2NUM(GL_FLOAT_MAT3x2));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT3x4", UINT2NUM(GL_FLOAT_MAT3x4));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT4x2", UINT2NUM(GL_FLOAT_MAT4x2));
    rb_define_const(rb_mGL, "GL_FLOAT_MAT4x3", UINT2NUM(GL_FLOAT_MAT4x3));
    rb_define_const(rb_mGL, "GL_SRGB", UINT2NUM(GL_SRGB));
    rb_define_const(rb_mGL, "GL_SRGB8", UINT2NUM(GL_SRGB8));
    rb_define_const(rb_mGL, "GL_SRGB_ALPHA", UINT2NUM(GL_SRGB_ALPHA));
    rb_define_const(rb_mGL, "GL_SRGB8_ALPHA8", UINT2NUM(GL_SRGB8_ALPHA8));
    rb_define_const(rb_mGL, "GL_COMPRESSED_SRGB", UINT2NUM(GL_COMPRESSED_SRGB));
    rb_define_const(rb_mGL, "GL_COMPRESSED_SRGB_ALPHA", UINT2NUM(GL_COMPRESSED_SRGB_ALPHA));
    rb_define_const(rb_mGL, "GL_COMPARE_REF_TO_TEXTURE", UINT2NUM(GL_COMPARE_REF_TO_TEXTURE));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE0", UINT2NUM(GL_CLIP_DISTANCE0));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE1", UINT2NUM(GL_CLIP_DISTANCE1));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE2", UINT2NUM(GL_CLIP_DISTANCE2));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE3", UINT2NUM(GL_CLIP_DISTANCE3));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE4", UINT2NUM(GL_CLIP_DISTANCE4));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE5", UINT2NUM(GL_CLIP_DISTANCE5));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE6", UINT2NUM(GL_CLIP_DISTANCE6));
    rb_define_const(rb_mGL, "GL_CLIP_DISTANCE7", UINT2NUM(GL_CLIP_DISTANCE7));
    rb_define_const(rb_mGL, "GL_MAX_CLIP_DISTANCES", UINT2NUM(GL_MAX_CLIP_DISTANCES));
    rb_define_const(rb_mGL, "GL_MAJOR_VERSION", UINT2NUM(GL_MAJOR_VERSION));
    rb_define_const(rb_mGL, "GL_MINOR_VERSION", UINT2NUM(GL_MINOR_VERSION));
    rb_define_const(rb_mGL, "GL_NUM_EXTENSIONS", UINT2NUM(GL_NUM_EXTENSIONS));
    rb_define_const(rb_mGL, "GL_CONTEXT_FLAGS", UINT2NUM(GL_CONTEXT_FLAGS));
    rb_define_const(rb_mGL, "GL_COMPRESSED_RED", UINT2NUM(GL_COMPRESSED_RED));
    rb_define_const(rb_mGL, "GL_COMPRESSED_RG", UINT2NUM(GL_COMPRESSED_RG));
    rb_define_const(rb_mGL, "GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT", UINT2NUM(GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT));
    rb_define_const(rb_mGL, "GL_RGBA32F", UINT2NUM(GL_RGBA32F));
    rb_define_const(rb_mGL, "GL_RGB32F", UINT2NUM(GL_RGB32F));
    rb_define_const(rb_mGL, "GL_RGBA16F", UINT2NUM(GL_RGBA16F));
    rb_define_const(rb_mGL, "GL_RGB16F", UINT2NUM(GL_RGB16F));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_INTEGER", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_INTEGER));
    rb_define_const(rb_mGL, "GL_MAX_ARRAY_TEXTURE_LAYERS", UINT2NUM(GL_MAX_ARRAY_TEXTURE_LAYERS));
    rb_define_const(rb_mGL, "GL_MIN_PROGRAM_TEXEL_OFFSET", UINT2NUM(GL_MIN_PROGRAM_TEXEL_OFFSET));
    rb_define_const(rb_mGL, "GL_MAX_PROGRAM_TEXEL_OFFSET", UINT2NUM(GL_MAX_PROGRAM_TEXEL_OFFSET));
    rb_define_const(rb_mGL, "GL_CLAMP_READ_COLOR", UINT2NUM(GL_CLAMP_READ_COLOR));
    rb_define_const(rb_mGL, "GL_FIXED_ONLY", UINT2NUM(GL_FIXED_ONLY));
    rb_define_const(rb_mGL, "GL_MAX_VARYING_COMPONENTS", UINT2NUM(GL_MAX_VARYING_COMPONENTS));
    rb_define_const(rb_mGL, "GL_TEXTURE_1D_ARRAY", UINT2NUM(GL_TEXTURE_1D_ARRAY));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_1D_ARRAY", UINT2NUM(GL_PROXY_TEXTURE_1D_ARRAY));
    rb_define_const(rb_mGL, "GL_TEXTURE_2D_ARRAY", UINT2NUM(GL_TEXTURE_2D_ARRAY));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_2D_ARRAY", UINT2NUM(GL_PROXY_TEXTURE_2D_ARRAY));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_1D_ARRAY", UINT2NUM(GL_TEXTURE_BINDING_1D_ARRAY));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_2D_ARRAY", UINT2NUM(GL_TEXTURE_BINDING_2D_ARRAY));
    rb_define_const(rb_mGL, "GL_R11F_G11F_B10F", UINT2NUM(GL_R11F_G11F_B10F));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_10F_11F_11F_REV", UINT2NUM(GL_UNSIGNED_INT_10F_11F_11F_REV));
    rb_define_const(rb_mGL, "GL_RGB9_E5", UINT2NUM(GL_RGB9_E5));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_5_9_9_9_REV", UINT2NUM(GL_UNSIGNED_INT_5_9_9_9_REV));
    rb_define_const(rb_mGL, "GL_TEXTURE_SHARED_SIZE", UINT2NUM(GL_TEXTURE_SHARED_SIZE));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH", UINT2NUM(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_BUFFER_MODE", UINT2NUM(GL_TRANSFORM_FEEDBACK_BUFFER_MODE));
    rb_define_const(rb_mGL, "GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS", UINT2NUM(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_VARYINGS", UINT2NUM(GL_TRANSFORM_FEEDBACK_VARYINGS));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_BUFFER_START", UINT2NUM(GL_TRANSFORM_FEEDBACK_BUFFER_START));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_BUFFER_SIZE", UINT2NUM(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE));
    rb_define_const(rb_mGL, "GL_PRIMITIVES_GENERATED", UINT2NUM(GL_PRIMITIVES_GENERATED));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN", UINT2NUM(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN));
    rb_define_const(rb_mGL, "GL_RASTERIZER_DISCARD", UINT2NUM(GL_RASTERIZER_DISCARD));
    rb_define_const(rb_mGL, "GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS", UINT2NUM(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS", UINT2NUM(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS));
    rb_define_const(rb_mGL, "GL_INTERLEAVED_ATTRIBS", UINT2NUM(GL_INTERLEAVED_ATTRIBS));
    rb_define_const(rb_mGL, "GL_SEPARATE_ATTRIBS", UINT2NUM(GL_SEPARATE_ATTRIBS));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_BUFFER", UINT2NUM(GL_TRANSFORM_FEEDBACK_BUFFER));
    rb_define_const(rb_mGL, "GL_TRANSFORM_FEEDBACK_BUFFER_BINDING", UINT2NUM(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_RGBA32UI", UINT2NUM(GL_RGBA32UI));
    rb_define_const(rb_mGL, "GL_RGB32UI", UINT2NUM(GL_RGB32UI));
    rb_define_const(rb_mGL, "GL_RGBA16UI", UINT2NUM(GL_RGBA16UI));
    rb_define_const(rb_mGL, "GL_RGB16UI", UINT2NUM(GL_RGB16UI));
    rb_define_const(rb_mGL, "GL_RGBA8UI", UINT2NUM(GL_RGBA8UI));
    rb_define_const(rb_mGL, "GL_RGB8UI", UINT2NUM(GL_RGB8UI));
    rb_define_const(rb_mGL, "GL_RGBA32I", UINT2NUM(GL_RGBA32I));
    rb_define_const(rb_mGL, "GL_RGB32I", UINT2NUM(GL_RGB32I));
    rb_define_const(rb_mGL, "GL_RGBA16I", UINT2NUM(GL_RGBA16I));
    rb_define_const(rb_mGL, "GL_RGB16I", UINT2NUM(GL_RGB16I));
    rb_define_const(rb_mGL, "GL_RGBA8I", UINT2NUM(GL_RGBA8I));
    rb_define_const(rb_mGL, "GL_RGB8I", UINT2NUM(GL_RGB8I));
    rb_define_const(rb_mGL, "GL_RED_INTEGER", UINT2NUM(GL_RED_INTEGER));
    rb_define_const(rb_mGL, "GL_GREEN_INTEGER", UINT2NUM(GL_GREEN_INTEGER));
    rb_define_const(rb_mGL, "GL_BLUE_INTEGER", UINT2NUM(GL_BLUE_INTEGER));
    rb_define_const(rb_mGL, "GL_RGB_INTEGER", UINT2NUM(GL_RGB_INTEGER));
    rb_define_const(rb_mGL, "GL_RGBA_INTEGER", UINT2NUM(GL_RGBA_INTEGER));
    rb_define_const(rb_mGL, "GL_BGR_INTEGER", UINT2NUM(GL_BGR_INTEGER));
    rb_define_const(rb_mGL, "GL_BGRA_INTEGER", UINT2NUM(GL_BGRA_INTEGER));
    rb_define_const(rb_mGL, "GL_SAMPLER_1D_ARRAY", UINT2NUM(GL_SAMPLER_1D_ARRAY));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D_ARRAY", UINT2NUM(GL_SAMPLER_2D_ARRAY));
    rb_define_const(rb_mGL, "GL_SAMPLER_1D_ARRAY_SHADOW", UINT2NUM(GL_SAMPLER_1D_ARRAY_SHADOW));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D_ARRAY_SHADOW", UINT2NUM(GL_SAMPLER_2D_ARRAY_SHADOW));
    rb_define_const(rb_mGL, "GL_SAMPLER_CUBE_SHADOW", UINT2NUM(GL_SAMPLER_CUBE_SHADOW));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_VEC2", UINT2NUM(GL_UNSIGNED_INT_VEC2));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_VEC3", UINT2NUM(GL_UNSIGNED_INT_VEC3));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_VEC4", UINT2NUM(GL_UNSIGNED_INT_VEC4));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_1D", UINT2NUM(GL_INT_SAMPLER_1D));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_2D", UINT2NUM(GL_INT_SAMPLER_2D));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_3D", UINT2NUM(GL_INT_SAMPLER_3D));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_CUBE", UINT2NUM(GL_INT_SAMPLER_CUBE));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_1D_ARRAY", UINT2NUM(GL_INT_SAMPLER_1D_ARRAY));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_2D_ARRAY", UINT2NUM(GL_INT_SAMPLER_2D_ARRAY));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_1D", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_1D));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_2D", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_2D));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_3D", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_3D));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_CUBE", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_CUBE));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY));
    rb_define_const(rb_mGL, "GL_QUERY_WAIT", UINT2NUM(GL_QUERY_WAIT));
    rb_define_const(rb_mGL, "GL_QUERY_NO_WAIT", UINT2NUM(GL_QUERY_NO_WAIT));
    rb_define_const(rb_mGL, "GL_QUERY_BY_REGION_WAIT", UINT2NUM(GL_QUERY_BY_REGION_WAIT));
    rb_define_const(rb_mGL, "GL_QUERY_BY_REGION_NO_WAIT", UINT2NUM(GL_QUERY_BY_REGION_NO_WAIT));
    rb_define_const(rb_mGL, "GL_BUFFER_ACCESS_FLAGS", UINT2NUM(GL_BUFFER_ACCESS_FLAGS));
    rb_define_const(rb_mGL, "GL_BUFFER_MAP_LENGTH", UINT2NUM(GL_BUFFER_MAP_LENGTH));
    rb_define_const(rb_mGL, "GL_BUFFER_MAP_OFFSET", UINT2NUM(GL_BUFFER_MAP_OFFSET));
    rb_define_const(rb_mGL, "GL_DEPTH_COMPONENT32F", UINT2NUM(GL_DEPTH_COMPONENT32F));
    rb_define_const(rb_mGL, "GL_DEPTH32F_STENCIL8", UINT2NUM(GL_DEPTH32F_STENCIL8));
    rb_define_const(rb_mGL, "GL_FLOAT_32_UNSIGNED_INT_24_8_REV", UINT2NUM(GL_FLOAT_32_UNSIGNED_INT_24_8_REV));
    rb_define_const(rb_mGL, "GL_INVALID_FRAMEBUFFER_OPERATION", UINT2NUM(GL_INVALID_FRAMEBUFFER_OPERATION));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_DEFAULT", UINT2NUM(GL_FRAMEBUFFER_DEFAULT));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_UNDEFINED", UINT2NUM(GL_FRAMEBUFFER_UNDEFINED));
    rb_define_const(rb_mGL, "GL_DEPTH_STENCIL_ATTACHMENT", UINT2NUM(GL_DEPTH_STENCIL_ATTACHMENT));
    rb_define_const(rb_mGL, "GL_MAX_RENDERBUFFER_SIZE", UINT2NUM(GL_MAX_RENDERBUFFER_SIZE));
    rb_define_const(rb_mGL, "GL_DEPTH_STENCIL", UINT2NUM(GL_DEPTH_STENCIL));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_24_8", UINT2NUM(GL_UNSIGNED_INT_24_8));
    rb_define_const(rb_mGL, "GL_DEPTH24_STENCIL8", UINT2NUM(GL_DEPTH24_STENCIL8));
    rb_define_const(rb_mGL, "GL_TEXTURE_STENCIL_SIZE", UINT2NUM(GL_TEXTURE_STENCIL_SIZE));
    rb_define_const(rb_mGL, "GL_TEXTURE_RED_TYPE", UINT2NUM(GL_TEXTURE_RED_TYPE));
    rb_define_const(rb_mGL, "GL_TEXTURE_GREEN_TYPE", UINT2NUM(GL_TEXTURE_GREEN_TYPE));
    rb_define_const(rb_mGL, "GL_TEXTURE_BLUE_TYPE", UINT2NUM(GL_TEXTURE_BLUE_TYPE));
    rb_define_const(rb_mGL, "GL_TEXTURE_ALPHA_TYPE", UINT2NUM(GL_TEXTURE_ALPHA_TYPE));
    rb_define_const(rb_mGL, "GL_TEXTURE_DEPTH_TYPE", UINT2NUM(GL_TEXTURE_DEPTH_TYPE));
    rb_define_const(rb_mGL, "GL_UNSIGNED_NORMALIZED", UINT2NUM(GL_UNSIGNED_NORMALIZED));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_BINDING", UINT2NUM(GL_FRAMEBUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_DRAW_FRAMEBUFFER_BINDING", UINT2NUM(GL_DRAW_FRAMEBUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_BINDING", UINT2NUM(GL_RENDERBUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_READ_FRAMEBUFFER", UINT2NUM(GL_READ_FRAMEBUFFER));
    rb_define_const(rb_mGL, "GL_DRAW_FRAMEBUFFER", UINT2NUM(GL_DRAW_FRAMEBUFFER));
    rb_define_const(rb_mGL, "GL_READ_FRAMEBUFFER_BINDING", UINT2NUM(GL_READ_FRAMEBUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_SAMPLES", UINT2NUM(GL_RENDERBUFFER_SAMPLES));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_COMPLETE", UINT2NUM(GL_FRAMEBUFFER_COMPLETE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT", UINT2NUM(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", UINT2NUM(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER", UINT2NUM(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER", UINT2NUM(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_UNSUPPORTED", UINT2NUM(GL_FRAMEBUFFER_UNSUPPORTED));
    rb_define_const(rb_mGL, "GL_MAX_COLOR_ATTACHMENTS", UINT2NUM(GL_MAX_COLOR_ATTACHMENTS));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT0", UINT2NUM(GL_COLOR_ATTACHMENT0));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT1", UINT2NUM(GL_COLOR_ATTACHMENT1));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT2", UINT2NUM(GL_COLOR_ATTACHMENT2));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT3", UINT2NUM(GL_COLOR_ATTACHMENT3));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT4", UINT2NUM(GL_COLOR_ATTACHMENT4));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT5", UINT2NUM(GL_COLOR_ATTACHMENT5));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT6", UINT2NUM(GL_COLOR_ATTACHMENT6));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT7", UINT2NUM(GL_COLOR_ATTACHMENT7));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT8", UINT2NUM(GL_COLOR_ATTACHMENT8));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT9", UINT2NUM(GL_COLOR_ATTACHMENT9));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT10", UINT2NUM(GL_COLOR_ATTACHMENT10));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT11", UINT2NUM(GL_COLOR_ATTACHMENT11));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT12", UINT2NUM(GL_COLOR_ATTACHMENT12));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT13", UINT2NUM(GL_COLOR_ATTACHMENT13));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT14", UINT2NUM(GL_COLOR_ATTACHMENT14));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT15", UINT2NUM(GL_COLOR_ATTACHMENT15));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT16", UINT2NUM(GL_COLOR_ATTACHMENT16));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT17", UINT2NUM(GL_COLOR_ATTACHMENT17));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT18", UINT2NUM(GL_COLOR_ATTACHMENT18));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT19", UINT2NUM(GL_COLOR_ATTACHMENT19));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT20", UINT2NUM(GL_COLOR_ATTACHMENT20));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT21", UINT2NUM(GL_COLOR_ATTACHMENT21));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT22", UINT2NUM(GL_COLOR_ATTACHMENT22));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT23", UINT2NUM(GL_COLOR_ATTACHMENT23));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT24", UINT2NUM(GL_COLOR_ATTACHMENT24));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT25", UINT2NUM(GL_COLOR_ATTACHMENT25));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT26", UINT2NUM(GL_COLOR_ATTACHMENT26));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT27", UINT2NUM(GL_COLOR_ATTACHMENT27));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT28", UINT2NUM(GL_COLOR_ATTACHMENT28));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT29", UINT2NUM(GL_COLOR_ATTACHMENT29));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT30", UINT2NUM(GL_COLOR_ATTACHMENT30));
    rb_define_const(rb_mGL, "GL_COLOR_ATTACHMENT31", UINT2NUM(GL_COLOR_ATTACHMENT31));
    rb_define_const(rb_mGL, "GL_DEPTH_ATTACHMENT", UINT2NUM(GL_DEPTH_ATTACHMENT));
    rb_define_const(rb_mGL, "GL_STENCIL_ATTACHMENT", UINT2NUM(GL_STENCIL_ATTACHMENT));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER", UINT2NUM(GL_FRAMEBUFFER));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER", UINT2NUM(GL_RENDERBUFFER));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_WIDTH", UINT2NUM(GL_RENDERBUFFER_WIDTH));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_HEIGHT", UINT2NUM(GL_RENDERBUFFER_HEIGHT));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_INTERNAL_FORMAT", UINT2NUM(GL_RENDERBUFFER_INTERNAL_FORMAT));
    rb_define_const(rb_mGL, "GL_STENCIL_INDEX1", UINT2NUM(GL_STENCIL_INDEX1));
    rb_define_const(rb_mGL, "GL_STENCIL_INDEX4", UINT2NUM(GL_STENCIL_INDEX4));
    rb_define_const(rb_mGL, "GL_STENCIL_INDEX8", UINT2NUM(GL_STENCIL_INDEX8));
    rb_define_const(rb_mGL, "GL_STENCIL_INDEX16", UINT2NUM(GL_STENCIL_INDEX16));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_RED_SIZE", UINT2NUM(GL_RENDERBUFFER_RED_SIZE));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_GREEN_SIZE", UINT2NUM(GL_RENDERBUFFER_GREEN_SIZE));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_BLUE_SIZE", UINT2NUM(GL_RENDERBUFFER_BLUE_SIZE));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_ALPHA_SIZE", UINT2NUM(GL_RENDERBUFFER_ALPHA_SIZE));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_DEPTH_SIZE", UINT2NUM(GL_RENDERBUFFER_DEPTH_SIZE));
    rb_define_const(rb_mGL, "GL_RENDERBUFFER_STENCIL_SIZE", UINT2NUM(GL_RENDERBUFFER_STENCIL_SIZE));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE", UINT2NUM(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_MAX_SAMPLES", UINT2NUM(GL_MAX_SAMPLES));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_SRGB", UINT2NUM(GL_FRAMEBUFFER_SRGB));
    rb_define_const(rb_mGL, "GL_HALF_FLOAT", UINT2NUM(GL_HALF_FLOAT));
    rb_define_const(rb_mGL, "GL_MAP_READ_BIT", UINT2NUM(GL_MAP_READ_BIT));
    rb_define_const(rb_mGL, "GL_MAP_WRITE_BIT", UINT2NUM(GL_MAP_WRITE_BIT));
    rb_define_const(rb_mGL, "GL_MAP_INVALIDATE_RANGE_BIT", UINT2NUM(GL_MAP_INVALIDATE_RANGE_BIT));
    rb_define_const(rb_mGL, "GL_MAP_INVALIDATE_BUFFER_BIT", UINT2NUM(GL_MAP_INVALIDATE_BUFFER_BIT));
    rb_define_const(rb_mGL, "GL_MAP_FLUSH_EXPLICIT_BIT", UINT2NUM(GL_MAP_FLUSH_EXPLICIT_BIT));
    rb_define_const(rb_mGL, "GL_MAP_UNSYNCHRONIZED_BIT", UINT2NUM(GL_MAP_UNSYNCHRONIZED_BIT));
    rb_define_const(rb_mGL, "GL_COMPRESSED_RED_RGTC1", UINT2NUM(GL_COMPRESSED_RED_RGTC1));
    rb_define_const(rb_mGL, "GL_COMPRESSED_SIGNED_RED_RGTC1", UINT2NUM(GL_COMPRESSED_SIGNED_RED_RGTC1));
    rb_define_const(rb_mGL, "GL_COMPRESSED_RG_RGTC2", UINT2NUM(GL_COMPRESSED_RG_RGTC2));
    rb_define_const(rb_mGL, "GL_COMPRESSED_SIGNED_RG_RGTC2", UINT2NUM(GL_COMPRESSED_SIGNED_RG_RGTC2));
    rb_define_const(rb_mGL, "GL_RG", UINT2NUM(GL_RG));
    rb_define_const(rb_mGL, "GL_RG_INTEGER", UINT2NUM(GL_RG_INTEGER));
    rb_define_const(rb_mGL, "GL_R8", UINT2NUM(GL_R8));
    rb_define_const(rb_mGL, "GL_R16", UINT2NUM(GL_R16));
    rb_define_const(rb_mGL, "GL_RG8", UINT2NUM(GL_RG8));
    rb_define_const(rb_mGL, "GL_RG16", UINT2NUM(GL_RG16));
    rb_define_const(rb_mGL, "GL_R16F", UINT2NUM(GL_R16F));
    rb_define_const(rb_mGL, "GL_R32F", UINT2NUM(GL_R32F));
    rb_define_const(rb_mGL, "GL_RG16F", UINT2NUM(GL_RG16F));
    rb_define_const(rb_mGL, "GL_RG32F", UINT2NUM(GL_RG32F));
    rb_define_const(rb_mGL, "GL_R8I", UINT2NUM(GL_R8I));
    rb_define_const(rb_mGL, "GL_R8UI", UINT2NUM(GL_R8UI));
    rb_define_const(rb_mGL, "GL_R16I", UINT2NUM(GL_R16I));
    rb_define_const(rb_mGL, "GL_R16UI", UINT2NUM(GL_R16UI));
    rb_define_const(rb_mGL, "GL_R32I", UINT2NUM(GL_R32I));
    rb_define_const(rb_mGL, "GL_R32UI", UINT2NUM(GL_R32UI));
    rb_define_const(rb_mGL, "GL_RG8I", UINT2NUM(GL_RG8I));
    rb_define_const(rb_mGL, "GL_RG8UI", UINT2NUM(GL_RG8UI));
    rb_define_const(rb_mGL, "GL_RG16I", UINT2NUM(GL_RG16I));
    rb_define_const(rb_mGL, "GL_RG16UI", UINT2NUM(GL_RG16UI));
    rb_define_const(rb_mGL, "GL_RG32I", UINT2NUM(GL_RG32I));
    rb_define_const(rb_mGL, "GL_RG32UI", UINT2NUM(GL_RG32UI));
    rb_define_const(rb_mGL, "GL_VERTEX_ARRAY_BINDING", UINT2NUM(GL_VERTEX_ARRAY_BINDING));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D_RECT", UINT2NUM(GL_SAMPLER_2D_RECT));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D_RECT_SHADOW", UINT2NUM(GL_SAMPLER_2D_RECT_SHADOW));
    rb_define_const(rb_mGL, "GL_SAMPLER_BUFFER", UINT2NUM(GL_SAMPLER_BUFFER));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_2D_RECT", UINT2NUM(GL_INT_SAMPLER_2D_RECT));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_BUFFER", UINT2NUM(GL_INT_SAMPLER_BUFFER));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_2D_RECT", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_2D_RECT));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_BUFFER", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_BUFFER));
    rb_define_const(rb_mGL, "GL_TEXTURE_BUFFER", UINT2NUM(GL_TEXTURE_BUFFER));
    rb_define_const(rb_mGL, "GL_MAX_TEXTURE_BUFFER_SIZE", UINT2NUM(GL_MAX_TEXTURE_BUFFER_SIZE));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_BUFFER", UINT2NUM(GL_TEXTURE_BINDING_BUFFER));
    rb_define_const(rb_mGL, "GL_TEXTURE_BUFFER_DATA_STORE_BINDING", UINT2NUM(GL_TEXTURE_BUFFER_DATA_STORE_BINDING));
    rb_define_const(rb_mGL, "GL_TEXTURE_RECTANGLE", UINT2NUM(GL_TEXTURE_RECTANGLE));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_RECTANGLE", UINT2NUM(GL_TEXTURE_BINDING_RECTANGLE));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_RECTANGLE", UINT2NUM(GL_PROXY_TEXTURE_RECTANGLE));
    rb_define_const(rb_mGL, "GL_MAX_RECTANGLE_TEXTURE_SIZE", UINT2NUM(GL_MAX_RECTANGLE_TEXTURE_SIZE));
    rb_define_const(rb_mGL, "GL_R8_SNORM", UINT2NUM(GL_R8_SNORM));
    rb_define_const(rb_mGL, "GL_RG8_SNORM", UINT2NUM(GL_RG8_SNORM));
    rb_define_const(rb_mGL, "GL_RGB8_SNORM", UINT2NUM(GL_RGB8_SNORM));
    rb_define_const(rb_mGL, "GL_RGBA8_SNORM", UINT2NUM(GL_RGBA8_SNORM));
    rb_define_const(rb_mGL, "GL_R16_SNORM", UINT2NUM(GL_R16_SNORM));
    rb_define_const(rb_mGL, "GL_RG16_SNORM", UINT2NUM(GL_RG16_SNORM));
    rb_define_const(rb_mGL, "GL_RGB16_SNORM", UINT2NUM(GL_RGB16_SNORM));
    rb_define_const(rb_mGL, "GL_RGBA16_SNORM", UINT2NUM(GL_RGBA16_SNORM));
    rb_define_const(rb_mGL, "GL_SIGNED_NORMALIZED", UINT2NUM(GL_SIGNED_NORMALIZED));
    rb_define_const(rb_mGL, "GL_PRIMITIVE_RESTART", UINT2NUM(GL_PRIMITIVE_RESTART));
    rb_define_const(rb_mGL, "GL_PRIMITIVE_RESTART_INDEX", UINT2NUM(GL_PRIMITIVE_RESTART_INDEX));
    rb_define_const(rb_mGL, "GL_COPY_READ_BUFFER", UINT2NUM(GL_COPY_READ_BUFFER));
    rb_define_const(rb_mGL, "GL_COPY_WRITE_BUFFER", UINT2NUM(GL_COPY_WRITE_BUFFER));
    rb_define_const(rb_mGL, "GL_UNIFORM_BUFFER", UINT2NUM(GL_UNIFORM_BUFFER));
    rb_define_const(rb_mGL, "GL_UNIFORM_BUFFER_BINDING", UINT2NUM(GL_UNIFORM_BUFFER_BINDING));
    rb_define_const(rb_mGL, "GL_UNIFORM_BUFFER_START", UINT2NUM(GL_UNIFORM_BUFFER_START));
    rb_define_const(rb_mGL, "GL_UNIFORM_BUFFER_SIZE", UINT2NUM(GL_UNIFORM_BUFFER_SIZE));
    rb_define_const(rb_mGL, "GL_MAX_VERTEX_UNIFORM_BLOCKS", UINT2NUM(GL_MAX_VERTEX_UNIFORM_BLOCKS));
    rb_define_const(rb_mGL, "GL_MAX_GEOMETRY_UNIFORM_BLOCKS", UINT2NUM(GL_MAX_GEOMETRY_UNIFORM_BLOCKS));
    rb_define_const(rb_mGL, "GL_MAX_FRAGMENT_UNIFORM_BLOCKS", UINT2NUM(GL_MAX_FRAGMENT_UNIFORM_BLOCKS));
    rb_define_const(rb_mGL, "GL_MAX_COMBINED_UNIFORM_BLOCKS", UINT2NUM(GL_MAX_COMBINED_UNIFORM_BLOCKS));
    rb_define_const(rb_mGL, "GL_MAX_UNIFORM_BUFFER_BINDINGS", UINT2NUM(GL_MAX_UNIFORM_BUFFER_BINDINGS));
    rb_define_const(rb_mGL, "GL_MAX_UNIFORM_BLOCK_SIZE", UINT2NUM(GL_MAX_UNIFORM_BLOCK_SIZE));
    rb_define_const(rb_mGL, "GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS", UINT2NUM(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS", UINT2NUM(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS", UINT2NUM(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS));
    rb_define_const(rb_mGL, "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT", UINT2NUM(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT));
    rb_define_const(rb_mGL, "GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH", UINT2NUM(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH));
    rb_define_const(rb_mGL, "GL_ACTIVE_UNIFORM_BLOCKS", UINT2NUM(GL_ACTIVE_UNIFORM_BLOCKS));
    rb_define_const(rb_mGL, "GL_UNIFORM_TYPE", UINT2NUM(GL_UNIFORM_TYPE));
    rb_define_const(rb_mGL, "GL_UNIFORM_SIZE", UINT2NUM(GL_UNIFORM_SIZE));
    rb_define_const(rb_mGL, "GL_UNIFORM_NAME_LENGTH", UINT2NUM(GL_UNIFORM_NAME_LENGTH));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_INDEX", UINT2NUM(GL_UNIFORM_BLOCK_INDEX));
    rb_define_const(rb_mGL, "GL_UNIFORM_OFFSET", UINT2NUM(GL_UNIFORM_OFFSET));
    rb_define_const(rb_mGL, "GL_UNIFORM_ARRAY_STRIDE", UINT2NUM(GL_UNIFORM_ARRAY_STRIDE));
    rb_define_const(rb_mGL, "GL_UNIFORM_MATRIX_STRIDE", UINT2NUM(GL_UNIFORM_MATRIX_STRIDE));
    rb_define_const(rb_mGL, "GL_UNIFORM_IS_ROW_MAJOR", UINT2NUM(GL_UNIFORM_IS_ROW_MAJOR));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_BINDING", UINT2NUM(GL_UNIFORM_BLOCK_BINDING));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_DATA_SIZE", UINT2NUM(GL_UNIFORM_BLOCK_DATA_SIZE));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_NAME_LENGTH", UINT2NUM(GL_UNIFORM_BLOCK_NAME_LENGTH));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS", UINT2NUM(GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES", UINT2NUM(GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER", UINT2NUM(GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER", UINT2NUM(GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER));
    rb_define_const(rb_mGL, "GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER", UINT2NUM(GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER));
    rb_define_const(rb_mGL, "GL_INVALID_INDEX", UINT2NUM(GL_INVALID_INDEX));
    rb_define_const(rb_mGL, "GL_CONTEXT_CORE_PROFILE_BIT", UINT2NUM(GL_CONTEXT_CORE_PROFILE_BIT));
    rb_define_const(rb_mGL, "GL_CONTEXT_COMPATIBILITY_PROFILE_BIT", UINT2NUM(GL_CONTEXT_COMPATIBILITY_PROFILE_BIT));
    rb_define_const(rb_mGL, "GL_LINES_ADJACENCY", UINT2NUM(GL_LINES_ADJACENCY));
    rb_define_const(rb_mGL, "GL_LINE_STRIP_ADJACENCY", UINT2NUM(GL_LINE_STRIP_ADJACENCY));
    rb_define_const(rb_mGL, "GL_TRIANGLES_ADJACENCY", UINT2NUM(GL_TRIANGLES_ADJACENCY));
    rb_define_const(rb_mGL, "GL_TRIANGLE_STRIP_ADJACENCY", UINT2NUM(GL_TRIANGLE_STRIP_ADJACENCY));
    rb_define_const(rb_mGL, "GL_PROGRAM_POINT_SIZE", UINT2NUM(GL_PROGRAM_POINT_SIZE));
    rb_define_const(rb_mGL, "GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS", UINT2NUM(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_ATTACHMENT_LAYERED", UINT2NUM(GL_FRAMEBUFFER_ATTACHMENT_LAYERED));
    rb_define_const(rb_mGL, "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS", UINT2NUM(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS));
    rb_define_const(rb_mGL, "GL_GEOMETRY_SHADER", UINT2NUM(GL_GEOMETRY_SHADER));
    rb_define_const(rb_mGL, "GL_GEOMETRY_VERTICES_OUT", UINT2NUM(GL_GEOMETRY_VERTICES_OUT));
    rb_define_const(rb_mGL, "GL_GEOMETRY_INPUT_TYPE", UINT2NUM(GL_GEOMETRY_INPUT_TYPE));
    rb_define_const(rb_mGL, "GL_GEOMETRY_OUTPUT_TYPE", UINT2NUM(GL_GEOMETRY_OUTPUT_TYPE));
    rb_define_const(rb_mGL, "GL_MAX_GEOMETRY_UNIFORM_COMPONENTS", UINT2NUM(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_GEOMETRY_OUTPUT_VERTICES", UINT2NUM(GL_MAX_GEOMETRY_OUTPUT_VERTICES));
    rb_define_const(rb_mGL, "GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS", UINT2NUM(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_VERTEX_OUTPUT_COMPONENTS", UINT2NUM(GL_MAX_VERTEX_OUTPUT_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_GEOMETRY_INPUT_COMPONENTS", UINT2NUM(GL_MAX_GEOMETRY_INPUT_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_GEOMETRY_OUTPUT_COMPONENTS", UINT2NUM(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS));
    rb_define_const(rb_mGL, "GL_MAX_FRAGMENT_INPUT_COMPONENTS", UINT2NUM(GL_MAX_FRAGMENT_INPUT_COMPONENTS));
    rb_define_const(rb_mGL, "GL_CONTEXT_PROFILE_MASK", UINT2NUM(GL_CONTEXT_PROFILE_MASK));
    rb_define_const(rb_mGL, "GL_DEPTH_CLAMP", UINT2NUM(GL_DEPTH_CLAMP));
    rb_define_const(rb_mGL, "GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION", UINT2NUM(GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION));
    rb_define_const(rb_mGL, "GL_FIRST_VERTEX_CONVENTION", UINT2NUM(GL_FIRST_VERTEX_CONVENTION));
    rb_define_const(rb_mGL, "GL_LAST_VERTEX_CONVENTION", UINT2NUM(GL_LAST_VERTEX_CONVENTION));
    rb_define_const(rb_mGL, "GL_PROVOKING_VERTEX", UINT2NUM(GL_PROVOKING_VERTEX));
    rb_define_const(rb_mGL, "GL_TEXTURE_CUBE_MAP_SEAMLESS", UINT2NUM(GL_TEXTURE_CUBE_MAP_SEAMLESS));
    rb_define_const(rb_mGL, "GL_MAX_SERVER_WAIT_TIMEOUT", UINT2NUM(GL_MAX_SERVER_WAIT_TIMEOUT));
    rb_define_const(rb_mGL, "GL_OBJECT_TYPE", UINT2NUM(GL_OBJECT_TYPE));
    rb_define_const(rb_mGL, "GL_SYNC_CONDITION", UINT2NUM(GL_SYNC_CONDITION));
    rb_define_const(rb_mGL, "GL_SYNC_STATUS", UINT2NUM(GL_SYNC_STATUS));
    rb_define_const(rb_mGL, "GL_SYNC_FLAGS", UINT2NUM(GL_SYNC_FLAGS));
    rb_define_const(rb_mGL, "GL_SYNC_FENCE", UINT2NUM(GL_SYNC_FENCE));
    rb_define_const(rb_mGL, "GL_SYNC_GPU_COMMANDS_COMPLETE", UINT2NUM(GL_SYNC_GPU_COMMANDS_COMPLETE));
    rb_define_const(rb_mGL, "GL_UNSIGNALED", UINT2NUM(GL_UNSIGNALED));
    rb_define_const(rb_mGL, "GL_SIGNALED", UINT2NUM(GL_SIGNALED));
    rb_define_const(rb_mGL, "GL_ALREADY_SIGNALED", UINT2NUM(GL_ALREADY_SIGNALED));
    rb_define_const(rb_mGL, "GL_TIMEOUT_EXPIRED", UINT2NUM(GL_TIMEOUT_EXPIRED));
    rb_define_const(rb_mGL, "GL_CONDITION_SATISFIED", UINT2NUM(GL_CONDITION_SATISFIED));
    rb_define_const(rb_mGL, "GL_WAIT_FAILED", UINT2NUM(GL_WAIT_FAILED));
    rb_define_const(rb_mGL, "GL_TIMEOUT_IGNORED", ULL2NUM(GL_TIMEOUT_IGNORED));
    rb_define_const(rb_mGL, "GL_SYNC_FLUSH_COMMANDS_BIT", UINT2NUM(GL_SYNC_FLUSH_COMMANDS_BIT));
    rb_define_const(rb_mGL, "GL_SAMPLE_POSITION", UINT2NUM(GL_SAMPLE_POSITION));
    rb_define_const(rb_mGL, "GL_SAMPLE_MASK", UINT2NUM(GL_SAMPLE_MASK));
    rb_define_const(rb_mGL, "GL_SAMPLE_MASK_VALUE", UINT2NUM(GL_SAMPLE_MASK_VALUE));
    rb_define_const(rb_mGL, "GL_MAX_SAMPLE_MASK_WORDS", UINT2NUM(GL_MAX_SAMPLE_MASK_WORDS));
    rb_define_const(rb_mGL, "GL_TEXTURE_2D_MULTISAMPLE", UINT2NUM(GL_TEXTURE_2D_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_2D_MULTISAMPLE", UINT2NUM(GL_PROXY_TEXTURE_2D_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_TEXTURE_2D_MULTISAMPLE_ARRAY", UINT2NUM(GL_TEXTURE_2D_MULTISAMPLE_ARRAY));
    rb_define_const(rb_mGL, "GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY", UINT2NUM(GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_2D_MULTISAMPLE", UINT2NUM(GL_TEXTURE_BINDING_2D_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY", UINT2NUM(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY));
    rb_define_const(rb_mGL, "GL_TEXTURE_SAMPLES", UINT2NUM(GL_TEXTURE_SAMPLES));
    rb_define_const(rb_mGL, "GL_TEXTURE_FIXED_SAMPLE_LOCATIONS", UINT2NUM(GL_TEXTURE_FIXED_SAMPLE_LOCATIONS));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D_MULTISAMPLE", UINT2NUM(GL_SAMPLER_2D_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_2D_MULTISAMPLE", UINT2NUM(GL_INT_SAMPLER_2D_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE));
    rb_define_const(rb_mGL, "GL_SAMPLER_2D_MULTISAMPLE_ARRAY", UINT2NUM(GL_SAMPLER_2D_MULTISAMPLE_ARRAY));
    rb_define_const(rb_mGL, "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY", UINT2NUM(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY));
    rb_define_const(rb_mGL, "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY", UINT2NUM(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY));
    rb_define_const(rb_mGL, "GL_MAX_COLOR_TEXTURE_SAMPLES", UINT2NUM(GL_MAX_COLOR_TEXTURE_SAMPLES));
    rb_define_const(rb_mGL, "GL_MAX_DEPTH_TEXTURE_SAMPLES", UINT2NUM(GL_MAX_DEPTH_TEXTURE_SAMPLES));
    rb_define_const(rb_mGL, "GL_MAX_INTEGER_SAMPLES", UINT2NUM(GL_MAX_INTEGER_SAMPLES));
    rb_define_const(rb_mGL, "GL_VERTEX_ATTRIB_ARRAY_DIVISOR", UINT2NUM(GL_VERTEX_ATTRIB_ARRAY_DIVISOR));
    rb_define_const(rb_mGL, "GL_SRC1_COLOR", UINT2NUM(GL_SRC1_COLOR));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_SRC1_COLOR", UINT2NUM(GL_ONE_MINUS_SRC1_COLOR));
    rb_define_const(rb_mGL, "GL_ONE_MINUS_SRC1_ALPHA", UINT2NUM(GL_ONE_MINUS_SRC1_ALPHA));
    rb_define_const(rb_mGL, "GL_MAX_DUAL_SOURCE_DRAW_BUFFERS", UINT2NUM(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS));
    rb_define_const(rb_mGL, "GL_ANY_SAMPLES_PASSED", UINT2NUM(GL_ANY_SAMPLES_PASSED));
    rb_define_const(rb_mGL, "GL_SAMPLER_BINDING", UINT2NUM(GL_SAMPLER_BINDING));
    rb_define_const(rb_mGL, "GL_RGB10_A2UI", UINT2NUM(GL_RGB10_A2UI));
    rb_define_const(rb_mGL, "GL_TEXTURE_SWIZZLE_R", UINT2NUM(GL_TEXTURE_SWIZZLE_R));
    rb_define_const(rb_mGL, "GL_TEXTURE_SWIZZLE_G", UINT2NUM(GL_TEXTURE_SWIZZLE_G));
    rb_define_const(rb_mGL, "GL_TEXTURE_SWIZZLE_B", UINT2NUM(GL_TEXTURE_SWIZZLE_B));
    rb_define_const(rb_mGL, "GL_TEXTURE_SWIZZLE_A", UINT2NUM(GL_TEXTURE_SWIZZLE_A));
    rb_define_const(rb_mGL, "GL_TEXTURE_SWIZZLE_RGBA", UINT2NUM(GL_TEXTURE_SWIZZLE_RGBA));
    rb_define_const(rb_mGL, "GL_TIME_ELAPSED", UINT2NUM(GL_TIME_ELAPSED));
    rb_define_const(rb_mGL, "GL_TIMESTAMP", UINT2NUM(GL_TIMESTAMP));
    rb_define_const(rb_mGL, "GL_INT_2_10_10_10_REV", UINT2NUM(GL_INT_2_10_10_10_REV));
}