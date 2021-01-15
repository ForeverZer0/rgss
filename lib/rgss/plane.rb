

module RGSS

  class Plane < Renderable

    attr_reader :zoom
    attr_reader :scroll
    attr_reader :texture

    def initialize(viewport = nil)
      super(viewport&.batch || Graphics.batch)

      vertex_setup

      @sampler = glGenSampler
      glSamplerParameteri(@sampler, GL_TEXTURE_WRAP_S, GL_REPEAT)
      glSamplerParameteri(@sampler, GL_TEXTURE_WRAP_T, GL_REPEAT)
      glSamplerParameteri(@sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST) # TODO: Linear
      glSamplerParameteri(@sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST)

      @scroll = Vec2.zero
      @zoom = Vec2.one
      @origin = Vec2.zero
      @texture = nil
      self.size = Size.empty
    end

    def scroll=(value)
      @scroll = value || Vec2.zero
      value
    end

    def texture=(value)
      @texture = value
      update_vertices
      value
    end

    def size=(value)
      super
      update_vertices
      value
    end

    def zoom=(value)
      @zoom = value || Vec2.one
      # update_vertices
      value
    end

    def update(delta)
      super(delta)
      @origin += @scroll * delta
      update_vertices
    end

    def render(alpha)
      return unless @texture && @visible && @opacity > 0.0
      super(alpha)

      glBindSampler(0, @sampler)
      @texture.bind

      glBindVertexArray(@vao)
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nil)
      
      glBindVertexArray(GL_NONE)
      glBindSampler(0, GL_NONE)
    end

    protected

    def update_vertices
      return unless @texture

      l = (@origin.x / self.width) *  @zoom.x;
      t = (@origin.y / self.height) * @zoom.y;
      r = l + ((self.width /  @texture.width.to_f) * @zoom.x);
      b = t + ((self.height / @texture.height.to_f) * @zoom.y);

      vertices = 
      [
        0.0, 1.0, l, b, # Bottom-Left
        1.0, 0.0, r, t, # Top-Right
        0.0, 0.0, l, t, # Top-Left
        1.0, 1.0, r, b, # Bottom-Right
      ].pack('f*')
      glBindBuffer(GL_ARRAY_BUFFER, @vbo)
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size, vertices)
      glBindBuffer(GL_ARRAY_BUFFER, 0)
    end

  end
end