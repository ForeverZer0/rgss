module RGSS
  ##
  # A structure for describing a color, in RGB, HSL, and HSV/HSB color-spaces, though RGBA is the internal format. All
  # components are normalized floating point values in the range of `0.0` and `1.0`. Internally this class is
  # represented as a typical 4 component vector, with 32-bit floating point values. Ruby uses a 64-bit `double` for
  # floating point numbers, which have a higher precision (about 13 digits) compared to a 32-bit `float` (about 7
  # digits), so bear this in mind and do not rely on any precision or accuracy of values beyond 6-7 digits, they should
  # be ignored beyond that point.
  #
  # @note All out of range values are automatically clamped to their respective limits.
  #
  # @note All relevant methods implemented in `Object` (i.e. `#dup`, `#clone`, Marshalling, equality) have also been
  #   properly implemented/overridden, but have been omitted from the documentation for brevity.
  class Color

    ##
    # The value of the red color component.
    # @return [Float] a normalized value between `0.0` amd `1.0`.
    attr_reader :r

    ##
    # The value of the green color component.
    # @return [Float] a normalized value between `0.0` amd `1.0`.
    attr_reader :g

    ##
    # The value of the blue color component.
    # @return [Float] a normalized value between `0.0` amd `1.0`.
    attr_reader :b

    ##
    # The value of the alpha color component.
    # @return [Float] a normalized value between `0.0` amd `1.0`.
    attr_reader :a

    ##
    # Creates a new instance of the {Color} class.
    # @param red [Float] the red component, a normalized value between `0.0` and `1.0`.
    # @param green [Float] the green component, a normalized value between `0.0` and `1.0`.
    # @param blue [Float] the blue component, a normalized value between `0.0` and `1.0`.
    # @param alpha [Float] the alpha component, a normalized value between `0.0` and `1.0`.
    def initialize(red, green, blue, alpha = 1.0)
    end

    ##
    # Creates an array instance that represents this object.
    # @return [Array(Float, Float, Float, Float)] The newly created Array.
    # @example
    #   Color::BLUE_VIOLET.to_a
    #   #=> []
    def to_a
    end
    BLUE_VIOLET = Color.new(0.54118, 0.16863, 0.88627, 1.00000)
    ##
    # Creates a hash instance that represents this object.
    # @return [Hash{Symbol => Float}] The newly created Hash.
    # @example
    #   Color::BLUE_VIOLET.to_h
    #   #=> { :red => 0.54118, :green => 0.16863, :blue => 0.88627, :alpha => 1.0 }
    def to_h
    end

    ##
    # Creates a new {Color} by inverting the values of the this instance, producing the "negative" of the color.
    # @param alpha [Boolean] `true` to invert the alpha component, otherwise `false` to maintain the same alpha.
    # @return [Color] the resulting {Color} instance.
    #
    # @example
    #   magenta = Color.new(1.00000, 0.00000, 1.00000, 1.00000)
    #   #=> <1.0, 0.0, 1.0, 1.0>
    #   negative = magenta.inverse
    #   #> <0.0, 1.0, 0.0, 1.0>
    def inverse(alpha = false)
    end

    ##
    # Creates a new {Color} by linearly scaling the components of this instance by the specified value.
    # @param scalar [Float] A scalar value to alter this value by.
    # @return [Color] the resulting {Color} instance.
    #
    # @example
    #   white = Color.new(1.0, 1.0, 1.0, 1.0)
    #   #=> <1.0, 1.0, 1.0, 1.0>
    #   mid_gray = white * 0.5
    #   #=> <0.5, 0.5, 0.5, 0.5>
    def *(scalar)
    end

    ##
    # The hue component of the color from the HSL and HSB/HSV color-spaces.
    # @return [Float] the hue of the color, in the range of `0.0` to `360.0`.
    def hue
    end

    ##
    # The saturation component of the color from the HSL and HSB/HSV color-spaces.
    # @return [Float] the hue of the color, in the range of `0.0` to `1.0`.
    def saturation
    end

    ##
    # The lightness component of the color from the HSL color-space.
    # @return [Float] the lightness of the color, in the range of `0.0` to `1.0`.
    def lightness
    end

    ##
    # The brightness/value component of the color from the HSB/HSV color-space.
    # @return [Float] the brightness of the color, in the range of `0.0` to `1.0`.
    def brightness
    end

    ##
    # The values of the color in the HSL color-space.
    # @return [Array(Float, Float, Float)] the HSL components.
    # @note It is more efficient to retrieve all at values at once than each component individually.
    def hsl
    end

    ##
    # The values of the color in the HSB/HSV color-space.
    # @return [Array(Float, Float, Float)] the HSB/HSV components.
    # @note It is more efficient to retrieve all at values at once than each component individually.
    def hsv
    end

    alias_method :red, :r
    alias_method :green, :g
    alias_method :blue, :b
    alias_method :alpha, :a
    alias_method :multiply, :*
    alias_method :value, :brightness
    alias_method :hsb, :hsv

    ##
    # Linearly interpolates two colors using the given the weight to produce a mixture of each.
    # @param color1 [Color] the base color to mix from.
    # @param color2 [Color] the target color to mix to.
    # @param weight [Float] the scaling factor of the second color to apply, where `0.0` means none, and *color1* will
    #   be returned, `1.0` means fully *color2*, and values in between are a weighted mixture.
    # @return [Color] the resulting {Color} instance.
    def self.mix(color1, color2, weight)
    end

    ##
    # Creates a new {Color} using from the HSL color-space.
    # @param hue [Float] the hue component in the range of `0.0` and `360.0`.
    # @param saturation [Float] the saturation component in the range of `0.0` and `1.0`.
    # @param lightness [Float] the lightness component in the range of `0.0` and `1.0`.
    # @param alpha [Float] the alpha component of the color in the range of `0.0` and `1.0`.
    # @return [Color] the newly created {Color}.
    def self.from_hsl(hue, saturation, lightness, alpha = 1.0)
    end

    ##
    # Creates a new {Color} using from the HSB/HSL color-space.
    # @param hue [Float] the hue component in the range of `0.0` and `360.0`.
    # @param saturation [Float] the saturation component in the range of `0.0` and `1.0`.
    # @param brightness [Float] the brightness/value component in the range of `0.0` and `1.0`.
    # @param alpha [Float] the alpha component of the color in the range of `0.0` and `1.0`.
    # @return [Color] the newly created {Color}.
    def self.from_hsb(hue, saturation, brightness, alpha = 1.0)
    end

    ##
    # Creates a new {Color} using from the HSB/HSL color-space.
    # @param hue [Float] the hue component in the range of `0.0` and `360.0`.
    # @param saturation [Float] the saturation component in the range of `0.0` and `1.0`.
    # @param value [Float] the brightness/value component in the range of `0.0` and `1.0`.
    # @param alpha [Float] the alpha component of the color in the range of `0.0` and `1.0`.
    # @return [Color] the newly created {Color}.
    def self.from_hsv(hue, saturation, value, alpha = 1.0)
    end

    NONE = Color.new(0.00000, 0.00000, 0.00000, 0.00000)
    TRANSPARENT = Color.new(1.00000, 1.00000, 1.00000, 0.00000)
    ALICE_BLUE = Color.new(0.94118, 0.97255, 1.00000, 1.00000)
    ANTIQUE_WHITE = Color.new(0.98039, 0.92157, 0.84314, 1.00000)
    AQUA = Color.new(0.00000, 1.00000, 1.00000, 1.00000)
    AQUAMARINE = Color.new(0.49804, 1.00000, 0.83137, 1.00000)
    AZURE = Color.new(0.94118, 1.00000, 1.00000, 1.00000)
    BEIGE = Color.new(0.96078, 0.96078, 0.86275, 1.00000)
    BISQUE = Color.new(1.00000, 0.89412, 0.76863, 1.00000)
    BLACK = Color.new(0.00000, 0.00000, 0.00000, 1.00000)
    BLANCHED_ALMOND = Color.new(1.00000, 0.92157, 0.80392, 1.00000)
    BLUE = Color.new(0.00000, 0.00000, 1.00000, 1.00000)
    BLUE_VIOLET = Color.new(0.54118, 0.16863, 0.88627, 1.00000)
    BROWN = Color.new(0.64706, 0.16471, 0.16471, 1.00000)
    BURLY_WOOD = Color.new(0.87059, 0.72157, 0.52941, 1.00000)
    CADET_BLUE = Color.new(0.37255, 0.61961, 0.62745, 1.00000)
    CHARTREUSE = Color.new(0.49804, 1.00000, 0.00000, 1.00000)
    CHOCOLATE = Color.new(0.82353, 0.41176, 0.11765, 1.00000)
    CORAL = Color.new(1.00000, 0.49804, 0.31373, 1.00000)
    CORNFLOWER_BLUE = Color.new(0.39216, 0.58431, 0.92941, 1.00000)
    CORNSILK = Color.new(1.00000, 0.97255, 0.86275, 1.00000)
    CRIMSON = Color.new(0.86275, 0.07843, 0.23529, 1.00000)
    CYAN = Color.new(0.00000, 1.00000, 1.00000, 1.00000)
    DARK_BLUE = Color.new(0.00000, 0.00000, 0.54510, 1.00000)
    DARK_CYAN = Color.new(0.00000, 0.54510, 0.54510, 1.00000)
    DARK_GOLDENROD = Color.new(0.72157, 0.52549, 0.04314, 1.00000)
    DARK_GRAY = Color.new(0.66275, 0.66275, 0.66275, 1.00000)
    DARK_GREEN = Color.new(0.00000, 0.39216, 0.00000, 1.00000)
    DARK_KHAKI = Color.new(0.74118, 0.71765, 0.41961, 1.00000)
    DARK_MAGENTA = Color.new(0.54510, 0.00000, 0.54510, 1.00000)
    DARK_OLIVE_GREEN = Color.new(0.33333, 0.41961, 0.18431, 1.00000)
    DARK_ORANGE = Color.new(1.00000, 0.54902, 0.00000, 1.00000)
    DARK_ORCHID = Color.new(0.60000, 0.19608, 0.80000, 1.00000)
    DARK_RED = Color.new(0.54510, 0.00000, 0.00000, 1.00000)
    DARK_SALMON = Color.new(0.91373, 0.58824, 0.47843, 1.00000)
    DARK_SEA_GREEN = Color.new(0.56078, 0.73725, 0.54510, 1.00000)
    DARK_SLATE_BLUE = Color.new(0.28235, 0.23922, 0.54510, 1.00000)
    DARK_SLATE_GRAY = Color.new(0.18431, 0.30980, 0.30980, 1.00000)
    DARK_TURQUOISE = Color.new(0.00000, 0.80784, 0.81961, 1.00000)
    DARK_VIOLET = Color.new(0.58039, 0.00000, 0.82745, 1.00000)
    DEEP_PINK = Color.new(1.00000, 0.07843, 0.57647, 1.00000)
    DEEP_SKY_BLUE = Color.new(0.00000, 0.74902, 1.00000, 1.00000)
    DIM_GRAY = Color.new(0.41176, 0.41176, 0.41176, 1.00000)
    DODGER_BLUE = Color.new(0.11765, 0.56471, 1.00000, 1.00000)
    FIREBRICK = Color.new(0.69804, 0.13333, 0.13333, 1.00000)
    FLORAL_WHITE = Color.new(1.00000, 0.98039, 0.94118, 1.00000)
    FOREST_GREEN = Color.new(0.13333, 0.54510, 0.13333, 1.00000)
    FUCHSIA = Color.new(1.00000, 0.00000, 1.00000, 1.00000)
    GAINSBORO = Color.new(0.86275, 0.86275, 0.86275, 1.00000)
    GHOST_WHITE = Color.new(0.97255, 0.97255, 1.00000, 1.00000)
    GOLD = Color.new(1.00000, 0.84314, 0.00000, 1.00000)
    GOLDENROD = Color.new(0.85490, 0.64706, 0.12549, 1.00000)
    GRAY = Color.new(0.50196, 0.50196, 0.50196, 1.00000)
    GREEN = Color.new(0.00000, 0.50196, 0.00000, 1.00000)
    GREEN_YELLOW = Color.new(0.67843, 1.00000, 0.18431, 1.00000)
    HONEYDEW = Color.new(0.94118, 1.00000, 0.94118, 1.00000)
    HOT_PINK = Color.new(1.00000, 0.41176, 0.70588, 1.00000)
    INDIAN_RED = Color.new(0.80392, 0.36078, 0.36078, 1.00000)
    INDIGO = Color.new(0.29412, 0.00000, 0.50980, 1.00000)
    IVORY = Color.new(1.00000, 1.00000, 0.94118, 1.00000)
    KHAKI = Color.new(0.94118, 0.90196, 0.54902, 1.00000)
    LAVENDER = Color.new(0.90196, 0.90196, 0.98039, 1.00000)
    LAVENDER_BLUSH = Color.new(1.00000, 0.94118, 0.96078, 1.00000)
    LAWN_GREEN = Color.new(0.48627, 0.98824, 0.00000, 1.00000)
    LEMON_CHIFFON = Color.new(1.00000, 0.98039, 0.80392, 1.00000)
    LIGHT_BLUE = Color.new(0.67843, 0.84706, 0.90196, 1.00000)
    LIGHT_CORAL = Color.new(0.94118, 0.50196, 0.50196, 1.00000)
    LIGHT_CYAN = Color.new(0.87843, 1.00000, 1.00000, 1.00000)
    LIGHT_GOLDENROD_YELLOW = Color.new(0.98039, 0.98039, 0.82353, 1.00000)
    LIGHT_GRAY = Color.new(0.82745, 0.82745, 0.82745, 1.00000)
    LIGHT_GREEN = Color.new(0.56471, 0.93333, 0.56471, 1.00000)
    LIGHT_PINK = Color.new(1.00000, 0.71373, 0.75686, 1.00000)
    LIGHT_SALMON = Color.new(1.00000, 0.62745, 0.47843, 1.00000)
    LIGHT_SEA_GREEN = Color.new(0.12549, 0.69804, 0.66667, 1.00000)
    LIGHT_SKY_BLUE = Color.new(0.52941, 0.80784, 0.98039, 1.00000)
    LIGHT_SLATE_GRAY = Color.new(0.46667, 0.53333, 0.60000, 1.00000)
    LIGHT_STEEL_BLUE = Color.new(0.69020, 0.76863, 0.87059, 1.00000)
    LIGHT_YELLOW = Color.new(1.00000, 1.00000, 0.87843, 1.00000)
    LIME = Color.new(0.00000, 1.00000, 0.00000, 1.00000)
    LIME_GREEN = Color.new(0.19608, 0.80392, 0.19608, 1.00000)
    LINEN = Color.new(0.98039, 0.94118, 0.90196, 1.00000)
    MAGENTA = Color.new(1.00000, 0.00000, 1.00000, 1.00000)
    MAROON = Color.new(0.50196, 0.00000, 0.00000, 1.00000)
    MEDIUM_AQUAMARINE = Color.new(0.40000, 0.80392, 0.66667, 1.00000)
    MEDIUM_BLUE = Color.new(0.00000, 0.00000, 0.80392, 1.00000)
    MEDIUM_ORCHID = Color.new(0.72941, 0.33333, 0.82745, 1.00000)
    MEDIUM_PURPLE = Color.new(0.57647, 0.43922, 0.85882, 1.00000)
    MEDIUM_SEA_GREEN = Color.new(0.23529, 0.70196, 0.44314, 1.00000)
    MEDIUM_SLATE_BLUE = Color.new(0.48235, 0.40784, 0.93333, 1.00000)
    MEDIUM_SPRING_GREEN = Color.new(0.00000, 0.98039, 0.60392, 1.00000)
    MEDIUM_TURQUOISE = Color.new(0.28235, 0.81961, 0.80000, 1.00000)
    MEDIUM_VIOLET_RED = Color.new(0.78039, 0.08235, 0.52157, 1.00000)
    MIDNIGHT_BLUE = Color.new(0.09804, 0.09804, 0.43922, 1.00000)
    MINT_CREAM = Color.new(0.96078, 1.00000, 0.98039, 1.00000)
    MISTY_ROSE = Color.new(1.00000, 0.89412, 0.88235, 1.00000)
    MOCCASIN = Color.new(1.00000, 0.89412, 0.70980, 1.00000)
    NAVAJO_WHITE = Color.new(1.00000, 0.87059, 0.67843, 1.00000)
    NAVY = Color.new(0.00000, 0.00000, 0.50196, 1.00000)
    OLD_LACE = Color.new(0.99216, 0.96078, 0.90196, 1.00000)
    OLIVE = Color.new(0.50196, 0.50196, 0.00000, 1.00000)
    OLIVE_DRAB = Color.new(0.41961, 0.55686, 0.13725, 1.00000)
    ORANGE = Color.new(1.00000, 0.64706, 0.00000, 1.00000)
    ORANGE_RED = Color.new(1.00000, 0.27059, 0.00000, 1.00000)
    ORCHID = Color.new(0.85490, 0.43922, 0.83922, 1.00000)
    PALE_GOLDENROD = Color.new(0.93333, 0.90980, 0.66667, 1.00000)
    PALE_GREEN = Color.new(0.59608, 0.98431, 0.59608, 1.00000)
    PALE_TURQUOISE = Color.new(0.68627, 0.93333, 0.93333, 1.00000)
    PALE_VIOLET_RED = Color.new(0.85882, 0.43922, 0.57647, 1.00000)
    PAPAYA_WHIP = Color.new(1.00000, 0.93725, 0.83529, 1.00000)
    PEACH_PUFF = Color.new(1.00000, 0.85490, 0.72549, 1.00000)
    PERU = Color.new(0.80392, 0.52157, 0.24706, 1.00000)
    PINK = Color.new(1.00000, 0.75294, 0.79608, 1.00000)
    PLUM = Color.new(0.86667, 0.62745, 0.86667, 1.00000)
    POWDER_BLUE = Color.new(0.69020, 0.87843, 0.90196, 1.00000)
    PURPLE = Color.new(0.50196, 0.00000, 0.50196, 1.00000)
    RED = Color.new(1.00000, 0.00000, 0.00000, 1.00000)
    ROSY_BROWN = Color.new(0.73725, 0.56078, 0.56078, 1.00000)
    ROYAL_BLUE = Color.new(0.25490, 0.41176, 0.88235, 1.00000)
    SADDLE_BROWN = Color.new(0.54510, 0.27059, 0.07451, 1.00000)
    SALMON = Color.new(0.98039, 0.50196, 0.44706, 1.00000)
    SANDY_BROWN = Color.new(0.95686, 0.64314, 0.37647, 1.00000)
    SEA_GREEN = Color.new(0.18039, 0.54510, 0.34118, 1.00000)
    SEA_SHELL = Color.new(1.00000, 0.96078, 0.93333, 1.00000)
    SIENNA = Color.new(0.62745, 0.32157, 0.17647, 1.00000)
    SILVER = Color.new(0.75294, 0.75294, 0.75294, 1.00000)
    SKY_BLUE = Color.new(0.52941, 0.80784, 0.92157, 1.00000)
    SLATE_BLUE = Color.new(0.41569, 0.35294, 0.80392, 1.00000)
    SLATE_GRAY = Color.new(0.43922, 0.50196, 0.56471, 1.00000)
    SNOW = Color.new(1.00000, 0.98039, 0.98039, 1.00000)
    SPRING_GREEN = Color.new(0.00000, 1.00000, 0.49804, 1.00000)
    STEEL_BLUE = Color.new(0.27451, 0.50980, 0.70588, 1.00000)
    TAN = Color.new(0.82353, 0.70588, 0.54902, 1.00000)
    TEAL = Color.new(0.00000, 0.50196, 0.50196, 1.00000)
    THISTLE = Color.new(0.84706, 0.74902, 0.84706, 1.00000)
    TOMATO = Color.new(1.00000, 0.38824, 0.27843, 1.00000)
    TURQUOISE = Color.new(0.25098, 0.87843, 0.81569, 1.00000)
    VIOLET = Color.new(0.93333, 0.50980, 0.93333, 1.00000)
    WHEAT = Color.new(0.96078, 0.87059, 0.70196, 1.00000)
    WHITE = Color.new(1.00000, 1.00000, 1.00000, 1.00000)
    WHITE_SMOKE = Color.new(0.96078, 0.96078, 0.96078, 1.00000)
    YELLOW = Color.new(1.00000, 1.00000, 0.00000, 1.00000)
    YELLOW_GREEN = Color.new(0.60392, 0.80392, 0.19608, 1.00000)
  end
end