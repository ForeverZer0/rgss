
module RGSS

  class Entity

    ##
    # @param delta [Float]
    # @return [void]
    def update(delta)
    end

    ##
    # @param angle [Float]
    # @param pivot [Vec2,Point,NilClass]
    # @return [void]
    def rotate(angle, pivot = nil)
    end

    ##
    # @return [Mat4]
    def model
    end

    ##
    # @param value [Mat4]
    # @return [Mat4]
    def model=(value)
    end

    ##
    # @return [Vec2]
    def position
    end

    ##
    # @param [Vec2,Point]
    # @return [Vec2]
    def position=(value)
    end

    ##
    # @return [Vec2]
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