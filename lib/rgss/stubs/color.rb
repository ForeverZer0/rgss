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
  #   properly implemented/overridden, but have been omitted from the documentation for the sake of brevity.
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
    #   #=> [0.54118, 0.16863, 0.88627, 1.00000]
    def to_a
    end

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

    ##
    # @return [Color] the preset color `#000000`.
    #
    # <div style="background-color:#000000;width:24px;height:24px;border:1px solid #000;"></div>
    NONE = Color.new(0.0, 0.0, 0.0, 0.0)

    ##
    # @return [Color] the preset color `#ffffff`.
    #
    # <div style="background-color:#ffffff;width:24px;height:24px;border:1px solid #000;"></div>
    TRANSPARENT = Color.new(1.0, 1.0, 1.0, 0.0)

    ##
    # @return [Color] the preset color `#f0f8ff`.
    #
    # <div style="background-color:#f0f8ff;width:24px;height:24px;border:1px solid #000;"></div>
    ALICE_BLUE = Color.new(0.94118, 0.97255, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#faebd7`.
    #
    # <div style="background-color:#faebd7 ;width:24px;height:24px;border:1px solid #000;"></div>
    ANTIQUE_WHITE = Color.new(0.98039, 0.92157, 0.84314, 1.0)

    ##
    # @return [Color] the preset color `#00ffff`.
    #
    # <div style="background-color:#00ffff ;width:24px;height:24px;border:1px solid #000;"></div>
    AQUA = Color.new(0.00000, 1.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#7fffd4`.
    #
    # <div style="background-color:#7fffd4 ;width:24px;height:24px;border:1px solid #000;"></div>
    AQUAMARINE = Color.new(0.49804, 1.00000, 0.83137, 1.0)

    ##
    # @return [Color] the preset color `#f0ffff`.
    #
    # <div style="background-color:#f0ffff ;width:24px;height:24px;border:1px solid #000;"></div>
    AZURE = Color.new(0.94118, 1.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#f5f5dc`.
    #
    # <div style="background-color:#f5f5dc ;width:24px;height:24px;border:1px solid #000;"></div>
    BEIGE = Color.new(0.96078, 0.96078, 0.86275, 1.0)

    ##
    # @return [Color] the preset color `#ffe4c4`.
    #
    # <div style="background-color:#ffe4c4 ;width:24px;height:24px;border:1px solid #000;"></div>
    BISQUE = Color.new(1.00000, 0.89412, 0.76863, 1.0)

    ##
    # @return [Color] the preset color `#000000`.
    #
    # <div style="background-color:#000000 ;width:24px;height:24px;border:1px solid #000;"></div>
    BLACK = Color.new(0.00000, 0.00000, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#ffebcd`.
    #
    # <div style="background-color:#ffebcd ;width:24px;height:24px;border:1px solid #000;"></div>
    BLANCHED_ALMOND = Color.new(1.00000, 0.92157, 0.80392, 1.0)

    ##
    # @return [Color] the preset color `#0000ff`.
    #
    # <div style="background-color:#0000ff ;width:24px;height:24px;border:1px solid #000;"></div>
    BLUE = Color.new(0.00000, 0.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#8a2be2`.
    #
    # <div style="background-color:#8a2be2 ;width:24px;height:24px;border:1px solid #000;"></div>
    BLUE_VIOLET = Color.new(0.54118, 0.16863, 0.88627, 1.0)

    ##
    # @return [Color] the preset color `#a52a2a`.
    #
    # <div style="background-color:#a52a2a ;width:24px;height:24px;border:1px solid #000;"></div>
    BROWN = Color.new(0.64706, 0.16471, 0.16471, 1.0)

    ##
    # @return [Color] the preset color `#deb887`.
    #
    # <div style="background-color:#deb887 ;width:24px;height:24px;border:1px solid #000;"></div>
    BURLY_WOOD = Color.new(0.87059, 0.72157, 0.52941, 1.0)

    ##
    # @return [Color] the preset color `#5f9ea0`.
    #
    # <div style="background-color:#5f9ea0 ;width:24px;height:24px;border:1px solid #000;"></div>
    CADET_BLUE = Color.new(0.37255, 0.61961, 0.62745, 1.0)

    ##
    # @return [Color] the preset color `#7fff00`.
    #
    # <div style="background-color:#7fff00 ;width:24px;height:24px;border:1px solid #000;"></div>
    CHARTREUSE = Color.new(0.49804, 1.00000, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#d2691e`.
    #
    # <div style="background-color:#d2691e ;width:24px;height:24px;border:1px solid #000;"></div>
    CHOCOLATE = Color.new(0.82353, 0.41176, 0.11765, 1.0)

    ##
    # @return [Color] the preset color `#ff7f50`.
    #
    # <div style="background-color:#ff7f50 ;width:24px;height:24px;border:1px solid #000;"></div>
    CORAL = Color.new(1.00000, 0.49804, 0.31373, 1.0)

    ##
    # @return [Color] the preset color `#6495ed`.
    #
    # <div style="background-color:#6495ed ;width:24px;height:24px;border:1px solid #000;"></div>
    CORNFLOWER_BLUE = Color.new(0.39216, 0.58431, 0.92941, 1.0)

    ##
    # @return [Color] the preset color `#fff8dc`.
    #
    # <div style="background-color:#fff8dc ;width:24px;height:24px;border:1px solid #000;"></div>
    CORNSILK = Color.new(1.00000, 0.97255, 0.86275, 1.0)

    ##
    # @return [Color] the preset color `#dc143c`.
    #
    # <div style="background-color:#dc143c ;width:24px;height:24px;border:1px solid #000;"></div>
    CRIMSON = Color.new(0.86275, 0.07843, 0.23529, 1.0)

    ##
    # @return [Color] the preset color `#00ffff`.
    #
    # <div style="background-color:#00ffff ;width:24px;height:24px;border:1px solid #000;"></div>
    CYAN = Color.new(0.00000, 1.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#00008b`.
    #
    # <div style="background-color:#00008b ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_BLUE = Color.new(0.00000, 0.00000, 0.54510, 1.0)

    ##
    # @return [Color] the preset color `#008b8b`.
    #
    # <div style="background-color:#008b8b ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_CYAN = Color.new(0.00000, 0.54510, 0.54510, 1.0)

    ##
    # @return [Color] the preset color `#b8860b`.
    #
    # <div style="background-color:#b8860b ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_GOLDENROD = Color.new(0.72157, 0.52549, 0.04314, 1.0)

    ##
    # @return [Color] the preset color `#a9a9a9`.
    #
    # <div style="background-color:#a9a9a9 ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_GRAY = Color.new(0.66275, 0.66275, 0.66275, 1.0)

    ##
    # @return [Color] the preset color `#006400`.
    #
    # <div style="background-color:#006400 ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_GREEN = Color.new(0.00000, 0.39216, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#bdb76b`.
    #
    # <div style="background-color:#bdb76b ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_KHAKI = Color.new(0.74118, 0.71765, 0.41961, 1.0)

    ##
    # @return [Color] the preset color `#8b008b`.
    #
    # <div style="background-color:#8b008b ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_MAGENTA = Color.new(0.54510, 0.00000, 0.54510, 1.0)

    ##
    # @return [Color] the preset color `#556b2f`.
    #
    # <div style="background-color:#556b2f ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_OLIVE_GREEN = Color.new(0.33333, 0.41961, 0.18431, 1.0)

    ##
    # @return [Color] the preset color `#ff8c00`.
    #
    # <div style="background-color:#ff8c00 ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_ORANGE = Color.new(1.00000, 0.54902, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#9932cc`.
    #
    # <div style="background-color:#9932cc ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_ORCHID = Color.new(0.60000, 0.19608, 0.80000, 1.0)

    ##
    # @return [Color] the preset color `#8b0000`.
    #
    # <div style="background-color:#8b0000 ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_RED = Color.new(0.54510, 0.00000, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#e9967a`.
    #
    # <div style="background-color:#e9967a ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_SALMON = Color.new(0.91373, 0.58824, 0.47843, 1.0)

    ##
    # @return [Color] the preset color `#8fbc8b`.
    #
    # <div style="background-color:#8fbc8b ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_SEA_GREEN = Color.new(0.56078, 0.73725, 0.54510, 1.0)

    ##
    # @return [Color] the preset color `#483d8b`.
    #
    # <div style="background-color:#483d8b ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_SLATE_BLUE = Color.new(0.28235, 0.23922, 0.54510, 1.0)

    ##
    # @return [Color] the preset color `#2f4f4f`.
    #
    # <div style="background-color:#2f4f4f ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_SLATE_GRAY = Color.new(0.18431, 0.30980, 0.30980, 1.0)

    ##
    # @return [Color] the preset color `#00ced1`.
    #
    # <div style="background-color:#00ced1 ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_TURQUOISE = Color.new(0.00000, 0.80784, 0.81961, 1.0)

    ##
    # @return [Color] the preset color `#9400d3`.
    #
    # <div style="background-color:#9400d3 ;width:24px;height:24px;border:1px solid #000;"></div>
    DARK_VIOLET = Color.new(0.58039, 0.00000, 0.82745, 1.0)

    ##
    # @return [Color] the preset color `#ff1493`.
    #
    # <div style="background-color:#ff1493 ;width:24px;height:24px;border:1px solid #000;"></div>
    DEEP_PINK = Color.new(1.00000, 0.07843, 0.57647, 1.0)

    ##
    # @return [Color] the preset color `#00bfff`.
    #
    # <div style="background-color:#00bfff ;width:24px;height:24px;border:1px solid #000;"></div>
    DEEP_SKY_BLUE = Color.new(0.00000, 0.74902, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#696969`.
    #
    # <div style="background-color:#696969 ;width:24px;height:24px;border:1px solid #000;"></div>
    DIM_GRAY = Color.new(0.41176, 0.41176, 0.41176, 1.0)

    ##
    # @return [Color] the preset color `#1e90ff`.
    #
    # <div style="background-color:#1e90ff ;width:24px;height:24px;border:1px solid #000;"></div>
    DODGER_BLUE = Color.new(0.11765, 0.56471, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#b22222`.
    #
    # <div style="background-color:#b22222 ;width:24px;height:24px;border:1px solid #000;"></div>
    FIREBRICK = Color.new(0.69804, 0.13333, 0.13333, 1.0)

    ##
    # @return [Color] the preset color `#fffaf0`.
    #
    # <div style="background-color:#fffaf0 ;width:24px;height:24px;border:1px solid #000;"></div>
    FLORAL_WHITE = Color.new(1.00000, 0.98039, 0.94118, 1.0)

    ##
    # @return [Color] the preset color `#228b22`.
    #
    # <div style="background-color:#228b22 ;width:24px;height:24px;border:1px solid #000;"></div>
    FOREST_GREEN = Color.new(0.13333, 0.54510, 0.13333, 1.0)

    ##
    # @return [Color] the preset color `#ff00ff`.
    #
    # <div style="background-color:#ff00ff ;width:24px;height:24px;border:1px solid #000;"></div>
    FUCHSIA = Color.new(1.00000, 0.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#dcdcdc`.
    #
    # <div style="background-color:#dcdcdc ;width:24px;height:24px;border:1px solid #000;"></div>
    GAINSBORO = Color.new(0.86275, 0.86275, 0.86275, 1.0)

    ##
    # @return [Color] the preset color `#f8f8ff`.
    #
    # <div style="background-color:#f8f8ff ;width:24px;height:24px;border:1px solid #000;"></div>
    GHOST_WHITE = Color.new(0.97255, 0.97255, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#ffd700`.
    #
    # <div style="background-color:#ffd700 ;width:24px;height:24px;border:1px solid #000;"></div>
    GOLD = Color.new(1.00000, 0.84314, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#daa520`.
    #
    # <div style="background-color:#daa520 ;width:24px;height:24px;border:1px solid #000;"></div>
    GOLDENROD = Color.new(0.85490, 0.64706, 0.12549, 1.0)

    ##
    # @return [Color] the preset color `#808080`.
    #
    # <div style="background-color:#808080 ;width:24px;height:24px;border:1px solid #000;"></div>
    GRAY = Color.new(0.50196, 0.50196, 0.50196, 1.0)

    ##
    # @return [Color] the preset color `#008000`.
    #
    # <div style="background-color:#008000 ;width:24px;height:24px;border:1px solid #000;"></div>
    GREEN = Color.new(0.00000, 0.50196, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#adff2f`.
    #
    # <div style="background-color:#adff2f ;width:24px;height:24px;border:1px solid #000;"></div>
    GREEN_YELLOW = Color.new(0.67843, 1.00000, 0.18431, 1.0)

    ##
    # @return [Color] the preset color `#f0fff0`.
    #
    # <div style="background-color:#f0fff0 ;width:24px;height:24px;border:1px solid #000;"></div>
    HONEYDEW = Color.new(0.94118, 1.00000, 0.94118, 1.0)

    ##
    # @return [Color] the preset color `#ff69b4`.
    #
    # <div style="background-color:#ff69b4 ;width:24px;height:24px;border:1px solid #000;"></div>
    HOT_PINK = Color.new(1.00000, 0.41176, 0.70588, 1.0)

    ##
    # @return [Color] the preset color `#cd5c5c`.
    #
    # <div style="background-color:#cd5c5c ;width:24px;height:24px;border:1px solid #000;"></div>
    INDIAN_RED = Color.new(0.80392, 0.36078, 0.36078, 1.0)

    ##
    # @return [Color] the preset color `#4b0082`.
    #
    # <div style="background-color:#4b0082 ;width:24px;height:24px;border:1px solid #000;"></div>
    INDIGO = Color.new(0.29412, 0.00000, 0.50980, 1.0)

    ##
    # @return [Color] the preset color `#fffff0`.
    #
    # <div style="background-color:#fffff0 ;width:24px;height:24px;border:1px solid #000;"></div>
    IVORY = Color.new(1.00000, 1.00000, 0.94118, 1.0)

    ##
    # @return [Color] the preset color `#f0e68c`.
    #
    # <div style="background-color:#f0e68c ;width:24px;height:24px;border:1px solid #000;"></div>
    KHAKI = Color.new(0.94118, 0.90196, 0.54902, 1.0)

    ##
    # @return [Color] the preset color `#e6e6fa`.
    #
    # <div style="background-color:#e6e6fa ;width:24px;height:24px;border:1px solid #000;"></div>
    LAVENDER = Color.new(0.90196, 0.90196, 0.98039, 1.0)

    ##
    # @return [Color] the preset color `#fff0f5`.
    #
    # <div style="background-color:#fff0f5 ;width:24px;height:24px;border:1px solid #000;"></div>
    LAVENDER_BLUSH = Color.new(1.00000, 0.94118, 0.96078, 1.0)

    ##
    # @return [Color] the preset color `#7cfc00`.
    #
    # <div style="background-color:#7cfc00 ;width:24px;height:24px;border:1px solid #000;"></div>
    LAWN_GREEN = Color.new(0.48627, 0.98824, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#fffacd`.
    #
    # <div style="background-color:#fffacd ;width:24px;height:24px;border:1px solid #000;"></div>
    LEMON_CHIFFON = Color.new(1.00000, 0.98039, 0.80392, 1.0)

    ##
    # @return [Color] the preset color `#add8e6`.
    #
    # <div style="background-color:#add8e6 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_BLUE = Color.new(0.67843, 0.84706, 0.90196, 1.0)

    ##
    # @return [Color] the preset color `#f08080`.
    #
    # <div style="background-color:#f08080 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_CORAL = Color.new(0.94118, 0.50196, 0.50196, 1.0)

    ##
    # @return [Color] the preset color `#e0ffff`.
    #
    # <div style="background-color:#e0ffff ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_CYAN = Color.new(0.87843, 1.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#fafad2`.
    #
    # <div style="background-color:#fafad2 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_GOLDENROD_YELLOW = Color.new(0.98039, 0.98039, 0.82353, 1.0)

    ##
    # @return [Color] the preset color `#d3d3d3`.
    #
    # <div style="background-color:#d3d3d3 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_GRAY = Color.new(0.82745, 0.82745, 0.82745, 1.0)

    ##
    # @return [Color] the preset color `#90ee90`.
    #
    # <div style="background-color:#90ee90 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_GREEN = Color.new(0.56471, 0.93333, 0.56471, 1.0)

    ##
    # @return [Color] the preset color `#ffb6c1`.
    #
    # <div style="background-color:#ffb6c1 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_PINK = Color.new(1.00000, 0.71373, 0.75686, 1.0)

    ##
    # @return [Color] the preset color `#ffa07a`.
    #
    # <div style="background-color:#ffa07a ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_SALMON = Color.new(1.00000, 0.62745, 0.47843, 1.0)

    ##
    # @return [Color] the preset color `#20b2aa`.
    #
    # <div style="background-color:#20b2aa ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_SEA_GREEN = Color.new(0.12549, 0.69804, 0.66667, 1.0)

    ##
    # @return [Color] the preset color `#87cefa`.
    #
    # <div style="background-color:#87cefa ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_SKY_BLUE = Color.new(0.52941, 0.80784, 0.98039, 1.0)

    ##
    # @return [Color] the preset color `#778899`.
    #
    # <div style="background-color:#778899 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_SLATE_GRAY = Color.new(0.46667, 0.53333, 0.60000, 1.0)

    ##
    # @return [Color] the preset color `#b0c4de`.
    #
    # <div style="background-color:#b0c4de ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_STEEL_BLUE = Color.new(0.69020, 0.76863, 0.87059, 1.0)

    ##
    # @return [Color] the preset color `#ffffe0`.
    #
    # <div style="background-color:#ffffe0 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIGHT_YELLOW = Color.new(1.00000, 1.00000, 0.87843, 1.0)

    ##
    # @return [Color] the preset color `#00ff00`.
    #
    # <div style="background-color:#00ff00 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIME = Color.new(0.00000, 1.00000, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#32cd32`.
    #
    # <div style="background-color:#32cd32 ;width:24px;height:24px;border:1px solid #000;"></div>
    LIME_GREEN = Color.new(0.19608, 0.80392, 0.19608, 1.0)

    ##
    # @return [Color] the preset color `#faf0e6`.
    #
    # <div style="background-color:#faf0e6 ;width:24px;height:24px;border:1px solid #000;"></div>
    LINEN = Color.new(0.98039, 0.94118, 0.90196, 1.0)

    ##
    # @return [Color] the preset color `#ff00ff`.
    #
    # <div style="background-color:#ff00ff ;width:24px;height:24px;border:1px solid #000;"></div>
    MAGENTA = Color.new(1.00000, 0.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#800000`.
    #
    # <div style="background-color:#800000 ;width:24px;height:24px;border:1px solid #000;"></div>
    MAROON = Color.new(0.50196, 0.00000, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#66cdaa`.
    #
    # <div style="background-color:#66cdaa ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_AQUAMARINE = Color.new(0.40000, 0.80392, 0.66667, 1.0)

    ##
    # @return [Color] the preset color `#0000cd`.
    #
    # <div style="background-color:#0000cd ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_BLUE = Color.new(0.00000, 0.00000, 0.80392, 1.0)

    ##
    # @return [Color] the preset color `#ba55d3`.
    #
    # <div style="background-color:#ba55d3 ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_ORCHID = Color.new(0.72941, 0.33333, 0.82745, 1.0)

    ##
    # @return [Color] the preset color `#9370db`.
    #
    # <div style="background-color:#9370db ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_PURPLE = Color.new(0.57647, 0.43922, 0.85882, 1.0)

    ##
    # @return [Color] the preset color `#3cb371`.
    #
    # <div style="background-color:#3cb371 ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_SEA_GREEN = Color.new(0.23529, 0.70196, 0.44314, 1.0)

    ##
    # @return [Color] the preset color `#7b68ee`.
    #
    # <div style="background-color:#7b68ee ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_SLATE_BLUE = Color.new(0.48235, 0.40784, 0.93333, 1.0)

    ##
    # @return [Color] the preset color `#00fa9a`.
    #
    # <div style="background-color:#00fa9a ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_SPRING_GREEN = Color.new(0.00000, 0.98039, 0.60392, 1.0)

    ##
    # @return [Color] the preset color `#48d1cc`.
    #
    # <div style="background-color:#48d1cc ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_TURQUOISE = Color.new(0.28235, 0.81961, 0.80000, 1.0)

    ##
    # @return [Color] the preset color `#c71585`.
    #
    # <div style="background-color:#c71585 ;width:24px;height:24px;border:1px solid #000;"></div>
    MEDIUM_VIOLET_RED = Color.new(0.78039, 0.08235, 0.52157, 1.0)

    ##
    # @return [Color] the preset color `#191970`.
    #
    # <div style="background-color:#191970 ;width:24px;height:24px;border:1px solid #000;"></div>
    MIDNIGHT_BLUE = Color.new(0.09804, 0.09804, 0.43922, 1.0)

    ##
    # @return [Color] the preset color `#f5fffa`.
    #
    # <div style="background-color:#f5fffa ;width:24px;height:24px;border:1px solid #000;"></div>
    MINT_CREAM = Color.new(0.96078, 1.00000, 0.98039, 1.0)

    ##
    # @return [Color] the preset color `#ffe4e1`.
    #
    # <div style="background-color:#ffe4e1 ;width:24px;height:24px;border:1px solid #000;"></div>
    MISTY_ROSE = Color.new(1.00000, 0.89412, 0.88235, 1.0)

    ##
    # @return [Color] the preset color `#ffe4b5`.
    #
    # <div style="background-color:#ffe4b5 ;width:24px;height:24px;border:1px solid #000;"></div>
    MOCCASIN = Color.new(1.00000, 0.89412, 0.70980, 1.0)

    ##
    # @return [Color] the preset color `#ffdead`.
    #
    # <div style="background-color:#ffdead ;width:24px;height:24px;border:1px solid #000;"></div>
    NAVAJO_WHITE = Color.new(1.00000, 0.87059, 0.67843, 1.0)

    ##
    # @return [Color] the preset color `#000080`.
    #
    # <div style="background-color:#000080 ;width:24px;height:24px;border:1px solid #000;"></div>
    NAVY = Color.new(0.00000, 0.00000, 0.50196, 1.0)

    ##
    # @return [Color] the preset color `#fdf5e6`.
    #
    # <div style="background-color:#fdf5e6 ;width:24px;height:24px;border:1px solid #000;"></div>
    OLD_LACE = Color.new(0.99216, 0.96078, 0.90196, 1.0)

    ##
    # @return [Color] the preset color `#808000`.
    #
    # <div style="background-color:#808000 ;width:24px;height:24px;border:1px solid #000;"></div>
    OLIVE = Color.new(0.50196, 0.50196, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#6b8e23`.
    #
    # <div style="background-color:#6b8e23 ;width:24px;height:24px;border:1px solid #000;"></div>
    OLIVE_DRAB = Color.new(0.41961, 0.55686, 0.13725, 1.0)

    ##
    # @return [Color] the preset color `#ffa500`.
    #
    # <div style="background-color:#ffa500 ;width:24px;height:24px;border:1px solid #000;"></div>
    ORANGE = Color.new(1.00000, 0.64706, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#ff4500`.
    #
    # <div style="background-color:#ff4500 ;width:24px;height:24px;border:1px solid #000;"></div>
    ORANGE_RED = Color.new(1.00000, 0.27059, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#da70d6`.
    #
    # <div style="background-color:#da70d6 ;width:24px;height:24px;border:1px solid #000;"></div>
    ORCHID = Color.new(0.85490, 0.43922, 0.83922, 1.0)

    ##
    # @return [Color] the preset color `#eee8aa`.
    #
    # <div style="background-color:#eee8aa ;width:24px;height:24px;border:1px solid #000;"></div>
    PALE_GOLDENROD = Color.new(0.93333, 0.90980, 0.66667, 1.0)

    ##
    # @return [Color] the preset color `#98fb98`.
    #
    # <div style="background-color:#98fb98 ;width:24px;height:24px;border:1px solid #000;"></div>
    PALE_GREEN = Color.new(0.59608, 0.98431, 0.59608, 1.0)

    ##
    # @return [Color] the preset color `#afeeee`.
    #
    # <div style="background-color:#afeeee ;width:24px;height:24px;border:1px solid #000;"></div>
    PALE_TURQUOISE = Color.new(0.68627, 0.93333, 0.93333, 1.0)

    ##
    # @return [Color] the preset color `#db7093`.
    #
    # <div style="background-color:#db7093 ;width:24px;height:24px;border:1px solid #000;"></div>
    PALE_VIOLET_RED = Color.new(0.85882, 0.43922, 0.57647, 1.0)

    ##
    # @return [Color] the preset color `#ffefd5`.
    #
    # <div style="background-color:#ffefd5 ;width:24px;height:24px;border:1px solid #000;"></div>
    PAPAYA_WHIP = Color.new(1.00000, 0.93725, 0.83529, 1.0)

    ##
    # @return [Color] the preset color `#ffdab9`.
    #
    # <div style="background-color:#ffdab9 ;width:24px;height:24px;border:1px solid #000;"></div>
    PEACH_PUFF = Color.new(1.00000, 0.85490, 0.72549, 1.0)

    ##
    # @return [Color] the preset color `#cd853f`.
    #
    # <div style="background-color:#cd853f ;width:24px;height:24px;border:1px solid #000;"></div>
    PERU = Color.new(0.80392, 0.52157, 0.24706, 1.0)

    ##
    # @return [Color] the preset color `#ffc0cb`.
    #
    # <div style="background-color:#ffc0cb ;width:24px;height:24px;border:1px solid #000;"></div>
    PINK = Color.new(1.00000, 0.75294, 0.79608, 1.0)

    ##
    # @return [Color] the preset color `#dda0dd`.
    #
    # <div style="background-color:#dda0dd ;width:24px;height:24px;border:1px solid #000;"></div>
    PLUM = Color.new(0.86667, 0.62745, 0.86667, 1.0)

    ##
    # @return [Color] the preset color `#b0e0e6`.
    #
    # <div style="background-color:#b0e0e6 ;width:24px;height:24px;border:1px solid #000;"></div>
    POWDER_BLUE = Color.new(0.69020, 0.87843, 0.90196, 1.0)

    ##
    # @return [Color] the preset color `#800080`.
    #
    # <div style="background-color:#800080 ;width:24px;height:24px;border:1px solid #000;"></div>
    PURPLE = Color.new(0.50196, 0.00000, 0.50196, 1.0)

    ##
    # @return [Color] the preset color `#ff0000`.
    #
    # <div style="background-color:#ff0000 ;width:24px;height:24px;border:1px solid #000;"></div>
    RED = Color.new(1.00000, 0.00000, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#bc8f8f`.
    #
    # <div style="background-color:#bc8f8f ;width:24px;height:24px;border:1px solid #000;"></div>
    ROSY_BROWN = Color.new(0.73725, 0.56078, 0.56078, 1.0)

    ##
    # @return [Color] the preset color `#4169e1`.
    #
    # <div style="background-color:#4169e1 ;width:24px;height:24px;border:1px solid #000;"></div>
    ROYAL_BLUE = Color.new(0.25490, 0.41176, 0.88235, 1.0)

    ##
    # @return [Color] the preset color `#8b4513`.
    #
    # <div style="background-color:#8b4513 ;width:24px;height:24px;border:1px solid #000;"></div>
    SADDLE_BROWN = Color.new(0.54510, 0.27059, 0.07451, 1.0)

    ##
    # @return [Color] the preset color `#fa8072`.
    #
    # <div style="background-color:#fa8072 ;width:24px;height:24px;border:1px solid #000;"></div>
    SALMON = Color.new(0.98039, 0.50196, 0.44706, 1.0)

    ##
    # @return [Color] the preset color `#f4a460`.
    #
    # <div style="background-color:#f4a460 ;width:24px;height:24px;border:1px solid #000;"></div>
    SANDY_BROWN = Color.new(0.95686, 0.64314, 0.37647, 1.0)

    ##
    # @return [Color] the preset color `#2e8b57`.
    #
    # <div style="background-color:#2e8b57 ;width:24px;height:24px;border:1px solid #000;"></div>
    SEA_GREEN = Color.new(0.18039, 0.54510, 0.34118, 1.0)

    ##
    # @return [Color] the preset color `#fff5ee`.
    #
    # <div style="background-color:#fff5ee ;width:24px;height:24px;border:1px solid #000;"></div>
    SEA_SHELL = Color.new(1.00000, 0.96078, 0.93333, 1.0)

    ##
    # @return [Color] the preset color `#a0522d`.
    #
    # <div style="background-color:#a0522d ;width:24px;height:24px;border:1px solid #000;"></div>
    SIENNA = Color.new(0.62745, 0.32157, 0.17647, 1.0)

    ##
    # @return [Color] the preset color `#c0c0c0`.
    #
    # <div style="background-color:#c0c0c0 ;width:24px;height:24px;border:1px solid #000;"></div>
    SILVER = Color.new(0.75294, 0.75294, 0.75294, 1.0)

    ##
    # @return [Color] the preset color `#87ceeb`.
    #
    # <div style="background-color:#87ceeb ;width:24px;height:24px;border:1px solid #000;"></div>
    SKY_BLUE = Color.new(0.52941, 0.80784, 0.92157, 1.0)

    ##
    # @return [Color] the preset color `#6a5acd`.
    #
    # <div style="background-color:#6a5acd ;width:24px;height:24px;border:1px solid #000;"></div>
    SLATE_BLUE = Color.new(0.41569, 0.35294, 0.80392, 1.0)

    ##
    # @return [Color] the preset color `#708090`.
    #
    # <div style="background-color:#708090 ;width:24px;height:24px;border:1px solid #000;"></div>
    SLATE_GRAY = Color.new(0.43922, 0.50196, 0.56471, 1.0)

    ##
    # @return [Color] the preset color `#fffafa`.
    #
    # <div style="background-color:#fffafa ;width:24px;height:24px;border:1px solid #000;"></div>
    SNOW = Color.new(1.00000, 0.98039, 0.98039, 1.0)

    ##
    # @return [Color] the preset color `#00ff7f`.
    #
    # <div style="background-color:#00ff7f ;width:24px;height:24px;border:1px solid #000;"></div>
    SPRING_GREEN = Color.new(0.00000, 1.00000, 0.49804, 1.0)

    ##
    # @return [Color] the preset color `#4682b4`.
    #
    # <div style="background-color:#4682b4 ;width:24px;height:24px;border:1px solid #000;"></div>
    STEEL_BLUE = Color.new(0.27451, 0.50980, 0.70588, 1.0)

    ##
    # @return [Color] the preset color `#d2b48c`.
    #
    # <div style="background-color:#d2b48c ;width:24px;height:24px;border:1px solid #000;"></div>
    TAN = Color.new(0.82353, 0.70588, 0.54902, 1.0)

    ##
    # @return [Color] the preset color `#008080`.
    #
    # <div style="background-color:#008080 ;width:24px;height:24px;border:1px solid #000;"></div>
    TEAL = Color.new(0.00000, 0.50196, 0.50196, 1.0)

    ##
    # @return [Color] the preset color `#d8bfd8`.
    #
    # <div style="background-color:#d8bfd8 ;width:24px;height:24px;border:1px solid #000;"></div>
    THISTLE = Color.new(0.84706, 0.74902, 0.84706, 1.0)

    ##
    # @return [Color] the preset color `#ff6347`.
    #
    # <div style="background-color:#ff6347 ;width:24px;height:24px;border:1px solid #000;"></div>
    TOMATO = Color.new(1.00000, 0.38824, 0.27843, 1.0)

    ##
    # @return [Color] the preset color `#40e0d0`.
    #
    # <div style="background-color:#40e0d0 ;width:24px;height:24px;border:1px solid #000;"></div>
    TURQUOISE = Color.new(0.25098, 0.87843, 0.81569, 1.0)

    ##
    # @return [Color] the preset color `#ee82ee`.
    #
    # <div style="background-color:#ee82ee ;width:24px;height:24px;border:1px solid #000;"></div>
    VIOLET = Color.new(0.93333, 0.50980, 0.93333, 1.0)

    ##
    # @return [Color] the preset color `#f5deb3`.
    #
    # <div style="background-color:#f5deb3 ;width:24px;height:24px;border:1px solid #000;"></div>
    WHEAT = Color.new(0.96078, 0.87059, 0.70196, 1.0)

    ##
    # @return [Color] the preset color `#ffffff`.
    #
    # <div style="background-color:#ffffff ;width:24px;height:24px;border:1px solid #000;"></div>
    WHITE = Color.new(1.00000, 1.00000, 1.00000, 1.0)

    ##
    # @return [Color] the preset color `#f5f5f5`.
    #
    # <div style="background-color:#f5f5f5 ;width:24px;height:24px;border:1px solid #000;"></div>
    WHITE_SMOKE = Color.new(0.96078, 0.96078, 0.96078, 1.0)

    ##
    # @return [Color] the preset color `#ffff00`.
    #
    # <div style="background-color:#ffff00 ;width:24px;height:24px;border:1px solid #000;"></div>
    YELLOW = Color.new(1.00000, 1.00000, 0.00000, 1.0)

    ##
    # @return [Color] the preset color `#9acd32`.
    #
    # <div style="background-color:#9acd32 ;width:24px;height:24px;border:1px solid #000;"></div>
    YELLOW_GREEN = Color.new(0.60392, 0.80392, 0.19608, 1.0)
  end
end