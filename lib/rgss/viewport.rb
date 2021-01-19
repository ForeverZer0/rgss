
module RGSS

  class Viewport < Renderable

    # include GL

    attr_reader :batch
    attr_accessor :back_color

    def initialize(*args)
      super(Graphics.batch)

      @batch = Batch.new
      rect = Rect.new(*args)
      @ortho = Mat4.ortho(0, rect.width, 0.0, rect.height, -1.0, 1.0)

      self.location = rect.location
      self.size = rect.size

      @texture = Texture.new(self.width, self.height)
      @framebuffer = glGenFramebuffer
      glBindFramebuffer(GL_FRAMEBUFFER, @framebuffer);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, @texture.id, 0)
      glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE)

      vertices = 
      [
        0.0, 1.0, 0.0, 1.0, # Bottom-Left
        1.0, 0.0, 1.0, 0.0, # Top-Right
        0.0, 0.0, 0.0, 0.0, # Top-Left
        1.0, 1.0, 1.0, 1.0, # Bottom-Right
      ]
      vertex_setup(vertices, nil, GL_STATIC_DRAW)
      @back_color = nil
    end


    def render(delta)
      return unless self.visible && self.opacity > 0.0

      glBindFramebuffer(GL_FRAMEBUFFER, @framebuffer)
      glClearColor(@back_color)
      glClear(GL_COLOR_BUFFER_BIT)
      glViewport(0, 0, self.width, self.height)


      Graphics.project(@ortho) do
        @batch.each { |obj| obj.render(delta) }
      end
      Graphics.restore

      super(delta)
      @texture.bind
      glBindVertexArray(self.vao)
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nil)
      glBindVertexArray(GL_NONE)
    end

    def dispose
      super
      glDeleteFramebuffer(@framebuffer)
      glDeleteTexture(@texture)
    end

  end
end