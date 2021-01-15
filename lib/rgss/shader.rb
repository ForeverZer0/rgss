
module RGSS

  class Shader < GameObject

    PROJECTION_BINDING = 0

    extend GL
    include GL

    def initialize(vertex, fragment, geometry = nil)
      super(glCreateProgram)

      raise(ArgumentError, 'vertex source cannot be nil') unless vertex
      raise(ArgumentError, 'fragment source cannot be nil') unless fragment

      v = compile(GL_VERTEX_SHADER, vertex)
      f = compile(GL_FRAGMENT_SHADER, fragment)
      g = geometry ? compile(GL_GEOMETRY_SHADER, geometry) : 0

      glAttachShader(self.id, v)
      glAttachShader(self.id, f)
      glAttachShader(self.id, g) if g > 0

      glLinkProgram(self.id)

      [v, f, g].each do |shader|
        next if shader.zero?
        glDetachShader(self.id, shader)
        glDeleteShader(shader)
      end

      buffer = "\0" * SIZEOF_INT
      glGetProgramiv(self.id, GL_LINK_STATUS, buffer)

      return if buffer.unpack1('i') == GL_TRUE

      glGetProgramiv(self.id, GL_INFO_LOG_LENGTH, buffer)
      length = buffer.unpack1('i')
      message = "\0" * length

      glGetProgramInfoLog(self.id, lengh, buffer, message)
      raise(RuntimeError, "failed to link shader program: #{message}")

      index = glGetUniformBlockIndex(self.id, 'ortho')
      glUniformBlockBinding(@id, index, 0) if index >= 0
      
      p index

      if block_given?
        yield self
        dispose
      end
    end

    def dispose
      glDeleteProgram(self.id) if self.id.nonzero?
      super
    end

    def valid?
      self.id.nonzero? && glIsProgram(self.id)
    end

    def locate(uniform)
      return uniform ? glGetUniformLocation(self.id, uniform) : -1
    end

    def use
      glUseProgram(@id)
    end

    protected

    def compile(type, source)

      id = glCreateShader(type)
      glShaderSource(id, source)
      glCompileShader(id)

      buffer = "\0" * SIZEOF_INT
      glGetShaderiv(id, GL_COMPILE_STATUS, buffer)
      return id if buffer.unpack1('i') == GL_TRUE

      glGetShaderiv(id, GL_INFO_LOG_LENGTH, buffer)
      length = buffer.unpack1('i')
      message = "\0" * length

      glGetShaderInfoLog(id, length, buffer, message)
      raise(SyntaxError, "failed to compile shader: #{message}")
    end
  end

end