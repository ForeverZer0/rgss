module RGSS

  ##
  # A general purpose particle emitter, capable of managing and rendering vast
  # quantities of particles, each with variable properties and configurable degree
  # of "randomness".
  #
  # Internally the class leverages the power of instanced rendering, a common technique
  # that allows drawing very large amounts of similiar data with much less overhead cost. By
  # employing this technique, the emitter is capable of smoothly drawing even hundreds of
  # thousands of particles, but it is important to remember that not all hardware is equal.
  # While it may work well on a basic/modern system, some users of your game may be using
  # older/less powerful hardware that struggles under such a load.
  class Emitter < Renderable

    # @!macro [new] getrange
    #   @return [Range(Float,Float)] a Range object with an inclusive Float upper and lower limit.

    # @!macro [new] setrange
    #   @param value [Numeric,Range,IVec2,Vec2,NilClass] Specifies the value or range of values
    #     that will be used. When value is a Range or vector type, it is interpreted as a minimum
    #     and maximum set of values, and a random value between those limits will be selected. When
    #     specifying a Numeric, it will be create a Range with the given value as the upper and lower
    #     limit.
    #   @return [Numeric,Range,IVec2,Vec2,NilClass] the given value.


    ##
    # The amount of force to simulate Earth gravity, measured in meters per second.
    # Multiply this by the number of pixels that represent a meter in your world to apply.
    EARTH_GRAVITY = 9.81

    ##
    # The source texture used for particles. When `nil`, it defaults to simply using circular
    # "point sprites".
    #
    # @return [Texture,NilClass] the source texture used for each particle.
    attr_accessor :texture

    ##
    # Gets the current value determining environmental effect on the x-axis.
    # @!macro getrange
    def wind
    end

    ##
    # Sets the amount of environmental effect on the x-axis, which causes particles to accelerate
    # by the given amount each second. Positive values indicate accelerating to the right (east), while
    # negative values indicate accelerating to the left (west).
    #
    # @macro setrange
    def wind=(value)
    end

    ##
    # Gets the current value determining environmental effect on the y-axis.
    # @!macro getrange
    def gravity
    end

    ##
    # Sets the amount of environmental effect on the y-axis, which causes particles to accelerate
    # by the given amount each second. Positive values indicate accelerating downward (south), while
    # negative values indicate accelerating to upwards (north).
    #
    # @macro setrange
    def gravity=(value)
    end
    
    attr_accessor :gravity
    attr_accessor :radius
    attr_accessor :rate
    attr_accessor :lifespan
    attr_accessor :spectrum
    attr_accessor :friction
    attr_accessor :force

    ##
    # @return [] The number of degrees of rotation to apply each second. Negative values CCW, positive CW
    attr_accessor :rotation
    attr_accessor :fade
    attr_accessor :grow

    attr_accessor :frequency
    attr_reader :interval

    def size=(value) # TODO Change to particle_size
    end
  end
end