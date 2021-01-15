module RGSS
  ##
  # A structure for altering the "balance" of colors and/or applying a grayscale effect. While it may appear similar to
  # color, it is not. It represents an **alteration** to a color, not the color itself. Internally this class is
  # represented as a typical 4 component vector, with 32-bit floating point values. Ruby uses a 64-bit `double` for
  # floating point numbers, which have a higher precision (about 13 digits) compared to a 32-bit `float` (about 7
  # digits), so bear this in mind and do not rely on any precision or accuracy of values beyond 6-7 digits, they should
  # be ignored beyond that point.
  #
  # For the RGB components, it adjusts the output color by simply adding/subtracting on a component-wise basis, where a
  # value of `0.0` has no effect, `-1.0` completely removes the component, and `1.0` is full output. The grayscale
  # component is calculated independently, and is a linear scaling factor on how strong the effect will be, where `0.0`
  # is no effect, and `1.0` is fully "shades of gray", with no color.
  #
  # @note All out of range values are automatically clamped to their respective limits.
  #
  # @note Unlike RGSS Maker games, the grayscale has no negative performance effect to use, as the exact same
  #   calculations are performed in the fragment shader with each draw, regardless of its value. Whether you use a value
  #   of `0.0` or `1.0`, it will have absolutely no effect on FPS or resource usage.
  #
  # @note All relevant methods implemented in `Object` (i.e. `#dup`, `#clone`, Marshalling, equality) have also been
  #   properly implemented/overridden, but have been omitted from the documentation for brevity.
  class Tone

    ##
    # The value of the red color component.
    # @return [Float] a normalized value between `-1.0` amd `1.0`.
    attr_reader :r

    ##
    # The value of the green color component.
    # @return [Float] a normalized value between `-1.0` amd `1.0`.
    attr_reader :g

    ##
    # The value of the blue color component.
    # @return [Float] a normalized value between `-1.0` amd `1.0`.
    attr_reader :b

    ##
    # The amount of grayscale that will be applied, where `0.0` is none, and `1.0` is fully gray (i.e. "black & white").
    # @return [Float] a normalized value between `0.0` and `1.0`.
    attr_reader :gray

    ##
    # Creates a new instance of the {Tone} class.
    #
    # @param red [Float] the value of the red color component, a normalized value between `-1.0` and `1.0`.
    # @param green [Float] the value of the green color component, a normalized value between `-1.0` and `1.0`.
    # @param blue [Float] the value of the blue color component, a normalized value between `-1.0` and `1.0`.
    # @param gray [Float] the value of the grayscale factor to apply, a normalized value between `0.0` and `1.0`.
    def initialize(red, green, blue, gray = 0.0)
    end

    alias_method :red, :r
    alias_method :green, :g
    alias_method :blue, :b
    alias_method :grey, :gray

    ##
    # A full grayscale {Tone} instance.
    GRAY = Tone.new(0.0, 0.0, 0.0, 1.0)

  end
end