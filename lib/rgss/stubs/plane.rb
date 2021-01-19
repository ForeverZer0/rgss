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

    def size=(value)
    end
  end
end