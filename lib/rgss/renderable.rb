
module RGSS

  class Renderable < Entity

    include GL

    VERTICES_COUNT = 16
    VERTICES_SIZE = SIZEOF_FLOAT * VERTICES_COUNT
    VERTICES_STRIDE = SIZEOF_FLOAT * 4

    class << self

      attr_reader :projection_id
      attr_reader :model_id
      attr_reader :color_id
      attr_reader :tone_id
      attr_reader :hue_id
      attr_reader :opacity_id
      attr_reader :flash_id

      def shader
        return @shader if @shader

        v = File.read(File.join(__dir__, 'shaders', 'sprite-vert.glsl'))
        f = File.read(File.join(__dir__, 'shaders', 'sprite-frag.glsl'))

        @shader = Shader.new(v, f)
        @projection_id = @shader.locate('projection')
        @model_id = @shader.locate('model')
        @hue_id = @shader.locate('hue')
        @color_id = @shader.locate('color')
        @tone_id = @shader.locate('tone')
        @flash_id = @shader.locate('flash')
        @opacity_id = @shader.locate('opacity')
        @shader
      end

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

    def initialize(parent)
      super()
      @parent = parent.is_a?(RGSS::Batch) ? parent : Graphics.batch
      @parent.add(self)

      @visible = true
      @color = Color::NONE
      @tone = Tone.new(0, 0, 0, 0)
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

      Renderable.shader.use
      glUniformMatrix4fv(Renderable.model_id, 1, false, self.model)
      glUniform4fv(Renderable.color_id, 1, self.color)
      glUniform4fv(Renderable.tone_id, 1, self.tone)
      glUniform4fv(Renderable.flash_id, 1, self.flash_color)
      glUniform1f(Renderable.hue_id, self.hue)
      glUniform1f(Renderable.opacity_id, self.opacity)
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