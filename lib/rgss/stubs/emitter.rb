module RGSS

  ##
  # A general purpose particle emitter, capable of managing and rendering vast
  # quantities of particles, each with variable properties.
  #
  # Internally the class leverages the power of instanced rendering, a common technique
  # that allows drawing very large amounts of similiar data with much less overhead cost. By
  # employing this technique, the emitter is capable of smoothly drawing even hundreds of
  # thousands of particles, but it is important to remember that not all hardware is equal.
  # While it may work well on a basic modern system, some users of your game may be using
  # older hardware that struggles under such a load.
  class Emitter < Renderable

    ##
    # The amount of force to simulate Earth gravity, measured in meters per second.
    # Multiply this by the number of pixels that represent a meter in your world to apply.
    EARTH_GRAVITY = 9.81

    ##
    # The source texture used for particles. When `nil`, it defaults to simply using pixels
    # that are colored as defined in the {#spectrum} property, and of the size specified by
    # {#width}, #{height}, and #{size}.
    #
    # @return [Texture,NilClass] the source texture used for each particle.
    attr_accessor :texture

    
    attr_accessor :wind
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

    def width=(value)
    end

    def height=(value)
    end

    def size=(value)
    end
  end
end