module RGSS

  class Plane < Renderable

    ##
    # @return [Vec2]
    attr_accessor :zoom

    ##
    # @return [Vec2]
    attr_accessor :scale

    ##
    # @return [Vec2]
    attr_accessor :origin

    ##
    # @param value [Numeric]
    # @return [Numeric]
    def width=(value)
    end

    ##
    # @param value [Numeric]
    # @param [Numeric]
    def height=(value)
    end

    ##
    # @param [Vec2,Size,NilClass]
    # @param [Vec2,Size,NilClass] returns the given value.
    def size=(value)
    end
  end
end