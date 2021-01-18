
module RGSS

  class Texture < GameObject

    attr_reader :width
    attr_reader :height

    include GL

    def self.from_id(id, width, height)
      texture = allocate
      texture.instance_variable_set(:@id, id)
      texture.instance_variable_set(:@width, width)
      texture.instance_variable_set(:@height, height)
      texture.instance_variable_set(:@fbo, 0)
      texture
    end

    def self.default_options
      @default_options ||= 
      { 
        format:     GL_RGBA, 
        internal:   GL_RGBA,
        type:       GL_UNSIGNED_BYTE, 
        wrap_s:     GL_CLAMP_TO_EDGE, 
        wrap_t:     GL_CLAMP_TO_EDGE, 
        min_filter: GL_NEAREST, 
        max_filter: GL_LINEAR 
      }
    end

    def self.load(path, **opts)
      raise(ArgumentError, 'path cannot be nil') unless path
      raise(Errno::ENOENT, path) unless File.exist?(path)
      img = Image.new(path)
      new(img.width, img.height, img.address)
    end

    def initialize(width, height, data = nil, **opts)
      super(glGenTexture)
      
      raise(ArgumentError, 'width must be greater than 0') unless width > 0
      raise(ArgumentError, 'height must be greater than 0') unless height > 0

      @width = width
      @height = height
      @fbo = 0
      op = self.class.default_options.merge(opts)

      glActiveTexture(GL_TEXTURE0)
      glBindTexture(GL_TEXTURE_2D, @id)
      glTexImage2D(GL_TEXTURE_2D, 0, op[:format], width, height, 0, op[:internal], op[:type], data)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, op[:wrap_s])
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, op[:wrap_t])
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, op[:min_filter])
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, op[:max_filter])
    end

    def size
      Size.new(@width, @height)
    end

    def rect
      Rect.new(0, 0, @width, @height)
    end

    alias_method :bounds, :rect

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