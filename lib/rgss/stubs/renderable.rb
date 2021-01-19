
module RGSS

  ##
  # @abstract
  class Renderable < Entity

    include GL # TODO: Remove and state in summary, as this pulls in waaaaay to many methods/constants

    FLIP_NONE = 0x00
    FLIP_X    = 0x01
    FLIP_Y    = 0x02
    FLIP_BOTH = FLIP_X | FLIP_Y

    VERTICES_COUNT  = 16
    VERTICES_SIZE   = SIZEOF_FLOAT * VERTICES_COUNT
    VERTICES_STRIDE = SIZEOF_FLOAT * 4
    INDICES_COUNT   = 6

    attr_accessor :color
    attr_accessor :flash_color
    attr_accessor :flash_duration
    attr_accessor :tone
    attr_accessor :opacity
    attr_accessor :hue
    attr_accessor :visible
    attr_accessor :blend
    attr_accessor :flip

    def dispose
    end

    def disposed?
    end

    def update(delta)
    end

    def flash(color, ticks)
    end

    def flashing?
    end
  
    def render(alpha)
    end

    def initialize(batch)
    end

    protected

    def parent
    end

    def vertex_setup(vertices = nil, indices = nil, vbo_usage = GL_DYNAMIC_DRAW, ebo_usage = GL_STATIC_DRAW)
    end

    def update_buffer(vertices)
    end

    def vao
    end

    def vbo
    end

    def ebo
    end

  end
end
