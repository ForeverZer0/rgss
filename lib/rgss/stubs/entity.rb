
module RGSS

  ##
  # @abstract 
  # Base class for objects that can occupy world space, with position, size, rotation, scale, etc.
  #
  # It essentially provides a mechanism for automatically maintaining a model matrix that the GPU
  # understands using only standard 2D positioning. While it is unlikely this class will ever be
  # used directly, it can be a huge timesaver for those who want to implement custom rendering
  # techniques but wish to avoid the hassle of computing their own matrix, as these are also often 
  # computationally expensive operations to perform in a  scripted language. Internally, these
  # calculations leverage the power of SIMD instructions to be quite resource-friendly for the CPU.
  class Entity

    ##
    # Called once each game tick, applies velocity and updates the model matrix of the object with
    # any pending changes.
    #
    # @param delta [Float] The frame delta, based off the target tick rate of the game.
    #
    # @return [void]
    def update(delta)
    end

    ##
    # Rotates given entity around a given point.
    #
    # @param angle [Float] The rotate to apply, in degree units (0.0...360.0), values out of range will wrap.
    # @param pivot [Vec2,Point,NilClass] The desired pivot point to rotate around **relative to the entity**.
    #   When pivot is `nil`, the rotation point is the center of the object's bounds.
    #
    # @return [void]
    def rotate(angle, pivot = nil)
    end

    ##
    # Gets the model matrix of the entity, used by the internal and shaders to determine positioning in world space.
    # Unless implementing a custom rendering techniques, this value will rarely ever be needed, and should be
    # considered a "black box".
    #
    # @return [Mat4] The current model matrix.
    def model
    end

    ##
    # Sets the model matrix of the entity, used by the internal and shaders to determine positioning in world space.
    #
    # @param value [Mat4] The model matrix to apply.
    # @return [Mat4] The value that was given.
    def model=(value)
    end

    ##
    # Gets the current position of the entity with sub-pixel precision.
    #
    # @return [Vec2] The current position of the entity.
    def position
    end

    ##
    # Ses the current position of the entity, with sub-pixel precision.
    #
    # @param [Vec2,Point] The position to set.
    # @return [Vec2] The value that was given.
    def position=(value)
    end

    ##
    # Gets the amount of offset to apply to the entity's position each game tick, using
    # pixel-per-second units, where a "pixel" is relative to the internal resolution,
    # not necessarily screen pixels (i.e. window has been resized).
    #
    # @return [Vec2] The current velocity.
    def velocity
    end

    ##
    # @param [Vec2,Point]
    # @return [Vec2]
    def velocity=(value)
    end

    ##
    # @return [Vec2]
    def scale
    end

    ##
    # @param [Vec2,Point]
    # @return [Vec2]
    def scale=(value)
    end

    ##
    # @return [Vec2]
    def pivot
    end

    ##
    # @param [Vec2,Point]
    # @return [Vec2]
    def pivot=(value)
    end

    ##
    # @return [Float]
    def angle
    end

    ##
    # @param [Numeric]
    # @return [Numeric]
    def angle=(value)
    end

    ##
    # @return [Integer]
    def x
    end

    ##
    # @param [Numeric]
    # @return [Numeric]
    def x=(value)
    end

    ##
    # @return [Integer]
    def y
    end

    ##
    # @param [Numeric]
    # @return [Numeric]
    def y=(value)
    end

    ##
    # @return [Integer]
    def z
    end

    ##
    # @param [Numeric]
    # @return [Numeric]
    def z=(value)
    end

    alias_method :depth, :z

    ##
    # @return [Point,Vec2]
    def location
    end

    ##
    # @param [Point]
    # @return [Point]
    def x=(value)
    end

    ##
    # @return [Size]
    def size
    end

    ##
    # @return [Integer]
    def width
    end

    ##
    # @return [Integer]
    def height
    end

    ##
    # @return [Rect]
    def bounds
    end

    protected

    ##
    # @param value [Size]
    # @return [Size,Vec2]
    def size=(value)
    end

    ##
    # @param value [Numeric]
    # @return [Numeric]
    def width=(value)
    end

    ##
    # @param value [Numeric]
    # @return [Numeric]
    def height=(value)
    end    
  end
end