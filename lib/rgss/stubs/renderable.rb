
module RGSS

  ##
  # @abstract 
  # Base class for all "sprite-like" objects that use the primary shader that is built-in.
  # 
  # Alone this class does nothing and cannot render anything, though it will automatically prepare
  # the shader and setup the context for a drawing call for classes that utilize it.
  class Renderable < Entity

    FLIP_NONE = 0x00
    FLIP_X    = 0x01
    FLIP_Y    = 0x02
    FLIP_BOTH = FLIP_X | FLIP_Y

    attr_accessor :color
    attr_accessor :flash_color
    attr_accessor :flash_duration
    attr_accessor :tone
    attr_accessor :opacity
    attr_accessor :hue
    attr_accessor :visible
    attr_accessor :blend
    attr_accessor :flip

    ##
    # Releases unmanaged resources, including the vertex arrays, buffers, etc., and invalidates
    # this object for futher use. Once disposed, most methods will cause an exception if called,
    # with the exception of {#disposed?}, which will **never** cause an exception.
    #
    # @return [void]
    def dispose
    end

    ##
    # Checks if the object has been disposed and is invalid for further use.
    # @return [Boolean] `true` if object has been disposed, otherwise `false`.
    def disposed?
    end

    ##
    # Frame update, should could be called once each tick to update any pending state changes.
    #
    # @note Unlike RPG Maker games, it is required to update every instance each frame, not just
    #   those that "flash", as the update also invokes the {Entity#update} to update the model
    #   matrix, which determines location, size, rotation, etc.
    def update(delta)
    end

    ##
    # Performs a flashing effect, where another color is temporarily blended with the object's
    # pixels (seperate from the {#color} blending).
    #
    # @param color [Color,NilClass] The color to flash, or `nil` to cancel a current flash effect.
    # @param ticks [Numeric] The number of game ticks the effect should last for.
    #
    # @return [void]
    # @see flashing?
    # @note This method is primarily to maintain a similiar API to that of RPG Maker.
    def flash(color, ticks)
    end

    ##
    # @return [Boolean] `true` if object is currently flashing, otherwise `false`.
    # @see flash
    def flashing?
    end
  
    ##
    # Renders the object onto the currenlty bound framebuffer, typically the screen or a {Viewport}.
    #
    # @param alpha [Float] A value between `0.0` and `1.0` indicating the percentage of elapsed time
    #   in the current game tick. This value is independent of speed or tick rate of the game, and
    #   can be used to interpolate between two states.
    #
    # @return [void]
    def render(alpha)
    end

    ##
    # Gets a unique numerical identifer for this instance. No two {Renderable} objects will possess
    # the same ID at the same time, so this can be a very efficient method of comparing two variables
    # are referring to the same object. 
    #
    # @note The same ID may be reused with a new object after another has been disposed.
    def id
    end

    def initialize(batch)
    end


    # @!group OpenGL Developer

    VERTICES_COUNT  = 16
    VERTICES_SIZE   = SIZEOF_FLOAT * VERTICES_COUNT
    VERTICES_STRIDE = SIZEOF_FLOAT * 4
    INDICES_COUNT   = 6

    protected

    ##
    # @api OpenGL
    # @return [Integer] the name of the OpenGL vertex array object used for this object.
    def vao
    end

    ##
    # @api OpenGL
    # @return [Integer] the name of the OpenGL vertex buffer object used for this object.
    def vbo
    end

    ##
    # @api OpenGL
    # @return [Integer] the name of the OpenGL element/index buffer object used for this object.
    def ebo
    end

    def vertex_setup(vertices = nil, indices = nil, vbo_usage = GL_DYNAMIC_DRAW, ebo_usage = GL_STATIC_DRAW)
    end

    ##
    # @api OpenGL
    # Updates the vertices in the object's vertex buffer.
    # 
    # This function is essentially the same as performing the following, but without the need to pack
    # any the data into a String or other C-compatible type. 
    #
    # @example
    #   vertices = 
    #   [
    #       0.0, 1.0, 0.0, 1.0, # Bottom-Left
    #       1.0, 0.0, 1.0, 0.0, # Top-Right
    #       0.0, 0.0, 0.0, 0.0, # Top-Left
    #       1.0, 1.0, 1.0, 1.0  # Bottom-Right
    #   ]
    #
    #   data = vertices.pack('f*')
    #   glBindBuffer(GL_ARRAY_BUFFER, self.vbo)
    #   glBufferSubData(GL_ARRAY_BUFFER, 0, data.bytesize, data)
    #   glBindBuffer(GL_ARRAY_BUFFER, GL_NONE)
    #
    # @param vertices [Array<Numeric>] An array containing {VERTICES_COUNT} values.
    # @return [void]
    # @raise [ArgumentError] when array is not of {VERTICES_COUNT} size.
    # @raise [TypeError] when not all items in the array are Numeric.
    def update_buffer(vertices)
    end

    # @!endgroup

    def parent
    end

  end
end
