
module RGSS


  ##
  # @abstract
  class IVec2

    ##
    # Creates a new instance of the {IVec2} class.
    # @overload initialize
    #
    # @overload initialize(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    # @overload initialize(other)
    #   @param other [IVec2]
    def initialize(*args)
    end

    def [](index)
    end

    def []=(index, value)
    end

    ##
    # Creates an array instance that represents this object.
    # @return [Array(Integer, Integer)] The newly created Array.
    def to_a
    end

    ##
    # Creates a hash instance that represents this object.
    # @return [Hash{Symbol => Integer}] The newly created Hash.
    def to_h
    end

    def +(other)
    end

    def -(other)
    end

    def set(x, y)
    end

    def empty?
    end

    alias_method :add, :+
    alias_method :subtract, :-
  end


  class Point < IVec2

    ##
    # @return [Integer]
    attr_accessor :x

    ##
    # @return [Integer]
    attr_accessor :y
  end

  class Size < IVec2

    ##
    # @return [Integer]
    attr_accessor :width

    ##
    # @return [Integer]
    attr_accessor :height

    ##
    #
    # @overload offset(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    #
    # @overload offset(other)
    #   @param other [IVec2]
    #
    # @return [Point]
    def offset
    end

    ##
    #
    # @overload offset!(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    #
    # @overload offset!(other)
    #   @param other [IVec2]
    #
    # @return [self]
    def offset!
    end
  end

  ##
  # Stores a set of four integers that represent the location and size of a rectangle, with the top-left at 0, 0.
  #
  # @note All relevant methods implemented in `Object` (i.e. `#dup`, `#clone`, Marshalling, equality) have also been
  #   properly implemented/overridden, but have been omitted from the documentation for brevity.
  class Rect

    ##
    # @return [Integer] the location of the top-left corner on the x-axis.
    attr_accessor :x

    ##
    # @return [Integer] the location of the top-left corner on the y-axis.
    attr_accessor :y

    ##
    # @return [Integer] the size of the rectangle on the x-axis.
    attr_accessor :width

    ##
    # @return [Integer] the size of the rectangle on the y-axis.
    attr_accessor :height

    ##
    # @return [Point] the location of the top-left corner of the rectangle.
    attr_accessor :location

    ##
    # @return [Size] the size (width and height) of the rectangle.
    attr_accessor :size

    ##
    # @return [Integer] the location of the left edge of the rectangle on the x-axis.
    attr_reader :left

    ##
    # @return [Integer] the location of the top edge of the rectangle on the y-axis.
    attr_reader :top

    ##
    # @return [Integer] the location of the right edge of the rectangle on the x-axis.
    attr_reader :right

    ##
    # @return [Integer] the location of the bottom edge of the rectangle on the y-axis.
    attr_reader :bottom

    ##
    # Creates and returns a new {Rect} with all values defaulted to `0`.
    #
    # @return [Rect] The newly created rectangle.
    def self.empty
    end

    ##
    # Creates a new instance of the {Rect} class.
    #
    # @overload initialize
    #   Creates a new empty rectangle.
    #
    # @overload initialize(location, size)
    #   Creates a new rectangle with the specified location and size.
    #   @param location [Point]
    #   @param size [Size]
    #
    # @overload initialize(x, y, width, height)
    #   Creates a new rectangle with the specified dimensions.
    #   @param x [Numeric]
    #   @param y [Numeric]
    #   @param width [Numeric]
    #   @param height [Numeric]
    def initialize(*args)
    end

    ##
    # Creates a new {Rect} by the specified edges.
    #
    # @param left [Numeric] the location of the rectangles left edge on the x-axis.
    # @param top [Numeric] the location of the rectangles top edge on the y-axis.
    # @param right [Numeric] the location of the rectangles right edge on the x-axis.
    # @param bottom [Numeric] the location of the rectangles bottom edge on the y-axis.
    #
    # @return [Rect] the newly created {Rect} instance.
    def self.ltrb(left, right, top, bottom)
    end

    ##
    # Provides access to the rectangle components by index.
    # @param index [Numeric] the index of the component (x, y, width, height) to retrieve.
    # @return [Integer] the value of the specified component.
    # @raise [IndexError] if index is less than `0` or greater than `3`.
    def [](index)
    end

    ##
    # Provides access to the rectangle components by index.
    # @param index [Numeric] the index of the component (x, y, width, height) to set.
    # @return [Integer] the value that was set.
    # @raise [IndexError] if index is less than `0` or greater than `3`.
    def []=(index, value)
    end

    def set(x, y, width, height)
    end

    ##
    #
    # @return [Boolean]
    def empty?
    end

    ##
    # @overload contains?(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    #
    # @overload contains?(point)
    #   @param point [Point]
    #
    # @overload contains?(rect)
    #   @param rect [Rect]
    #
    # @return [Boolean]
    def contains?(*args)
    end

    ##
    # @overload inflate(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    #
    # @overload inflate(amount)
    #   @param amount [IVec2]
    #
    # @return [Rect]
    def inflate(*args)
    end

    ##
    # @overload inflate!(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    #
    # @overload inflate!(amount)
    #   @param amount [IVec2]
    #
    # @return [self]
    def inflate!(*args)
    end

    ##
    #
    # @param other [Rect]
    #
    # @return [Rect]
    def intersect(other)
    end

    ##
    #
    # @param other [Rect]
    #
    # @return [self]
    def intersect!(other)
    end

    ##
    # @param other [Rect]
    #
    # @return [Boolean]
    def intersects?(other)
    end

    ##
    # @overload offset(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    #
    # @overload offset(amount)
    #   @param amount [IVec2]
    #
    # @return [Rect]
    def offset(*args)
    end

    ##
    # @overload offset(x, y)
    #   @param x [Numeric]
    #   @param y [Numeric]
    #
    # @overload offset(amount)
    #   @param amount [IVec2]
    #
    # @return [self]
    def offset!(*args)
    end

    ##
    # Creates an array instance that represents this object.
    # @return [Array(Integer, Integer, Integer, Integer)] The newly created Array.
    def to_a
    end

    ##
    # Creates a hash instance that represents this object.
    # @return [Hash{Symbol => Integer}] The newly created Hash.
    def to_h
    end

    alias_method :include?, :contains?
  end






end



