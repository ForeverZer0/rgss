
module RGSS

  class Texture < GameObject

    attr_reader :width
    attr_reader :height

    include GL

    def initialize(*args, **opts)
      super(glGenTexture)

      image = nil
      case args.size
      when 1
        source = args.first
        image = source.is_a?(String) ? Image.new(source) : source
        raise(TypeError, "#{source} is not a String or Image") unless image.is_a?(Image)
      when 2
        image = Image.new(*args, **opts)
      else
        raise(ArgumentError, "wrong number of arguments (given #{args.size}, expected 1, 2)")
      end

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, self.id);
  
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.address);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      @width = image.width
      @height = image.height
      image.dispose
      @fbo = 0
    end

    def size
      Size.new(@width, @height)
    end

    def rect
      Rect.new(0, 0, @width, @height)
    end

    def bind(target = 0)
      glActiveTexture(GL_TEXTURE0 + target)
      glBindTexture(GL_TEXTURE_2D, self.id);
    end

    def target(area = nil)
      raise(LocalJumpError, "block required") unless block_given?

      area ||= Rect.new(0, 0, @width, @height)

      Graphics.project(area.x, area.y, area.width, area.height) do

        bind_framebuffer
        glViewport(area.x, area.y, area.width, area.height)
        glScissor(area.x, area.y, area.width, area.height)
        yield
      end
    
      Graphics.restore
      self
    end

    def blit(src, src_rect, dst_rect, opacity = 1.0)
      raise(ArgumentError, "source Texture cannot be nil") unless src
      

    end

    def clear
      target do
        glClearColor(nil)
        glClear(GL_COLOR_BUFFER_BIT)
      end
    end

    def fill_rect(rect, color)
      target(rect) do
        glClearColor(color)
        glClear(GL_COLOR_BUFFER_BIT)
      end
    end

    def to_image
      pixels = "\0" * @width * @height * 4
      bind_framebuffer { glReadPixels(0, 0, @width, @height, GL_RGBA, GL_UNSIGNED_BYTE, pixels) }
      Image.new(@width, @height, pixels)
    end

    def valid?
      self.id.nonzero? && glIsTexture(self.id)
    end

    def dispose
      if @fbo && @fbo.nonzero?
        glDeleteFramebuffer(@fbo)
        @fbo = nil
      end
      glDeleteTexture(self.id)
      super
    end

    protected

    def bind_framebuffer
      if @fbo.zero?
        @fbo = glGenFramebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, @fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self.id, 0);
      else
        glBindFramebuffer(GL_FRAMEBUFFER, @fbo);
      end  

      if block_given?
        yield
        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE)
      end

    end

    def undbind_framebuffer
      glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE)
    end
  end
end