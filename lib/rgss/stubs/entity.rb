
module RGSS

  class Entity

    ##
    # @param delta [Float]
    # @return [void]
    def update(delta)
    end

    ##
    # @param angle [Float]
    # @param pivot [Vec2?]
    # @return [void]
    def rotate(angle, pivot = nil)
    end

    ##
    # @return [Mat4]
    attr_accessor :model

    ##
    # @return [Vec2]
    attr_accessor :position

    ##
    # @return [Vec2]
    attr_accessor :velocity

    ##
    # @return [Vec2]
    attr_accessor :scale

    ##
    # @return [Vec2]
    attr_accessor :pivot

    ##
    # @return [Float]
    attr_accessor :angle

    ##
    # @return [Integer]
    attr_accessor :x

    ##
    # @return [Integer]
    attr_accessor :y

    ##
    # @return [Integer]
    attr_accessor :z

    alias_method :depth, :z

    ##
    # @return [Point]
    attr_accessor :location

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
    # @return [Size]
    def size=(value)
    end

    ##
    # @param value [Integer]
    # @return [Integer]
    def width=(value)
    end

    ##
    # @param value [Integer]
    # @return [Integer]
    def height=(value)
    end    
  end
end