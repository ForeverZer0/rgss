
module RGSS

  class TextShader < Shader

    def initialize
      vert = File.read(File.join(__dir__, 'shaders', 'sprite-vert.glsl'))
      frag = File.read(File.join(__dir__, 'shaders', 'font-frag.glsl'))
      super(vert, frag)
      @model_id = locate('model')
      @color_id = locate('color')
    end

    def configure(renderable)
      use
      glUniformMatrix4fv(@model_id, 1, false, renderable.model)
      glUniform4fv(@color_id, 1, renderable.color)
    end

  end

  class SpriteShader < Shader

    def initialize
      vert = File.read(File.join(__dir__, 'shaders', 'sprite-vert.glsl'))
      frag = File.read(File.join(__dir__, 'shaders', 'sprite-frag.glsl'))
      super(vert, frag)
      @model_id = locate('model')
      @hue_id = locate('hue')
      @color_id = locate('color')
      @tone_id = locate('tone')
      @flash_id = locate('flash')
      @opacity_id = locate('opacity')
    end

    def configure(renderable)
      use
      glUniformMatrix4fv(@model_id, 1, false, renderable.model)
      glUniform4fv(@color_id, 1, renderable.color)
      glUniform4fv(@tone_id, 1, renderable.tone)
      glUniform4fv(@flash_id, 1, renderable.flash_color)
      glUniform1f(@hue_id, renderable.hue)
      glUniform1f(@opacity_id, renderable.opacity)
    end

    def model(value)
      glUniformMatrix4fv(@model_id, 1, false, value)
    end

    def color(color)
      glUniform4fv(@color_id, 1, color)
    end

    def tone(tone)
      glUniform4fv(@tone_id, 1, tone)
    end

    def flash_color(color)
      glUniform4fv(@flash_id, 1, color)
    end

    def hue(degrees)
      glUniform1f(@hue_id, degrees)
    end

    def opacity(alpha)
      glUniform1f(@opacity_id, alpha)
    end
  end

  class Renderable < Entity

    include GL

    VERTICES_COUNT = 16
    VERTICES_SIZE = SIZEOF_FLOAT * VERTICES_COUNT
    VERTICES_STRIDE = SIZEOF_FLOAT * 4

    def self.shader
      @shader ||= SpriteShader.new
    end

    ##
    # @return [Color]
    attr_accessor :color

    ##
    # @return [Tone]
    attr_accessor :tone

    ##
    # @return [Float]
    attr_accessor :hue

    ##
    # @return [Float]
    attr_reader :opacity

    ##
    # @return [Color]
    attr_reader :flash_color

    ##
    # @return [Boolean]
    attr_accessor :visible

    ##
    # @return [Blend]
    attr_accessor :blend

    def initialize(parent, **opts)
      super()
      @parent = parent.is_a?(RGSS::Batch) ? parent : Graphics.batch
      @parent.add(self)

      @visible = true
      @color = Color::NONE
      @tone = Tone.new(0.0, 0.0, 0.0, 0.0)
      @flash_color = Color::NONE
      @flash_duration = -1
      @hue = 0.0
      @opacity = 1.0
      @blend = Blend.default
    end

    def dispose
      @parent&.remove(self)
      glDeleteVertexArray(@vao) if @vao
      glDeleteBuffer(@vbo) if @vbo
      glDeleteBuffer(@ebo) if @ebo
    end

    def flashing?
      @flash_duration > -1
    end

    def opacity=(value)
      @opacity = [0.0, [1.0, value.to_f].min].max
    end

    def update(delta)
      super
      if @flash_duration > -1
        @flash_duration -= 1
        @flash_color = Color::NONE if @flash_duration < 0
      end
    end

    def flash(color, duration)
      @flash_color = color
      @flash_duration = duration
    end

    def render(delta)

      if @blend
        glBlendEquation(@blend.op)
        glBlendFunc(@blend.src, @blend.dst)
      end

      Renderable.shader.configure(self)
    end

    def z=(value)
      @parent.invalidate
      super(value)
    end

    protected

    def parent
      @parent
    end

    def vertex_setup(vertices = nil, indices = nil, vbo_usage = GL_DYNAMIC_DRAW, ebo_usage = GL_STATIC_DRAW)

      @vao = glGenVertexArray
      @vbo = glGenBuffer
      @ebo = glGenBuffer

      glBindVertexArray(@vao)
      glBindBuffer(GL_ARRAY_BUFFER, @vbo)
      glBufferData(GL_ARRAY_BUFFER, vertices&.bytesize || VERTICES_SIZE, vertices, vbo_usage)

      indices ||= [0, 1, 2, 0, 3, 1].pack('C*')
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, @ebo)
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.bytesize, indices, ebo_usage)

      glEnableVertexAttribArray(0)
      glVertexAttribPointer(0, 4, GL_FLOAT, false, SIZEOF_FLOAT * 4, nil)
      glBindVertexArray(GL_NONE)
    end
  end
end